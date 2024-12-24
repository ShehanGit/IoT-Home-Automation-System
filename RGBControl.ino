#include "Config.h"
// Define PWM properties
#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8  // 8-bit resolution (0-255)
#define RED_CHANNEL 0
#define GREEN_CHANNEL 1
#define BLUE_CHANNEL 2

void setupRGBLED() {
    // Configure pins as outputs
    pinMode(RGB_RED_PIN, OUTPUT);
    pinMode(RGB_GREEN_PIN, OUTPUT);
    pinMode(RGB_BLUE_PIN, OUTPUT);
    
    // Configure LED PWM functionalities
    analogWrite(RGB_RED_PIN, 0);
    analogWrite(RGB_GREEN_PIN, 0);
    analogWrite(RGB_BLUE_PIN, 0);
}

void setRGBColor(int r, int g, int b) {
    // Constrain values to 0-255 range
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);
    
    // Invert values for common anode LED
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
    
    // Write analog values to pins
    analogWrite(RGB_RED_PIN, r);
    analogWrite(RGB_GREEN_PIN, g);
    analogWrite(RGB_BLUE_PIN, b);
    
    Serial.printf("Setting PWM - R:%d G:%d B:%d\n", r, g, b);
}



bool isColorCycling = false;
unsigned long lastColorChange = 0;
const unsigned long COLOR_CHANGE_INTERVAL = 3000; // 3 seconds per color

void cycleColors() {
    if (!isColorCycling || millis() - lastColorChange < COLOR_CHANGE_INTERVAL) {
        return;
    }
    
    static int hue = 0;
    float r, g, b;
    
    // Convert HSV to RGB (hue: 0-360)
    float h = hue / 360.0f;
    float s = 1.0f;
    float v = 1.0f;
    
    int i = int(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    
    switch (i % 6) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    
    setRGBColor(r * 255, g * 255, b * 255);
    
    hue = (hue + 1) % 360;
    lastColorChange = millis();
}

void setColorCycling(bool enabled) {
    isColorCycling = enabled;
}