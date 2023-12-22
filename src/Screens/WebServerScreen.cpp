#include "WebServerScreen.h"

#include <Arduino_JSON.h>

#include "OptimaLTStd12pt7b.h"
#include "Watchy.h"
#include "WebServer.h"


void WebServerScreen::show() {
  Watchy::RTC.setRefresh(RTC_REFRESH_NONE);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  
  if( ! Watchy_WebServer::running ) 
    Watchy::display.print("\nNOT ");
  Watchy::display.print("running");
  Watchy::display.print("\npress up/down to start");
  Watchy::display.print("\npress menu to update");
  
}

void WebServerScreen::up() {
  Watchy_WebServer::webServer.begin();
}
void WebServerScreen::down() {
  Watchy_WebServer::stopServer();
}

void WebServerScreen::menu() {
  WebServerScreen::show();
}

void WebServerScreen::back() {
  Screen::back();
}
