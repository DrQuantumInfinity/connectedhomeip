
#include "DeviceLightLevel.h"
#include "AttrList.h"
#include "EndpointApi.h"
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
#define NODE_LABEL_SIZE (32)
#define DESCRIPTION_ATTR_ARRAY_LEN (254) // max

/* BRIDGED DEVICE ENDPOINT: contains the following clusters:
   - On/Off
   - Descriptor
   - Bridged Device Basic Information
*/

const EmberAfCluster bridgedLightClusters[] = {
    OnOffCluster::cluster,
    LevelControlCluster::cluster,
    DescriptorCluster::cluster,
    BasicCluster::cluster,
};
static_assert(ArraySize(bridgedLightClusters) == DEVICE_LIGHT_LEV_NUM_CLUSTERS);

// Declare Bridged Light endpoint
const EmberAfEndpointType bridgedLightEndpoint = { .cluster      = bridgedLightClusters,
                                                   .clusterCount = ArraySize(bridgedLightClusters),
                                                   .endpointSize = 0 };

// (taken from chip-devices.xml)
#define DEVICE_TYPE_BRIDGED_NODE 0x0013
// (taken from lo-devices.xml)
// #define DEVICE_TYPE_LO_ON_OFF_LIGHT 0x0100
#define DEVICE_TYPE_LO_LEVEL_LIGHT 0x0101
// Device Version for dynamic endpoints:
#define DEVICE_VERSION_DEFAULT 1
const EmberAfDeviceType bridgedOnOffDeviceTypes[] = {
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
// static EmberAfStatus ProcessAttributeOnOff(DeviceLightLevel *pDeviceLight, chip::AttributeId attributeId, uint8_t* buffer);
/**************************************************************************
 *                                  Variables
 **************************************************************************/
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
DeviceLightLevel::DeviceLightLevel(const char * pName, const char * pLocation, DEVICE_LIGHT_LEVEL_WRITE_CALLBACK pfnWriteCallback)
{
    _pfnWriteCallback          = pfnWriteCallback;
    ENDPOINT_DATA endpointData = {
        .index                    = 0 /*base class index*/,
        .pObject                  = this,
        .pfnReadCallback          = NULL /*local read function specific to a DeviceLightLevel*/,
        .pfnWriteCallback         = GoogleWriteCallback,
        .pfnInstantActionCallback = NULL, // worry about this later
        .name                     = { 0 },
        .location                 = { 0 },
        .ep                       = &bridgedLightEndpoint,
        .pDeviceTypeList          = bridgedOnOffDeviceTypes,
        .deviceTypeListLength     = ArraySize(bridgedOnOffDeviceTypes),
        .pDataVersionStorage      = _dataVersions,
        .dataVersionStorageLength = ArraySize(_dataVersions),
        .parentEndpointId         = 1,
    };
    strcpy(endpointData.name, pName);
    strcpy(endpointData.location, pLocation);
    EndpointAdd(&endpointData);
}
DeviceLightLevel::~DeviceLightLevel()
{
    EndpointRemove(0 /*TODO: pDeviceLight->base.index*/);
}

// void DeviceLightLevel::SetOn(bool on)
// {
//     onOffCluster.SetOn(on, GetIndex());
//     // EndpointReportChange(GetIndex(), OnOff::Id, OnOff::Attributes::OnOff::Id);
// }
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
static EmberAfStatus GoogleReadCallback(void * pObject, ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata,
                                        uint8_t * buffer, uint16_t maxReadLength)
{
    DeviceLightLevel * pDeviceLight = (DeviceLightLevel *) pObject;
    return pDeviceLight->ReadCluster(clusterId, attributeMetadata, buffer, maxReadLength);
}
EmberAfStatus DeviceLightLevel::ReadCluster(ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer,
                          uint16_t maxReadLength)
{
    EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;
    if (basicCluster._isReachable)
    {
        switch (clusterId)
        {
        case BridgedDeviceBasicInformation::Id:
            status = basicCluster.Read(attributeMetadata->attributeId, buffer, maxReadLength);
            break;
        case OnOff::Id:
            status = onOffCluster.Read(attributeMetadata->attributeId, buffer, maxReadLength);
            break;
        case LevelControl::Id:
            status = levelControlCluster.Read(attributeMetadata->attributeId, buffer, maxReadLength);
            break;
        default:
            status = EMBER_ZCL_STATUS_SUCCESS;
            break;
        }
    }
    return status;
}
static EmberAfStatus GoogleWriteCallback(void * pObject, ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata,
                                         uint8_t * buffer)
{
    DeviceLightLevel * pDeviceLight = (DeviceLightLevel *) pObject;
    pDeviceLight->WriteCluster(clusterId, attributeMetadata, buffer);
    pDeviceLight->_pfnWriteCallback(pDeviceLight, clusterId, attributeMetadata, buffer);
    return EMBER_ZCL_STATUS_SUCCESS;
}
EmberAfStatus DeviceLightLevel::WriteCluster(ClusterId clusterId, const EmberAfAttributeMetadata * attributeMetadata,
                                             uint8_t * buffer)
{
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    if (basicCluster._isReachable)
    {
        switch (clusterId)
        {
        case OnOff::Id:
            status = onOffCluster.Write(attributeMetadata->attributeId, buffer);
            break;
        case LevelControl::Id:
            status = levelControlCluster.Write(attributeMetadata->attributeId, buffer);
            break;
        default:
            status = EMBER_ZCL_STATUS_SUCCESS;
            break;
        }
    }
    return status;
}
