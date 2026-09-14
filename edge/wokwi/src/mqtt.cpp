#include "mqtt.hpp"

#include "esp_log.h"
#include "mqtt_client.h"

static const char *TAG = "MQTT";

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
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGW(TAG, "Disconnected from MQTT broker");
        break;

    default:
        break;
    }
}

void mqtt_start()
{
    ESP_LOGI(TAG, "Starting MQTT client...");
}