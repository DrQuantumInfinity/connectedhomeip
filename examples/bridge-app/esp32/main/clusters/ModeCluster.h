#pragma once
#include "Cluster.h"
#include <app/util/attribute-metadata.h>

#include <app/util/attribute-storage.h>

#include <app/util/af-types.h>
#include "mode.h"

using namespace ::chip;
using namespace ::chip::app::Clusters;


class TestModeCluster : public Cluster
{
public:
    TestModeCluster(void) { _id = TestMode::Id; }
    TestMode::ModeTag _mode = TestMode::ModeTag::kNormal;
    void SetMode(TestMode::ModeTag newMode, uint16_t index);
    EmberAfStatus Write(chip::AttributeId attributeId, uint8_t * buffer) override;
    EmberAfStatus Read(chip::AttributeId attributeId, uint8_t * buffer, uint16_t maxReadLength) override;
};

 EmberAfCluster ClusterTestModeGetObject(void);