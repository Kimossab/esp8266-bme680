
#include <Arduino.h>
#include "BME680Handler.h"
#include "WifiHandler.h"

#define NTP_ADDRESS "europe.pool.ntp.org"

WiFiHandler *wifi;
BMEHandler *bme;

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  bme = new BMEHandler();
  wifi = new WiFiHandler(bme);
}

void loop()
{
  bme->update();
  wifi->update();

  // delay(5000);
}
