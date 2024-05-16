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
  drawLocation(&Watchy_GetLocation::currentLocation);
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

void TimezoneScreen::drawLocation(const Watchy_GetLocation::location *loc){
  drawLocation(loc->lat,loc->lon);
}

void TimezoneScreen::drawLocation(const float latitude, const float longitude){
  int fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
  int bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;

  float lat = latitude; //-60 85
  float lon = longitude;//-180 +180

  //map range [-180,180] (length 360) to [0,200] (200 image width)
  const double lonScale = (200.0L/360.0L);
  int16_t x = (lon+180.0f)*lonScale;

  //map range [-60,85] (length 145) to [0,112] (112 image height)
  if ( lat < -60.0f) lat = -60.0f;
  if ( lat >  85.0f) lat =  85.0f;
  const double latScale = (112.0L/145.0L);
  int16_t y = (85.0f - lat)*latScale;

  log_d("[%f,%f] -> [%i,%i]: ", longitude, latitude, x,y);

  y = y + 88;

  Watchy::display.drawFastHLine(0,y,200,fgColor);
  Watchy::display.drawFastVLine(x,88,112,fgColor);
  Watchy::display.drawCircle(x, y, 5, bgColor);
  Watchy::display.drawCircle(x, y, 4, fgColor);
  Watchy::display.drawCircle(x, y, 3, bgColor);
}

void TimezoneScreen::menu() {
    log_d("darkMode: %d", darkMode);
    darkMode = darkMode ? false : true;
    log_d("darkMode toggled: %d", darkMode);
    Watchy::showWatchFace(true);
}
