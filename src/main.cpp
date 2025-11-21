#include <Arduino.h>

const int HEATER_RELAY_PIN = 5;  // Digital pin for relay control

void heaterOn() {
  digitalWrite(HEATER_RELAY_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
}

void heaterOff() {
  digitalWrite(HEATER_RELAY_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(HEATER_RELAY_PIN, OUTPUT);
  heaterOff();  // Ensure heat pad is OFF at startup
}

void loop() {
  heaterOff();
  delay(20000);
  heaterOn();
  delay(20000);
}