#pragma once

#include "driver/gpio.h"
#include "ColorFormat.h"
#include "driver/rmt.h"
#include "led_strip.h"


class Ws2812Driver
{
public:
    Ws2812Driver(gpio_num_t pin, uint16_t stripLength);
    void SetAllLeds(RgbColor_t rgb);
    void SetLed(RgbColor_t rgb, uint16_t ledIdx);

private:
    uint16_t mStripLength;
    led_strip_t* mStrip;
    rmt_channel_t mNextFreeRmtChannel = (rmt_channel_t)0;
    rmt_channel_t mRmtChannel;
    gpio_num_t mGpioPin;
};
