#include "./tasks/telemetry_task.h"

#include "mqtt_service.h"
#include "fan_simulator.h"
#include "models.h"
#include "queues.h"

#include "config.h"

// #include <cmath>
#include <cJSON.h>

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
            FanState fanState = context->fanSimulator->getState();

            ESP_LOGI(TAG, "T=%.2f C RH=%.2f %% FAN=%u%% RPM=%u",
                     reading.temperature,
                     reading.humidity,
                     fanState.speedPercent,
                     fanState.rpm
            );

            if (!context->mqtt->isConnected())
            {
                ESP_LOGW(TAG, "MQTT not connected, telemetry dropped");
                continue;
            }

            // cJSON* root = cJSON_CreateObject();

            // if (root == nullptr)
            // {
            //     ESP_LOGE(TAG, "Failed to create JSON");
            //     continue;
            // }

            // cJSON_AddStringToObject(root, "device_id", DEVICE_ID);
            // cJSON_AddNumberToObject(root, "temperature", reading.temperature);
            // cJSON_AddNumberToObject(root, "humidity", reading.humidity);
            // cJSON_AddNumberToObject(root, "fan_speed_rpm", fanState.rpm);
            // cJSON_AddStringToObject(root, "fan_status", fanState.running ? "running" : "stopped");

            // char* payload = cJSON_PrintUnformatted(root);

            char payload[256];

            snprintf(
                payload,
                sizeof(payload),
                "{\"device_id\":\"%s\","
                "\"temperature\":%.2f,"
                "\"humidity\":%.2f,"
                "\"fan_speed_rpm\":%u,"
                "\"fan_status\":\"%s\"}",
                DEVICE_ID,
                reading.temperature,
                reading.humidity,
                fanState.rpm,
                fanState.running ? "running" : "stopped"
            );

            if (payload == nullptr)
            {
                ESP_LOGE(TAG, "Failed to serialize JSON");
                // cJSON_Delete(root);
                continue;
            }

            ESP_LOGI(TAG, "Telemetry: %s", payload);

            esp_err_t err = 
                context->mqtt->publish(
                    Config::MQTT_TELEMETRY_TOPIC,
                    payload,
                    1,
                    false
                );

            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Telemetry publish failed: %s",
                         esp_err_to_name(err)
                );
            }

            // cJSON_free(payload);
            // cJSON_Delete(root);
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