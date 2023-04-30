#include "BSECHandler.h"

BSECHandler::BSECHandler() : Logger("BSEC")
{
  delay(1000);
  hasError = false;

  iaqSensor = new Bsec();
  iaqSensor->begin(BME68X_I2C_ADDR_HIGH, Wire);

  log("BSEC library version " + String(iaqSensor->version.major) + "." + String(iaqSensor->version.minor) + "." + String(iaqSensor->version.major_bugfix) + "." + String(iaqSensor->version.minor_bugfix));

  checkIaqSensorStatus();

  if (hasError)
  {
    return;
  }

  bsec_virtual_sensor_t sensorList[13] = {
      BSEC_OUTPUT_IAQ,
      BSEC_OUTPUT_STATIC_IAQ,
      BSEC_OUTPUT_CO2_EQUIVALENT,
      BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
      BSEC_OUTPUT_RAW_TEMPERATURE,
      BSEC_OUTPUT_RAW_PRESSURE,
      BSEC_OUTPUT_RAW_HUMIDITY,
      BSEC_OUTPUT_RAW_GAS,
      BSEC_OUTPUT_STABILIZATION_STATUS,
      BSEC_OUTPUT_RUN_IN_STATUS,
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
      BSEC_OUTPUT_GAS_PERCENTAGE};

  iaqSensor->updateSubscription(sensorList, 13, BSEC_SAMPLE_RATE_LP);

  checkIaqSensorStatus();
}

BSECHandler::~BSECHandler()
{
  delete iaqSensor;
}

void BSECHandler::update()
{
  if (hasError)
  {
    return;
  }

  iaqSensor->run();

  checkIaqSensorStatus();
}

void BSECHandler::checkIaqSensorStatus()
{
  if (iaqSensor->bsecStatus != BSEC_OK)
  {
    if (iaqSensor->bsecStatus < BSEC_OK)
    {
      log("BSEC error code : " + String(iaqSensor->bsecStatus));
      hasError = true;
    }
    else
    {
      log("BSEC warning code : " + String(iaqSensor->bsecStatus));
    }
  }

  if (iaqSensor->bme68xStatus != BME68X_OK)
  {
    if (iaqSensor->bme68xStatus < BME68X_OK)
    {
      log("BME68X error code : " + String(iaqSensor->bme68xStatus));
      hasError = true;
    }
    else
    {
      log("BME68X warning code : " + String(iaqSensor->bme68xStatus));
    }
  }
}

BmeValues BSECHandler::getData()
{
  return {
      iaqSensor->staticIaq,
      iaqSensor->co2Equivalent,
      iaqSensor->breathVocEquivalent,
      iaqSensor->pressure,
      iaqSensor->temperature,
      iaqSensor->humidity,
      iaqSensor->gasPercentage,
  };
}