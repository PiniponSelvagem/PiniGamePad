#include "controller.hpp"
#include "data/memory.hpp"
#include "utils/utils.hpp"
#include "data/board.hpp"


#define TAG_CONTROLLER  "controller"

#define GP_BUTTONS      8
#define GP_HAT_SWITCHES 4


void Controller::setup() {
    INFO(TAG_CONTROLLER, "Setup start");
    /* Hardware setup */
	m_leds.setup();
	m_sensors.setup();

    /* Gamemap setup */
    //setupGamepad();

    INFO(TAG_CONTROLLER, "Setup completed");
}

void Controller::loop() {
    loopGamepad();
}

void Controller::setRTC(long timestamp) {
	struct timeval tv;
	tv.tv_sec = timestamp;
	tv.tv_usec = 0;
	settimeofday(&tv, NULL);
	DEBUG(TAG_CONTROLLER, "Setup RTC with timestamp: %li", timestamp);
}


void Controller::setupGamepad() {
    m_gamepadConfig.setAutoReport(false);
    m_gamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);

    // Input
    m_gamepadConfig.setButtonCount(GP_BUTTONS);
    m_gamepadConfig.setHatSwitchCount(GP_HAT_SWITCHES);
    m_gamepadConfig.setAxesMin(0x0000);
    m_gamepadConfig.setAxesMax(0x7FFF);

    // Identification and Information
    m_gamepadConfig.setVid(DEVICE_VID);
    m_gamepadConfig.setPid(DEVICE_PID);
    m_gamepadConfig.setSerialNumber((char*)board.getUniqueId());
    m_gamepadConfig.setModelNumber((char*)DEVICE_MODEL_NUMBER);
    m_gamepadConfig.setFirmwareRevision((char*)DEVICE_FIRMWARE_REVISION);
    m_gamepadConfig.setSoftwareRevision((char*)DEVICE_SOFTWARE_REVISION);
    m_gamepadConfig.setHardwareRevision((char*)DEVICE_HARDWARE_REVISION);

    m_gamepad.begin(&m_gamepadConfig);  // changing m_gamepadConfig after the begin function has no effect, unless you call the begin function again
}

void Controller::loopGamepad() {
    if (!m_gamepad.isConnected()) return;
    m_gamepad.sendReport();
    
    TRACE(TAG_CONTROLLER, "sendReport");
    delay(1000);
}