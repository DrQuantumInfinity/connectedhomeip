#pragma once

#include "driver/gpio.h"
#include "driver/ledc.h"


class LedcDriver
{
public:
    LedcDriver(gpio_num_t pin);
    void SetDutyCycle(uint8_t dutyCycleOf255);

private:
    void DoFirstInit(void);

    bool mFirstInit = true;
    static inline ledc_channel_t mNextFreeLedcChannel = (ledc_channel_t)0;
    ledc_channel_t mLedcChannel;
    gpio_num_t mGpioPin;
};
