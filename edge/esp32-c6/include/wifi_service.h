#pragma once

#include "esp_err.h"
#include "esp_event.h"

class WiFiService
{
public:

    WiFiService() = default;

    esp_err_t init();

    bool isConnected() const;

private:

    static void eventHandler(
        void* arg,
        esp_event_base_t eventBase,
        int32_t eventId,
        void* eventData
    );

    bool connected_ = false;

};