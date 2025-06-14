#include "SevenSegScreen.h"

#include "DSEG7_Classic_Bold_53.h"
#include "Seven_Segment10pt7b.h"
#include "DSEG7_Classic_Bold_25.h"
#include "DSEG7_Classic_Regular_39.h"

#include "icons7Seg.bold.h"

#include "GetWeather.h"

using namespace Watchy;

#define DARKMODE false //true
RTC_DATA_ATTR boolean SevenSegScreen::darkMode = DARKMODE;

const uint8_t BATTERY_SEGMENT_COUNT = 3;
const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = BATTERY_SEGMENT_WIDTH + 2;
const uint8_t BATTERY_SEGMENTS_WIDTH = (BATTERY_SEGMENT_COUNT * BATTERY_SEGMENT_WIDTH);
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;
const uint8_t HOUR_12_24 = 24;

void SevenSegScreen::show() {
    log_d("show called");
    Watchy::RTC.setRefresh(RTC_REFRESH_MIN);

    uint16_t fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
    uint16_t bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;

    display.fillScreen(bgColor);
    display.setTextColor(fgColor);

    log_d("darkMode: %d", darkMode);
    if( Watchy::display.epd2.darkBorder != darkMode )
        Watchy::display.epd2.setDarkBorder(darkMode);

    tm localTime;
    time_t tNow = now();
    localtime_r(&tNow, &localTime);

    drawTime(localTime);
    drawDate(localTime);
    drawSteps();
    drawWeather();
    drawBattery();
    if( Watchy::AIRPLANE_MODE ) {
        //place for ble and wifi icons starts at 100 and reaches to 120+18=138 
        //width is 38
        //middle point is 100+(38/2)=119
        //start for airplane mode on is at 119-(21/2)=109
        display.drawBitmap(109, 75, airplane_mode_on21x21, 21, 21, fgColor );
    }else{
        display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, fgColor);
        if(BLE_CONFIGURED){
            display.drawBitmap(100, 75, bluetooth13x21, 13, 21, fgColor);
        }
    }
}

void SevenSegScreen::drawTime(tm &t){
   
    display.setFont(&DSEG7_Classic_Bold_53);
    display.setCursor(5, 53+5);
    int displayHour;
    if(HOUR_12_24==12){
        displayHour = ((t.tm_hour+11)%12)+1;
    } else {
        displayHour = t.tm_hour;
    }
    if(displayHour < 10){
        display.print("0");
    }
    display.print(displayHour);
    display.print(":");
    if(t.tm_min < 10){
        display.print("0");
    }
    display.println(t.tm_min);
}

void SevenSegScreen::drawDate(tm &currentTime){

    display.setFont(&Seven_Segment10pt7b);

    int16_t  x1, y1;
    uint16_t w, h;

    String dayOfWeek = dayStr(currentTime.tm_wday+1);
    display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
    if((currentTime.tm_wday+1) == dowWednesday){
        w = w - 5;
    }
    display.setCursor(85 - w, 85);
    display.println(dayOfWeek);

    String month = monthShortStr(currentTime.tm_mon+1);
    display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
    display.setCursor(85 - w, 110);
    display.println(month);

    display.setFont(&DSEG7_Classic_Bold_25);
    display.setCursor(5, 120);
    if(currentTime.tm_mday < 10){
        display.print("0");
    }
    display.println(currentTime.tm_mday);
    display.setCursor(5, 150);
    display.println(currentTime.tm_year+1900);// offset from 1900
}

void SevenSegScreen::drawSteps(){
    uint16_t fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
    uint16_t bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;

    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    uint32_t stepCount = sensor.getCounter();
    display.drawBitmap(10, 165, steps19x23, 19, 23, fgColor);
    display.setCursor(35, 190);
    display.println(stepCount);
}

