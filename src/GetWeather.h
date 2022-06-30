#pragma once

#include "Watchy.h"

namespace Watchy_GetWeather {

constexpr const char* OPENWEATHERMAP_APIKEY =
    "f6554fe2f66ee9d7b4f64a479728695f";// "f058fe1cad2afe8e2ddc5d063a64cecb";  // use your own API key :)
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
