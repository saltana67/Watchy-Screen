#pragma once

#include "Watchy.h"

namespace Watchy_GetWeather {

#ifndef DEFAULT_OPENWEATHERMAP_APIKEY 
  #define DEFAULT_OPENWEATHERMAP_APIKEY "12345678901234567890123456789012" // use your own API key !!!
#endif
constexpr const char* OPENWEATHERMAP_APIKEY = DEFAULT_OPENWEATHERMAP_APIKEY; 

constexpr const char* OPENWEATHERMAP_URL =
    "http://api.openweathermap.org/data/2.5/weather";
constexpr const char* TEMP_UNIT = "metric";  // use "imperial" for Fahrenheit"
const int WEATHER_UPDATE_INTERVAL = 15 * 60;  // 15 minutes in seconds

typedef struct weatherData {
  int8_t temperature;
  int16_t weatherConditionCode;
  char weatherCity[25];
} weatherData;

extern weatherData getWeather(boolean forceNow = false);
extern time_t lastGetWeatherTS; // timestamp of last successful getWeather call

}  // namespace Watchy_GetWeather
