#pragma once

#include "JsonHandler.h"

#include "GetWeather.h"

class WeatherForecastHandler: public JsonHandler 
{
    public:

        void endArray(ElementPath path);

        void endDocument();

        void endObject(ElementPath path);

        void startArray(ElementPath path);
    
        void startDocument();

        void startObject(ElementPath path);

        void value(ElementPath path, ElementValue value);
    
        void whitespace(char c) {};

        void setOnWeatherDataParsed(void (*f)(Watchy_GetWeather::weatherData &weatherData));

    protected:

        enum ParserState {
                START=0
            ,   TOP_OBJECT
            ,   LIST
            ,   LIST_ELEMENT
            ,   MAIN
            ,   WEATHER_LIST
            ,   WEATHER
            ,   CLOUDS
            ,   WIND
            ,   RAIN
            ,   SYS
        };

        ParserState state = START;

        Watchy_GetWeather::weatherData tmpWeatherData;

        void weatherDataParseStart(Watchy_GetWeather::weatherData &weatherData);
        void weatherDataParsed(Watchy_GetWeather::weatherData &weatherData);

        void (*onWeatherDataParsed)(Watchy_GetWeather::weatherData &weatherData) {};
};