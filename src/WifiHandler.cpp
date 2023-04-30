#include "WifiHandler.h"

// public
WiFiHandler::WiFiHandler(BSECHandler *sensor) : Logger("WifiHandler")
{
  bsec = sensor;
  server = new ESP8266WebServer(80);

  // WIFI
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  String wifiSsid = WiFi.SSID(getBestWifi());

  WiFi.begin(wifiSsid, WIFI_PASSWORD);

  log("Connecting to " + wifiSsid);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    log(".", true);
  }
  log("IP: " + WiFi.localIP().toString());
  emptyLine();

  // Web server
  server->on(
      "/",
      [this]()
      { this->handleHomePage(); });
  server->onNotFound(
      [this]()
      { server->send(404, "text/plain", "Not found"); });

  server->begin();

  // NTP
  configTime("WET0WEST,M3.5.0/1,M10.5.0", "europe.pool.ntp.org", "pool.ntp.org", "");
}

void WiFiHandler::update()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    server->handleClient();
  }
  else
  {
    log("WiFi Disconnected!!!");
    log("Trying to establish the connection...", false);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      log(".", false);
    }
    emptyLine();
    log("IP: " + WiFi.localIP().toString());
    emptyLine();
  }
}

// private
int WiFiHandler::getBestWifi()
{
  log("Start Scanning - please stand-by");

  int n = WiFi.scanNetworks();

  if (n == 0)
  {
    log("No Networks found.");
    return -1;
  }

  log("Networks found: " + String(n));

  int bestWifi = -1;
  int bestRssi = -99;

  for (int i = 0; i < n; ++i)
  {
    String ssid = WiFi.SSID(i);
    int rssi = WiFi.RSSI(i);

    if (!ssid.startsWith(WIFI_NAME))
    {
      continue;
    }

    if (rssi > bestRssi)
    {
      bestWifi = i;
      bestRssi = rssi;
    }

    delay(10);
  }

  if (bestWifi >= 0)
  {
    log("Best network: " + WiFi.SSID(bestWifi));
    return bestWifi;
  }

  log("No good network found");
  return -1;
}

void WiFiHandler::handleHomePage()
{
  BmeValues latestValue = bsec->getData();

  String html =
      "<!DOCTYPE html>"
      "<html>"
      "<head>"
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
      "<link rel=\"icon\" href=\"data:,\">"
      "<style>"
      "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}"
      ".sensor { display: grid; grid-template-columns: 1fr auto; margin: 0 auto; gap: 1rem; width: fit-content; }"
      ".name { text-align: right; }"
      ".value { text-align: left; }"
      "canvas { max-width: 720px; margin:0 auto; height: 400px; }"
      "</style>"
      "<script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>"
      "</head>"
      "<body>"
      "<h1>ESP8266 Web Server</h1>"
      "<div class=\"sensor\">"
      "<div class=\"name\">Temperature:</div>"
      "<div class=\"value\">" +
      String(latestValue.temperature, 2) +
      "</div>"
      "<div class=\"name\">Humidity:</div>"
      "<div class=\"value\">" +
      String(latestValue.humidity, 2) +
      "</div>"
      "<div class=\"name\">Pressure:</div>"
      "<div class=\"value\">" +
      String(latestValue.pressure, 2) +
      "</div>"
      "<div class=\"name\">IAQ:</div>"
      "<div class=\"value\">" +
      String(latestValue.iaq, 2) +
      "</div>"
      "</div>"
      "<canvas id=\"myChart\"></canvas>"
      "<script>"
      "let latestRecord = 0;"
      ""
      "const ctx = document.getElementById('myChart');"
      "const chart = new Chart(ctx, {"
      "  type: 'line', data: {"
      "    labels: [],"
      "    datasets: [{"
      "      label: 'temperature',"
      "      data: []"
      "    }]"
      "  }"
      "});"
      "</script>"
      "</body>"
      "</html>";

  server->send(200, "text/html", html);
}

String WiFiHandler::getArgByName(String name)
{
  for (int i = 0; i < server->args(); i++)
  {
    if (server->argName(i) == name)
    {
      return server->arg(i);
    }
  }

  return "";
}