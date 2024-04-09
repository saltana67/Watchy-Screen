#include "WeatherForecastHandler.h"

#include <Arduino.h>

void WeatherForecastHandler::startDocument() {
    log_d("startDocument");
    state = START;
}

void WeatherForecastHandler::startObject(ElementPath path)  {
    // char fullPath[200] = "";
    // memset(fullPath, 0 , sizeof(fullPath));          
    // path.toString(fullPath);
    // log_d("startObject, fullPath: %s", fullPath);
    switch(state) {
        case START  :   state = TOP_OBJECT; 
                        break;
        case LIST   :   //if( strncmp(path.getKey(),"list", sizeof("list")) == 0 ) {
                            weatherDataParseStart(tmpWeatherData);
                            state = LIST_ELEMENT;
                        //}
                        break;
        case LIST_ELEMENT  :
                        if( strncmp(path.getKey(),"main", sizeof("main")) == 0 ) {
                            state = MAIN;
                        }else if( strncmp(path.getKey(),"clouds", sizeof("clouds")) == 0 ) {
                            state = CLOUDS;
                        }else if( strncmp(path.getKey(),"wind", sizeof("wind")) == 0 ) {
                            state = WIND;
                        }else if( strncmp(path.getKey(),"rain", sizeof("rain")) == 0 ) {
                            state = RAIN;
                        }else if( strncmp(path.getKey(),"sys", sizeof("sys")) == 0 ) {
                            state = SYS;
                        }
                        break;
        case WEATHER_LIST :  
                        //if( strncmp(path.getKey(),"main", sizeof("main")) == 0 ) {
                            state = WEATHER;
                        //}
                        break;

    }

    // const char *ckey = NULL;
    // int cindx = -1;
    // ElementSelector* current = path.getCurrent();
    // if( current != NULL ){
    //     if( current->isObject() ) {
    //         ckey = current->getKey();
    //     }else{
    //         cindx = current->getIndex();
    //     };
    // }
    // log_d("startObject, path: %s, ckey: %s, cindx: %i", fullPath, ckey, cindx);
};

void WeatherForecastHandler::startArray(ElementPath path)  {
    // char fullPath[200] = "";
    // memset(fullPath, 0 , sizeof(fullPath));          
    // path.toString(fullPath);
    // log_d("startArray, fullPath: %s", fullPath);
    // delay(1000);
    // log_d("key: %s", path.getKey());

    switch(state) {
        case TOP_OBJECT: 
                if( strncmp(path.getKey(),"list", sizeof("list")) == 0 ) {
                    state = LIST;
                }
                break;
        case LIST_ELEMENT: 
                if( strncmp(path.getKey(),"weather", sizeof("weather")) == 0 ) {
                    state = WEATHER_LIST;
                }
                break;
    }

    //log_d("startArray, path.key: %s, index: %i", path.getKey(), path.getIndex());
};

void WeatherForecastHandler::value(ElementPath path, ElementValue value)  {
    // char fullPath[200] = "";
    // memset(fullPath, 0 , sizeof(fullPath));          
    // path.toString(fullPath);
    // log_d("value, fullPath: %s", fullPath);
    // delay(1000);
    // char valueString[200] = "";
    // //const char * v =  value.getString();
    // value.toString(valueString);

    // log_d("key: %s : %s ", path.getKey(), valueString );
    // delay(1000);

    switch(state) {
        case TOP_OBJECT: 
                if( strncmp(path.getKey(),"cnt", sizeof("cnt")) == 0 ) {
//                    log_d("cnt: %i", value.getInt());
                }
                break;
        case LIST_ELEMENT: 
                if( strncmp(path.getKey(),"dt", sizeof("dt")) == 0 ) {
//                    log_d("dt: %ld (ul: %ld)", value.getInt(), value.getInt());
                    tmpWeatherData.dt = (time_t) value.getInt();
                }else if( strncmp(path.getKey(),"visibility", sizeof("visibility")) == 0 ) {
//                    log_d("visibility: %i", value.getInt());
                    tmpWeatherData.visibility = value.getInt();
                }else if( strncmp(path.getKey(),"pop", sizeof("pop")) == 0 ) {
//                    log_d("pop: %i", (int) (value.getFloat() * 100.0) );
                    tmpWeatherData.pop = (int) (value.getFloat() * 100.0);
                }
                break;
        case MAIN: 
                if( strncmp(path.getKey(),"temp", sizeof("temp")) == 0 ){ 
//                    log_d("temp: %.2f", value.getFloat());
                    tmpWeatherData.temperature = value.getFloat();
                }else if( strncmp(path.getKey(),"pressure", sizeof("pressure")) == 0 ) {
//                    log_d("pressure: %i", value.getInt());
                    tmpWeatherData.pressure = value.getInt();
                }else if( strncmp(path.getKey(),"humidity", sizeof("humidity")) == 0 ) {
//                    log_d("humidity: %i", value.getInt());
                    tmpWeatherData.humidity = value.getInt();
                }
                break;
        case WEATHER: 
                if( path.getIndex() > 0 ) break;
                if( strncmp(path.getKey(),"id", sizeof("id")) == 0 ){
//                    log_d("id: %i", value.getInt());
                    tmpWeatherData.weatherConditionCode = value.getInt();
                }
                break;
        case CLOUDS: 
                if( strncmp(path.getKey(),"all", sizeof("all")) == 0 ){
//                    log_d("all: %i", value.getInt());
                    tmpWeatherData.clouds = value.getInt();
                }
                break;
        case WIND: 
                if( strncmp(path.getKey(),"speed", sizeof("speed")) == 0 ){
//                    log_d("speed: %.2f", value.getFloat());
                    tmpWeatherData.wind.speed = value.getFloat();
                }else if( strncmp(path.getKey(),"deg", sizeof("deg")) == 0 ){
//                    log_d("deg: %i", value.getInt());
                    tmpWeatherData.wind.direction = value.getInt();
                }else if( strncmp(path.getKey(),"gust", sizeof("gust")) == 0 ) {
//                    log_d("gust: %.2f", value.getFloat());
                    tmpWeatherData.wind.gust = value.getFloat();
                }
                break;
        case RAIN: 
                if( strncmp(path.getKey(),"3h", sizeof("3h")) == 0 ) {
//                    log_d("3h: %i", value.getInt());
                    tmpWeatherData.rain = value.getInt();
                }else if( strncmp(path.getKey(),"1h", sizeof("1h")) == 0 ) {
//                    log_d("1h: %i", value.getInt());
                    tmpWeatherData.rain = value.getInt();
                }
                break;
        case SYS: 
                if( strncmp(path.getKey(),"pod", sizeof("pod")) == 0 ){
                    bool night = (value.getString()[0] == 'n');
//                    log_d("pod: %s", night?"night":"day");
                    tmpWeatherData.night = night;
                }
                break;
    }

    //log_d("value, path.key: %s, index: %i -> value: %s", path.getKey(), path.getIndex(), value.getString());
};

