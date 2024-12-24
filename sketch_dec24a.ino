void setup() {
    setupSerial();
    setupRGBLED();
    setupWiFi();
    setupWebServer();
}

void loop() {
    handleWebServer();
    handleSerialCommands();
    handleColorCycle();  // Changed from cycleColors to handleColorCycle
}