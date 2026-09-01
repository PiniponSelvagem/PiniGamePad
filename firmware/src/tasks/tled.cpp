#include "tasks/tled.hpp"
#include <pinicore.hpp>
#include "global.hpp"


void TaskLED::init() {
   m_led.init(PIN_LED_R, PIN_LED_G, PIN_LED_B);
}

bool TaskLED::start() {
    if (m_taskHandle != nullptr) {
        return false;
    }
    m_stopRequested = false;
    
    BaseType_t result = xTaskCreate(
        taskEntry,
        "LED",
        2048,
        this,
        1,
        &m_taskHandle
    );
    
    return (result == pdPASS);
}

void TaskLED::stop() {
    if (m_taskHandle == nullptr) {
        return;
    }
    m_stopRequested = true;
    
    while (m_taskHandle != nullptr) {
        delay(10);
    }
}

bool TaskLED::isRunning() const {
    return (m_taskHandle != nullptr);
}


void TaskLED::taskEntry(void *pvParameters) {
    TaskLED* self = static_cast<TaskLED*>(pvParameters);
    self->taskLoop();
    self->m_taskHandle = nullptr;
    vTaskDelete(nullptr);
}

void TaskLED::taskLoop() {
    uint8_t r=0, g=0, b=0;
    while (!m_stopRequested) {
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
        delay(50);
    }
}
