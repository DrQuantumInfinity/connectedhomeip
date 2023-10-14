#include "TimerTick.h"

static bool TimerTickMsbIsSet(TickType_t tick);

bool TimerTick::TimerTickHasElapsed(void)
{
    bool hasElapsed = false;
    if (_tick)
    {
        TickType_t ticksRemaining = _tick - xTaskGetTickCount();
        //Is the MSB set? We would have underflow if the timer has elapsed.
        hasElapsed = TimerTickMsbIsSet(ticksRemaining);
    }
    return hasElapsed;
}
TickType_t TimerTick::TimerTicksRemaining(void)
{
    TickType_t ticksRemaining = _tick - xTaskGetTickCount();
    //Is the MSB set? We would have underflow if the timer has elapsed.
    if (TimerTickMsbIsSet(ticksRemaining))
    {
        ticksRemaining = 0;
    }
    return ticksRemaining;
}
void TimerTick::TimerTickSetFromNow(uint32_t msFromNow)
{
    _tick = xTaskGetTickCount() + pdMS_TO_TICKS(msFromNow);
    if (_tick == 0)
    {
        _tick++;
    }
}
void TimerTick::TimerTickIncrement(uint32_t numMs)
{
    _tick += pdMS_TO_TICKS(numMs);
    if (_tick == 0)
    {
        _tick++;
    }
}
static bool TimerTickMsbIsSet(TickType_t tick)
{
    return tick & (1 << (sizeof(TickType_t)*8  - 1))
}