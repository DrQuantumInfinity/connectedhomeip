

#include "LedcDriver.h"
#include "esp_log.h"
#include "hal/ledc_types.h"

static const char * TAG = "LedcDriver";

// static ledc_channel_t mNextFreeLedcChannel = (ledc_channel_t)0;

LedcDriver::LedcDriver(gpio_num_t pin)
{
    mGpioPin = pin;
    mLedcChannel = mNextFreeLedcChannel;
    mNextFreeLedcChannel = (ledc_channel_t)((int)mNextFreeLedcChannel + 1);

    ESP_LOGI(TAG, "channel %d next free %d next free addr %u", (int)mLedcChannel, (int)mNextFreeLedcChannel, (unsigned int)&mNextFreeLedcChannel);
    DoFirstInit();

    ledc_channel_config_t ledc_channel = {
        .gpio_num   = pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = mLedcChannel,
        .intr_type  = LEDC_INTR_DISABLE,
        .timer_sel  = LEDC_TIMER_1,
        .duty       = 0,
        .hpoint     = 0,
    };
    ledc_channel_config(&ledc_channel);
}
void LedcDriver::DoFirstInit(void)
{
    if (mFirstInit)
    {
        mFirstInit = false;
        
        ledc_timer_config_t ledc_timer = {
            .speed_mode      = LEDC_LOW_SPEED_MODE, // timer mode
            .duty_resolution = LEDC_TIMER_8_BIT,    // resolution of PWM duty
            .timer_num       = LEDC_TIMER_1,        // timer index
            .freq_hz         = 5000,                // frequency of PWM signal
            .clk_cfg         = LEDC_AUTO_CLK,       // Auto select the source clock
        };
        ledc_timer_config(&ledc_timer);
    }
}
void LedcDriver::SetDutyCycle(uint8_t dutyCycleOf255)
{
    ESP_LOGI(TAG, "SetDutyCycle to GPIO number %d to %u with channel %d", mGpioPin, dutyCycleOf255, mLedcChannel);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, mLedcChannel, dutyCycleOf255);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, mLedcChannel);
}