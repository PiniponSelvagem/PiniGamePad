#pragma once

#ifndef PINI_LEDS_H
#define PINI_LEDS_H

#include "global.hpp"

#define TAG_LEDS    "leds"

#define LEDS_MAX 2

// https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
enum ELed {
    LED_UNKOWN = 255,

    LED_1  =  2,    // built in ESP WiFi
};

class Leds {
    public:
        void setup() {
            INFO(TAG_LEDS, "Setup");
            pinMode(ELed::LED_1,  OUTPUT);
            allOFF();
        };

        void toggle(ELed pin) {
            write(pin, !read(pin));
        }

        void write(ELed pin, bool state) {
            digitalWrite(pin, state);
            m_leds[getIdx(pin)].state = state;
        }

        bool read(ELed pin) { return m_leds[getIdx(pin)].state; }

        uint8_t getIdx(ELed pin) { return _getIdx(pin); }
        ELed getPin(uint8_t idx) { return _getPin(idx); }

        void allOFF() {
            for (size_t i = 0; i < LEDS_MAX; i++) {
                write(m_leds[i].pin, false);
            }
        };


    private:
        uint8_t _getIdx(ELed pin) {
            switch (pin) {
                case ELed::LED_1:  return 0;
                default: return -1;
            }
        }
        ELed _getPin(uint8_t idx) {
            switch (idx) {
                case 0:  return ELed::LED_1;
                default: return ELed::LED_UNKOWN;
            }
        }


        struct Led {
            ELed pin;
            bool state;
        };
        Led m_leds[LEDS_MAX] = {
            { ELed::LED_1,  false },
        };
};

#endif // PINI_LEDS_H