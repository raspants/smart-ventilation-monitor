#include "tasks/fan_hw_task.h"

#include "motor_driver.h"

#include <climits>
#include "esp_log.h"

static const char* TAG = "[FAN_TASK]";

TaskHandle_t fanTaskHandle = nullptr;

esp_err_t startFanTask(MotorDriver* motor)
{
    if (motor == nullptr)
    {
        return ESP_ERR_INVALID_ARG;
    }

    BaseType_t result = xTaskCreate(
        fanTask,
        "fan",
        2048,
        motor,
        2,
        &fanTaskHandle
    );

    if (result != pdPASS)
    {
        fanTaskHandle = nullptr;
        return ESP_ERR_NO_MEM;
    }

    return ESP_OK;
}

void fanTask(void* parameter)
{
    auto* motor =
        static_cast<MotorDriver*>(parameter);

    uint32_t speedPercent = 0;

    while (true)
    {
        if (xTaskNotifyWait(
            0,
            ULONG_MAX,
            &speedPercent,
            portMAX_DELAY
            ) == pdTRUE)
        {
            ESP_LOGI(TAG, "Fan speed command: %lu%%",
                     static_cast<unsigned long>(speedPercent)
            );

            esp_err_t err = 
                motor->setSpeed(
                    static_cast<uint8_t>(speedPercent)
                );

            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to set fan speed: %s",
                         esp_err_to_name(err)
                );
            }
        }
    }
}