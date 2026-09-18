#include "wifi_service.h"
#include "config.h"
#include "secrets.h"

#include <cstring>

#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"

static const char* TAG = "WiFiService";

esp_err_t WiFiService::init()
{
    ESP_LOGI(TAG, "Initializing WiFi");

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifiConfig = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&wifiConfig));

    ESP_ERROR_CHECK(
        esp_event_handler_register(
            WIFI_EVENT,
            ESP_EVENT_ANY_ID,
            &WiFiService::eventHandler,
            this
        )
    );

    wifi_config_t config = {};

    std::strncpy(
        reinterpret_cast<char*>(config.sta.ssid),
        Secrets::WIFI_SSID,
        sizeof(config.sta.ssid)
    );

    std::strncpy(
        reinterpret_cast<char*>(config.sta.password),
        Secrets::WIFI_PASSWORD,
        sizeof(config.sta.password)
    );

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &config));
    ESP_ERROR_CHECK(esp_wifi_start());

    return ESP_OK;
}

bool WiFiService::isConnected() const
{
    return connected_;
}

void WiFiService::eventHandler(
    void* arg,
    esp_event_base_t eventBase,
    int32_t eventId,
    void* eventData)
{
    auto *self = static_cast<WiFiService*>(arg);

    if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_START)
    {
        ESP_LOGI(TAG, "WiFi started, connecting...");
        esp_wifi_connect();
    }
    else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_DISCONNECTED)
    {
        self->connected_ = false;
        ESP_LOGW(TAG, "WiFi disconnected, reconnecting...");
        esp_wifi_connect();
    }
    else if (eventBase == IP_EVENT && eventId == IP_EVENT_STA_GOT_IP)
    {
        auto* event = static_cast<ip_event_got_ip_t*>(eventData);
        self->connected_ = true;
        ESP_LOGI(TAG, 
            "Connected, IP: " IPSTR,
            IP2STR(&event->ip_info.ip)
        );
    }
}