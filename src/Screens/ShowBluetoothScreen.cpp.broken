#include "ShowBluetoothScreen.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "OptimaLTStd12pt7b.h"
#include "Watchy.h"
#include "esp_wifi.h"

const BLEUUID WIFI_SERVICE_UUID("bc515ca2-ffe1-447b-acf0-23adba95aa97");
const BLEUUID SSID_CHARACTERISTIC_UUID ("5b2519d5-9ffd-47c7-b799-1991d9885247");
const BLEUUID PASSWORD_CHARACTERISTIC_UUID ("a5d48484-e3cc-42d0-9cd7-b387c71ca200");

const BLEUUID RTC_SERVICE_UUID ("c9ef2c03-d6db-47b3-873e-c3a43540be71");
const BLEUUID TIME_CHARACTERISTIC_UUID ("d8e77c60-0d1d-4c9c-8c4b-3c3ebc883db9");
const BLEUUID DATE_CHARACTERISTIC_UUID ("094e3003-5a43-4bd2-8554-4e8c29e3cec2");
const BLEUUID TZ_CHARACTERISTIC_UUID ("ecd540d8-e805-412e-838e-e511a498f454");

const BLEUUID LOCATION_SERVICE_UUID ("3b4b2f44-e9d0-4e66-aa61-2360100de52b");
const BLEUUID LAT_CHARACTERISTIC_UUID ("d7e2a9ca-a2b0-42de-8061-4c9a6cfd7e7f");
const BLEUUID LON_CHARACTERISTIC_UUID ("3562643a-9528-4ee4-9f73-9b7bdcfda377");

const BLEUUID ACCELEROMETER_SERVICE_UUID ("042f30f4-9d03-455d-b1a1-2fe506bdcf85");
const BLEUUID X_CHARACTERISTIC_UUID ("034526bd-a6cb-4b90-9828-0e156f156b67");
const BLEUUID Y_CHARACTERISTIC_UUID ("97ed8e5b-1223-4402-be7a-8c9f923aaa7b");
const BLEUUID Z_CHARACTERISTIC_UUID ("38d8142d-23d6-46f7-aa4b-62985141964c");
const BLEUUID ORIENTATION_CHARACTERISTIC_UUID ("085b5a8b-56ed-453f-9f72-402b680213b4");

const BLEUUID BATTERY_SERVICE_UUID ("e16157ca-dae2-4dc0-a2c8-32c1d6053932");
const BLEUUID VOLTAGE_CHARACTERISTIC_UUID ("c67d5954-580d-4b8f-a85a-9148733e5ea1");

/*
Services

  WiFi
    SSID
    Password

  RTC
    Time
    Date
    TZ

  Location
    Lat
    Lon

  Accelerometer
    Steps
    X
    Y
    Z
    Orientation

  Battery
    Voltage

*/

class SSIDChanged : public BLECharacteristicCallbacks {
  public:
   void onWrite(BLECharacteristic *pCharacteristic) override {
     wifi_config_t conf;
     esp_err_t ret = esp_wifi_get_config(ESP_IF_WIFI_STA, &conf);
     if (ret == ESP_ERR_WIFI_NOT_INIT) {
       wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
       esp_wifi_init(&wifi_config);
       ret = esp_wifi_get_config(ESP_IF_WIFI_STA, &conf);
     };
     if (ret != ESP_OK) {
       log_e("error: %s", esp_err_to_name(ret));
       return;
     }
     strncpy(reinterpret_cast<char *>(conf.sta.ssid),
             pCharacteristic->getValue().c_str(), sizeof(conf.sta.ssid));
     ret = esp_wifi_set_config(ESP_IF_WIFI_STA, &conf);
          log_i("set ssid %s", conf.sta.ssid);
     if (ret != ESP_OK) {
       log_e("error: %s", esp_err_to_name(ret));
     }
   };
} ssidChanged;

class PasswordChanged : public BLECharacteristicCallbacks {
  public:
   void onWrite(BLECharacteristic *pCharacteristic) override {
     wifi_config_t conf;
     esp_err_t ret = esp_wifi_get_config(ESP_IF_WIFI_STA, &conf);
     if (ret == ESP_ERR_WIFI_NOT_INIT) {
       wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
       esp_wifi_init(&wifi_config);
       ret = esp_wifi_get_config(ESP_IF_WIFI_STA, &conf);
     };
     if (ret != ESP_OK) {
       log_e("error: %s", esp_err_to_name(ret));
       return;
     }
     strncpy(reinterpret_cast<char *>(conf.sta.password),
             pCharacteristic->getValue().c_str(), sizeof(conf.sta.password));
     ret = esp_wifi_set_config(ESP_IF_WIFI_STA, &conf);
     log_i("set password %s", conf.sta.password);
     if (ret != ESP_OK) {
       log_e("error: %s", esp_err_to_name(ret));
     }
   };
} passwordChanged;

Watchy_Event::BackgroundTask advertiseBLE("advertiseBLE", []() {
  BLEDevice::init("Watchy");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService;
  BLECharacteristic *pCharacteristic;

  pService = pServer->createService(WIFI_SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      SSID_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(&ssidChanged);
  wifi_config_t conf;
  esp_err_t ret = esp_wifi_get_config(ESP_IF_WIFI_STA, &conf);
  if (ret == ESP_ERR_WIFI_NOT_INIT) {
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_config);
    ret = esp_wifi_get_config(ESP_IF_WIFI_STA, &conf);
  };
  if (ret != ESP_OK) {
    log_e("error: %s", esp_err_to_name(ret));
  }
  pCharacteristic->setValue(
      conf.sta.ssid,
      strnlen(reinterpret_cast<char *>(conf.sta.ssid), sizeof(conf.sta.ssid)));
  pCharacteristic = pService->createCharacteristic(
      PASSWORD_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(&passwordChanged);
  pCharacteristic->setValue(conf.sta.password,
                            strnlen(reinterpret_cast<char *>(conf.sta.password),
                                    sizeof(conf.sta.password)));
  pService->start();

  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is
  // working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(WIFI_SERVICE_UUID);
  pAdvertising->setScanResponse(true);

  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMaxPreferred(0x12);

  BLEDevice::startAdvertising();
  vTaskDelay(portMAX_DELAY);
});

void ShowBluetoothScreen::show() {
  Watchy::RTC.setRefresh(RTC_REFRESH_NONE);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  Watchy::display.setCursor(0, 0);

  Watchy::display.print(
      "\n"
      "BLE Server\n"
      "You can set and\n"
      "get values via BLE.\n"
      "\n"
      "Back to exit.");
  advertiseBLE.begin();
}

void ShowBluetoothScreen::back() {
  advertiseBLE.kill();
  Screen::back();
}