void setup() {
    setupSerial();
    setupRGBLED();
    setupWiFi();
    setupWebServer();
}

void loop() {
    handleWebServer();
    handleSerialCommands();
    cycleColors();
}