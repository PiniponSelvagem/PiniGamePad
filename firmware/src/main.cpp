#include "global.hpp"
#include "controller.hpp"
#include <pinicore.hpp>

using namespace pinicore;

#define TAG_MAIN	"main"

Controller controller;

void setup() {
    Serial.begin(115200);
    Serial.println();	// Just to start on a new clean line

    LOG_I(TAG_MAIN, "Firmware: [%d] | Build: [%s, %s]", FIRMWARE_VERSION, __DATE__, __TIME__);
    LOG_I(TAG_MAIN, "PiniGamepad, by: PiniponSelvagem");

    if (wasLastResetFatal()) {
        LOG_E(TAG_MAIN, "\e[1;33mI hit the 'panic' button! Maybe it's time for you to hit the 'learn to code' button?\e[0m");
    }

    /* Watchdog setup */
    watchdogSetup(WDTG_INTERNAL_TIMER_IN_SECONDS);
    watchdogEnable();

    controller.init();

    LOG_D(TAG_MAIN, "Setup complete");
}

void loop() {
    watchdogIamAlive();
    controller.loop();
}
