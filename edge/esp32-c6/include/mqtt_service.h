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