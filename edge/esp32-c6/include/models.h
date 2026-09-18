#pragma once

#include <cstdint>

struct FanState
{
    uint8_t speedPercent = 0;
    uint16_t rpm = 0;
    bool running = false;
};

struct SensorReading
{
    float temperature = 0.0f;
    float humidity = 0.0f;
    bool valid = false;
};


struct DeviceCommand
{
    uint8_t fanSpeedSetting = 0;
    uint32_t measurementIntervalMs = 5000;
};


