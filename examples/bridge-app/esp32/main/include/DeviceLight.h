
#pragma once
#include "Device.h"
#include <app/util/af-types.h>
using namespace ::chip;
/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define DEVICE_LIGHT_NUM_CLUSTERS   (3)
/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/

typedef struct 
{
    Device* device;
    DataVersion dataVersions[DEVICE_LIGHT_NUM_CLUSTERS];
}DEVICE_LIGHT;

/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
DEVICE_LIGHT* DeviceLightAllocate(const char* pName, const char* pLocation, Device::DeviceCallback_fn pfnChangedCallback);
void DeviceLightFree(DEVICE_LIGHT* pDeviceLight);
const EmberAfEndpointType* DeviceLightGetEndpoint(void);
chip::Span<const EmberAfDeviceType> DeviceLightGetTypes(void);