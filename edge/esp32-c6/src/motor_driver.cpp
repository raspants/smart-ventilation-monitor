#include "motor_driver.h"

#include "driver/ledc.h"
#include "esp_log.h"

static const char* TAG = "[MOTOR_DRIVER]";

static constexpr ledc_mode_t LEDC_MODE = LEDC_LOW_SPEED_MODE;

static constexpr ledc_timer_t LEDC_TIMER = LEDC_TIMER_0;

static constexpr ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_0;

static constexpr ledc_timer_bit_t LEDC_RESOLUTION = LEDC_TIMER_10_BIT;

static constexpr uint32_t PWM_FREQUENCY = 20000;
static constexpr uint32_t MAX_DUTY = 1023;

esp_err_t MotorDriver::init(gpio_num_t pin)
{
    pin_ = pin;

    ledc_timer_config_t timerConfig{};
    timerConfig.speed_mode = LEDC_MODE;
    timerConfig.timer_num = LEDC_TIMER;
    timerConfig.duty_resolution = LEDC_RESOLUTION;
    timerConfig.freq_hz = PWM_FREQUENCY;
    timerConfig.clk_cfg = LEDC_AUTO_CLK;

    esp_err_t err = ledc_timer_config(&timerConfig);

    if (err != ESP_OK)
    {
        return err;
    }

    ledc_channel_config_t channelConfig{};
    channelConfig.gpio_num = pin_;
    channelConfig.speed_mode = LEDC_MODE;
    channelConfig.channel = LEDC_CHANNEL;
    channelConfig.intr_type = LEDC_INTR_DISABLE;
    channelConfig.timer_sel = LEDC_TIMER;
    channelConfig.duty = 0;
    channelConfig.hpoint = 0;

    err = ledc_channel_config(&channelConfig);

    if (err != ESP_OK)
    {
        return err;
    }

    initialized_ = true;

    ESP_LOGI(TAG, "Motor initialized on GPIO %d", pin_);

    return ESP_OK;
}

esp_err_t MotorDriver::setSpeed(uint8_t percent)
{
    if (!initialized_)
    {
        return ESP_ERR_INVALID_STATE;
    }

    if (percent > 100)
    {
        percent = 100;
    }

    uint32_t duty = (MAX_DUTY * percent) / 100;

    esp_err_t err =
        ledc_set_duty(
            LEDC_MODE,
            LEDC_CHANNEL,
            duty
        );

    if (err != ESP_OK)
    {
        return err;
    }

    err = 
        ledc_update_duty(
            LEDC_MODE,
            LEDC_CHANNEL
        );

    if (err != ESP_OK)
    {
        return err;
    }

    speedPercent_ = percent;

    ESP_LOGI(TAG, "Motor speed=%u%% duty=%lu",
             speedPercent_,
             static_cast<unsigned long>(duty)
    );

    return ESP_OK;
}

void MotorDriver::stop()
{
    if (initialized_)
    {
        setSpeed(0);
    }
}