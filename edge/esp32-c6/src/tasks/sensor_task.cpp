#include "./tasks/sensor_task.h"

#include "sht30_driver.h"
#include "models.h"
#include "queues.h"

#include <climits>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "[SENSOR_TASK]";

TaskHandle_t sensorTaskHandle = nullptr;

void sensorTask(void* parameter)
{
    auto* sht30 = static_cast<SHT30Driver*>(parameter);

    uint32_t measurementIntervalMs = 5000;

    while (true)
    {
        SensorReading reading{};
        
        esp_err_t err = sht30->read(reading);

        if (err == ESP_OK)
        {
            ESP_LOGI(TAG, "Temperature: %.2f C, Humidity: %.2f %%RH",
                     reading.temperature,
                     reading.humidity
            );

            if (xQueueSend(telemetryQueue, &reading, 0) != pdTRUE)
            {
                ESP_LOGW(TAG, "Telemetry queue full, measurement dropped");
            }

        } else {

            ESP_LOGE(TAG, "Sensor read failed: %s",
                     esp_err_to_name(err)
            );
        }

        uint32_t newIntervalMs = 0;

        if (xTaskNotifyWait(
            0,
            ULONG_MAX,
            &newIntervalMs,
            pdMS_TO_TICKS(measurementIntervalMs)
            ) == pdTRUE)
        {
            measurementIntervalMs = newIntervalMs * 1000U;

            ESP_LOGI(TAG, "Measurement interval updated to %lu ms",
                     static_cast<unsigned long>(
                        measurementIntervalMs
                     )
            );
        }
    }
}

esp_err_t startSensorTask(SHT30Driver* sht30)
{
    BaseType_t result = xTaskCreate(
        sensorTask,
        "sensor",
        3072,
        sht30,
        2,
        &sensorTaskHandle
    );

    if (result != pdPASS)
    {
        return ESP_ERR_NO_MEM;
    }

    return ESP_OK;
}