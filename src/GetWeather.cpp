#include "GetWeather.h"

#include <Arduino_JSON.h>

#include "GetLocation.h"
#include "Watchy.h"
#include "WatchyErrors.h"
#include "config.h"  // should be first

#include "WeatherForecastHandler.h"
#include <ArduinoStreamParser.h>

namespace Watchy_GetWeather {
RTC_DATA_ATTR weatherData currentWeather = {
                                              .temperature = 22
                                            , .weatherConditionCode = 800
                                            , .pressure = 1015
                                            , .humidity = 50
                                            , .wind = {0.0f, 0, 0.0f}
                                            };
static const size_t MAX_FORECASTS = 40;
RTC_DATA_ATTR size_t nr_forecasts = 0; 
RTC_DATA_ATTR weatherData forecastWeather[MAX_FORECASTS]; 

RTC_DATA_ATTR time_t lastGetWeatherTS = 0;

boolean parseWeatherData( weatherData &weatherData, JSONVar &weatherDataJson ){
  weatherData.dt = long(weatherDataJson["dt"]);
  weatherData.temperature = int(weatherDataJson["main"]["temp"]);
  weatherData.weatherConditionCode = int(weatherDataJson["weather"][0]["id"]);
  weatherData.pressure = int(weatherDataJson["main"]["pressure"]);
  weatherData.humidity = int(weatherDataJson["main"]["humidity"]);

  weatherData.wind.speed      = (float) double(weatherDataJson["wind"]["speed"]);
  weatherData.wind.direction  = int(weatherDataJson["wind"]["deg"]);
  weatherData.wind.gust       = (float) double(weatherDataJson["wind"]["gust"]);

  weatherData.clouds = int(weatherDataJson["clouds"]["all"]);
  weatherData.visibility = int(weatherDataJson["visibility"]);
  
  weatherData.pop = 0;
  
  if( weatherDataJson.hasOwnProperty("pop"))
    weatherData.pop = int((double(weatherDataJson["pop"]))*100.0);

  weatherData.rain = 0;

  if( weatherDataJson.hasOwnProperty("rain") ){
    JSONVar rain = weatherDataJson["rain"];
    if( rain.hasOwnProperty("1h") )
      weatherData.rain = int(rain["1h"] );
    else if( rain.hasOwnProperty("3h") )
      weatherData.rain = int(rain["3h"] );
  }
  if( weatherData.rain > 0)
    weatherData.pop = 100;

  weatherData.night = false;
  const char* pod_s = (const char *) weatherDataJson["sys"]["pod"];
  if( pod_s != null ) {
    //log_d("pod_s: %s", pod_s);
    char pod = pod_s[0]; //char(weatherDataJson["sys"]["pod"]);
    weatherData.night = (pod == 'n');
  }

  return true;
}


void weatherForecastCopy(weatherData &tgt, const weatherData &src){
        tgt.dt = src.dt
    ,   tgt.night = src.night
    ,   tgt.temperature = src.temperature
    ,   tgt.weatherConditionCode = src.weatherConditionCode
    ,   tgt.pressure = src.pressure
    ,   tgt.humidity = src.humidity
    ,   tgt.wind.speed = src.wind.speed
    ,   tgt.wind.direction = src.wind.direction
    ,   tgt.wind.gust = src.wind.gust
    ,   tgt.clouds = src.clouds
    ,   tgt.visibility = src.visibility
    ,   tgt.pop = src.pop
    ,   tgt.rain = src.rain
    ,   tgt.snow = src.snow
    ;
}

boolean _p_start = true;
boolean parseForecastResponse(HTTPClient& http){
    ArudinoStreamParser parser;                   // <---------- Declare stream parser
    WeatherForecastHandler custom_handler;        // <---------- Declare stream handler
    _p_start = true;
    
    custom_handler.setOnWeatherDataParsed([](weatherData &weather) { 
          if( _p_start ){
            nr_forecasts = 0;
            _p_start = false;
          }
          if( nr_forecasts >= MAX_FORECASTS ){
              log_w("nr_forecasts (%i) >= MAX_FORECASTS (%i), ignoring ... ", nr_forecasts, MAX_FORECASTS);
              return;
          }
          //log_weatherForecast(weather);

        // weatherData tgt = forecastWeather[tmpForecasts];
          weatherForecastCopy(forecastWeather[nr_forecasts],weather);
          
          //weatherData w = forecastWeather[tmpForecasts];
          //log_weatherForecast(forecastWeather[tmpForecasts]);
          nr_forecasts++;
      });
    parser.setHandler(&custom_handler); // <--------- Link to customer listener (parser to be honest)

    http.writeToStream(&parser); // <--------- Shoot the characters straight to the parser

    return true;
}

boolean parseForecastResponseOld(HTTPClient& http) {
      String payload = http.getString();
      log_i("response payload: %s", payload.c_str());
      JSONVar responseObject = JSON.parse(payload);
      //hasOwnProperty("cnt")
      nr_forecasts = 0;
      //const char* cnt_s = (const char *) responseObject["cnt"];
      //log_i("cnt_s: %s", cnt_s);
      int n = int(responseObject["cnt"]);
      log_i("n: %d", n);
      if(n<0)
        nr_forecasts = 0;
      else if( n > MAX_FORECASTS )
        nr_forecasts = MAX_FORECASTS;
      else
        nr_forecasts = (size_t) n;
      
      log_i("nr_forecasts: %d", nr_forecasts);
      JSONVar forecastList = responseObject["list"];

      for( int i = 0; i < nr_forecasts; i++){
        JSONVar forecastJson = forecastList[i];
        weatherData &weatherData = forecastWeather[i];
        parseWeatherData(weatherData,forecastJson);
        log_d("forecast %d: dt: %d, night: %d, temp %d, code %d, pressure: %d, humidity: %d, wind: %f %d %f, clouds: %d, visibility: %d, pop: %d, rain: %d, snow: %d", 
        i, 
        weatherData.dt, weatherData.night,
        weatherData.temperature, weatherData.weatherConditionCode,
        weatherData.pressure, weatherData.humidity, 
        weatherData.wind.speed, weatherData.wind.direction, weatherData.wind.gust,
        weatherData.clouds, weatherData.visibility, weatherData.pop, weatherData.rain, weatherData.snow
        );
      }
  return true;
}

void getForecast(boolean forceNow){
  // only update if WEATHER_UPDATE_INTERVAL has elapsed i.e. 30 minutes
  log_d("forceNow: %d, lastGetWeatherTS: %d, now() - lastGetWeatherTS: %d, WEATHER_UPDATE_INTERVAL: %d, check yelds: %d", 
          forceNow, lastGetWeatherTS,now() - lastGetWeatherTS, WEATHER_UPDATE_INTERVAL, (lastGetWeatherTS && (now() - lastGetWeatherTS < WEATHER_UPDATE_INTERVAL)));
  if ((!forceNow) && lastGetWeatherTS &&
      (now() - lastGetWeatherTS < WEATHER_UPDATE_INTERVAL)) {
    // too soon to update, just re-use existing values. Not an error
    log_d("RATE_LIMITED");
    Watchy::err = Watchy::RATE_LIMITED;
    //return currentWeather;
  }
  if (!Watchy::getWiFi()) {
    Watchy::err = Watchy::WIFI_FAILED;
    log_e("Wifi connect failed");
    // No WiFi, return RTC Temperature (this isn't actually useful...)
  }

  // WiFi is connected Use Weather API for live data
  HTTPClient http;
  http.setConnectTimeout(10000);  // 10 second max timeout
  const unsigned int weatherQueryURLSize =
        strlen(OPENWEATHERMAP_FORECAST_URL) 
      + strlen("?lat=") + 8 
      + strlen("&lon=") + 8 
      + strlen("&units=") + strlen(TEMP_UNIT) 
      + strlen("&cnt=") + 3
      + strlen("&appid=") + strlen(OPENWEATHERMAP_APIKEY) + 1;
  char weatherQueryURL[weatherQueryURLSize];
  Watchy_GetLocation::getLocation(); // maybe update location?
  snprintf(weatherQueryURL, weatherQueryURLSize,
           "%s?lat=%.4f&lon=%.4f&units=%s&cnt=%d&appid=%s", OPENWEATHERMAP_FORECAST_URL,
           Watchy_GetLocation::currentLocation.lat,
           Watchy_GetLocation::currentLocation.lon, TEMP_UNIT,
           MAX_FORECASTS,
           OPENWEATHERMAP_APIKEY);
  if (!http.begin(weatherQueryURL)) {
    Watchy::err = Watchy::REQUEST_FAILED;
    log_e("http.begin failed");
  } else {
    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {

      if( parseForecastResponse(http) )
        Watchy::err = Watchy::OK;
      else{
        Watchy::err = Watchy::REQUEST_FAILED;
        log_e("parseForecastResponse failed!");
      }

    } else {
      Watchy::err = Watchy::REQUEST_FAILED;
      log_e("http response %d", httpResponseCode);
    }
    http.end();
  }
  Watchy::releaseWiFi();

}

weatherData getWeather(boolean forceNow) {
  // only update if WEATHER_UPDATE_INTERVAL has elapsed i.e. 30 minutes
  log_d("forceNow: %d, lastGetWeatherTS: %d, now() - lastGetWeatherTS: %d, WEATHER_UPDATE_INTERVAL: %d, check yelds: %d", 
          forceNow, lastGetWeatherTS,now() - lastGetWeatherTS, WEATHER_UPDATE_INTERVAL, (lastGetWeatherTS && (now() - lastGetWeatherTS < WEATHER_UPDATE_INTERVAL)));
  if ((!forceNow) && lastGetWeatherTS &&
      (now() - lastGetWeatherTS < WEATHER_UPDATE_INTERVAL)) {
    // too soon to update, just re-use existing values. Not an error
    log_d("RATE_LIMITED");
    Watchy::err = Watchy::RATE_LIMITED;
    return currentWeather;
  }
  if (!Watchy::getWiFi()) {
    Watchy::err = Watchy::WIFI_FAILED;
    log_e("Wifi connect failed");
    // No WiFi, return RTC Temperature (this isn't actually useful...)
    uint8_t temperature = Watchy::sensor.readTemperature(); //TODO !!!! Watchy::RTC.temperature() / 4;  // celsius
    if (strcmp(TEMP_UNIT, "imperial") == 0) {
      temperature = temperature * 9. / 5. + 32.;  // fahrenheit
    }
    currentWeather.temperature = temperature;
    // we don't know, weatherConditionCode keep the last one
    return currentWeather;
  }

  // WiFi is connected Use Weather API for live data
  HTTPClient http;
  http.setConnectTimeout(10000);  // 10 second max timeout
  const unsigned int weatherQueryURLSize =
      strlen(OPENWEATHERMAP_URL) + strlen("?lat=") + 8 + strlen("&lon=") + 8 +
      strlen("&units=") + strlen(TEMP_UNIT) + strlen("&appid=") +
      strlen(OPENWEATHERMAP_APIKEY) + 1;
  char weatherQueryURL[weatherQueryURLSize];
  Watchy_GetLocation::getLocation(); // maybe update location?
  snprintf(weatherQueryURL, weatherQueryURLSize,
           "%s?lat=%.4f&lon=%.4f&units=%s&appid=%s", OPENWEATHERMAP_URL,
           Watchy_GetLocation::currentLocation.lat,
           Watchy_GetLocation::currentLocation.lon, TEMP_UNIT,
           OPENWEATHERMAP_APIKEY);
  if (!http.begin(weatherQueryURL)) {
    Watchy::err = Watchy::REQUEST_FAILED;
    log_e("http.begin failed");
  } else {
    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {
      String payload = http.getString();
      JSONVar weatherJson = JSON.parse(payload);
//      currentWeather.temperature = int(responseObject["main"]["temp"]);
//      currentWeather.weatherConditionCode =
//          int(responseObject["weather"][0]["id"]);
      parseWeatherData(currentWeather,weatherJson);
      log_d("currenWeather: dt: %d, night: %d, temp %d, code %d, pressure: %d, humidity: %d, wind: %f %d %f, clouds: %d, visibility: %d, pop: %d, rain: %d, snow: %d", 
        currentWeather.dt, currentWeather.night,
        currentWeather.temperature, currentWeather.weatherConditionCode,
        currentWeather.pressure, currentWeather.humidity, 
        currentWeather.wind.speed, currentWeather.wind.direction, currentWeather.wind.gust,
        currentWeather.clouds, currentWeather.visibility, currentWeather.pop, currentWeather.rain, currentWeather.snow
        );
//      strncpy(currentWeather.weatherCity, Watchy_GetLocation::currentLocation.city,
//              sizeof(currentWeather.weatherCity));
      lastGetWeatherTS = now();
      Watchy::err = Watchy::OK;
    } else {
      Watchy::err = Watchy::REQUEST_FAILED;
      log_e("http response %d", httpResponseCode);
    }
    http.end();
  }
  Watchy::releaseWiFi();
  return currentWeather;
}

}  // namespace Watchy_GetWeather

/* weather response JSON prettified
{
    "coord": {
        "lon": 144.968,
        "lat": -37.8008
    },
    "weather": [
        {
            "id": 804,
            "main": "Clouds",
            "description": "overcast clouds",
            "icon": "04n"
        }
    ],
    "base": "stations",
    "main": {
        "temp": 11.76,
        "feels_like": 11.16,
        "temp_min": 9.78,
        "temp_max": 13.05,
        "pressure": 1026,
        "humidity": 83
    },
    "visibility": 10000,
    "wind": {
        "speed": 0.89,
        "deg": 228,
        "gust": 1.79
    },
    "clouds": {
        "all": 90
    },
    "dt": 1628326663,
    "sys": {
        "type": 2,
        "id": 2008797,
        "country": "AU",
        "sunrise": 1628284471,
        "sunset": 1628321843
    },
    "timezone": 36000,
    "id": 2171000,
    "name": "Collingwood",
    "cod": 200
}
*/
