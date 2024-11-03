#pragma once

#ifndef PINI_UTILS_H
#define PINI_UTILS_H

#include <esp_timer.h>

/**
 * Returns the milliseconds since BOOT.
 * Recommended to use this instead of "millis()" since that loops after +/-49 days.
 * "esp_timer_get_time()" loops after +/- 200 years.
 */
inline unsigned long getMillis() {
    return (esp_timer_get_time() / 1000);
}

#endif // PINI_UTILS_H