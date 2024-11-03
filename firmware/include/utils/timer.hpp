#pragma once

#ifndef PINI_TIMER_H
#define PINI_TIMER_H

#include "utils.hpp"

class Timer {
    public:
        /**
         * @brief Start the timer if not running.
         */
        void start() {
            if (!m_running) {
                m_startTime = getMillis();
                m_running = true;
            }
        }

        /**
         * @brief Stop the timer if running.
         */
        void stop() {
            if (m_running) {
                m_stopTime = getMillis();
                m_running = false;
            }
        }

        /**
         * @brief Reset the timer and start it.
         */
        void reset() {
            m_startTime = getMillis();
            m_stopTime = getMillis();
            m_running = true;
        }

        /**
         * @brief Check if timer is active.
         */
        bool isRunning() const {
            return m_running;
        }

        /**
         * @brief Get the elapsed time so far.
         *        If timer is stopped, it will return the elapsed time from when started to when it was stopped.
         */
        unsigned long getElapsedTime() const {
            if (m_running) {
                return getMillis() - m_startTime;
            } else {
                return m_stopTime - m_startTime;  // return the time when the timer was stopped
            }
        }

        /**
         * @brief Set an elapsed time.
         */
        void setElapsedTime(unsigned long elapsedTime) {
            m_startTime = getMillis() - elapsedTime;
            if (!m_running) {
                m_stopTime = m_startTime + elapsedTime;  // update stop time to match the new elapsed time
            }
        }


    private:
        unsigned long m_startTime = 0;
        unsigned long m_stopTime = 0;
        bool m_running = false;
};


#endif // PINI_TIMER_H