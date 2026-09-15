#include "esp_log.h"
#include "nvs_flash.h"

#include "wifi_service.h"
#include "mqtt_service.h"

static const char* TAG = "Main";

static WiFiService wifi;
static MqttService mqtt;

extern "C" void app_main() 
{

    ESP_LOGI(TAG, "Starting SmartVent edge");

    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(wifi.init());
    ESP_ERROR_CHECK(mqtt.init());

    ESP_LOGI(TAG, "SmartVent initialized");

    // NVS
    // Wi-Fi
    // MQTT
    // Sensor
    // Queues
    // FreeRTOS tasks
}