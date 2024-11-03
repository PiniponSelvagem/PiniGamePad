#include "global.hpp"
#include <esp_task_wdt.h>
#include "data/memory.hpp"
#include "communication/wifi.hpp"
#include "controller.hpp"
#include "ap.hpp"
#include "utils/utils.hpp"

#define DEBUG_START_DELAY	// delay the startup a bit so all the setup prints show in console

#define TAG_MAIN	"main"
#define TAG_TASKAP	"taskAP"

TaskHandle_t TaskAP;

/* Do not use these directly other than setting 'network' pointer to them */
WiFiComm wifi;


Controller controller;
AP ap;


void taskAP(void *arg) {
	INFO(TAG_TASKAP, "Started");
	vTaskDelay(pdMS_TO_TICKS(5000));	// Not the best, but makes sure that the AP starts after trying to connect to a wifi
	DEBUG(TAG_TASKAP, "5s delay ended");
	ap.setup();

	while (true) {
    	vTaskDelay(pdMS_TO_TICKS(1));
		ap.loop();
	}
}


void createTask(const char* name, TaskHandle_t taskHandle, TaskFunction_t pTaskFunction, void* taskParams) {
	if (xTaskCreatePinnedToCore(pTaskFunction, name, 4096, taskParams, configMAX_PRIORITIES, &taskHandle, 0) == pdFALSE) {
		FATAL(TAG_MAIN, "Task '%s' not created. Rebooting...", name);
		delay(5000);
		ESP.restart();
	}
}


void setup() {
	Serial.begin(115200);

#ifdef DEBUG_START_DELAY
	delay(5000);
#endif
	Serial.println();	// Just to start on a new clean line

	INFO(TAG_MAIN, "Setup started");
	INFO(TAG_MAIN ,"Firmware version %d", FIRMWARE_VERSION);
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
}

void loop() {
	vTaskDelay(pdMS_TO_TICKS(1));	// ensure other tasks also get CPU time
	esp_task_wdt_reset();

	controller.loop();
}
