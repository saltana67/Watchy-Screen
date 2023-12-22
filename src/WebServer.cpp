#include "WebServer.h"


#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <Watchy.h>
#include "captive_index_gz.h"

namespace Watchy_WebServer {
    
    Watchy_Event::BackgroundTask webServer("webServer", []() {
      startServer();
    },ESP_PRO_CORE);

    AsyncWebServer server(80);
    bool running = false;

    void notFound(AsyncWebServerRequest *request) {
        log_d("notFound");
        request->send(404, "text/plain", "Not found");
    }

    void onRoot(AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", INDEX_GZ, sizeof(INDEX_GZ));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    }

    void startServer(){
        log_d("startServer() called");

        Watchy::getWiFi();

        server.on("/", HTTP_ANY, onRoot);
        server.on("/index.html", HTTP_ANY, onRoot);
        server.onNotFound(notFound);

        server.begin();
        running = true;
        
        while(running){
            log_d("web server running ...");
            sleep(5);
        }
    }

    void stopServer(){
        server.end();
        running = false;
        Watchy::releaseWiFi();
    }
}