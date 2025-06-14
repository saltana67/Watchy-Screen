#pragma once

#include "Screen.h"

class CalendarScreen : public Screen {
 public:
  CalendarScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
  void menu() override;
  void up() override;
  void down() override;
  static boolean darkMode;
  static int8_t  modes[];
  static uint8_t modeIndex;
  static uint8_t mode;
  static int8_t  phase;
 
 private:
 	void drawPixel(int16_t x, int16_t y,uint16_t col);
	void drawBitmap(int16_t x, int16_t y, 
                    const uint8_t *bitmap, int16_t w, int16_t h, 
                    int16_t xoffset, 
                    int16_t xsize, 
                    uint16_t color1, uint16_t color2);
	void drawBitmapCol(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color1, uint16_t color2);
  void drawWatchFace();
};
