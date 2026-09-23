#include "./tasks/command_task.h"

#include "fan_simulator.h"
#include "models.h"
#include "queues.h"

#include "./tasks/sensor_task.h"
#include "./tasks/fan_hw_task.h"

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "[COMMAND_TASK]";

void commandTask(void* parameter)
{
    auto* fanSimulator = static_cast<FanSimulator*>(parameter);

    DeviceCommand command{};

    while (true)
    {
        if (xQueueReceive(
            commandQueue,
            &command,
            portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGI(TAG, "Command received: fan=%u%% interval=%lu ms",
                     command.fanSpeedSetting,
                     static_cast<unsigned long>(
                        command.measurementIntervalMs
                     ) * 1000U
            );

            fanSimulator->setSpeed(command.fanSpeedSetting);

            fanSimulator->update();

            if (fanTaskHandle != nullptr)
            {
                xTaskNotify(fanTaskHandle,
                            command.fanSpeedSetting,
                            eSetValueWithOverwrite
                );
            }

            FanState state = fanSimulator->getState();

            ESP_LOGI(TAG, "Fan updated: speed=%u%% rpm=%u running=%s",
                     state.speedPercent,
                     state.rpm,
                     state.running ? "true" : "false"
            );
        }

        if (sensorTaskHandle != nullptr)
        {
            xTaskNotify(
                sensorTaskHandle,
                command.measurementIntervalMs,
                eSetValueWithOverwrite
            );

            ESP_LOGI(TAG, "Sensor interval changed to %lu ms",
                     static_cast<unsigned long>(
                        command.measurementIntervalMs * 1000U
                     )
            );
        }
    }
}

esp_err_t startCommandTask(FanSimulator* fanSimulator)
{
    BaseType_t result = xTaskCreate(commandTask, 
                                    "command",
                                    3072,
                                    fanSimulator,
                                    3,
                                    nullptr
    );

    if (result != pdPASS)
    {
        return ESP_ERR_NO_MEM;
    }

    return ESP_OK;
}