#include "global.hpp"
#include "controller.hpp"
#include <pinicore.hpp>

using namespace pinicore;

#define TAG_MAIN	"main"

Controller controller;

//WiFiComm wifi;
int i = 0;
#define PIN_PWM_R 14
#define PIN_PWM_G 15
#define PIN_PWM_B 13

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
    
    /*
    wifi.init();
    wifi.config("MATRIX", "PASSWORD");
    wifi.enable();
    wifi.connect();
    */

    controller.init();
    LOG_I(TAG_MAIN, "PIN_PWM_R");
    pinMode(PIN_PWM_R, OUTPUT);
    LOG_I(TAG_MAIN, "PIN_PWM_G");
    pinMode(PIN_PWM_G, OUTPUT);
    LOG_I(TAG_MAIN, "PIN_PWM_B");
    pinMode(PIN_PWM_B, OUTPUT);

    LOG_I(TAG_MAIN, "Setup complete");
}

void loop() {
    watchdogIamAlive();

    //wifi.maintain();
    controller.loop();

    /*
    // LED test
    analogWrite(PIN_PWM_R, i);
    analogWrite(PIN_PWM_G, i);
    analogWrite(PIN_PWM_B, ++i);
    LOG_I(TAG_MAIN, "%d", i);
    delay(10);

    if (i > 256) {
        uint64_t timesleep_ns = 10;
        timesleep_ns = timesleep_ns * 1000000LL; // sec * nano
        esp_sleep_enable_timer_wakeup(timesleep_ns);
        LOG_I(TAG_MAIN, "Sleeping...");
        pinMode(PIN_PWM_R,INPUT);
        pinMode(PIN_PWM_G,INPUT);
        pinMode(PIN_PWM_B,INPUT);
        delay(1000);
        esp_deep_sleep_start();
    }
    */
}
