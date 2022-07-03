#ifndef QRCODEGXEPD2BW_H
#define QRCODEGXEPD2BW_H

/* ESP_QRcode. e-ink GxEPD2_BW version
 * Import this .h when using some GxEPD2_BW display
 */

#define EINKDISPLAY

#include <qrcodedisplay.h>
//#include <Adafruit_GFX.h>   
#include <GxEPD2_BW.h>

#ifndef EINK_MODEL
#define EINK_MODEL 128
#endif

template<typename GxEPD2_Type, const uint16_t page_height>
class QRCodeGxEPD2_BW : public QRcodeDisplay
{
	private:
		GxEPD2_BW<GxEPD2_Type, page_height> *display;
        void drawPixel(int x, int y, int color);
	public:
		QRCodeGxEPD2_BW(GxEPD2_BW<GxEPD2_Type, page_height> *display);
		void init();
		void screenwhite();
		void screenupdate();
};

// include template definition implementation file
// advice take from https://www.bogotobogo.com/cplusplus/template_declaration_definition_header_implementation_file.php
#include "QRCodeGxEPD2_BW.hpp"

#endif