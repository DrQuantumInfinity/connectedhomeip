
#include "DeviceLightRGB.h"
#include "EndpointApi.h"
#include "BasicCluster.h"
#include "DescriptorCluster.h"
#include "LevelControlCluster.h"
#include "ColourCluster.h"
#include "OnOffCluster.h"
#include "SerialTask.h"
#include <app/util/attribute-storage.h>

#include <app/util/endpoint-config-defines.h>
#include <app/util/af-types.h>
using namespace ::chip;
using namespace ::chip::app::Clusters;
/**************************************************************************
 *                                  Constants
 **************************************************************************/

const EmberAfGenericClusterFunction chipFuncArrayOnOffServer[] = {
    //   emberAfOnOffClusterServerInitCallback
    // static_cast<  EmberAfGenericClusterFunction>    (static_cast<void*emberAfOnOffClusterServerInitCallback)),
    // static_cast<  EmberAfGenericClusterFunction>    (MatterOnOffClusterServerShutdownCallback),
    (EmberAfGenericClusterFunction) emberAfOnOffClusterServerInitCallback,
    (EmberAfGenericClusterFunction) MatterOnOffClusterServerShutdownCallback,
};
 const EmberAfGenericClusterFunction chipFuncArrayLevelControlServer[] = {
    (EmberAfGenericClusterFunction) emberAfLevelControlClusterServerInitCallback,
    (EmberAfGenericClusterFunction) MatterLevelControlClusterServerShutdownCallback,
};

using namespace chip;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::ColorControl;
using chip::Protocols::InteractionModel::Status;

// void emberAfColorControlClusterServerInitCallback2(EndpointId endpoint)
// {
// #ifdef EMBER_AF_PLUGIN_COLOR_CONTROL_SERVER_TEMP
//     ColorControlServer::Instance().startUpColorTempCommand(endpoint);
// #endif // EMBER_AF_PLUGIN_COLOR_CONTROL_SERVER_TEMP
// }

// void MatterColorControlClusterServerShutdownCallback2(EndpointId endpoint)
// {
//     ChipLogProgress(Zcl, "Shuting down color control server cluster on endpoint %d", endpoint);
//     ColorControlServer::Instance().cancelEndpointTimerCallback(endpoint);
// }

 const EmberAfGenericClusterFunction chipFuncArrayColorControlServer[] = {
    (EmberAfGenericClusterFunction) emberAfColorControlClusterServerInitCallback,
    (EmberAfGenericClusterFunction) MatterColorControlClusterServerShutdownCallback,
};

// EmberAfCluster ofclust =  std::copy(OnOffCluster::cluster);

