#include "BlufiScreen.h"

#include "blufi.h"

#include "OptimaLTStd12pt7b.h"
#include "Watchy.h"


Watchy_Event::BackgroundTask blufi("blufi", []() {
  blufi_init("Watchy");
  vTaskDelay(portMAX_DELAY);
});

void BlufiScreen::show() {
  Watchy::RTC.setRefresh(RTC_REFRESH_NONE);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  Watchy::display.setCursor(0, 0);

  Watchy::display.print(
      "\n"
      "Blufi server\n"
      "started. Configure\n"
      "Wifi via Blufi app.\n"
      "\n"
      "Back to exit.");
  blufi.begin();
}

void BlufiScreen::back() {
  blufi.kill();
  Screen::back();
}