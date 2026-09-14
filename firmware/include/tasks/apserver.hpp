/**
* @file		apserver.hpp
* @brief	Task for Access Point web server control.
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

#ifndef PINICORE_TASK_APSERVER_H
#define PINICORE_TASK_APSERVER_H

#include <stdint.h>
#include <pinicore.hpp>
#include "tasks/itask.hpp"
#include <WebServer.h>

class TaskAPServer : public ITask {
    private:
        const char* getTaskName()      const override { return "APServer"; };
        uint32_t getTaskPriority()     const override { return 2;          };
        uint32_t getTaskCoreAffinity() const override { return 1;          };
        uint32_t getTaskStackSize()    const override { return 4096;       };

        /**
         * @brief   Acquire the Access Point web server resources, called once per start(), before the main loop.
         */
        void init() override;

        /**
         * @brief   Main loop for the Access Point web server task.
         */
        void loop() override;

        /**
         * @brief   Release everything acquired by init(), called once per start(), after the main loop.
         */
        void deinit() override;
        
        
        bool m_initialized = false;

        pinicore::WiFiComm m_wifi;
        WebServer m_webServer{80};
};

#endif // PINICORE_TASK_APSERVER_H