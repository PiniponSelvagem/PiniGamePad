/**
* @file		itask.hpp
* @brief	Interface for tasks.
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

#ifndef PINICORE_ITASK_H
#define PINICORE_ITASK_H

#include <stdint.h>
#include "ledrgb.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class ITask {
    public:
        /**
         * @brief   Destructor for the task interface.
         * @warning Stop the task before destroying iTask instance.
         */
        virtual ~ITask() = default;

        /**
         * @brief   Start the task.
         * @return  True if the task was successfully started, false if it was already running or creation failed.
         */
        bool start();

        /**
         * @brief   Request to stop the task and block until it has fully stopped.
         * @note    If called from inside the task itself, it only requests the stop and returns immediately.
         */
        void stop();

        /**
         * @brief   Check if the task is currently running.
         * @return  True if the task is running, false otherwise.
         */
        bool isRunning() const { return m_running; }


    private:
        /**
         * @brief   Get the name of the task.
         * @return  The name of the task.
         */
        virtual const char* getTaskName() const = 0;
        /**
         * @brief   Get the priority of the task.
         * @return  The priority of the task.
         */
        virtual uint32_t getTaskPriority() const = 0;
        /**
         * @brief   Get the core affinity of the task.
         * @return  The core affinity of the task.
         * @note    By default, the task has no specific core affinity.
         */
        virtual uint32_t getTaskCoreAffinity() const { return tskNO_AFFINITY; };
        /**
         * @brief   Get the stack size of the task.
         * @return  The stack size of the task.
         * @note    By default, the task has a stack size of 8192 bytes.
         */
        virtual uint32_t getTaskStackSize() const { return 8192; };

        /**
         * @brief   Entry point for the task.
         * @param   pvParameters Pointer to the task parameters.
         */
        static void taskEntry(void *pvParameters);

        /**
         * @brief   Main loop for the task.
         */
        void taskLoop();

        /**
         * @brief   Acquire the resources needed by the task, called once per start(), before the main loop.
         * @note    Optional.
         */
        virtual void init() {};

        /**
         * @brief   Main loop for the task.
         */
        virtual void loop() = 0;

        /**
         * @brief   Release everything acquired by init(), called once per start(), after the main loop.
         * @note    Optional.
         */
        virtual void deinit() {}
        

        TaskHandle_t m_taskHandle = nullptr;
        volatile bool m_running = false;
        volatile bool m_stopRequested = false;
};

#endif // PINICORE_ITASK_H