#pragma once

#include "driver/gpio.h"

// =============================
// Tests
// =============================
#define MQTT_TEST 0
#define SENSOR_TEST 0
#define FAN_SIM_TEST 0

// =============================
// Configuration
// =============================
namespace Config
{    
    
    #define DEVICE_ID "vent-002"

    constexpr char MQTT_COMMAND_TOPIC[] = "smartvent/" DEVICE_ID "/command";
    constexpr char MQTT_TELEMETRY_TOPIC[] = "smartvent/" DEVICE_ID "/telemetry"; 
        
    constexpr char MQTT_BROKER_URI[] = "mqtt://192.168.0.2:1883";

    constexpr gpio_num_t I2C_SDA_PIN = GPIO_NUM_4;
    constexpr gpio_num_t I2C_SCL_PIN = GPIO_NUM_7;

    constexpr gpio_num_t MOTOR_PIN = GPIO_NUM_22;

}

