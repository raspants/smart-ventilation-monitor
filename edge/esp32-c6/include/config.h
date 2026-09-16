#pragma once

// =============================
// Tests
// =============================
#define MQTT_TEST 1
#define SENSOR_TEST 0
#define FAN_SIM_TEST 0

// =============================
// Configuration
// =============================
namespace Config
{
    constexpr char WIFI_SSID[] = "Tele2_333f71_2.4Ghz";
    constexpr char WIFI_PASSWORD[] = "qdzjtnwi";

    constexpr char DEVICE_ID[] = "smartvent-01";

    constexpr char MQTT_BROKER_URI[] = "mqtt://192.168.0.2:1883";
}

