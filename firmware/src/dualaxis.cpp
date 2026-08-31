#include "dualaxis.hpp"
#include <pinicore.hpp>

using namespace pinicore;

//namespace pinicore {

void DualAxis::init(uint8_t pinX, uint8_t pinY) {
    m_pinX = pinX;
    m_pinY = pinY;
    
    analogReadResolution(12);
}

void DualAxis::calibrate(int samples) {
    long sumX = 0;
    long sumY = 0;

    for (int i = 0; i < samples; i++) {
        sumX += analogRead(m_pinX);
        sumY += analogRead(m_pinY);
        delay(2);
    }

    m_centerX = (float)(sumX / samples);
    m_centerY = (float)(sumY / samples);

    // initialize drift to center
    m_driftX = m_centerX;
    m_driftY = m_centerY;
}

void DualAxis::pullEvents() {
    float rawX = (float)(analogRead(m_pinX) - m_centerX);
    float rawY = (float)(analogRead(m_pinY) - m_centerY);

    // normalize to [-1, 1]
    rawX /= 2048.0f;
    rawY /= 2048.0f;

    // clamp just in case
    rawX = constrain(rawX, -1.0f, 1.0f);
    rawY = constrain(rawY, -1.0f, 1.0f);

    m_axisX = rawX;
    m_axisY = rawY;

    m_updated = true;
}

void DualAxis::getAxis(float &outX, float &outY) {
    if (m_updated) {
        process();
        m_updated = false;
    }

    outX = m_smoothX;
    outY = m_smoothY;
}

void DualAxis::setDeadzone(float dz) {
    m_deadzone = constrain(dz, 0.0f, 1.0f);
}

void DualAxis::setSmoothing(float alphaSlow, float alphaFast, float threshold) {
    m_alphaSlow = alphaSlow;
    m_alphaFast = alphaFast;
    m_threshold = threshold;
}


float DualAxis::smooth(float input, float prev) {
    float delta = fabsf(input - prev);
    float alpha = (delta > m_threshold) ? m_alphaFast : m_alphaSlow;
    return prev + alpha * (input - prev);
}

void DualAxis::process() {
    float x = m_axisX;
    float y = m_axisY;

    float mag = sqrtf(x * x + y * y);

    // radial deadzone
    if (mag <= m_deadzone) {
        m_smoothX = 0.0f;
        m_smoothY = 0.0f;
        return;
    }

    // normalize direction
    float nx = x / mag;
    float ny = y / mag;

    // rescale magnitude
    float scaled = (mag - m_deadzone) / (1.0f - m_deadzone);
    scaled = constrain(scaled, 0.0f, 1.0f);

    float tx = nx * scaled;
    float ty = ny * scaled;

    // adaptive smoothing
    m_smoothX = smooth(tx, m_smoothX);
    m_smoothY = smooth(ty, m_smoothY);

    // micro clamp
    if (fabsf(m_smoothX) < 0.002f) m_smoothX = 0.0f;
    if (fabsf(m_smoothY) < 0.002f) m_smoothY = 0.0f;
}

//} // pinicore