#include "TimezoneScreen.h"
#include "OptimaLTStd12pt7b.h"
#include "world_map.h"

using namespace Watchy;

RTC_DATA_ATTR boolean TimezoneScreen::darkMode = false;

void TimezoneScreen::show(){
  Watchy::display.fillScreen(darkMode ? GxEPD_BLACK : GxEPD_WHITE);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  Watchy::display.setCursor(0, 0);
  Watchy::display.setTextColor(darkMode ? GxEPD_WHITE : GxEPD_BLACK);
  printLocationTimezone(&Watchy_GetLocation::currentLocation);
  drawImage(world_map);//200x112px
}

void TimezoneScreen::printLocationTimezone(const Watchy_GetLocation::location *loc) {
  //Watchy::display.printf("\nsuccess\nlat %.4f\nlon %.4f",
  //                       loc->lat, loc->lon);
  //Watchy::display.printf("\n%s", loc->timezone);
  log_d("timezone: %s - %s city: %s" ,loc->olsonTimezone,loc->timezone, loc->city);
  Watchy::display.printf("\n%s\n%s", sizeof(loc->city) > 0 ? loc->city : loc->olsonTimezone, loc->timezone);
}

void TimezoneScreen::drawImage(const unsigned char world_map []){
  int fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
  int bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;

  //Watchy::display.fillScreen(bgColor);
  // even space between top<->icon<->label<->bottom
  //int space = (DISPLAY_HEIGHT - (height + h)) / 3;
  Watchy::display.drawBitmap(0,200-112, world_map, 200, 112, fgColor);
  //Watchy::display.drawImagePart(world_map, 64, 64, 200, 112, 100, 200-112, 64, 64);
}

void TimezoneScreen::menu() {
    log_d("darkMode: %d", darkMode);
    darkMode = darkMode ? false : true;
    log_d("darkMode toggled: %d", darkMode);
    Watchy::showWatchFace(true);
}
