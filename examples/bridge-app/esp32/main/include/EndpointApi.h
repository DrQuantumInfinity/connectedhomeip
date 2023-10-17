
#pragma once
#include "Device.h"
#include "DeviceLight.h"
#include "DeviceCallbacks.h"
#include <span>

using namespace ::chip;
// using namespace ::chip::DeviceManager;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;
/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define ENDPOINT_NAME_LENGTH        (32)
#define ENDPOINT_LOCATION_LENGTH    (32)
/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/
typedef EmberAfStatus (*GOOGLE_READ_CALLBACK)(EndpointId endpoint, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer, uint16_t maxReadLength);
typedef EmberAfStatus (*GOOGLE_WRITE_CALLBACK)(EndpointId endpoint, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer);
//typedef EmberAfStatus (*GOOGLE_INSTANT_ACTION_CALLBACK)(app::CommandHandler* commandObj, const app::ConcreteCommandPath & commandPath, const Actions::Commands::InstantAction::DecodableType & commandData);
typedef struct
{
    Device * dev;

    uint16_t index;
    char name[ENDPOINT_NAME_LENGTH];
    char location[ENDPOINT_LOCATION_LENGTH];
    const EmberAfEndpointType* ep;
    const Span<const EmberAfDeviceType> & deviceTypeList;
    const Span<DataVersion> & dataVersionStorage;
    chip::EndpointId parentEndpointId;
}ENDPOINT_DATA;

    
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
void AddDeviceLightEndpointScheduler(DEVICE_LIGHT* pDeviceData);
int AddDeviceEndpoint(Device * dev, const EmberAfEndpointType * ep, const Span<const EmberAfDeviceType> & deviceTypeList,
                      const Span<DataVersion> & dataVersionStorage, chip::EndpointId parentEndpointId);
CHIP_ERROR RemoveDeviceEndpoint(Device * dev);
void InitDevMgr();

void HandleDeviceStatusChanged(Device * dev, Device::Changed_t itemChangedMask);
                                                