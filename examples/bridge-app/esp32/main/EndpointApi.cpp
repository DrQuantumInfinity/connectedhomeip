/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "EndpointApi.h"
#include "Device.h"
#include "DeviceCallbacks.h"
#include "esp_log.h"
// #include "nvs_flash.h"
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
// #include <app/clusters/identify-server/identify-server.h>
#include <app/reporting/reporting.h>
// #include <app/server/OnboardingCodesUtil.h>
#include <app/util/attribute-storage.h>
#include <common/Esp32AppServer.h>
// #include <credentials/DeviceAttestationCredsProvider.h>
// #include <credentials/examples/DeviceAttestationCredsExample.h>
#include <lib/core/CHIPError.h>
// #include <lib/support/CHIPMem.h>
// #include <lib/support/CHIPMemString.h>
#include <lib/support/ZclString.h>
// #include <platform/ESP32/ESP32Utils.h>

#include <app/InteractionModelEngine.h>
#include <app/server/Server.h>

#if CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER
#include <platform/ESP32/ESP32FactoryDataProvider.h>
#endif // CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER

#if CONFIG_ENABLE_ESP32_DEVICE_INFO_PROVIDER
#include <platform/ESP32/ESP32DeviceInfoProvider.h>
#else
#include <DeviceInfoProviderImpl.h>
#endif // CONFIG_ENABLE_ESP32_DEVICE_INFO_PROVIDER

namespace {
#if CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER
chip::DeviceLayer::ESP32FactoryDataProvider sFactoryDataProvider;
#endif // CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER

#if CONFIG_ENABLE_ESP32_DEVICE_INFO_PROVIDER
chip::DeviceLayer::ESP32DeviceInfoProvider gExampleDeviceInfoProvider;
#else
chip::DeviceLayer::DeviceInfoProviderImpl gExampleDeviceInfoProvider;
#endif // CONFIG_ENABLE_ESP32_DEVICE_INFO_PROVIDER
} // namespace

/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define ZCL_BRIDGED_DEVICE_BASIC_INFORMATION_CLUSTER_REVISION   (2u)
#define ZCL_ON_OFF_CLUSTER_REVISION                             (4u)

static const char * TAG = "endpoint-api";

// (taken from chip-devices.xml)
#define DEVICE_TYPE_ROOT_NODE 0x0016
// (taken from chip-devices.xml)
#define DEVICE_TYPE_BRIDGE 0x000e
// Device Version for dynamic endpoints:
#define DEVICE_VERSION_DEFAULT 1
const EmberAfDeviceType gRootDeviceTypes[]          = { { DEVICE_TYPE_ROOT_NODE, DEVICE_VERSION_DEFAULT } };
const EmberAfDeviceType gAggregateNodeDeviceTypes[] = { { DEVICE_TYPE_BRIDGE, DEVICE_VERSION_DEFAULT } };

/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/
typedef struct
{
    uint16_t index;
    GOOGLE_READ_CALLBACK pfnReadCallback;
    GOOGLE_WRITE_CALLBACK pfnWriteCallback;
//  GOOGLE_INSTANT_ACTION_CALLBACK pfnInstantActionCallback;
}ENDPOINT;


typedef struct
{
    EndpointId currentEndpointId;
    EndpointId firstDynamicEndpointId;
    ENDPOINT endpoint[CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT];
}ENDPOINT_API;

/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
//workers
static void EndpointAddWorker(intptr_t context);
static void EndpointRemoveWorker(intptr_t context);
static void EndpointReportUpdateWorker(intptr_t closure);

//Read callback
static EmberAfStatus HandleReadBridgedDeviceBasicAttribute(Device* dev, chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength);
static EmberAfStatus HandleReadOnOffAttribute(Device* dev, chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength);
//Write callback
static EmberAfStatus HandleWriteOnOffAttribute(Device* dev, chip::AttributeId attributeId, uint8_t* buffer);
/**************************************************************************
 *                                  Variables
 **************************************************************************/
static ENDPOINT_API endpointApi;
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
void EndpointApiInit(void/*Maybe inject some callbacks from the application*/)
{
    ESP_LOGI(TAG, "Init");
    memset(&endpointApi, 0x00, sizeof(endpointApi));

    //Gordon: Move these to the application. This API will be "dumb".
    // Set starting endpoint id where dynamic endpoints will be assigned, which
    // will be the next consecutive endpoint id after the last fixed endpoint.
    endpointApi.firstDynamicEndpointId = static_cast<chip::EndpointId>(static_cast<int>(emberAfEndpointFromIndex(static_cast<uint16_t>(emberAfFixedEndpointCount() - 1))) + 1);
    endpointApi.currentEndpointId = endpointApi.firstDynamicEndpointId;

    // Disable last fixed endpoint, which is used as a placeholder for all of the
    // supported clusters so that ZAP will generated the requisite code.
    emberAfEndpointEnableDisable(emberAfEndpointFromIndex(static_cast<uint16_t>(emberAfFixedEndpointCount() - 1)), false);

    // A bridge has root node device type on EP0 and aggregate node device type (bridge) at EP1
    emberAfSetDeviceTypeList(0, Span<const EmberAfDeviceType>(gRootDeviceTypes));
    emberAfSetDeviceTypeList(1, Span<const EmberAfDeviceType>(gAggregateNodeDeviceTypes));
}
void EndpointAdd(ENDPOINT_DATA *pData)
{
    chip::DeviceLayer::PlatformMgr().ScheduleWork(EndpointAddWorker, reinterpret_cast<intptr_t>(pData));
}
void EndpointRemove(uint16_t index)
{
    chip::DeviceLayer::PlatformMgr().ScheduleWork(EndpointRemoveWorker, reinterpret_cast<intptr_t>((void*)(uint32_t)index));
}
void EndpointReportChange(uint16_t index, ClusterId cluster, AttributeId attribute)
{
    auto * path = Platform::New<app::ConcreteAttributePath>(emberAfEndpointFromIndex(index), cluster, attribute);
    DeviceLayer::PlatformMgr().ScheduleWork(EndpointReportUpdateWorker, reinterpret_cast<intptr_t>(path));
}
/**************************************************************************
 *                                  Magic Callback Functions
 **************************************************************************/
