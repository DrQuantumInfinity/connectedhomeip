
#include "DeviceTemperature.h"

#include "BasicCluster.h"
#include "DescriptorCluster.h"
#include "TempCluster.h"
#include <app/util/attribute-storage.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;
/**************************************************************************
 *                                  Constants
 **************************************************************************/

const EmberAfCluster bridgedClusters[] = {
    TempCluster::cluster,
    DescriptorCluster::cluster,
    BasicCluster::cluster,
};

// Declare Bridged Light endpoint
const EmberAfEndpointType bridgedEndpoint = { 
    .cluster = bridgedClusters, 
    .clusterCount = ArraySize(bridgedClusters), 
    .endpointSize = 0 
};


// (taken from chip-devices.xml)
#define DEVICE_TYPE_BRIDGED_NODE 0x0013
// (taken from lo-devices.xml)
#define DEVICE_TYPE_TEMP_MEASURE 0x0302
// Device Version for dynamic endpoints:
#define DEVICE_VERSION_DEFAULT 1
const EmberAfDeviceType bridgedDeviceTypes[] = { 
    [0] = {.deviceId = DEVICE_TYPE_TEMP_MEASURE, .deviceVersion = DEVICE_VERSION_DEFAULT},
    [1] = {.deviceId = DEVICE_TYPE_BRIDGED_NODE,    .deviceVersion = DEVICE_VERSION_DEFAULT} 
};
/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
// static EmberAfStatus GoogleReadCallback(void *pObject, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer, uint16_t maxReadLength);
// static EmberAfStatus GoogleWriteCallback(void *pObject, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer);
/**************************************************************************
 *                                  Variables
 **************************************************************************/
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
DeviceTemperature::DeviceTemperature(const char* pName, const char* pLocation, DEVICE_WRITE_CALLBACK pfnWriteCallback, float temp)
{
    _pfnWriteCallback = pfnWriteCallback;
    DataVersion* pDataVersions = (DataVersion*)malloc(sizeof(DataVersion)*ArraySize(bridgedClusters));
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
    AddCluster(&descriptorCluster);
    AddCluster(&tempCluster);
    tempCluster.UpdateTemp(temp, GetIndex());
    basicCluster.SetName(pName, GetIndex());
    strcpy(endpointData.name, pName);
    strcpy(endpointData.location, pLocation);

    memcpy(&_endpointData, &endpointData, sizeof(_endpointData));
    EndpointAdd(&_endpointData);
}

DeviceTemperature::~DeviceTemperature(void)
{
    free(_endpointData.pDataVersionStorage);
    EndpointRemove(GetIndex());
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
