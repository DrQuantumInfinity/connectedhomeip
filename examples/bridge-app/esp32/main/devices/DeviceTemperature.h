
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
class DeviceTemperature;
typedef void (*DEVICE_TEMP_WRITE_CALLBACK)(DeviceTemperature * device, ClusterId clusterId,
                                             const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer);

class DeviceTemperature : public Device
{
public:
    DeviceTemperature(const char * pName, const char * pLocation, DEVICE_WRITE_CALLBACK pfnWriteCallback);
    ~DeviceTemperature(void);
    void UpdateTemp(int16_t temp) { tempCluster.UpdateTemp(temp, GetIndex()); }

    // // protected...
    // DEVICE_TEMP_WRITE_CALLBACK _pfnWriteCallback;

private:
    TempCluster tempCluster;
    DescriptorCluster descriptorCluster;
    ENDPOINT_DATA _endpointData;
};
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/