#pragma once

#ifndef PINI_EEPROM_H
#define PINI_EEPROM_H

#include "global.hpp"
#include <EEPROM.h>

#define TAG_MEMORY	"memory"


#define EEPROM_SIZE 0x1000
#define MAX_EEPROM_LEN 63   // Not 64 because the last one is the terminal character. Also careful when increasing this, since data size can overlap.

/* This 'CHECK' is used to check if the EEPROM is formated. */
#define EEPROM_ADDRESS_CHECK 0x0000
/**
 * @brief Acts like a EEPROM version, if changed it will format the eeprom on next boot.
 * @note If this data check is changed, 'migrateOldData' will also need to be updated with a data migration logic.
 * @warning Never use use a previously used data check as the newest, unless you are 100% sure that version is no longer being used.
 *          Previously used versions can be seen in 'migrateOldData' function.
 */
#define EEPROM_DATA_CHECK "PINI_DATA_0000"  // acts like a eeprom version, if changed, it will format the eeprom on next boot

/* Default DATA: WiFi SSID */
#define EEPROM_ADDRESS_WIFISSID 0x0020
#define EEPROM_DATA_WIFI_SSID ""

/* Default DATA: WiFi PASSWORD */
#define EEPROM_ADDRESS_WIFIPASS 0x0060
#define EEPROM_DATA_WIFI_PASSWORD ""


class Memory {
    public:
        /**
         * Checks if the eeprom is formated.
         * If data check fails, will format the eeprom with default data.
         */
        void setup() {
            INFO(TAG_MEMORY, "Setup started");
            EEPROM.begin(EEPROM_SIZE);
            String dataCheck = readStringEEPROM(EEPROM_ADDRESS_CHECK);
            if (migrateOldData(dataCheck)) {
                writeStringEEPROM(EEPROM_ADDRESS_CHECK, EEPROM_DATA_CHECK);
                INFO(TAG_MEMORY, "Old data migrated to version '%s'", dataCheck.c_str());
            }
            else {
                if (dataCheck != EEPROM_DATA_CHECK) {
                    INFO(TAG_MEMORY, "Data not found");
                    formatWithDefaultData();
                    INFO(TAG_MEMORY, "Format completed");
                }
                else {
                    INFO(TAG_MEMORY, "Found version '%s'", dataCheck.c_str());
                }
            }

            //printEEPROM(16, false);
            INFO(TAG_MEMORY, "Setup completed");
        }

        /* Network */
        void saveWifiSSID(String ssid) { writeStringEEPROM(EEPROM_ADDRESS_WIFISSID, ssid); }
        void saveWifiPASS(String pass) { writeStringEEPROM(EEPROM_ADDRESS_WIFIPASS, pass); }
        String getWifiSSID() { return readStringEEPROM(EEPROM_ADDRESS_WIFISSID); }
        String getWifiPASS() { return readStringEEPROM(EEPROM_ADDRESS_WIFIPASS); }


    private:
        /**
         * @brief Migrate old data to new
         */
        bool migrateOldData(String dataCheck) {
            bool migratedOldData = false;
            /*
            if (dataCheck == "PINI_DATA_0000") {
                String oldData_wifiSSID = readStringEEPROM(0x90);
                String oldData_wifiPASS = readStringEEPROM(0xF4);
                saveWifiSSID(oldData_wifiSSID);
                saveWifiPASS(oldData_wifiPASS);
                migratedOldData = true;
            }
            */
            return migratedOldData;
        }

        /**
         * @brief Format EEPROM with default data.
         */
        void formatWithDefaultData() {
            INFO(TAG_MEMORY, "Formating using default data to version '%s'", EEPROM_DATA_CHECK);
            // Write EEPROM version
            writeStringEEPROM(EEPROM_ADDRESS_CHECK, EEPROM_DATA_CHECK);

            // Default data
            saveWifiSSID(EEPROM_DATA_WIFI_SSID);
            saveWifiPASS(EEPROM_DATA_WIFI_PASSWORD);
        }

        /**
         * @brief Dumps the EEPROM data to Serial.
         * @param bytesPerLine Number of bytes per line. Only tested with values multiple of 8.
         * @param printChar When true, will print in ASCII.
         * @note Only use this for debugging EEPROM data.
         * @warning Raw data is printed inverted, this means:
         *          Example:
         *          - 0x76543210, 0 being the lowest byte and 3 the highest byte
         *          - will be printed: 0x10325476
        */
        void printEEPROM(uint8_t bytesPerLine, bool printChar) {
            DEBUG(TAG_MEMORY, "Showing raw memory data");
            for (int i = 0; i < EEPROM_SIZE; i++) {
                if (i%bytesPerLine == 0) {
                    if (i!=0)
                        Serial.println();
                    Serial.printf("\t0x%04x - ", i);
                }
                else if (i%8==0) {
                    Serial.print(" ");
                }
                if (printChar)
                    Serial.printf("%c", EEPROM.read(i));
                else
                    Serial.printf("%02x", EEPROM.read(i));
            }
            Serial.println();
        }

        void writeStringEEPROM(int address, String data) {
            int _size = data.length();
            if (_size > MAX_EEPROM_LEN) {
                WARN(TAG_MEMORY, "Unable to write on address 0x%x because data size [%d] exceeds acceptable size [%d]", address, _size, MAX_EEPROM_LEN);
                return;
            }
            int i;
            for (i = 0; i < _size; i++) {
                EEPROM.write(address + i, data[i]);
            }
            EEPROM.write(address + _size, '\0');  //Add termination null character for String Data
            EEPROM.commit();
        }
        String readStringEEPROM(int address) {
            int i;
            char data[MAX_EEPROM_LEN + 1];
            int len = 0;
            unsigned char k;
            k = EEPROM.read(address);
            while (len < MAX_EEPROM_LEN) {  //Read until null character   
                k = EEPROM.read(address + len);
                data[len] = k;
                len++;
            }
            data[len] = '\0';
            return String(data);
        }

        void writeBytesEEPROM(int address, uint8_t *data, size_t size) {
            if ((address+size) > EEPROM_SIZE) {
                WARN(TAG_MEMORY, "Unable to write on address 0x%x because data size exceeds address space by %d bytes", address, (address+size)-EEPROM_SIZE);
                return;
            }
            for (int i = 0; i < size; i++) {
                EEPROM.write(address + i, data[i]);
            }
            EEPROM.commit();
        }
        void readBytesEEPROM(int address, uint8_t *data, size_t size) {
            uint8_t k;
            for (int len=0; len<size; ++len) {
                k = EEPROM.read(address + len);
                data[len] = k;
            }
        }
};

inline Memory memory;

#endif // PINI_EEPROM_H