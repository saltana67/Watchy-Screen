#pragma once

#define BATTERY_MIN_VOLTAGE ((float)3.200)   //empty if under this value
#define BATTERY_MAX_VOLTAGE ((float)4.200)   //full if over this value

namespace Watchy {
extern float getBatteryVoltage();
extern float getBatteryState(float batteryVoltage);
extern float mapBatteryVoltage(const float v,
               const float outMax, const float outMin);
}