#include "mqtt.hpp"

#include "esp_log.h"
#include "mqtt_client.h"

static const char *TAG = "MQTT";

static const char *DEVICE_ID = "vent-01";
static const char *COMMAND_TOPIC = "smartvent/vent-01/command";

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
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGW(TAG, "Disconnected from MQTT broker");
        break;

    case MQTT_EVENT_DATA:
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
        break;

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

    esp_mqtt_client_handle_t client =
        esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(
        client,
        MQTT_EVENT_ANY,
        mqtt_event_handler,
        nullptr);

    esp_mqtt_client_start(client);
}