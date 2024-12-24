#include <WebServer.h>
#include "WebPage.h"

WebServer server(80);

void setupWebServer() {
    server.on("/", handleRoot);
    server.on("/setColor", HTTP_GET, handleSetColor);
    server.on("/toggleRelax", HTTP_GET, handleToggleRelax);
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