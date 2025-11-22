#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int HEATER_PIN = 5;  // Digital pin for relay control
const int PUMP_PIN = 7;  // Digital pin for water pump control
const int TEMP_SENSOR_PIN = 3;  // Digital pin for DS18B20 temperature sensor
const float TEMP_TOO_LOW = 10;  
const float TEMP_TOO_HIGH = 15;  
const int TEN_SECONDS = 10000;  // 10 seconds in milliseconds

const unsigned long TEMP_READ_INTERVAL = TEN_SECONDS * 6;
unsigned long lastTempRead = 0;

// Pump configuration
const unsigned long PUMP_INTERVAL = TEN_SECONDS * 6;  // How often to pump in millis
const unsigned long PUMP_DURATION = TEN_SECONDS;  // How long to pump each time in millis
unsigned long lastPumpStart = 0;
bool pumpRunning = false;

OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

void heaterOn() {
  digitalWrite(HEATER_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
}

void heaterOff() {
  digitalWrite(HEATER_PIN, LOW);
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

void runPump(unsigned long currentMillis) {
  if (!pumpRunning && (currentMillis - lastPumpStart >= PUMP_INTERVAL)) {
    // Start pumping
    pumpOn();
    pumpRunning = true;
    lastPumpStart = currentMillis;
    Serial.println("Pump started");
  } else if (pumpRunning && (currentMillis - lastPumpStart >= PUMP_DURATION)) {
    // Stop pumping after duration
    pumpOff();
    pumpRunning = false;
    Serial.println("Pump stopped");
  }
}

void runHeating(unsigned long currentMillis) {
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(HEATER_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  tempSensor.begin();
  heaterOff();  // Ensure heat pad is OFF at startup
  pumpOff();  // Ensure pump is OFF at startup
}

void loop() {
  unsigned long currentMillis = millis();

  runPump(currentMillis);
  runHeating(currentMillis);
}