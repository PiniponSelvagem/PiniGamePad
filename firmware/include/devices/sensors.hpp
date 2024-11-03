#pragma once

#ifndef PINI_SENSORS_H
#define PINI_SENSORS_H

#include "global.hpp"

#define TAG_SENSORS "sensors"

#define SENSORS_MAX 1

// https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
enum ESensor {
    SENSOR_UNKOWN = 255,

    // currently in use
    SENSOR_1 = 34,    // always INPUT_PULLDOWN
    
    //// OTHER POSSIBLE PINS - ESP SIM ////
    //SENSOR_1 = 36,      //  +-  > always INPUT_PULLDOWN
    //SENSOR_1 = 39,      //  +-  > always INPUT_PULLDOWN
    //SENSOR_1 = 35,      //  +-  > always INPUT_PULLUP
    //SENSOR_1 = 32,      //  +-  > if WIFI can be INPUT_PULLUP or INPUT_PULLDOWN, if GSM is always INPUT_PULLUP
    //SENSOR_1 = 33,      //  +-  > if WIFI can be INPUT_PULLUP or INPUT_PULLDOWN, if GSM is always INPUT_PULLUP
    //SENSOR_1 = 25,      //  OK  > can be INPUT_PULLUP or INPUT_PULLDOWN
    //SENSOR_1 = 26,      //  *w  > CANNOT BE USED, used by GSM modem, but can if WIFI with INPUT_PULLUP or INPUT_PULLDOWN
    //SENSOR_1 = 27,      //  *w  > CANNOT BE USED, used by GSM modem, but can if WIFI with INPUT_PULLUP or INPUT_PULLDOWN
    //SENSOR_1 = 14,      //  OK  > can be INPUT_PULLUP or INPUT_PULLDOWN
    //SENSOR_1 = 12,      //  **  > CANNOT BE USED, if pulled HIGH it fails to boot, but can be INPUT_PULLUP or INPUT_PULLDOWN
    //
    //SENSOR_1 = 13,      //  OK  > can be INPUT_PULLUP or INPUT_PULLDOWN
    //SENSOR_1 = 23,      //  *w! > CANNOT BE USED, used by GSM modem, but can if WIFI with INPUT_PULLUP or INPUT_PULLDOWN (but GSM modem will turn ON or OFF depending on the input value)
    //SENSOR_1 = 22,      //  +-  > always INPUT_PULLUP
    //SENSOR_1 = 1,       //  **  > CANNOT BE USED, used by SERIAL PORT
    //SENSOR_1 = 3,       //  +-  > always INPUT_PULLUP (also related to SERIAL PORT, but seems that can be used since we dont send data to ESP over SERIAL)
    //SENSOR_1 = 21,      //  +-  > always INPUT_PULLUP
    //SENSOR_1 = 19,      //  OK  > can be INPUT_PULLUP or INPUT_PULLDOWN
    //SENSOR_1 = 18,      //  OK  > can be INPUT_PULLUP or INPUT_PULLDOWN
    //SENSOR_1 = 5,       //  OK  > can be INPUT_PULLUP or INPUT_PULLDOWN
    //SENSOR_1 = 4,       //  OK  > can be INPUT_PULLUP or INPUT_PULLDOWN
    //SENSOR_1 = 0,       //  OK  > can be INPUT_PULLUP or INPUT_PULLDOWN
    //SENSOR_1 = 2,       //  OK  > can be INPUT_PULLUP or INPUT_PULLDOWN
    //SENSOR_1 = 15,      //  OK  > can be INPUT_PULLUP or INPUT_PULLDOWN
};

class Sensors {
    public:
        void setup() {
            INFO(TAG_SENSORS "Setup");
            pinMode(ESensor::SENSOR_1, INPUT_PULLDOWN);
        };

        uint16_t readAnalog(ESensor pin)  { return analogRead(m_sensors[getIdx(pin)].pin);  }
        bool readDigital(ESensor pin)     { return digitalRead(m_sensors[getIdx(pin)].pin); }

        uint8_t getIdx(ESensor pin) { return _getIdx(pin); }
        ESensor getPin(uint8_t idx) { return _getPin(idx); }


    private:
        uint8_t _getIdx(ESensor pin) {
            switch (pin) {
                case ESensor::SENSOR_1:  return 0;
                default: return -1;
            }
        }
        ESensor _getPin(uint8_t idx) {
            switch (idx) {
                case 0:  return ESensor::SENSOR_1;
                default: return ESensor::SENSOR_UNKOWN;
            }
        }

        struct Sensor {
            ESensor pin;
        };
        Sensor m_sensors[SENSORS_MAX] = {
            { ESensor::SENSOR_1 },
        };
};

#endif // PINI_SENSORS_H