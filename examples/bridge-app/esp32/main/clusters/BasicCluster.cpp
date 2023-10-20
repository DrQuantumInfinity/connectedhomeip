#include "BasicCluster.h"
#include "EndpointApi.h"


static EmberAfStatus BasicCluster::Read(chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength)
{
    using namespace BridgedDeviceBasicInformation::Attributes;
    //TODO: add debug
//     ChipLogProgress(DeviceLayer, "HandleReadBridgedDeviceBasicAttribute: attrId=%" PRIu32 ", maxReadLength=%u", attributeId,
//                     maxReadLength);

    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;

    if ((attributeId == Reachable::Id) && (maxReadLength == 1))
    {
        *buffer = _isReachable() ? 1 : 0;
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
// void OnOffCluster::SetOn(bool on, uint16_t index)
// {
//     _isOn = on;
//     EndpointReportChange(index, OnOff::Id, OnOff::Attributes::OnOff::Id);
// }

// EmberAfStatus OnOffCluster::ProcessAttributeOnOff(chip::AttributeId attributeId, uint8_t* buffer)
// {
//     EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
//     switch (attributeId)
//     {
//     case OnOff::Attributes::OnOff::Id:  _isOn = (bool)buffer[0];   break;
//     default:                            status = EMBER_ZCL_STATUS_FAILURE;      break;
//     }
//     return status;
// }
