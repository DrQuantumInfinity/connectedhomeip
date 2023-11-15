#include "LevelControlCluster2.h"
#include "EndpointApi.h"

#include <app-common/zap-generated/callback.h>
#include <app/util/attribute-storage.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;
/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define ZCL_LEVEL_CLUSTER_REVISION (5u)
static const char * TAG = "LevelControlCluster";

const EmberAfGenericClusterFunction chipFuncArrayLevelControlServer[] = {
    (EmberAfGenericClusterFunction) emberAfLevelControlClusterServerInitCallback,
    (EmberAfGenericClusterFunction) MatterLevelControlClusterServerShutdownCallback,
};

//constexpr const EmberAfAttributeMetadata generatedAttributes[] = {
//    /* Endpoint: 2, Cluster: Level Control (server) */ 
//    { ZAP_SIMPLE_DEFAULT(0x00), 0x00000000, 1, ZAP_TYPE(INT8U), ZAP_ATTRIBUTE_MASK(TOKENIZE) | ZAP_ATTRIBUTE_MASK(NULLABLE) }, /* CurrentLevel */
//    { ZAP_SIMPLE_DEFAULT(0x0000), 0x00000001, 2, ZAP_TYPE(INT16U), 0 }, /* RemainingTime */
//    { ZAP_SIMPLE_DEFAULT(0x01), 0x00000002, 1, ZAP_TYPE(INT8U), 0 }, /* MinLevel */
//    { ZAP_SIMPLE_DEFAULT(0xFE), 0x00000003, 1, ZAP_TYPE(INT8U), 0 }, /* MaxLevel */
//    { ZAP_SIMPLE_DEFAULT(0x0000), 0x00000004, 2, ZAP_TYPE(INT16U), 0 }, /* CurrentFrequency */
//    { ZAP_SIMPLE_DEFAULT(0x0000), 0x00000005, 2, ZAP_TYPE(INT16U), 0 }, /* MinFrequency */
//    { ZAP_SIMPLE_DEFAULT(0x0000), 0x00000006, 2, ZAP_TYPE(INT16U), 0 }, /* MaxFrequency */
//    { ZAP_MIN_MAX_DEFAULTS_INDEX(2), 0x0000000F, 1, ZAP_TYPE(BITMAP8), ZAP_ATTRIBUTE_MASK(MIN_MAX) | ZAP_ATTRIBUTE_MASK(WRITABLE) }, /* Options */
//    { ZAP_SIMPLE_DEFAULT(0x0000), 0x00000010, 2, ZAP_TYPE(INT16U), ZAP_ATTRIBUTE_MASK(WRITABLE) }, /* OnOffTransitionTime */
//    { ZAP_SIMPLE_DEFAULT(0xFE), 0x00000011, 1, ZAP_TYPE(INT8U), ZAP_ATTRIBUTE_MASK(WRITABLE) | ZAP_ATTRIBUTE_MASK(NULLABLE) }, /* OnLevel */
//    { ZAP_EMPTY_DEFAULT(), 0x00000012, 2, ZAP_TYPE(INT16U), ZAP_ATTRIBUTE_MASK(WRITABLE) | ZAP_ATTRIBUTE_MASK(NULLABLE) }, /* OnTransitionTime */
//    { ZAP_EMPTY_DEFAULT(), 0x00000013, 2, ZAP_TYPE(INT16U), ZAP_ATTRIBUTE_MASK(WRITABLE) | ZAP_ATTRIBUTE_MASK(NULLABLE) }, /* OffTransitionTime */
//    { ZAP_EMPTY_DEFAULT(), 0x00000014, 1, ZAP_TYPE(INT8U), ZAP_ATTRIBUTE_MASK(WRITABLE) | ZAP_ATTRIBUTE_MASK(NULLABLE) }, /* DefaultMoveRate */
//    { ZAP_SIMPLE_DEFAULT(255), 0x00004000, 1, ZAP_TYPE(INT8U), ZAP_ATTRIBUTE_MASK(TOKENIZE) | ZAP_ATTRIBUTE_MASK(WRITABLE) | ZAP_ATTRIBUTE_MASK(NULLABLE) }, /* StartUpCurrentLevel */
//    { ZAP_SIMPLE_DEFAULT(3), 0x0000FFFC, 4, ZAP_TYPE(BITMAP32), 0 }, /* FeatureMap */
//    { ZAP_SIMPLE_DEFAULT(5), 0x0000FFFD, 2, ZAP_TYPE(INT16U), 0 }, /* ClusterRevision */
//};
static constexpr EmberAfAttributeMetadata attributes[] = {
    {   .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = LevelControl::Attributes::CurrentLevel::Id,
        .size          = 1,
        .attributeType = ZAP_TYPE(INT8U),
        .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) 
    },
    {   .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = LevelControl::Attributes::MinLevel::Id,
        .size          = 1,
        .attributeType = ZAP_TYPE(INT8U),
        .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) 
    },
    {   .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = LevelControl::Attributes::MaxLevel::Id,
        .size          = 1,
        .attributeType = ZAP_TYPE(INT8U),
        .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) 
    },
    {   .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = LevelControl::Attributes::FeatureMap::Id,
        .size          = 4,
        .attributeType = ZAP_TYPE(BITMAP32),
        .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) 
    },
    {   .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = 0xFFFD,
        .size          = 2,
        .attributeType = ZAP_TYPE(INT16U),
        .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) 
    },
};

