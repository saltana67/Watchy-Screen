#include "Watchy_BadForEye.h"
#include "BadForEye.h"

const unsigned char *numbers [10] = {numbers0, numbers1, numbers2, numbers3, numbers4, numbers5, numbers6, numbers7, numbers8, numbers9};

using namespace Watchy;

void BadForEye::show(){
    
    tm localTime;
    time_t tNow = now();
    localtime_r(&tNow, &localTime);
    tmElements_t currentTime;
    currentTime.Day = localTime.tm_mday;
    currentTime.Month = localTime.tm_mon + 1;
    currentTime.Year = localTime.tm_year - 70;
    currentTime.Wday = localTime.tm_wday + 1;
    currentTime.Hour = localTime.tm_hour;
    currentTime.Minute = localTime.tm_min;
    currentTime.Second = localTime.tm_sec;

    drawWatchFace(currentTime);
}

void BadForEye::drawWatchFace(tmElements_t currentTime){
    display.fillScreen(GxEPD_BLACK);
    display.drawBitmap(0, 0, window, DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_WHITE);

    //Hour
    display.drawBitmap(50, 10, numbers[currentTime.Hour/10], 39, 80, GxEPD_BLACK); //first digit
    display.drawBitmap(110, 10, numbers[currentTime.Hour%10], 39, 80, GxEPD_BLACK); //second digit
    

    //Minute
    display.drawBitmap(50, 110, numbers[currentTime.Minute/10], 39, 80, GxEPD_BLACK); //first digit
    display.drawBitmap(110, 110, numbers[currentTime.Minute%10], 39, 80, GxEPD_BLACK); //second digit
}
