#pragma once

#include "esp_err.h"

class SHT30Driver;

esp_err_t startSensorTask(SHT30Driver* sht30);
void sensorTask(void* parameter);