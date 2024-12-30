#include "ap.hpp"

#include "global.hpp"
#include "utils/utils.hpp"
#include "data/board.hpp"
#include "data/memory.hpp"

#define TAG_AP	"ap"

#define AP_SSID_PREFIX  "PGP_"
#define AP_SSID_SIZE    ((sizeof(AP_SSID_PREFIX)-1) + BOARD_UNIQUEID_SIZE + 1)

void AP::setup() {
    INFO(TAG_AP, "Setup started");
    char apSSID[AP_SSID_SIZE];
    sprintf(apSSID, "%s%s", AP_SSID_PREFIX, board.getUniqueId());

    WiFi.softAP(apSSID, "");

    server.on("/", [this]() {
        String uniqueId = String(board.getUniqueId());
        String wifiSSID = memory.getWifiSSID();
        bool emptyWifiPASS = memory.getWifiPASS().isEmpty();

        String content = start;
        content += script;
        content += style;
        content += mid0;
        content += uniqueId;
        content += mid1;
        content += wifiSSID;
        content += mid2;
        content += (emptyWifiPASS) ? "" : MOCK_VALUE_PASSWORD_FIELD;
        content += end;

        server.send(200, "text/html", content);
    });

    server.on("/sendWifiData", HTTP_POST, [this]() {
        String ssid = server.arg("wifi-ssid");
        String password = server.arg("wifi-password");

        /*
        memory.saveWifiSSID(ssid);
        memory.saveWifiPASS(password);
        */

        DEBUG(TAG_AP, "Received WiFi configuration: [SSID: %s] [PASS: %s]", ssid.c_str(), password.c_str());

        //m_restart = 1;
    });

    server.on("/upload", HTTP_POST, [this]() {
        server.sendHeader("Connection", "close");
        if (Update.hasError()) {
            server.send(500, "text/plain", "error");
        } else {
            server.send(200, "text/plain", "ok");
            m_restart = 1;
        }
    }, [this]() {
        /*
        info.connected = getMillis() + 60000;

        HTTPUpload& upload = server.upload();

        if (upload.status == UPLOAD_FILE_START) {
            INFO(TAG_AP, "Received update file '%s'", upload.filename);
            
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
                Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
            /* flashing firmware to ESP*
            if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
                Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_END) {
            if (Update.end(true)) { //true to set the size to the current progress
                INFO(TAG_AP, "Update completed with success, total size %u", upload.totalSize);
            } else {
                ERROR(TAG_AP, "Update failed with error (see next line):");
                Update.printError(Serial);
            }
        }
        */
    });

    server.begin();
    INFO(TAG_AP, "HTTP server started on '%s'", WiFi.softAPIP().toString());

    m_isRunning = true;
    INFO(TAG_AP, "Setup completed");
}

bool AP::loop() {
    if (m_isRunning) {
        server.handleClient();
        if (m_restart == 1) {
            INFO(TAG_AP, "Reboot requested...");
            stop();
            return true;
        }

        if (m_timeoutStartAt!=0 && (millis() > (m_timeoutStartAt+AP_TIMEOUT))) {
            if (stationsConnected() == 0) {   // only turn off AP when user is not conected
                INFO(TAG_AP, "Timeout reached and no stations currently connected. Reboot requested...");
                stop();
                return true;
            }
            else {
                m_timeoutStartAt = millis();
            }
        }
    }
    return false;
}

uint8_t AP::stationsConnected() {
    return WiFi.softAPgetStationNum();
}
void AP::startTimeout() {
    DEBUG(TAG_AP, "Timeout counter started");
    m_timeoutStartAt = getMillis();
}
bool AP::isActive() {
    return m_isRunning;
}

void AP::stop() {
    m_isRunning = false;
    WiFi.enableAP(false);
}