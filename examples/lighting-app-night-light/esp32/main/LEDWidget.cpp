/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "LEDWidget.h"
#include "led_strip.h"
#include "esp_timer.h"
#include <cstring>
#include "esp_log.h"

static const char * TAG = "LEDWidget";

void LEDWidget::InitColor(gpio_num_t pin, uint16_t stripLength)
{
    LED_DATA ledConfig;
    ledConfig.rgbWs2812.ws2812 = new Ws2812Driver(pin, stripLength);
    ledConfig.rgbWs2812.stripLength = stripLength;
    Init(LED_RGB_WS2812, &ledConfig);
}
void LEDWidget::InitMono(gpio_num_t pin)
{
    LED_DATA ledConfig;
    ledConfig.monoPwm.ledc = new LedcDriver(pin);
    Init(LED_MONO_PWM, &ledConfig);
}
void LEDWidget::InitColorPwm(gpio_num_t pinR, gpio_num_t pinG, gpio_num_t pinB)
{
    LED_DATA ledConfig;
    ledConfig.rgbPwm.ledcR = new LedcDriver(pinR);
    ledConfig.rgbPwm.ledcG = new LedcDriver(pinG);
    ledConfig.rgbPwm.ledcB = new LedcDriver(pinB);
    Init(LED_RGB_PWM, &ledConfig);
}

void LEDWidget::Init(LED_TYPE ledType, LED_DATA* pLedData)
{
    memcpy(&mLedData, pLedData, sizeof(mLedData));
    mState      = false;
    mBrightness = UINT8_MAX;
    mLedType = ledType;
    mHue        = 90;
    mSaturation = 255;
    mBrightness = 10;
}

void LEDWidget::Set(bool state)
{
    ESP_LOGI(TAG, "Setting state to %d", state ? 1 : 0);
    if (state == mState)
    {
        return;
    }
    else if (state == true)
    {
        mTurnOnTimeMs = esp_timer_get_time();
        ESP_LOGI(TAG, "Turn on at %llu", mTurnOnTimeMs);
    }

    mState = state;

    DoSet();
}

void LEDWidget::Toggle()
{
    ESP_LOGI(TAG, "Toggling state to %d", !mState);
    mState = !mState;

    DoSet();
}

void LEDWidget::SetBrightness(uint8_t brightness)
{
    ESP_LOGI(TAG, "Setting brightness to %d", brightness);
    if (brightness == mBrightness)
        return;

    if (brightness != 1 || 
        esp_timer_get_time() - mTurnOnTimeMs > 500*1000)
    {
        mBrightness = brightness;
    }
    else
    {
        ESP_LOGI(TAG, "Ignoring brightness to %d. timeDelta = %llu", brightness, esp_timer_get_time() - mTurnOnTimeMs);
        return;
    }

    DoSet();
}

uint8_t LEDWidget::GetLevel()
{
    return this->mBrightness;
}

bool LEDWidget::IsTurnedOn()
{
    return this->mState;
}

void LEDWidget::SetColor(int16_t Hue, int16_t Saturation)
{
    ESP_LOGI(TAG, "Setting Color to Hue:%d, Sat:%d", Hue , Saturation);
    if (mLedType == LED_RGB_WS2812 || 
        mLedType == LED_RGB_PWM)
    {
        if (Hue < 0)
        {
            Hue = mHue;
        }
        if (Saturation < 0)
        {
            Saturation = mSaturation;
        }

        if (Hue != mHue || 
            Saturation != mSaturation)
        {
            mHue = Hue;
            mSaturation = Saturation;
            DoSet();
        }
    }
}

void LEDWidget::DoSet(void)
{
    uint8_t brightness = mState ? mBrightness : 0;

    switch (mLedType)
    {
        case LED_RGB_WS2812:   DoSetColor(brightness);      break;
        case LED_MONO_PWM:     DoSetMono(brightness);       break;
        case LED_RGB_PWM:      DoSetPwmColor(brightness);   break;
    }
}

void LEDWidget::DoSetColor(uint8_t brightness)
{
    
    mLedData.rgbWs2812.ws2812->SetAllLeds(GetRgb(brightness));
}

void LEDWidget::DoSetMono(uint8_t brightness)
{
    mLedData.monoPwm.ledc->SetDutyCycle(brightness);
}

void LEDWidget::DoSetPwmColor(uint8_t brightness)
{
    RgbColor_t rgb = GetRgb(brightness);
    mLedData.rgbPwm.ledcR->SetDutyCycle(rgb.r);
    mLedData.rgbPwm.ledcG->SetDutyCycle(rgb.g);
    mLedData.rgbPwm.ledcB->SetDutyCycle(rgb.b);
}
RgbColor_t LEDWidget::GetRgb(uint8_t brightness)
{
    HsvColor_t hsv = { (uint16_t)(mHue*359/255), (uint8_t)(mSaturation*100/255), brightness };
    return HsvToRgb(hsv);
}