void setup() {
    setupSerial();
    setupRGBLED();
    setupDHT();  
    setupWiFi();
    setupWebServer();
}

void loop() {
    handleWebServer();
    handleSerialCommands();
    cycleColors();
}