#include "mqtt.hpp"

#include "esp_log.h"
#include "mqtt_client.h"

#include "cJSON.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "MQTT";

static const char *DEVICE_ID = "vent-01";
static const char *COMMAND_TOPIC = "smartvent/vent-01/command";
static const char *TELEMETRY_TOPIC = "smartvent/vent-01/telemetry";

static esp_mqtt_client_handle_t mqtt_client = nullptr;
static TaskHandle_t telemetry_task_handle = nullptr;

static int fan_speed_setting = 0;
static int measurement_interval = 5;

static void publish_telemetry();
static void telemetry_task(void *parameter);

static void mqtt_event_handler(
    void *handler_args,
    esp_event_base_t base,
    int32_t event_id,
    void *event_data)
{
    auto event = static_cast<esp_mqtt_event_handle_t>(event_data);

    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "Connected to MQTT broker");

        esp_mqtt_client_subscribe(
            event->client,
            COMMAND_TOPIC,
            1);

        ESP_LOGI(TAG, "Subscribed to %s", COMMAND_TOPIC);

        if (telemetry_task_handle == nullptr)
        {
            xTaskCreate(
                telemetry_task,
                "telemetry_task",
                4096,
                nullptr,
                5,
                &telemetry_task_handle);

            ESP_LOGI(TAG, "Telemetry task started");
        }

        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGW(TAG, "Disconnected from MQTT broker");
        break;

    case MQTT_EVENT_DATA:
    {
        ESP_LOGI(
            TAG,
            "Topic: %.*s",
            event->topic_len,
            event->topic);

        ESP_LOGI(
            TAG,
            "Payload: %.*s",
            event->data_len,
            event->data);

        char *payload = strndup(
            event->data,
            event->data_len);

        if (payload == nullptr)
        {
            ESP_LOGE(TAG, "Failed to allocate payload");
            break;
        }

        cJSON *root = cJSON_Parse(payload);

        if (root == nullptr)
        {
            ESP_LOGE(TAG, "Invalid JSON payload");
            free(payload);
            break;
        }

        cJSON *fan_speed =
            cJSON_GetObjectItem(root, "fan_speed_setting");

        cJSON *interval =
            cJSON_GetObjectItem(root, "measurement_interval");

        if (cJSON_IsNumber(fan_speed))
        {
            int new_fan_speed = fan_speed->valueint;

            if (new_fan_speed >= 0)
            {
                fan_speed_setting = new_fan_speed;
                ESP_LOGI(TAG, "Fan speed setting updated to %d", fan_speed_setting);
            }
            else
            {
                ESP_LOGW(TAG, "Invalid fan speed setting: %d", new_fan_speed);
            }
        }

        if (cJSON_IsNumber(interval))
        {
            int new_interval = interval->valueint;

            if (new_interval > 0)
            {
                measurement_interval = new_interval;
                ESP_LOGI(TAG, "Measurement interval updated to %d", measurement_interval);
            }
            else
            {
                ESP_LOGW(TAG, "Invalid measurement interval: %d", new_interval);
            }
        }

        ESP_LOGI(
            TAG,
            "Updated settings: fan_speed=%d, interval=%d",
            fan_speed_setting,
            measurement_interval);

        cJSON_Delete(root);
        free(payload);

        break;
    }

    default:
        break;
    }
}

void mqtt_start()
{
    ESP_LOGI(TAG, "Starting MQTT client...");

    esp_mqtt_client_config_t mqtt_cfg = {};

    mqtt_cfg.broker.address.uri =
        "mqtt://host.wokwi.internal:1883";

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(
        mqtt_client,
        MQTT_EVENT_ANY,
        mqtt_event_handler,
        nullptr);

    esp_mqtt_client_start(mqtt_client);
}

static void publish_telemetry()
{
    const char *payload =
        "{\"device_id\":\"vent-01\","
        "\"temperature\":22.0,"
        "\"humidity\":45.0,"
        "\"fan_speed_rpm\":1200,"
        "\"fan_status\":\"running\"}";

    esp_mqtt_client_publish(
        mqtt_client,
        TELEMETRY_TOPIC,
        payload,
        0,
        1,
        0);

    ESP_LOGI(TAG, "Published telemetry");
}

static void telemetry_task(void *parameter)
{
    while (true)
    {
        publish_telemetry();

        vTaskDelay(
            pdMS_TO_TICKS(measurement_interval * 1000));
    }
}