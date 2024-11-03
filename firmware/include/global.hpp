#pragma once

#ifndef PINI_GLOBAL_H
#define PINI_GLOBAL_H

// Select on of the LOG leves from 'log.hpp', THIS MUST BE DEFINED BEFORE INCLUDE OF LOG.HPP
#define LOG_LEVEL LOG_LEVEL_TRACE
#define LOG_COLORED

#include <pinicore.h>

/** FIRMWARE_VERSION
 * 1 -> First version of PiniGamePad
 */
#define FIRMWARE_VERSION 1
#define WDTG_TIMER_IN_SECONDS 120

#endif // PINI_GLOBAL_H