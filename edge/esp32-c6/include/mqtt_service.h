#pragma once

#include "esp_err.h"
#include "esp_event.h"
#include "mqtt_client.h"

class MqttService
{
public:

    MqttService() = default;
    esp_err_t init();
    bool isConnected() const;

    esp_err_t publish(
        const char* topic,
        const char* payload,
        int qos = 1,
        bool retain = false
    );

    esp_err_t subscribe(
        const char* topic,
        int qos = 1
    );

private:

    static void eventHandler(
        void* handlerArgs,
        esp_event_base_t base,
        int32_t eventId,
        void* eventData      
    );

    esp_mqtt_client_handle_t client_ = nullptr;
    bool connected_ = false;

};