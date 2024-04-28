#include "AirplaneModeScreen.h"

#include "OptimaLTStd12pt7b.h"
#include "Watchy.h"


void AirplaneModeScreen::show() {
  Watchy::RTC.setRefresh(RTC_REFRESH_NONE);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  
  Watchy::display.print("\nAirplane mode is\n");
  if( Watchy::AIRPLANE_MODE ) {
    Watchy::display.print("   ON");
    Watchy::display.print("\npress down to stop");
  }else{
    Watchy::display.print("   OFF");
    Watchy::display.print("\npress up to start");
  }
  Watchy::display.print("\npress menu to update");  
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
