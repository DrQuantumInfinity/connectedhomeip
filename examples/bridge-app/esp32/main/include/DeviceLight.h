
#pragma once
#include "EndpointApi.h"

#include <app/util/af-types.h>
using namespace ::chip;
/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define DEVICE_LIGHT_NUM_CLUSTERS   (3) //ArraySize(bridgedLightClusters)
/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/
class DeviceLight //Extend base class
{
public:
    DeviceLight(const char* pName, const char* pLocation, GOOGLE_WRITE_CALLBACK pfnWriteCallback);
    void Dispose(void);
    //TODO: Add commands for turning the light on and off

private:
    DataVersion dataVersions[DEVICE_LIGHT_NUM_CLUSTERS];
};
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/