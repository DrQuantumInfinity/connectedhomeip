
#include "SerialTask.h"
#include "TaskMessage.h"
#include "TimerTick.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"

using namespace chip;

/**************************************************************************
 *                                  Constants
 **************************************************************************/
static const char * TAG =               "serial-task";
#define SERIAL_TASK_MSG_QUEUE_DEPTH     (10)
#define SERIAL_TASK_NAME                "SERIAL"
#define SERIAL_TASK_STACK_SIZE          (2000)

#define SERIAL_TX_MAX_SIZE              (300)
#define SERIAL_TX_QUEUE_DEPTH           (10)

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
/**************************************************************************
 *                                  Types
 **************************************************************************/
typedef struct
{
    QueueHandle_t publicQueue;
    TaskHandle_t task;
    TimerTick timerTick;
    QueueHandle_t uartQueue;
}SERIAL_TASK;
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
static void SerialMain(void* pvParameter);
//Setup
static void SerialSetup(void);
static void SerialUartInit(void);
//Timer Handling
static TickType_t SerialGetTimeoutTick(void);
static void SerialHandleTimeout(void);
//Message Processing
static void SerialProcessMyMsg(const MSG_HEADER* pMsg);
static void SerialTxMsg(const void* pData, uint32_t dataLength);
/**************************************************************************
 *                                  Variables
 **************************************************************************/
SERIAL_TASK serialTask;
/**************************************************************************
 *                                  OS Functions
 **************************************************************************/
CHIP_ERROR SerialTaskStart(void)
{
    memset(&serialTask, 0x00, sizeof(serialTask));
    serialTask.publicQueue = xQueueCreate(SERIAL_TASK_MSG_QUEUE_DEPTH, sizeof(void*));
    if (serialTask.publicQueue == NULL)
    {
        ESP_LOGE(TAG, "Failed to allocate app event queue");
        return ERROR_EVENT_QUEUE_FAILED;
    }

    // Start App task.
    BaseType_t xReturned;
    xReturned = xTaskCreate(SerialMain, SERIAL_TASK_NAME, SERIAL_TASK_STACK_SIZE, NULL, 1, &serialTask.task);
    return (xReturned == pdPASS) ? CHIP_NO_ERROR : ERROR_CREATE_TASK_FAILED;
}
/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
void SerialTransmit(const void* pData, uint32_t dataLength)
{
    MsgPost(serialTask.publicQueue, MID_SERIAL_TX, pData, dataLength);
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
static void SerialMain(void* pvParameter)
{
    SerialSetup();
    ESP_LOGI(TAG, "Task started");

    MSG_HEADER *pMsg;
    while (true)
    {
        BaseType_t eventReceived = xQueueReceive(serialTask.publicQueue, &pMsg, SerialGetTimeoutTick());
        if (eventReceived == pdTRUE)
        {
            SerialProcessMyMsg(pMsg);
            free(pMsg);
        }
        else
        {
            SerialHandleTimeout();
        }
    }
}
//Setup
static void SerialSetup(void)
{
    SerialUartInit();
    serialTask.timerTick.SetFromNow(1000);
}
static void SerialUartInit(void)
{
    uart_config_t uartConfig = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122, //dummy number
        .source_clk = (uart_sclk_t)0, //dummy number
    };
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uartConfig));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, SERIAL_TX_MAX_SIZE, 300, SERIAL_TX_QUEUE_DEPTH, &serialTask.uartQueue, 0));
}
//Timer Handling
static TickType_t SerialGetTimeoutTick(void)
{
    return serialTask.timerTick.GetRemaining();
}
static void SerialHandleTimeout(void)
{
    if (serialTask.timerTick.HasElapsed())
    {
        serialTask.timerTick.Disable();
    }
}
//Message Processing
static void SerialProcessMyMsg(const MSG_HEADER* pMsg)
{
    switch (pMsg->mid)
    {
    case MID_SERIAL_TX: SerialTxMsg(pMsg->data, pMsg->length);  break;
    default:            /*Do Nothing*/                          break;
    }
}
static void SerialTxMsg(const void* pData, uint32_t dataLength)
{
    uart_write_bytes(UART_NUM_1, pData, dataLength);
}
