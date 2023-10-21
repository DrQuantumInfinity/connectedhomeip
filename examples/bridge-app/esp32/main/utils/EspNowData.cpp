#include "EspNowData.h"

#include <string.h>
#include <stdlib.h>

/**************************************************************************
 *                                  Constants
 **************************************************************************/
char const * const pDeviceTypeStrings[ESP_NOW_DEVICE_TYPE_COUNT] = {
    "WifiDht", 
    "WifiMotion", 
    "WifiBool", 
    "WifiToggle"
};

/**************************************************************************
 *                                  Macros
 **************************************************************************/
typedef struct toggleDev 
{
    struct toggleDev *nextDev;
    uint8_t macShort[6];
    bool value;
} TOGGLE_DEV;

/**************************************************************************
 *                                  Types
 **************************************************************************/
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
static TOGGLE_DEV *FindDev(uint8_t *macShort);
static bool MatchFound(uint8_t *mac1, uint8_t *mac2);
static TOGGLE_DEV *GetDev(uint8_t *macShort);
static TOGGLE_DEV *AddDev(uint8_t *macShort);

/**************************************************************************
 *                                  Variables
 **************************************************************************/
static TOGGLE_DEV *devsHead;
static TOGGLE_DEV *devsTail;

/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
bool EspNowDataToggleDevice(uint8_t *macShort) 
{
    TOGGLE_DEV *dev = GetDev(macShort);
    dev->value = !dev->value;
    return dev->value;
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
static TOGGLE_DEV *GetDev(uint8_t *macShort) 
{
    TOGGLE_DEV *result;
    if (devsHead == NULL) 
    {
        devsHead = AddDev(macShort);
        result = devsHead;
    } 
    else 
    {
        result = FindDev(macShort);
    }

    if (result == NULL) 
    {
        result = AddDev(macShort);
    }
    return result;
}

static TOGGLE_DEV *AddDev(uint8_t *macShort) 
{
    TOGGLE_DEV *result = (TOGGLE_DEV *)malloc(sizeof(TOGGLE_DEV));
    memcpy(result->macShort, macShort, 6);
    result->value = 0;
    if (devsTail != NULL) 
    {
        devsTail->nextDev = result;
    }
    devsTail = result;
    return result;
}

static TOGGLE_DEV *FindDev(uint8_t *macShort) 
{
    TOGGLE_DEV *currentDev = devsHead;
    TOGGLE_DEV *match = NULL;
    while (true) 
    {
        if (MatchFound(currentDev->macShort, macShort)) 
        {
            match = currentDev;
            break;
        } 
        else if (currentDev->nextDev != NULL) 
        {
            currentDev = currentDev->nextDev;
        } 
        else 
        {
            break;
        }
    }
    return match;
}

static bool MatchFound(uint8_t *mac1, uint8_t *mac2) 
{ 
    return !memcmp(mac1, mac2, 6); 
}