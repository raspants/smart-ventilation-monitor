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
    
    #define DEVICE_ID "vent-01"

    constexpr char MQTT_COMMAND_TOPIC[] = "smartvent/" DEVICE_ID "/command";
    constexpr char MQTT_TELEMETRY_TOPIC[] = "smartvent/" DEVICE_ID "/telemetry"; 
        
    constexpr char MQTT_BROKER_URI[] = "mqtt://192.168.0.2:1883";


}

