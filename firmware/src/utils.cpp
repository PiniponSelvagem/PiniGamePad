#include "utils.hpp"
#include <Arduino.h>
#include <pinicore.hpp>

void logTaskStats(const char* tag, const char* taskName) {
    LOG_D(
        tag,
        "Stats: [task: %s] [core: %d] [heap: %u | %u/%u] [stack_marker: %u]",
        taskName,
        xPortGetCoreID(),
        ESP.getFreeHeap(),
        ESP.getMinFreeHeap(),
        ESP.getHeapSize(),
        uxTaskGetStackHighWaterMark(nullptr)
    );
}
