#include "freertos/FreeRTOS.h"

class TimerTick
{
    public:
        bool TimerTickHasElapsed(void);
        TickType_t TimerTicksRemaining(void);
        void TimerTickSetFromNow(uint32_t msFromNow);
        void TimerTickIncrement(uint32_t numMs);

    private:
        TickType_t _tick;
};