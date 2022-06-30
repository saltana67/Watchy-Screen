#include "WatchyRTC.h"
#include "config.h"


RTC_DATA_ATTR RTC_REFRESH_t WatchyRTC::_refresh = RTC_REFRESH_NONE;

WatchyRTC::WatchyRTC(void) : rtc_pcf() {
}

void WatchyRTC::init() {

  {
  uint64_t wire_begin = micros();
  Wire.begin((int)SDA, (int)SCL, 400000UL);
  uint64_t elapsed = micros() - wire_begin;
      log_i("Wire.begin took %llu.%03llums ***", elapsed / 1000,
        elapsed % 1000);
  }
  
  byte error;
  Wire.beginTransmission(RTCC_ADDR);
  error = Wire.endTransmission();
  if (error != 0) {
    log_e("RTC Error: %d", error);
  }/*else
    rtc_pcf = Rtc_Pcf8563();
  */
}

void WatchyRTC::config(tmElements_t *initialTime){
      rtc_pcf.initClock();
//    rtc_pcf.zeroClock();
    if( initialTime ) 
        set(*initialTime);
}

void WatchyRTC::read(tmElements_t &tm){
    rtc_pcf.readDateTime();
    tm.Year = CalendarYrToTm( (rtc_pcf.getCentury() ? 1900 : 2000) + rtc_pcf.getYear() );
    tm.Month = rtc_pcf.getMonth();
    tm.Day = rtc_pcf.getDay();
    tm.Wday = rtc_pcf.getWeekday() + 1;
    tm.Hour = rtc_pcf.getHour();
    tm.Minute = rtc_pcf.getMinute();
    tm.Second = rtc_pcf.getSecond();
}

void WatchyRTC::set(tmElements_t tm){
    byte year = tmYearToCalendar(tm.Year);
    byte century = 0;
    if( year < 2000 ){
      century = 1;
      year -= 1900;
    }else
      year -= 2000;

    rtc_pcf.setDateTime(tm.Day, tm.Wday - 1, tm.Month, century, year, tm.Hour, tm.Minute, tm.Second);
}


#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i)    \
    (((i) & 0x80ll) ? '1' : '0'), \
    (((i) & 0x40ll) ? '1' : '0'), \
    (((i) & 0x20ll) ? '1' : '0'), \
    (((i) & 0x10ll) ? '1' : '0'), \
    (((i) & 0x08ll) ? '1' : '0'), \
    (((i) & 0x04ll) ? '1' : '0'), \
    (((i) & 0x02ll) ? '1' : '0'), \
    (((i) & 0x01ll) ? '1' : '0')

#define PRINTF_COLTROL1_PATTERN "TEST1[0]:mode %s, STOP[5]: RTC %s, TESTC[3]: POR %s"
#define PRINTF_COLTROL1(i)    \
    (((i) & (0x01<<7)) ? "EXT_CLK test(1)"  : "normal(0)")    , \
    (((i) & (0x01<<5)) ? "stopped(1)"       : "runs(0)")      , \
    (((i) & (0x01<<3)) ? "may be enabled(1)": "disabled(0)")

#define PRINTF_COLTROL2_PATTERN "TI_TP[4]:INT %s, AF[3]: %s, TF[2]: %s, AIE[1] %s, TIE[0]: %s"
#define PRINTF_COLTROL2(i)    \
    (((i) & (0x01<<4)) ? "pulses(1)"        : "follows TF(0)")    , \
    (((i) & (0x01<<3)) ? "active(1)"        : "inactive(0)")      , \
    (((i) & (0x01<<2)) ? "active(1)"        : "inactive(0)")      , \
    (((i) & (0x01<<1)) ? "enabled(1)"       : "disabled(0)")      , \
    (((i) & (0x01<<0)) ? "enabled(1)"       : "disabled(0)")

#define PRINTF_TIMER_COLTROL_PATTERN "TE[7]: %s,  TD[1:0]: %s"
#define PRINTF_TIMER_COLTROL(i)    \
    (((i) & (0x01<<7)) ? "enabled(1)"       : "disabled(0)")      , \
    ( (((i) & 0x03) == TMR_4096HZ) ? "4096HZ(00)" : ( (((i) & 0x03) == TMR_64Hz) ? "64Hz(01)" : ((((i) & 0x03) == TMR_1Hz) ? "1Hz(10)" : "1MIN(11)")  ) )

