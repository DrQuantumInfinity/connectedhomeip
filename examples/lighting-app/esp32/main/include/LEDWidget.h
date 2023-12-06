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

#pragma once

#include "driver/gpio.h"
#include "esp_log.h"

#include "ColorFormat.h"
#include "driver/ledc.h"
#include "LedcDriver.h"
#include "driver/rmt.h"
#include "hal/ledc_types.h"
#include "led_strip.h"

class LEDWidget
{
public:
    void InitColor(gpio_num_t pin, rmt_channel_t rmtChannel);
    void InitMono(gpio_num_t pin);
    void InitColorPwm(gpio_num_t pinR, gpio_num_t pinB, gpio_num_t pinG);
    void Set(bool state);
    void Toggle(void);

    void SetBrightness(uint8_t brightness);
    void UpdateState();
    void SetColor(int16_t Hue, int16_t Saturation);
    uint8_t GetLevel(void);
    bool IsTurnedOn(void);

private:
    typedef enum
    {
        LED_RGB_WS2812,
        LED_MONO_PWM,
        LED_RGB_PWM,
    }LED_TYPE;

    typedef struct
    {
        led_strip_t * strip;
        gpio_num_t pin;
        rmt_channel_t rmtChannel;
    }RGB_WS2812;
    typedef struct
    {
        LedcDriver *ledc;
    }MONO_PWM;
    typedef struct
    {
        LedcDriver *ledcR;
        LedcDriver *ledcG;
        LedcDriver *ledcB;
    }RGB_PWM;
    typedef union
    {
        RGB_WS2812 rgbWs2812;
        MONO_PWM monoPwm;
        RGB_PWM rgbPwm;
    }LED_DATA;
    
    void Init(LED_TYPE ledType, LED_DATA* pLedConfig);
    void InitColorLocal(LED_DATA* pLedConfig);
    void InitMonoLocal(LED_DATA* pLedConfig);
    void InitColorPwmLocal(LED_DATA* pLedConfig);
    void DoSetColor(uint8_t brightness);
    void DoSetMono(uint8_t brightness);
    void DoSetPwmColor(uint8_t brightness);
    RgbColor_t GetRgb(uint8_t brightness);

    bool mState;
    uint8_t mBrightness;
    uint64_t mTurnOnTimeMs;
    LED_TYPE mLedType;

    uint8_t mHue;
    uint8_t mSaturation;
    
    LED_DATA mLedData;

    void DoSet(void);
};
