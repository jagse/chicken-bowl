#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int HEATER_PIN = 5;  // Digital pin for relay control
const int PUMP_PIN = 7;  // Digital pin for water pump control
const int TEMP_SENSOR_PIN = 3;  // Digital pin for DS18B20 temperature sensor
const float TEMP_TOO_LOW = 8;
const float TEMP_TOO_HIGH = 12;  
const int TEN_SECONDS = 10000;  // 10 seconds in milliseconds

const unsigned long TEMP_READ_INTERVAL = 60000;  // Read temperature every 60 seconds
unsigned long lastTempRead = 0;

// Display update interval
const unsigned long DISPLAY_UPDATE_INTERVAL = 1000;  // Update display every 1 second
unsigned long lastDisplayUpdate = 0;

// Pump configuration
const unsigned long PUMP_INTERVAL = 300000; //3600000;  // How often to pump in millis (1 hour)
const unsigned long PUMP_DURATION = TEN_SECONDS;  // How long to pump each time in millis
unsigned long lastPumpStart = 0;
bool pumpRunning = false;

OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float currentTemp = 0.0;
bool heaterState = false;

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Display temperature with countdown
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(currentTemp, 1);
  display.print("C -> ");

  unsigned long timeSinceTempRead = millis() - lastTempRead;
  unsigned long timeUntilNextRead = TEMP_READ_INTERVAL - timeSinceTempRead;
  unsigned long secondsUntilNextRead = timeUntilNextRead / 1000;
  unsigned long minutesUntilNextRead = secondsUntilNextRead / 60;
  unsigned long remainingSecondsTemp = secondsUntilNextRead % 60;

  if (minutesUntilNextRead > 0) {
    display.print(minutesUntilNextRead);
    display.print("m ");
  }
  display.print(remainingSecondsTemp);
  display.print("s");

  // Display pump state with countdown
  display.setCursor(0, 12);
  display.print("Pump: ");
  display.print(pumpRunning ? "ON" : "OFF");

  if (!pumpRunning) {
    display.print(" -> ");
    unsigned long timeSinceStart = millis() - lastPumpStart;
    unsigned long timeUntilNext = PUMP_INTERVAL - timeSinceStart;
    unsigned long secondsUntilNext = timeUntilNext / 1000;
    unsigned long minutesUntilNext = secondsUntilNext / 60;
    unsigned long remainingSeconds = secondsUntilNext % 60;

    if (minutesUntilNext > 0) {
      display.print(minutesUntilNext);
      display.print("m ");
    }
    display.print(remainingSeconds);
    display.print("s");
  }

  display.setCursor(0, 24);
  display.print("Heater: ");
  display.print(heaterState ? "ON" : "OFF");

  display.display();
}

void heaterOn() {
  digitalWrite(HEATER_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  heaterState = true;
}

void heaterOff() {
  digitalWrite(HEATER_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  heaterState = false;
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

    // Re-initialize display after MOSFET switching
    delay(50);
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();

  } else if (pumpRunning && (currentMillis - lastPumpStart >= PUMP_DURATION)) {
    // Stop pumping after duration
    pumpOff();
    pumpRunning = false;
    Serial.println("Pump stopped");

    // Re-initialize display after MOSFET switching
    delay(50);
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();

  }
}

void runHeating(unsigned long currentMillis) {
  if (currentMillis - lastTempRead >= TEMP_READ_INTERVAL) {
    lastTempRead = currentMillis;
    currentTemp = readTemperature();
    Serial.print("Temperature: ");
    Serial.print(currentTemp);
    Serial.println(" °C");

    if (currentTemp <= TEMP_TOO_LOW) {
      heaterOn();
      Serial.println("Heater ON");
    } else if (currentTemp >= TEMP_TOO_HIGH) {
      heaterOff();
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

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();

  // Show initial reading
  currentTemp = readTemperature();
  updateDisplay();
}

void loop() {
  unsigned long currentMillis = millis();

  runPump(currentMillis);
  runHeating(currentMillis);

  // Update display at regular intervals
  if (currentMillis - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
    lastDisplayUpdate = currentMillis;
    updateDisplay();
  }
}