#include "CalendarScreen.h"

#include "OptimaLTStd12pt7b.h"

#define GREY 0x7BEF
using namespace Watchy;

RTC_DATA_ATTR boolean CalendarScreen::darkMode = false;

void CalendarScreen::show(){
  log_d("darkMode: %d", darkMode);
  if( Watchy::display.epd2.darkBorder != darkMode )
    Watchy::display.epd2.setDarkBorder(darkMode);

  drawWatchFace();
}

void CalendarScreen::menu() {
    log_d("darkMode: %d", darkMode);
    darkMode = darkMode ? false : true;
    log_d("darkMode toggled: %d", darkMode);

    Watchy::showWatchFace(true);
}
void CalendarScreen::up() {
    Watchy::showWatchFace(true);
}
void CalendarScreen::down() {
    Watchy::showWatchFace(true);
}

void CalendarScreen::drawWatchFace(){
    int fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
    int bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;

    Watchy::display.fillScreen(bgColor);
    //Watchy::display.setFont(OptimaLTStd12pt7b);

    /*
    display width/weekdays: 200/7=28.xxx
    day max width * weekdays: 28*7=196
    day separation line width * (weekdays-1): 1*(7-1)=6
    196+6=202 -> 28 too wide: make it 27
    day max width * weekdays: 27*7=189
    total width: 189+6=195 -> 5 pixel extra space ... 
    height: 27*weeks + day separation line width * (weeks-1): 27*6 + 1*(6-1)=162+5=167 -> 38 pixel left for weekday
    -> only 11 pixels left for month ...
    
    */
    int lineWidth=1;

    /* calculate cell width */
    bool showWeekNumber = true;
    int nCellsH=7+(showWeekNumber?1:0); //7
    int w = 200;
    w -= (nCellsH-1)*lineWidth; //194
    int cellW=w/nCellsH; //27
    int rH = 200-((nCellsH*cellW)-((nCellsH-1)*lineWidth)); //200-189-6=5

    /* calculate cell height */
    int h = 200-30; //180
    int nCellsV=6+1;//7: 6 for weeks and 1 with column headers for weekday name
    h -= (nCellsV-1)*lineWidth+1; //173
    int cellH=h/nCellsV; //24

    int dY = cellH+lineWidth;
    int dX = cellW+lineWidth;
    //hlines
    int y = 200;
    for( int n=0; n<nCellsV-1; n++){
      y -= dY;
      display.drawFastHLine(0,y,200,fgColor);  
    }
    y -= dY;
    int x = 0;
    for( int n=0; n<nCellsH-1; n++){
      x += dX;
      display.drawFastVLine(x,y,200-y,fgColor);  
    }

    if( rH > 0 ){
      x++;
      display.drawFastVLine(x,y,200-y,fgColor);
    }

    tm localTime;
    time_t tNow = now();
    localtime_r(&tNow, &localTime);
    
    //ErrSunMonTueWedThuFriSat
    //0  1  2  3  4  5  6  7
    //   1  2  3  4  5  6  0
    //   0  1  2  3  4  5  6
    String dayOfWeekShort = dayShortStr(localTime.tm_wday==0?7:localTime.tm_wday);
    String dayOfWeekShort2 = dayOfWeekShort.substring(0,dayOfWeekShort.length() - 1);
    String dayOfWeek = dayStr(localTime.tm_wday+1);

    display.setFont(OptimaLTStd12pt7b);
    x=dX, y=50;
    int wdayI = localTime.tm_wday==0?7:localTime.tm_wday ;

    for(int i = 0; i < 7; i++){
      dayOfWeekShort = dayShortStr(i+1);
      dayOfWeekShort2 = dayOfWeekShort.substring(0,dayOfWeekShort.length() - 1);
      log_d("i: %d, x: %d, y: %d, dayOfWeekShort: %s, dayOfWeekShort2: %s", i, x,y, dayOfWeekShort.c_str(), dayOfWeekShort2.c_str());
      display.setCursor(x,y);
      display.print(dayOfWeekShort2);
      x += dX;
    }

    int wDay = localTime.tm_wday;
    int wDayI = (wDay==0?7:wDay)-1;
    
    log_d("localTime.tm_wday: %d, localTime.tm_mday: %d, dayOfWeekShort2: %s, dayOfWeek: %s", localTime.tm_wday, localTime.tm_mday, dayOfWeekShort2.c_str(), dayOfWeek.c_str());
    
}

void CalendarScreen::drawPixel(int16_t x, int16_t y,uint16_t col){
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

void CalendarScreen::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, int16_t xoffset, int16_t xsize, uint16_t color1, uint16_t color2){
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

void CalendarScreen::drawBitmapCol(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color1, uint16_t color2){
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

