#pragma once

#include "Screen.h"

class WebServerScreen : public Screen {
 public:
  WebServerScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
  void menu() override;
  void back() override;
  void up() override;
  void down() override;
};