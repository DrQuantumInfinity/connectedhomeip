#include "OnOffCluster2.h"
#include "EndpointApi.h"

#include "esp_log.h"
#include <app-common/zap-generated/callback.h>
#include <lib/support/ZclString.h>
/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define ZCL_ON_OFF_CLUSTER_REVISION (4u)
static const char * TAG = "OnOffCluster";

const EmberAfGenericClusterFunction chipFuncArrayOnOffServer[] = {
    (EmberAfGenericClusterFunction) emberAfOnOffClusterServerInitCallback,
    (EmberAfGenericClusterFunction) MatterOnOffClusterServerShutdownCallback,
};

//constexpr const EmberAfAttributeMetadata generatedAttributes[] = {
//    /* Endpoint: 2, Cluster: On/Off (server) */
//    { ZAP_SIMPLE_DEFAULT(0x00), 0x00000000, 1, ZAP_TYPE(BOOLEAN), 0 }, /* OnOff */
//    { ZAP_SIMPLE_DEFAULT(0), 0x0000FFFC, 4, ZAP_TYPE(BITMAP32), 0 }, /* FeatureMap */
//    { ZAP_SIMPLE_DEFAULT(5), 0x0000FFFD, 2, ZAP_TYPE(INT16U), 0 }, /* ClusterRevision */
//};
static constexpr EmberAfAttributeMetadata attributes[] = {
    { // onOff attribute
        .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = OnOff::Attributes::OnOff::Id,
        .size          = 1,
        .attributeType = ZAP_TYPE(BOOLEAN),
        .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
    { // ClusterRevision attribute
        .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = 0xFFFD,
        .size          = 2,
        .attributeType = ZAP_TYPE(INT16U),
        .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
};

//constexpr const chip::CommandId generatedCommands[] = {
//    0x00000000 /* Off */,
//    0x00000001 /* On */,
//    0x00000002 /* Toggle */,
//};
static constexpr CommandId acceptedCommandList[] = {
    app::Clusters::OnOff::Commands::Off::Id,
    app::Clusters::OnOff::Commands::On::Id,
    app::Clusters::OnOff::Commands::Toggle::Id,
    app::Clusters::OnOff::Commands::OffWithEffect::Id,
    app::Clusters::OnOff::Commands::OnWithRecallGlobalScene::Id,
    app::Clusters::OnOff::Commands::OnWithTimedOff::Id,
    kInvalidCommandId,
};


//constexpr const EmberAfCluster generatedClusters[] = {
//    { 
//        /* Endpoint: 2, Cluster: On/Off (server) */ 
//        .clusterId = 0x00000006, 
//        .attributes = ZAP_ATTRIBUTE_INDEX(210), 
//        .attributeCount = 3, 
//        .clusterSize = 7, 
//        .mask = ZAP_CLUSTER_MASK(SERVER) | ZAP_CLUSTER_MASK(INIT_FUNCTION) | ZAP_CLUSTER_MASK(SHUTDOWN_FUNCTION), 
//        .functions = chipFuncArrayOnOffServer, 
//        .acceptedCommandList = ZAP_GENERATED_COMMANDS_INDEX( 59 ), 
//        .generatedCommandList = nullptr, 
//        .eventList = nullptr, 
//        .eventCount = 0, 
//    },
//};
static EmberAfCluster cluster = { 
    .clusterId            = OnOff::Id,
    .attributes           = attributes,
    .attributeCount       = ArraySize(attributes),
    .clusterSize          = 0, //Assigned dynamically upon GetObject()
    .mask                 = ZAP_CLUSTER_MASK(SERVER) | ZAP_CLUSTER_MASK(INIT_FUNCTION) | ZAP_CLUSTER_MASK(SHUTDOWN_FUNCTION),
    .functions            = chipFuncArrayOnOffServer,
    .acceptedCommandList  = acceptedCommandList,
    .generatedCommandList = nullptr,
    .eventList            = nullptr,
    .eventCount           = 0 
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
const EmberAfCluster* ClusterOnOffGetObject(void)
{
    if (cluster.clusterSize == 0) //only perform this the first time
    {
        for (int i = 0; i < cluster.attributeCount; i++)
        {
            cluster.clusterSize += cluster.attributes[i].size;
        }
    }

    return &cluster;
}

//TODO: do a malloc/free implementation to store local variables.
/*
void OnOffCluster::SetOn(bool on, uint16_t index)
{
    _isOn = on;
    EndpointReportChange(index, OnOff::Id, OnOff::Attributes::OnOff::Id);
}

EmberAfStatus OnOffCluster::Write(chip::AttributeId attributeId, uint8_t* buffer)
{
    ESP_LOGI(TAG, "OnOffCluster Write called");
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    switch (attributeId)
    {
        case OnOff::Attributes::OnOff::Id:  _isOn = (bool)buffer[0];   break;
        default:                            status = EMBER_ZCL_STATUS_FAILURE;      break;
    }
    return status;
}

EmberAfStatus OnOffCluster::Read(chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength){

    ESP_LOGI(TAG, "OnOffCluster Read called");
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    if ((attributeId == OnOff::Attributes::OnOff::Id) && (maxReadLength == 1))
    {
        *buffer = _isOn ? 1 : 0;
    }
    else if ((attributeId == OnOff::Attributes::ClusterRevision::Id) && (maxReadLength == 2))
    {
        uint16_t rev = ZCL_ON_OFF_CLUSTER_REVISION;
        memcpy(buffer, &rev, sizeof(rev));
    }
    else
    {
        status = EMBER_ZCL_STATUS_FAILURE;
    }
    return status;
}
*/
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
