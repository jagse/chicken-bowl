#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int HEATER_RELAY_PIN = 5;  // Digital pin for relay control
const int PUMP_PIN = 7;  // Digital pin for water pump control
const int TEMP_SENSOR_PIN = 3;  // Digital pin for DS18B20 temperature sensor
const float TEMP_TOO_LOW = 10;  
const float TEMP_TOO_HIGH = 15;  
const int TEN_SECONDS = 10000;  // 10 seconds in milliseconds

const unsigned long TEMP_READ_INTERVAL = TEN_SECONDS * 6;
unsigned long lastTempRead = 0;

OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

void heaterOn() {
  digitalWrite(HEATER_RELAY_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
}

void heaterOff() {
  digitalWrite(HEATER_RELAY_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

void pumpOn() {
  digitalWrite(PUMP_PIN, HIGH);
}

void pumpOff() {
  digitalWrite(PUMP_PIN, LOW);
}

float readTemperature() {
  tempSensor.requestTemperatures();
  return tempSensor.getTempCByIndex(0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(HEATER_RELAY_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  tempSensor.begin();
  heaterOff();  // Ensure heat pad is OFF at startup
  pumpOff();  // Ensure pump is OFF at startup
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastTempRead >= TEMP_READ_INTERVAL) {
    lastTempRead = currentMillis;
    float temp = readTemperature();
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");

    if (temp <= TEMP_TOO_LOW) {
      // heaterOn();
      Serial.println("Heater ON");
    } else if (temp >= TEMP_TOO_HIGH) {
      // heaterOff();
      Serial.println("Heater OFF");
    }
  }
}