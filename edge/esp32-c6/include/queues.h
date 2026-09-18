#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

extern QueueHandle_t fanCommandQueue;
extern QueueHandle_t telemetryQueue;

esp_err_t startFanCommandQueue();
esp_err_t startTelemetryQueue();

esp_err_t initQueues();