// ofclust.function        = chipFuncArrayOnOffServer;
// EmberAfCluster ofclust = OnOffCluster::cluster  ;
// // ofclust= OnOffCluster::cluster  ;//
//    ofclust->function   = chipFuncArrayOnOffServer;
// LevelControlCluster::cluster.function = chipFuncArrayLevelControlServer;
// ColourCluster::hsCluster.function       = EmberAfGenericClusterFunction;


     constexpr EmberAfAttributeMetadata onOffAttrs[] = {
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
    
     constexpr CommandId incomingCommands[] = {
        app::Clusters::OnOff::Commands::Off::Id,
        app::Clusters::OnOff::Commands::On::Id,
        app::Clusters::OnOff::Commands::Toggle::Id,
        app::Clusters::OnOff::Commands::OffWithEffect::Id,
        app::Clusters::OnOff::Commands::OnWithRecallGlobalScene::Id,
        app::Clusters::OnOff::Commands::OnWithTimedOff::Id,
        kInvalidCommandId,
    };


     constexpr EmberAfCluster ofcluster = { .clusterId            = OnOff::Id,
                                                .attributes           = onOffAttrs,
                                                .attributeCount       = ArraySize(onOffAttrs),
                                                .clusterSize          = 0,
                                                .mask                 = ZAP_CLUSTER_MASK(SERVER) | ZAP_CLUSTER_MASK(INIT_FUNCTION) | ZAP_CLUSTER_MASK(SHUTDOWN_FUNCTION),
                                                .functions            = chipFuncArrayOnOffServer,
                                                .acceptedCommandList  = incomingCommands,
                                                .generatedCommandList = nullptr,
                                                .eventList            = nullptr,
                                                .eventCount           = 0 };


    const EmberAfAttributeMetadata levelAttrs[] = {
        { .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = LevelControl::Attributes::CurrentLevel::Id,
          .size          = 1,
          .attributeType = ZAP_TYPE(INT8U),
          .mask          = ZAP_ATTRIBUTE_MASK(WRITABLE) | ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
        { .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = 0xFFFD,
          .size          = 2,
          .attributeType = ZAP_TYPE(INT16U),
          .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
    };
                                                                                                                   
    // static constexpr  EmberAfGenericClusterFunction chipFuncArrayLevelControlServer[] = {                          
    //     // (EmberAfGenericClusterFunction) emberAfLevelControlClusterServerInitCallback,                           
    //     // (EmberAfGenericClusterFunction) MatterLevelControlClusterServerShutdownCallback,                        
    // };                                                                                                             
   

    const CommandId incominglevCommands[] = {
        app::Clusters::LevelControl::Commands::MoveToLevel::Id,
        app::Clusters::LevelControl::Commands::Move::Id,
        app::Clusters::LevelControl::Commands::Step::Id,
        app::Clusters::LevelControl::Commands::Stop::Id,
        app::Clusters::LevelControl::Commands::MoveToLevelWithOnOff::Id,
        app::Clusters::LevelControl::Commands::MoveWithOnOff::Id,
        app::Clusters::LevelControl::Commands::StepWithOnOff::Id,
        app::Clusters::LevelControl::Commands::StopWithOnOff::Id,
        app::Clusters::LevelControl::Commands::MoveToClosestFrequency::Id,
        kInvalidCommandId,
    };

    static constexpr EmberAfCluster lccluster = { .clusterId            = LevelControl::Id,
                                                .attributes           = levelAttrs,
                                                .attributeCount       = ArraySize(levelAttrs),
                                                .clusterSize          = 27,
                                                .mask                 = ZAP_CLUSTER_MASK(SERVER)  | ZAP_CLUSTER_MASK(INIT_FUNCTION) | ZAP_CLUSTER_MASK(SHUTDOWN_FUNCTION),
                                                .functions            = chipFuncArrayLevelControlServer,
                                                .acceptedCommandList  = incominglevCommands,
                                                .generatedCommandList = nullptr,
                                                .eventList            = nullptr,
                                                .eventCount           = 0 };




    const EmberAfAttributeMetadata hsAttrs[] = {
        { .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = ColorControl::Attributes::CurrentHue::Id,
          .size          = 1,
          .attributeType = ZAP_TYPE(INT8U),
          .mask          = ZAP_ATTRIBUTE_MASK(WRITABLE) | ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
        { .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = ColorControl::Attributes::CurrentSaturation::Id,
          .size          = 1,
          .attributeType = ZAP_TYPE(INT8U),
          .mask          = ZAP_ATTRIBUTE_MASK(WRITABLE) | ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
        { .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = 0xFFFD,
          .size          = 2,
          .attributeType = ZAP_TYPE(INT16U),
          .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
    };

    const EmberAfAttributeMetadata tempAttrs[] = {
        { .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = ColorControl::Attributes::ColorTemperatureMireds::Id,
          .size          = 2,
          .attributeType = ZAP_TYPE(INT16U),
          .mask          = ZAP_ATTRIBUTE_MASK(WRITABLE) | ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
        { .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = 0xFFFD,
          .size          = 2,
          .attributeType = ZAP_TYPE(INT16U),
          .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
    };

    const EmberAfAttributeMetadata bothAttrs[] = {
        { .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = ColorControl::Attributes::CurrentHue::Id,
          .size          = 1,
          .attributeType = ZAP_TYPE(INT8U),
          .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
        { .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = ColorControl::Attributes::CurrentSaturation::Id,
          .size          = 1,
          .attributeType = ZAP_TYPE(INT8U),
          .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
        { .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = ColorControl::Attributes::ColorTemperatureMireds::Id,
          .size          = 2,
          .attributeType = ZAP_TYPE(INT16U),
          .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
        { .defaultValue  = ZAP_EMPTY_DEFAULT(),
          .attributeId   = 0xFFFD,
          .size          = 2,
          .attributeType = ZAP_TYPE(INT16U),
          .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
    };

    const CommandId incomingCommandsBoth[] = {
        app::Clusters::ColorControl::Commands::MoveToHue::Id,
        app::Clusters::ColorControl::Commands::MoveHue::Id,
        app::Clusters::ColorControl::Commands::StepHue::Id,
        app::Clusters::ColorControl::Commands::MoveToSaturation::Id,
        app::Clusters::ColorControl::Commands::MoveSaturation::Id,
        app::Clusters::ColorControl::Commands::StepSaturation::Id,
        app::Clusters::ColorControl::Commands::MoveToHueAndSaturation::Id,
        app::Clusters::ColorControl::Commands::MoveToColor::Id,
        app::Clusters::ColorControl::Commands::MoveColor::Id,
        app::Clusters::ColorControl::Commands::StepColor::Id,
        app::Clusters::ColorControl::Commands::MoveToColorTemperature::Id,
        app::Clusters::ColorControl::Commands::EnhancedMoveToHue::Id,
        app::Clusters::ColorControl::Commands::EnhancedMoveHue::Id,
        app::Clusters::ColorControl::Commands::EnhancedStepHue::Id,
        app::Clusters::ColorControl::Commands::EnhancedMoveToHueAndSaturation::Id,
        app::Clusters::ColorControl::Commands::ColorLoopSet::Id,
        app::Clusters::ColorControl::Commands::StopMoveStep::Id,
        app::Clusters::ColorControl::Commands::MoveColorTemperature::Id,
        app::Clusters::ColorControl::Commands::StepColorTemperature::Id,
        kInvalidCommandId,
    };

    const CommandId incomingCommandsHS[] = {
        app::Clusters::ColorControl::Commands::MoveToHue::Id,
        app::Clusters::ColorControl::Commands::MoveHue::Id,
        app::Clusters::ColorControl::Commands::StepHue::Id,
        app::Clusters::ColorControl::Commands::MoveToSaturation::Id,
        app::Clusters::ColorControl::Commands::MoveSaturation::Id,
        app::Clusters::ColorControl::Commands::StepSaturation::Id,
        app::Clusters::ColorControl::Commands::MoveToHueAndSaturation::Id,
        app::Clusters::ColorControl::Commands::MoveToColor::Id,
        app::Clusters::ColorControl::Commands::MoveColor::Id,
        app::Clusters::ColorControl::Commands::StepColor::Id,
        app::Clusters::ColorControl::Commands::EnhancedMoveToHue::Id,
        app::Clusters::ColorControl::Commands::EnhancedMoveHue::Id,
        app::Clusters::ColorControl::Commands::EnhancedStepHue::Id,
        app::Clusters::ColorControl::Commands::EnhancedMoveToHueAndSaturation::Id,
        app::Clusters::ColorControl::Commands::ColorLoopSet::Id,
        app::Clusters::ColorControl::Commands::StopMoveStep::Id,
        kInvalidCommandId,
    };

    const CommandId incomingCommandsTemp[] = {
        app::Clusters::ColorControl::Commands::MoveToColorTemperature::Id,
        app::Clusters::ColorControl::Commands::MoveColorTemperature::Id,
        app::Clusters::ColorControl::Commands::StepColorTemperature::Id,
        kInvalidCommandId,
    };
    // const EmberAfGenericClusterFunction chipFuncArrayColorControlServer[] = {                                     
        // (EmberAfGenericClusterFunction) emberAfColorControlClusterServerInitCallback,                                      
        // (EmberAfGenericClusterFunction) MatterColorControlClusterServerShutdownCallback,                                      
    // };   
    const EmberAfCluster hsCluster = { .clusterId            = ColorControl::Id,
                                                  .attributes           = hsAttrs,
                                                  .attributeCount       = ArraySize(hsAttrs),
                                                  .clusterSize          = 0,
                                                  .mask                 = ZAP_CLUSTER_MASK(SERVER) | ZAP_CLUSTER_MASK(INIT_FUNCTION) | ZAP_CLUSTER_MASK(SHUTDOWN_FUNCTION),
                                                  .functions            = chipFuncArrayColorControlServer,
                                                  .acceptedCommandList  = incomingCommandsHS,
                                                  .generatedCommandList = nullptr,
                                                  .eventList            = nullptr,
                                                  .eventCount           = 0 };

    static constexpr EmberAfCluster tempCluster = { .clusterId            = ColorControl::Id,
                                                    .attributes           = tempAttrs,
                                                    .attributeCount       = ArraySize(tempAttrs),
                                                    .clusterSize          = 0,
                                                    .mask                 = ZAP_CLUSTER_MASK(SERVER) | ZAP_CLUSTER_MASK(INIT_FUNCTION) | ZAP_CLUSTER_MASK(SHUTDOWN_FUNCTION),
                                                    .functions            = chipFuncArrayColorControlServer,
                                                    .acceptedCommandList  = incomingCommandsTemp,
                                                    .generatedCommandList = nullptr,
                                                    .eventList            = nullptr,
                                                    .eventCount           = 0 };

    static constexpr EmberAfCluster bothcluster = { .clusterId            = ColorControl::Id,
                                                    .attributes           = bothAttrs,
                                                    .attributeCount       = ArraySize(bothAttrs),
                                                    .clusterSize          = 0,
                                                    .mask                 = ZAP_CLUSTER_MASK(SERVER) | ZAP_CLUSTER_MASK(INIT_FUNCTION) | ZAP_CLUSTER_MASK(SHUTDOWN_FUNCTION),
                                                    .functions            = chipFuncArrayColorControlServer,
                                                    .acceptedCommandList  = incomingCommandsBoth,
                                                    .generatedCommandList = nullptr,
                                                    .eventList            = nullptr,
                                                    .eventCount           = 0 };



const EmberAfCluster bridgedClusters[] = {
    BasicCluster::cluster,
    DescriptorCluster::cluster,
    // OnOffCluster::cluster,
    ofcluster,
    // LevelControlCluster::cluster,
    lccluster,
    // ColourCluster::hsCluster,
    hsCluster
};

// Declare Bridged Light endpoint
const EmberAfEndpointType bridgedEndpoint = { 
    .cluster      = bridgedClusters,
    .clusterCount = ArraySize(bridgedClusters),
    .endpointSize = 0 
};

// (taken from chip-devices.xml)
#define DEVICE_TYPE_BRIDGED_NODE 0x0013
// Device Version for dynamic endpoints:
#define DEVICE_VERSION_DEFAULT 1
const EmberAfDeviceType bridgedDeviceTypes[] = {
    { .deviceId = 0x0101, .deviceVersion = DEVICE_VERSION_DEFAULT },
    { .deviceId = DEVICE_TYPE_BRIDGED_NODE, .deviceVersion = DEVICE_VERSION_DEFAULT }
};
/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
/**************************************************************************
 *                                  Variables
 **************************************************************************/
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
DeviceLightRGB::DeviceLightRGB(const char* pName, const char* pLocation, DEVICE_WRITE_CALLBACK pfnWriteCallback)
{
    _pfnWriteCallback          = pfnWriteCallback;
    DataVersion* pDataVersions = (DataVersion*)malloc(sizeof(DataVersion)*ArraySize(bridgedClusters));
    ENDPOINT_DATA endpointData = {
        .index                    = GetIndex(),
        .pObject                  = this,
        .pfnReadCallback          = GoogleReadCallback /*local read function specific to a DeviceLightLevel*/,
        .pfnWriteCallback         = GoogleWriteCallback,
        .pfnInstantActionCallback = NULL, // worry about this later
        .name                     = { 0 },
        .location                 = { 0 },
        .ep                       = &bridgedEndpoint,
        .pDeviceTypeList          = bridgedDeviceTypes,
        .deviceTypeListLength     = ArraySize(bridgedDeviceTypes),
        .pDataVersionStorage      = pDataVersions,
        .dataVersionStorageLength = ArraySize(bridgedClusters),
        .parentEndpointId         = 1,
    };
    
    AddCluster(&descriptorCluster);
    AddCluster(&onOffCluster);
    AddCluster(&levelControlCluster);
    AddCluster(&colourCluster);

    strncpy(basicCluster._name, pName, sizeof(basicCluster._name));

    strcpy(endpointData.name, pName);
    strcpy(endpointData.location, pLocation);

    memcpy(&_endpointData, &endpointData, sizeof(_endpointData));
    EndpointAdd(&_endpointData);
}
DeviceLightRGB::~DeviceLightRGB()
{   
    free(_endpointData.pDataVersionStorage);
    EndpointRemove(GetIndex());
}

/**************************************************************************
 *                                  Private Functions
 **************************************************************************/

void DeviceLightRGB::sendEspNowMessage()
{
    _espNowData.data.lightRgb.onOff      = onOffCluster._isOn;
    _espNowData.data.lightRgb.brightness = levelControlCluster._level;
    _espNowData.data.lightRgb.hue        = colourCluster._hue;
    _espNowData.data.lightRgb.saturation = colourCluster._sat;
    _espNowData.data.lightRgb.mode       = ESP_NOW_DATA_LIGHT_RGB_MODE_STATIC;
    _espNowData.type   = ESP_NOW_DEVICE_TYPE_LIGHT_RGB;
    uint8_t macAddr[6] = { 0x18, 0xFE, 0x34, 0xDC, 0x1F, 0xAF };
    memcpy(_espNowData.macAddr, macAddr, sizeof(_espNowData.macAddr));

    SerialTransmit(&_espNowData, offsetof(ESP_NOW_DATA, data) + sizeof(ESP_NOW_DATA_LIGHT_RGB));
}