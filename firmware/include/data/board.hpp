#pragma once

#ifndef PINI_BOARD_H
#define PINI_BOARD_H

#include "global.hpp"

#define TAG_BOARD  "board"

#define BOARD_UNIQUEID_SIZE 12

class Board {        
    public:
        const char* getUniqueId() {
            if (!m_hasUniqueId) {
                uint8_t baseMac[6];
                if (esp_read_mac(baseMac, ESP_MAC_WIFI_STA) == ESP_OK) // Get MAC address for WiFi station
                    m_hasUniqueId = true;
                sprintf(m_uniqueId, "%02X%02X%02X%02X%02X%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
            }
            return m_uniqueId;
        };

        private:
            bool m_hasUniqueId = false;
            char m_uniqueId[BOARD_UNIQUEID_SIZE + 1] = {0};      // 12 -> mac address size; 1 -> termination character
};

inline Board board;

#endif // PINI_BOARD_H