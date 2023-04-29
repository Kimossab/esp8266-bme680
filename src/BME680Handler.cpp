#include "BME680Handler.h"

BmeValue::BmeValue(float _temperature, double _pressure, float _humidity, double _gas, float _altitude, time_t _timestamp)
{
  temperature = _temperature;
  pressure = _pressure;
  humidity = _humidity;
  gas = _gas;
  altitude = _altitude;
  timestamp = _timestamp;
}
BmeValue::~BmeValue() {}

// PUBLIC
BMEHandler::BMEHandler() : Logger("BMEHandler")
{
  lastUpdate = 0;

  bme = new Adafruit_BME680(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

  if (!bme->begin())
  {
    log("Could not find a valid BME680 sensor, check wiring!");
  }
  else
  {
    log("BME begun!");
    bme->setHumidityOversampling(BME680_OS_2X);
    bme->setTemperatureOversampling(BME680_OS_8X);
    bme->setPressureOversampling(BME680_OS_4X);
    bme->setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme->setGasHeater(320, 150); // 320*C for 150 ms
  }
  history = new LinkedList<BmeValue>();
}
BMEHandler::~BMEHandler()
{
  delete bme;
}

BmeValue *BMEHandler::getLatest()
{
  return history->last()->getData();
}
void BMEHandler::update()
{
  unsigned long curTime = millis();
  if (!lastUpdate || curTime >= lastUpdate + SAMPLING_RATE)
  {
    lastUpdate = curTime;
    BmeValue *values = readValues();
    registerHistory(values);
    printValues();
  }
}
LinkedList<BmeValue> *BMEHandler::getHistory()
{
  return history;
}

DynamicJsonDocument BMEHandler::getSensorJson(SensorType sensor, unsigned long defaultTimestamp)
{
  DynamicJsonDocument doc(24576);

  JsonArray arrayData = doc.createNestedArray(stringify(sensor));
  JsonArray timestamp = doc.createNestedArray("timestamp");

  LinkedListNode<BmeValue> *node = history->find(
      [defaultTimestamp](BmeValue *d)
      { return d->timestamp == defaultTimestamp; });

  if (!node)
  {
    node = history->first();
  }
  else
  {
    node = node->getNext();
  }
  if (!node)
  {
    log("ERRROR!!! NO NODE FOUND");
  }

  while (node)
  {
    BmeValue *data = node->getData();
    if (!data)
    {
      log("ERRROR!!! DATA MISSING");
    }
    switch (sensor)
    {
    case temperature:
      arrayData.add(data->temperature);
      break;
    case pressure:
      arrayData.add(data->pressure);
      break;
    case humidity:
      arrayData.add(data->humidity);
      break;
    case gas:
      arrayData.add(data->gas);
      break;
    case altitude:
      arrayData.add(data->altitude);
      break;
    }

    timestamp.add(data->timestamp);

    node = node->getNext();
  }

  return doc;
}

// PRIVATE
BmeValue *BMEHandler::readValues()
{
  struct tm timeInfo;
  getLocalTime(&timeInfo);
  time_t timestamp = mktime(&timeInfo);

  bme->performReading();

  return new BmeValue(
      bme->temperature,
      bme->pressure / 100.0,
      bme->humidity,
      bme->gas_resistance / 1000.0,
      bme->readAltitude(SEALEVELPRESSURE_HPA),
      timestamp);
}

void BMEHandler::printValues()
{
  log("Temperature = " + String(getLatest()->temperature) + "*C");
  log("Pressure = " + String(getLatest()->pressure) + "hPa");
  log("Humidity = " + String(getLatest()->humidity) + "%");
  log("Gas = " + String(getLatest()->gas) + "KOhms");
  log("Approx. Altitude = " + String(getLatest()->altitude) + "m");
  emptyLine();
}

void BMEHandler::registerHistory(BmeValue *data)
{
  history->insertEnd(data);
  if (history->getCount() > MAX_HISTORY)
  {
    history->removeStart();
  }
}