void WatchyRTC::setRefresh(RTC_REFRESH_t r){
/*
  if(rtc_pcf.alarmActive()){
    rtc_pcf.resetAlarm();
  }
  if(rtc_pcf.timerActive()){
    rtc_pcf.clearTimer();
  }
*/

  rtc_pcf.readAll();
  /*
  log_d("status 1     : " PRINTF_BINARY_PATTERN_INT8 " : " PRINTF_COLTROL1_PATTERN , PRINTF_BYTE_TO_BINARY_INT8(rtc_pcf.getStatus1()), PRINTF_COLTROL1(rtc_pcf.getStatus1()) );
  log_d("status 2     : " PRINTF_BINARY_PATTERN_INT8 " : " PRINTF_COLTROL2_PATTERN , PRINTF_BYTE_TO_BINARY_INT8(rtc_pcf.getStatus2()), PRINTF_COLTROL2(rtc_pcf.getStatus2()) );
  log_d("timer control: " PRINTF_BINARY_PATTERN_INT8 " : " PRINTF_TIMER_COLTROL_PATTERN , PRINTF_BYTE_TO_BINARY_INT8(rtc_pcf.getTimerControl()), PRINTF_TIMER_COLTROL(rtc_pcf.getTimerControl()) );
  log_d("timer value  : %d", rtc_pcf.getTimerValue() );
  log_d("timer enabled: %s", rtc_pcf.timerEnabled() ? "true" : "false");
  log_d("alarm enabled: %s", rtc_pcf.alarmEnabled() ? "true" : "false");
  */
  rtc_pcf.clearAlarm();
  rtc_pcf.clearTimer();
  rtc_pcf.clearSquareWave();

  /*
  rtc_pcf.readAll();
  log_d("status 1     : " PRINTF_BINARY_PATTERN_INT8 PRINTF_COLTROL1_PATTERN , PRINTF_BYTE_TO_BINARY_INT8(rtc_pcf.getStatus1()), PRINTF_COLTROL1(rtc_pcf.getStatus1()) );
  log_d("status 2     : " PRINTF_BINARY_PATTERN_INT8 PRINTF_COLTROL2_PATTERN , PRINTF_BYTE_TO_BINARY_INT8(rtc_pcf.getStatus2()), PRINTF_COLTROL2(rtc_pcf.getStatus2()) );
  log_d("timer control: " PRINTF_BINARY_PATTERN_INT8 PRINTF_TIMER_COLTROL_PATTERN , PRINTF_BYTE_TO_BINARY_INT8(rtc_pcf.getTimerControl()), PRINTF_TIMER_COLTROL(rtc_pcf.getTimerControl()) );
  log_d("timer value  : %d", rtc_pcf.getTimerValue() );
  log_d("timer enabled: %s", rtc_pcf.timerEnabled() ? "true" : "false");
  log_d("alarm enabled: %s", rtc_pcf.alarmEnabled() ? "true" : "false");
  */
  //rtc_pcf.setTimer(10);

  if( r == RTC_REFRESH_MIN ){
    byte next_minute = rtc_pcf.getMinute();
    log_i("minute: %d, next: %d, trimmed: %d", next_minute, next_minute+1, (next_minute+1)%60);
    next_minute = (++next_minute)%60;
    log_i("setting alarm for next minute: %d", next_minute);
    rtc_pcf.setAlarm(next_minute, RTCC_NO_ALARM, RTCC_NO_ALARM, RTCC_NO_ALARM);

    esp_sleep_enable_ext0_wakeup(RTC_PIN, 0);
  }else if( r == RTC_REFRESH_NONE ){
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_EXT0);
  }else if( r == RTC_REFRESH_FAST ){
    //log_d("unimplemented refresh: %s (%d)", "RTC_REFRESH_FAST", r);
  }else if( r == RTC_REFRESH_SEC ){
    log_e("unimplemented refresh: %s (%d)", "RTC_REFRESH_SEC", r);
  }

  WatchyRTC::_refresh = r;
  log_d("refresh returns, refresh: %d", WatchyRTC::_refresh);
}

void WatchyRTC::clearAlarm(){
  rtc_pcf.clearAlarm();
}


