#include "MQTT.h"

MQTT::MQTT(String host, int _port, BSECHandler *_bme) : Logger("MQTT")
{
  bsec = _bme;
  lastUpdate = 0;
  domain = host;
  port = _port;

  wifiClient = new WiFiClient();
  mqttClient = new PubSubClient(*wifiClient);

  mqttClient->setBufferSize(516);

  nextConnectionAttempt = 0; // 30 seconds
  connect();

  log("initialized");
}

MQTT::~MQTT()
{
  delete wifiClient;
}

void MQTT::update()
{
  if (!mqttClient->connected())
  {
    connect();
    return;
  }

  if (millis() > lastUpdate + MQTT_UPDATE_RATE)
  {

    StaticJsonDocument<516> doc;
    JsonObject object = doc.to<JsonObject>();
    BmeValues sensorValues = bsec->getData();
    object["temperature"] = sensorValues.temperature;
    object["humidity"] = sensorValues.humidity;
    object["pressure"] = sensorValues.pressure;
    object["iaq"] = sensorValues.iaq;
    object["co2"] = sensorValues.co2;

    char message[516];
    serializeJson(doc, message);

    log("Sending MQTT Message: ", true);
    log(message, true);
    emptyLine();
    mqttClient->publish(STATE_TOPIC, message);

    log("published");
    lastUpdate = millis();
  }

  mqttClient->loop();
}

void MQTT::connect()
{
  unsigned long curTime = millis();
  if (!mqttClient->connected() && curTime >= nextConnectionAttempt)
  {
    mqttClient->setServer(domain.c_str(), port);
    log("Attempting to connect to MQTT broker");

    if (mqttClient->connect(SERIAL_NUMBER))
    {
      log("Connected");

      publishConfiguration();
      mqttClient->subscribe("#");
    }
    else
    {
      log("Failed to connect. Reason=", true);
      log(String(mqttClient->state()), true);

      nextConnectionAttempt = curTime + 30 * 1000; // 5 seconds
    }
  }
}

void MQTT::publishConfiguration()
{
  String tempUniqueId = SERIAL_NUMBER "-T";
  String humUniqueId = SERIAL_NUMBER "-H";
  String pressUniqueId = SERIAL_NUMBER "-P";
  String iaqUniqueId = SERIAL_NUMBER "-IAQ";
  String co2UniqueId = SERIAL_NUMBER "-CO2";

  String tempBaseTopic = BASE_TOPIC + tempUniqueId;
  String humBaseTopic = BASE_TOPIC + humUniqueId;
  String pressBaseTopic = BASE_TOPIC + pressUniqueId;
  String iaqBaseTopic = BASE_TOPIC + iaqUniqueId;
  String co2BaseTopic = BASE_TOPIC + co2UniqueId;

  char config[256];
  StaticJsonDocument<256> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();

  // TEMPERATURE
  jsonObject["device_class"] = "temperature",
  jsonObject["name"] = "TEST TEMPERATURE",
  jsonObject["state_class"] = "measurement",
  jsonObject["state_topic"] = STATE_TOPIC;
  jsonObject["unique_id"] = tempUniqueId;
  jsonObject["unit_of_measurement"] = "°C";
  jsonObject["value_template"] = "{{ value_json.temperature }}";
  serializeJson(jsonDoc, config);

  log("Publishing Configuration: " + tempBaseTopic + "/config" + " | message: " + config);
  if (!mqttClient->publish((tempBaseTopic + "/config").c_str(), config, 256))
  {
    log("Failed to publish. Reason=", true);
    log(String(mqttClient->state()), true);
    emptyLine();
  };

  // HUMIDITY
  jsonObject["device_class"] = "humidity",
  jsonObject["name"] = "TEST HUMIDITY",
  jsonObject["state_class"] = "measurement",
  jsonObject["state_topic"] = STATE_TOPIC;
  jsonObject["unique_id"] = humUniqueId;
  jsonObject["unit_of_measurement"] = "%";
  jsonObject["value_template"] = "{{ value_json.humidity }}";
  serializeJson(jsonDoc, config);

  log("Publishing Configuration: " + humBaseTopic + "/config" + " | message: " + config);
  if (!mqttClient->publish((humBaseTopic + "/config").c_str(), config, 256))
  {
    log("Failed to publish. Reason=", true);
    log(String(mqttClient->state()), true);
    emptyLine();
  };

  // PRESSURE
  jsonObject["device_class"] = "pressure",
  jsonObject["name"] = "TEST PRESSURE",
  jsonObject["state_class"] = "measurement",
  jsonObject["state_topic"] = STATE_TOPIC;
  jsonObject["unique_id"] = pressUniqueId;
  jsonObject["unit_of_measurement"] = "hPa";
  jsonObject["value_template"] = "{{ value_json.pressure }}";
  serializeJson(jsonDoc, config);

  log("Publishing Configuration: " + pressBaseTopic + "/config" + " | message: " + config);
  if (!mqttClient->publish((pressBaseTopic + "/config").c_str(), config, 256))
  {
    log("Failed to publish. Reason=", true);
    log(String(mqttClient->state()), true);
    emptyLine();
  };

  // IAQ
  jsonObject["device_class"] = "aqi",
  jsonObject["name"] = "TEST IAQ",
  jsonObject["state_class"] = "measurement",
  jsonObject["state_topic"] = STATE_TOPIC;
  jsonObject["unique_id"] = iaqUniqueId;
  jsonObject["unit_of_measurement"] = "";
  jsonObject["value_template"] = "{{ value_json.iaq }}";
  serializeJson(jsonDoc, config);

  log("Publishing Configuration: " + iaqBaseTopic + "/config" + " | message: " + config);
  if (!mqttClient->publish((iaqBaseTopic + "/config").c_str(), config, 256))
  {
    log("Failed to publish. Reason=", true);
    log(String(mqttClient->state()), true);
    emptyLine();
  };

  // CO2
  jsonObject["device_class"] = "carbon_dioxide",
  jsonObject["name"] = "TEST CO2",
  jsonObject["state_class"] = "measurement",
  jsonObject["state_topic"] = STATE_TOPIC;
  jsonObject["unique_id"] = co2UniqueId;
  jsonObject["unit_of_measurement"] = "ppm";
  jsonObject["value_template"] = "{{ value_json.co2 }}";
  serializeJson(jsonDoc, config);

  log("Publishing Configuration: " + co2BaseTopic + "/config" + " | message: " + config);
  if (!mqttClient->publish((co2BaseTopic + "/config").c_str(), config, 256))
  {
    log("Failed to publish. Reason=", true);
    log(String(mqttClient->state()), true);
    emptyLine();
  };
}