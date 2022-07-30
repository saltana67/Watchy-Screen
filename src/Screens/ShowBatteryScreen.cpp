#include "ShowBatteryScreen.h"

#include <time.h>

#include "OptimaLTStd12pt7b.h"
#include "Watchy.h"

const int batteryReadingssz = 24 * 7;
RTC_DATA_ATTR float batteryReadings[batteryReadingssz];
RTC_DATA_ATTR int nextReading = 0;
RTC_DATA_ATTR time_t lastReadingTS = 0;
const time_t updateInterval = SECS_PER_HOUR;

typedef struct {
  time_t          ts;
  float           voltage;
} ButteryReading;

const int           fastBatteryReadingssz = 60 * 2; //2 hours
RTC_DATA_ATTR ButteryReading fastBatteryReadings[fastBatteryReadingssz];
RTC_DATA_ATTR int   fastNextReading = 0;
const time_t        fastUpdateInterval = 60; //seconds
RTC_DATA_ATTR time_t fastLastReadingTS = 0;
RTC_DATA_ATTR int   fastLastReadingsDone = 0;

void OnWake(esp_sleep_wakeup_cause_t wakeup_reason) {

  { // just read coltage and save in fast  table
      log_d("fastNextReading: %d", fastNextReading);
      fastBatteryReadings[fastNextReading].voltage = Watchy::getBatteryVoltage();
      fastBatteryReadings[fastNextReading].ts = now();
      log_d("now: %lu, fastBatteryReadings[fastNextReading].ts: %lu", now(), fastBatteryReadings[fastNextReading].ts);
      fastLastReadingTS = fastBatteryReadings[fastNextReading].ts;
      fastLastReadingsDone = min(fastBatteryReadingssz, fastLastReadingsDone+1);
      fastNextReading = (fastNextReading + 1) % fastBatteryReadingssz;
  }

  // setup
  if ((lastReadingTS != 0) && (now() < lastReadingTS + updateInterval)) {
    return;  // hasn't been an updateInterval yet
  }
  do {
    batteryReadings[nextReading] = Watchy::getBatteryVoltage();
    nextReading = (nextReading + 1) % batteryReadingssz;
    if (lastReadingTS == 0) {
      // first time
      lastReadingTS = now();
    } else {
      // take another reading one updateInterval after the last one
      lastReadingTS += updateInterval;
    }
    // catch up in case we slept for a very long time
  } while (now() >= lastReadingTS + updateInterval);
}

ShowBatteryScreen::ShowBatteryScreen(uint16_t bg) : Screen(bg) {
  Watchy::AddOnWakeCallback(&OnWake);
}

float mapRange(const float v, const float inMax, const float inMin,
               const float outMax, const float outMin) {
  return (min(inMax, max(inMin, v)) - inMin) / (inMax - inMin) *
             (outMax - outMin) +
         outMin;
}

void logFastReadings(){
  String s = "\n\n";
  if( fastLastReadingTS == 0 )
    return;

  s += "| timestamp | voltage |\n";
  s += "|----------:|--------:|\n";

  char rBuff[100];

  log_d("battery fast reading:%s\n", s);
  log_d("| idx | timestamp | voltage |");
  log_d("|----:|----------:|--------:|");

  for( int i = 0; i < fastBatteryReadingssz; i++ ){
    int ri = (i+(fastNextReading-1))%fastBatteryReadingssz;
    ButteryReading br = fastBatteryReadings[ri];
    log_d("| %u | %lu | %f |", ri, br.ts, br.voltage );
    sprintf(rBuff, "| %u | %lu | %f |\n", ri, br.ts, br.voltage );
    s += rBuff;
  }

  log_d("battery fast reading:%s\n", s);
}

void ShowBatteryScreen::show() {
  logFastReadings();
  Watchy::RTC.setRefresh(RTC_REFRESH_MIN);
  float fgColor = (bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  
  float batteryVoltage = Watchy::getBatteryVoltage();
  float ttl            = Watchy::getTTL(batteryVoltage);
  float batteryState   = Watchy::getBatteryState(batteryVoltage);

  //double fractional = some_double - floor(some_double);

  byte daysLeft  = ttl / 24;
  byte hoursLeft = (ttl - (daysLeft*24));
  log_d("daysLeft: %u, hoursLeft: %u", daysLeft, hoursLeft);

  Watchy::display.setCursor(0,16);
  Watchy::display.printf("battery %5.3fv %.0f\nTTL %u days %u hours", batteryVoltage, batteryState*100.0f, daysLeft, hoursLeft);
  // draw the history graph
  const int16_t margin = 16;
  const int16_t margin_bottom = 2;
  const int16_t height = 120;
  const int16_t width = batteryReadingssz;
  const int16_t x0 = margin + 1;  // left edge of graph
  const int16_t y0 = DISPLAY_HEIGHT - margin_bottom - height - 1;  // top of graph
  // draw a border around it
  Watchy::display.drawRect(x0 - 1, y0 - 1, width + 2, height + 2, fgColor);
  for (int i = 0; i < batteryReadingssz; i++) {
    const float v = batteryReadings[(nextReading + i) % batteryReadingssz];
    if (v > 0.0) {
      // map [4200 .. 3200) onto [100..0)
      const int h = int(Watchy::mapBatteryVoltage(v, height, 0.0) + 0.5);
      log_d("drawLine(%d, %d, %d, %d, fgColor)", x0 + i, y0 + height - h, x0 + i, y0 + height);
      Watchy::display.drawLine(x0 + i, y0 + height - h, x0 + i, y0 + height,
                               fgColor);
    }
  }
}
