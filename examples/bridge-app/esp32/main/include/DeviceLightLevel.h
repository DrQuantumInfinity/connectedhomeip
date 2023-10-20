
#pragma once
#include "../clusters/OnOffCluster.h"
#include "../clusters/BasicCluster.h"
#include "../clusters/LevelControlCluster.h"
#include "../clusters/DescriptorCluster.h"
#include "EndpointApi.h"
#include "Device.h"
#include <app/InteractionModelEngine.h>
#include <app/util/af-types.h>
using namespace ::chip;
/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define DEVICE_LIGHT_LEV_NUM_CLUSTERS   (4) //ArraySize(bridgedLightClusters)
/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/
class DeviceLightLevel; //forward declare
typedef void (*DEVICE_LIGHT_LEVEL_WRITE_CALLBACK)(DeviceLightLevel *deviceLight, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer);

class DeviceLightLevel : public Device
{
public:
    DeviceLightLevel(const char* pName, const char* pLocation, DEVICE_LIGHT_LEVEL_WRITE_CALLBACK pfnWriteCallback);
    ~DeviceLightLevel();
    // EmberAfStatus ReadCluster(ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer, uint16_t maxReadLength);
    // EmberAfStatus WriteCluster(ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer);
    
    //protected...
    DEVICE_LIGHT_LEVEL_WRITE_CALLBACK _pfnWriteCallback;
    // bool _isOn;
    
    OnOffCluster onOffCluster;
    LevelControlCluster levelControlCluster;
    DescriptorCluster descriptorCluster;
    // BasicCluster basicCluster;

private:
    ENDPOINT_DATA _endpointData;
};
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/