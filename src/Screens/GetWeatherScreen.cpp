#include "GetWeatherScreen.h"

#include <Arduino_JSON.h>

#include "GetWeather.h"
#include "OptimaLTStd12pt7b.h"
#include "Watchy.h"

enum GetWeatherState {
  ready,
  waiting,
  done
};

RTC_DATA_ATTR GetWeatherState getWeatherState = ready;

void printWeather(Watchy_GetWeather::weatherData wd) {
  Watchy::display.printf("\nsuccess\nTemp %d\nWeather %d", wd.temperature,
                         wd.weatherConditionCode);
}

void GetWeatherScreen::show() {
  Watchy::RTC.setRefresh(RTC_REFRESH_NONE);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  switch (getWeatherState) {
    case ready: {
      getWeatherState = waiting;
      Watchy::display.print("\nwaiting");
      Watchy::display.display(true);
      Watchy::display.fillScreen(bgColor);
      Watchy::display.setCursor(0, 0);
      auto weather = Watchy_GetWeather::getWeather();
      getWeatherState = done;
      printWeather(weather);
      break;
    }
    case waiting:
      Watchy::display.print("\nwaiting");
      break;
    case done:
      printWeather(Watchy_GetWeather::getWeather());
      break;
    default:
      return;
  }
  Watchy::display.print("\npress back to exit");
}

void GetWeatherScreen::back() {
  getWeatherState = ready;
  Screen::back();
}
