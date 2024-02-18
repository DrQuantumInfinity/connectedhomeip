
#include "MatterTask.h"
#include "TaskMessage.h"
#include "TimerTick.h"
#include "EspNowData.h"
#include "transportEspNow.h"
#include "freertos/FreeRTOS.h"

using namespace chip;

/**************************************************************************
 *                                  Constants
 **************************************************************************/
static const char * TAG =               "matter-task";
#define MATTER_TASK_MSG_QUEUE_DEPTH     (10)
#define MATTER_TASK_NAME                "MATTER"
#define MATTER_TASK_STACK_SIZE          (2000)

#if SOC_UART_NUM > 2
#define MATTER_UART                     (UART_NUM_2)
#else
#define MATTER_UART                     (UART_NUM_1)
#endif
#define MATTER_TX_MAX_SIZE              (300)
#define MATTER_TX_QUEUE_DEPTH           (10)
#define MATTER_RX_MAX_SIZE              (300)
const uint32_t startKey =               0x1f5a3db9;

// Application-defined error codes in the CHIP_ERROR space.
#define ERROR_EVENT_QUEUE_FAILED        CHIP_APPLICATION_ERROR(0x01)
#define ERROR_CREATE_TASK_FAILED        CHIP_APPLICATION_ERROR(0x02)
#define ERROR_UNHANDLED_EVENT           CHIP_APPLICATION_ERROR(0x03)
#define ERROR_CREATE_TIMER_FAILED       CHIP_APPLICATION_ERROR(0x04)
#define ERROR_START_TIMER_FAILED        CHIP_APPLICATION_ERROR(0x05)
#define ERROR_STOP_TIMER_FAILED         CHIP_APPLICATION_ERROR(0x06)
/**************************************************************************
 *                                  Macros
 **************************************************************************/
#ifndef MAX
#define MAX(x, y)   ( ((x) > (y))? (x) : (y) )
#endif
#ifndef MIN
#define MIN(x, y)   ( ((x) < (y))? (x) : (y) )
#endif
/**************************************************************************
 *                                  Types
 **************************************************************************/
typedef struct
{
    uint32_t offset;
    uint8_t data[MATTER_RX_MAX_SIZE];
}RX_FRAMING;

typedef struct
{
    QueueHandle_t publicQueue;
    TaskHandle_t task;
    TimerTick timerTick;
}MATTER_TASK;
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
static void MatterMain(void* pvParameter);
//Setup
static void MatterSetup(void);
//Timer Handling
static TickType_t MatterGetTimeoutTick(void);
static void MatterHandleTimeout(void);
//Message Processing
static void MatterProcessMyMsg(const MSG_HEADER* pMsg);
static void MatterEspNowRxMsg(const ESP_NOW_DATA* pEspMsg, uint32_t dataLength);
/**************************************************************************
 *                                  Variables
 **************************************************************************/
static MATTER_TASK matterTask;
/**************************************************************************
 *                                  OS Functions
 **************************************************************************/
CHIP_ERROR MatterTaskStart(void)
{
    memset(&matterTask, 0x00, sizeof(matterTask));
    matterTask.publicQueue = xQueueCreate(MATTER_TASK_MSG_QUEUE_DEPTH, sizeof(void*));
    if (matterTask.publicQueue == NULL)
    {
        ESP_LOGE(TAG, "Failed to allocate app event queue");
        return ERROR_EVENT_QUEUE_FAILED;
    }

    // Start App task.
    BaseType_t xReturned;
    xReturned = xTaskCreate(MatterMain, MATTER_TASK_NAME, MATTER_TASK_STACK_SIZE, NULL, 1, &matterTask.task);
    return (xReturned == pdPASS) ? CHIP_NO_ERROR : ERROR_CREATE_TASK_FAILED;
}
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
void MatterProcessEspNowMessage(const ESP_NOW_DATA *pEspMsg, uint32_t dataLength)
{
    MsgPost(matterTask.publicQueue, MID_MATTER_PROCESS_ESP_NOW, pEspMsg, dataLength);
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
static void MatterMain(void* pvParameter)
{
    MatterSetup();
    ESP_LOGI(TAG, "Task started");

    MSG_HEADER *pMsg;
    while (true)
    {
        BaseType_t eventReceived = xQueueReceive(matterTask.publicQueue, &pMsg, MatterGetTimeoutTick());
        if (eventReceived == pdTRUE)
        {
            MatterProcessMyMsg(pMsg);
            free(pMsg);
        }
        else
        {
            MatterHandleTimeout();
        }
    }
}
//Setup
static void MatterSetup(void)
{
    matterTask.timerTick.Disable();
}
//Timer Handling
static TickType_t MatterGetTimeoutTick(void)
{
    return matterTask.timerTick.GetRemaining();
}
static void MatterHandleTimeout(void)
{
    if (matterTask.timerTick.HasElapsed())
    {
        matterTask.timerTick.Disable();
    }
}
//Message Processing
static void MatterProcessMyMsg(const MSG_HEADER* pMsg)
{
    const void* pData = pMsg->data;
    switch (pMsg->mid)
    {
    case MID_MATTER_PROCESS_ESP_NOW:    MatterEspNowRxMsg((const ESP_NOW_DATA*)pData, pMsg->length);    break;
    default:                            /*Do Nothing*/                                                  break;
    }
}
static void MatterEspNowRxMsg(const ESP_NOW_DATA* pEspMsg, uint32_t dataLength)
{
    TransportEspNow::HandleSerialRx(pEspMsg, dataLength);
}