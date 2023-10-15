
#include "DeviceLight.h"

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
/**************************************************************************
 *                                  Variables
 **************************************************************************/
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
const EmberAfEndpointType* DeviceLightGetEndpoint(void)
{
    return &bridgedLightEndpoint;
}
chip::Span<const EmberAfDeviceType> DeviceLightGetTypes(void)
{
    return Span<const EmberAfDeviceType>(bridgedOnOffDeviceTypes);
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/