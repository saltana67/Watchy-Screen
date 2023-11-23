# Watchy-Screen

**This is my (saltana67) fork of Watchy-Screen by Charles Heynes. Original README follows.**

[![Build repo](https://github.com/charles-haynes/Watchy-Screen/actions/workflows/build.yml/badge.svg)](https://github.com/charles-haynes/Watchy-Screen/actions/workflows/build.yml)

Screen based implementation of the [Watchy](https://github.com/sqfmi/Watchy) library. It assumes some familiarity with the Watchy library and only documents the differences.

## Building

To build this project, install [PlatformIO](https://docs.platformio.org/en/latest/core/installation.html) and simply run `pio run -t upload` to build and upload to the watch.

## Screen abstraction

```c++
class Screen {
 public:
  Screen* parent;
  uint16_t bgColor;
  Screen(uint16_t bg = GxEPD_BLACK) : bgColor(bg){};
  virtual void show() = 0;
  virtual void up() {}
  virtual void down() {}
  virtual void back() { Watchy::setScreen(parent); }
  virtual void menu() {}
};};
```

A screen encapsulates the logic to display one screenful of information, and to respond to button presses. To make a `Screen` instance, subclass `Screen` and implement the `show` method to display your screen. `show` is called with the screen cleared to your `bgColor`, text color set to the opposite of `bgColor`, and the cursor set to 0,0. After `show` is done painting, it should return and the display will do a partial refresh.

By default the screen does nothing on up, down, or menu button press, and goes up to it's parent (if any) on back button press. To enable the screen to respond to buttons override the corresponding button method in your subclass. For example, if there was a screen to show the time called `timeScreen` and another to display a content called `contentScreen` Where pressing the `up` button while displaying the time screen should display the content, and pressing `Back` while displaying the content screen should return to the time screen:

```c++
// ContentScreen.h
#include "Screen.h"
#include "TimeScreen.h"
#include "Watchy.h"

class ContentScreen : public Screen {
    public:
    void show() { Watchy::display.print("This is the content."); }
    void back() { Watchy::setScreen(&timeScreen); }
}

ContentScreen contentScreen;
```

```c++
//TimeScreen.h
#include "ContentScreen.h"
#include "Screen.h"
#include "Watchy.h"

class TimeScreen : public Screen {
    public:
    void show() { Watchy::display.print("This is the time."); }
    void menu() { Watchy::setScreen(&contentScreen); }
}

TimeScreen timeScreen;
```

And in the main program, make `TimeScreen` the initial screen on startup

```c++
// main.cpp
#include "TimeScreen.h"
#include "Watchy.h"  //include the Watchy library

void setup() {
  if (Watchy::screen == nullptr) { Watchy::screen = &timeScreen; }
  Watchy::init();
}

void loop() {} // this should never run, Watchy deep sleeps after init();
```

Manually coding the relationships between screens is tedious and error prone though. See `Menu` and `Carousel` below for a nicer approach.

## Watchy changes

* Watchy is no longer a class, you don't have to instantiate it. Now it's a namespace, which means that any references to Watchy need to be prefixed with `Watchy::`
* You need to call `Watchy::init` from your setup function.
* You need to initialize `Watchy::screen` on initial boot before calling `Watchy::init` (it's persistent after that)

### Additions to Watchy for Screens

* Use `Watchy::setScreen(Screen *s)` to change screens.
* Use `Watchy::showWatchFace(bool partialRefresh)` to redisplay the current screen.

There are some helper functions if you want to do polling UI instead of sleeping and waiting for an event. Normally you should just use the event driven UI, but look at `SetTime.{h,cpp}` and `UpdateFW.{h,cpp}` for examples of how to implement a captive UI. `Orientation.{h,cpp}` shows how to refresh the display more frequently than once a minute. Unfortunately all of those use a traditional polling loop, which is energy intensive. Eventually they should all be event driven.

### Caveats

Because of how deep sleep works, all screen instances must be globally persistent. The easiest way is to make each screen a global instance and let class initialization set them up. In particular the heap is not preserved across deep sleep, so you probably can't easily dynamically allocate screens.

The fields in a screen instance are not persistent across deep sleep. Do not expect to store persistent state in a screen instance. If you need persistence across sleep, make the state static and tag it with `RTC_DATA_ATTR` but this will only work for a single instance of a screen. If you have multiple instances of the same screen that need different persistent instance data you'll have do it some other way.

If you want multiple sub-screens for a single action you can instantiate local anonymous subclasses of `Screen` see `UpdateFW.cpp` for an example.

## Example Screens

The `Screens` directory includes individual screens for:

* Time/Date  
* Weather  
* Battery  
* Bluetooth Status
* Orientation
* Set Time - Set the current time and date
* Setup Wifi - Sets up Wifi credentials
* Update Firmware - Update the watch firmware Over-The-Air via bluetooth
* Number of Steps
* Wifi Status

And two "aggregate" screens used to organize other screens:

* Menu - Simple scrollable text menu.
* Carousel - A list of "splash" screens and their corresponding display screens.

## UI

### Menu

The menu screen implements a scrollable text menu. It takes an array of menu items, each consisting of a textual menu item name and corresponding screen to invoke. Adding new items to the menu is as simple as adding another `MenuItem` to the array in the call to the constructor. `up` and `down` navigate the menu, `menu` invokes the current item. When in a menu item's screen `back` takes you back to the menu. If there are more items than can be displayed, the menu will show scroll arrows at the top or bottom of the screen to indicate there are more items.

### Carousel

The carousel implements a cyclic view of spash screens and a corresponding display. Typically a splash screen is a simple static graphical or textual screen, and a display screen has more complex UI and active logic. To construct a carousel create an array of `CarouselItems` each one is a pair of screens the `splash` that is displayed in the carousel and the optional `child` that is displayed when you press `Menu` on the splash screen. The `child` screen is free to use any of the buttons for its own purposes, though conventionally the `back` button should return to the splash screen.

In a carousel `up` takes you to the previous splash screen, `down` takes you to the next one, `menu` shows the child screen for that splash screen, `back` while looking at a splash screen takes you back to the first screen of the carousel. A splash screen uses all of the buttons for its UI. In a child screen the child has complete control of the buttons, though by convention `back` should take you back to the carousel (the `parent` of the child screen). If the splash screen contains all information for the display and doesn't need any additional UI, the child screen can be null. To add another screen to a carousel, add another `CarouselItem` to the array passed to the `Carousel` constructor in the location you want the screen to appear in the carousel.

## Demo

The face is organized as a carousel of time, weather, battery, steps, orientation, bluetooth status, weather status, and a settings menu. The time and weather screens are standalone, they don't have a child display associated.

The settings display screen is a menu that lets you set the time, setup the wifi, update the firmware, synchronize the time with NTP, get your location by IP geolocation, refresh the weather, or buzz the motor.

There are no dependencies between the screens, all of the screen instances and their organization will be in `main.cpp` most of the screens are just a `show` method.

## Porting from Watchy

See the file `PORTING.md` for information on how to port a watchface from Watchy to Watchy-Screen.