void SevenSegScreen::drawBattery(){
    uint16_t fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
    uint16_t bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;
    display.drawBitmap(154, 73, battery37x21, 37, 21, fgColor);
    display.fillRect(159, 78, 27, BATTERY_SEGMENT_HEIGHT, bgColor);//clear battery segments
    int8_t batteryLevel = 0;
    float VBAT = getBatteryVoltage();
/* 
    if(VBAT > 4.1){
        batteryLevel = 3;
    }
    else if(VBAT > 3.95 && VBAT <= 4.1){
        batteryLevel = 2;
    }
    else if(VBAT > 3.80 && VBAT <= 3.95){
        batteryLevel = 1;
    }
    else if(VBAT <= 3.80){
        batteryLevel = 0;
    }
 */    
    float batStat = getBatteryState(VBAT);
/*
    if(batStat > 0.7f){
        batteryLevel = 3;
    }
    else if(batStat > 0.4f && batStat <= 0.7f){
        batteryLevel = 2;
    }
    else if(batStat > 0.1f && batStat <= 0.4f){
        batteryLevel = 1;
    }
    else if(batStat <= 0.1f){
        batteryLevel = 0;
    }

    for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++){
        display.fillRect(159 + (batterySegments * BATTERY_SEGMENT_SPACING), 78, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
*/
    float zeroThreshold = 0.1f;

    for( int8_t batterySegment = 0, fillWidth = (batStat * BATTERY_SEGMENTS_WIDTH) + 0.5f; 
        (batterySegment < BATTERY_SEGMENT_COUNT) && (fillWidth > 0); 
        batterySegment++, fillWidth -= BATTERY_SEGMENT_WIDTH){
        int8_t segmentWidth = ((fillWidth >= BATTERY_SEGMENT_WIDTH) ? BATTERY_SEGMENT_WIDTH : fillWidth );
        if( segmentWidth < 1 )
            break;
        log_d("batterySegment: %d, fillWidth: %d, segmentWidth: %d", batterySegment, fillWidth, segmentWidth);
        display.fillRect(159 + (batterySegment * BATTERY_SEGMENT_SPACING), 78, segmentWidth, BATTERY_SEGMENT_HEIGHT, fgColor);
    }
}

void SevenSegScreen::drawWeather(){

    uint16_t fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
    uint16_t bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;

    Watchy_GetWeather::weatherData currentWeather = Watchy_GetWeather::getWeather();

    int8_t temperature = currentWeather.temperature;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;

    display.setFont(&DSEG7_Classic_Regular_39);
    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
    if(159 - w - x1 > 87){
        display.setCursor(159 - w - x1, 150);
    }else{
        display.setFont(&DSEG7_Classic_Bold_25);
        display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
        display.setCursor(159 - w - x1, 136);
    }
    display.println(temperature);

    boolean isMetric = true;
    if (strcmp(Watchy_GetWeather::TEMP_UNIT, "imperial") == 0 ) 
        isMetric = false;
    
    display.drawBitmap(165, 110, isMetric ? celsius : fahrenheit, 26, 20, fgColor);
    const unsigned char* weatherIcon;

    //https://openweathermap.org/weather-conditions
    if(weatherConditionCode > 801){//Cloudy
    weatherIcon = cloudy;
    }else if(weatherConditionCode == 801){//Few Clouds
    weatherIcon = cloudsun;
    }else if(weatherConditionCode == 800){//Clear
    weatherIcon = sunny;
    }else if(weatherConditionCode >=700){//Atmosphere
    weatherIcon = atmosphere;
    }else if(weatherConditionCode >=600){//Snow
    weatherIcon = snow;
    }else if(weatherConditionCode >=500){//Rain
    weatherIcon = rain;
    }else if(weatherConditionCode >=300){//Drizzle
    weatherIcon = drizzle;
    }else if(weatherConditionCode >=200){//Thunderstorm
    weatherIcon = thunderstorm;
    }else
    return;
    display.drawBitmap(145, 158, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, fgColor);
}

/*
void TestScreen::up() {
    log_d("up called");
}

void TestScreen::down() {
    log_d("down called");
}

void TestScreen::back() {
    log_d("back called, calling super");
    Screen::back();
}
*/
void SevenSegScreen::menu() {
    log_d("darkMode: %d", darkMode);
    darkMode = darkMode ? false : true;
    log_d("darkMode toggled: %d", darkMode);

    Watchy::showWatchFace(true);
}
