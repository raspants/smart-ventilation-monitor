#pragma once

#include <cstdint>

struct FanState
{
    uint8_t speedPercent = 0;
    uint16_t rpm = 0;
    float vibration = 0.0f;
    bool running = false;
};

struct SensorReading
{
    float temperature = 0.0f;
    float humidity = 0.0f;
    bool valid = false;
};

struct FanCommand
{
    uint8_t speedPercent = 0;
};
