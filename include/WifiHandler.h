#ifndef wifi_handler_h__
#define wifi_handler_h__

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <time.h>
#include <ArduinoJson.h>
#include <string>
#include "BME680Handler.h"
#include "Logger.h"
#include "BufferedPrintResponse.h"

#define WIFI_NAME "XXXXXX"
#define WIFI_PASSWORD "XXXXXX"

class WiFiHandler : Logger
{
public:
  WiFiHandler(BMEHandler *bme);
  void update();

private:
  ESP8266WebServer *server;

  int getBestWifi();
  void handleHomePage();
  void handleGetSensor(SensorType);
  String getArgByName(String name);
  BMEHandler *bme;
};

#endif