#include "ResetWifiScreen.h"

#include <Arduino_JSON.h>

#include "GetWeather.h"
#include "OptimaLTStd12pt7b.h"
#include "Watchy.h"


void ResetWifiScreen::show() {
  Watchy::RTC.setRefresh(RTC_REFRESH_NONE);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  
  Watchy::resetWiFi();
  if( Watchy::getWiFi()){
    Watchy::releaseWiFi();
    Watchy::display.print("\nwifi reset.\n");
  }else
    Watchy::display.print("\nwifi error.\n");
  
  Watchy::display.print("press back to exit");
}

void ResetWifiScreen::back() {
  Screen::back();
}
