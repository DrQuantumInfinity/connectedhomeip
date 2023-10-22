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

#include "Device.h"
#include "EndpointApi.h"
#include "esp_log.h"

static const char * TAG = "Device";
Device::Device(void)
{
    for (_index = 0; _index < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT; _index++)
    {
        if (!_indexList[_index])
        {
            break;
        }
    }
    basicCluster._reachable = true;
    // basicCluster.SetReachable(true, GetIndex());
}
Device::~Device(void)
{
    _indexList[_index] = false;
}

uint16_t Device::GetIndex(void)
{
    return _index;
}

void Device::AddCluster(Cluster* newCluster, int ind){
    _clusters[ind] = newCluster;
    // _clusters.push_back(std::make_shared<Cluster>(std::move(newCluster)));
}

EmberAfStatus Device::ReadCluster(ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer,
                          uint16_t maxReadLength)
{
    ESP_LOGI(TAG, "Device Read called");
    EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;
    if (basicCluster._reachable)
    {
        ESP_LOGI(TAG, "Device Reachable");
        status = EMBER_ZCL_STATUS_SUCCESS;
        // for(auto& cluster : _clusters){
            ESP_LOGI(TAG, "ClusterId of message: %lu", clusterId);
        for(int ind = 0; ind < 2; ind++){
            ESP_LOGI(TAG, "Checking clusterId: %lu", _clusters[ind]->_id);
            if(_clusters[ind]->_id == clusterId){
            ESP_LOGI(TAG, "Device with clusterId found");
                status = _clusters[ind]->Read(attributeMetadata->attributeId, buffer, maxReadLength);
            }
        }
        // switch (clusterId)
        // {
        // case BridgedDeviceBasicInformation::Id:
        //     status = basicCluster.Read(attributeMetadata->attributeId, buffer, maxReadLength);
        //     break;
        // case OnOff::Id:
        //     status = onOffCluster.Read(attributeMetadata->attributeId, buffer, maxReadLength);
        //     break;
        // case LevelControl::Id:
        //     status = levelControlCluster.Read(attributeMetadata->attributeId, buffer, maxReadLength);
        //     break;
        // default:
        //     status = EMBER_ZCL_STATUS_SUCCESS;
        //     break;
        // }
    }
    ESP_LOGI(TAG, "Device return from read");
    return status;
}
EmberAfStatus Device::WriteCluster(ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata,
                                             uint8_t * buffer)
{
    ESP_LOGI(TAG, "Device write called");
    EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;
    if (basicCluster._reachable)
    {
        ESP_LOGI(TAG, "Device reachable for write");
        status = EMBER_ZCL_STATUS_SUCCESS;
        // for(auto& cluster : _clusters){
        for(int ind = 0; ind < 2; ind++){
       
            if(_clusters[ind]->_id == clusterId){
                ESP_LOGI(TAG, "Device found clusterid for write");
                status = _clusters[ind]->Write(attributeMetadata->attributeId, buffer);
            }
        }
        // switch (clusterId)
        // {
        // case OnOff::Id:
        //     status = onOffCluster.Write(attributeMetadata->attributeId, buffer);
        //     break;
        // case LevelControl::Id:
        //     status = levelControlCluster.Write(attributeMetadata->attributeId, buffer);
        //     break;
        // default:
        //     status = EMBER_ZCL_STATUS_SUCCESS;
        //     break;
        // }
    }
    ESP_LOGI(TAG, "Device return from write");
    return status;
}
// bool Device::IsReachable(void) 
// {
//     return _reachable;
// }
// void Device::SetReachable(bool reachable)
// {
//     _reachable = reachable;
//     EndpointReportChange(_index, BridgedDeviceBasicInformation::Id, BridgedDeviceBasicInformation::Attributes::Reachable::Id);
// }






// EmberAfStatus Device::ReadCluster(ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer,
//                           uint16_t maxReadLength)
// {
//     EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;
//     if (basicCluster._reachable)
//     {
//         status = EMBER_ZCL_STATUS_SUCCESS;
//         for(auto& cluster : _clusters){
//         // for(int ind = 0; ind < 2; ind++){
//             if(cluster->_id == clusterId){
//                 status = cluster->Read(attributeMetadata->attributeId, buffer, maxReadLength);
//             }
//         }
//         // switch (clusterId)
//         // {
//         // case BridgedDeviceBasicInformation::Id:
//         //     status = basicCluster.Read(attributeMetadata->attributeId, buffer, maxReadLength);
//         //     break;
//         // case OnOff::Id:
//         //     status = onOffCluster.Read(attributeMetadata->attributeId, buffer, maxReadLength);
//         //     break;
//         // case LevelControl::Id:
//         //     status = levelControlCluster.Read(attributeMetadata->attributeId, buffer, maxReadLength);
//         //     break;
//         // default:
//         //     status = EMBER_ZCL_STATUS_SUCCESS;
//         //     break;
//         // }
//     }
//     return status;
// }
// EmberAfStatus Device::WriteCluster(ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata,
//                                              uint8_t * buffer)
// {
//     EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;
//     if (basicCluster._reachable)
//     {
//         status = EMBER_ZCL_STATUS_SUCCESS;
//         for(auto& cluster : _clusters){
//         // for(int ind = 0; ind < 2; ind++){
       
//             if(cluster->_id == clusterId){
//                 status = cluster->Write(attributeMetadata->attributeId, buffer);
//             }
//         }
//         // switch (clusterId)
//         // {
//         // case OnOff::Id:
//         //     status = onOffCluster.Write(attributeMetadata->attributeId, buffer);
//         //     break;
//         // case LevelControl::Id:
//         //     status = levelControlCluster.Write(attributeMetadata->attributeId, buffer);
//         //     break;
//         // default:
//         //     status = EMBER_ZCL_STATUS_SUCCESS;
//         //     break;
//         // }
//     }
//     return status;
// }
// // bool Device::IsReachable(void) 
// // {
// //     return _reachable;
// // }
// // void Device::SetReachable(bool reachable)
// // {
// //     _reachable = reachable;
// //     EndpointReportChange(_index, BridgedDeviceBasicInformation::Id, BridgedDeviceBasicInformation::Attributes::Reachable::Id);
// // }