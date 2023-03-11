#include "WeatherForecastScreen.h"

#include "GetWeather.h"
#include "OptimaLTStd12pt7b.h"

using namespace Watchy;

  typedef struct int8_stats {
    int8_t min;
    int8_t max;
    int8_t range;
  } int8_stats_t;

  typedef struct int16_stats {
    int16_t min;
    int16_t max;
    int16_t range;
  } int16_stats_t;

  typedef struct float_stats {
    float min;
    float max;
    float range;
  } float_stats_t;

  typedef struct stats {
    int8_stats_t temperature;
    int16_stats pressure;
    float_stats windSpeed;
  } stats_t;

void initStats(int8_stats &stats, int8_t value){
    stats.min = value;
    stats.max = value;
    stats.range = 0;
}
void updateStats(int8_stats &stats, int8_t value){
    if( value < stats.min )
      stats.min = value;
    if( value > stats.max )
      stats.max = value;
    stats.range = stats.max - stats.min;
}
void initStats(int16_stats &stats, int16_t value){
    stats.min = value;
    stats.max = value;
    stats.range = 0;
}
void updateStats(int16_stats &stats, int16_t value){
    if( value < stats.min )
      stats.min = value;
    if( value > stats.max )
      stats.max = value;
    stats.range = stats.max - stats.min;
}
void initStats(float_stats &stats, float value){
    stats.min = value;
    stats.max = value;
    stats.range = 0;
}
void updateStats(float_stats &stats, float value){
    if( value < stats.min )
      stats.min = value;
    if( value > stats.max )
      stats.max = value;
    stats.range = stats.max - stats.min;
}
void initStats(stats_t &stats, Watchy_GetWeather::weatherData weather){
    initStats(stats.temperature ,weather.temperature);
    initStats(stats.pressure    ,weather.pressure);
    initStats(stats.windSpeed   ,weather.wind.speed);
}
void updateStats(stats_t &stats, Watchy_GetWeather::weatherData weather){
    updateStats(stats.temperature ,weather.temperature);
    updateStats(stats.pressure    ,weather.pressure);
    updateStats(stats.windSpeed   ,weather.wind.speed);
}


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


  stats_t stats;

  initStats(stats,Watchy_GetWeather::currentWeather);

//  int8_stats_t   tempStats = {Watchy_GetWeather::currentWeather.temperature,Watchy_GetWeather::currentWeather.temperature,0};
//  int16_stats_t presStats = {Watchy_GetWeather::currentWeather.pressure,Watchy_GetWeather::currentWeather.pressure,0};
//  float_stats_t wndsStats = {Watchy_GetWeather::currentWeather.wind.speed,Watchy_GetWeather::currentWeather.wind.speed,0};

  log_i("nr_forecasts: %d", Watchy_GetWeather::nr_forecasts);

  for( int i = 0; i < Watchy_GetWeather::nr_forecasts; i++ ) {
    Watchy_GetWeather::weatherData forecast = Watchy_GetWeather::forecastWeather[i];
    updateStats(stats,forecast);
//    updateStats(tempStats,forecast.temperature);
//    updateStats(presStats,forecast.pressure);
//    updateStats(wndsStats,forecast.pressure);
//    if( temp < tempStats.min )
//      tempStats.min = temp;
//    if( temp > tempStats.max )
//      tempStats.max = temp;
//    log_d("forecast %d: temp: %d min: %d max: %d", i, temp, minTemp, maxTemp);
  }
//  tempStats.range = tempStats.max - tempStats.min;
//  presStats.range = presStats.max - presStats.min;
//  wndsStats.range = wndsStats.max - wndsStats.min;

  int8_stats_t   tempStats = stats.temperature;

  log_d("tempRange: %d", tempStats.range); 
  Watchy::display.printf("\n\nnr_forecasts:%d\ntemp: %d - %d : %d"
    , Watchy_GetWeather::nr_forecasts
    , tempStats.min, tempStats.max, tempStats.range
  );

  float fgColor = (bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE);

  const int16_t height = 50;
  const int16_t width  = 40 * 3; //3 pixel per forecast
  const int16_t x0 = 200 - width;
  const int16_t y0 = 120;

//  Watchy::display.drawLine(x0, y0,   199, y0 + height,   fgColor);
//  Watchy::display.drawLine(x0, y0+1, 199, y0 + height+1, fgColor);
  
  const float scale = ((float) height) / ((float) tempStats.range);

  int16_t x = x0;
  int16_t y = y0 + height - int( ( ((float)(Watchy_GetWeather::currentWeather.temperature - tempStats.min)) *  scale) + 0.5);;

  for( int i = 0; i < Watchy_GetWeather::nr_forecasts; i++ ) {
    Watchy_GetWeather::weatherData forecast = Watchy_GetWeather::forecastWeather[i];
    int8_t temp = forecast.temperature;

    const int16_t lastX = x;
    const int16_t lastY = y;

    x += 3;
    y = y0 + height - int( ( ((float)(temp - tempStats.min)) *  scale) + 0.5);
  
    Watchy::display.drawLine(lastX, lastY,   x, y,    fgColor);
    Watchy::display.drawLine(lastX, lastY+1, x, y+1,  fgColor);
    Watchy::display.drawLine(lastX, lastY-1, x, y-1,  fgColor);
    Watchy::display.drawLine(lastX, lastY+2, x, y+2,  fgColor);
    Watchy::display.drawLine(lastX, lastY-2, x, y-2,  fgColor);
    
  /*
      log_d("drawLine(%d, %d, %d, %d, fgColor)", x0 + i, y0 + height - h, x0 + i, y0 + height);
      Watchy::display.drawLine(x0 + i, y0 + height - h, x0 + i, y0 + height,
                               fgColor);
  */
  }
}
