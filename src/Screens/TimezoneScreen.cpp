#include "TimezoneScreen.h"
#include "OptimaLTStd12pt7b.h"
#include "world_map.h"

using namespace Watchy;

RTC_DATA_ATTR boolean TimezoneScreen::darkMode = false;

void TimezoneScreen::show(){
  Watchy::display.fillScreen(darkMode ? GxEPD_BLACK : GxEPD_WHITE);
  if( Watchy::display.epd2.darkBorder != darkMode )
    Watchy::display.epd2.setDarkBorder(darkMode);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  Watchy::display.setCursor(0, 0);
  Watchy::display.setTextColor(darkMode ? GxEPD_WHITE : GxEPD_BLACK);
  printLocationTimezone(&Watchy_GetLocation::currentLocation);
  drawTimezoneMap(world_map, &Watchy_GetLocation::currentLocation);//200x112px
  drawLocation(&Watchy_GetLocation::currentLocation);
}

void TimezoneScreen::printLocationTimezone(const Watchy_GetLocation::location *loc) {
  //Watchy::display.printf("\nsuccess\nlat %.4f\nlon %.4f",
  //                       loc->lat, loc->lon);
  //Watchy::display.printf("\n%s", loc->timezone);
  log_d("timezone: %s - %s city: %s" ,loc->olsonTimezone,loc->timezone, loc->city);
  Watchy::display.printf("\n%s\n%s", sizeof(loc->city) > 0 ? loc->city : loc->olsonTimezone, loc->timezone);
}

/// Convert angle from degrees to radians.
inline constexpr double deg_to_rad(double degree) noexcept {
    return degree * (PI / 180.0);
}

inline double lon_to_x(double lon){
  const double lonScale = (200.0L/360.0L);
  double x = (lon+180.0f)*lonScale;
  return x;
}

// faster version of the canonical log(tan()) version
inline double lat_to_y_with_sin(double lat) {
    const double f = std::sin(deg_to_rad(lat));
    const double y = std::log((1 + f) / (1 - f)) / 2.0f;
    return y;
}

inline double lat_to_y(double lat, double width=190, double y0=((200-112)+(112/2)+16)) {
    double mercN = 0.0f;
    if (lat < -78.0 || lat > 78.0)
        mercN = lat_to_y_with_sin(lat);
    else
      // get y value
      mercN = log(tan((PI/4.0f)+(deg_to_rad(lat)/2.0f)));
    //return mercN;
    double y   = (width/(2*PI))*mercN;
    return y0-y;
}

void TimezoneScreen::drawTimezoneMap(const unsigned char world_map [], const Watchy_GetLocation::location *loc){
  int fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
  int bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;
  // int mapColor0 = darkMode ? GxEPD_GREY : GxEPD_GREY;
  // int mapColor1 = darkMode ? GxEPD_GREY : GxEPD_GREY;
  int mapColor0 = darkMode ? GxEPD_GREY : GxEPD_GREY;
  int mapColor1 = darkMode ? GxEPD_GREY : GxEPD_GREY;
  int zoneBgColor0 = darkMode ? GxEPD_2_16_MIRRORED_INVERS : GxEPD_2_16_MIRRORED;
  int zoneBgColor1 = darkMode ? bgColor : bgColor;
  int equatorColor = darkMode ? GxEPD_RIGHT_DIAG_INVERS : GxEPD_RIGHT_DIAG;
  //int zoneBgColor = darkMode ? GxEPD_DARKGREY : GxEPD_LIGHTGREY;
  //int zoneBgColor = darkMode ? GxEPD_RIGHT_DIAG_INVERS : GxEPD_RIGHT_DIAG;
  int currentZoneFgColor = darkMode ? bgColor : bgColor;
  int currentZoneBgColor = darkMode ? fgColor : fgColor;

  //Watchy::display.fillScreen(bgColor);
  // even space between top<->icon<->label<->bottom
  //int space = (DISPLAY_HEIGHT - (height + h)) / 3;
  //Watchy::display.drawBitmap(0,200-112, world_map, 200, 112, /*fgColor*/mapColor);

  int16_t x = lon_to_x(loc->lon);
  int currentTimezone = x/(200/25);
  log_d("lon: %f, x: %i -> currentTimezone: %i", loc->lon, x, currentTimezone);

  for( int i=0; i<25; i++){
      Watchy::display.drawBitmapPart(0,200-112, world_map, 200, 112, i*(200/25), 200/25, i%2?mapColor0:mapColor1,i%2?zoneBgColor0:zoneBgColor1 );
  }
  Watchy::display.drawFastHLine(0,lat_to_y(0),200,equatorColor);
  Watchy::display.drawBitmapPart(0,200-112, world_map, 200, 112, currentTimezone*(200/25), 200/25, currentZoneFgColor,currentZoneBgColor );
  if( false /*! darkMode */){
    Watchy::display.drawRect(currentTimezone*(200/25),200-112,200/25,112,fgColor);
  }
}

void TimezoneScreen::drawLocation(const Watchy_GetLocation::location *loc, bool lines){
  drawLocation(78.661136,-32.730804, false); //top of Greenland
  drawLocation(59.624911,-44.075582, false); //bottom of Greenland
  drawLocation(-34.913285f,19.969203, false); //Cap Good Hope
//  drawLocation(8.922576, -80.083389); //Panama
  drawLocation(-55.978367,-67.276516, false); //Cap Horn
//  drawLocation(0.0f,0.0f, false); //dead center ;)
  drawLocation(loc->lat,loc->lon, false);
}

void TimezoneScreen::drawLocation(const float latitude, const float longitude, bool lines){
  int fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
  int bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;

  float lat = latitude; //-60 85
  float lon = longitude;//-180 +180

  //map range [-180,180] (length 360) to [0,200] (200 image width)
  int16_t x = lon_to_x(lon);

  //map range [-60,85] (length 145) to [0,112] (112 image height)
  if ( lat < -60.0f) lat = -60.0f;
  if ( lat >  85.0f) lat =  85.0f;
  int16_t y = lat_to_y(lat);

  log_d("[%f,%f] -> [%i,%i]", longitude, latitude, x,y);

  if( lines ) {
  Watchy::display.drawFastHLine(0,y,200,fgColor);
  Watchy::display.drawFastVLine(x,88,112,fgColor);
  }
  Watchy::display.drawCircle(x, y, 5, fgColor);
//  Watchy::display.drawCircle(x, y, 4, GxEPD_INVERT);
//  Watchy::display.drawCircle(x, y, 3, GxEPD_INVERT);
}

void TimezoneScreen::menu() {
    log_d("darkMode: %d", darkMode);
    darkMode = darkMode ? false : true;
    log_d("darkMode toggled: %d", darkMode);
    Watchy::showWatchFace(true);
}
