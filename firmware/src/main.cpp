#include <pinicore.hpp>
#include "global.hpp"
#include "tasks/led.hpp"
#include "tasks/gamepad.hpp"
#include "tasks/apserver.hpp"

using namespace pinicore;

#define TAG_MAIN	"main"

TaskLED taskLED;
TaskGamepad taskGamepad;
TaskAPServer taskAPServer;

void setup() {
    Serial.begin(115200);
    Serial.println();	// Just to start on a new clean line

    LOG_I(TAG_MAIN, "Firmware: [%d] | Build: [%s, %s]", FIRMWARE_VERSION, __DATE__, __TIME__);
    LOG_I(TAG_MAIN, "PiniGamepad, by: PiniponSelvagem");

    if (wasLastResetFatal()) {
        LOG_E(TAG_MAIN, "\e[1;33mI hit the 'panic' button! Maybe it's time for you to hit the 'learn to code' button?\e[0m");
    }

    /* Tasks */
    taskLED.start();
    taskGamepad.start();
    taskAPServer.start();

    vTaskDelete(NULL);
}
void loop() {}
