#pragma once

#ifndef PINI_GLOBAL_H
#define PINI_GLOBAL_H

#define FIRMWARE_VERSION 1
#define STORAGE_ID          "PINI_GAMEPAD"
#define DEVICE_MANUFACTURER "PiniponSelvagem"
#define DEVICE_TYPE         "PiniGamepad"
#define WDTG_INTERNAL_TIMER_IN_SECONDS 30

/* XBOX Gamepad configuration */
// This configuration should later go to runtime configuration changeable by the user
/**
 * Define this if using an Xbox Series X controller, comment for Xbox One S controller.
 * The XBox series X controller only works on linux kernels >= 6.5.
 */
#define XBOX_SERIES_X

#define AP_SSID      DEVICE_TYPE
#define AP_PASSWORD  ""

/* ESP32 PINs configuration */
#define PIN_SDA         21 // I2C SDA
#define PIN_SCL         22 // I2C SCL
#define PIN_LED_R       14 // LED Red
#define PIN_LED_G       15 // LED Green
#define PIN_LED_B       13 // LED Blue
#define PIN_BAT_ALERT   16 // Battery Alert Interrupt
#define PIN_PERIPHERALS 27 // Hardware Power Control
#define PIN_AXIS_LT     32 // Input Axis: Left Trigger
#define PIN_AXIS_LS_A0  36 // Input Axis: Left Stick (A0)
#define PIN_AXIS_LS_A1  39 // Input Axis: Left Stick (A1)
#define PIN_AXIS_RT     33 // Input Axis: Right Trigger
#define PIN_AXIS_RS_A0  34 // Input Axis: Right Stick (A0)
#define PIN_AXIS_RS_A1  35 // Input Axis: Right Stick (A1)
/****************************/
/* Left Multiplexer */
#define PIN_M0_LB        3 // Input Button: Left Bumper
#define PIN_M0_UP        5 // Input Button: Up
#define PIN_M0_DOWN      4 // Input Button: Down
#define PIN_M0_LEFT      6 // Input Button: Left
#define PIN_M0_RIGHT     2 // Input Button: Right
#define PIN_M0_LSB       0 // Input Button: Left Stick Button
#define PIN_M0_SELECT    1 // Input Button: Select
#define PIN_M0_MACRO     7 // Input Button: Macro
/********************/
/* Right Multiplexer */
#define PIN_M1_RB       11 // Input Button: Right Bumper
#define PIN_M1_X        14 // Input Button: X
#define PIN_M1_Y         9 // Input Button: Y
#define PIN_M1_A        10 // Input Button: A
#define PIN_M1_B         8 // Input Button: B
#define PIN_M1_RSB      15 // Input Button: Right Stick Button
#define PIN_M1_START    13 // Input Button: Start
#define PIN_M1_HOME     12 // Input Button: Home
/*********************/

#endif // PINI_GLOBAL_H