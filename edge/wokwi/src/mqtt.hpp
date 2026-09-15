#pragma once

void mqtt_start();

void mqtt_publish_telemetry(
    float temperature,
    float humidity,
    int fan_speed_rpm,
    const char *fan_status);