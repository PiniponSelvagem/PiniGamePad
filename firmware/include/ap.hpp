#pragma once 
    
#ifndef PINI_AP_H
#define PINI_AP_H

#include <HTTPClient.h>

#define AP_TIMEOUT (5 * 60 * 1000)

#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>

#include "global.hpp"

inline WebServer server(80);

#define MOCK_VALUE_PASSWORD_FIELD   "PiniponSelvagem"   // THIS IS USED TO DISPLAY A MOCK PASSWORD ON THE CLIENT SIDE, NEVER SET A REAL PASSWORD ON THIS DEFINE!!!!!!

#define COLOR_ON    "#00AA00"
#define COLOR_OFF   "#AA0000"

class AP {   
    public:
        void setup();

        /**
         * AP handle state update.
         * @return If true, then ESP32 should restart as soon as possible.
         */
        bool loop();

        uint8_t stationsConnected();
        void startTimeout();

        bool isActive();


    private:
        void stop();

        long m_timeoutStartAt = 0;
        bool m_isRunning = false;
        int m_restart = 0;

        String start = "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n"
            "<head>\n"
            "    <meta charset=\"UTF-8\">\n"
            "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
            "    <title>PiniGamePad</title>\n";
        
        String script = "<script>\n"
            "    addEventListener('load', init, false);\n"
            "    function init() {\n"
            "        const $form = document.querySelector('#ap');\n"
            "        const $progress = document.querySelector('#progress');\n"
            "        const $resultOk = document.querySelector('#result-ok');\n"
            "        const $resultNotOk = document.querySelector('#result-not-ok');\n"
            "        const $sendWifiButton = document.querySelector('#send-wifi');\n"
            "        $form.addEventListener('submit', upload, false);\n"
            "        $sendWifiButton.addEventListener('click', sendWifiData, false);\n"
            "        async function upload(e) {\n"
            "            e.preventDefault();\n"
            "            $resultOk.style.display = 'none';\n"
            "            $resultNotOk.style.display = 'none';\n"
            "            const data = new FormData();\n"
            "            data.append('file', document.querySelector('#file').files[0]);\n"
            "            const uploadRequest = new XMLHttpRequest();\n"
            "            uploadRequest.open('POST', '/upload');\n"
            "            uploadRequest.upload.addEventListener('progress', e => {\n"
            "                if (!e.lengthComputable) {\n"
            "                    return false;\n"
            "                }\n"
            "                $progress.style.display = 'block';\n"
            "                let percentage = Math.round(e.loaded / e.total * 100);\n"
            "                document.querySelector('#progress > td').innerHTML = 'Uploading... ' + percentage + '%';\n"
            "            });\n"
            "            uploadRequest.addEventListener('load', e => {\n"
            "                $progress.style.display = 'none';\n"
            "                if (uploadRequest.status === 200) {\n"
            "                    $resultOk.style.display = 'block';\n"
            "                } else {\n"
            "                    $resultNotOk.style.display = 'block';\n"
            "                }\n"
            "            });\n"
            "            uploadRequest.send(data);\n"
            "        }\n"
            "        async function sendWifiData(e) {\n"
            "            e.preventDefault();\n"
            "            const wifiSSID = document.querySelector('#wifi-ssid').value;\n"
            "            const wifiPASS = document.querySelector('#wifi-password').value;\n"
            "            if ((wifiSSID.trim() !== \"\") && (wifiPASS === \"" MOCK_VALUE_PASSWORD_FIELD "\")) {\n"
            "                alert('WiFi password field not changed.\\nWiFi data not updated.');\n"
            "                return;\n"
            "            }\n"
            "            const data = new FormData();\n"
            "            data.append('wifi-ssid', wifiSSID);\n"
            "            data.append('wifi-password', wifiPASS);\n"
            "            const response = await fetch('/sendWifiData', {\n"
            "                method: 'POST',\n"
            "                body: data,\n"
            "            });\n"
            "            const result = await response.json();\n"
            "            if (result.isWIFI_and_notGSM) {\n"
            "                alert('Após reboot irá ligar-se á WiFi: '+wifiSSID+'\\n\\nA reiniciar...');\n"
            "            } else {\n"
            "                alert('Após reboot irá usar GSM.\\n\\nA reiniciar...');\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "</script>\n";

