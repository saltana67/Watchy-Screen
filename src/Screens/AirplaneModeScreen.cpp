#include "AirplaneModeScreen.h"

#include "OptimaLTStd12pt7b.h"
#include "Watchy.h"

void AirplaneModeScreen::drawVerticalButtonsBar(boolean state){

  int16_t xOn, yOn, xOff, yOff;
  uint16_t wOn, wOff, hOn, hOff, w, h;
  Watchy::display.getTextBounds("ON"  ,100,50, &xOn, &yOn, &wOn, &hOn);
  log_d("TextBounds(ON) : x: %i, y:%i, w: %i, h: %i", xOn, yOn, wOn, hOn);
  Watchy::display.getTextBounds("OFF" ,100,190, &xOff, &yOff, &wOff, &hOff);
  log_d("TextBounds(OFF): x: %i, y:%i, w: %i, h: %i", xOff, yOff, wOff, hOff);

  w = wOff > wOn ? wOff : wOn;
  h = hOff > hOn ? hOff : hOn;

  xOn  = 199 - wOn  - (w-wOn/2 ) + (xOn  - 100);
  xOff = 199 - wOff - (w-wOff/2) + (xOff - 100);

  yOn = h - hOn;
  yOff = 199 - h + (h - hOff);


  Watchy::display.setCursor(xOn, yOn);
  Watchy::display.print("ON");

  Watchy::display.setCursor(xOff, yOff);
  Watchy::display.print("OFF");


}

void AirplaneModeScreen::show() {
  Watchy::RTC.setRefresh(RTC_REFRESH_NONE);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);

  Watchy::display.setTextSize(2);
  Watchy::display.setCursor(0, 50);
  Watchy::display.print("Airplane\n");
  Watchy::display.setTextSize(1);
  Watchy::display.print("mode is");
  Watchy::display.setTextSize(2);
  if( Watchy::AIRPLANE_MODE ) {
    Watchy::display.print("\n   ON");
  }else{
    Watchy::display.print("\n   OFF");
  }
  Watchy::display.setTextSize(1);

  Watchy::display.setCursor(199-26+((26-23)/2), 17/*+(17-11)*/);
  Watchy::display.print("ON");

  Watchy::display.setCursor(199-26+((26-26)/2), 199/*-(17+(17-17))*/);
  Watchy::display.print("OFF");

  Watchy::display.drawRoundRect(    199-26,17+3,                      26,200-17-17-6,    13,bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE);
  if( Watchy::AIRPLANE_MODE ) {
    Watchy::display.fillRoundRect(199-26+3,17+3+3,                    20,(200-17-17-6)/2,10,bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE);
  }else{
    Watchy::display.drawRoundRect(199-26+3,17+3+3+((200-17-17-6)/2)-6,20,(200-17-17-6)/2,10,bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE);
  }

  Watchy::display.setCursor(0, 199/*-(17+(17-17))*/);
  Watchy::display.print("X");  
}

void AirplaneModeScreen::up() {
  if( ! Watchy::AIRPLANE_MODE ) 
    Watchy::AIRPLANE_MODE = true;
  Watchy::showWatchFace(true);
}
void AirplaneModeScreen::down() {
  if( Watchy::AIRPLANE_MODE ) 
    Watchy::AIRPLANE_MODE = false;
  Watchy::showWatchFace(true);
}

void AirplaneModeScreen::menu() {
  Watchy::showWatchFace(true);
}

void AirplaneModeScreen::back() {
  Screen::back();
}
