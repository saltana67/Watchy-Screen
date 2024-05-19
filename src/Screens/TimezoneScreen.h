#pragma once

#include "Screen.h"
#include "GetLocation.h"

class TimezoneScreen : public Screen {
 public:
  TimezoneScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
  void menu() override;
  static boolean darkMode;
 private:
    void printLocationTimezone(const Watchy_GetLocation::location *loc);
	void drawTimezoneMap(const unsigned char world_map [], const Watchy_GetLocation::location *loc);
    void drawLocation(const Watchy_GetLocation::location *loc, bool lines=true);
    void drawLocation(const float lat, const float lon, bool lines=true);
};

