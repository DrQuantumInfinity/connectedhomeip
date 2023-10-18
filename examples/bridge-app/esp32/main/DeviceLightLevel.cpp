
#include "DeviceLightLevel.h"
#include "EndpointApi.h"
#include "OnOffCluster.h"
#include <app/util/attribute-storage.h>
#include "AttrList.h"
using namespace ::chip;
using namespace ::chip::app::Clusters;
/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define NODE_LABEL_SIZE             (32)
#define DESCRIPTION_ATTR_ARRAY_LEN  (254) //max

/* BRIDGED DEVICE ENDPOINT: contains the following clusters:
   - On/Off
   - Descriptor
   - Bridged Device Basic Information
*/

const EmberAfCluster bridgedLightClusters[] = {
    OnOffCluster::cluster,
    { 
        .clusterId = LevelControl::Id, 
        .attributes = levelAttrs, 
        .attributeCount = ArraySize(levelAttrs), 
        .clusterSize = 0, 
        .mask = ZAP_CLUSTER_MASK(SERVER), 
        .functions = NULL, 
        .acceptedCommandList = nullptr, 
        .generatedCommandList = nullptr,
        .eventList = nullptr,
        .eventCount = 0
    },
    { 
        .clusterId = Descriptor::Id, 
        .attributes = descriptorAttrs, 
        .attributeCount = ArraySize(descriptorAttrs), 
        .clusterSize = 0, 
        .mask = ZAP_CLUSTER_MASK(SERVER), 
        .functions = NULL, 
        .acceptedCommandList = nullptr, 
        .generatedCommandList = nullptr,
        .eventList = nullptr,
        .eventCount = 0
    },
    { 
        .clusterId = BridgedDeviceBasicInformation::Id, 
        .attributes = bridgedDeviceBasicAttrs, 
        .attributeCount = ArraySize(bridgedDeviceBasicAttrs), 
        .clusterSize = 0, 
        .mask = ZAP_CLUSTER_MASK(SERVER), 
        .functions = NULL, 
        .acceptedCommandList = nullptr, 
        .generatedCommandList = nullptr,
        .eventList = nullptr,
        .eventCount = 0
    },
};
static_assert(ArraySize(bridgedLightClusters) == DEVICE_LIGHT_LEV_NUM_CLUSTERS);

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
//of type GOOGLE_WRITE_CALLBACK
static EmberAfStatus GoogleWriteCallback(void *pObject, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer);
static EmberAfStatus ProcessAttributeOnOff(DeviceLightLevel *pDeviceLight, chip::AttributeId attributeId, uint8_t* buffer);
/**************************************************************************
 *                                  Variables
 **************************************************************************/
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
DeviceLightLevel::DeviceLightLevel(const char* pName, const char* pLocation, DEVICE_LIGHT_LEVEL_WRITE_CALLBACK pfnWriteCallback)
{
    _pfnWriteCallback = pfnWriteCallback;
    ENDPOINT_DATA endpointData = {
        .index = 0/*base class index*/,
        .pObject = this,
        .pfnReadCallback = NULL /*local read function specific to a DeviceLightLevel*/,
        .pfnWriteCallback = GoogleWriteCallback,
        .pfnInstantActionCallback = NULL, //worry about this later
        .name = {0},
        .location = {0},
        .ep = &bridgedLightEndpoint,
        .deviceTypeList = Span<const EmberAfDeviceType>(bridgedOnOffDeviceTypes),
        .dataVersionStorage = Span<DataVersion>(_dataVersions),
        .parentEndpointId = 1,
    };
    strcpy(endpointData.name, pName);
    strcpy(endpointData.location, pLocation);
    EndpointAdd(&endpointData);
}
DeviceLightLevel::~DeviceLightLevel()
{
    EndpointRemove(0/*TODO: pDeviceLight->base.index*/);
}

void DeviceLightLevel::SetOn(bool on)
{
    onOffcluster.SetOn(on,GetIndex());
    // EndpointReportChange(GetIndex(), OnOff::Id, OnOff::Attributes::OnOff::Id);
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
//GoogleReadCallback
static EmberAfStatus GoogleWriteCallback(void *pObject, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer)
{
    DeviceLightLevel *pDeviceLight = (DeviceLightLevel*)pObject;
    (*pDeviceLight).ProcessCluster(clusterId, attributeMetadata, buffer);
    pDeviceLight->_pfnWriteCallback(pDeviceLight, clusterId, attributeMetadata, buffer);
    return EMBER_ZCL_STATUS_SUCCESS;
}
EmberAfStatus DeviceLightLevel::ProcessCluster(ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer)
{
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    if (Device::IsReachable())
    {
        switch (clusterId)
        {
        case OnOff::Id: status = onOffcluster.ProcessAttributeOnOff(attributeMetadata->attributeId, buffer);
        default:        status = EMBER_ZCL_STATUS_SUCCESS;                                                      break;
        }
    }
    return status;
}
