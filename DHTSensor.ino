#include <DHT.h>
#include "Config.h"

DHT dht(DHT_PIN, DHT22);

void setupDHT() {
    dht.begin();
    Serial.println("DHT22 initialized");
}

float getTemperature() {
    float temp = dht.readTemperature();
    if (isnan(temp)) {
        Serial.println("Failed to read temperature from DHT22!");
        return 0.0;
    }
    return temp;
}

float getHumidity() {
    float humidity = dht.readHumidity();
    if (isnan(humidity)) {
        Serial.println("Failed to read humidity from DHT22!");
        return 0.0;
    }
    return humidity;
}