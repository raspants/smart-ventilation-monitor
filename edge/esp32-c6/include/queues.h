#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

extern QueueHandle_t fanCommandQueue;

esp_err_t startFanCommandQueue();

esp_err_t initQueues();