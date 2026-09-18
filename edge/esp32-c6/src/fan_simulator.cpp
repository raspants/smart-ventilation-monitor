#include "fan_simulator.h"

// #include "queues.h"
// #include "models.h"

#include "esp_log.h"

static const char* TAG = "[FAN_SIMULATOR]";

void FanSimulator::setSpeed(uint8_t percent)
{
    if (percent > 100)
    {
        percent = 100;
    }

    state_.speedPercent = percent;
}

void FanSimulator::update()
{
    state_.running = state_.speedPercent > 0;

    state_.rpm = (MAX_RPM * state_.speedPercent) / 100;
}

FanState FanSimulator::getState() const
{
    return state_;
}




