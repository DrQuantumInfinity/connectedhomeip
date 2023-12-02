#pragma once
#include "Cluster.h"
#include <app/util/attribute-metadata.h>

#include <app/util/attribute-storage.h>

#include <app/util/af-types.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;

class OnOffCluster : public Cluster
{
public:
    OnOffCluster(void) { _id = OnOff::Id; }
    bool _isOn = false;
    void SetOn(bool on, uint16_t index);
    EmberAfStatus Write(chip::AttributeId attributeId, uint8_t * buffer) override;
    EmberAfStatus Read(chip::AttributeId attributeId, uint8_t * buffer, uint16_t maxReadLength) override;
};

 EmberAfCluster ClusterOnOffGetObject(void);