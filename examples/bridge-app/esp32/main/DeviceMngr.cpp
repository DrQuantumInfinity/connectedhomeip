// /*
//  *
//  *    Copyright (c) 2020 Project CHIP Authors
//  *
//  *    Licensed under the Apache License, Version 2.0 (the "License");
//  *    you may not use this file except in compliance with the License.
//  *    You may obtain a copy of the License at
//  *
//  *        http://www.apache.org/licenses/LICENSE-2.0
//  *
//  *    Unless required by applicable law or agreed to in writing, software
//  *    distributed under the License is distributed on an "AS IS" BASIS,
//  *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  *    See the License for the specific language governing permissions and
//  *    limitations under the License.
//  */

// #include "DeviceMngr.h"
// #include "Device.h"
// #include "DeviceCallbacks.h"
// #include "esp_log.h"
// // #include "nvs_flash.h"
// #include <app-common/zap-generated/ids/Attributes.h>
// #include <app-common/zap-generated/ids/Clusters.h>
// #include <app/ConcreteAttributePath.h>
// // #include <app/clusters/identify-server/identify-server.h>
// #include <app/reporting/reporting.h>
// // #include <app/server/OnboardingCodesUtil.h>
// #include <app/util/attribute-storage.h>
// #include <common/Esp32AppServer.h>
// // #include <credentials/DeviceAttestationCredsProvider.h>
// // #include <credentials/examples/DeviceAttestationCredsExample.h>
// #include <lib/core/CHIPError.h>
// // #include <lib/support/CHIPMem.h>
// // #include <lib/support/CHIPMemString.h>
// #include <lib/support/ZclString.h>
// // #include <platform/ESP32/ESP32Utils.h>

// #include <app/InteractionModelEngine.h>
// #include <app/server/Server.h>

// #if CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER
// #include <platform/ESP32/ESP32FactoryDataProvider.h>
// #endif // CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER

// #if CONFIG_ENABLE_ESP32_DEVICE_INFO_PROVIDER
// #include <platform/ESP32/ESP32DeviceInfoProvider.h>
// #else
// #include <DeviceInfoProviderImpl.h>
// #endif // CONFIG_ENABLE_ESP32_DEVICE_INFO_PROVIDER

// namespace {
// #if CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER
// chip::DeviceLayer::ESP32FactoryDataProvider sFactoryDataProvider;
// #endif // CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER

// #if CONFIG_ENABLE_ESP32_DEVICE_INFO_PROVIDER
// chip::DeviceLayer::ESP32DeviceInfoProvider gExampleDeviceInfoProvider;
// #else
// chip::DeviceLayer::DeviceInfoProviderImpl gExampleDeviceInfoProvider;
// #endif // CONFIG_ENABLE_ESP32_DEVICE_INFO_PROVIDER
// } // namespace

// /**************************************************************************
//  *                                  Constants
//  **************************************************************************/

// static const char * TAG = "device-mngr";

// /**************************************************************************
//  *                                  Macros
//  **************************************************************************/
// /**************************************************************************
//  *                                  Types
//  **************************************************************************/
// /**************************************************************************
//  *                                  Prototypes
//  **************************************************************************/
// DeviceMngr_ID ReserveSafeID();
// void FreeID(DeviceMngr_ID id);
// /**************************************************************************
//  *                                  Variables
//  **************************************************************************/

// typedef struct
// {
//     bool inUse;
//     DeviceMngr_DevType type;
//     void * pDevice;
// } DevMngrDevice;
// #define NUM_IDS = 10
// DevMngrDevice devList[NUM_IDS];

// /**************************************************************************
//  *                                  Global Functions
//  **************************************************************************/
// DeviceMngr_ID ReserveSafeID()
// {
//     for (DeviceMngr_ID ind = 0; ind < NUM_IDS; ind++)
//     {
//         if (!devList[ind].inUse)
//         {
//             devList[ind].inUse = true;
//             return ind;
//         }
//     }
//     return -1;
// }

// void FreeID(DeviceMngr_ID id)
// {
//     devList[id].inUse = false;
// }

// DeviceMngr_ID AddDevice(DevType devType)
// {
//     DeviceMngr_ID id = ReserveSafeID();
//     if (id > 0)
//     {
//         DevMngrDevice dev = devList[id];
//         dev.type = devType;
//         switch (devType)
//         {
//         case DeviceMngr_DevType_ONOFF_LIGHT:
//             dev.pDevice = DeviceLightAllocate();
//             break;
//         case default:
//             // unsupported device type
//         }
//     }
// }

// void RemoveDevice(DeviceMngr_ID deviceId)
// {
//     DevMngrDevice dev = devList[deviceId];
//     switch (dev.type)
//     {
//     case DeviceMngr_DevType_ONOFF_LIGHT:
//         DeviceLightFree(dev.pDevice);
//         break;
//     case default:
//         // unsupported device type
//     }
//     FreeID(deviceId);
// }

// void InitDevMgr()
// {
//     ESP_LOGI(TAG, "Init");
//     memset(devList, 0, sizeof devList);
//     // call endpoint init
// }

// /**************************************************************************
//  *                                  Private Functions
//  **************************************************************************/