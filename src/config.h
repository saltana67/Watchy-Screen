#pragma once

#ifndef TAG
#define TAG "Watchy"
#endif

#include "secrets.h"

//#ifdef ARDUINO_WATCHY
//take out all pins already defined in watchy arduino framework variant
//~.platformio/packages/framework-arduinoespressif32/variants/watchy/pins_arduino.h
//legacy defines, absent in newer versions
/*
#ifndef GPIO_SEL_4
# define GPIO_SEL_4 (1ULL << GPIO_NUM_4)
#endif
#ifndef GPIO_SEL_14
# define GPIO_SEL_14 (1ULL << GPIO_NUM_14)
#endif
#ifndef GPIO_SEL_25
# define GPIO_SEL_25 (1ULL << GPIO_NUM_25)
#endif
#ifndef GPIO_SEL_26
# define GPIO_SEL_26 (1ULL << GPIO_NUM_26)
#endif
#ifndef GPIO_SEL_35
# define GPIO_SEL_35 (1ULL << GPIO_NUM_35)
#endif
*/


// hardware
//#define WATCHY_HW_VERSION 20

//pins
//#define SDA 21 //static const uint8_t SDA = 21; in ~/.platformio/packages/framework-arduinoespressif32/variants/watchy/pins_arduino.h:9
//#define SCL 22 //static const uint8_t SCL = 22; in ~/.platformio/packages/framework-arduinoespressif32/variants/watchy/pins_arduino.h:10

/*
//static const uint8_t BATT_ADC_PIN = 34; in ~/.platformio/packages/framework-arduinoespressif32/variants/watchy/pins_arduino.h:40
#if WATCHY_HW_VERSION == 20
#   define ADC_PIN 34
#elif WATCHY_HW_VERSION == 15
#   define ADC_PIN 35
#else
#   define ADC_PIN 33
#endif
ADC_PIN semms never to be used so ... ignore
#define ADC_PIN BATT_ADC_PIN
*/

/*
static const uint8_t RTC_INT_PIN = 27; //pins_arduino.h:27
//RTC_PIN exchanged by RTC_INT_PIN to match names, no need for mapping
//#define RTC_PIN RTC_INT_PIN
*/
/* 
// unused?
#define CS 5
#define DC 10
#define RESET 9
#define BUSY 19
*/

/*
// defined in  pins_arduino.h:20:23
//static const uint8_t DISPLAY_CS = 5;
//static const uint8_t DISPLAY_RES = 9;
//static const uint8_t DISPLAY_DC = 10;
//static const uint8_t DISPLAY_BUSY = 19;
#define DISPLAY_CS 5
#define DISPLAY_RES 9
#define DISPLAY_DC 10
#define DISPLAY_BUSY 19
*/

//static const uint8_t VIB_MOTOR_PIN = 13; pins_arduino.h:26
//#define VIB_MOTOR_PIN 13

/*
//pins_arduino.h:17:19
//static const uint8_t MENU_BTN_PIN = 26;
//static const uint8_t BACK_BTN_PIN = 25;
//static const uint8_t DOWN_BTN_PIN = 4;
#define MENU_BTN_PIN 26
#define BACK_BTN_PIN 25
#define DOWN_BTN_PIN 4
*/

/* pins_arduino.h:29...
//#if defined(ARDUINO_WATCHY_V10)
//static const uint8_t UP_BTN_PIN = 32;
//static const uint8_t BATT_ADC_PIN = 33;
//#define BAT_VOLT_PIN BATT_ADC_PIN
//#define RTC_TYPE     1  //DS3231
//#elif defined(ARDUINO_WATCHY_V15)
//static const uint8_t UP_BTN_PIN = 32;
//static const uint8_t BATT_ADC_PIN = 35;
//#define RTC_TYPE 2  //PCF8563
//#elif defined(ARDUINO_WATCHY_V20)
//static const uint8_t UP_BTN_PIN = 35;
//static const uint8_t BATT_ADC_PIN = 34;
//#define RTC_TYPE 2  //PCF8563
//#endif

#if WATCHY_HW_VERSION == 20
#   define UP_BTN_PIN 35
#   define UP_BTN_MASK GPIO_SEL_35
#elif WATCHY_HW_VERSION == 15
#   define UP_BTN_PIN 32
#   define UP_BTN_MASK GPIO_SEL_32
#else
#   define UP_BTN_PIN 32
#   define UP_BTN_MASK GPIO_SEL_32
#endif
*/

