#include <Arduino.h>

const int HEATER_RELAY_PIN = 5;  // Digital pin for relay control

void heaterOn() {
  digitalWrite(HEATER_RELAY_PIN, HIGH);
}

void heaterOff() {
  digitalWrite(HEATER_RELAY_PIN, LOW);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(HEATER_RELAY_PIN, OUTPUT);
  heaterOff();  // Ensure heat pad is OFF at startup
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // LED on
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);   // LED off
  delay(5000);
}