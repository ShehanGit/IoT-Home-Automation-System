#include "Config.h"

#include "esp32-hal.h"

// Define PWM properties
const int PWM_FREQUENCY = 5000;
const int PWM_RESOLUTION = 8;  // 8-bit resolution (0-255)
const int RED_CHANNEL = 0;
const int GREEN_CHANNEL = 1;
const int BLUE_CHANNEL = 2;

bool isRelaxMode = false;
unsigned long lastColorChange = 0;
float currentHue = 0.0;
const unsigned long COLOR_CHANGE_INTERVAL = 50; // milliseconds

void setupRGBLED() {
    // Configure LED PWM functionalities
    ledcSetup(RED_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcSetup(GREEN_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcSetup(BLUE_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    
    // Attach the PWM channels to the GPIO pins
    ledcAttachPin(RGB_RED_PIN, RED_CHANNEL);
    ledcAttachPin(RGB_GREEN_PIN, GREEN_CHANNEL);
    ledcAttachPin(RGB_BLUE_PIN, BLUE_CHANNEL);
    
    // Start with LEDs off
    setRGBColor(0, 0, 0);
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
    
    // Write PWM values
    ledcWrite(RED_CHANNEL, r);
    ledcWrite(GREEN_CHANNEL, g);
    ledcWrite(BLUE_CHANNEL, b);
}

void HSVtoRGB(float h, float s, float v, int& r, int& g, int& b) {
    float c = v * s;
    float x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
    float m = v - c;
    
    float r1, g1, b1;
    if(h >= 0 && h < 60) {
        r1 = c; g1 = x; b1 = 0;
    } else if(h >= 60 && h < 120) {
        r1 = x; g1 = c; b1 = 0;
    } else if(h >= 120 && h < 180) {
        r1 = 0; g1 = c; b1 = x;
    } else if(h >= 180 && h < 240) {
        r1 = 0; g1 = x; b1 = c;
    } else if(h >= 240 && h < 300) {
        r1 = x; g1 = 0; b1 = c;
    } else {
        r1 = c; g1 = 0; b1 = x;
    }
    
    r = (r1 + m) * 255;
    g = (g1 + m) * 255;
    b = (b1 + m) * 255;
}

void handleColorCycle() {
    if (!isRelaxMode) return;
    
    unsigned long currentTime = millis();
    if (currentTime - lastColorChange >= COLOR_CHANGE_INTERVAL) {
        currentHue += 0.5; // Smaller increment for smoother transition
        if (currentHue >= 360.0) currentHue = 0.0;
        
        int r, g, b;
        HSVtoRGB(currentHue, 1.0, 0.8, r, g, b); // Slightly reduced brightness
        setRGBColor(r, g, b);
        
        lastColorChange = currentTime;
    }
}

void setRelaxMode(bool enabled) {
    isRelaxMode = enabled;
    if (!enabled) {
        setRGBColor(0, 0, 0);
    } else {
        currentHue = 0.0;
        lastColorChange = millis();
    }
}