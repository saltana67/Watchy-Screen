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
  Watchy_GetWeather::getForecast(true);
  // assumes TEMP_UNITS is metric, and that wd.temperature is in C
  /*
  display.printf("\n\n%-20s,\n%dC %3.0fF\n%s", 
    wd.weatherCity,
    wd.temperature,
    ((wd.temperature*1.8)+32),
    weatherConditionCodeToString(wd.weatherConditionCode)
  );
  */

  int8_t minTemp = 0;
  int8_t maxTemp = 0;
  int8_t tempRange = 0;

  log_i("nr_forecasts: %d", Watchy_GetWeather::nr_forecasts);

  for( int i = 0; i < Watchy_GetWeather::nr_forecasts; i++ ) {
    Watchy_GetWeather::weatherData forecast = Watchy_GetWeather::forecastWeather[i];
    int8_t temp = forecast.temperature;
    if( temp < minTemp )
      minTemp = temp;
    if( temp > maxTemp )
      maxTemp = temp;
    log_i("forecast %d: temp: %d min: %d max: %d", i, temp, minTemp, maxTemp);
  }
  tempRange = maxTemp - minTemp;
  log_i("tempRange: %d", tempRange); 
  display.printf("\n\nnr_forecasts:%d\ntemp: %d - %d : %d"
    , Watchy_GetWeather::nr_forecasts
    , minTemp, maxTemp, tempRange
  );

  float fgColor = (bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE);

  const int16_t height = 50;
  const int16_t width  = 40 * 3; //3 pixel per forecast
  const int16_t x0 = 200 - width;
  const int16_t y0 = 120;

  Watchy::display.drawLine(x0, y0,   199, y0 + height,   fgColor);
//  Watchy::display.drawLine(x0, y0+1, 199, y0 + height+1, fgColor);
  
  const float scale = ((float) height) / ((float) tempRange);
  int16_t x = x0;
  int16_t y = y0;

  for( int i = 0; i < Watchy_GetWeather::nr_forecasts; i++ ) {
    Watchy_GetWeather::weatherData forecast = Watchy_GetWeather::forecastWeather[i];
    int8_t temp = forecast.temperature;

    const int16_t lastX = x;
    const int16_t lastY = y;

    x += 3;
    y = y0 + height - int( ( ((float)(temp - minTemp)) *  scale) + 0.5);
  
    Watchy::display.drawLine(lastX, lastY,   x, y,    fgColor);
    Watchy::display.drawLine(lastX, lastY+1, x, y+1,  fgColor);
    Watchy::display.drawLine(lastX, lastY-1, x, y-1,  fgColor);
    
  /*
      log_d("drawLine(%d, %d, %d, %d, fgColor)", x0 + i, y0 + height - h, x0 + i, y0 + height);
      Watchy::display.drawLine(x0 + i, y0 + height - h, x0 + i, y0 + height,
                               fgColor);
  */
  }
}
