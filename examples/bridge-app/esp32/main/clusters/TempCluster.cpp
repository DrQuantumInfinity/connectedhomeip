#include "TempCluster.h"
#include "EndpointApi.h"

#include <app/util/attribute-storage.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;

EmberAfStatus TempCluster::Write(chip::AttributeId attributeId, uint8_t * buffer)
{
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    switch (attributeId)
    {
    case TemperatureMeasurement::Attributes::MeasuredValue::Id:
        memcpy(&_temp, buffer, sizeof(_temp));
        break;
    default:
        status = EMBER_ZCL_STATUS_FAILURE;
        break;
    }
    return status;
}

EmberAfStatus TempCluster::Read(chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength){
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    if (maxReadLength == 2)
    {
        memcpy(buffer, &_temp, maxReadLength);
    }
    else
    {
        status = EMBER_ZCL_STATUS_FAILURE;
    }
    return status;
}