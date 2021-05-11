#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include <array>

#define SSID "myssid"
#define PASSWORD "mypass"

// r g b w
std::array<int, 4> pins = {D1, D2, D5, D6};

void reset() {
  for (const auto &pin : pins) {
    digitalWrite(pin, LOW);
  }
}

void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0) {
  std::array<uint8_t, 4> values = {r, g, b, w};

  reset();

  for (int i = 0; i < 4; i++) {
    analogWrite(pins[i], values[i] * 1023 / 255);
  }
}

void setup() {
  Serial.begin(115200);
  delay(5000);

  for (const auto &pin : pins) {
    pinMode(pin, OUTPUT);
  }
}

void loop() {
  if (digitalRead(D7) == HIGH) {
    setColor(255, 255, 255, 255);
  } else {
    reset();
  }
}