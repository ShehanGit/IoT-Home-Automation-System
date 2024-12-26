#include <WebServer.h>
#include "WebPage.h"

WebServer server(80);

// Function declarations
float getTemperature();
float getHumidity();

void handleGetSensorData() {
    float temp = getTemperature();
    float humidity = getHumidity();

    // --- Read from MAX30102 ---
    float heartRate = getHeartRate();
    float spo2      = getSpO2();

    // Build JSON
    String json = "{\"temperature\":" + String(temp) + 
                  ",\"humidity\":" + String(humidity) + 
                  ",\"heartRate\":" + String(heartRate) + 
                  ",\"spo2\":" + String(spo2) +
                  "}";

    server.send(200, "application/json", json);
}



void setupWebServer() {

    server.enableCORS(true);
    
    server.on("/", handleRoot);
    server.on("/setColor", HTTP_GET, handleSetColor);
    server.on("/toggleRelax", HTTP_GET, handleToggleRelax);
    server.on("/getSensorData", HTTP_GET, handleGetSensorData);  // Added this line here

    // New route to get MAX30102 data
    server.on("/getMaxData", HTTP_GET, handleGetMaxData);

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

void handleGetMaxData() {
    float hr   = getHeartRate();  // from MAX30102Sensor.cpp
    float spo2 = getSpO2();       // from MAX30102Sensor.cpp

    // Create a JSON string
    String json = "{\"heartRate\":" + String(hr) +
                  ",\"SpO2\":"      + String(spo2) + "}";

    // Respond with JSON
    server.send(200, "application/json", json);
}


