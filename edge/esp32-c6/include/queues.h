#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

extern QueueHandle_t commandQueue;
extern QueueHandle_t telemetryQueue;

esp_err_t startCommandQueue();
esp_err_t startTelemetryQueue();

esp_err_t initQueues();