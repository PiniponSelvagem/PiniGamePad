/**
* @file		tled.hpp
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
#include "ledrgb.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class TaskLED {
    public:
        /**
         * @brief   Initialize LED RGB pins.
         * @note    This function must be called prior to any other functions.
         */
        void init();

        /**
         * @brief   Start the LED task.
         * @return  True if the task was successfully started, false otherwise.
         */
        bool start();

        /**
         * @brief   Request to stop the LED task.
         */
        void stop();

        /**
         * @brief   Check if the LED task is currently running.
         * @return  True if the task is running, false otherwise.
         */
        bool isRunning() const;


    private:
        /**
         * @brief   Entry point for the LED task.
         * @param   pvParameters Pointer to the task parameters.
         */
        static void taskEntry(void *pvParameters);

        /**
         * @brief   Main loop for the LED task.
         */
        void taskLoop();
        

        TaskHandle_t m_taskHandle = NULL;
        LedRGB m_led;
        volatile bool m_stopRequested;
};

#endif // PINICORE_TASK_LED_H