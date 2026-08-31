#include "controller.hpp"

using namespace pinicore;

#define TAG_CONTROLLER  "controller"

#define AXIS0_VRX 36   // VP
#define AXIS0_VRY 39   // VN
#define AXIS0_SW  26

#define AXIS1_VRX 34
#define AXIS1_VRY 35
#define AXIS1_SW  27

//#define AXIS2_VRX 32
//#define AXIS2_VRY 33
//#define AXIS2_SW  27

Controller::Controller() : m_bleHID(DEVICE_TYPE, DEVICE_MANUFACTURER) {}

void Controller::init() {
    LOG_D(TAG_CONTROLLER, "Setup start");

    /* Hardware setup */
    pinMode(AXIS0_SW, INPUT_PULLUP);
    pinMode(AXIS1_SW, INPUT_PULLUP);
    m_dualAxis0.init(AXIS0_VRX, AXIS0_VRY);
    m_dualAxis1.init(AXIS1_VRX, AXIS1_VRY);
    //m_dualAxis2.init(AXIS2_VRX, AXIS2_VRY);
    m_dualAxis0.calibrate();
    m_dualAxis1.calibrate();
    //m_dualAxis2.calibrate();
    
    /* Gamemap setup */

    // Uncomment one of the following two config types depending on which controller version you want to use
    // The XBox series X controller only works on linux kernels >= 6.5
    //XboxOneSControllerDeviceConfiguration* config = new XboxOneSControllerDeviceConfiguration();
    XboxSeriesXControllerDeviceConfiguration* config = new XboxSeriesXControllerDeviceConfiguration();

    BLEHostConfiguration hostConfig = config->getIdealHostConfiguration();
    m_gamepad = new XboxGamepadDevice(config);

    //m_gamepad->onVibrate.attach(vibrationSlot);
    m_bleHID.addDevice(m_gamepad);
    m_bleHID.begin(hostConfig);

    LOG_I(TAG_CONTROLLER, "BLE configuration: [VID: 0x%x] [PID: 0x%x]", hostConfig.getVid(), hostConfig.getPid());
}

static inline int16_t floatToInt16(float v) {
    v = fmaxf(-1.0f, fminf(1.0f, v));
    return (int16_t)(v * (v >= 0.0f ? 32767.0f : 32768.0f));
}
static inline int16_t floatToInt16_trigger(float v) {
    v = fmaxf(-1.0f, fminf(1.0f, v));
    float scaled = (v + 1.0f) * 0.5f * 1023.0f;
    return (uint16_t)(scaled + 0.5f);
}

#include <NimBLEDevice.h>
void Controller::loop() {
    if (!m_bleHID.isConnected()) return;

    float x0, y0, x1, y1;//, x2, y2;
    m_dualAxis0.pullEvents();
    m_dualAxis1.pullEvents();
    //m_dualAxis2.pullEvents();
    m_dualAxis0.getAxis(x0, y0);
    m_dualAxis1.getAxis(x1, y1);
    //m_dualAxis2.getAxis(x2, y2);
    m_gamepad->setLeftThumb(floatToInt16(x0), floatToInt16(y0));
    m_gamepad->setRightThumb(floatToInt16(x1), floatToInt16(y1));
    //m_gamepad->setLeftTrigger(floatToInt16_trigger(x2));
    //m_gamepad->setRightTrigger(floatToInt16_trigger(y2));
    
    bool pressed0 = (digitalRead(AXIS0_SW) == LOW);
    bool pressed1 = (digitalRead(AXIS1_SW) == LOW);
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

    m_gamepad->sendGamepadReport();

    delay(10);

    /*
    int btn = digitalRead(14);
    
    if (btn == LOW) {
        m_compositeHID.press(BUTTON_4);
    }
    else {
        m_compositeHID.release(BUTTON_4);
    }

    m_compositeHID.sendReport();
    delay(10);
    */
}


void Controller::disconnect() {
    NimBLEDevice::deleteAllBonds();
    NimBLEServer* server = NimBLEDevice::getServer();
    server->disconnect(0);
    NimBLEDevice::getAdvertising()->start();
}

float Controller::normalizeAxis(int raw, int center, int deadzone) {
    int v = raw - center;

    if (abs(v) < deadzone) return 0.0f;

    if (v > 0) {
        return (float)(v - deadzone) / (2048.0f - deadzone);
    } else {
        return (float)(v + deadzone) / (2048.0f - deadzone);
    }
}

int16_t Controller::toXBOX(float v) {
    if (v > 1.0f) v = 1.0f;
    if (v < -1.0f) v = -1.0f;
    
    return (int16_t)(v * 32767);
}


/*
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
*/
