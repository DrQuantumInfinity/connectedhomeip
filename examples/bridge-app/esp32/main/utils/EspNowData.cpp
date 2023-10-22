#include "EspNowData.h"

#include <string.h>
#include <stdlib.h>

/**************************************************************************
 *                                  Constants
 **************************************************************************/
char const * const pDeviceTypeStrings[ESP_NOW_DEVICE_TYPE_COUNT] = {
    "Dht", 
    "Motion", 
    "Bool", 
    "Toggle"
};

#ifndef MEM_CMP_MATCH
#define MEM_CMP_MATCH 0
#endif
/**************************************************************************
 *                                  Macros
 **************************************************************************/
#ifndef ArraySize
#define ArraySize(a) (sizeof(a) / sizeof((a)[0]))
#endif
/**************************************************************************
 *                                  Types
 **************************************************************************/
typedef struct _ESP_NOW_DEV_NODE 
{
    struct _ESP_NOW_DEV_NODE *pNextNode;

    ESP_NOW_DATA dev;
} ESP_NOW_DEV_NODE;

typedef struct
{
    ESP_NOW_DEV_NODE* pHeadNode;
    ESP_NOW_DEV_NODE* pTailNode;
    uint32_t numDevices;
} ESP_NOW_DEVICE_LIST_REAL;
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
static ESP_NOW_DEV_NODE* GetDevNodeByDevMac(ESP_NOW_DEVICE_LIST* pList, const ESP_NOW_DATA* pDev);
/**************************************************************************
 *                                  Variables
 **************************************************************************/
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
ESP_NOW_DEVICE_LIST* EspNowCreateDeviceList(void)
{
    ESP_NOW_DEVICE_LIST_REAL *pList = (ESP_NOW_DEVICE_LIST_REAL*)malloc(sizeof(ESP_NOW_DEVICE_LIST_REAL));
    memset(pList, 0x00, sizeof(*pList));
    return (ESP_NOW_DEVICE_LIST*)pList;
}
void EspNowDestroyDeviceList(ESP_NOW_DEVICE_LIST* pList)
{
    ESP_NOW_DEVICE_LIST_REAL* pListReal = (ESP_NOW_DEVICE_LIST_REAL*)pList;
    
    ESP_NOW_DEV_NODE* pNode = pListReal->pHeadNode;
    while (pNode != NULL)
    {
        ESP_NOW_DEV_NODE* pFree = pNode;
        pNode = pNode->pNextNode;
        free(pFree);
    }
    free(pList);
}
const char* EspNowGetName(const ESP_NOW_DATA* pDev)
{
    const char* pRetVal = "UNKNOWN";
    if (pDev->type < ArraySize(pDeviceTypeStrings))
    {
        pRetVal = pDeviceTypeStrings[pDev->type];
    }
    return pRetVal;
}
ESP_NOW_DATA* AddOrUpdateDev(ESP_NOW_DEVICE_LIST* pList, ESP_NOW_DATA* pDev)
{
    ESP_NOW_DEVICE_LIST_REAL* pListReal = (ESP_NOW_DEVICE_LIST_REAL*)pList;

    ESP_NOW_DEV_NODE* pNode = GetDevNodeByDevMac(pList, pDev);
    if (pNode == NULL)
    {
        //This is a new device
        pNode = (ESP_NOW_DEV_NODE*)malloc(sizeof(ESP_NOW_DEV_NODE));
        pNode->pNextNode = NULL;

        if (pListReal->numDevices == 0)
        {
            pListReal->pHeadNode = pNode;
            pListReal->pTailNode = pNode;
        }
        else
        {
            pListReal->pTailNode->pNextNode = pNode;
            pListReal->pTailNode = pNode;
        }
        pListReal->numDevices++;
    }
    else
    {
        //Node already exists. Local data will be updated.
    }
    memcpy(&pNode->dev, pDev, sizeof(pNode->dev));

    return &pNode->dev;
}
void RemoveDev(ESP_NOW_DEVICE_LIST* pList, ESP_NOW_DATA* pDev)
{
    ESP_NOW_DEVICE_LIST_REAL* pListReal = (ESP_NOW_DEVICE_LIST_REAL*)pList;
    
    ESP_NOW_DEV_NODE* pPreviousNode = pListReal->pHeadNode;
    ESP_NOW_DEV_NODE* pNode = pPreviousNode;
    if (pPreviousNode != NULL)
    {
        if (memcmp(pPreviousNode->dev.macAddr, pDev->macAddr, sizeof(pPreviousNode->dev.macAddr)) == MEM_CMP_MATCH)   
        {
            pListReal->pHeadNode = pPreviousNode->pNextNode;
            free(pPreviousNode);
            pListReal->numDevices--;
        }
        else
        {
            pNode = pPreviousNode->pNextNode;
            while (pNode != NULL)
            {
                if (memcmp(pNode->dev.macAddr, pDev->macAddr, sizeof(pNode->dev.macAddr)) == MEM_CMP_MATCH)
                {
                    pPreviousNode->pNextNode = pNode->pNextNode;
                    free(pNode);
                    pListReal->numDevices--;
                    break;
                }
                else
                {
                    pNode = pNode->pNextNode;
                }
            }
        }
    }

    if (pNode == NULL)
    {
        //Device never existed in the list...
    }
}
ESP_NOW_DATA* GetDevByDevMac(ESP_NOW_DEVICE_LIST* pList, const ESP_NOW_DATA* pDev)
{
    ESP_NOW_DATA* pDevStored = NULL;
    ESP_NOW_DEV_NODE* pNode = GetDevNodeByDevMac(pList, pDev);
    if (pNode != NULL)
    {
        pDevStored = &pNode->dev;
    }
    return pDevStored;
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
static ESP_NOW_DEV_NODE* GetDevNodeByDevMac(ESP_NOW_DEVICE_LIST* pList, const ESP_NOW_DATA* pDev)
{
    ESP_NOW_DEVICE_LIST_REAL* pListReal = (ESP_NOW_DEVICE_LIST_REAL*)pList;
    
    ESP_NOW_DEV_NODE* pNode = pListReal->pHeadNode;
    while (pNode != NULL)
    {
        if (memcmp(pNode->dev.macAddr, pDev->macAddr, sizeof(pNode->dev.macAddr)) == MEM_CMP_MATCH)
        {
            break;
        }
        else
        {
            pNode = pNode->pNextNode;
        }
    }
    return pNode;
}