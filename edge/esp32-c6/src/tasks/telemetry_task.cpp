#include "./tasks/telemetry_task.h"

#include "mqtt_service.h"
#include "fan_simulator.h"
#include "models.h"
#include "queues.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "[TELEMETRY_TASK]";

void telemetryTask(void* parameter)
{
    auto* context = static_cast<TelemeteryTaskContext*>(parameter);

    SensorReading reading{};

    while (true)
    {
        if (xQueueReceive(
            telemetryQueue,
            &reading,
            portMAX_DELAY) == pdTRUE)
        {
            const FanState& fanState =
                context->fanSimulator->getState();

            ESP_LOGI(TAG, "T=%.2f C RH=%.2f %% FAN=%u%% RPM=%u",
                     reading.temperature,
                     reading.humidity,
                     fanState.speedPercent,
                     fanState.rpm
            );
        }
    }
}

esp_err_t startTelemetryTask(TelemeteryTaskContext* context)
{
    BaseType_t result = xTaskCreate(
        telemetryTask,
        "telemetry",
        4096,
        context,
        2,
        nullptr
    );

    if (result != pdPASS)
    {
        return ESP_ERR_NO_MEM;
    }

    return ESP_OK;
}