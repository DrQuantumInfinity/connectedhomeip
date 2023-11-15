
#include "DeviceLightRGB.h"
#include "EndpointApi.h"
#include "BasicCluster.h"
#include "DescriptorCluster.h"
#include "LevelControlCluster.h"
#include "LevelControlCluster2.h"
#include "SwitchCluster2.h"
#include "ColourCluster.h"
#include "OnOffCluster.h"
#include "OnOffCluster2.h"
#include "SerialTask.h"
#include <app/util/attribute-storage.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;
/**************************************************************************
 *                                  Constants
 **************************************************************************/

EmberAfCluster bridgedClusters[] = {   
    {0},
    {0},
    DescriptorCluster::cluster,
//    {0}, 
    BasicCluster::cluster,
};

// Declare Bridged Light endpoint
EmberAfEndpointType bridgedEndpoint = { 
    .cluster      = bridgedClusters,
    .clusterCount = ArraySize(bridgedClusters),
    .endpointSize = 0 //Assigned dynamically on first constructor()
};

// (taken from chip-devices.xml)
#define DEVICE_TYPE_BRIDGED_NODE 0x0013
// Device Version for dynamic endpoints:
#define DEVICE_VERSION_DEFAULT 1
const EmberAfDeviceType bridgedDeviceTypes[] = {
    { .deviceId = 0x0101, .deviceVersion = DEVICE_VERSION_DEFAULT },
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
/**************************************************************************
 *                                  Variables
 **************************************************************************/
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
DeviceLightRGB::DeviceLightRGB(const char* pName, const char* pLocation, DEVICE_WRITE_CALLBACK pfnWriteCallback)
{
    memcpy(&bridgedClusters[0], ClusterOnOffGetObject(), sizeof(bridgedClusters[0]));
    memcpy(&bridgedClusters[1], ClusterLevelControlGetObject(), sizeof(bridgedClusters[1]));
//    memcpy(&bridgedClusters[3], ClusterSwitchGetObject(), sizeof(bridgedClusters[3]));
    if (bridgedEndpoint.endpointSize == 0) //only perform this the first time
    {
        for (int i = 0; i < bridgedEndpoint.clusterCount; i++)
        {
            bridgedEndpoint.endpointSize += bridgedEndpoint.cluster[i].clusterSize;
        }
    }
    
    _pfnWriteCallback          = pfnWriteCallback;
    DataVersion* pDataVersions = (DataVersion*)malloc(sizeof(DataVersion)*ArraySize(bridgedClusters));
    ENDPOINT_DATA endpointData = {
        .index                    = GetIndex(),
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
    
    levelControlCluster._level = 74*255/100;
    levelControlCluster._minLevel = 0;
    levelControlCluster._maxLevel = 255;
    colourCluster._hue = 100;
    colourCluster._sat = 90;
    strncpy(basicCluster._name, pName, sizeof(basicCluster._name));

    strcpy(endpointData.name, pName);
    strcpy(endpointData.location, pLocation);

    memcpy(&_endpointData, &endpointData, sizeof(_endpointData));
    EndpointAdd(&_endpointData);
}
DeviceLightRGB::~DeviceLightRGB()
{   
    free(_endpointData.pDataVersionStorage);
    EndpointRemove(GetIndex());
}

/**************************************************************************
 *                                  Private Functions
 **************************************************************************/

void DeviceLightRGB::sendEspNowMessage()
{
    _espNowData.data.lightRgb.onOff      = onOffCluster._isOn;
    _espNowData.data.lightRgb.brightness = levelControlCluster._level;
    _espNowData.data.lightRgb.hue        = colourCluster._hue;
    _espNowData.data.lightRgb.saturation = colourCluster._sat;
    _espNowData.data.lightRgb.mode       = ESP_NOW_DATA_LIGHT_RGB_MODE_STATIC;
    _espNowData.type   = ESP_NOW_DEVICE_TYPE_LIGHT_RGB;
    uint8_t macAddr[6] = { 0x18, 0xFE, 0x34, 0xDC, 0x1F, 0xAF };
    memcpy(_espNowData.macAddr, macAddr, sizeof(_espNowData.macAddr));

    SerialTransmit(&_espNowData, offsetof(ESP_NOW_DATA, data) + sizeof(ESP_NOW_DATA_LIGHT_RGB));
}