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
    AddCluster(&basicCluster);
    basicCluster._reachable = true;
}
Device::~Device(void)
{
    _indexList[_index] = false;
}

uint16_t Device::GetIndex(void)
{
    return _index;
}

void Device::AddCluster(Cluster* newCluster){
    _clusters.push_back(newCluster);
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
        for(auto& cluster : _clusters){
            ESP_LOGI(TAG, "ClusterId of message:  %04lX", clusterId);
            ESP_LOGI(TAG, "Checking clusterId:  %04lX", cluster->_id);
            if(cluster->_id == clusterId){
            ESP_LOGI(TAG, "Device with clusterId found");
                status = cluster->Read(attributeMetadata->attributeId, buffer, maxReadLength);
            }
        }
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
        for(auto& cluster : _clusters){       
            if(cluster->_id == clusterId){
                ESP_LOGI(TAG, "Device found clusterid for write");
                status = cluster->Write(attributeMetadata->attributeId, buffer);
            }
        }
    }
    ESP_LOGI(TAG, "Device return from write");
    return status;
}