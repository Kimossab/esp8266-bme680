
#include <Arduino.h>
#include "BSECHandler.h"
#include "WifiHandler.h"
#include "MQTT.h"
#include <Esp.h>

#define NTP_ADDRESS "europe.pool.ntp.org"

BSECHandler *bsec;
WiFiHandler *wifi;
MQTT *mqtt;

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  bsec = new BSECHandler();
  wifi = new WiFiHandler(bsec);
  mqtt = new MQTT("192.168.1.65", 1883, bsec);
  ESP.wdtEnable(1000);
}

void loop()
{
  bsec->update();
  wifi->update();
  mqtt->update();
  ESP.wdtFeed();
}