EmberAfStatus emberAfExternalAttributeReadCallback(
    EndpointId endpoint, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer, uint16_t maxReadLength)
{
    uint16_t index = emberAfGetDynamicIndexFromEndpoint(endpoint);
/*
    //TODO: consider calling up to the application with a callback here.

    if (index < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    {
        Device * dev = gDevices[index];

        if (clusterId == BridgedDeviceBasicInformation::Id)
        {
            return HandleReadBridgedDeviceBasicAttribute(dev, attributeMetadata->attributeId, buffer, maxReadLength);
        }
        else if (clusterId == OnOff::Id)
        {
            return HandleReadOnOffAttribute(dev, attributeMetadata->attributeId, buffer, maxReadLength);
        }
    }*/

    return EMBER_ZCL_STATUS_FAILURE;
}
EmberAfStatus emberAfExternalAttributeWriteCallback(
    EndpointId endpoint, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer)
{
    uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(endpoint);
/*
    if (endpointIndex < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    {
        Device * dev = gDevices[endpointIndex];

        if ((dev->IsReachable()) && (clusterId == OnOff::Id))
        {
            return HandleWriteOnOffAttribute(dev, attributeMetadata->attributeId, buffer);
        }
    }
*/
    return EMBER_ZCL_STATUS_FAILURE;
}
//Another magic callback. I think we want to use this. Review the path below for examples.
//"/root/esp-matter/connectedhomeip/connectedhomeip/examples/bridge-app/linux/main.cpp"
bool emberAfActionsClusterInstantActionCallback(
    app::CommandHandler* commandObj, const app::ConcreteCommandPath & commandPath, 
    const Actions::Commands::InstantAction::DecodableType & commandData)
{
    // No actions are implemented, just return status NotFound.
    commandObj->AddStatus(commandPath, Protocols::InteractionModel::Status::NotFound);
    return true;
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
static void EndpointAddWorker(intptr_t context)
{
    ENDPOINT_DATA *pData = reinterpret_cast<ENDPOINT_DATA *>(context);
    ESP_LOGI(TAG, "Adding device %u: %s", pData->index, pData->name);
    
    if (pData->index < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    {
        while (true)
        {
            //TODO: Is it necessary to do this before adding the endpoint?
            //TODO: does the device need to know its endpoint?
            pData->dev->SetEndpointId(endpointApi.currentEndpointId); 
            EmberAfStatus ret = emberAfSetDynamicEndpoint(pData->index, endpointApi.currentEndpointId, pData->ep, pData->dataVersionStorage, pData->deviceTypeList, pData->parentEndpointId);
            if (ret == EMBER_ZCL_STATUS_SUCCESS)
            {
                ChipLogProgress(DeviceLayer, "Added device %u: %s at dynamic endpoint %u", pData->index, pData->name, endpointApi.currentEndpointId);
                return;
            }
            else if (ret != EMBER_ZCL_STATUS_DUPLICATE_EXISTS)
            {
                return;
            }
            // Handle wrap condition
            if (++endpointApi.currentEndpointId < endpointApi.firstDynamicEndpointId)
            {
                endpointApi.currentEndpointId = endpointApi.firstDynamicEndpointId;
            }
        }
    }
    ChipLogProgress(DeviceLayer, "Failed to add dynamic endpoint: No endpoints available!");
}
static void EndpointRemoveWorker(intptr_t context)
{
    uint16_t index = (uint16_t)(uint32_t)reinterpret_cast<void*>(context);
    EndpointId ep = emberAfClearDynamicEndpoint(index);
    ChipLogProgress(DeviceLayer, "Removed device %u from dynamic endpoint %d", index, ep);

    // Silence complaints about unused ep when progress logging is disabled.
    UNUSED_VAR(ep);
}
static void EndpointReportUpdateWorker(intptr_t closure)
{
    auto path = reinterpret_cast<app::ConcreteAttributePath *>(closure);
    MatterReportingAttributeChangeCallback(*path);
    Platform::Delete(path);
}