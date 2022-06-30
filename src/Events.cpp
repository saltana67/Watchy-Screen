#include "Events.h"

#include <ctime>

#include <WiFi.h>
#include <esp_task.h>
#include <esp_task_wdt.h>
#include <freertos/queue.h>

#include "interrupt_handler.h"
#include "Screen.h"
#include "GetWeather.h"

namespace Watchy_Event {

RTC_DATA_ATTR uint32_t updateInterval = 0;

const char * IDtoString(ID id) {
  switch (id) {
    case NULL_EVENT: return "NULL_EVENT";
    case MENU_BTN_DOWN: return "MENU_BTN_DOWN";
    case BACK_BTN_DOWN: return "BACK_BTN_DOWN";
    case UP_BTN_DOWN: return "UP_BTN_DOWN";
    case DOWN_BTN_DOWN: return "DOWN_BTN_DOWN";
    case ALARM_TIMER: return "ALARM_TIMER";
    case LOCATION_UPDATE: return "LOCATION_UPDATE";
    case TIME_SYNC: return "TIME_SYNC";
    case MAX: return "MAX";
    default: return "unknown";
  }
}

// we're not going to actually run as a background task, this is just to keep
// from deep sleeping while we're running.
BackgroundTask handlerTask("handler", nullptr);

void Event::handle() {
  log_i("%6d handle event %s", millis(), IDtoString(id));
  if (Watchy::screen != nullptr) {
    handlerTask.add();
    switch (id) {
      case MENU_BTN_DOWN:
        log_i("previous bounces: %d", bounces);
        Watchy::screen->menu();
        break;
      case BACK_BTN_DOWN:
        log_i("previous bounces: %d", bounces);
        Watchy::screen->back();
        break;
      case UP_BTN_DOWN:
        log_i("previous bounces: %d", bounces);
        Watchy::screen->up();
        break;
      case DOWN_BTN_DOWN:
        log_i("previous bounces: %d", bounces);
        Watchy::screen->down();
        break;
      case ALARM_TIMER:
        // resets the alarm flag in the RTC
        Watchy::RTC.clearAlarm();
        Watchy::showWatchFace(true);
        break;
      case LOCATION_UPDATE:
        Watchy_GetLocation::location oldLocation;
        oldLocation = Watchy_GetLocation::currentLocation;
        Watchy_GetLocation::currentLocation = loc;
        log_d("LOCATION_UPDATE\n\tcity     : %.30s\n\ttimezone : %s", Watchy_GetLocation::currentLocation.city, Watchy_GetLocation::currentLocation.timezone);
        log_d("       prev    \n\tcity     : %.30s\n\ttimezone : %s", oldLocation.city, oldLocation.timezone);
        log_d("location changed, we hope, force weather update ... ");
        Watchy_GetWeather::getWeather(true);
        break;
      case TIME_SYNC: 
      {
        log_i("time sync: %lu %lu", tv.tv_sec, tv.tv_usec);
        // consider using tv.tv_usec as well
        tmElements_t tm;
        breakTime(tv.tv_sec, tm);
        Watchy::RTC.set(tm);  // set RTC
        setTime(tv.tv_sec);          // set system time
        settimeofday(&tv, nullptr);    // set posix
        log_i("now %ld", now());
      }; 
      break;
      default:
        log_e("unknown event id: %d", id);
        break;
    }
    handlerTask.remove();
  }
}

void Event::send() {
  log_i("send event %s 0x%08x", IDtoString(id), this);
  xQueueSendToBack(Q(), reinterpret_cast<void*>(this), 0);
}

void Event::handleAll() {
  Event e;
  while (xQueueReceive(Q(), &e, 10)) {
    e.handle();
  }
}

QueueHandle_t Event::Q() {
  if (_q != nullptr) {
    return _q;
  }
  _q = xQueueCreate(10, sizeof(Event));

  WatchyInterrupts::buttonSetup(MENU_BTN_PIN, WatchyInterrupts::menu_btn);
  WatchyInterrupts::buttonSetup(BACK_BTN_PIN, WatchyInterrupts::back_btn);
  WatchyInterrupts::buttonSetup(UP_BTN_PIN, WatchyInterrupts::up_btn);
  WatchyInterrupts::buttonSetup(DOWN_BTN_PIN, WatchyInterrupts::down_btn);
  // register for RTC gpio to send screen update events during long running
  // tasks. Figure out how to do this for ESP RTC wakeup timer too.
  return _q;
}

std::vector<const BackgroundTask*> BackgroundTask::tasks;
xSemaphoreHandle taskVectorLock = xSemaphoreCreateMutex();

void BackgroundTask::add() const {
  xSemaphoreTake(taskVectorLock, portMAX_DELAY);
  log_d("%6d %s %08x", millis(), name, reinterpret_cast<uint32_t>(this));
  tasks.push_back(this);
  xSemaphoreGive(taskVectorLock);
}

void BackgroundTask::remove() const {
  xSemaphoreTake(taskVectorLock, portMAX_DELAY);
  log_d("%6d %s %08x", millis(), Name(), reinterpret_cast<uint32_t>(this));
  for (auto it = tasks.begin(); it != tasks.end(); it++) {
    if (*it == this) {
      it = tasks.erase(it);
      break;
    }
  }
  xSemaphoreGive(taskVectorLock);
}

bool BackgroundTask::running() {
  xSemaphoreTake(taskVectorLock, portMAX_DELAY);
  bool r = tasks.size() > 0;
  xSemaphoreGive(taskVectorLock);
  return r;
}

void BackgroundTask::begin() {
  Watchy::initTime(); // kludge - make sure "now()" is initialized for rate limit checkers
  add();
  BaseType_t res = xTaskCreatePinnedToCore(
      [](void *p) {
        BackgroundTask *b = static_cast<BackgroundTask*>(p);
        b->taskFunction();
        b->kill();
      },
      name, 4096, reinterpret_cast<void *>(this), tskIDLE_PRIORITY, &task, 1);
  configASSERT(task);
  if (res != pdPASS) {
    log_d("create background task result %d", res);
  }
}

void BackgroundTask::kill() {
  if (task != nullptr) {
    TaskHandle_t t = task;
    log_i("%s stack high water %d", Name(), uxTaskGetStackHighWaterMark(t));
    task = nullptr;
    remove();
    vTaskDelete(t); // doesn't return, because it's deleting us...
  }
}

BackgroundTask::~BackgroundTask() {
  kill();
}

QueueHandle_t Event::_q;
}  // namespace Watchy_Event