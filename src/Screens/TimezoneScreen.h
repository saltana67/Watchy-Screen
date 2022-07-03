#pragma once

#include "Screen.h"


class TimezoneScreen : public Screen {
 public:
  TimezoneScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
  void menu() override;
  static boolean darkMode;
 private:
	void drawImage(const unsigned char world_map []);
};

