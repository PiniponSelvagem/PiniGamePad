#pragma once

#ifndef _PINICONTROLLER_GAMEPAD_H_
#define _PINICONTROLLER_GAMEPAD_H_

#include <pinicore.hpp>
#include <BleCompositeHID.h>
#include <XboxGamepadDevice.h>

#include "global.hpp"
#include "dualaxis.hpp"

class Controller {
    public:
        Controller();
        ~Controller() = default;

        /**
         * @brief   Initialize the controller.
         */
        void init();

        /**
         * @brief   The controller basic loop code.
         */
        void loop();


    private:
        /**
         * @brief   Disconnects from current connected device and removes it from known devices, making it possible for new device pairing.
         */
        void disconnect();

        float normalizeAxis(int raw, int center, int deadzone);
        int16_t toXBOX(float v);


        float m_centerX, m_centerY;

        XboxGamepadDevice *m_gamepad;
        BleCompositeHID m_bleHID;

        DualAxis m_dualAxis0;
        DualAxis m_dualAxis1;
        //DualAxis m_dualAxis2;
};


/* Works on Windows / Android */
/*
void loopGamepad() {
    if (!m_gamepad.isConnected()) return;

    int btn = digitalRead(14);
    
    if (btn == LOW) {
        m_gamepad.press(BUTTON_4);
    }
    else {
        m_gamepad.release(BUTTON_4);
    }

    m_gamepad.sendReport();
    delay(10);
}
*/

/* Works on Android */
/*
int lastState = HIGH;
void loopGamepad() {
    if (!m_gamepad.isConnected()) return;

    int currentState = digitalRead(14);
    if (currentState != lastState) {
        if (currentState == LOW) {
            m_gamepad.press(BUTTON_4);
        } else {
            m_gamepad.release(BUTTON_4);
        }
        lastState = currentState;
        TRACE(TAG_MAIN, "button changed");
        m_gamepad.sendReport();
    }
    delay(10);
}
*/

#endif // _PINICONTROLLER_GAMEPAD_H_