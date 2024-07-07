// Copyright © 2023 Robert Takacs
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "shutter_controller.h" 
#include "../credentials/credentials.h"

// Define the macro to disable transmission, and to enable printing to Serial.
// #define DEBUG

const unsigned int TRANSMIT_PIN = 1;
// Set web server port number to 80
AsyncWebServer server(80);
ShutterController controller(TRANSMIT_PIN);

const char* command_param = "command";
const char* shutter_scale_param = "shutter_scale";

const char* living_room_door_param = "living_room_door";
const char* living_room_window_param = "living_room_window";
const char* bedroom_door_param = "bedroom_door";
const char* bedroom_window_param = "bedroom_window";

unsigned long prev_exec_time_ms = 0;
unsigned long exec_period_ms = 20; 


void notFound(AsyncWebServerRequest *request) 
{
    request->send(404, "text/plain", "Not found");
}


void setup() 
{
#ifdef DEBUG
    Serial.begin(9600);
#endif
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

    if (!LittleFS.begin())
    {
        return;
    }

    // Connect to Wi-Fi network with SSID and password
    WiFi.begin(Credentials::ssid.c_str(), Credentials::password.c_str());
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
          { request->send(LittleFS, "/index.html", "text/html"); });

    // Route for root index.css
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/style.css", "text/css"); });


    // Route for root index.js
    server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.js", "text/javascript"); }); 

    server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    if (request->url() == "/api/calibrate") 
    {
        JsonDocument  ret;
        deserializeJson(ret, data);
        if (ret.isNull()) 
        {
            notFound(request);
        }
        if (!ret.containsKey("calibrate")) 
        {
            notFound(request);
        }
        controller.createCalibrationCommand(ret["calibrate"]);
        request->send(200);
    }
    });

    // Send a GET request to <ESP_IP>/get?xy
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) 
    {
        if (request->hasParam(command_param)) 
        {
            // Normal motion command
            String command = request->getParam(command_param)->value();
            controller.createRelativeCommand(command);
        }
        else if (request->hasParam(shutter_scale_param))
        {
        // Absolute motion command
        String position_str = request->getParam(shutter_scale_param)->value();
        const size_t param_num = request->params();
        for (size_t param_id = 0; param_id < param_num; param_id++)
        {
            AsyncWebParameter* p = request->getParam(param_id);
            if (p->name() == shutter_scale_param)
            {
                continue;
            }
            controller.createAbsoluteCommand(p->name(), position_str);
      }
    }
        request->send(LittleFS, "/index.html", "text/html");
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop()
{
    const auto time_ms = millis();
    if (time_ms - prev_exec_time_ms > exec_period_ms)
    {
        controller.execute();
        prev_exec_time_ms = time_ms;
    }
}
