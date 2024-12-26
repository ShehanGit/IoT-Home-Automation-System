#include "Config.h" // Include the relay configuration macros
#include <WebServer.h>
#include "WebPage.h"

void setup() {
    setupSerial();
    setupRGBLED();
    setupDHT();  
    setupWiFi();

    // Initialize relay pins
    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);
    pinMode(RELAY3_PIN, OUTPUT);
    pinMode(RELAY4_PIN, OUTPUT);

    // Ensure all relays are OFF initially
    digitalWrite(RELAY1_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
    digitalWrite(RELAY2_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
    digitalWrite(RELAY3_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
    digitalWrite(RELAY4_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);

    setupWebServer();
}

void loop() {
    handleWebServer();
    handleSerialCommands();
    cycleColors();
}
