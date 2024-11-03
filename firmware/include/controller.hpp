#pragma once

#ifndef PINI_CONTROLLER_H
#define PINI_CONTROLLER_H

#include <Arduino.h>
#include <functional>

#include "data/memory.hpp"

#include "devices/leds.hpp"
#include "devices/sensors.hpp"

#include "utils/timer.hpp"


class Controller {
    public:
        void setup();
        void loop();

        void setRTC(long timestamp);
        bool isValidRTC();


    private:
        /* Hardware control */
        Leds m_leds;
        Sensors m_sensors;
};

#endif // PINI_CONTROLLER_H