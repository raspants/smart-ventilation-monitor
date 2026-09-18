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

    static constexpr uint16_t MAX_RPM = 2400;
    FanState state_{};

};