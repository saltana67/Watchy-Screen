//#include "BlufiScreen.h"
#include "RestartScreen.h"
#include "BuzzScreen.h"
#include "CarouselScreen.h"
#include "Events.h"
#include "ResetWifiScreen.h"
#include "GetLocation.h"
#include "GetWeatherScreen.h"
#include "IconScreen.h"
#include "ImageScreen.h"
#include "MenuScreen.h"
#include "OptimaLTStd22pt7b.h"
#include "OTAScreen.h"
#include "SetLocationScreen.h"
#include "SetTimeScreen.h"
#include "ShowBatteryScreen.h"

#include "ShowBluetoothScreen.h"
#include "ShowOrientationScreen.h"
#include "ShowStepsScreen.h"
#include "ShowWifiScreen.h"

#include "SyncTime.h"
#include "SyncTimeScreen.h"
#include "TimeScreen.h"
//#include "UpdateFWScreen.h"
#include "Watchy.h"
#include "WatchyErrors.h"
#include "WeatherScreen.h"
#include "WeatherForecastScreen.h"
#include "MoonScreen.h"
#include "WrappedTextScreen.h"
#include "icons.h"

#include <time.h>
#include "GetWeather.h"

#include "TestScreen.h"
#include "SevenSeg/SevenSegScreen.h"
#include "Chronometer/ChronometerScreen.h"
#include "Tetris/TetrisScreen.h"
#include "BadForEye/Watchy_BadForEye.h"
#include "TypoStyle/TypoStyle.h"
#include "AnalogGabel/AnalogGabel_V2.h"
#include "Maze4Watchy/Maze4Watchy.h"
#include "QRCodeScreen.h"
#include "TimezoneScreen.h"
#include "WebServer.h"
#include "WebServerScreen.h"
#include "AirplaneModeScreen.h"


TestScreen testScreen;

ResetWifiScreen resetWifiScreen;
SetTimeScreen setTimeScreen;
//BlufiScreen blufiScreen;
//UpdateFWScreen updateFWScreen;
SyncTimeScreen syncTimeScreen;
SetLocationScreen setLocationScreen;
GetWeatherScreen getWeatherScreen;
BuzzScreen buzzScreen;
OTAScreen otaScreen;
RestartScreen restartScreen;
WebServerScreen webServerScreen;
AirplaneModeScreen airplaneModeScreen;
MenuItem menuItems[] = {
                        {"Airplane mode", &airplaneModeScreen},
                        {"Reset Wifi", &resetWifiScreen},
                        {"Web server", &webServerScreen},
                        {"Set Time", &setTimeScreen},
                      //  {"Blufi", &blufiScreen},
                        {"Update (OTA)", &otaScreen},
//                        {"Update (BLE)", &updateFWScreen},
                        {"Sync Time", &syncTimeScreen},
                        {"Set Location", &setLocationScreen},
                        {"Get Weather", &getWeatherScreen},
                        {"Buzz", &buzzScreen},
                        {"Restart", &restartScreen}
                      };

MenuScreen menu(menuItems, sizeof(menuItems) / sizeof(menuItems[0]));


TimeScreen timeScreen;
SevenSegScreen sevenSegScreen;
ChronometerScreen chronometerScreen;
TetrisScreen tetrisScreen;
BadForEye badForEyeScreen;
TypoStyle typoStyleScreen;
AnalogGabel_V2 analogGabel_V2Screen;
Maze4Watchy maze4WatchyScreen;
QRCodeScreen qrCodeScreen;
TimezoneScreen timezoneScreen;

WeatherScreen weatherScreen;
WeatherForecastScreen weatherForecastScreen;
MoonScreen moonScreen;
IconScreen battery(&rle_battery, "battery", OptimaLTStd22pt7b);
IconScreen steps(&rle_steps, "steps", OptimaLTStd22pt7b);
IconScreen orientation(&rle_orientation, "orientation", OptimaLTStd22pt7b);
IconScreen bluetooth(&rle_bluetooth, "bluetooth", OptimaLTStd22pt7b);
IconScreen wifi(&rle_wifi, "wifi", OptimaLTStd22pt7b);
IconScreen settings(&rle_settings, "settings", OptimaLTStd22pt7b);
IconScreen text(&rle_text, "wrap text", OptimaLTStd22pt7b);
ImageScreen weather(cloud, 96, 96, "weather", OptimaLTStd22pt7b);
ImageScreen weatherForecast(cloud, 96, 96, "forecast", OptimaLTStd22pt7b);
ImageScreen moon(cloud, 96, 96, "moon", OptimaLTStd22pt7b);