void WeatherForecastHandler::endArray(ElementPath path)  {
    // char fullPath[200] = "";
    // memset(fullPath, 0 , sizeof(fullPath));          
    // path.toString(fullPath);
    //log_d("endArray, fullPath: %s", fullPath);
    switch(state) {
        case LIST: 
                if( strncmp(path.getKey(),"list", sizeof("list")) == 0 ) {
                    state = TOP_OBJECT;
                }
                break;
        case WEATHER_LIST: 
                if( strncmp(path.getKey(),"weather", sizeof("weather")) == 0 ) {
                    state = LIST_ELEMENT;
                }
                break;
    }

   // log_d("startArray, path.key: %s, index: %i", path.getKey(), path.getIndex());
};

void WeatherForecastHandler::endObject(ElementPath path)  {
    // char fullPath[200] = "";
    // memset(fullPath, 0 , sizeof(fullPath));          
    // path.toString(fullPath);
    // log_d("endObject, fullPath: %s", fullPath);
    switch(state) {
        case LIST_ELEMENT   :   weatherDataParsed(tmpWeatherData); 
                                state = LIST            ;   break;
        case MAIN           :   state = LIST_ELEMENT    ;   break;
        case CLOUDS         :   state = LIST_ELEMENT    ;   break;
        case WIND           :   state = LIST_ELEMENT    ;   break;
        case RAIN           :   state = LIST_ELEMENT    ;   break;
        case SYS            :   state = LIST_ELEMENT    ;   break;
        case WEATHER        :   state = WEATHER_LIST    ;   break;
    }
    //log_d("endObject, path.key: %s, index: %i", path.getKey(), path.getIndex());
};

void WeatherForecastHandler::endDocument() {
   log_d("endDocument");
}

void WeatherForecastHandler::weatherDataParseStart(Watchy_GetWeather::weatherData &weatherData) {
        weatherData.dt = 0;
        weatherData.night = false;
        weatherData.temperature = 0.0;
        weatherData.weatherConditionCode = 0;
        weatherData.pressure = 0;
        weatherData.humidity = 0;
        weatherData.wind.speed = 0;
        weatherData.wind.direction = 0, 
        weatherData.wind.gust = 0,
        weatherData.clouds = 0, 
        weatherData.visibility = 0, 
        weatherData.pop = 0, 
        weatherData.rain = 0L, 
        weatherData.snow = 0L;
}
void WeatherForecastHandler::weatherDataParsed(Watchy_GetWeather::weatherData &weatherData) {
    if( weatherData.rain > 0)
        weatherData.pop = 100;
    // log_d("forecast %d: dt: %d, night: %d, temp %d, code %d, pressure: %d, humidity: %d, wind: %f %d %f, clouds: %d, visibility: %d, pop: %d, rain: %d, snow: %d", 
    //     0, 
    //     weatherData.dt, weatherData.night,
    //     weatherData.temperature, weatherData.weatherConditionCode,
    //     weatherData.pressure, weatherData.humidity, 
    //     weatherData.wind.speed, weatherData.wind.direction, weatherData.wind.gust,
    //     weatherData.clouds, weatherData.visibility, weatherData.pop, weatherData.rain, weatherData.snow
    //     );
    if( onWeatherDataParsed != NULL )
        onWeatherDataParsed(weatherData);
}

void WeatherForecastHandler::setOnWeatherDataParsed(void (*f)(Watchy_GetWeather::weatherData &weatherData)){
    WeatherForecastHandler::onWeatherDataParsed = f;
}