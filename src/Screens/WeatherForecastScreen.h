#pragma once

#include "Screen.h"

class WeatherForecastScreen : public Screen {
 public:
  WeatherForecastScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
};
