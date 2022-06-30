#include "SetLocationScreen.h"

#include "OptimaLTStd12pt7b.h"
#include "GetLocation.h"
#include "IPAddress.h"

enum GetLocationState {
  ready,
  waiting,
  done
};

RTC_DATA_ATTR GetLocationState getLocationState = ready;

void printLocation(const Watchy_GetLocation::location *loc) {
  Watchy::display.printf("\nsuccess\nlat %.4f\nlon %.4f",
                         loc->lat, loc->lon);
  Watchy::display.printf("\n%s", loc->timezone);
}

void SetLocationScreen::show() {
  Watchy::RTC.setRefresh(RTC_REFRESH_NONE);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  switch (getLocationState) {
    case ready: {
      getLocationState = waiting;
      Watchy::display.print("\nwaiting");
      Watchy::display.display(true);
      Watchy::display.fillScreen(bgColor);
      Watchy::display.setCursor(0, 0);
      Watchy_GetLocation::getLocation();
      getLocationState = done;
      printLocation(&Watchy_GetLocation::currentLocation);
      break;
    }
    case waiting:
      Watchy::display.print("\nwaiting");
      break;
    case done:
      printLocation(&Watchy_GetLocation::currentLocation);
      break;
    default:
      return;
  }
  Watchy::display.print("\npress back to exit");
}

void SetLocationScreen::back() {
  getLocationState = ready;
  Screen::back();
}