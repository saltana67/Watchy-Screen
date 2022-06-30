#include "Maze4Watchy.h"
#include "imgs.h"

using namespace Watchy;

RTC_DATA_ATTR boolean Maze4Watchy::darkMode = false;

void Maze4Watchy::show(){
  log_d("darkMode: %d", darkMode);
  tmElements_t currentTime;
  getLocalTime(currentTime);

  drawWatchFace(currentTime);
}


void Maze4Watchy::menu() {
    log_d("darkMode: %d", darkMode);
    darkMode = darkMode ? false : true;
    log_d("darkMode toggled: %d", darkMode);
    Watchy::showWatchFace(true);
}

void Maze4Watchy::drawWatchFace(tmElements_t currentTime) {
  const unsigned char *images [10] = {img0,img1,img2,img3,img4,img5,img6,img7,img8,img9};

  int fgColor = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
  int bgColor = darkMode ? GxEPD_BLACK : GxEPD_WHITE;
        
  //drawbg
  display.fillScreen(fgColor);
  display.drawBitmap(0, 0, background, 200, 200, bgColor);
  //draw time
	display.fillRect(29,29,66,66, fgColor);
	display.fillRect(29,107,66,66, fgColor);	
	display.fillRect(107,29,66,66, fgColor);	
	display.fillRect(107,107,66,66, fgColor);	
							
	display.drawBitmap(29, 29, images[currentTime.Hour/10], 66, 66, bgColor);
  display.drawBitmap(107, 29, images[currentTime.Hour%10], 66, 66, bgColor);
						
	display.drawBitmap(29, 107, images[currentTime.Minute/10], 66, 66, bgColor);
  display.drawBitmap(107, 107, images[currentTime.Minute%10], 66, 66, bgColor);
 
  // draw battery
  float batt = (getBatteryVoltage()-3.3)/0.9;
  if (batt > 0) {
   display.fillRect(0,185,2,12*batt,fgColor);
  }                 
}


