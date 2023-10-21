#pragma once
#include <app/util/af-types.h>
#include <app/util/attribute-metadata.h>
#include <app/util/attribute-storage.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;

class Cluster
{
public:
    virtual ~Cluster() = default;
    ClusterId _id;
    ClusterId GetId() { return _id; }
    virtual EmberAfStatus Write(chip::AttributeId attributeId, uint8_t * buffer) { return EMBER_ZCL_STATUS_FAILURE; }
    virtual EmberAfStatus Read(chip::AttributeId attributeId, uint8_t * buffer, uint16_t maxReadLength)
    {
        return EMBER_ZCL_STATUS_FAILURE;
    }
};