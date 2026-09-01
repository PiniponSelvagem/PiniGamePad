/**
* @file		ledrgb.hpp
* @brief	LED RGB driver API.
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

#ifndef PINICORE_IO_LEDRGB_H
#define PINICORE_IO_LEDRGB_H

#include <stdint.h>

//namespace pinicore {

class LedRGB {
    public:
        /**
         * @brief   Initialize LED RGB pins.
         * @param   pinR Digital output pin for Red channel.
         * @param   pinG Digital output pin for Green channel.
         * @param   pinB Digital output pin for Blue channel.
         * @note    This function must be called prior to any other functions.
         */
        void init(uint8_t pinR, uint8_t pinG, uint8_t pinB);

        /**
         * @brief   Set LED RGB color.
         * @param   red Red channel intensity in range [0, 255].
         * @param   green Green channel intensity in range [0, 255].
         * @param   blue Blue channel intensity in range [0, 255].
         */
        void set(uint8_t red, uint8_t green, uint8_t blue);

        /**
         * @brief   Get current LED RGB color.
         * @param   red Reference to store current Red channel intensity.
         * @param   green Reference to store current Green channel intensity.
         * @param   blue Reference to store current Blue channel intensity.
         */
        void get(uint8_t &red, uint8_t &green, uint8_t &blue);


    private:
        uint8_t m_pinR, m_pinG,  m_pinB;
        uint8_t m_red,  m_green, m_blue;
};

//} // pinicore

#endif // PINICORE_IO_LEDRGB_H