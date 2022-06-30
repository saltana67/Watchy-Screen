#pragma once

#include "Screen.h"

class AnalogGabel_V2 : public Screen {
 public:
  AnalogGabel_V2(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
  void menu() override;
  static boolean darkMode;
 private:
  void drawWatchFace(tmElements_t currentTime);
};