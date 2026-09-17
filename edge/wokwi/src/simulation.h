#pragma once

struct DeviceState
{
    float temperature;
    float humidity;
    int fan_speed_setting;
    int fan_speed_rpm;
    bool fan_running;
};

struct SimulationSettings
{
    int fan_speed_setting;
    int measurement_interval;
};

struct SimulationParamaters
{
    int natural_room_temperature;
    int natural_heat_gain;
    int fan_cooling_rate;
};

void simulation_init();

void simulation_update();

DeviceState simulation_get_state();

SimulationSettings simulation_get_settings();

void simulation_set_settings(const SimulationSettings &new_settings);
