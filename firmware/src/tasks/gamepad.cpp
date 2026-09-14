#include "tasks/gamepad.hpp"
#include <NimBLEDevice.h>

#define TAG_GAMEPAD  "gamepad"

static inline int16_t floatToInt16(float v) {
    v = fmaxf(-1.0f, fminf(1.0f, v));
    return (int16_t)(v * XBOX_STICK_MAX);
}
static inline uint16_t floatToInt16_trigger(float v) {
    v = fminf(fabsf(v), 1.0f);
    return (uint16_t)(v * XBOX_TRIGGER_MAX);
}


TaskGamepad::TaskGamepad() : m_bleHID(DEVICE_TYPE, DEVICE_MANUFACTURER) {}
TaskGamepad::~TaskGamepad() {
    deinit();
}

void TaskGamepad::init() {
    if (m_initialized) return;

    /* Watchdog setup */
    pinicore::watchdogSetup(WDTG_INTERNAL_TIMER_IN_SECONDS);
    pinicore::watchdogEnable();

    /* Hardware setup */
    pinMode(PIN_M0_LSB, INPUT_PULLUP);
    pinMode(PIN_M1_RSB, INPUT_PULLUP);
    m_dualAxisLS.init(PIN_AXIS_LS_A0, PIN_AXIS_LS_A1);
    m_dualAxisRS.init(PIN_AXIS_RS_A0, PIN_AXIS_RS_A1);
    m_dualAxisLTRT.init(PIN_AXIS_LT, PIN_AXIS_RT);
    m_dualAxisLS.calibrate();
    m_dualAxisRS.calibrate();
    m_dualAxisLTRT.calibrate();

    /* Gamepad setup */
#ifdef XBOX_SERIES_X
    m_gamepadConfig = new XboxSeriesXControllerDeviceConfiguration();
#else
    m_gamepadConfig = new XboxOneSControllerDeviceConfiguration();
#endif

    /* Bluetooth setup */
    BLEHostConfiguration hostConfig = m_gamepadConfig->getIdealHostConfiguration();
    m_gamepad = new XboxGamepadDevice(m_gamepadConfig);
    m_bleHID.addDevice(m_gamepad);
    m_bleHID.begin(hostConfig);

    LOG_I(TAG_GAMEPAD, "BLE configuration: [VID: 0x%x] [PID: 0x%x]", hostConfig.getVid(), hostConfig.getPid());
    m_initialized = true;
}

void TaskGamepad::deinit() {
    if (!m_initialized) return;
    
    pinicore::watchdogDisable();
    m_bleHID.end();
    delete m_gamepad;
    delete m_gamepadConfig;
    m_initialized = false;
}

void TaskGamepad::loop() {
    pinicore::watchdogIamAlive();

    if (m_bleHID.isConnected()) {
        updateInput_axis();
        updateInput_buttons();   
        m_gamepad->sendGamepadReport();
    }
    
    delay(10);  // Later this value might be changed after input lag testing
}


void TaskGamepad::disconnect() {
    NimBLEDevice::deleteAllBonds();
    NimBLEServer* server = NimBLEDevice::getServer();
    server->disconnect(0);
    NimBLEDevice::getAdvertising()->start();
}

void TaskGamepad::updateInput_axis() {
    float x0, y0, x1, y1, x2, y2;
    m_dualAxisLS.pullEvents();
    m_dualAxisRS.pullEvents();
    m_dualAxisLTRT.pullEvents();
    m_dualAxisLS.getAxis(x0, y0);
    m_dualAxisRS.getAxis(x1, y1);
    m_dualAxisLTRT.getAxis(x2, y2);
    m_gamepad->setLeftThumb(floatToInt16(x0), floatToInt16(y0));
    m_gamepad->setRightThumb(floatToInt16(x1), floatToInt16(y1));
    m_gamepad->setLeftTrigger(floatToInt16_trigger(x2));
    m_gamepad->setRightTrigger(floatToInt16_trigger(y2));
}

void TaskGamepad::updateInput_buttons() {
    bool pressed0 = (digitalRead(PIN_M0_LSB) == LOW);
    bool pressed1 = (digitalRead(PIN_M1_RSB) == LOW);
    if (pressed0) {
        m_gamepad->press(XBOX_BUTTON_LS);
    }
    else {
        m_gamepad->release(XBOX_BUTTON_LS);
    }
    if (pressed1) {
        m_gamepad->press(XBOX_BUTTON_RS);
    }
    else {
        m_gamepad->release(XBOX_BUTTON_RS);
    }
}



float TaskGamepad::normalizeAxis(int raw, int center, int deadzone) {
    int v = raw - center;

    if (abs(v) < deadzone) return 0.0f;

    if (v > 0) {
        return (float)(v - deadzone) / (2048.0f - deadzone);
    } else {
        return (float)(v + deadzone) / (2048.0f - deadzone);
    }
}

int16_t TaskGamepad::toXBOX(float v) {
    if (v > 1.0f) v = 1.0f;
    if (v < -1.0f) v = -1.0f;
    
    return (int16_t)(v * 32767);
}


/*
void TaskGamepad::setupGamepad() {
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
*/
