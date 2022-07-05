#include "Watchy.h"

#include <vector>

#include "Events.h"
#include "GetLocation.h"  // bad dependency
#include "Screen.h"
#include "Sensor.h"
#include "WatchyErrors.h"
#include "esp_wifi.h"
#include "GetWeather.h"

namespace Watchy {

Error err;

uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data,
                       uint16_t len);
uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data,
                        uint16_t len);

WatchyRTC RTC;
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(
  GxEPD2_154_D67(CS, DC, RESET, BUSY));
RTC_DATA_ATTR Screen *screen = nullptr;

RTC_DATA_ATTR BMA423 sensor;
RTC_DATA_ATTR bool WIFI_CONFIGURED;
RTC_DATA_ATTR bool BLE_CONFIGURED;

void handleButtonPress() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
  switch (wakeupBit & BTN_PIN_MASK) {
    case MENU_BTN_MASK:
      Watchy_Event::Event{
          .id = Watchy_Event::MENU_BTN_DOWN,
          .micros = micros(),
      }.send();
      break;
    case BACK_BTN_MASK:
      Watchy_Event::Event{
          .id = Watchy_Event::BACK_BTN_DOWN,
          .micros = micros(),
      }.send();
      break;
    case UP_BTN_MASK:
      Watchy_Event::Event{
          .id = Watchy_Event::UP_BTN_DOWN,
          .micros = micros(),
      }.send();
      break;
    case DOWN_BTN_MASK:
      Watchy_Event::Event{
          .id = Watchy_Event::DOWN_BTN_DOWN,
          .micros = micros(),
      }.send();
      break;
    default:
      sensor.getINT();
      boolean accInterrupt = sensor.isDoubleClick();
      log_d("accInterrupt: %d",accInterrupt);
      Watchy_Event::Event{
          .id = Watchy_Event::DOWN_BTN_DOWN,
          .micros = micros(),
      }.send();
      break;
  }
}

QueueHandle_t i2cMutex = xSemaphoreCreateRecursiveMutex();

tmElements_t currentTime
= {
      .Second = 0,
      .Minute = 30,
      .Hour = 13,
      .Wday = dowFriday,
      .Day = 30,
      .Month = 4,
      .Year = CalendarYrToTm(2022)
}
;  // should probably be in SyncTime

tmElements_t getLocalTime(tmElements_t &currentTime) {
    tm localTime;
    getLocalTime(localTime);
    currentTime.Day = localTime.tm_mday;
    currentTime.Month = localTime.tm_mon + 1;
    currentTime.Year = localTime.tm_year - 70;
    currentTime.Wday = localTime.tm_wday + 1;
    currentTime.Hour = localTime.tm_hour;
    currentTime.Minute = localTime.tm_min;
    currentTime.Second = localTime.tm_sec;
    return currentTime;
}

tm getLocalTime(tm &localTime) {
    time_t tNow = now();
    localtime_r(&tNow, &localTime);
    return localTime;
}

// doesn't persist over deep sleep. don't care.
std::vector<OnWakeCallback> owcVec;

void AddOnWakeCallback(const OnWakeCallback owc) { owcVec.push_back(owc); }

const char *wakeupReasonToString(esp_sleep_wakeup_cause_t wakeup_reason) {
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_UNDEFINED:
      return ("UNDEFINED");
    case ESP_SLEEP_WAKEUP_ALL:
      return ("ALL");
    case ESP_SLEEP_WAKEUP_EXT0:
      return ("EXT0");
    case ESP_SLEEP_WAKEUP_EXT1:
      return ("EXT1");
    case ESP_SLEEP_WAKEUP_TIMER:
      return ("TIMER");
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
      return ("TOUCHPAD");
    case ESP_SLEEP_WAKEUP_ULP:
      return ("ULP");
    case ESP_SLEEP_WAKEUP_GPIO:
      return ("GPIO");
    case ESP_SLEEP_WAKEUP_UART:
      return ("UART");
    default:
      return ("unknown");
  }
}

uint64_t start;

