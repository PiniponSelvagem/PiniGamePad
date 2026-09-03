#include "tasks/itask.hpp"
#include <pinicore.hpp>
#include "global.hpp"
#include "utils.hpp"

#define TAG_TASK  "itask"

//#define DEBUG_TASK_STATS     // Enable debug logging for task statistics
#define TASK_STATS_PERIOD_MS 10000

bool ITask::start() {
    if (m_running) {
        LOG_W(TAG_TASK, "Already running: %s", getTaskName());
        return false;
    }
    m_stopRequested = false;
    m_running = true;   // set before creating, the task can start running before this call returns

    BaseType_t result = xTaskCreatePinnedToCore(
        taskEntry,
        getTaskName(),
        getTaskStackSize(),
        this,
        getTaskPriority(),
        nullptr,
        getTaskCoreAffinity()
    );
    if (result != pdPASS) {
        m_running = false;
        LOG_E(TAG_TASK, "Failed to create task: %s", getTaskName());
        return false;
    }
    LOG_D(TAG_TASK, "Started task: %s", getTaskName());
    return true;
}

void ITask::stop() {
    if (!m_running) {
        return;
    }
    m_stopRequested = true;
    LOG_D(TAG_TASK, "Stop requested: %s", getTaskName());

    if (xTaskGetCurrentTaskHandle() == m_taskHandle) {
        return;     // called from inside the task, waiting here would deadlock
    }
    while (m_running) {
        delay(10);
    }
}


void ITask::taskEntry(void *pvParameters) {
    ITask* self = static_cast<ITask*>(pvParameters);
    self->m_taskHandle = xTaskGetCurrentTaskHandle();
    self->taskLoop();
    LOG_D(TAG_TASK, "Stopped task: %s", self->getTaskName());
    self->m_taskHandle = nullptr;
    self->m_running = false;    // last write, start() is allowed again from here on
    vTaskDelete(nullptr);
}

void ITask::taskLoop() {
    init();
#ifdef DEBUG_TASK_STATS
    TickType_t lastStatsTick = xTaskGetTickCount();
    const TickType_t statsPeriod = pdMS_TO_TICKS(TASK_STATS_PERIOD_MS);
#endif
    while (!m_stopRequested) {
        loop();
#ifdef DEBUG_TASK_STATS
        if ((xTaskGetTickCount()-lastStatsTick) >= statsPeriod) {
            lastStatsTick = xTaskGetTickCount();
            logTaskStats(TAG_TASK, getTaskName());
        }
#endif
    }
    deinit();
}