#ifndef wifi_handler_h__
#define wifi_handler_h__

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <time.h>
#include <ArduinoJson.h>
#include <string>
#include "BSECHandler.h"
#include "Logger.h"
#include "BufferedPrintResponse.h"

#define WIFI_NAME "MEO-45E4F0"
#define WIFI_PASSWORD "bbbe59a2ae"

class WiFiHandler : Logger
{
public:
  WiFiHandler(BSECHandler *bsec);
  void update();

private:
  ESP8266WebServer *server;

  int getBestWifi();
  void handleHomePage();
  String getArgByName(String name);
  BSECHandler *bsec;
};

#endif