#pragma once

#include "Screen.h"


class Maze4Watchy : public Screen {
 public:
  Maze4Watchy(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
  void menu() override;
  static boolean darkMode;
 private:
  void drawWatchFace(tmElements_t currentTime);
};

