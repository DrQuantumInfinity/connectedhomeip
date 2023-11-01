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
            _indexList[_index] = true;
            break;
        }
    }
    AddCluster(&basicCluster);
    basicCluster.SetReachable(true, GetIndex());
    basicCluster.SetName(pName, GetIndex());

    ENDPOINT_DATA endpointData = {
        .index = GetIndex(),
        .pObject = this,
        .pfnReadCallback = GoogleReadCallback,
        .pfnWriteCallback = GoogleWriteCallback,
        .pfnInstantActionCallback = NULL, //worry about this later
        .name = {0},
        .location = {0},
        .ep = &bridgedEndpoint,
        .pDeviceTypeList = bridgedDeviceTypes,
        .deviceTypeListLength = ArraySize(bridgedDeviceTypes),
        .pDataVersionStorage = pDataVersions,
        .dataVersionStorageLength = ArraySize(bridgedClusters),
        .parentEndpointId = 1,
    };
    strcpy(endpointData.name, pName);
    strcpy(endpointData.location, pLocation);
    memcpy(&_endpointData, &endpointData, sizeof(_endpointData));
}
Device::~Device(void)
{
    _indexList[_index] = false;
}

uint16_t Device::GetIndex(void)
{
    return _index;
}

void Device::AddCluster(Cluster * newCluster)
{
    _clusters.push_back(newCluster);
}

EmberAfStatus Device::GoogleReadCallback(void * pObject, ClusterId clusterId,
                                                const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer,
                                                uint16_t maxReadLength)
{
    Device * pDevice = (Device *) pObject;
    return pDevice->ReadCluster(clusterId, attributeMetadata, buffer, maxReadLength);
}

EmberAfStatus Device::GoogleWriteCallback(void * pObject, ClusterId clusterId,
                                                 const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer)
{

    Device * pDevice     = (Device *) pObject;
    EmberAfStatus status = pDevice->WriteCluster(clusterId, attributeMetadata, buffer);
    pDevice->sendEspNowMessage();
    if (pDevice->_pfnWriteCallback)
    {
        pDevice->_pfnWriteCallback(pDevice, clusterId, attributeMetadata, buffer);
    }
    return status;
}

void Device::sendEspNowMessage(void){
    return;
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
        for (auto & cluster : _clusters)
        {
            ESP_LOGI(TAG, "ClusterId of message:  %04lX", clusterId);
            ESP_LOGI(TAG, "Checking clusterId:  %04lX", cluster->_id);
            if (cluster->_id == clusterId)
            {
                ESP_LOGI(TAG, "Device with clusterId found");
                status = cluster->Read(attributeMetadata->attributeId, buffer, maxReadLength);
            }
        }
    }
    ESP_LOGI(TAG, "Device return from read");
    return status;
}
EmberAfStatus Device::WriteCluster(ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer)
{
    ESP_LOGI(TAG, "Device write called");
    EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;
    if (basicCluster._reachable)
    {
        ESP_LOGI(TAG, "Device reachable for write");
        status = EMBER_ZCL_STATUS_SUCCESS;
        for (auto & cluster : _clusters)
        {
            if (cluster->_id == clusterId)
            {
                ESP_LOGI(TAG, "Device found clusterid for write");
                status = cluster->Write(attributeMetadata->attributeId, buffer);
            }
        }
    }
    ESP_LOGI(TAG, "Device return from write");
    return status;
}