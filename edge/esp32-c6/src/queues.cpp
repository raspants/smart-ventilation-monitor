#include "queues.h"
#include "models.h"

#include "esp_log.h"

static const char* TAG = "[QUEUES]";

QueueHandle_t fanCommandQueue = nullptr;

esp_err_t startFanCommandQueue()
{
    fanCommandQueue = xQueueCreate(
        5, 
        sizeof(FanCommand)
    );

    if (fanCommandQueue == nullptr)
    {
        return ESP_ERR_NO_MEM;
    }

    return ESP_OK;
}