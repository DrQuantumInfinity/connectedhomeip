#pragma once
#include <app/util/attribute-metadata.h>
#include <app/util/attribute-storage.h>
#include <app/util/af-types.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;

class Cluster{
    public:
        virtual EmberAfStatus Write(chip::AttributeId attributeId, uint8_t* buffer);
        virtual EmberAfStatus Read(chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength);
};