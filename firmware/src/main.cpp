#include "global.hpp"
#include <esp_task_wdt.h>
#include "data/memory.hpp"
#include "communication/wifi.hpp"
#include "controller.hpp"
#include "ap.hpp"
#include "utils/utils.hpp"

//#define DEBUG_START_DELAY	// delay the startup a bit so all the setup prints show in console

#define TAG_MAIN	"main"
#define TAG_TASKAP	"taskAP"

TaskHandle_t TaskAP;

/* Do not use these directly other than setting 'network' pointer to them */
WiFiComm wifi;


Controller controller;
AP ap;


void taskAP(void *arg) {
	INFO(TAG_TASKAP, "Started...");
	ap.setup();

    unsigned long time = millis();
	while (true) {
    	vTaskDelay(pdMS_TO_TICKS(1));
		bool shouldRestart = ap.loop();
        if (shouldRestart) {
            //controller.requestRestart();
            WARN(TAG_TASKAP, "Request restart not implemented!");
        }

        if (!ap.isActive()) {
			INFO(TAG_TASKAP, "Terminated");
			vTaskDelete(NULL);
		}
	}
}


void createTask(const char* name, TaskHandle_t taskHandle, TaskFunction_t pTaskFunction, void* taskParams) {
	if (xTaskCreate(pTaskFunction, name, 4096, taskParams, configMAX_PRIORITIES, &taskHandle) == pdFALSE) {
		FATAL(TAG_MAIN, "Task '%s' not created. Rebooting...", name);
		delay(5000);
		ESP.restart();
	}
}

bool wasLastResetManual() {
    bool wasManual = false;
    esp_reset_reason_t resetReason = esp_reset_reason();
    switch (resetReason) {
        case ESP_RST_POWERON:
        case ESP_RST_EXT:
        case ESP_RST_WDT:   // The reset 'button' can trigger this reason
            wasManual = true;
            break;
        default:    // There are more cases but not relevant
            break;
    }

    DEBUG(TAG_MAIN, "Last reset was '%s' and caused by 'esp_reset_reason_t = %d'", wasManual ? "manual" : "auto", resetReason);
    return wasManual;
}
bool wasLastResetFatal() { return esp_reset_reason() == ESP_RST_PANIC; }

#include "data/board.hpp"
BleGamepad m_gamepad = BleGamepad(DEVICE_TYPE, DEVICE_MANUFACTURER);
BleGamepadConfiguration m_gamepadConfig;
void setup() {
	Serial.begin(115200);

#ifdef DEBUG_START_DELAY
	delay(5000);
#endif
	Serial.println();	// Just to start on a new clean line

	INFO(TAG_MAIN, "Setup started");
	INFO(TAG_MAIN ,"Firmware version %d", FIRMWARE_VERSION);
    if (wasLastResetFatal()) {
        FATAL(TAG_MAIN, "\e[1;33mI hit the 'panic' button! Maybe it's time for you to hit the 'learn to code' button?\e[0m");
    }  
    
	memory.setup();

	/* Network type selection */
	String wifiSSID = memory.getWifiSSID();
	wifi.setup();

	/* Create WiFi AP Task */
	createTask("WiFi AP", TaskAP, taskAP, NULL);

	controller.setup();
	ap.startTimeout();
	
	INFO(TAG_MAIN, "Internal watchdog configured to %d seconds", WDTG_TIMER_IN_SECONDS);
	esp_task_wdt_init(WDTG_TIMER_IN_SECONDS, true);
	esp_task_wdt_add(NULL);
	INFO(TAG_MAIN, "Setup completed");




    m_gamepadConfig.setAutoReport(false);
    m_gamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);

    // Input
    m_gamepadConfig.setButtonCount(8);
    m_gamepadConfig.setHatSwitchCount(4);
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

    m_gamepad.begin(&m_gamepadConfig);
}

void loopGamepad() {
    if (!m_gamepad.isConnected()) return;
    m_gamepad.sendReport();
    
    TRACE(TAG_MAIN, "sendReport");
    delay(1000);
}

void loop() {
	vTaskDelay(pdMS_TO_TICKS(1));	// ensure other tasks also get CPU time
	esp_task_wdt_reset();

	controller.loop();
    loopGamepad();
}
