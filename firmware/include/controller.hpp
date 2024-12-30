#pragma once

#ifndef PINI_CONTROLLER_H
#define PINI_CONTROLLER_H

#include <Arduino.h>
#include <BleGamepad.h>

#include "data/memory.hpp"

#include "devices/leds.hpp"
#include "devices/sensors.hpp"

#include "utils/timer.hpp"


class Controller {
    public:
        void setup();
        void loop();

        void setRTC(long timestamp);


    private:
        void setupGamepad();
        void loopGamepad();

        /* Hardware control */
        Leds m_leds;
        Sensors m_sensors;

        /* Gamepad */
        BleGamepad m_gamepad = BleGamepad(DEVICE_TYPE, DEVICE_MANUFACTURER);
        BleGamepadConfiguration m_gamepadConfig;
};

#endif // PINI_CONTROLLER_H