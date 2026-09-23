#include "esp_log.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi_service.h"
#include "mqtt_service.h"
#include "fan_simulator.h"
#include "queues.h"
#include "sht30_driver.h"
#include "motor_driver.h"

#include "./tasks/sensor_task.h"
#include "./tasks/telemetry_task.h"
#include "./tasks/command_task.h"
#include "./tasks/fan_hw_task.h"

#include "config.h"

#ifdef MQTT_TEST 1
#include "../test/test_config.h"
#endif

static const char* TAG = "[MAIN]";

static WiFiService wifi;
static MqttService mqtt;
static FanSimulator fanSimulator;
static SHT30Driver sht30;
static MotorDriver motor;

TelemeteryTaskContext telemetryContext{
    .mqtt = &mqtt,
    .fanSimulator = &fanSimulator
};


extern "C" void app_main() 
{

    ESP_LOGI(TAG, "Starting SmartVent edge");

    //============ NVS =====================
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    

    //============ QUEUES ==================
    ESP_ERROR_CHECK(startCommandQueue());
    ESP_ERROR_CHECK(startTelemetryQueue());

    //============ HARDWARE ================
    ESP_ERROR_CHECK(sht30.init());
    ESP_ERROR_CHECK(motor.init(Config::MOTOR_PIN));

    //============ TASKS ===================

    ESP_ERROR_CHECK(startSensorTask(&sht30));
    ESP_ERROR_CHECK(startTelemetryTask(&telemetryContext));
    ESP_ERROR_CHECK(startCommandTask(&fanSimulator));
    ESP_ERROR_CHECK(startFanTask(&motor));

    //============ COMMUNICATION ===========
    ESP_ERROR_CHECK(wifi.init());
    ESP_ERROR_CHECK(mqtt.init());
    
    ESP_LOGI(TAG, "SmartVent initialized");

#if MQTT_TEST == 1
    while (!mqtt.isConnected())
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    ESP_LOGI(TAG, "Waiting for MQTT connection...");
    ESP_ERROR_CHECK(
        mqtt.publish(
            TestConfig::MQTT_TOPIC, 
            TestConfig::MQTT_PAYLOAD, 
            1, 
            false
        )
    );

    ESP_ERROR_CHECK(
        mqtt.subscribe(
            TestConfig::MQTT_COMMAND_TOPIC,
            1
        )
    );
#endif    

}