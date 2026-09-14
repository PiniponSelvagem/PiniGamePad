#include "tasks/apserver.hpp"
#include <pinicore.hpp>
#include "global.hpp"
#include "utils.hpp"

#define TAG_TASK_APSERVER  "task_apserver"

void TaskAPServer::init() {
    if (m_initialized) return;
    m_wifi.init();
    m_wifi.enable();
    m_wifi.configAP("PiniGamepad", "PINI666pt", false);
    m_wifi.connectAP();
    m_webServer.begin();
    m_initialized = true;
}

void TaskAPServer::loop() {
    m_webServer.handleClient();
    taskYIELD();
}

void TaskAPServer::deinit() {
    if (!m_initialized) return;
    m_webServer.stop();
    m_wifi.disable();
    m_initialized = false;
}
