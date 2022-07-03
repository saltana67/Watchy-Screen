#ifndef QRCODEGXEPD2BW_HPP
#define QRCODEGXEPD2BW_HPP

//#include <Arduino.h>
#include "QRCodeGxEPD2_BW.h"
#include "qrencode.h"

template<typename GxEPD2_Type, const uint16_t page_height>
QRCodeGxEPD2_BW<GxEPD2_Type,page_height>::QRCodeGxEPD2_BW(GxEPD2_BW<GxEPD2_Type, page_height> *display) {
    this->display = display;
}

template<typename GxEPD2_Type, const uint16_t page_height>
void QRCodeGxEPD2_BW<GxEPD2_Type,page_height>::init() {
    //display->init();
    this->screenwidth = display->width();
    this->screenheight = display->height();
    //display->eraseDisplay();
    int min = screenwidth;
    if (screenheight<screenwidth)
        min = screenheight;
    multiply = min/WD;
    offsetsX = (screenwidth-(WD*multiply))/2;
    offsetsY = (screenheight-(WD*multiply))/2;
}

template<typename GxEPD2_Type, const uint16_t page_height>
void QRCodeGxEPD2_BW<GxEPD2_Type,page_height>::screenwhite() {
    display->fillScreen(GxEPD_WHITE);
}

template<typename GxEPD2_Type, const uint16_t page_height>
void QRCodeGxEPD2_BW<GxEPD2_Type,page_height>::screenupdate() {
    //display->update();
}

template<typename GxEPD2_Type, const uint16_t page_height>
void QRCodeGxEPD2_BW<GxEPD2_Type,page_height>::drawPixel(int x, int y, int color) {
    if(color==1) {
        color = GxEPD_BLACK;
    } else {
        color = GxEPD_WHITE;
    }
    display->fillRect(x,y,multiply,multiply,color);
}

#endif //#ifndef QRCODEGXEPD2BW_HPP