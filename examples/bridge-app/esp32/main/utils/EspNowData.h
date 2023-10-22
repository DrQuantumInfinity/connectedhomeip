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

typedef void ESP_NOW_DEVICE_LIST;
/**************************************************************************
 *                                  Variables
 **************************************************************************/
extern char const * const pDeviceTypeStrings[ESP_NOW_DEVICE_TYPE_COUNT];

/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
ESP_NOW_DEVICE_LIST* EspNowCreateDeviceList(void);
void EspNowDestroyDeviceList(ESP_NOW_DEVICE_LIST* pList);
const char* EspNowGetName(const ESP_NOW_DATA* pDev);

ESP_NOW_DATA* AddOrUpdateDev(ESP_NOW_DEVICE_LIST* pList, ESP_NOW_DATA* pDev);
void RemoveDev(ESP_NOW_DEVICE_LIST* pList, ESP_NOW_DATA* pDev);
ESP_NOW_DATA* GetDevByDevMac(ESP_NOW_DEVICE_LIST* pList, const ESP_NOW_DATA* pDev);