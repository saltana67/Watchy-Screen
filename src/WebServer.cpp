#include "WebServer.h"


#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

#include "Watchy.h"
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

    void onConfig(AsyncWebServerRequest *request) {
        /*
            {"name":"My ESPhome",
                "aps":[
                    {}
                    ,{"ssid":"Hermione","rssi":-50,"lock":0}
                    ,{"ssid":"Neville","rssi":-65,"lock":1}
                    ,{"ssid":"Gandalf the Grey","rssi":-85,"lock":1}
                    ,{"ssid":"Hagrid","rssi":-95,"lock":0}
                ]
            }
        */

        String jsonString= "{\"name\":\"AZ Home\",\"aps\":[{}]}";

        JSONVar responseJson;
        
        JSONVar empty = JSON.parse("{}");
        JSONVar aps;
        aps[0] = empty;
        JSONVar ap;
        
        responseJson["name"] = "AZ Home";
        
        log_d("scaning networks ... ");
        bool async = false;
        bool show_hidden = true;
        bool passive = true;
        uint32_t max_ms_per_chan = 50;
        int16_t err = WiFi.scanNetworks(async, show_hidden, passive, max_ms_per_chan);
        log_d("scaning networks ... done. %i", err);

        log_d("empty: %s", JSON.stringify(empty).c_str());
        log_d("empty aps: %s", JSON.stringify(aps).c_str());

        for( int16_t i = 0; i < err; i++ ) {
            String ssid = WiFi.SSID(i);
            int32_t rssi = WiFi.RSSI(i);
            wifi_auth_mode_t authmode = WiFi.encryptionType(i);
            uint8_t *bssid = WiFi.BSSID(i);
            int32_t channel = WiFi.channel(i);
            ap["ssid"] = ssid;
            ap["rssi"] = rssi;
            ap["lock"] = (authmode == WIFI_AUTH_OPEN ? 0 : 1);
            log_d("ap: %s", JSON.stringify(ap).c_str());
            aps[i+1] = ap;
        }
/*
        ap["ssid"] = "Hermione";ap["rssi"] = -50;ap["lock"] = 0;
        log_d("ap: %s", JSON.stringify(ap).c_str());
        aps[1] = ap;
        ap["ssid"] = "Neville";ap["rssi"] = -65;ap["lock"] = 1;
        aps[2] = ap;
        ap["ssid"] = "Gandalf the Grey";ap["rssi"] = -85;ap["lock"] = 1;
        aps[3] = ap;
        ap["ssid"] = "Hagrid";ap["rssi"] = -95;ap["lock"] = 0;
        aps[4] = ap;
*/
        log_d("aps: %s", JSON.stringify(aps).c_str());

        responseJson["aps"] = aps;

        jsonString = JSON.stringify(responseJson);
        log_d("jsonString: %s", jsonString.c_str());
        AsyncWebServerResponse *response = request->beginResponse(200, "application/json", jsonString);
        //response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    }


    void startServer(){
        log_d("startServer() called");

        Watchy::getWiFi();

        server.on("/", HTTP_ANY, onRoot);
        server.on("/index.html", HTTP_ANY, onRoot);
        server.on("/config.json", HTTP_ANY, onConfig);
        server.onNotFound(notFound);

        server.begin();
        running = true;
        
        while(running){
            //WiFi.localIP().toString();
            log_d("web server running @ %s ... ", WiFi.localIP().toString().c_str());
            sleep(5);
        }
    }

    void stopServer(){
        server.end();
        running = false;
        Watchy::releaseWiFi();
    }
}