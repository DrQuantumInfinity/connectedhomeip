#include "LevelControlCluster.h"
#include "EndpointApi.h"

void LevelControlCluster::SetLevel(uint8_t level, uint16_t index)
{
    _level = level;
    EndpointReportChange(index, LevelControl::Id, LevelControl::Attributes::LevelControl::Id);
}

EmberAfStatus LevelControlCluster::Write(chip::AttributeId attributeId, uint8_t * buffer)
{
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    switch (attributeId)
    {
    case LevelControl::Attributes::LevelControl::Id:
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
    if (maxReadLength == 2)
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