#include "MoonScreen.h"

#include "Moon.h"

#define GREY 0x7BEF
using namespace Watchy;

RTC_DATA_ATTR boolean MoonScreen::darkMode = true;
RTC_DATA_ATTR int8_t  MoonScreen::phase    = 0;
RTC_DATA_ATTR uint8_t MoonScreen::modeIndex= 4;
RTC_DATA_ATTR uint8_t MoonScreen::mode     = 0b0101;
#define MODE_COUNT 8
/*
    background  | night dark | night light | day dark | day light 
                    grey         grey          grey       white         0b1111
                    black        grey          grey       white         0b0111
                    black        black         grey       white         0b0011
                    grey         grey          black      white         0b1101
                    black        grey          black      white         0b0101
                    black        black         black      white         0b0001
                    grey         grey          black      grey          0b1100
                    black        black         black      grey          0b0000
*/
int8_t   MoonScreen::modes[] = {0b00001111, 0b00000111, 0b00000011, 0b00001101, 0b00000101, 0b00000001, 0b00001100, 0b00000000};

void MoonScreen::show(){
  log_d("darkMode: %d", darkMode);

  drawWatchFace();
}

void MoonScreen::menu() {
    // log_d("darkMode: %d", darkMode);
    // darkMode = darkMode ? false : true;
    // log_d("darkMode toggled: %d", darkMode);

    modeIndex = (++modeIndex) % MODE_COUNT;
    mode = modes[modeIndex];

    Watchy::showWatchFace(true);
}
void MoonScreen::up() {
    phase = 1 - phase;
    Watchy::showWatchFace(true);
}
void MoonScreen::down() {
    phase = 1 - phase;
    Watchy::showWatchFace(true);
}

void MoonScreen::drawWatchFace(){
    int fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
    int bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;

    int dayLight;
    int dayDark;
    int nightLight;
    int nightDark;
    
    if( mode & 0b00000001 )
        dayLight = GxEPD_WHITE;
    else    
        dayLight = GREY;

    if( mode & 0b00000010 )
        dayDark = GREY;
    else    
        dayDark = GxEPD_BLACK;

    if( mode & 0b00000100 )
        nightLight = GREY;
    else    
        nightLight = GxEPD_BLACK;

    if( mode & 0b00001000 )
        nightDark = GREY;
    else    
        nightDark = GxEPD_BLACK;


    Watchy::display.fillScreen(bgColor);
    //Watchy::display.setFont(OptimaLTStd12pt7b);

    drawBitmap(0, 0, &moon_img_bits[0], 200, 200, 0, 200, dayDark, dayLight );

    if( phase != 0 )
        drawBitmap(0, 0, &moon_img_bits[0], 200, 200, 0, 100, nightDark, nightLight );
}

void MoonScreen::drawPixel(int16_t x, int16_t y,uint16_t col){
  switch (col){
    case GREY:
      if(y&1){
        if(x&1){
          display.drawPixel(x, y, GxEPD_BLACK);
        }else{
          display.drawPixel(x, y, GxEPD_WHITE);
        }
      }else{
        if(x&1){
          display.drawPixel(x, y, GxEPD_WHITE);
        }else{
          display.drawPixel(x, y, GxEPD_BLACK);
        }
      }
      break;
    default:
      display.drawPixel(x, y, col);
      break;
  }
}

void MoonScreen::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, int16_t xoffset, int16_t xsize, uint16_t color1, uint16_t color2){
  int16_t i, j, byteWidth = (w + 7) / 8;
  int16_t bit_index;

  for(j=0; j<h; j++) {
    for(i=xoffset; (i<w) && (i<xoffset+xsize) ; i++ ) {
      if((pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7)))) {
        drawPixel(x+i, y+j, color1);
      }else{
        drawPixel(x+i, y+j, color2);
      }
    }
  }
}

void MoonScreen::drawBitmapCol(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color1, uint16_t color2){
  int16_t i, j, byteWidth = (w + 7) / 8;
  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if((pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7)))) {
        drawPixel(x+i, y+j, color1);
      }else{
        drawPixel(x+i, y+j, color2);
      }
    }
  }
}

