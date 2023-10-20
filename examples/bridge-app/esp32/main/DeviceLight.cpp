
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

// // Declare Cluster List for Bridged Light endpoint
// const CommandId onOffIncomingCommands[] = {
//     app::Clusters::OnOff::Commands::Off::Id,
//     app::Clusters::OnOff::Commands::On::Id,
//     app::Clusters::OnOff::Commands::Toggle::Id,
//     app::Clusters::OnOff::Commands::OffWithEffect::Id,
//     app::Clusters::OnOff::Commands::OnWithRecallGlobalScene::Id,
//     app::Clusters::OnOff::Commands::OnWithTimedOff::Id,
//     kInvalidCommandId,
// };

// // Declare On/Off cluster attributes
// const EmberAfAttributeMetadata onOffAttrs[] = {
//     { //onOff attribute
//         .defaultValue = ZAP_EMPTY_DEFAULT(),
//         .attributeId = OnOff::Attributes::OnOff::Id,
//         .size = 1,
//         .attributeType = ZAP_TYPE(BOOLEAN),
//         .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
//     },
//     { //end?
//         .defaultValue = ZAP_EMPTY_DEFAULT(),
//         .attributeId = 0xFFFD,
//         .size = 2,
//         .attributeType = ZAP_TYPE(INT16U),
//         .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
//     },
// };

// // Declare Descriptor cluster attributes
// const EmberAfAttributeMetadata descriptorAttrs[] = {
//     { //device list
//         .defaultValue = ZAP_EMPTY_DEFAULT(),
//         .attributeId = Descriptor::Attributes::DeviceTypeList::Id,
//         .size = DESCRIPTION_ATTR_ARRAY_LEN,
//         .attributeType = ZAP_TYPE(ARRAY),
//         .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
//     },
//     { //server list
//         .defaultValue = ZAP_EMPTY_DEFAULT(),
//         .attributeId = Descriptor::Attributes::ServerList::Id,
//         .size = DESCRIPTION_ATTR_ARRAY_LEN,
//         .attributeType = ZAP_TYPE(ARRAY),
//         .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
//     },
//     { //client list
//         .defaultValue = ZAP_EMPTY_DEFAULT(),
//         .attributeId = Descriptor::Attributes::ClientList::Id,
//         .size = DESCRIPTION_ATTR_ARRAY_LEN,
//         .attributeType = ZAP_TYPE(ARRAY),
//         .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
//     },
//     { //parts list
//         .defaultValue = ZAP_EMPTY_DEFAULT(),
//         .attributeId = Descriptor::Attributes::PartsList::Id,
//         .size = DESCRIPTION_ATTR_ARRAY_LEN,
//         .attributeType = ZAP_TYPE(ARRAY),
//         .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
//     },
//     { //end?
//         .defaultValue = ZAP_EMPTY_DEFAULT(),
//         .attributeId = 0xFFFD,
//         .size = 2,
//         .attributeType = ZAP_TYPE(INT16U),
//         .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
//     },
// };

// // Declare Bridged Device Basic Information cluster attributes
// const EmberAfAttributeMetadata bridgedDeviceBasicAttrs[] = {
//     { //node label
//         .defaultValue = ZAP_EMPTY_DEFAULT(),
//         .attributeId = BridgedDeviceBasicInformation::Attributes::NodeLabel::Id,
//         .size = NODE_LABEL_SIZE,
//         .attributeType = ZAP_TYPE(CHAR_STRING),
//         .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
//     },
//     { //reachable
//         .defaultValue = ZAP_EMPTY_DEFAULT(),
//         .attributeId = BridgedDeviceBasicInformation::Attributes::Reachable::Id,
//         .size = 1,
//         .attributeType = ZAP_TYPE(BOOLEAN),
//         .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
//     },
//     { //end?
//         .defaultValue = ZAP_EMPTY_DEFAULT(),
//         .attributeId = 0xFFFD,
//         .size = 2,
//         .attributeType = ZAP_TYPE(INT16U),
//         .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
//     },
// };


