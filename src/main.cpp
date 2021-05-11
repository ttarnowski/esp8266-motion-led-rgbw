#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include <EventDispatcher.hpp>
#include <LittleFS.h>
#include <Timer.hpp>
#include <WiFiManager.hpp>
#include <array>

#define SSID "VM3549886"
#define PASSWORD "mc7RsdnxV4qp"

#define JSON_SIZE 256
ESP8266WebServer server(80);
Timer timer;
EventDispatcher dispatcher;
ESP8266WiFiMulti wifiMulti;

WiFiManager wifiManager(&wifiMulti, &dispatcher, &timer, SSID, PASSWORD);

// r g b w
std::array<int, 4> pins = {D1, D2, D5, D6};

const char *contentType = "application/json";

class Color {
public:
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t w;

  void fromJSONString(const char *jsonString) {
    StaticJsonDocument<JSON_SIZE> doc;
    deserializeJson(doc, jsonString);

    this->r = doc["r"];
    this->g = doc["g"];
    this->b = doc["b"];
    this->w = doc["w"];
  }

  void toJSONString(char *buff) {
    char jsonString[JSON_SIZE];
    StaticJsonDocument<JSON_SIZE> doc;

    doc["r"] = this->r;
    doc["g"] = this->g;
    doc["b"] = this->b;
    doc["w"] = this->w;

    serializeJson(doc, jsonString);
    strcpy(buff, jsonString);
  }
};

Color color;

void reset() {
  for (const auto &pin : pins) {
    digitalWrite(pin, LOW);
  }
}

void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0) {
  std::array<uint8_t, 4> values = {r, g, b, w};

  reset();

  for (int i = 0; i <= 4; i++) {
    analogWrite(pins[i], values[i] * 1023 / 255);
  }
}

const char *configFileName = "config.json";

void readConfig() {
  File file = LittleFS.open(configFileName, "r");
  if (!file) {
    Serial.println("could not open config file for read");
    return;
  }

  color.fromJSONString(file.readString().c_str());
  file.close();
}

void writeConfig() {
  File file = LittleFS.open(configFileName, "w");
  if (!file) {
    Serial.println("could not open config file for write");
    return;
  }

  char content[JSON_SIZE];
  color.toJSONString(content);

  file.write((const char *)content);
  file.close();
}

void setup() {
  Serial.begin(115200);
  delay(5000);
  LittleFS.begin();
  readConfig();

  for (const auto &pin : pins) {
    pinMode(pin, OUTPUT);
  }

  server.on("/color", HTTPMethod::HTTP_ANY, []() {
    char body[256];

    switch (server.method()) {
    case HTTPMethod::HTTP_PUT:
      server.arg("plain").toCharArray(body, sizeof(body) - 1);

      color.fromJSONString(body);

      writeConfig();

      server.send(200, contentType, "{\"message\":\"ok\"}");
      break;

    case HTTPMethod::HTTP_GET:
      color.toJSONString(body);

      server.send(200, contentType, body);
      break;

    default:
      server.send(405, contentType, "{\"message\":\"invalid method\"}");
    }
  });

  server.serveStatic("/", LittleFS, "/");

  wifiManager.connect([](wl_status_t status) {
    if (status != WL_CONNECTED) {
      Serial.println("could not connect to wifi");
      return;
    }

    server.begin();

    Serial.printf("listening on http://%s:80\n",
                  WiFi.localIP().toString().c_str());

    timer.setOnLoop([]() { server.handleClient(); });
  });

  timer.setOnLoop([]() {
    if (digitalRead(D7) == HIGH) {
      setColor(color.r, color.g, color.b, color.w);
    } else {
      reset();
    }
  });
}

void loop() { timer.tick(); }