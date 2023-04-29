#ifndef mqtt_h__
#define mqtt_h__

#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "Logger.h"
#include "BME680Handler.h"

#define SERIAL_NUMBER "KimoTest-0001"

#define MQTT_UPDATE_RATE 5000

class MQTT : Logger
{
public:
  MQTT(String host, int port, BMEHandler *_bme);
  ~MQTT();
  void update();
  void static callback(char *topic, byte *payload, unsigned int length);

private:
  WiFiClient *wifiClient;
  PubSubClient *mqttClient;
  BMEHandler *bme;
  unsigned long nextConnectionAttempt;
  unsigned long lastUpdate;
  void connect();
  void publishConfiguration();
  String domain;
  uint16_t port;
};

#endif