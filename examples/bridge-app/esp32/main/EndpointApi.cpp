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
#include <app/util/attribute-storage.h>
#include <app/reporting/reporting.h> //for MatterReportingAttributeChangeCallback()

/**************************************************************************
 *                                  Constants
 **************************************************************************/
static const char * TAG = "endpoint-api";
// (taken from chip-devices.xml)
#define DEVICE_TYPE_ROOT_NODE 0x0016
// (taken from chip-devices.xml)
#define DEVICE_TYPE_BRIDGE 0x000e
// Device Version for dynamic endpoints:
#define DEVICE_VERSION_DEFAULT 1
const EmberAfDeviceType rootDeviceTypes[]          = { { DEVICE_TYPE_ROOT_NODE, DEVICE_VERSION_DEFAULT } };
const EmberAfDeviceType aggregateNodeDeviceTypes[] = { { DEVICE_TYPE_BRIDGE, DEVICE_VERSION_DEFAULT } };

/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/
typedef struct
{
    uint16_t index;
    void *pObject;
    GOOGLE_READ_CALLBACK pfnReadCallback;
    GOOGLE_WRITE_CALLBACK pfnWriteCallback;
    GOOGLE_INSTANT_ACTION_CALLBACK pfnInstantActionCallback;
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
static void EndpointApiInitWorker(intptr_t context);
static void EndpointAddWorker(intptr_t context);
static void EndpointRemoveWorker(intptr_t context);
static void EndpointReportUpdateWorker(intptr_t closure);
/**************************************************************************
 *                                  Variables
 **************************************************************************/
static ENDPOINT_API endpointApi;
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
void EndpointApiInit(void)
{
    chip::DeviceLayer::PlatformMgr().ScheduleWork(EndpointApiInitWorker);
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
    EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;

    uint16_t index = emberAfGetDynamicIndexFromEndpoint(endpoint);
    ESP_LOGI(TAG, "Read callback for index %u, cluster %04lX, attr %04lX", index, clusterId, attributeMetadata->attributeId);

    if (index < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    {
        if (endpointApi.endpoint[index].pfnReadCallback)
        {
            status = endpointApi.endpoint[index].pfnReadCallback(endpointApi.endpoint[index].pObject, clusterId, attributeMetadata, buffer, maxReadLength);
        }
    }
    else
    {
        ESP_LOGE(TAG, "Read invalid index %u, %u", index, endpoint);
    }
    return status;
}
EmberAfStatus emberAfExternalAttributeWriteCallback(
    EndpointId endpoint, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer)
{
    EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;
    
    uint16_t index = emberAfGetDynamicIndexFromEndpoint(endpoint);
    ESP_LOGI(TAG, "Write callback for index %u, cluster %04lX, attr %04lX", index, clusterId, attributeMetadata->attributeId);

    if (index < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    {
        if (endpointApi.endpoint[index].pfnWriteCallback)
        {
            status = endpointApi.endpoint[index].pfnWriteCallback(endpointApi.endpoint[index].pObject, clusterId, attributeMetadata, buffer);
        }
    }
    else
    {
        ESP_LOGE(TAG, "Write invalid index %u, %u", index, endpoint);
    }
    return status;
}
//Another magic callback. I think we want to use this. Review the path below for examples.
//"/root/esp-matter/connectedhomeip/connectedhomeip/examples/bridge-app/linux/main.cpp"
bool emberAfActionsClusterInstantActionCallback(
    app::CommandHandler* commandObj, const app::ConcreteCommandPath & commandPath, 
    const Actions::Commands::InstantAction::DecodableType & commandData)
{
    bool retVal = true;
    
    //TODO: how do I know what index received an instant action?
/*  if (endpointApi.endpoint[index].pfnInstantActionCallback)
    {
        retVal = endpointApi.endpoint[index].pfnInstantActionCallback(commandObj, commandPath, commandData);
    }
    else*/
    {
        // No actions are implemented, just return status NotFound.
        commandObj->AddStatus(commandPath, Protocols::InteractionModel::Status::NotFound);
    }
    return retVal;
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
static void EndpointApiInitWorker(intptr_t context)
{
    // Silence complaints about unused variable.
    UNUSED_VAR(context);

    ESP_LOGI(TAG, "Init");
    memset(&endpointApi, 0x00, sizeof(endpointApi));

    // Set starting endpoint id where dynamic endpoints will be assigned, which
    // will be the next consecutive endpoint id after the last fixed endpoint.
    endpointApi.firstDynamicEndpointId = static_cast<chip::EndpointId>(static_cast<int>(emberAfEndpointFromIndex(static_cast<uint16_t>(emberAfFixedEndpointCount() - 1))) + 1);
    endpointApi.currentEndpointId = endpointApi.firstDynamicEndpointId;

    // Disable last fixed endpoint, which is used as a placeholder for all of the
    // supported clusters so that ZAP will generated the requisite code.
    emberAfEndpointEnableDisable(emberAfEndpointFromIndex(static_cast<uint16_t>(emberAfFixedEndpointCount() - 1)), false);

    // A bridge has root node device type on EP0 and aggregate node device type (bridge) at EP1
    emberAfSetDeviceTypeList(0, Span<const EmberAfDeviceType>(rootDeviceTypes));
    emberAfSetDeviceTypeList(1, Span<const EmberAfDeviceType>(aggregateNodeDeviceTypes));
}
static void EndpointAddWorker(intptr_t context)
{
    ENDPOINT_DATA *pData = reinterpret_cast<ENDPOINT_DATA *>(context);
    ESP_LOGI(TAG, "Adding device %u: %s", pData->index, pData->name);
    
    if (pData->index < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    {
        if (endpointApi.endpoint[pData->index].index == 0)
        {
            endpointApi.endpoint[pData->index].index = pData->index;
            endpointApi.endpoint[pData->index].pObject = pData->pObject;
            endpointApi.endpoint[pData->index].pfnReadCallback = pData->pfnReadCallback;
            endpointApi.endpoint[pData->index].pfnWriteCallback = pData->pfnWriteCallback;
            endpointApi.endpoint[pData->index].pfnInstantActionCallback = pData->pfnInstantActionCallback;

            while (true)
            {
                Span<DataVersion> dataVersion = Span<DataVersion>(pData->pDataVersionStorage, pData->dataVersionStorageLength);
                Span<const EmberAfDeviceType> deviceTypeList= Span<const EmberAfDeviceType>(pData->pDeviceTypeList, pData->deviceTypeListLength);
                EmberAfStatus ret = emberAfSetDynamicEndpoint(
                    pData->index, endpointApi.currentEndpointId, pData->ep, 
                    dataVersion, deviceTypeList, 
                    pData->parentEndpointId);
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
        else
        {
            ESP_LOGE(TAG, "Index already in use: %u: %s", pData->index, pData->name);
        }
    }
    else
    {
        ESP_LOGE(TAG, "Index out of range: %u: %s", pData->index, pData->name);
    }
    ChipLogProgress(DeviceLayer, "Failed to add dynamic endpoint: No endpoints available!");
}
static void EndpointRemoveWorker(intptr_t context)
{
    uint16_t index = (uint16_t)(uint32_t)reinterpret_cast<void*>(context);
    ESP_LOGI(TAG, "Removing device: %u", index);

    if (index < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    {
        if (endpointApi.endpoint[index].index == index)
        {
            memset(&endpointApi.endpoint[index], 0x00, sizeof(endpointApi.endpoint[index]));
            EndpointId ep = emberAfClearDynamicEndpoint(index);
            ChipLogProgress(DeviceLayer, "Removed device %u from dynamic endpoint %d", index, ep);

            // Silence complaints about unused ep when progress logging is disabled.
            UNUSED_VAR(ep);
        }
        else
        {
            ESP_LOGE(TAG, "Index not in use: %u", index);
        }
    }
    else
    {
        ESP_LOGE(TAG, "Index out of range: %u", index);
    }
}
static void EndpointReportUpdateWorker(intptr_t closure)
{
    auto path = reinterpret_cast<app::ConcreteAttributePath *>(closure);
    ESP_LOGI(TAG, "Update endpoint/cluster/attr %u/%lu/%lu", path->mEndpointId, path->mClusterId, path->mAttributeId);
    MatterReportingAttributeChangeCallback(*path);
    Platform::Delete(path);
}