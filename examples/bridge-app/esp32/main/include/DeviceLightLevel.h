
#pragma once
#include "OnOffCluster.h"
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
    void SetOn(bool on);
    EmberAfStatus ProcessCluster(ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer);
    
    //protected...
    DEVICE_LIGHT_LEVEL_WRITE_CALLBACK _pfnWriteCallback;
    bool _isOn;
    
private:
    DataVersion _dataVersions[DEVICE_LIGHT_LEV_NUM_CLUSTERS];
    
    OnOffCluster onOffcluster;
};
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/