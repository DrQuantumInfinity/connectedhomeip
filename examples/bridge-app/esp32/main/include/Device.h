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

#include <functional>
#include <stdbool.h>
#include <stdint.h>

class Device
{
public:
    Device();
    virtual ~Device() {}
    uint16_t GetIndex() {return _index;}
    bool IsReachable(void) {return _reachable;}
    void SetReachable(bool reachable);

private:
    uint16_t _index;
    bool _reachable;
    static inline bool _indexList[CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT] = {0};
};
