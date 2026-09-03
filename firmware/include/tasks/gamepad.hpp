/**
* @file		gamepad.hpp
* @brief	Task for Gamepad logic.
* @author	PiniponSelvagem
*
* Copyright(C) PiniponSelvagem
*
***********************************************************************
* Software that is described here, is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
**********************************************************************/

#pragma once

#ifndef _PINIGAMEPAD_TASK_GAMEPAD_H_
#define _PINIGAMEPAD_TASK_GAMEPAD_H_

#include <memory>
#include <pinicore.hpp>
#include <BleCompositeHID.h>
#include <XboxGamepadDevice.h>

#include "global.hpp"
#include "dualaxis.hpp"
#include "tasks/itask.hpp"

class TaskGamepad : public ITask {
    public:
        TaskGamepad();
        ~TaskGamepad() override;

    private:
        const char* getTaskName()      const override { return "Gamepad"; };
        uint32_t getTaskPriority()     const override { return 2;         };
        uint32_t getTaskCoreAffinity() const override { return 1;         };

        /**
         * @brief   Acquire the gamepad resources, called once per start().
         */
        void init() override;

        /**
         * @brief   Release everything acquired by init().
         */
        void deinit() override;

        /**
         * @brief   Main loop for the Gamepad task.
         */
        void loop() override;

        /**
         * @brief   Disconnects from current connected device and removes it from known devices, making it possible for new device pairing.
         */
        void disconnect();

        /**
         * @brief   Reads and updates the current state of all the axis inputs into XboxGamepadDevice.
         */
        void updateInput_axis();

        /**
         * @brief   Reads and updates the current state of all the button inputs into XboxGamepadDevice.
         */
        void updateInput_buttons();


        float normalizeAxis(int raw, int center, int deadzone);
        int16_t toXBOX(float v);


        float m_centerX, m_centerY;

        /* XBOX Gamepad */
        XboxGamepadDevice *m_gamepad;
    #ifdef XBOX_SERIES_X
        XboxSeriesXControllerDeviceConfiguration* m_gamepadConfig = new XboxSeriesXControllerDeviceConfiguration();
    #else
        XboxOneSControllerDeviceConfiguration* m_gamepadConfig = new XboxOneSControllerDeviceConfiguration();
    #endif
        BleCompositeHID m_bleHID;
        bool m_initialized = false;

        DualAxis m_dualAxisLS;      // Left Stick
        DualAxis m_dualAxisRS;      // Right Stick
        DualAxis m_dualAxisLTRT;    // Left Trigger (axisX) & Right Trigger (axisY)
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

#endif // _PINIGAMEPAD_TASK_GAMEPAD_H_