/* BRIDGED DEVICE ENDPOINT: contains the following clusters:
   - On/Off
   - Descriptor
   - Bridged Device Basic Information
*/
const EmberAfCluster bridgedLightClusters[] = {
    OnOffCluster::cluster,
    DescriptorCluster::cluster,
    BasicCluster::cluster,
    // { 
    //     .clusterId = OnOff::Id, 
    //     .attributes = onOffAttrs, 
    //     .attributeCount = ArraySize(onOffAttrs), 
    //     .clusterSize = 0, 
    //     .mask = ZAP_CLUSTER_MASK(SERVER), 
    //     .functions = NULL, 
    //     .acceptedCommandList = onOffIncomingCommands, 
    //     .generatedCommandList = nullptr,
    //     .eventList = nullptr,
    //     .eventCount = 0
    // },
    // { 
    //     .clusterId = Descriptor::Id, 
    //     .attributes = descriptorAttrs, 
    //     .attributeCount = ArraySize(descriptorAttrs), 
    //     .clusterSize = 0, 
    //     .mask = ZAP_CLUSTER_MASK(SERVER), 
    //     .functions = NULL, 
    //     .acceptedCommandList = nullptr, 
    //     .generatedCommandList = nullptr,
    //     .eventList = nullptr,
    //     .eventCount = 0
    // },
    // { 
    //     .clusterId = BridgedDeviceBasicInformation::Id, 
    //     .attributes = bridgedDeviceBasicAttrs, 
    //     .attributeCount = ArraySize(bridgedDeviceBasicAttrs), 
    //     .clusterSize = 0, 
    //     .mask = ZAP_CLUSTER_MASK(SERVER), 
    //     .functions = NULL, 
    //     .acceptedCommandList = nullptr, 
    //     .generatedCommandList = nullptr,
    //     .eventList = nullptr,
    //     .eventCount = 0
    // },
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
    // _clusters.push_back(std::make_unique<DescriptorCluster>());
    // _clusters.push_back(std::make_unique<OnOffCluster>());
    // AddCluster( &descriptorCluster, 0);
    // AddCluster( &onOffCluster, 1);
    // AddCluster(std::make_unique<DescriptorCluster>());
    // AddCluster(std::make_unique<OnOffCluster>());
    strcpy(endpointData.name, pName);
    strcpy(endpointData.location, pLocation);

    memcpy(&_endpointData, &endpointData, sizeof(_endpointData));
    EndpointAdd(&_endpointData);
    // basicCluster.SetReachable(true, GetIndex());
}

DeviceLight::~DeviceLight(void)
{
    free(_endpointData.pDataVersionStorage);
    EndpointRemove(GetIndex());
}
// void DeviceLight::SetOn(bool on)
// {
//     _isOn = on;
//     EndpointReportChange(GetIndex(), OnOff::Id, OnOff::Attributes::OnOff::Id);
// }
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
static EmberAfStatus GoogleReadCallback(void *pObject, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer, uint16_t maxReadLength)
{
    DeviceLight * pDeviceLight = (DeviceLight *) pObject;
    return pDeviceLight->ReadCluster(clusterId, attributeMetadata, buffer, maxReadLength);
    // DeviceLight *pDeviceLight = (DeviceLight*)pObject;
    // return ReadCluster(pDeviceLight, clusterId, attributeMetadata, buffer, maxReadLength);
}
// static EmberAfStatus ReadCluster(DeviceLight *pDeviceLight, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer, uint16_t maxReadLength)
// {
//     EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;
//     if (pDeviceLight->IsReachable())
//     {
//         switch (clusterId)
//         {
//         case BridgedDeviceBasicInformation::Id: status = ReadAttributeBasicInfo(pDeviceLight, attributeMetadata->attributeId, buffer, maxReadLength);   break;
//         case OnOff::Id:                         status = ReadAttributeOnOff(pDeviceLight, attributeMetadata->attributeId, buffer, maxReadLength);       break;
//         default:                                status = EMBER_ZCL_STATUS_SUCCESS;                                                                      break;
//         }
//     }
//     return status;
// }

