#pragma once
#include "DeviceLight.h"
#include <app/util/attribute-storage.h>
#include <app/util/attribute-metadata.h>

using namespace ::chip::app::Clusters;
using namespace ::chip;

#include "EndpointApi.h"

#include <app/util/af-types.h>

extern const EmberAfAttributeMetadata levelAttrs[2];
extern const EmberAfAttributeMetadata onOffAttrs[2];
extern const EmberAfAttributeMetadata descriptorAttrs[5];
extern const EmberAfAttributeMetadata bridgedDeviceBasicAttrs[3];
extern const CommandId onOffIncomingCommands[7];