            String style = "<style>\n"
            "   * {\n"
            "       margin: 0;\n"
            "       padding: 0;\n"
            "    }\n"
            "   body, html {\n"
            "       padding: 0;\n"
            "       display: flex;\n"
            "       justify-content: center;\n"
            "       align-items: center;\n"
            "       background-color: #f0f0f0;\n"
            "       font-family: Arial, sans-serif;\n"
            "       overflow-x: hidden;\n"
            "   }\n"
            "   .center {\n"
            "       width: 100%;\n"
            "       padding: 10px;\n"
            "       background-color: white;\n"
            "       border: 1px solid #ccc;\n"
            "       border-radius: 8px;\n"
            "       box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);\n"
            "       box-sizing: border-box;\n"
            "   }\n"
            "   h4 {\n"
            "       margin: 0;\n"
            "       padding: 0;\n"
            "       text-align: center;\n"
            "   }\n"
            "   input[type=\"text\"], input[type=\"password\"] {\n"
            "       height: 100%;\n"
            "       width: 100%;\n"
            "       padding: 4px;\n"
            "       box-sizing: border-box;\n"
            "   }\n"
            "   td {\n"
            "       text-align: center;\n"
            "   }\n"
            "   table {\n"
            "       width: 100%;\n"
            "       table-layout: auto;\n"
            "       margin-top: 20px;\n"
            "   }\n"
            "</style>\n";

            String mid0 = "</head>\n"
            "<body>\n"
            "    <form id=\"ap\" enctype=\"multipart/form-data\">\n"
            "        <table class=\"center\">\n"
            "            <tr>\n"
            "                <td><h4 style=\"text-align:center\">" DEVICE_EMOJI " " DEVICE_TYPE " " DEVICE_EMOJI "</h4></td>\n"
            "            </tr>\n"
            "            <tr>\n"
            "                <td><h6 style=\"text-align:center\">version: " TOSTRING(FIRMWARE_VERSION) "</h6></td>\n"
            "            </tr>\n"
            "            <tr>\n"
            "                <td><h4 style=\"text-align:center\">";
            
            String mid1 = "</h4></td>\n"
            "            </tr>\n"
            "            <tr>\n"
            "                <td style=\"text-align:center\"><input type=\"text\" id=\"wifi-ssid\" placeholder=\"WiFi SSID\" value=\"";
            
            String mid2 = "\"></td>\n"
            "            </tr>\n"
            "            <tr>\n"
            "                <td style=\"text-align:center\"><input type=\"password\" id=\"wifi-password\" placeholder=\"WiFi Password\" value=\"";

            String end = "\"></td>\n"
            "            </tr>\n"
            "            <tr>\n"
            "                <td style=\"text-align:center\"><button type=\"button\" id=\"send-wifi\">Write WiFi data</button></td>\n"
            "            </tr>\n"
            "            <tr><td>&nbsp;</td><tr>\n"
            "            <tr>\n"
            "                <td><input type=\"file\" id=\"file\"></td>\n"
            "            </tr>\n"
            "            <tr>\n"
            "                <td style=\"text-align:left;\"><input type=\"submit\" value=\"Upload binary\"></td>\n"
            "            </tr>\n"
            "            <tr id=\"progress\" style=\"display: none;\">\n"
            "                <td></td>\n"
            "            </tr>\n"
            "            <tr id=\"result-ok\" style=\"display: none;\">\n"
            "                <td>🟢 File uploaded with success 🟢</td>\n"
            "            </tr>\n"
            "            <tr id=\"result-not-ok\" style=\"display: none;\">\n"
            "                <td>🟥 Error while uploading binary. 🟥<br>Try again.</td>\n"
            "            </tr>\n"
            "        </table>\n"
            "    </form>\n"
            "</body>\n"
            "</html>";
};

#endif // PINI_AP_H