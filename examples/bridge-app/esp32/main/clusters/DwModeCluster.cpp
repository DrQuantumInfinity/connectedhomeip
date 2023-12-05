#include "DwModeCluster.h"
#include "EndpointApi.h"
#include "dwMode.h"

#include <app-common/zap-generated/callback.h>
#include "esp_log.h"
#include <lib/support/ZclString.h>
/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define ZCL_ON_OFF_CLUSTER_REVISION (4u)
static const char * TAG = "DwModeCluster";

const EmberAfGenericClusterFunction chipFuncArrayOnOffServer[] = {
    (EmberAfGenericClusterFunction) emberAfDishwasherModeClusterInitCallback,
    // (EmberAfGenericClusterFunction) MatterTestModeClusterServerShutdownCallback, //TODO fix
};

static constexpr EmberAfAttributeMetadata attributes[] = {
    { // onOff attribute
        .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = DishwasherMode::Attributes::CurrentMode::Id,
        .size          = 1,
        .attributeType = ZAP_TYPE(INT8U),
        .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
    { // ClusterRevision attribute
        .defaultValue  = ZAP_EMPTY_DEFAULT(),
        .attributeId   = 0xFFFD,
        .size          = 2,
        .attributeType = ZAP_TYPE(INT16U),
        .mask          = ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) },
};
static constexpr CommandId acceptedCommandList[] = {
    // app::Clusters::OnOff::Commands::Off::Id,
    // app::Clusters::OnOff::Commands::On::Id,
    // app::Clusters::OnOff::Commands::Toggle::Id,
    // app::Clusters::OnOff::Commands::OffWithEffect::Id,
    // app::Clusters::OnOff::Commands::OnWithRecallGlobalScene::Id,
    // app::Clusters::OnOff::Commands::OnWithTimedOff::Id,
    kInvalidCommandId,
};

static EmberAfCluster cluster = { 
    .clusterId            = DishwasherMode::Id,
    .attributes           = attributes,
    .attributeCount       = ArraySize(attributes),
    .clusterSize          = 0, //Assigned dynamically upon GetObject()
    .mask                 = ZAP_CLUSTER_MASK(SERVER) | ZAP_CLUSTER_MASK(INIT_FUNCTION),// | ZAP_CLUSTER_MASK(SHUTDOWN_FUNCTION),
    .functions            = chipFuncArrayOnOffServer,
    .acceptedCommandList  = acceptedCommandList,
    .generatedCommandList = nullptr,
    .eventList            = nullptr,
    .eventCount           = 0 
};
/**************************************************************************
 *                                  Class Functions
 **************************************************************************/
void DwModeCluster::SetMode(DishwasherMode::ModeTag newMode, uint16_t index)
{
    _mode = newMode;
    EndpointReportChange(index, DishwasherMode::Id, ModeBase::Attributes::CurrentMode::Id); //TODO
}

EmberAfStatus DwModeCluster::Write(chip::AttributeId attributeId, uint8_t* buffer)
{
    ESP_LOGI(TAG, "DwModeCluster Write called");
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    switch (attributeId)
    {
        // case CurrentMode::Id:  _mode = (bool)buffer[0];   break;
        case ModeBase::Attributes::CurrentMode::Id:   DishwasherMode::Instance()->UpdateCurrentMode(buffer[0]); break;
        default:                            status = EMBER_ZCL_STATUS_FAILURE;      break;
    }
    return status;
}

EmberAfStatus DwModeCluster::Read(chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength){

    ESP_LOGI(TAG, "DwModeCluster Read called");
    EmberAfStatus status = EMBER_ZCL_STATUS_SUCCESS;
    
    if ((attributeId == ModeBase::Attributes::CurrentMode::Id) && (maxReadLength == 1))
    {
        // *buffer = _mode;
        *buffer = DishwasherMode::Instance()->GetCurrentMode();
    }
    else if ((attributeId == ModeBase::Attributes::ClusterRevision::Id) && (maxReadLength == 2))
    {
        uint16_t rev = ZCL_ON_OFF_CLUSTER_REVISION;
        memcpy(buffer, &rev, sizeof(rev));
    }
    else
    {
        ESP_LOGE(TAG, "Failed to read. attr = %x", attributeId);
   
        status = EMBER_ZCL_STATUS_FAILURE;
    }
    return status;
}

/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
EmberAfCluster ClusterDwModeGetObject(void)
{
    if (cluster.clusterSize == 0) //only perform this the first time
    {
        for (int i = 0; i < cluster.attributeCount; i++)
        {
            cluster.clusterSize += cluster.attributes[i].size;
        }
    }
    return cluster;
}