/* pins_arduino.h:44....
//#define UP_BTN_MASK   (BIT64(UP_BTN_PIN))
//#define MENU_BTN_MASK (BIT64(MENU_BTN_PIN))
//#define BACK_BTN_MASK (BIT64(BACK_BTN_PIN))
//#define DOWN_BTN_MASK (BIT64(DOWN_BTN_PIN))
//#define ACC_INT_MASK  (BIT64(ACC_INT_1_PIN))
//#define BTN_PIN_MASK  (MENU_BTN_MASK | BACK_BTN_MASK | UP_BTN_MASK | DOWN_BTN_MASK)

#define MENU_BTN_MASK GPIO_SEL_26
#define BACK_BTN_MASK GPIO_SEL_25
#define DOWN_BTN_MASK GPIO_SEL_4
#define ACC_INT_MASK GPIO_SEL_14
#define BTN_PIN_MASK (MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK)

*/



//-----------------------------------------------------------------------------------------
#ifndef ARDUINO_WATCHY

//legacy defines, absent in newer versions
#ifndef GPIO_SEL_4
# define GPIO_SEL_4 (1ULL << GPIO_NUM_4)
#endif
#ifndef GPIO_SEL_14
# define GPIO_SEL_14 (1ULL << GPIO_NUM_14)
#endif
#ifndef GPIO_SEL_25
# define GPIO_SEL_25 (1ULL << GPIO_NUM_25)
#endif
#ifndef GPIO_SEL_26
# define GPIO_SEL_26 (1ULL << GPIO_NUM_26)
#endif
#ifndef GPIO_SEL_35
# define GPIO_SEL_35 (1ULL << GPIO_NUM_35)
#endif



// hardware
#define WATCHY_HW_VERSION 20

//pins
//#define SDA 21 //static const uint8_t SDA = 21; in .platformio/packages/framework-arduinoespressif32/variants/pico32/pins_arduino.h:17
//#define SCL 22 //static const uint8_t SCL = 22; .platformio/packages/framework-arduinoespressif32/variants/pico32/pins_arduino.h:18:22

#if WATCHY_HW_VERSION == 20
#   define ADC_PIN 34
#elif WATCHY_HW_VERSION == 15
#   define ADC_PIN 35
#else
#   define ADC_PIN 33
#endif

#define RTC_INT_PIN GPIO_NUM_27
#define CS 5
#define DC 10
#define RESET 9
#define BUSY 19
#define DISPLAY_CS 5
#define DISPLAY_RES 9
#define DISPLAY_DC 10
#define DISPLAY_BUSY 19
#define VIB_MOTOR_PIN 13
#define MENU_BTN_PIN 26
#define BACK_BTN_PIN 25
#if WATCHY_HW_VERSION == 20
#define UP_BTN_PIN 35
#define UP_BTN_MASK GPIO_SEL_35
#elif WATCHY_HW_VERSION == 15
#define UP_BTN_PIN 32
#define UP_BTN_MASK GPIO_SEL_32
#else
#define UP_BTN_PIN 32
#define UP_BTN_MASK GPIO_SEL_32
#endif
#define DOWN_BTN_PIN 4
#define MENU_BTN_MASK GPIO_SEL_26
#define BACK_BTN_MASK GPIO_SEL_25
#define DOWN_BTN_MASK GPIO_SEL_4
#define ACC_INT_MASK GPIO_SEL_14
#define BTN_PIN_MASK (MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK)

#endif //#ifndef ARDUINO_WATCHY
//-----------------------------------------------------------------------------------------

// battery
// ADC Channel and Attenuation
#define BATT_ADC_UNIT      ADC_UNIT_1
#define BATT_ADC_CHANNEL   ADC_CHANNEL_6  // GPIO34 == BATT_ADC_PIN
#define BATT_ATTEN         ADC_ATTEN_DB_12

//display
const int DISPLAY_WIDTH = 200;
const int DISPLAY_HEIGHT = 200;

//wifi
constexpr const char * WIFI_AP_SSID = "Watchy AP";
//set time
const int YEAR_OFFSET = 1970;
constexpr const char *NTP_SERVER = "pool.ntp.org";
constexpr const char *DEFAULT_TIMEZONE = "UTC0"; // posix format

//BLE OTA
constexpr const char * BLE_DEVICE_NAME = "Watchy BLE OTA";
constexpr const char * WATCHFACE_NAME = "Watchy 7 Segment";
const int SOFTWARE_VERSION_MAJOR = 1;
const int SOFTWARE_VERSION_MINOR = 0;
const int SOFTWARE_VERSION_PATCH = 0;
const int HARDWARE_VERSION_MAJOR = 1;
const int HARDWARE_VERSION_MINOR = 0;

//#include "wifi_config.h"
