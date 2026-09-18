#pragma once

#include "esp_err.h"

class FanSimulator;

esp_err_t startCommandTask(FanSimulator* FanSimulator);
void commandTask(void* parameter);