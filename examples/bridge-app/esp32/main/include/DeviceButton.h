
#pragma once
#include "../clusters/DescriptorCluster.h"
#include "../clusters/OnOffCluster.h"
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
class DeviceButton;
typedef void (*DEVICE_BUTTON_WRITE_CALLBACK)(DeviceButton * deviceButton, ClusterId clusterId,
                                             const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer);

class DeviceButton : public Device
{
public:
    DeviceButton(const char * pName, const char * pLocation, DEVICE_BUTTON_WRITE_CALLBACK pfnWriteCallback);
    ~DeviceButton(void);
    void Toggle(void) { OnOffCluster.SetOn(!OnOffCluster._isOn, GetIndex()); }
    // OnOffCluster GetOnOffCluster(void) { return onOffCluster; }
    // DescriptorCluster GetDescriptorCluster(void) { return descriptorCluster; }

    // protected...
    DEVICE_LIGHT_WRITE_CALLBACK _pfnWriteCallback;

private:
    OnOffCluster onOffCluster;
    DescriptorCluster descriptorCluster;
    ENDPOINT_DATA _endpointData;
};
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/