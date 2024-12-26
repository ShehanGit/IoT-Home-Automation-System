#include <WebServer.h>
#include "WebPage.h"

WebServer server(80);

// Function declarations
float getTemperature();
float getHumidity();

void handleGetSensorData() {
    float temp = getTemperature();
    float humidity = getHumidity();
    
    String json = "{\"temperature\":" + String(temp) + 
                  ",\"humidity\":" + String(humidity) + "}";
    
    server.send(200, "application/json", json);
}

void setupWebServer() {

    server.enableCORS(true);
    
    server.on("/", handleRoot);
    server.on("/setColor", HTTP_GET, handleSetColor);
    server.on("/toggleRelax", HTTP_GET, handleToggleRelax);
    server.on("/getSensorData", HTTP_GET, handleGetSensorData);  // Added this line here
    server.on("/relay", HTTP_GET, handleRelayControl);
    server.begin();
}

void handleWebServer() {
    server.handleClient();
}

void handleRoot() {
    server.send(200, "text/html", index_html);
}

void handleSetColor() {
    int r = server.arg("r").toInt();
    int g = server.arg("g").toInt();
    int b = server.arg("b").toInt();
    setRGBColor(r, g, b);
    server.send(200, "text/plain", "OK");
}

void handleToggleRelax() {
    String enabled = server.arg("enabled");
    bool isEnabled = (enabled == "true");
    server.send(200, "text/plain", "OK");
}

void handleRelayControl() {
    if (server.hasArg("relay") && server.hasArg("state")) {
        String relayNumber = server.arg("relay");
        String state = server.arg("state");
        int relayIndex = relayNumber.toInt();

        if (relayIndex >= 1 && relayIndex <= 4) {
            int relayPin;
            switch (relayIndex) {
                case 1: relayPin = RELAY1_PIN; break;
                case 2: relayPin = RELAY2_PIN; break;
                case 3: relayPin = RELAY3_PIN; break;
                case 4: relayPin = RELAY4_PIN; break;
                default: relayPin = -1; break;
            }

            if (relayPin != -1) {
                if (state == "on") {
                    digitalWrite(relayPin, RELAY_ACTIVE_LOW ? LOW : HIGH);
                } else if (state == "off") {
                    digitalWrite(relayPin, RELAY_ACTIVE_LOW ? HIGH : LOW);
                }
                server.send(200, "text/plain", "OK");
            } else {
                server.send(400, "text/plain", "Invalid relay");
            }
        } else {
            server.send(400, "text/plain", "Invalid relay");
        }
    } else {
        server.send(400, "text/plain", "Bad Request");
    }
}