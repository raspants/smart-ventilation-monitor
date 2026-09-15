#include "mqtt_service.h"

#include "esp_log.h"

#include "config.h"

static const char* TAG = "MqttService";

esp_err_t MqttService::init()
{
    ESP_LOGI(TAG, "Initializing MQTT client");
    esp_mqtt_client_config_t mqttConfig = {};

    mqttConfig.broker.address.uri = Config::MQTT_BROKER_URI;

    client_ = esp_mqtt_client_init(&mqttConfig);

    if (client_ == nullptr)
    {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return ESP_FAIL;
    }

    ESP_ERROR_CHECK(
        esp_mqtt_client_register_event(
            client_,
            MQTT_EVENT_ANY,
            &MqttService::eventHandler,
            this
        )
    );

    ESP_ERROR_CHECK(esp_mqtt_client_start(client_));
    return ESP_OK;
}

void MqttService::eventHandler(
    void* handlerArgs,
    esp_event_base_t base,
    int32_t eventId,
    void* eventData)
{
    auto* self = static_cast<MqttService*>(handlerArgs);

    switch (static_cast<esp_mqtt_event_id_t>(eventId))
    {
        case MQTT_EVENT_CONNECTED:
        {
            self->connected_ = true;
            ESP_LOGI(TAG, "MQTT connected");
            break;
        }
        case MQTT_EVENT_DISCONNECTED:
        {
            self->connected_ = false;
            ESP_LOGW(TAG, "MQTT disconnected");
            break;
        }
        case MQTT_EVENT_ERROR:
        {
            ESP_LOGE(TAG, "MQTT error");
            break;
        }
        default:
            break;
    }
}

bool MqttService::isConnected() const
{
    return connected_;
}