//constexpr const chip::CommandId generatedCommands[] = {
//  /* Endpoint: 2, Cluster: Level Control (server) */
//  /*   AcceptedCommandList (index=63) */
//  0x00000000 /* MoveToLevel */,
//  0x00000001 /* Move */,
//  0x00000002 /* Step */,
//  0x00000003 /* Stop */,
//  0x00000004 /* MoveToLevelWithOnOff */,
//  0x00000005 /* MoveWithOnOff */,
//  0x00000006 /* StepWithOnOff */,
//  0x00000007 /* StopWithOnOff */,
//  chip::kInvalidCommandId /* end of list */,
//}
static constexpr CommandId acceptedCommandList[] = {
    app::Clusters::LevelControl::Commands::MoveToLevel::Id,
    app::Clusters::LevelControl::Commands::Move::Id,
    app::Clusters::LevelControl::Commands::Step::Id,
    app::Clusters::LevelControl::Commands::Stop::Id,
    app::Clusters::LevelControl::Commands::MoveToLevelWithOnOff::Id,
    app::Clusters::LevelControl::Commands::MoveWithOnOff::Id,
    app::Clusters::LevelControl::Commands::StepWithOnOff::Id,
    app::Clusters::LevelControl::Commands::StopWithOnOff::Id,
    app::Clusters::LevelControl::Commands::MoveToClosestFrequency::Id,
    kInvalidCommandId,
};

//constexpr const EmberAfCluster generatedClusters[] = {
//    { 
//        /* Endpoint: 2, Cluster: Level Control (server) */ 
//        .clusterId = 0x00000008, 
//        .attributes = ZAP_ATTRIBUTE_INDEX(213), 
//        .attributeCount = 16, 
//        .clusterSize = 27, 
//        .mask = ZAP_CLUSTER_MASK(SERVER) | ZAP_CLUSTER_MASK(INIT_FUNCTION) | ZAP_CLUSTER_MASK(SHUTDOWN_FUNCTION), 
//        .functions = chipFuncArrayLevelControlServer, 
//        .acceptedCommandList = ZAP_GENERATED_COMMANDS_INDEX( 63 ), 
//        .generatedCommandList = nullptr, 
//        .eventList = nullptr, 
//        .eventCount = 0, 
//    },
//};
static EmberAfCluster cluster = { 
    .clusterId            = LevelControl::Id,
    .attributes           = attributes,
    .attributeCount       = ArraySize(attributes),
    .clusterSize          = 0, //Assigned dynamically upon GetObject()
    .mask                 = ZAP_CLUSTER_MASK(SERVER) | ZAP_CLUSTER_MASK(INIT_FUNCTION) | ZAP_CLUSTER_MASK(SHUTDOWN_FUNCTION), 
    .functions            = chipFuncArrayLevelControlServer,
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
const EmberAfCluster* ClusterLevelControlGetObject(void)
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
void LevelControlCluster::SetLevel(uint8_t level, uint16_t index)
{
    _level = level;
    EndpointReportChange(index, LevelControl::Id, LevelControl::Attributes::CurrentLevel::Id);
}

EmberAfStatus LevelControlCluster::Write(chip::AttributeId attributeId, uint8_t * buffer)
{
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    switch (attributeId)
    {
    case LevelControl::Attributes::CurrentLevel::Id:
        _level = (uint8_t) buffer[0];
        break;
    default:
        status = EMBER_ZCL_STATUS_FAILURE;
        break;
    }
    return status;
}

EmberAfStatus LevelControlCluster::Read(chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength){
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    if ((attributeId == LevelControl::Attributes::CurrentLevel::Id) && (maxReadLength == 1))
    {
        *buffer = _level;
    }
    else if ((attributeId == LevelControl::Attributes::MinLevel::Id) && (maxReadLength == 1))
    {
        *buffer = _minLevel;
    }
    else if ((attributeId == LevelControl::Attributes::MaxLevel::Id) && (maxReadLength == 1))
    {
        *buffer = _maxLevel;
    }
    else if ((attributeId == LevelControl::Attributes::ClusterRevision::Id) && (maxReadLength == 2))
    {
        uint16_t rev = ZCL_LEVEL_CLUSTER_REVISION;
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
