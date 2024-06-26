#include "AnalogGabel_V2.h"
#include "clockFace_square2.h"
#include <Fonts/FreeSerifBoldItalic9pt7b.h>
#include <Fonts/FreeSerifItalic9pt7b.h>
#include <Fonts/FreeSerifBold12pt7b.h>
#include "FreeSerifBoldItalic4pt7b.h"

#include <Fonts/FreeMonoOblique24pt7b.h> //include any fonts you want to use

RTC_DATA_ATTR boolean AnalogGabel_V2::darkMode = false;


using namespace Watchy;

void AnalogGabel_V2::show(){
  log_d("darkMode: %d", darkMode);
  tmElements_t currentTime;
  getLocalTime(currentTime);

  drawWatchFace(currentTime);
}


void AnalogGabel_V2::menu() {
    log_d("darkMode: %d", darkMode);
    darkMode = darkMode ? false : true;
    log_d("darkMode toggled: %d", darkMode);
    Watchy::showWatchFace(true);
}

void AnalogGabel_V2::drawWatchFace(tmElements_t currentTime){ 
  
  int fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
  int bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;

  Watchy::display.fillScreen(bgColor);
  //background
  display.drawBitmap(0, 0, clockFace_square2, 200, 200, fgColor);
  display.fillCircle(100,100,3,fgColor);

  //Logo
  display.setCursor(80, 40);
  display.setFont(&FreeSerifBoldItalic9pt7b);
  display.setTextColor(fgColor);
  display.print("Gabel ");
  display.setCursor(54, 60);
  display.setFont(&FreeSerifItalic9pt7b);
  display.print("Chronometrie");

  //date
  String dateDay = "";
  if(currentTime.Day < 10){
    dateDay += "0";
  }
  dateDay += currentTime.Day;
  display.fillRect(128, 88, 27, 24, fgColor);
  display.setFont(&FreeSerifBold12pt7b);
  display.setTextColor(bgColor);
  display.setCursor(129, 107);
  display.print(dateDay);
  //weekday
  String wDay = dayShortStr(currentTime.Wday);
  wDay = wDay.substring(0,wDay.length() - 1);
  display.fillRect(86, 140, 29, 22, fgColor);
  display.setFont(&FreeSerifBold12pt7b);
  display.setTextColor(bgColor);
  display.setCursor(87, 158);
  display.print(wDay);
  // draw battery
  display.drawCircleHelper(45, 100, 20, 2, fgColor);
  display.drawCircleHelper(45, 100, 20, 4, fgColor);
  display.drawPixel(65, 100, fgColor);
  display.drawFastVLine(45, 79, 4, fgColor);
  display.drawFastHLine(63, 100, 4, fgColor);
  display.drawFastVLine(45, 118, 4, fgColor);
  display.setFont(&FreeSerifBoldItalic4pt7b);
  display.setTextColor(fgColor);
  display.setCursor(44, 76);
  display.print("1");
  display.setCursor(70, 97);
  display.print("1");
  display.setCursor(69, 101);
  display.print("--");
  display.drawPixel(71, 100, fgColor);
  display.setCursor(69, 107);
  display.print("2");
  display.setCursor(43, 127);
  display.print("0");
  display.fillCircle(45, 100, 2, fgColor);
  double batteryCurrent = (getBatteryVoltage() - 3.3) / 0.9;
  double batteryAngle = batteryCurrent * 180;
  double radBattery = ((batteryAngle) * 71) / 4068.0;
  double bx1 = 45 + (sin(radBattery) * 16);
  double by1 = 100 + (cos(radBattery) * 16);
  display.drawLine(45, 100, (int)bx1, (int)by1, fgColor);
  //minute pointer
  int currentMinute = currentTime.Minute;
  int minuteAngle = currentMinute * 6;
  double radMinute = ((minuteAngle + 180) * 71) / 4068.0;
  double mx1 = 100 - (sin(radMinute) * 85);
  double my1 = 100 + (cos(radMinute) * 85);
  display.drawLine(100, 100, (int)mx1, (int)my1, fgColor);
  //hour pointer
  int currentHour= currentTime.Hour;
  double hourAngle = (currentHour * 30) + currentMinute * 0.5;
  double radHour = ((hourAngle + 180) * 71) / 4068.0;
  double hx1 = 100 - (sin(radHour) * 45);
  double hy1 = 100 + (cos(radHour) * 45);
  display.drawLine(100, 100, (int)hx1, (int)hy1, fgColor);

}