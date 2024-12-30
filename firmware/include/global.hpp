#pragma once

#ifndef PINI_GLOBAL_H
#define PINI_GLOBAL_H

// Select one of the LOG levels from 'log.hpp', THIS MUST BE DEFINED BEFORE INCLUDE OF LOG.HPP
#define PLOG_LEVEL PLOG_LEVEL_TRACE
#define PLOG_COLORED

#include <pinicore.h>

/** FIRMWARE_VERSION
 * 1 -> First version of PiniGamepad
 */
#define FIRMWARE_VERSION 1

/** HARDWARE_VERSION compatible with FIRWARE_VERSION
 *  // Hver: [Fver start, Fver end] -> Information
 * 1:[1,X] -> First version of PiniGamepad
 */
#define HARDWARE_VERSION 1

#define WDTG_TIMER_IN_SECONDS 120

#define DEVICE_MANUFACTURER "PiniponSelvagem"
#define DEVICE_TYPE         "PiniGamepad"
#define DEVICE_EMOJI        "🎮"

#define DEVICE_VID                  0xF666      // Vendor ID
#define DEVICE_PID                  0x0001      // Product ID
#define DEVICE_MODEL_NUMBER         "PGP-0001"
#define DEVICE_FIRMWARE_REVISION    TOSTRING(FIRMWARE_VERSION)
#define DEVICE_SOFTWARE_REVISION    DEVICE_FIRMWARE_REVISION
#define DEVICE_HARDWARE_REVISION    TOSTRING(HARDWARE_VERSION)

#endif // PINI_GLOBAL_H