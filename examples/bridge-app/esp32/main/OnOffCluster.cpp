#include "OnOffCluster.h"
#include "EndpointApi.h"


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

// // // Declare On/Off cluster attributes
// static const EmberAfAttributeMetadata onOffAttrs[] = {
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

// const EmberAfCluster OnOffCluster::cluster={ 
//         .clusterId = OnOff::Id, 
//         .attributes = onOffAttrs, 
//         .attributeCount = ArraySize(onOffAttrs), 
//         .clusterSize = 0, 
//         .mask = ZAP_CLUSTER_MASK(SERVER), 
//         .functions = NULL, 
//         .acceptedCommandList = onOffIncomingCommands, 
//         .generatedCommandList = nullptr,
//         .eventList = nullptr,
//         .eventCount = 0
//     };

void OnOffCluster::SetOn(bool on, uint16_t index)
{
    _isOn = on;
    EndpointReportChange(index, OnOff::Id, OnOff::Attributes::OnOff::Id);
}

EmberAfStatus OnOffCluster::ProcessAttributeOnOff(chip::AttributeId attributeId, uint8_t* buffer)
{
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    switch (attributeId)
    {
    case OnOff::Attributes::OnOff::Id:  _isOn = (bool)buffer[0];   break;
    default:                            status = EMBER_ZCL_STATUS_FAILURE;      break;
    }
    return status;
}
