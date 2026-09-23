#pragma once

#include "config.h"

#include <cstdint>
#include "esp_err.h"

class MotorDriver
{
public:

    esp_err_t init(gpio_num_t pin);
    esp_err_t setSpeed(uint8_t percent);
    void stop();

private:

    gpio_num_t pin_ = Config::MOTOR_PIN;

    uint8_t speedPercent_ = 0;
    bool initialized_ = false;

};
