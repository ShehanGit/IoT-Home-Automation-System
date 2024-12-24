#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>
#include <Arduino.h>
#include <esp32-hal-ledc.h>
#include "Config.h"
#include "WebPage.h"
#include "SinricProConfig.h"

AsyncWebServer server(80);
TaskHandle_t knightRiderTaskHandle = NULL;

// Global variables for color sync
uint8_t lastR = 0, lastG = 0, lastB = 0;
unsigned long lastSampleTime = 0;
const unsigned long SAMPLE_INTERVAL = 100;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }
  Serial.println("\n\n=== ESP32 Starting up... ===");
  
  // Initialize pins
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);

  // Set initial relay states
  digitalWrite(RELAY1_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
  digitalWrite(RELAY2_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
  digitalWrite(RELAY3_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
  digitalWrite(RELAY4_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);

  setupRGBLED();
  setupSerial();
  initializeWiFi();
  initializeOTA();
  initializeWebServer();
  setupSinricPro();

  Serial.println("Setup complete");
}

void loop() {
  ArduinoOTA.handle();
  // SinricPro.handle();
  handleSerialCommands();
  delay(10);
}