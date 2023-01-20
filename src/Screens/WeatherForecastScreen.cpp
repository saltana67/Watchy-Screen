#include "WeatherForecastScreen.h"

#include "GetWeather.h"
#include "OptimaLTStd12pt7b.h"

using namespace Watchy;


void WeatherForecastScreen::show() {
  Watchy::RTC.setRefresh(RTC_REFRESH_NONE);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  Watchy::display.print("\nwaiting");
  Watchy::display.display(true);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setCursor(0, 0);
  Watchy_GetWeather::getForecast();
  // assumes TEMP_UNITS is metric, and that wd.temperature is in C
  /*
  display.printf("\n\n%-20s,\n%dC %3.0fF\n%s", 
    wd.weatherCity,
    wd.temperature,
    ((wd.temperature*1.8)+32),
    weatherConditionCodeToString(wd.weatherConditionCode)
  );
  */
  display.printf("\n\nnr_forecasts:%d", Watchy_GetWeather::nr_forecasts);
}
