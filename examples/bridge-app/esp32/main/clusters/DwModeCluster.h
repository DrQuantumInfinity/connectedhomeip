#pragma once
#include "Cluster.h"
#include <app/util/attribute-metadata.h>

#include <app/util/attribute-storage.h>

#include <app/util/af-types.h>
#include "dwMode.h"

using namespace ::chip;
using namespace ::chip::app::Clusters;


class DwModeCluster : public Cluster
{
public:
    DwModeCluster(void) { _id = DishwasherMode::Id; }
    DishwasherMode::ModeTag _mode = DishwasherMode::ModeTag::kNormal;
    void SetMode(DishwasherMode::ModeTag newMode, uint16_t index);
    EmberAfStatus Write(chip::AttributeId attributeId, uint8_t * buffer) override;
    EmberAfStatus Read(chip::AttributeId attributeId, uint8_t * buffer, uint16_t maxReadLength) override;
};

EmberAfCluster ClusterDwModeGetObject(void);
