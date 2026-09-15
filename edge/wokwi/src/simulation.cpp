#include "simulation.h"

static DeviceState state;
static SimulationSettings settings;

void simulation_init()
{
    settings.fan_speed_setting = 68;
    settings.measurement_interval = 6;

    state.temperature = 22.0f;
    state.humidity = 45.0f;
    state.fan_speed_setting = settings.fan_speed_setting;
    state.fan_speed_rpm = 0;
    state.fan_running = false;
}

void simulation_update()
{
    
}

DeviceState simulation_get_state()
{
    return state;
}

void simulation_set_settings(const SimulationSettings& new_settings)
{ 
    settings = new_settings;
    state.fan_speed_setting = settings.fan_speed_setting;
}