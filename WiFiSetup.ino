void initializeWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWiFi connection failed!");
    return;
  }
  
  Serial.println("\nConnected to WiFi!");
  Serial.println(WiFi.localIP());
  
  if (!MDNS.begin("esp32")) {
    Serial.println("MDNS setup failed!");
    return;
  }
  
  MDNS.addService("http", "tcp", 80);
  Serial.println("MDNS started - http://esp32.local");
}