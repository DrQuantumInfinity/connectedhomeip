
#include "DeviceLight.h"
#include "EndpointApi.h"

#include <app/util/attribute-storage.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;
/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define NODE_LABEL_SIZE             (32)
#define DESCRIPTION_ATTR_ARRAY_LEN  (254) //max

// Declare Cluster List for Bridged Light endpoint
const CommandId onOffIncomingCommands[] = {
    app::Clusters::OnOff::Commands::Off::Id,
    app::Clusters::OnOff::Commands::On::Id,
    app::Clusters::OnOff::Commands::Toggle::Id,
    app::Clusters::OnOff::Commands::OffWithEffect::Id,
    app::Clusters::OnOff::Commands::OnWithRecallGlobalScene::Id,
    app::Clusters::OnOff::Commands::OnWithTimedOff::Id,
    kInvalidCommandId,
};

// Declare On/Off cluster attributes
const EmberAfAttributeMetadata onOffAttrs[] = {
    [0] = { //onOff attribute
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = OnOff::Attributes::OnOff::Id,
        .size = 1,
        .attributeType = ZAP_TYPE(BOOLEAN),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    [1] = { //end?
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = 0xFFFD,
        .size = 2,
        .attributeType = ZAP_TYPE(INT16U),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
};

// Declare Descriptor cluster attributes
const EmberAfAttributeMetadata descriptorAttrs[] = {
    [0] = { //device list
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = Descriptor::Attributes::DeviceTypeList::Id,
        .size = DESCRIPTION_ATTR_ARRAY_LEN,
        .attributeType = ZAP_TYPE(ARRAY),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    [1] = { //server list
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = Descriptor::Attributes::ServerList::Id,
        .size = DESCRIPTION_ATTR_ARRAY_LEN,
        .attributeType = ZAP_TYPE(ARRAY),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    [2] = { //client list
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = Descriptor::Attributes::ClientList::Id,
        .size = DESCRIPTION_ATTR_ARRAY_LEN,
        .attributeType = ZAP_TYPE(ARRAY),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    [3] = { //parts list
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = Descriptor::Attributes::PartsList::Id,
        .size = DESCRIPTION_ATTR_ARRAY_LEN,
        .attributeType = ZAP_TYPE(ARRAY),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    [4] = { //end?
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = 0xFFFD,
        .size = 2,
        .attributeType = ZAP_TYPE(INT16U),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
};

// Declare Bridged Device Basic Information cluster attributes
const EmberAfAttributeMetadata bridgedDeviceBasicAttrs[] = {
    [0] = { //node label
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = BridgedDeviceBasicInformation::Attributes::NodeLabel::Id,
        .size = NODE_LABEL_SIZE,
        .attributeType = ZAP_TYPE(CHAR_STRING),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    [1] = { //reachable
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = BridgedDeviceBasicInformation::Attributes::Reachable::Id,
        .size = 1,
        .attributeType = ZAP_TYPE(BOOLEAN),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    [2] = { //end?
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = 0xFFFD,
        .size = 2,
        .attributeType = ZAP_TYPE(INT16U),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
};


/* BRIDGED DEVICE ENDPOINT: contains the following clusters:
   - On/Off
   - Descriptor
   - Bridged Device Basic Information
*/
const EmberAfCluster bridgedLightClusters[] = {
    [0] = { 
        .clusterId = OnOff::Id, 
        .attributes = onOffAttrs, 
        .attributeCount = ArraySize(onOffAttrs), 
        .clusterSize = 0, 
        .mask = ZAP_CLUSTER_MASK(SERVER), 
        .functions = NULL, 
        .acceptedCommandList = onOffIncomingCommands, 
        .generatedCommandList = nullptr,
        .eventList = nullptr,
        .eventCount = 0
    },
    [1] = { 
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
    [2] = { 
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
static_assert(ArraySize(bridgedLightClusters) == DEVICE_LIGHT_NUM_CLUSTERS);

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
static EmberAfStatus ProcessCluster(DeviceLight *pDeviceLight, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer);
static EmberAfStatus ProcessAttributeOnOff(DeviceLight *pDeviceLight, chip::AttributeId attributeId, uint8_t* buffer);
/**************************************************************************
 *                                  Variables
 **************************************************************************/
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
DeviceLight::DeviceLight(const char* pName, const char* pLocation, DEVICE_LIGHT_WRITE_CALLBACK pfnWriteCallback)
{
    _pfnWriteCallback = pfnWriteCallback;
    ENDPOINT_DATA endpointData = {
        .index = 0/*base class index*/,
        .pObject = this,
        .pfnReadCallback = NULL /*local read function specific to a DeviceLight*/,
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
void DeviceLight::Dispose(void)
{
    EndpointRemove(0/*TODO: pDeviceLight->base.index*/);
}
void DeviceLight::SetOn(bool on)
{
    _isOn = on;
    EndpointReportChange(GetIndex(), OnOff::Id, OnOff::Attributes::OnOff::Id);
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
//GoogleReadCallback
static EmberAfStatus GoogleWriteCallback(void *pObject, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer)
{
    DeviceLight *pDeviceLight = (DeviceLight*)pObject;
    ProcessCluster(pDeviceLight, clusterId, attributeMetadata, buffer);
    pDeviceLight->_pfnWriteCallback(pDeviceLight, clusterId, attributeMetadata, buffer);
    return EMBER_ZCL_STATUS_SUCCESS;
}
static EmberAfStatus ProcessCluster(DeviceLight *pDeviceLight, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer)
{
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    if (pDeviceLight->IsReachable())
    {
        switch (clusterId)
        {
        case OnOff::Id: status = ProcessAttributeOnOff(pDeviceLight, attributeMetadata->attributeId, buffer);   break;
        default:        status = EMBER_ZCL_STATUS_SUCCESS;                                                      break;
        }
    }
    return status;
}
static EmberAfStatus ProcessAttributeOnOff(DeviceLight *pDeviceLight, chip::AttributeId attributeId, uint8_t* buffer)
{
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    switch (attributeId)
    {
    case OnOff::Attributes::OnOff::Id:  pDeviceLight->_isOn = (bool)buffer[0];   break;
    default:                            status = EMBER_ZCL_STATUS_FAILURE;      break;
    }
    return status;
}
