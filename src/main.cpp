
#include <Arduino.h>
#include "BME680Handler.h"
#include "WifiHandler.h"
#include "MQTT.h"

#define NTP_ADDRESS "europe.pool.ntp.org"

WiFiHandler *wifi;
BMEHandler *bme;
MQTT *mqtt;

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  bme = new BMEHandler();
  wifi = new WiFiHandler(bme);
  mqtt = new MQTT("192.168.1.65", 1883, bme);
}

void loop()
{
  bme->update();
  wifi->update();
  mqtt->update();

  // delay(5000);
}
