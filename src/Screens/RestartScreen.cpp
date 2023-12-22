#include "RestartScreen.h"

#include <Arduino_JSON.h>

#include "GetWeather.h"
#include "OptimaLTStd12pt7b.h"
#include "Watchy.h"


void RestartScreen::show() {
  Watchy::RTC.setRefresh(RTC_REFRESH_NONE);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  
  Watchy::display.print("\npress back to exit");
  Watchy::display.print("\npress menu to reset");
  
}

void RestartScreen::menu() {
  ESP.restart();
}

void RestartScreen::back() {
  Screen::back();
}
