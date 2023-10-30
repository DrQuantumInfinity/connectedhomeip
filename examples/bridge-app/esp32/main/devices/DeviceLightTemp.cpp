
#include "DeviceLightTemp.h"
#include "AttrList.h"
#include "EndpointApi.h"
#include "BasicCluster.h"
#include "DescriptorCluster.h"
#include "LevelControlCluster.h"
#include "ColourCluster.h"
#include "OnOffCluster.h"
#include "SerialTask.h"
#include <app/util/attribute-storage.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;
/**************************************************************************
 *                                  Constants
 **************************************************************************/

const EmberAfCluster bridgedClusters[] = {
    OnOffCluster::cluster,
    LevelControlCluster::cluster,
    ColourCluster::hsCluster,
    DescriptorCluster::cluster,
    BasicCluster::cluster,
};

// Declare Bridged Light endpoint
const EmberAfEndpointType bridgedEndpoint = { .cluster      = bridgedClusters,
                                                   .clusterCount = ArraySize(bridgedClusters),
                                                   .endpointSize = 0 };

// (taken from chip-devices.xml)
#define DEVICE_TYPE_BRIDGED_NODE 0x0013
// (taken from lo-devices.xml)
// #define DEVICE_TYPE_LO_ON_OFF_LIGHT 0x0100
#define DEVICE_TYPE_LO_LEVEL_LIGHT 0x0101
// Device Version for dynamic endpoints:
#define DEVICE_VERSION_DEFAULT 1
const EmberAfDeviceType bridgedDeviceTypes[] = {
    { .deviceId = DEVICE_TYPE_LO_LEVEL_LIGHT, .deviceVersion = DEVICE_VERSION_DEFAULT },
    { .deviceId = DEVICE_TYPE_BRIDGED_NODE, .deviceVersion = DEVICE_VERSION_DEFAULT }
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
// of type GOOGLE_WRITE_CALLBACK
static EmberAfStatus GoogleWriteCallback(void * pObject, ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata,
                                         uint8_t * buffer);
static EmberAfStatus GoogleReadCallback(void * pObject, ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata,
                                        uint8_t * buffer, uint16_t maxReadLength);
/**************************************************************************
 *                                  Variables
 **************************************************************************/
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
DeviceLightTemp::DeviceLightTemp(const char * pName, const char * pLocation, DEVICE_LIGHT_TEMP_WRITE_CALLBACK pfnWriteCallback)
{
    _pfnWriteCallback          = pfnWriteCallback;
    DataVersion* pDataVersions = (DataVersion*)malloc(sizeof(DataVersion)*ArraySize(bridgedClusters));
    ENDPOINT_DATA endpointData = {
        .index                    = 0 /*base class index*/,
        .pObject                  = this,
        .pfnReadCallback          = GoogleReadCallback /*local read function specific to a DeviceLightLevel*/,
        .pfnWriteCallback         = GoogleWriteCallback,
        .pfnInstantActionCallback = NULL, // worry about this later
        .name                     = { 0 },
        .location                 = { 0 },
        .ep                       = &bridgedEndpoint,
        .pDeviceTypeList          = bridgedDeviceTypes,
        .deviceTypeListLength     = ArraySize(bridgedDeviceTypes),
        .pDataVersionStorage      = pDataVersions,
        .dataVersionStorageLength = ArraySize(bridgedClusters),
        .parentEndpointId         = 1,
    };
    
    AddCluster(&descriptorCluster);
    AddCluster(&onOffCluster);
    AddCluster(&levelControlCluster);
    AddCluster(&colourCluster);
    strcpy(endpointData.name, pName);
    strcpy(endpointData.location, pLocation);

    memcpy(&_endpointData, &endpointData, sizeof(_endpointData));
    EndpointAdd(&_endpointData);
}
DeviceLightTemp::~DeviceLightTemp()
{   
    free(_endpointData.pDataVersionStorage);
    EndpointRemove(GetIndex());
}

/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
static EmberAfStatus GoogleReadCallback(void * pObject, ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata,
                                        uint8_t * buffer, uint16_t maxReadLength)
{
    DeviceLightTemp * pDevice = (DeviceLightTemp *) pObject;
    return pDevice->ReadCluster(clusterId, attributeMetadata, buffer, maxReadLength);
}

static EmberAfStatus GoogleWriteCallback(void * pObject, ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata,
                                         uint8_t * buffer)
{
    DeviceLightTemp * pDevice = (DeviceLightTemp *) pObject;
    EmberAfStatus status  = pDevice->WriteCluster(clusterId, attributeMetadata, buffer);
    pDevice->sendEspNowMessage();
    if (pDevice->_pfnWriteCallback)
    {
        pDevice->_pfnWriteCallback(pDevice, clusterId, attributeMetadata, buffer);
    }
    return status;
}

void DeviceLightTemp::sendEspNowMessage()
{
    _espNowData.data.lightTempRgb.onOff = onOffCluster._isOn;
    _espNowData.data.lightTempRgb.brightness = levelControlCluster._level;
    _espNowData.data.lightTempRgb.hue = colourCluster._hue;
    _espNowData.data.lightTempRgb.saturation = colourCluster._sat;
    _espNowData.data.lightTempRgb.tempKelvin = 1000_000 / colourCluster._temp;
    SerialTransmit(&_espNowData, sizeof(_espNowData));
}