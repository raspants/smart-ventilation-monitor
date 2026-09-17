#pragma once

#include "models.h"

#include <cstdint>
class FanSimulator
{
public:

    void setSpeed(uint8_t percent);
    void update();

    FanState getState() const;

private:

    FanState state_;

};