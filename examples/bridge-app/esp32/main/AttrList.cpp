
#include "AttrList.h"

#include "DeviceLight.h"

#include <app/util/attribute-storage.h>
using namespace ::chip::app::Clusters;
using namespace ::chip;

#include "EndpointApi.h"

#include <app/util/af-types.h>


#define DESCRIPTION_ATTR_ARRAY_LEN  (254) //max
#define NODE_LABEL_SIZE             (32)


// Declare On/Off cluster attributes
const EmberAfAttributeMetadata levelAttrs[] = {
    { //onOff attribute
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = LevelControl::Attributes::CurrentLevel::Id,
        .size = 1,
        .attributeType = ZAP_TYPE(INT8U),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    { //end?
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = 0xFFFD,
        .size = 2,
        .attributeType = ZAP_TYPE(INT16U),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
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
    { //device list
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = Descriptor::Attributes::DeviceTypeList::Id,
        .size = DESCRIPTION_ATTR_ARRAY_LEN,
        .attributeType = ZAP_TYPE(ARRAY),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    { //server list
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = Descriptor::Attributes::ServerList::Id,
        .size = DESCRIPTION_ATTR_ARRAY_LEN,
        .attributeType = ZAP_TYPE(ARRAY),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    { //client list
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = Descriptor::Attributes::ClientList::Id,
        .size = DESCRIPTION_ATTR_ARRAY_LEN,
        .attributeType = ZAP_TYPE(ARRAY),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    { //parts list
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = Descriptor::Attributes::PartsList::Id,
        .size = DESCRIPTION_ATTR_ARRAY_LEN,
        .attributeType = ZAP_TYPE(ARRAY),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    { //end?
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = 0xFFFD,
        .size = 2,
        .attributeType = ZAP_TYPE(INT16U),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
};

// Declare Bridged Device Basic Information cluster attributes
const EmberAfAttributeMetadata bridgedDeviceBasicAttrs[] = {
    { //node label
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = BridgedDeviceBasicInformation::Attributes::NodeLabel::Id,
        .size = NODE_LABEL_SIZE,
        .attributeType = ZAP_TYPE(CHAR_STRING),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    { //reachable
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = BridgedDeviceBasicInformation::Attributes::Reachable::Id,
        .size = 1,
        .attributeType = ZAP_TYPE(BOOLEAN),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
    { //end?
        .defaultValue = ZAP_EMPTY_DEFAULT(),
        .attributeId = 0xFFFD,
        .size = 2,
        .attributeType = ZAP_TYPE(INT16U),
        .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    },
};