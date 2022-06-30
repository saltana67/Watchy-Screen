#pragma once

#include "Screen.h"


class TypoStyle : public Screen {
 public:
  TypoStyle(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
  void menu() override;
  static boolean darkMode;
 private:

 		void drawPixel(int16_t x, int16_t y,uint16_t col);
		void drawBitmapCol(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color1, uint16_t color2);
		void drawMyRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1);
		void drawWatchFace(tmElements_t currentTime);
};

