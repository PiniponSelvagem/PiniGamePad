/**
* @file		dualaxis.hpp
* @brief	Dual axis joystick driver API, KY-023.
* @author	PiniponSelvagem
*
* Copyright(C) PiniponSelvagem
*
***********************************************************************
* Software that is described here, is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
**********************************************************************/

#pragma once

#ifndef PINICORE_IO_DUALAXIS_H
#define PINICORE_IO_DUALAXIS_H

#include <stdint.h>

//namespace pinicore {

class DualAxis {
    public:
        /**
         * @brief   Initialize joystick pins.
         * @param   pinX Analog input pin for X axis.
         * @param   pinY Analog input pin for Y axis.
         * @note    This function must be called prior to any other functions.
         */
        void init(uint8_t pinX, uint8_t pinY);

        /**
         * @brief   Calibrate joystick center position.
         * @param   samples Number of samples used for averaging center position.
         * @note    Keep joystick in neutral position during calibration.
         */
        void calibrate(int samples = 200);

        /**
         * @brief   Read and update axis state.
         * @note    Should be called regularly in the main loop.
         */
        void pullEvents();

        /**
         * @brief   Get processed joystick axis values.
         * @param   outX Output X axis value in range [-1.0f, 1.0f].
         * @param   outY Output Y axis value in range [-1.0f, 1.0f].
         */
        void getAxis(float &outX, float &outY);

        /**
         * @brief   Set radial deadzone.
         * @param   dz Deadzone value in range [0.0f, 1.0f].
         */
        void setDeadzone(float dz);

        /**
         * @brief   Configure adaptive smoothing behavior.
         * @param   alphaSlow Smoothing factor when joystick is idle (0.0f–1.0f).
         * @param   alphaFast Smoothing factor when joystick is moving (0.0f–1.0f).
         * @param   threshold Movement threshold to switch between slow/fast smoothing.
         * @note    Higher alpha = more responsive, lower alpha = smoother output.
         */
        void setSmoothing(float alphaSlow, float alphaFast, float threshold);


    private:
        /**
         * @brief   Adaptive smoothing helper function.
         * @param   input Current input value.
         * @param   prev Previous filtered value.
         * @return  Smoothed output value.
         */
        float smooth(float input, float prev);
        
        /**
         * @brief   Apply deadzone, normalization, and smoothing pipeline.
         */
        void process();
        

        uint8_t m_pinX, m_pinY;
        
        float m_centerX = 0.0f;
        float m_centerY = 0.0f;

        float m_axisX = 0.f;
        float m_axisY = 0.f;

        float m_driftX = 0.0f;
        float m_driftY = 0.0f;

        float m_deadzone = 0.05f;
        
        float m_smoothX = 0.0f;
        float m_smoothY = 0.0f;

        float m_alphaSlow = 0.04f;
        float m_alphaFast = 0.50f;
        float m_threshold = 0.05f;

        bool m_updated = false;
};

//} // pinicore

#endif // PINICORE_IO_DUALAXIS_H