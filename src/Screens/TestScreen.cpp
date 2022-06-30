#include "TestScreen.h"

using namespace Watchy;

void TestScreen::show() {
    log_d("show called");
    Watchy::RTC.setRefresh(RTC_REFRESH_MIN);
}

void TestScreen::up() {
    log_d("up called");
}

void TestScreen::down() {
    log_d("down called");
}

void TestScreen::back() {
    log_d("back called, calling super");
    Screen::back();
}

void TestScreen::menu() {
    log_d("menu called");
}

