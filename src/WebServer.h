#pragma once

#include "Events.h"

namespace Watchy_WebServer {
    extern Watchy_Event::BackgroundTask webServer;
    extern bool running;
    extern void startServer();
    extern void stopServer();
}