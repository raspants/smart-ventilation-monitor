#pragma once

#include "esp_err.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class MotorDriver;

extern TaskHandle_t fanTaskHandle;

esp_err_t startFanTask(MotorDriver* motor);
void fanTask(void* parameter);