#pragma once

#include "Screen.h"

class TetrisScreen : public Screen {
 public:
  TetrisScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
 private:
  void drawNumber(int x, int y, int value, int max_digits);
  double random();
};