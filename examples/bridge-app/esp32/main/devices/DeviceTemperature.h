
#pragma once
#include "DescriptorCluster.h"
#include "TempCluster.h"
#include "Device.h"
#include "EndpointApi.h"
#include <app/InteractionModelEngine.h>
#include <app/util/af-types.h>
using namespace ::chip;
/**************************************************************************
 *                                  Constants
 **************************************************************************/
/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/

class DeviceTemperature : public Device
{
public:
    DeviceTemperature(const char * pName, const char * pLocation, DEVICE_WRITE_CALLBACK pfnWriteCallback, float temp);
    ~DeviceTemperature(void);
    void UpdateTemp(float temp) { tempCluster.UpdateTemp(temp, GetIndex()); }

private:
    TempCluster tempCluster;
    DescriptorCluster descriptorCluster;
    ENDPOINT_DATA _endpointData;
};
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/