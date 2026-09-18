#pragma once

#include "esp_err.h"
#include "driver/i2c_master.h"

class SHT30Driver
{
public:

    esp_err_t init();
    esp_err_t read(float& temperature, float& humidity);

private:

    i2c_master_bus_handle_t busHandle_ = nullptr;
    i2c_master_dev_handle_t deviceHandle_ = nullptr;

    static constexpr uint8_t ADDRESS = 0x44;

    static uint8_t calculateCrc(
        const uint8_t* data,
        size_t length
    );

};