// #include <lib/support/ZclString.h>
// #define ZCL_BRIDGED_DEVICE_BASIC_INFORMATION_CLUSTER_REVISION (2u)
// #define ZCL_ON_OFF_CLUSTER_REVISION (4u)
// static EmberAfStatus ReadAttributeBasicInfo(DeviceLight *pDeviceLight, chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength)
// {
//     using namespace BridgedDeviceBasicInformation::Attributes;
//     //TODO: add debug
// //     ChipLogProgress(DeviceLayer, "HandleReadBridgedDeviceBasicAttribute: attrId=%" PRIu32 ", maxReadLength=%u", attributeId,
// //                     maxReadLength);

//     EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;

//     if ((attributeId == Reachable::Id) && (maxReadLength == 1))
//     {
//         *buffer = pDeviceLight->IsReachable() ? 1 : 0;
//     }
//     else if ((attributeId == NodeLabel::Id) && (maxReadLength == 32))
//     {
//         MutableByteSpan zclNameSpan(buffer, maxReadLength);
//         MakeZclCharString(zclNameSpan, /*dev->GetName()*/"cat lol"); //TODO: get this from the info cluster
//     }
//     else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2))
//     {
//         uint16_t rev = ZCL_BRIDGED_DEVICE_BASIC_INFORMATION_CLUSTER_REVISION;
//         memcpy(buffer, &rev, sizeof(rev));
//     }
//     else
//     {
//         status = EMBER_ZCL_STATUS_FAILURE;
//     }

//     return status;
// }
// static EmberAfStatus ReadAttributeOnOff(DeviceLight *pDeviceLight, chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength)
// {
//     //TODO: debug
// //    ChipLogProgress(DeviceLayer, "HandleReadOnOffAttribute: attrId=%" PRIu32 ", maxReadLength=%u", attributeId, maxReadLength);
    
//     EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;

//     if ((attributeId == OnOff::Attributes::OnOff::Id) && (maxReadLength == 1))
//     {
//         *buffer = pDeviceLight->_isOn ? 1 : 0;
//     }
//     else if ((attributeId == OnOff::Attributes::ClusterRevision::Id) && (maxReadLength == 2))
//     {
//         uint16_t rev = ZCL_ON_OFF_CLUSTER_REVISION;
//         memcpy(buffer, &rev, sizeof(rev));
//     }
//     else
//     {
//         status = EMBER_ZCL_STATUS_FAILURE;
//     }
//     return status;
// }

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
// static EmberAfStatus GoogleWriteCallback(void *pObject, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer)
// {
//     DeviceLight *pDeviceLight = (DeviceLight*)pObject;
//     EmberAfStatus status = WriteCluster(pDeviceLight, clusterId, attributeMetadata, buffer);
//     if (pDeviceLight->_pfnWriteCallback)
//     {
//         pDeviceLight->_pfnWriteCallback(pDeviceLight, clusterId, attributeMetadata, buffer);
//     }
//     return status;
// }
// static EmberAfStatus WriteCluster(DeviceLight *pDeviceLight, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer)
// {
//     EmberAfStatus status = EMBER_ZCL_STATUS_FAILURE;
//     if (pDeviceLight->IsReachable())
//     {
//         switch (clusterId)
//         {
//         case OnOff::Id: status = WriteAttributeOnOff(pDeviceLight, attributeMetadata->attributeId, buffer); break;
//         default:        status = EMBER_ZCL_STATUS_SUCCESS;                                                  break;
//         }
//     }
//     return status;
// }
// static EmberAfStatus WriteAttributeOnOff(DeviceLight *pDeviceLight, chip::AttributeId attributeId, uint8_t* buffer)
// {
//     EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
//     switch (attributeId)
//     {
//     case OnOff::Attributes::OnOff::Id:  pDeviceLight->_isOn = (bool)buffer[0];   break;
//     default:                            status = EMBER_ZCL_STATUS_FAILURE;      break;
//     }
//     return status;
// }
