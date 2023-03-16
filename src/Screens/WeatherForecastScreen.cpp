#include "WeatherForecastScreen.h"

#include "GetWeather.h"
#include "OptimaLTStd12pt7b.h"
#include "OptimaLTStd7pt7b.h"

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
    int8_stats_t clouds;
    int8_stats_t humidity;
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
    initStats(stats.clouds      ,weather.clouds);
    initStats(stats.humidity    ,weather.humidity);
}
void updateStats(stats_t &stats, Watchy_GetWeather::weatherData weather){
    updateStats(stats.temperature ,weather.temperature);
    updateStats(stats.pressure    ,weather.pressure);
    updateStats(stats.windSpeed   ,weather.wind.speed);
    updateStats(stats.clouds      ,weather.clouds);
    updateStats(stats.humidity    ,weather.humidity);
}

float getTemperature(Watchy_GetWeather::weatherData weather) {
  return (float) weather.temperature;
}
float getPressure(Watchy_GetWeather::weatherData weather) {
  return (float) weather.pressure;
}
float getClouds(Watchy_GetWeather::weatherData weather) {
  return (float) weather.clouds;
}
float getWindSpeed(Watchy_GetWeather::weatherData weather) {
  return (float) weather.wind.speed;
}
float getHumidity(Watchy_GetWeather::weatherData weather) {
  return (float) weather.humidity;
}

void drawWeatherTimeline(const int16_t x0, const int16_t y0, 
                    const int16_t width, const int16_t height,
                    const int precission,
                    const float min, const float max,
                    float (*getValue)(Watchy_GetWeather::weatherData),
                    const uint16_t fgColor){
  
  const float scale = ((float) height) / (max - min);
  log_d("scale: %f ", scale);

  float value = getValue(Watchy_GetWeather::currentWeather);
  log_d("first value: %d ", value);

  display.setFont(OptimaLTStd12pt7b);
  display.setCursor(0, y0+(height/2)+6);
  if( precission > 0 )
    display.print(value,precission);
  else
    display.print(int(value));

  display.setFont(OptimaLTStd7pt7b);
  display.setCursor(x0-20, y0+10);
  if( precission > 0 )
    display.print(max,precission);
  else
    display.print(int(max));
  display.setCursor(x0-20, y0+height-3);
  if( precission > 0 )
    display.print(min,precission);
  else
    display.print(int(min));

  Watchy::display.drawLine(x0, y0,        x0+width, y0        ,fgColor);
  Watchy::display.drawLine(x0, y0+height, x0+width, y0+height ,fgColor);

  int16_t x = x0;
  int16_t y = y0 + height - int( ( ((float)(value - min)) *  scale) + 0.5);

  for( int i = 0; i < Watchy_GetWeather::nr_forecasts; i++ ) {
    Watchy_GetWeather::weatherData forecast = Watchy_GetWeather::forecastWeather[i];
    value = getValue(forecast);

    const int16_t lastX = x;
    const int16_t lastY = y;

    x += 3;
    y = y0 + height - int( ( ((float)(value - min)) *  scale) + 0.5);

    log_d("value: %d, line: %d:%d  -> %d:%d", value, lastX, lastY, x, y);
  
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

void drawWeatherTimelineBackground(const int16_t x0, const int16_t y0, 
                    const int16_t width, const int16_t height,
                    const uint16_t fgColor){

  int16_t x = x0;
  for( int i = 0; i < Watchy_GetWeather::nr_forecasts; i++ ){
    Watchy_GetWeather::weatherData forecast = Watchy_GetWeather::forecastWeather[i];
    tm t;
    localtime_r(&forecast.dt, &t);
    if( t.tm_hour == 0 ){
      Watchy::display.drawLine(x, y0,   x, y0+height,    fgColor);
    }
    x += 3;
  }
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

  log_i("nr_forecasts: %d", Watchy_GetWeather::nr_forecasts);

  for( int i = 0; i < Watchy_GetWeather::nr_forecasts; i++ ) {
    Watchy_GetWeather::weatherData forecast = Watchy_GetWeather::forecastWeather[i];
    updateStats(stats,forecast);
  }

  int8_stats_t   tempStats = stats.temperature;

  log_d("tempRange: %d", tempStats.range); 

  uint16_t fgColor = (bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE);

  const int16_t height = 40;
  const int16_t width  = 40 * 3; //3 pixel per forecast
  const int16_t x0 = 200 - width;

  int16_t y0 = 0;
  
  drawWeatherTimelineBackground(x0, y0, width, height, fgColor);

  drawWeatherTimeline(x0, y0, width, height, 
                      0,(float) stats.temperature.min, (float) stats.temperature.max,
                      getTemperature, fgColor);

  y0 += height;

  drawWeatherTimeline(x0, y0, width, height, 
                      0,(float) stats.pressure.min, (float) stats.pressure.max,
                      getPressure, fgColor);

  y0 += height;

  drawWeatherTimeline(x0, y0, width, height, 
                      0,(float) stats.humidity.min, (float) stats.humidity.max,
                      getHumidity, fgColor);

  y0 += height;

  drawWeatherTimeline(x0, y0, width, height, 
                      0,(float) stats.windSpeed.min, (float) stats.windSpeed.max,
                      getWindSpeed, fgColor);

  y0 += height;

  drawWeatherTimeline(x0, y0, width, height, 
                      0,(float) stats.clouds.min, (float) stats.clouds.max,
                      getClouds, fgColor);

}