void initTime(boolean initialReset) {
  static bool done;
  if (done) { return; }

  //Wire.begin((int)SDA, (int)SCL, 400000UL);

  RTC.init();
  // sync ESP32 clocks to RTC
  if(initialReset)
    RTC.config(&currentTime);
  else
    RTC.read(currentTime);
  RTC.setRefresh(RTC_REFRESH_MIN);

  /*
  log_i("RTC Current time: %02d/%02d/%02d %02d:%02d:%02d %d", currentTime.Day,
        currentTime.Month, currentTime.Year, currentTime.Hour,
        currentTime.Minute, currentTime.Second, currentTime.Wday);
  */

  log_i("RTC Current time: %04d-%02d-%02d %s %02d:%02d:%02d",
        tmYearToCalendar(currentTime.Year),currentTime.Month, currentTime.Day, 
        dayShortStr(currentTime.Wday),
        currentTime.Hour,currentTime.Minute, currentTime.Second
        );

  setenv("TZ", Watchy_GetLocation::currentLocation.timezone, 1);
  tzset();
  time_t t = makeTime(currentTime);
  setTime(t);
  timeval tv = {t, 0};
  settimeofday(&tv, nullptr);
  done = true;
}

void initTimeOld(String datetime) {
  static bool done;
  if (done) { return; }
  //Wire.begin(SDA, SCL);  // init i2c
  Wire.begin((int)SDA, (int)SCL, 400000UL);

  RTC.init();
  // sync ESP32 clocks to RTC
  //RTC.config(datetime);
  RTC.readNew(currentTime);
  log_i("RTC Current time: %02d/%02d/%02d %02d:%02d:%02d %d", currentTime.Day,
        currentTime.Month, currentTime.Year, currentTime.Hour,
        currentTime.Minute, currentTime.Second, currentTime.Wday);
  //TODO setenv("TZ", Watchy_GetLocation::currentLocation.timezone, 1);
  tzset();
  time_t t = makeTime(currentTime);
  setTime(t);
  timeval tv = {t, 0};
  settimeofday(&tv, nullptr);
  done = true;
}


void init() {
  start = micros();
  Watchy_Event::BackgroundTask initTask("init", nullptr);
  initTask.add();
  esp_sleep_wakeup_cause_t wakeup_reason =
      esp_sleep_get_wakeup_cause();  // get wake up reason
  log_i("reason %s (%d)", wakeupReasonToString(wakeup_reason), wakeup_reason);

  boolean reinit = (
                      (wakeup_reason <= ESP_SLEEP_WAKEUP_UNDEFINED)
                    ||(wakeup_reason > ESP_SLEEP_WAKEUP_UART)
  );

  initTime( reinit );


  for (auto &&owc : owcVec) {
    owc(wakeup_reason);
  }

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_TIMER:  // ESP Internal RTC
      Watchy_Event::Event{
          .id = Watchy_Event::ALARM_TIMER,
          .micros = micros(),
      }.send();
      break;
    case ESP_SLEEP_WAKEUP_EXT0:  // RTC Alarm
      Watchy_Event::Event{
          .id = Watchy_Event::ALARM_TIMER,
          .micros = micros(),
      }.send();
      break;
    case ESP_SLEEP_WAKEUP_EXT1:  // button Press
      handleButtonPress();
      break;
    default:  // reset
      bmaConfig();
      showWatchFace(false);  // full update on reset
      break;
  }
  //TODO -< showWatchFace(false);

  initTask.remove();
  for (;;) {
    Watchy_Event::Event::handleAll();
    if (!Watchy_Event::BackgroundTask::running()) {
      if (RTC.refresh() != RTC_REFRESH_FAST) {
        break;
      }
      showWatchFace(true);
    }
  }
  //TODO WatchyRTC2::init(wakeup_reason);
  //TODO RTC.setRefresh(RTC_REFRESH_MIN);
  Watchy::deepSleep();
}

void deepSleep() {
  uint64_t elapsed = micros() - start;
  display.hibernate();
  esp_sleep_enable_ext1_wakeup(
      BTN_PIN_MASK|ACC_INT_MASK,
      ESP_EXT1_WAKEUP_ANY_HIGH);  // enable deep sleep wake on button press
  log_i("%6d *** sleeping after %llu.%03llums ***\n", millis(), elapsed / 1000,
        elapsed % 1000);
  esp_deep_sleep_start();
}

void showWatchFace(bool partialRefresh, Screen *s) {
  uint64_t start = micros();
  display.init(0, false);  //_initial_refresh to false to prevent full update on init
  display.setFullWindow();
  display.setTextColor((s->bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE));
  display.setCursor(0, 0);
  s->show();
  display.display(partialRefresh);  // partial refresh
  uint64_t elapsed = micros() - start;
  log_d("took %llu.%03llums", elapsed / 1000,elapsed % 1000);
}

