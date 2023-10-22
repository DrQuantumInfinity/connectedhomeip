
#include "AppTask.h"
#include "DeviceLight.h"
#include "DeviceButton.h"
// #include "SerialTask.h"

#include "esp_log.h"
#include "esp_now.h"
#include "freertos/FreeRTOS.h"


/**************************************************************************
 *                                  Constants
 **************************************************************************/
#define APP_TASK_NAME "APP"
#define APP_EVENT_QUEUE_SIZE 10
#define APP_TASK_STACK_SIZE (5000)
#define ESPNOW_SERVER_MAC_ADDR  {0x18, 0xFE, 0x34, 0xD1, 0xB0, 0x77}

/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/
using namespace chip;

/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
static bool EspNowInit(void);
static void EspNowSendCallback(const uint8_t *mac_addr, esp_now_send_status_t status);

/**************************************************************************
 *                                  Variables
 **************************************************************************/
static const char * TAG = "app-task";

namespace {
    QueueHandle_t sAppEventQueue;
    TaskHandle_t sAppTaskHandle;
} // namespace

AppTask AppTask::sAppTask;

/**************************************************************************
 *                                  Global Functions
 **************************************************************************/
CHIP_ERROR AppTask::StartAppTask()
{
    sAppEventQueue = xQueueCreate(APP_EVENT_QUEUE_SIZE, sizeof(AppEvent));
    if (sAppEventQueue == NULL)
    {
        ESP_LOGE(TAG, "Failed to allocate app event queue");
        return APP_ERROR_EVENT_QUEUE_FAILED;
    }

    // Start App task.
    BaseType_t xReturned;
    xReturned = xTaskCreate(AppTaskMain, APP_TASK_NAME, APP_TASK_STACK_SIZE, NULL, 1, &sAppTaskHandle);
    return (xReturned == pdPASS) ? CHIP_NO_ERROR : APP_ERROR_CREATE_TASK_FAILED;
}
/**************************************************************************
 *                                  Private Functions
 **************************************************************************/
CHIP_ERROR AppTask::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    EspNowInit();
    timerTick.SetFromNow(10000);
    return err;
}
static bool EspNowInit(void)
{
    esp_err_t espNowErr;

    uint32_t attempts = 10;
    do 
    {
        espNowErr = esp_now_init();
        if (espNowErr != ESP_OK)
        {
            vTaskDelay(pdMS_TO_TICKS(200));
        }
    } while (attempts--);

    if (espNowErr == ESP_OK)
    {
        uint32_t version;
        espNowErr = esp_now_get_version(&version);
        if (espNowErr == ESP_OK)
        {
            ESP_LOGI(TAG, "esp_now_get_version() success with %08lX", version);
            esp_now_register_send_cb(EspNowSendCallback);
            esp_now_peer_info_t peerInfo;
            memset(&peerInfo, 0x00, sizeof(peerInfo));
            uint8_t peerAddr[6] = ESPNOW_SERVER_MAC_ADDR;
            memcpy(peerInfo.peer_addr, peerAddr, sizeof(peerInfo.peer_addr));
            esp_now_add_peer(&peerInfo);
            #define MESSAGE "Hey Paul! I sent an EspNow message!"
            espNowErr = esp_now_send(peerInfo.peer_addr, (uint8_t *)MESSAGE, strlen(MESSAGE)+1);
            if (espNowErr == ESP_OK)
            {
                ESP_LOGI(TAG, "esp_now_send() success");
            }
            else
            {
                ESP_LOGI(TAG, "esp_now_send() failed due to %08X", espNowErr);
            }
        }
        else
        {
            ESP_LOGI(TAG, "esp_now_get_version() failed due to %08X", espNowErr);
        }
    }
    else
    {
        ESP_LOGI(TAG, "esp_now_init() failed due to %08X", espNowErr);
    }
    return espNowErr == ESP_OK;
}
void EspNowSendCallback(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    if (status == ESP_NOW_SEND_SUCCESS)
    {
        ESP_LOGI(TAG, "EspNowSendCallback() success");
    }
    else
    {
        ESP_LOGI(TAG, "EspNowSendCallback() fail");
    }
}

void AppTask::AppTaskMain(void * pvParameter)
{
    AppEvent event;
    CHIP_ERROR err = sAppTask.Init();
    if (err != CHIP_NO_ERROR)
    {
        ESP_LOGI(TAG, "AppTask.Init() failed due to %" CHIP_ERROR_FORMAT, err.Format());
        return;
    }

    ESP_LOGI(TAG, "App Task started");

    while (true)
    {
        BaseType_t eventReceived = xQueueReceive(sAppEventQueue, &event, sAppTask.GetTimeoutTick());
        if (eventReceived == pdTRUE)
        {
            sAppTask.DispatchEvent(&event);
        }
        else
        {
            sAppTask.HandleTimeout();
        }
    }
}
TickType_t AppTask::GetTimeoutTick(void)
{
    return sAppTask.timerTick.GetRemaining();
}
static DeviceLight *deviceLight;
static DeviceButton *deviceButton;
void AppTask::HandleTimeout(void)
{
    ESP_LOGI(TAG, "Handle timeout");

    if (sAppTask.timerTick.HasElapsed())
    {
        ESP_LOGI(TAG, "Adding Light");
        sAppTask.timerTick.Disable();
        // deviceLight = new DeviceLight("Light 6", "nowhere", NULL);
        deviceButton = new DeviceButton("Light 6", "nowhere", NULL);
        sAppTask.timerTick.Disable();
        // SerialTransmit("Hey Paul", strlen("Hey Paul") + 1);
    }
}
void AppTask::PostEvent(const AppEvent * aEvent)
{
    if (sAppEventQueue != NULL)
    {
        BaseType_t status;
        if (xPortInIsrContext())
        {
            BaseType_t higherPrioTaskWoken = pdFALSE;
            status                         = xQueueSendFromISR(sAppEventQueue, aEvent, &higherPrioTaskWoken);
        }
        else
        {
            status = xQueueSend(sAppEventQueue, aEvent, 1);
        }
        if (!status)
            ESP_LOGE(TAG, "Failed to post event to app task event queue");
    }
    else
    {
        ESP_LOGE(TAG, "Event Queue is NULL should never happen");
    }
}

void AppTask::DispatchEvent(AppEvent * aEvent)
{
    if (aEvent->mHandler)
    {
        aEvent->mHandler(aEvent);
    }
    else
    {
        ESP_LOGI(TAG, "Event received with no handler. Dropping event.");
    }
}
