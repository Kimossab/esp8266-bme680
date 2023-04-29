#ifndef bme680_handler_h__
#define bme680_handler_h__

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <ArduinoJson.h>
#include "LinkedList.h"
#include "Logger.h"

#define BME_SCK 14  // scl d5
#define BME_MISO 12 // sdo d6
#define BME_MOSI 13 // sda d7
#define BME_CS 15   // cs d8
#define SEALEVELPRESSURE_HPA (1013.25)

#define SAMPLING_RATE 5000 // 5000 ms = 5 s
#define MAX_HISTORY 150

#define stringify(name) #name

enum SensorType
{
  temperature,
  humidity,
  pressure,
  gas,
  altitude
};

class BmeValue
{
public:
  BmeValue(float = 0, double = 0, float = 0, double = 0, float = 0, time_t = 0);
  ~BmeValue();
  float temperature;
  double pressure;
  float humidity;
  double gas;
  float altitude;
  time_t timestamp;
};

class BMEHandler : Logger
{
public:
  BMEHandler();
  ~BMEHandler();
  BmeValue *getLatest();
  void update();
  LinkedList<BmeValue> *getHistory();
  DynamicJsonDocument getSensorJson(SensorType, unsigned long = 0);

private:
  unsigned long lastUpdate;
  Adafruit_BME680 *bme;
  BmeValue *readValues();
  void printValues();
  void registerHistory(BmeValue *);
  LinkedList<BmeValue> *history;
};

#endif