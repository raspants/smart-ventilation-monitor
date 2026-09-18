#include "tasks/fan_task.h"

#include "models.h"
#include "queues.h"
#include "fan_simulator.h"

#include "esp_log.h"

static const char* TAG = "FanTask";

void fanSimulationTask(void* parameter)
{
    
    auto* fanSimulator =
        static_cast<FanSimulator*>(parameter);
    
    FanCommand command{};

    while (true)
    {
        if (xQueueReceive(
                fanCommandQueue,
                &command,
                portMAX_DELAY) == pdTRUE)
        {
            fanSimulator->setSpeed(command.speedPercent);

            ESP_LOGI(TAG,
                     "Fan config updated: speed=%u%% interval=%lu ms",
                     command.speedPercent,
                     command.measurementIntervalMs
            );

            while (xQueueReceive(
                    fanCommandQueue,
                    &command,
                    pdMS_TO_TICKS(command.measurementIntervalMs))
                    != pdTRUE)
            {
                const FanState& state =
                    fanSimulator->getState();

                ESP_LOGI(TAG,
                    "Fan measurement: speed=%u%% rpm=%u running=%s",
                    state.speedPercent,
                    state.rpm,
                    state.running ? "true" : "false")
            };
        }

        fanSimulator->setSpeed(command.speedPercent);
    }
}

