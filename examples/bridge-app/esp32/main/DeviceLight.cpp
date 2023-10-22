
#include "DeviceLight.h"

#include "clusters/BasicCluster.h"
#include "clusters/DescriptorCluster.h"
#include "clusters/LevelControlCluster.h"
#include "clusters/OnOffCluster.h"
#include <app/util/attribute-storage.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;
/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define NODE_LABEL_SIZE             (32)
#define DESCRIPTION_ATTR_ARRAY_LEN  (254) //max

const EmberAfCluster bridgedLightClusters[] = {
    OnOffCluster::cluster,
    DescriptorCluster::cluster,
    BasicCluster::cluster,
};

// Declare Bridged Light endpoint
const EmberAfEndpointType bridgedLightEndpoint = { 
    .cluster = bridgedLightClusters, 
    .clusterCount = ArraySize(bridgedLightClusters), 
    .endpointSize = 0 
};


// (taken from chip-devices.xml)
#define DEVICE_TYPE_BRIDGED_NODE 0x0013
// (taken from lo-devices.xml)
#define DEVICE_TYPE_LO_ON_OFF_LIGHT 0x0100
// Device Version for dynamic endpoints:
#define DEVICE_VERSION_DEFAULT 1
const EmberAfDeviceType bridgedOnOffDeviceTypes[] = { 
    [0] = {.deviceId = DEVICE_TYPE_LO_ON_OFF_LIGHT, .deviceVersion = DEVICE_VERSION_DEFAULT},
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
static EmberAfStatus GoogleReadCallback(void *pObject, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer, uint16_t maxReadLength);
static EmberAfStatus ReadCluster(DeviceLight *pDeviceLight, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer, uint16_t maxReadLength);
static EmberAfStatus ReadAttributeBasicInfo(DeviceLight *pDeviceLight, chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength);
static EmberAfStatus ReadAttributeOnOff(DeviceLight *pDeviceLight, chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength);
static EmberAfStatus GoogleWriteCallback(void *pObject, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer);
static EmberAfStatus WriteCluster(DeviceLight *pDeviceLight, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer);
static EmberAfStatus WriteAttributeOnOff(DeviceLight *pDeviceLight, chip::AttributeId attributeId, uint8_t* buffer);
/**************************************************************************
 *                                  Variables
 **************************************************************************/
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
DeviceLight::DeviceLight(const char* pName, const char* pLocation, DEVICE_LIGHT_WRITE_CALLBACK pfnWriteCallback)
{
    _pfnWriteCallback = pfnWriteCallback;
    DataVersion* pDataVersions = (DataVersion*)malloc(sizeof(DataVersion)*ArraySize(bridgedLightClusters));
    ENDPOINT_DATA endpointData = {
        .index = GetIndex(),
        .pObject = this,
        .pfnReadCallback = GoogleReadCallback,
        .pfnWriteCallback = GoogleWriteCallback,
        .pfnInstantActionCallback = NULL, //worry about this later
        .name = {0},
        .location = {0},
        .ep = &bridgedLightEndpoint,
        .pDeviceTypeList = bridgedOnOffDeviceTypes,
        .deviceTypeListLength = ArraySize(bridgedOnOffDeviceTypes),
        .pDataVersionStorage = pDataVersions,
        .dataVersionStorageLength = ArraySize(bridgedLightClusters),
        .parentEndpointId = 1,
    };
    AddCluster(std::make_unique<DescriptorCluster>());
    AddCluster(std::make_unique<OnOffCluster>());
    strcpy(endpointData.name, pName);
    strcpy(endpointData.location, pLocation);

    memcpy(&_endpointData, &endpointData, sizeof(_endpointData));
    EndpointAdd(&_endpointData);
}

DeviceLight::~DeviceLight(void)
{
    free(_endpointData.pDataVersionStorage);
    EndpointRemove(GetIndex());
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
static EmberAfStatus GoogleReadCallback(void *pObject, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer, uint16_t maxReadLength)
{
    DeviceLight * pDeviceLight = (DeviceLight *) pObject;
    return pDeviceLight->ReadCluster(clusterId, attributeMetadata, buffer, maxReadLength);
}

static EmberAfStatus GoogleWriteCallback(void * pObject, ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata,
                                         uint8_t * buffer)
{
    DeviceLight * pDeviceLight = (DeviceLight *) pObject;
    EmberAfStatus status = pDeviceLight->WriteCluster(clusterId, attributeMetadata, buffer);
    if (pDeviceLight->_pfnWriteCallback)
    {
        pDeviceLight->_pfnWriteCallback(pDeviceLight, clusterId, attributeMetadata, buffer);
    }
    return status;
}