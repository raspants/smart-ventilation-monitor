#pragma once

namespace TestConfig
{
    constexpr char MQTT_TOPIC[] =
            "smartvent/smartvent-01/telemetry";

    constexpr char MQTT_PAYLOAD[] = R"({
        "device_id":"vent-01",
        "temperature":22.5,
        "humidity":45.0,
        "fan_speed_rpm":1200,
        "fan_status":"running"
    })";

    constexpr char MQTT_COMMAND_TOPIC[] =
        "smartvent/vent-001/command";
}