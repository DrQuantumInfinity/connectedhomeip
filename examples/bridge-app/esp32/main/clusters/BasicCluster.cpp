#include "BasicCluster.h"
#include "EndpointApi.h"



void BasicCluster::SetReachable(bool reachable, uint16_t index ){
    _reachable = reachable;
    EndpointReportChange(index, BridgedDeviceBasicInformation::Id, BridgedDeviceBasicInformation::Attributes::Reachable::Id);
}
EmberAfStatus BasicCluster::Write(chip::AttributeId attributeId, uint8_t* buffer)
{
    return EMBER_ZCL_STATUS_FAILURE;
}

EmberAfStatus BasicCluster::Read(chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength)
{
    using namespace BridgedDeviceBasicInformation::Attributes;
    //TODO: add debug
//     ChipLogProgress(DeviceLayer, "HandleReadBridgedDeviceBasicAttribute: attrId=%" PRIu32 ", maxReadLength=%u", attributeId,
//                     maxReadLength);

    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;

    if ((attributeId == Reachable::Id) && (maxReadLength == 1))
    {
        *buffer = _reachable ? 1 : 0;
    }
    else if ((attributeId == NodeLabel::Id) && (maxReadLength == 32))
    {
        MutableByteSpan zclNameSpan(buffer, maxReadLength);
        MakeZclCharString(zclNameSpan, /*dev->GetName()*/"cat lol"); //TODO: get this from the info cluster
    }
    else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2))
    {
        uint16_t rev = ZCL_BRIDGED_DEVICE_BASIC_INFORMATION_CLUSTER_REVISION;
        memcpy(buffer, &rev, sizeof(rev));
    }
    else
    {
        status = EMBER_ZCL_STATUS_FAILURE;
    }

    return status;
}