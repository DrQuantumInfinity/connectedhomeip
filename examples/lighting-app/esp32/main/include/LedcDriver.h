#pragma once

#include "driver/gpio.h"
#include "esp_log.h"

#include "driver/ledc.h"
#include "hal/ledc_types.h"


class LedcDriver
{
public:
    LedcDriver(gpio_num_t pin);
    void SetDutyCycle(uint8_t dutyCycleOf255);

private:
    void DoFirstInit(void);

    bool mFirstInit = true;
    ledc_channel_t mNextFreeLedcChannel = (ledc_channel_t)0;
    ledc_channel_t mLedcChannel;
    gpio_num_t mGpioPin;
};
