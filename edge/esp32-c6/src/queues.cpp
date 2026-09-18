#include "queues.h"
#include "models.h"

#include "esp_log.h"

static const char* TAG = "[QUEUES]";

QueueHandle_t commandQueue = nullptr;
QueueHandle_t telemetryQueue = nullptr;

esp_err_t startCommandQueue()
{
    commandQueue = xQueueCreate(5, sizeof(DeviceCommand));

    if (commandQueue == nullptr)
    {
        return ESP_ERR_NO_MEM;
    }

    return ESP_OK;
}

esp_err_t startTelemetryQueue()
{
    telemetryQueue = xQueueCreate(5, sizeof(SensorReading));

    if (telemetryQueue == nullptr)
    {
        return ESP_ERR_NO_MEM;
    }

    return ESP_OK;

}