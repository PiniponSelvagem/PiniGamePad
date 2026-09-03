#include "tasks/led.hpp"
#include <pinicore.hpp>
#include "global.hpp"
#include "utils.hpp"

#define TAG_TASK_LED  "task_led"

void TaskLED::init() {
    if (m_initialized) return;
    m_led.init(PIN_LED_R, PIN_LED_G, PIN_LED_B);
    r = 255; g = 0; b = 0;
    lastTickTime = xTaskGetTickCount();
    m_initialized = true;
}

void TaskLED::loop() {
    if ((r == 255) && (b == 0) && (g < 255)) {
        ++g; // Red -> Yellow -> Green
    }
    else if ((g == 255) && (b == 0) && (r > 0)) {
        --r; // Green
    }
    else if ((g == 255) && (r == 0) && (b < 255)) {
        ++b; // Green -> Cyan -> Blue
    }
    else if ((b == 255) && (r == 0) && (g > 0)) {
        --g; // Blue
    }
    else if ((b == 255) && (g == 0) && (r < 255)) {
        ++r; // Blue -> Magenta -> Red
    }
    else if ((r == 255) && (g == 0) && (b > 0)) {
        --b; // Back to Red
    }
    m_led.set(r, g, b);
    //LOG_D(TAG_TASK_LED, "LED color: R=%d, G=%d, B=%d", r, g, b);
    vTaskDelayUntil(&lastTickTime, pdMS_TO_TICKS(50));
}
