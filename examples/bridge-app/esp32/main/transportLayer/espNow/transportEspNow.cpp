
#include "transportEspNow.h"
#include "SerialTask.h"

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
struct TransportEspNow::Private
{
    static void DeviceLightRgb(TransportEspNow& self, const DeviceLightRGB* pDevice);
};
/**************************************************************************
 *                                  Variables
 **************************************************************************/
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
TransportEspNow::TransportEspNow(const ESP_NOW_DATA* pData)
{
    memcpy(&_data, pData, sizeof(_data));
}
TransportEspNow::~TransportEspNow(void)
{
    
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
void TransportEspNow::Send(const void* pDevice, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer)
{
    switch (_data.type)
    {
        case ESP_NOW_DEVICE_TYPE_LIGHT_RGB: Private::DeviceLightRgb(*this, (const DeviceLightRGB*)pDevice); break;
        default:                            /*Support this type!*/                                          break;
    }
}
void TransportEspNow::Private::DeviceLightRgb(TransportEspNow& self, const DeviceLightRGB* pDevice)
{
    self._data.data.lightRgb.onOff      = pDevice->onOffCluster._isOn;
    self._data.data.lightRgb.brightness = pDevice->levelControlCluster._level;
    self._data.data.lightRgb.hue        = pDevice->colourCluster._hue;
    self._data.data.lightRgb.saturation = pDevice->colourCluster._sat;
    self._data.data.lightRgb.mode       = ESP_NOW_DATA_LIGHT_RGB_MODE_STATIC;
    self._data.type                     = ESP_NOW_DEVICE_TYPE_LIGHT_RGB;

    SerialTransmit(&self._data, offsetof(ESP_NOW_DATA, data) + sizeof(ESP_NOW_DATA_LIGHT_RGB));
}
/*
void TransportEspNow::DeviceLightRgb(const DeviceLightRGB* pDevice)
{    
    _data.data.lightRgb.onOff      = pDevice->onOffCluster._isOn;
    _data.data.lightRgb.brightness = pDevice->levelControlCluster._level;
    _data.data.lightRgb.hue        = pDevice->colourCluster._hue;
    _data.data.lightRgb.saturation = pDevice->colourCluster._sat;
    _data.data.lightRgb.mode       = ESP_NOW_DATA_LIGHT_RGB_MODE_STATIC;
    _data.type                     = ESP_NOW_DEVICE_TYPE_LIGHT_RGB;

    SerialTransmit(&_data, offsetof(ESP_NOW_DATA, data) + sizeof(ESP_NOW_DATA_LIGHT_RGB));
}
void DeviceLight::sendEspNowMessage()
{
    _espNowData.data.lightOnOff.onOff = onOffCluster._isOn;
    SerialTransmit(&_espNowData, sizeof(_espNowData));
}
void DeviceLightLevel::sendEspNowMessage()
{
    _espNowData.data.lightDimmer.onOff      = onOffCluster._isOn;
    _espNowData.data.lightDimmer.brightness = levelControlCluster._level;
    SerialTransmit(&_espNowData, sizeof(_espNowData));
}
void DeviceLightTemp::sendEspNowMessage()
{
    _espNowData.data.lightTempRgb.onOff = onOffCluster._isOn;
    _espNowData.data.lightTempRgb.brightness = levelControlCluster._level;
    _espNowData.data.lightTempRgb.hue = colourCluster._hue;
    _espNowData.data.lightTempRgb.saturation = colourCluster._sat;
    _espNowData.data.lightTempRgb.tempKelvin = 1000'000 / colourCluster._temp;
    SerialTransmit(&_espNowData, sizeof(_espNowData));
}
*/