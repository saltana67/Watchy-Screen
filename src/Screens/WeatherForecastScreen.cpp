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
    int8_stats_t pop;
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
    initStats(stats.pop         ,weather.pop);
}
void updateStats(stats_t &stats, Watchy_GetWeather::weatherData weather){
    updateStats(stats.temperature ,weather.temperature);
    updateStats(stats.pressure    ,weather.pressure);
    updateStats(stats.windSpeed   ,weather.wind.speed);
    updateStats(stats.clouds      ,weather.clouds);
    updateStats(stats.humidity    ,weather.humidity);
    updateStats(stats.pop         ,weather.pop);
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
float getPop(Watchy_GetWeather::weatherData weather) {
  return (float) weather.pop;
}

void drawWeatherTimeline(const int16_t x0, const int16_t y0, 
                    const int16_t width, const int16_t height,
                    const String label,
                    const String units,
                    const int precission,
                    const float min, const float max,
                    float (*getValue)(Watchy_GetWeather::weatherData),
                    const uint16_t fgColor){
  
  const float scale = ((float) height) / (max - min);
  log_d("scale: %f ", scale);
  const float xScale = ((float) width) / ((float) Watchy_GetWeather::nr_forecasts);
  log_d("xScale: %f ", xScale);

  float value = getValue(Watchy_GetWeather::currentWeather);
  log_d("first value: %d ", value);

  display.setFont(OptimaLTStd12pt7b);
  display.setCursor(0, y0+(height/2)+16);
  if( precission > 0 )
    display.print(value,precission);
  else
    display.print(int(value));

  display.setFont(OptimaLTStd7pt7b);

  display.setCursor(0, y0+14);
  display.print(label);

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
  display.setCursor(x0-20, y0+(height/2)+5);
  display.print(units);

  Watchy::display.drawLine(x0, y0,        x0+width, y0        ,fgColor);
  Watchy::display.drawLine(x0, y0+height, x0+width, y0+height ,fgColor);

  int16_t x = x0;
  int16_t y = y0 + height - int( ( ((float)(value - min)) *  scale) + 0.5);

  for( int i = 0; i < Watchy_GetWeather::nr_forecasts; i++ ) {
    Watchy_GetWeather::weatherData forecast = Watchy_GetWeather::forecastWeather[i];
    value = getValue(forecast);

    const int16_t lastX = x;
    const int16_t lastY = y;

    //x += 3;
    x = x0 + int( ( ((float)(i+1)) * xScale) + 0.5);
    y = y0 + height - int( ( ((float)(value - min)) * scale) + 0.5);

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

void drawNightBackground(const int16_t x0, const int16_t y0, 
                    const int16_t width, const int16_t height,
                    const uint16_t fgColor){

  int16_t offset = 0;
  for( int16_t y = y0; y < (y0 + height); y += 2 ){
    for( int16_t x = x0 + offset; x < (x0 + width); x += 2){
      display.drawPixel(x,y,fgColor);
    }
    offset = 1 - offset;
  }
}

void drawWeatherTimelineBackground(const int16_t x0, const int16_t y0, 
                    const int16_t width, const int16_t height,
                    const uint16_t fgColor){


  //drawNightBackground(x0+10,y0,12, height, fgColor);

  int16_t x = x0;
  int hour = 0;

  boolean night = false;
  int16_t night_x;
  int16_t day_x = x0;
  
  for( int i = 0; i < Watchy_GetWeather::nr_forecasts; i++ ){
    Watchy_GetWeather::weatherData forecast = Watchy_GetWeather::forecastWeather[i];

    tm t;
    localtime_r(&forecast.dt, &t);
    display.setFont(OptimaLTStd7pt7b);

    if( forecast.night != night ) {
      if( night ){
        /* end of night -> draw night background */
        log_d("end of night: %d:%d - %d:%d", night_x, y0, (x-night_x)-1, height);
        drawNightBackground(night_x, y0, (x-night_x)-1, height, fgColor );
        night_x = x-1;
      }else{
        /* start of night -> save night background x */
        night_x = x-1;
        log_d("start of night: %d", night_x);
      }
      night = forecast.night;
    }

    log_d("forecast %d: hour: %d, day: %d, month: %d", i, t.tm_hour, t.tm_mday, t.tm_mon);
    if( t.tm_hour == 0 || (t.tm_hour < hour) ){
      log_d("midnight line: %d:%d->%d:%d", x, y0, x, y0+height);
      if( t.tm_hour == 0 )
        Watchy::display.drawLine(x, y0,   x,   y0+height,    fgColor);
      else
        Watchy::display.drawLine(x-1, y0, x-1, y0+height,    fgColor);

      if( (x - day_x) > 6 ){
        //ErrSunMonTueWedThuFriSat
        //0  1  2  3  4  5  6  7
        //   1  2  3  4  5  6  0
        String wDay = dayShortStr(t.tm_wday==0?7:t.tm_wday);
        wDay = wDay.substring(0,wDay.length() - 1);
        const int16_t wDay_x = day_x+3;
        const int16_t wDay_y = y0+9;
        log_d("tm_wday: %d, day: %s, x: %d, y: %d", t.tm_wday, wDay, wDay_x, wDay_y);
        display.setCursor(wDay_x, wDay_y);
        display.print(wDay);
      }   
      day_x = x;
    }
    hour = t.tm_hour;
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

  const int16_t nrTimelines = 4;
  const int16_t headerHeight = 12;
  const int16_t topMargin = 4;
  const int16_t height = ((display.height() - headerHeight) / nrTimelines) - topMargin;
  const int16_t width  = 40 * 3; //3 pixel per forecast
  const int16_t x0 = display.width() - width;

  
  int16_t y0 = 0;

  drawWeatherTimelineBackground(x0, y0, width, display.height(), fgColor);

  y0 = headerHeight + topMargin;
  drawWeatherTimeline(x0, y0, width, height, 
                      "Temp","degC",0,(float) stats.temperature.min, (float) stats.temperature.max,
                      getTemperature, fgColor);

  y0 += height + topMargin;

  drawWeatherTimeline(x0, y0, width, height, 
                      "Pressure","hPa",0,(float) stats.pressure.min, (float) stats.pressure.max,
                      getPressure, fgColor);

  y0 += height+ topMargin;

  drawWeatherTimeline(x0, y0, width, height, 
                      "Clouds", "%", 0,(float) stats.clouds.min, (float) stats.clouds.max,
                      getClouds, fgColor);

  // y0 += height + topMargin;

  // drawWeatherTimeline(x0, y0, width, height, 
  //                     "Precip","%",0,(float) stats.pop.min, (float) stats.pop.max,
  //                     getPop, fgColor);

  // y0 += height + topMargin;
  // drawWeatherTimeline(x0, y0, width, height, 
  //                     "Humidity","%",0,(float) stats.humidity.min, (float) stats.humidity.max,
  //                     getHumidity, fgColor);

  y0 += height + topMargin;

  drawWeatherTimeline(x0, y0, width, height, 
                      "Wind","m/s", 0,(float) stats.windSpeed.min, (float) stats.windSpeed.max,
                      getWindSpeed, fgColor);


}
