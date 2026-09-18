#include "sht30_driver.h"

#include "config.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "[SHT30]";

esp_err_t SHT30Driver::init()
{
    i2c_master_bus_config_t busConfig = {};

    busConfig.i2c_port = I2C_NUM_0;
    busConfig.sda_io_num = Config::I2C_SDA_PIN;
    busConfig.scl_io_num = Config::I2C_SCL_PIN;
    busConfig.clk_source = I2C_CLK_SRC_DEFAULT;
    busConfig.glitch_ignore_cnt = 7;
    busConfig.flags.enable_internal_pullup = true;

    esp_err_t err =
        i2c_new_master_bus(
            &busConfig,
            &busHandle_
        );

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to create I2C bus: %s", esp_err_to_name(err));
        return err;
    }
    
    ESP_LOGI(TAG, "Scanning I2C bus...");

    bool found = false;

    for (uint8_t address = 1; address < 127; ++address)
    {
        esp_err_t result =
            i2c_master_probe(busHandle_, address, 50);

        if (result == ESP_OK)
        {
            ESP_LOGI(
                TAG,
                "I2C device found at 0x%02X",
                address
            );

            found = true;
        }
    }

    if (!found)
    {
        ESP_LOGW(TAG, "No I2C devices found");
        return ESP_ERR_NOT_FOUND;
    }

    i2c_device_config_t deviceConfig = {};

    deviceConfig.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    deviceConfig.device_address = ADDRESS;
    deviceConfig.scl_speed_hz = 100000;

    err = i2c_master_bus_add_device(
        busHandle_,
        &deviceConfig,
        &deviceHandle_
    );

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to add SHT30: %s", esp_err_to_name(err));
        return err;
    }

    return ESP_OK;

}

esp_err_t SHT30Driver::read(SensorReading& reading)
{
    reading.valid = false;

    uint8_t command[2] = {
        0x24,
        0x00
    };

    esp_err_t err = i2c_master_transmit(
        deviceHandle_,
        command,
        sizeof(command),
        1000
    );

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, 
                 "Failed to start measurement: %s",
                 esp_err_to_name(err));
        return err;
    }

    // ESP_LOGI(TAG, "Measurement command sent");

    vTaskDelay(pdMS_TO_TICKS(50));

    uint8_t data[6] = {};

    err = i2c_master_receive(
        deviceHandle_,
        data,
        sizeof(data),
        1000
    );

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read measurement: %s",
                 esp_err_to_name(err)
        );
        return err;
    }

    // ESP_LOGI(TAG, "Raw data: %02X %02X %02X %02X %02X %02X",
    //          data[0],
    //          data[1],
    //          data[2],
    //          data[3],
    //          data[4],
    //          data[5]
    // );

    uint8_t temperatureCrc = calculateCrc(&data[0], 2);
    uint8_t humidityCrc = calculateCrc(&data[3], 2);

    // ESP_LOGI(TAG, "Temperature CRC: calculated=0x%02X received=0x%02X",
    //          temperatureCrc,
    //          data[2]
    // );

    // ESP_LOGI(TAG, "Humidity CRC: calculated=0x%02X received=0x%02X",
    //          humidityCrc,
    //          data[5]
    // );

    if (temperatureCrc != data[2])
    {
        ESP_LOGE(TAG, "Temperature CRC validation failed");
        return ESP_ERR_INVALID_CRC;
    }

    if (humidityCrc != data[5])
    {
        ESP_LOGE(TAG, "Humidity CRC validation failed");
        return ESP_ERR_INVALID_CRC;
    }

    uint16_t rawTemperature = 
        (static_cast<uint16_t>(data[0]) << 8) |
        data[1];

    uint16_t rawHumidity =
        (static_cast<uint16_t>(data[3]) << 8) |
        data[4];

    reading.humidity = 
        100.0f *
        static_cast<float>(rawHumidity) /
        65535.0f;

    reading.temperature = 
        -45.0f +
        (175.0f *
         static_cast<float>(rawTemperature) /
         65535.0f);

    reading.valid = true;

    // ESP_LOGI(TAG, "Humidity: %.2f %%RH", reading.humidity);
    // ESP_LOGI(TAG, "Temperature: %.2f C", reading.temperature);

    return ESP_OK;

}

uint8_t SHT30Driver::calculateCrc(
    const uint8_t* data,
    size_t length)
{
    uint8_t crc = 0xFF;

    for (size_t i = 0; i < length; ++i)
    {
        crc ^= data[i];

        for (uint8_t bit = 0; bit < 8; ++bit)
        {
            if (crc & 0x80)
            {
                crc = static_cast<uint8_t>(
                         (crc << 1) ^ 0x31
                );
            } else {

                crc <<= 1;
            }
        }
    }

    return crc;
}
