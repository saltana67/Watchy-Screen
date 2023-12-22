#pragma once

#include "Screen.h"

class RestartScreen : public Screen {
 public:
  RestartScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
  void menu() override;
  void back() override;
};