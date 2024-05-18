#include "QRCodeScreen.h"

#include <QRCodeGxEPD2_BW.h>

using namespace Watchy;

RTC_DATA_ATTR boolean QRCodeScreen::darkMode = false;

void QRCodeScreen::show(){
  drawQRCode(getQRString());
}

void QRCodeScreen::drawQRCode(String content){
//  QRCodeGxEPD2_BW<GxEPD2_154_D67,GxEPD2_154_D67::HEIGHT> qrcode (&display);
  QRCodeGxEPD2_BW<WatchyDisplay,WatchyDisplay::HEIGHT> qrcode (&display);
  qrcode.init();
  qrcode.create(content);
}

void QRCodeScreen::menu() {
    log_d("darkMode: %d", darkMode);
    darkMode = darkMode ? false : true;
    log_d("darkMode toggled: %d", darkMode);
    Watchy::showWatchFace(true);
}
