#pragma once

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class SHT30Driver;

extern TaskHandle_t sensorTaskHandle;

esp_err_t startSensorTask(SHT30Driver* sht30);
void sensorTask(void* parameter);