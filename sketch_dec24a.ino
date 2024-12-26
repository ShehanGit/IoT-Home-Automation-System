#include <MAX30105.h>

void setup() {
    setupSerial();
    setupRGBLED();
    setupDHT();  
    setupWiFi();
    
    // --- Initialize MAX30102 sensor ---
    Serial.println("Initializing MAX30102 sensor...");
    if (!setupMAX30102()) {
        Serial.println("ERROR: MAX30102 not found. Check wiring!");
    } else {
        Serial.println("MAX30102 initialized successfully.");
    }

    setupWebServer();
}


void loop() {
    handleWebServer();
    handleSerialCommands();
    cycleColors();
}