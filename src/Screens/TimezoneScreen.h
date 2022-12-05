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
	void drawImage(const unsigned char world_map []);
    void printLocationTimezone(const Watchy_GetLocation::location *loc);
};