const Screen *getScreen() { return screen; }

// setScreen is used to set a new screen on the display
void setScreen(Screen *s) {
  if (s == nullptr) {
    return;
  }
  screen = s;
  showWatchFace(true);
}

// This mutex protects the WiFi object, wifiConnectionCount
// and wifiReset
auto wifiMutex = xSemaphoreCreateMutex();
RTC_DATA_ATTR bool wifiReset = false;

void checkWifiConfig(){
  log_d("\n\n----");

    const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };

    wifi_mode_t mode;
    esp_err_t wifiError;

    wifiError = esp_wifi_get_mode(&mode);

    if( wifiError != ESP_OK ){
      switch(wifiError){
        case(ESP_ERR_WIFI_NOT_INIT): log_e("esp_wifi_get_mode: ESP_ERR_WIFI_NOT_INIT"); break;
        case(ESP_ERR_INVALID_ARG)  : log_e("esp_wifi_get_mode: ESP_ERR_INVALID_ARG")  ; break;
      }
      return;
    }
    log_e("esp_wifi_get_mode: %d : %s", mode, modes[mode]);

    const char* secondChanName[] = { 
        "WIFI_SECOND_CHAN_NONE"
      , "WIFI_SECOND_CHAN_ABOVE"
      , "WIFI_SECOND_CHAN_BELOW"
    };
    

    uint8_t primaryChan;
    wifi_second_chan_t secondChan;
    wifiError = esp_wifi_get_channel(&primaryChan, &secondChan);

    if( wifiError != ESP_OK ){
      switch(wifiError){
        case(ESP_ERR_WIFI_NOT_INIT): log_e("esp_wifi_get_channel: ESP_ERR_WIFI_NOT_INIT"); break;
        case(ESP_ERR_INVALID_ARG)  : log_e("esp_wifi_get_channel: ESP_ERR_INVALID_ARG")  ; break;
      }
      return;
    }
    log_d("esp_wifi_get_channel: primary: %d, secondary: %s (%d)", 
                          primaryChan
                        , secondChan > WIFI_SECOND_CHAN_BELOW ? "????" : secondChanName[secondChan%WIFI_SECOND_CHAN_BELOW]
                        , secondChan
                        );


    /*
        p.print("AP id: ");
        p.println(wifi_station_get_current_ap_id());

        p.print("Status: ");
        p.println(wifi_station_get_connect_status());
    */

    wifi_config_t conf;
    wifiError = esp_wifi_get_config((wifi_interface_t)WIFI_IF_STA, &conf);

    if( wifiError != ESP_OK ){
      switch(wifiError){
        case(ESP_ERR_WIFI_NOT_INIT): log_e("esp_wifi_get_config: ESP_ERR_WIFI_NOT_INIT"); break;
        case(ESP_ERR_INVALID_ARG)  : log_e("esp_wifi_get_config: ESP_ERR_INVALID_ARG")  ; break;
        case(ESP_ERR_WIFI_IF)      : log_e("esp_wifi_get_config: ESP_ERR_WIFI_IF")      ; break;
      }
      return;
    }
//    log_d("esp_wifi_get_channel: primary: %d, secondary: %s", primaryChan, secondChanName[secondChan]);

    const char* ssid = reinterpret_cast<const char*>(conf.sta.ssid);
    log_d("SSID(%d): %s", ssid?strlen(ssid):0, ssid?ssid:"");

    const char* passphrase = reinterpret_cast<const char*>(conf.sta.password);
    log_d("PASSWORD(%d): %s", ssid?strlen(passphrase):0, passphrase?passphrase:"");

/*
    const char* passphrase = reinterpret_cast<const char*>(conf.sta.password);
    p.print("Passphrase (");
    p.print(strlen(passphrase));
    p.print("): ");
    p.println(passphrase);

    p.print("BSSID set: ");
    p.println(conf.sta.bssid_set);

*/
  log_d("\n----\n\n");
}

