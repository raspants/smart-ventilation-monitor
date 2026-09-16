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
    auto* event = static_cast<esp_mqtt_event_handle_t>(eventData);

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
        case MQTT_EVENT_PUBLISHED:
        {
            ESP_LOGI(TAG, "MQTT message published, id=%d", event->msg_id);
            break;
        }
        case MQTT_EVENT_SUBSCRIBED:
        {
            ESP_LOGI(TAG, "MQTT subscribed, id=%d", event->msg_id);
            break;
        }
        case MQTT_EVENT_DATA:
        {
            ESP_LOGI(TAG, "MQTT message received");

            ESP_LOGI(TAG,
                "topic: %.*s",
                event->topic_len,
                event->topic
            );

            ESP_LOGI(TAG,
                "Payload: %.*s",
                event->data_len,
                event->data
            );
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

esp_err_t MqttService::publish(
    const char* topic,
    const char* payload,
    int qos,
    bool retain)
{
    if (client_ == nullptr)
    {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (!connected_)
    {
        ESP_LOGW(TAG, "Cannot publish, MQTT not connected");
        return ESP_ERR_INVALID_STATE;
    }

    int messageId = esp_mqtt_client_publish(
        client_,
        topic,
        payload,
        0,
        qos,
        retain ? 1 : 0
    );

    if (messageId < 0)
    {
        ESP_LOGE(TAG, "Failed to publish MQTT message");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG,
        "Published message, id=%d, topic=%s",
        messageId,
        topic
    );

    return ESP_OK;
}

esp_err_t MqttService::subscribe(
    const char* topic,
    int qos)
{
    if (client_ == nullptr)
    {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (!connected_)
    {
        ESP_LOGW(TAG, "Cannot subscribe, MQTT not connected");
        return ESP_ERR_INVALID_STATE;
    }

    int messageId = esp_mqtt_client_subscribe(
        client_,
        topic,
        qos
    );

    if (messageId < 0)
    {
        ESP_LOGE(TAG, "Failed to subscribe to %s", topic);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG,
        "Subscription requested, topic=%s, id=%d",
        topic,
        messageId
    );

    return ESP_OK;
}