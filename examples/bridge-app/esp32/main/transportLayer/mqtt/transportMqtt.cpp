
#include "transportMqtt.h"
#include "DeviceList.h"

//Devices
#include "DeviceLightRGB.h"

/**************************************************************************
 *                                  Constants
 **************************************************************************/
/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
struct TransportMqtt::Private
{
    static void DeviceLightRgbSend(TransportMqtt& self, const DeviceLightRGB* pDevice);
};
/**************************************************************************
 *                                  Variables
 **************************************************************************/
DeviceList deviceList;
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
void TransportMqttHandleTopicRx(const char* pTopic, const char *pPayload)
{
    Device *pDevice = deviceList.GetDevice(pTopic);
    if (pDevice == NULL)
    {
        //char nameBuf[32] = "test";
        /*sprintf(nameBuf, "%s %02X:%02X:%02X:%02X:%02X:%02X", MqttGetName(pData),
            pData->macAddr[0], pData->macAddr[1], pData->macAddr[2], 
            pData->macAddr[3], pData->macAddr[4], pData->macAddr[5]);*/
        //switch(pData->type)
        //{
        //    case ESP_NOW_DEVICE_TYPE_LIGHT_RGB: pDevice = new DeviceLightRGB(nameBuf, "Z"/*, new TransportMqtt(pData)*/); break;
        //    default:                            /*Support this type!*/                      break;
        //}
    }
    
    if (pDevice)
    {
        deviceList.Upsert(pTopic, pDevice);
    }
}
TransportMqtt::TransportMqtt(const char* pTopic)
{
    
}
TransportMqtt::~TransportMqtt(void)
{
    
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
void TransportMqtt::Send(const Device* pDevice, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer)
{
    //switch (_data.type)
    //{
    //    case ESP_NOW_DEVICE_TYPE_LIGHT_RGB: Private::DeviceLightRgbSend(*this, (const DeviceLightRGB*)pDevice); break;
    //    default:                            /*Support this type!*/                                              break;
    //}
}
/*void TransportMqtt::Private::DeviceLightRgbSend(TransportMqtt& self, const DeviceLightRGB* pDevice)
{
    self._data.data.lightRgb.onOff      = pDevice->onOffCluster._isOn;
    self._data.data.lightRgb.brightness = pDevice->levelControlCluster._level;
    self._data.data.lightRgb.hue        = pDevice->colourCluster._hue;
    self._data.data.lightRgb.saturation = pDevice->colourCluster._sat;
    self._data.data.lightRgb.mode       = ESP_NOW_DATA_LIGHT_RGB_MODE_STATIC;
    self._data.type                     = ESP_NOW_DEVICE_TYPE_LIGHT_RGB;

    SerialTransmit(&self._data, offsetof(ESP_NOW_DATA, data) + sizeof(ESP_NOW_DATA_LIGHT_RGB));
}*/
/*
void TransportMqtt::DeviceLightRgb(const DeviceLightRGB* pDevice)
{    
    _data.data.lightRgb.onOff      = pDevice->onOffCluster._isOn;
    _data.data.lightRgb.brightness = pDevice->levelControlCluster._level;
    _data.data.lightRgb.hue        = pDevice->colourCluster._hue;
    _data.data.lightRgb.saturation = pDevice->colourCluster._sat;
    _data.data.lightRgb.mode       = ESP_NOW_DATA_LIGHT_RGB_MODE_STATIC;
    _data.type                     = ESP_NOW_DEVICE_TYPE_LIGHT_RGB;

    SerialTransmit(&_data, offsetof(ESP_NOW_DATA, data) + sizeof(ESP_NOW_DATA_LIGHT_RGB));
}
void DeviceLight::sendMqttMessage()
{
    _espNowData.data.lightOnOff.onOff = onOffCluster._isOn;
    SerialTransmit(&_espNowData, sizeof(_espNowData));
}
void DeviceLightLevel::sendMqttMessage()
{
    _espNowData.data.lightDimmer.onOff      = onOffCluster._isOn;
    _espNowData.data.lightDimmer.brightness = levelControlCluster._level;
    SerialTransmit(&_espNowData, sizeof(_espNowData));
}
void DeviceLightTemp::sendMqttMessage()
{
    _espNowData.data.lightTempRgb.onOff = onOffCluster._isOn;
    _espNowData.data.lightTempRgb.brightness = levelControlCluster._level;
    _espNowData.data.lightTempRgb.hue = colourCluster._hue;
    _espNowData.data.lightTempRgb.saturation = colourCluster._sat;
    _espNowData.data.lightTempRgb.tempKelvin = 1000'000 / colourCluster._temp;
    SerialTransmit(&_espNowData, sizeof(_espNowData));
}
*/