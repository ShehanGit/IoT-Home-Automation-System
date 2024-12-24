void setupSerial() {
    Serial.begin(115200);
    Serial.println("Serial initialized");
}

void handleSerialCommands() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        
        if (command.startsWith("RGB:")) {
            // Parse RGB values
            command = command.substring(4); // Remove "RGB:"
            int firstComma = command.indexOf(',');
            int secondComma = command.indexOf(',', firstComma + 1);
            
            if (firstComma != -1 && secondComma != -1) {
                int r = command.substring(0, firstComma).toInt();
                int g = command.substring(firstComma + 1, secondComma).toInt();
                int b = command.substring(secondComma + 1).toInt();
                
                setRGBColor(r, g, b);
            }
        }
    }
}