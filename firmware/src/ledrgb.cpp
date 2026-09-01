#include "ledrgb.hpp"
#include <pinicore.hpp>

using namespace pinicore;

//namespace pinicore {

void LedRGB::init(uint8_t pinR, uint8_t pinG, uint8_t pinB) {
    m_pinR = pinR;
    m_pinG = pinG;
    m_pinB = pinB;

    pinMode(m_pinR, OUTPUT);
    pinMode(m_pinG, OUTPUT);
    pinMode(m_pinB, OUTPUT);

    set(0,0,0);
}

void LedRGB::set(uint8_t red, uint8_t green, uint8_t blue) {
    m_red   = red;
    m_green = green;
    m_blue  = blue;

    analogWrite(m_pinR, m_red);
    analogWrite(m_pinG, m_green);
    analogWrite(m_pinB, m_blue);
}

void LedRGB::get(uint8_t &red, uint8_t &green, uint8_t &blue) {
    red   = m_red;
    green = m_green;
    blue  = m_blue;
}

//} // pinicore
