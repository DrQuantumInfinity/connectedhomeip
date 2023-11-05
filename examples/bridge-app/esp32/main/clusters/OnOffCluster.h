#pragma once
#include "Cluster.h"
#include <app/util/attribute-metadata.h>

#include <app/util/attribute-storage.h>
//       util/common.h
//           -> util/af.h
//           -> util/config.h
//           -> zap-generated/endpoint_config.h
#include <app-common/zap-generated/callback.h>
#include <app/util/endpoint-config-defines.h>
#include <app/util/af-types.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;

class OnOffCluster : public Cluster
{
public:
    OnOffCluster(void) { _id = OnOff::Id; }
    bool _isOn;
    void SetOn(bool on, uint16_t index);
    EmberAfStatus Write(chip::AttributeId attributeId, uint8_t * buffer) override;
    EmberAfStatus Read(chip::AttributeId attributeId, uint8_t * buffer, uint16_t maxReadLength) override;

    static constexpr EmberAfAttributeMetadata onOffAttrs[] = {
        { // onOff attribute
          .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = OnOff::Attributes::OnOff::Id,
          .size          = 1,
          .attributeType = ZAP_TYPE(BOOLEAN),
          .mask          = ZAP_ATTRIBUTE_MASK(WRITABLE) | ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
        { // end?
          .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = 0xFFFD,
          .size          = 2,
          .attributeType = ZAP_TYPE(INT16U),
          .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
    };
    
    static constexpr CommandId incomingCommands[] = {
        app::Clusters::OnOff::Commands::Off::Id,
        app::Clusters::OnOff::Commands::On::Id,
        app::Clusters::OnOff::Commands::Toggle::Id,
        app::Clusters::OnOff::Commands::OffWithEffect::Id,
        app::Clusters::OnOff::Commands::OnWithRecallGlobalScene::Id,
        app::Clusters::OnOff::Commands::OnWithTimedOff::Id,
        kInvalidCommandId,
    };

    // static constexpr const EmberAfGenericClusterFunction bs(EmberAfGenericClusterFunction any) { return any; }

// typedef const Control_base (*Control_base_func)(void);

// constexpr Control_base_func Control_base_arr[] = {&Control_base::high_clock};


    static constexpr EmberAfCluster cluster = { .clusterId            = OnOff::Id,
                                                .attributes           = onOffAttrs,
                                                .attributeCount       = ArraySize(onOffAttrs),
                                                .clusterSize          = 0,
                                                .mask                 = ZAP_CLUSTER_MASK(SERVER) | ZAP_CLUSTER_MASK(INIT_FUNCTION) | ZAP_CLUSTER_MASK(SHUTDOWN_FUNCTION),
                                                .functions            = nullptr,
                                                .acceptedCommandList  = incomingCommands,
                                                .generatedCommandList = nullptr,
                                                .eventList            = nullptr,
                                                .eventCount           = 0 };
    //                                                 const  EmberAfGenericClusterFunction chipFuncArrayOnOffServer[2] = {     
    //       // static_cast<  EmberAfGenericClusterFunction>    (emberAfOnOffClusterServerInitCallback),
    //       // static_cast<  EmberAfGenericClusterFunction>    (MatterOnOffClusterServerShutdownCallback),                                                  
    //     (EmberAfGenericClusterFunction) emberAfOnOffClusterServerInitCallback,                                                     
    //     (EmberAfGenericClusterFunction) MatterOnOffClusterServerShutdownCallback,                                                  
    // };     
};