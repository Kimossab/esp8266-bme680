#include "MQTT.h"

MQTT::MQTT(String host, int port, BMEHandler *_bme) : Logger("MQTT")
{
  wifiClient = new WiFiClient();
  mqttClient = new PubSubClient(*wifiClient);

  mqttClient->setBufferSize(516);

  mqttClient->setServer(host.c_str(), port);
  nextConnectionAttempt = 0; // 30 seconds
  connect();

  bme = _bme;
  lastUpdate = 0;
}

MQTT::~MQTT()
{
  mqttClient->~PubSubClient();
  wifiClient->~WiFiClient();
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
    String baseTopic = "homeassistant/sensor/" + String(SERIAL_NUMBER);

    StaticJsonDocument<256> doc;
    JsonObject object = doc.to<JsonObject>();
    BmeValue *sensorValues = bme->getLatest();
    object["temperature"] = sensorValues->temperature;

    char message[256];
    serializeJson(doc, message);

    log("Sending MQTT Message: ", true);
    log(message, true);
    emptyLine();
    mqttClient->publish((baseTopic + "/state").c_str(), message);

    lastUpdate = millis();
  }

  mqttClient->loop();
}

void MQTT::connect()
{
  unsigned long curTime = millis();
  if (!mqttClient->connected() && curTime >= nextConnectionAttempt)
  {
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
      emptyLine();

      log(String(curTime) + " > " + String(curTime + 30 * 1000));
      nextConnectionAttempt = curTime + 30 * 1000; // 5 seconds
    }
  }
}

void MQTT::publishConfiguration()
{
  String baseTopic = "homeassistant/sensor/" + String(SERIAL_NUMBER);

  StaticJsonDocument<256> doc;
  JsonObject object = doc.to<JsonObject>();
  object["device_class"] = "temperature",
  object["name"] = "TEST TEMPERATURE",
  object["state_class"] = "measurement",
  object["state_topic"] = baseTopic + "/state";
  object["unique_id"] = String(SERIAL_NUMBER) + "-T";
  object["unit_of_measurement"] = "°C";
  object["value_template"] = "{{ value_json.temperature}}";

  char temperatureConfig[256];
  serializeJson(doc, temperatureConfig);

  log("Publishing Configuration: " + baseTopic + "-T/config" + " | message: " + temperatureConfig);

  if (!mqttClient->publish((baseTopic + "-T/config").c_str(), temperatureConfig, 256))
  {
    log("Failed to publish. Reason=", true);
    log(String(mqttClient->state()));
    emptyLine();
  };
}