#include "TimezoneScreen.h"
#include "world_map.h"

using namespace Watchy;

RTC_DATA_ATTR boolean TimezoneScreen::darkMode = false;

void TimezoneScreen::show(){
  drawImage(world_map);//200x112px
}

void TimezoneScreen::drawImage(const unsigned char world_map []){
  int fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
  int bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;

  Watchy::display.fillScreen(bgColor);
    // even space between top<->icon<->label<->bottom
    //int space = (DISPLAY_HEIGHT - (height + h)) / 3;
    Watchy::display.drawBitmap(0,200-112, world_map, 200, 112, fgColor);
}

void TimezoneScreen::menu() {
    log_d("darkMode: %d", darkMode);
    darkMode = darkMode ? false : true;
    log_d("darkMode toggled: %d", darkMode);
    Watchy::showWatchFace(true);
}
