

#include "Ws2812Driver.h"
#include "esp_timer.h"

static const char * TAG = "Ws2812Driver";

Ws2812Driver::Ws2812Driver(gpio_num_t pin, uint16_t stripLength)
{
    mGpioPin = pin;
    mStripLength = stripLength;
    mRmtChannel = mNextFreeRmtChannel;
    mNextFreeRmtChannel = (rmt_channel_t)((int)mNextFreeRmtChannel + 1);

    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(mGpioPin, (rmt_channel_t)mRmtChannel);
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(mStripLength, (led_strip_dev_t)config.channel);

    config.clk_div = 2;
    rmt_config(&config);
    rmt_driver_install(config.channel, 0, 0);

    mStrip = led_strip_new_rmt_ws2812(&strip_config);
}
void Ws2812Driver::SetAllLeds(RgbColor_t rgb)
{
    ESP_LOGI(TAG, "DoSetColor to GPIO number %d, old %d:%d:%d", mGpioPin, rgb.r, rgb.g, rgb.b);
    for (uint16_t ledIdx = 0; ledIdx < mStripLength; ledIdx++)
    {
        mStrip->set_pixel(mStrip, ledIdx, rgb.r, rgb.g, rgb.b);
        mStrip->refresh(mStrip, 100);
    }
}
void Ws2812Driver::SetLed(RgbColor_t rgb, uint16_t ledIdx)
{
    ESP_LOGI(TAG, "DoSetColor to GPIO number %d, old %d:%d:%d", mGpioPin, rgb.r, rgb.g, rgb.b);
    mStrip->set_pixel(mStrip, ledIdx, rgb.r, rgb.g, rgb.b);
    mStrip->refresh(mStrip, 100);
}