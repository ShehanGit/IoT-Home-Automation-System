void initializeWebServer() {
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", index_html);
    });

    server.on("/color", HTTP_GET, [](AsyncWebServerRequest *request) {
        String response = "Error";
        
        if (request->hasParam("hex")) {
            String hex = request->getParam("hex")->value();
            if (hex.length() == 6) {
                int r = strtol(hex.substring(0,2).c_str(), NULL, 16);
                int g = strtol(hex.substring(2,4).c_str(), NULL, 16);
                int b = strtol(hex.substring(4,6).c_str(), NULL, 16);
                setRGBColor(r, g, b);
                response = "OK";
            }
        } else if (request->hasParam("c")) {
            String color = request->getParam("c")->value();
            if (color == "red") setRGBColor(255, 0, 0);
            else if (color == "green") setRGBColor(0, 255, 0);
            else if (color == "blue") setRGBColor(0, 0, 255);
            else if (color == "off") setRGBColor(0, 0, 0);
            response = "OK";
        }
        
        request->send(200, "text/plain", response);
    });

    server.on("/relay", HTTP_GET, [](AsyncWebServerRequest *request) {
        if(request->hasParam("relay") && request->hasParam("state")) {
            int relay = request->getParam("relay")->value().toInt();
            int state = request->getParam("state")->value().toInt();
            
            switch(relay) {
                case 1: digitalWrite(RELAY1_PIN, state ? !RELAY_ACTIVE_LOW : RELAY_ACTIVE_LOW); break;
                case 2: digitalWrite(RELAY2_PIN, state ? !RELAY_ACTIVE_LOW : RELAY_ACTIVE_LOW); break;
                case 3: digitalWrite(RELAY3_PIN, state ? !RELAY_ACTIVE_LOW : RELAY_ACTIVE_LOW); break;
                case 4: digitalWrite(RELAY4_PIN, state ? !RELAY_ACTIVE_LOW : RELAY_ACTIVE_LOW); break;
            }
        }
        request->send(200, "text/plain", "OK");
    });

    server.begin();
    Serial.println("Web Server started");
}