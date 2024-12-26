#include "MAX30102Sensor.h"

// Create a global sensor object (managed in this .cpp file)
static SparkFun_MAX3010x pulseSensor; 

// Sensor initialization
bool setupMAX30102() {
    // If you use default I2C pins on ESP32: SDA = 21, SCL = 22
    // Adjust if you have different pin assignments
    Wire.begin(21, 22);

    if (!pulseSensor.begin()) {
        // Could not find sensor
        return false;
    }

    // Set up sensor with default recommended settings
    // You can customize sample rate, LED power, etc. if you want.
    pulseSensor.setup();

    return true;
}

// Read current heart rate (in BPM) from the sensor
float getHeartRate() {
    // Some libraries return 0 or negative values if HR not found
    // You can handle error-checking as needed
    return pulseSensor.getHeartRate();
}

// Read current SpO2 (in %) from the sensor
float getSpO2() {
    // May return 0 or 255 if sensor can’t measure
    return pulseSensor.getSpO2();
}
