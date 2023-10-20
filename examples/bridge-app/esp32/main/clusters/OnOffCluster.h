#pragma once
#include <app/util/attribute-metadata.h>
#include "Cluster.h"

#include <app/util/attribute-storage.h>

#include <app/util/af-types.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;

static const CommandId onOffIncomingCommands[] = {
    app::Clusters::OnOff::Commands::Off::Id,
    app::Clusters::OnOff::Commands::On::Id,
    app::Clusters::OnOff::Commands::Toggle::Id,
    app::Clusters::OnOff::Commands::OffWithEffect::Id,
    app::Clusters::OnOff::Commands::OnWithRecallGlobalScene::Id,
    app::Clusters::OnOff::Commands::OnWithTimedOff::Id,
    kInvalidCommandId,
};


class OnOffCluster : Cluster {
    public:
    bool _isOn;
    void SetOn(bool on, uint16_t index);
    EmberAfStatus Write(chip::AttributeId attributeId, uint8_t* buffer);
    EmberAfStatus Read(chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength);
    
    // Declare On/Off cluster attributes

    static constexpr EmberAfAttributeMetadata onOffAttrs[] = {
        { //onOff attribute
            .defaultValue = ZAP_EMPTY_DEFAULT(),
            .attributeId = OnOff::Attributes::OnOff::Id,
            .size = 1,
            .attributeType = ZAP_TYPE(BOOLEAN),
            .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
        },
        { //end?
            .defaultValue = ZAP_EMPTY_DEFAULT(),
            .attributeId = 0xFFFD,
            .size = 2,
            .attributeType = ZAP_TYPE(INT16U),
            .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
        },
    };

    static constexpr EmberAfCluster cluster={ 
            .clusterId = OnOff::Id, 
            .attributes = onOffAttrs, 
            .attributeCount = ArraySize(onOffAttrs), 
            .clusterSize = 0, 
            .mask = ZAP_CLUSTER_MASK(SERVER), 
            .functions = NULL, 
            .acceptedCommandList = onOffIncomingCommands, 
            .generatedCommandList = nullptr,
            .eventList = nullptr,
            .eventCount = 0
        };
    // Declare On/Off cluster attributes

    // static inline const EmberAfAttributeMetadata onOffAttrs[] = {
    //     { //onOff attribute
    //         .defaultValue = ZAP_EMPTY_DEFAULT(),
    //         .attributeId = OnOff::Attributes::OnOff::Id,
    //         .size = 1,
    //         .attributeType = ZAP_TYPE(BOOLEAN),
    //         .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    //     },
    //     { //end?
    //         .defaultValue = ZAP_EMPTY_DEFAULT(),
    //         .attributeId = 0xFFFD,
    //         .size = 2,
    //         .attributeType = ZAP_TYPE(INT16U),
    //         .mask = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE)
    //     },
    // };

    // static inline const EmberAfCluster cluster={ 
    //         .clusterId = OnOff::Id, 
    //         .attributes = onOffAttrs, 
    //         .attributeCount = ArraySize(onOffAttrs), 
    //         .clusterSize = 0, 
    //         .mask = ZAP_CLUSTER_MASK(SERVER), 
    //         .functions = NULL, 
    //         .acceptedCommandList = onOffIncomingCommands, 
    //         .generatedCommandList = nullptr,
    //         .eventList = nullptr,
    //         .eventCount = 0
    //     };
};