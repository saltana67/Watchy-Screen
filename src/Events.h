#pragma once

#include <functional>
#include <vector>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <sys/time.h>

#include "GetLocation.h"

namespace Watchy_Event {

#define ESP_CORE_0 ((BaseType_t)0)
#define ESP_CORE_1 ((BaseType_t)1)
#define ESP_PRO_CORE ESP_CORE_0
#define ESP_APP_CORE ESP_CORE_1

typedef enum {
  NULL_EVENT = 0,
  MENU_BTN_DOWN,
  BACK_BTN_DOWN,
  UP_BTN_DOWN,
  DOWN_BTN_DOWN,
  ALARM_TIMER,
  LOCATION_UPDATE,
  TIME_SYNC,
  MAX
} ID;

class Event {
  private:
  static QueueHandle_t _q;

  public:
  ID id;
  uint64_t micros;
  union {
    Watchy_GetLocation::location loc; // LOCATION_UPDATE
    timeval tv; // TIME_SYNC
    int bounces; // BTN_DOWN
  };
  void send();
  void handle();
  static void handleAll();
  static QueueHandle_t ISR_Q() { return _q; }; // must initialize first
  static QueueHandle_t Q();
};

extern QueueHandle_t q;

typedef std::function<void()> VoidF_t;

class BackgroundTask {
 private:
  static std::vector<const BackgroundTask *> tasks;

  const char *name;
  const VoidF_t taskFunction;
  TaskHandle_t task;
  const BaseType_t coreID;

 public:
  BackgroundTask(const char *n, const VoidF_t t, const BaseType_t c = ESP_APP_CORE)
      : name(n), taskFunction(t), task(nullptr), coreID(c) {};
  const char *Name() const { return name; }
  void begin();
  void kill();
  ~BackgroundTask();

  void add() const;
  void remove() const;
  static bool running();
};

extern void start();

extern void handle(); // handle all pending events
}  // namespace Watchy_Events