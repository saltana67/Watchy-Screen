#pragma once

#include "Screen.h"

class TestScreen : public Screen {
 public:
  TestScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
  void up() override; 
  void down() override;
  void back() override;
  void menu() override;
};