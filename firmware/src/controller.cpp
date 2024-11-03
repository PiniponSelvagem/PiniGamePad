#include "controller.hpp"
#include "data/memory.hpp"
#include "utils/utils.hpp"


#define TAG_CONTROLLER  "controller"

#define LED_TOGGLE_MS   500

#define TIME_HOUR (60 * 60)
#define TIME_TO_SEND_FLOW_IN_MILLIS (TIME_HOUR * 1000)

#define MAX_TIME_NO_NETWORK (2 * 60 * 1000)

#define MQTT_RETRY_COUNT_MAX 5
#define MQTT_RETRY_GRACE_PERIOD (10 * 1000)


void Controller::setup() {
    INFO(TAG_CONTROLLER, "Setup start");
    /* Hardware setup */
	m_leds.setup();
	m_sensors.setup();

    INFO(TAG_CONTROLLER, "Setup completed");
}

void Controller::loop() {
}

void Controller::setRTC(long timestamp) {
	struct timeval tv;
	tv.tv_sec = timestamp;
	tv.tv_usec = 0;
	settimeofday(&tv, NULL);
	DEBUG(TAG_CONTROLLER, "Setup RTC with timestamp: %li", timestamp);
}