ShowBatteryScreen showBattery;
ShowBluetoothScreen showBluetooth;
ShowOrientationScreen showOrientation;
ShowStepsScreen showSteps;
ShowWifiScreen showWifi;

WrappedTextScreen wrappedTextScreen(
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod "
    "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim "
    "veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea "
    "commodo consequat. Duis aute irure dolor in reprehenderit in voluptate "
    "velit esse cillum dolore eu fugiat nulla pariatur.",
    "n0-=-5p4c35,.,withaverylongwordthatshouldbehardwrapped?\n\nnotice how "
    "it goes away on screen refresh?        \t    \t         \r                "
    "      \n     that's deep sleep.");

CarouselItem carouselItems[] = {
                                {&sevenSegScreen, nullptr},
                                {&chronometerScreen, nullptr},
                                {&analogGabel_V2Screen, nullptr},
                                {&tetrisScreen, nullptr},
                                {&badForEyeScreen, nullptr},
                                {&maze4WatchyScreen, nullptr},
                                {&typoStyleScreen, nullptr},
                                {&timeScreen, nullptr},
                                {&weather, &weatherScreen},
                                {&weatherForecast, &weatherForecastScreen},
                                {&moon, &moonScreen},
                                {&battery, &showBattery},
                                {&steps, &showSteps},
                                {&orientation, &showOrientation},
                                {&text, &wrappedTextScreen},
                                {&bluetooth, &showBluetooth},
                                {&wifi, &showWifi},
                                {&qrCodeScreen, nullptr},
                                {&timezoneScreen, nullptr},
                                {&settings, &menu}
                                };

CarouselScreen carousel(carouselItems,
                        sizeof(carouselItems) / sizeof(carouselItems[0]));


Watchy_Event::BackgroundTask timeSync("timeSync", []() {
  Watchy_SyncTime::syncTime(Watchy_GetLocation::currentLocation.timezone);
},ESP_PRO_CORE);


Watchy_Event::BackgroundTask getLocation("getLocation", 
  Watchy_GetLocation::getLocation
  ,ESP_PRO_CORE
);

Watchy_Event::BackgroundTask getWeather("getWeather", []() {
  Watchy_GetWeather::getWeather();
},ESP_PRO_CORE);

Watchy_Event::BackgroundTask webServer("webServer", []() {
  Watchy_WebServer::startServer();
},ESP_PRO_CORE);

void setup() {
  Serial.begin(115200);
#if 0
  esp_log_level_set("*", static_cast<esp_log_level_t>(CORE_DEBUG_LEVEL));
#endif
  log_d("micros %ld", micros());  // fail if debugging macros not defined

  // initializing time and location can be a little tricky, because the
  // calls can fail for a number of reasons, but you don't want to just
  // keep trying because you can't know if the error is transient or
  // persistent. So whenever we wake up, try to sync the time and location
  // if they haven't ever been synced. If there is a persistent failure
  // this can drain your battery...
  if (Watchy_SyncTime::lastSyncTimeTS < SECS_YR_2000) {
    timeSync.begin();
  }

  if (Watchy_GetLocation::lastGetLocationTS < SECS_YR_2000) {
    getLocation.begin();
  }

  if (Watchy_GetWeather::lastGetWeatherTS < SECS_YR_2000) {
    getWeather.begin();
  }

  if (Watchy::screen == nullptr) {
    Watchy::screen = &carousel;
  }
  Watchy::init();
}

void loop() {
  // should never be called, but if it is we want to know how often
  static int count;
  count++;
  if (count % 10000 == 0) {
    log_i("%d", count/10000);
  }
  Watchy_Event::Event::handleAll();
}