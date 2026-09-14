/**
* @file		led.hpp
* @brief	Task for RGB LED control.
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

#ifndef PINICORE_TASK_LED_H
#define PINICORE_TASK_LED_H

#include <stdint.h>
#include "tasks/itask.hpp"
#include "ledrgb.hpp"

class TaskLED : public ITask {
    private:
        const char* getTaskName()      const override { return "LED"; };
        uint32_t getTaskPriority()     const override { return 1;     };
        uint32_t getTaskStackSize()    const override { return 2048;  };

        /**
         * @brief   Acquire the LED resources, called once per start(), before the main loop.
         */
        void init() override;

        /**
         * @brief   Main loop for the LED task.
         */
        void loop() override;
        
        
        bool m_initialized = false;

        LedRGB m_led;

        /* LOOP variables */
        uint8_t r=255, g=0, b=0;
        TickType_t lastTickTime = 0;
};

#endif // PINICORE_TASK_LED_H