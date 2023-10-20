
#pragma once
#include "Device.h"
#include "EndpointApi.h"
#include "../clusters/DescriptorCluster.h"
#include "../clusters/OnOffCluster.h"
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
class DeviceLight;
typedef void (*DEVICE_LIGHT_WRITE_CALLBACK)(DeviceLight *deviceLight, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer);

class DeviceLight : public Device
{
public:
    DeviceLight(const char* pName, const char* pLocation, DEVICE_LIGHT_WRITE_CALLBACK pfnWriteCallback);
    ~DeviceLight(void);
    // void SetOn(bool on);

    OnOffCluster onOffCluster;
    DescriptorCluster descriptorCluster;
    //protected...
    DEVICE_LIGHT_WRITE_CALLBACK _pfnWriteCallback;
    // bool _isOn;

private:
    ENDPOINT_DATA _endpointData;
};
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/