namespace {

RTC_DATA_ATTR time_t  firstUnsuccessfulConnectionTimeTS = 0;
RTC_DATA_ATTR time_t  lastUnsuccessfulConnectionTimeTS = 0;
RTC_DATA_ATTR uint8_t unsuccessfulConnectionAttemps = 0;
RTC_DATA_ATTR time_t  allowedPeriod = 0;

void logUnsuccessfulConnectionAttempt(){
  time_t n = now();
  lastUnsuccessfulConnectionTimeTS = n;
  if( firstUnsuccessfulConnectionTimeTS == 0 )
    firstUnsuccessfulConnectionTimeTS = lastUnsuccessfulConnectionTimeTS;
  unsuccessfulConnectionAttemps += unsuccessfulConnectionAttemps>100?0:1;

  if( unsuccessfulConnectionAttemps < 3 ) {
    allowedPeriod = 30;
  }else if(unsuccessfulConnectionAttemps < 10){
    allowedPeriod = 60;
  }else if(unsuccessfulConnectionAttemps < 20){
    allowedPeriod = (60*10);
  }else if(unsuccessfulConnectionAttemps < 30){
    allowedPeriod = (60*30);
  }else{
    allowedPeriod = (60*60);
  }
}
void clearUnsuccessfulConnectionAttemptLog(){
  unsuccessfulConnectionAttemps = 0;
}

}

boolean wifiConnectionAttemptAllowed(){
  log_d("attempts: %d", unsuccessfulConnectionAttemps);

  if( unsuccessfulConnectionAttemps < 1 )
    return true;

  time_t n = now();
  time_t fromFirst  = n - firstUnsuccessfulConnectionTimeTS;
  time_t fromLast   = n - lastUnsuccessfulConnectionTimeTS;

  time_t sLeft   = allowedPeriod - fromLast;

  log_d("attempts: %d, fromLast: %ld, allowedTS: %ld, left: %ld, (left > 0): %d", 
    unsuccessfulConnectionAttemps, fromLast, allowedPeriod, sLeft, (sLeft > 0));

  if( sLeft > 0 )
    return false;
  else
    return true;
}

bool connectWiFi() {
  // in theory this is re-entrant, but in practice if you call WiFi.begin()
  // while it's still trying to connect, it will return an error. Better
  // to serialize WiFi.begin()
  if (!wifiReset) {
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_config);
    wifiReset = true;
  }
#if !defined(WIFI_SSID) || !defined(WIFI_PASSWORD)
  if (WL_CONNECT_FAILED == WiFi.begin()) {
    // WiFi not setup, you can also use hard coded credentials with
    // WiFi.begin(SSID,PASS); by defining WIFI_SSID and WIFI_PASSWORD
#else
  if (WL_CONNECT_FAILED == WiFi.begin() &&
      WL_CONNECT_FAILED == WiFi.begin(WIFI_SSID, WIFI_PASSWORD)) {
    // WiFi not setup
#endif
    WIFI_CONFIGURED = false;
    logUnsuccessfulConnectionAttempt();
  } else {
    if (WL_CONNECTED ==
        WiFi.waitForConnectResult()) {  // attempt to connect for 10s
      checkWifiConfig();
      WIFI_CONFIGURED = true;
      clearUnsuccessfulConnectionAttemptLog();
    } else {  // connection failed, time out
      WIFI_CONFIGURED = false;
      logUnsuccessfulConnectionAttempt();
      // turn off radios
      WiFi.mode(WIFI_OFF);
      btStop();
    }
  }
  return WIFI_CONFIGURED;
}

unsigned int wifiConnectionCount = 0;

bool getWiFi() {
  checkWifiConfig();
  xSemaphoreTake(wifiMutex, portMAX_DELAY);
  if( ! Watchy::wifiConnectionAttemptAllowed() ){
    log_d("wifiConnectionAttemptAllowed: false");
    xSemaphoreGive(wifiMutex);
    return false;
  }
  if (wifiConnectionCount == 0) {
    if (!connectWiFi()) {
      xSemaphoreGive(wifiMutex);
      return false;
    }
    log_d("wifi connected");
  }
  wifiConnectionCount++;
  xSemaphoreGive(wifiMutex);
  return true;
}

void releaseWiFi() {
  xSemaphoreTake(wifiMutex, portMAX_DELAY);
  wifiConnectionCount--;
  if (wifiConnectionCount == 0) {
    log_d("wifi disconnected");
    btStop();
    WiFi.mode(WIFI_OFF);
  }
  xSemaphoreGive(wifiMutex);
}



}  // namespace Watchy