#include "Config.h" 
#include <WebServer.h>
#include "WebPage.h"
#include "SinricProConfig.h"
#include <SinricPro.h>
#include <SinricProSwitch.h>

void setup() {
    setupSerial();
    setupRGBLED();
    setupDHT();  
    setupWiFi();
    setupSinricPro();


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
    SinricPro.handle();
    handleSerialCommands();
    cycleColors();
}
