#include "WebServerScreen.h"

#include <Arduino_JSON.h>
#include <WiFi.h>

#include "OptimaLTStd12pt7b.h"
#include "Watchy.h"
#include "WebServer.h"

void WebServerScreen::show() {
  Watchy::RTC.setRefresh(RTC_REFRESH_NONE);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  
  Watchy::display.print("\n");
  if( ! Watchy_WebServer::running ) 
    Watchy::display.print("NOT ");
  Watchy::display.print("running");
  if( Watchy_WebServer::running ) {
    Watchy::display.print("\n@");
    Watchy::display.print(WiFi.localIP().toString().c_str());
    Watchy::display.print("\npress down to stop");
  }else{
    Watchy::display.print("\npress up to start");
  }
  Watchy::display.print("\npress menu to update");
  
}

void WebServerScreen::up() {
  Watchy_WebServer::webServer.begin();
  Watchy::showWatchFace(true);
}
void WebServerScreen::down() {
  Watchy_WebServer::stopServer();
  Watchy::showWatchFace(true);
}

void WebServerScreen::menu() {
  Watchy::showWatchFace(true);
}

void WebServerScreen::back() {
  Screen::back();
}
