void setupSerial() {
    Serial.begin(115200);
}

void handleSerialCommands() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        
        if (command.startsWith("RGB:")) {
            command = command.substring(4); // Remove "RGB:"
            int firstComma = command.indexOf(',');
            int secondComma = command.indexOf(',', firstComma + 1);
            
            if (firstComma != -1 && secondComma != -1) {
                int r = command.substring(0, firstComma).toInt();
                int g = command.substring(firstComma + 1, secondComma).toInt();
                int b = command.substring(secondComma + 1).toInt();
                
                // Use the same function that works with the web interface
                setRGBColor(r, g, b);
                
                // Echo back for debugging
                Serial.printf("Set LED to R:%d G:%d B:%d\n", r, g, b);
            }
        }
    }
}