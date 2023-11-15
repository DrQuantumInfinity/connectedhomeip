#include "SwitchCluster2.h"
#include "EndpointApi.h"

#include <app-common/zap-generated/callback.h>
#include <app/util/attribute-storage.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;
/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define ZCL_LEVEL_CLUSTER_REVISION (5u)
static const char * TAG = "SwitchCluster";

//constexpr const EmberAfAttributeMetadata generatedAttributes[] = {
//   /* Endpoint: 2, Cluster: Switch (server) */
//   { ZAP_SIMPLE_DEFAULT(2), 0x00000000, 1, ZAP_TYPE(INT8U), 0 }, /* NumberOfPositions */
//   { ZAP_SIMPLE_DEFAULT(1), 0x00000001, 1, ZAP_TYPE(INT8U), 0 }, /* CurrentPosition */
//   { ZAP_SIMPLE_DEFAULT(2), 0x00000002, 1, ZAP_TYPE(INT8U), 0 }, /* MultiPressMax */
//   { ZAP_SIMPLE_DEFAULT(0), 0x0000FFFC, 4, ZAP_TYPE(BITMAP32), 0 }, /* FeatureMap */
//   { ZAP_SIMPLE_DEFAULT(1), 0x0000FFFD, 2, ZAP_TYPE(INT16U), 0 }, /* ClusterRevision */
//};
static constexpr EmberAfAttributeMetadata attributes[] = {
    {   .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = Switch::Attributes::NumberOfPositions::Id,
        .size          = 1,
        .attributeType = ZAP_TYPE(INT8U),
        .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) 
    },
    {   .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = Switch::Attributes::CurrentPosition::Id,
        .size          = 1,
        .attributeType = ZAP_TYPE(INT8U),
        .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) 
    },
    {   .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = Switch::Attributes::MultiPressMax::Id,
        .size          = 1,
        .attributeType = ZAP_TYPE(INT8U),
        .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) 
    },
    {   .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = Switch::Attributes::FeatureMap::Id,
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

//const expr const EmberAfCluster generatedClusters[] = {
//  {
//        /* Endpoint: 2, Cluster: Switch (server) */
//        .clusterId = 0x0000003B,
//        .attributes = ZAP_ATTRIBUTE_INDEX(235),
//        .attributeCount = 5,
//        .clusterSize = 9,
//        .mask = ZAP_CLUSTER_MASK(SERVER),
//        .functions = NULL,
//        .acceptedCommandList = nullptr,
//        .generatedCommandList = nullptr,
//        .eventList = nullptr,
//        .eventCount = 0,
//    },
//};
static EmberAfCluster cluster = { 
    .clusterId            = Switch::Id,
    .attributes           = attributes,
    .attributeCount       = ArraySize(attributes),
    .clusterSize          = 0, //Assigned dynamically upon GetObject()
    .mask                 = ZAP_CLUSTER_MASK(SERVER),
    .functions            = NULL,
    .acceptedCommandList  = nullptr,
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
const EmberAfCluster* ClusterSwitchGetObject(void)
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
