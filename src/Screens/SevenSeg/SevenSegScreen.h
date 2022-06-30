#pragma once

#include "Screen.h"

class SevenSegScreen : public Screen {
 public:
  SevenSegScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;

 private:
  void drawBattery();
  void drawTime(tm &currentTime);
  void drawDate(tm &currentTime);
  void drawSteps();
  void drawWeather();
};
