#ifndef WATCHY_BadForEye_H
#define WATCHY_BadForEye_H

#include <Screen.h>

class BadForEye : public Screen{
    public:
        BadForEye(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
        void show() override;
        void drawWatchFace(tmElements_t t);
};

#endif
