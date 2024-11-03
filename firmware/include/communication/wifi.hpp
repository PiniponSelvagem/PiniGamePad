#pragma once

#ifndef PINI_WIFI_H
#define PINI_WIFI_H

#include "global.hpp"
#include "data/memory.hpp"
#include "utils/utils.hpp"
#include <WiFi.h>

#define TAG_WIFI	"wifi"

#define WIFI_RECONNECT_DELAY_MS		(10 * 1000)

WiFiClient wifiClient;

class WiFiComm {
	public:
		void setup() {
			String ssid = memory.getWifiSSID();
			String password = memory.getWifiPASS();

			WiFi.disconnect(true);
			WiFi.mode(WIFI_MODE_APSTA);
			WiFi.begin(ssid, password);
			DEBUG(TAG_WIFI, "Setup: [SSID: %s] [PASS: %s]", ssid, password);
		};

		void loop() {
			if(WiFi.status() != WL_CONNECTED) {
				if (getMillis() > nextReconnect) {
					WARN(TAG_WIFI, "Reconnecting...");
					reconnect();
					nextReconnect = getMillis() + WIFI_RECONNECT_DELAY_MS;
				}
			}
		};

		const char* getType() { return "WiFi"; }

		Client* getClient() { return m_client; }

		bool isConnected() { return WiFi.status() == WL_CONNECTED; };

		void reconnect() {
			WiFi.reconnect();
		}



	private:
		WiFiClient *m_client = &wifiClient;

		long nextReconnect = 0;
};

#endif // PINI_WIFI_H