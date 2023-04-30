#ifndef mqtt_h__
#define mqtt_h__

#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "Logger.h"
#include "BSECHandler.h"

#define SERIAL_NUMBER "KimoTest-0001"
#define BASE_TOPIC "homeassistant/sensor/"
#define STATE_TOPIC BASE_TOPIC SERIAL_NUMBER "/state"

#define MQTT_UPDATE_RATE 5000

class MQTT : Logger
{
public:
  MQTT(String host, int port, BSECHandler *_bsec);
  ~MQTT();
  void update();

private:
  WiFiClient *wifiClient;
  PubSubClient *mqttClient;
  BSECHandler *bsec;
  unsigned long nextConnectionAttempt;
  unsigned long lastUpdate;
  void connect();
  void publishConfiguration();
  String domain;
  int port;
};

#endif