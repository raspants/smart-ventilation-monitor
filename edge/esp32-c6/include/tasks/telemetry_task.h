#pragma once

#include "esp_err.h"

class MqttService;
class FanSimulator;

struct TelemeteryTaskContext
{
    MqttService* mqtt;
    FanSimulator* fanSimulator;
};

esp_err_t startTelemetryTask(
    TelemeteryTaskContext* context
);

void telemetryTask(void* parameter);