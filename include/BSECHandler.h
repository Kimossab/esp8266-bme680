#ifndef bsec_handler_h__
#define bsec_handler_h__

#include "Logger.h"
#include "bsec.h"

struct BmeValues
{
  float iaq;
  float co2;
  float breathVoc;
  float pressure;
  float temperature;
  float humidity;
  float gasPercentage;
};

class BSECHandler : Logger
{
public:
  BSECHandler();
  ~BSECHandler();
  void update();
  BmeValues getData();

private:
  Bsec *iaqSensor;
  bool hasError;
  void checkIaqSensorStatus();
};

#endif