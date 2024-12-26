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