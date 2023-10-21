#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

/**************************************************************************
 *                                  Constants
 **************************************************************************/
/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/
typedef enum __attribute__((packed))
{
    ESP_NOW_DEVICE_TYPE_DHT,
    ESP_NOW_DEVICE_TYPE_MOTION,
    ESP_NOW_DEVICE_TYPE_BOOL,
    ESP_NOW_DEVICE_TYPE_TOGGLE,
    ESP_NOW_DEVICE_TYPE_COUNT
}ESP_NOW_DEVICE_TYPE;

typedef struct __attribute__((packed))
{
    float temperature;
    float humidity;
}ESP_NOW_DATA_DHT;

typedef struct __attribute__((packed))
{
    bool dummy; //always true
}ESP_NOW_DATA_MOTION;

typedef struct __attribute__((packed))
{
    bool dummy; //always true
}ESP_NOW_DATA_TOGGLE;

typedef struct __attribute__((packed))
{
    bool onOff;
}ESP_NOW_DATA_ON_OFF;

typedef union __attribute__((packed))
{
    ESP_NOW_DATA_DHT dht;
    ESP_NOW_DATA_MOTION motion;
    ESP_NOW_DATA_ON_OFF onOff;
    ESP_NOW_DATA_TOGGLE toggle;
}ESP_NOW_DATA_UNION;

typedef struct __attribute__((packed))
{
    uint8_t macAddr[6];
    ESP_NOW_DEVICE_TYPE type;
    ESP_NOW_DATA_UNION data;
}ESP_NOW_DATA;

/**************************************************************************
 *                                  Variables
 **************************************************************************/
extern char const * const pDeviceTypeStrings[ESP_NOW_DEVICE_TYPE_COUNT];

/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
bool EspNowDataToggleDevice(uint8_t *macShort);