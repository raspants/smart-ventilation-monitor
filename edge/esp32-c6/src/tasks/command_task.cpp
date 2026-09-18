#include "./tasks/command_task.h"

#include "fan_simulator.h"
#include "models.h"
#include "queues.h"

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
                     )
            );

            fanSimulator->setSpeed(command.fanSpeedSetting);

            fanSimulator->update();

            FanState state = fanSimulator->getState();

            ESP_LOGI(TAG, "Fan updated: speed=%u%% rpm=%u running=%s",
                     state.speedPercent,
                     state.rpm,
                     state.running ? "true" : "false"
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