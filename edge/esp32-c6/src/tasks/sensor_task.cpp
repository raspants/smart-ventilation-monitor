#include "./tasks/sensor_task.h"

#include "sht30_driver.h"
#include "models.h"
#include "queues.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "[SENSOR_TASK]";

void sensorTask(void* parameter)
{
    auto* sht30 = static_cast<SHT30Driver*>(parameter);

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

        } else {

            ESP_LOGE(TAG, "Sensor read failed: %s",
                     esp_err_to_name(err)
            );
        }

        if (xQueueSend(telemetryQueue, &reading, 0) != pdTRUE)
        {
            ESP_LOGW(TAG, "Telemetry queue full, measurement dropped");
        }
        
        vTaskDelay(pdMS_TO_TICKS(5000));
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
        nullptr
    );

    if (result != pdPASS)
    {
        return ESP_ERR_NO_MEM;
    }

    return ESP_OK;
}