// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

#include "src/RNDShutterController.h" 
#include "credentials/Credentials.h"
#include "frontend/index.h"

const unsigned int TRANSMIT_PIN = 1;
// Set web server port number to 80
AsyncWebServer server(80);
// TODO redo this
RNDShutterController controller(TRANSMIT_PIN);

const char* command_param = "command";// String type input
const char* shutter_scale_param = "shutter_scale";// String type input

const char* living_room_door_param = "living_room_door";// String type input
const char* living_room_window_param = "living_room_window";// String type input
const char* bedroom_door_param = "bedroom_door";// String type input
const char* bedroom_window_param = "bedroom_window";// String type input

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() 
{
  //GPIO 1 (TX) swap the pin to a GPIO.
  // TODO redo thiss
  pinMode(TRANSMIT_PIN, FUNCTION_3); 
  //Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(TRANSMIT_PIN, OUTPUT);

  // Connect to Wi-Fi network with SSID and password
  WiFi.begin(Credentials::ssid, Credentials::password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

    // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_page);
  });

   // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    //int paramsNr = request->params();
    if (request->hasParam(command_param)) {
      // Normal motion command
      String command = request->getParam(command_param)->value();
      ShutterCommand normal_command = controller.decodeCommand(command);
      controller.addCommand(normal_command);
    }
    else if (request->hasParam(shutter_scale_param))
    {
      // Absolute motion command
      String position_str = request->getParam(shutter_scale_param)->value();
      const size_t param_num = request->params();
      for (size_t param_id = 0; param_id < param_num; param_id++)
      {
        AsyncWebParameter* p = request->getParam(param_id);
        if(p->name() == shutter_scale_param)
        {
          continue;
        }
        ShutterCommand absolute_command = controller.decodeAbsoluteCommand(p->name(), position_str);
        controller.addCommand(absolute_command);

      }
    }
    request->send_P(200, "text/html", index_page);
  });

  server.onNotFound(notFound);

  // Start web server
  server.begin();
}

void loop(){
  controller.execute();
}
