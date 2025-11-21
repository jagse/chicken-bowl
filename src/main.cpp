#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // LED on
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);   // LED off
    delay(5000);
}