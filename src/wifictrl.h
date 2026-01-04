#pragma once

#include <Arduino.h>

namespace Watchy {

void initMultiWifi(void);

// these two keep track of references to wifi and only close it when there
// are no more references to it
bool getWiFi();
void releaseWiFi();
boolean wifiConnectionAttemptAllowed();
void resetWiFi();

extern bool WIFI_CONFIGURED;
extern bool BLE_CONFIGURED;
extern bool AIRPLANE_MODE;
}