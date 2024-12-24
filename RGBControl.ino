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
    
    // Write analog values to pins
    analogWrite(RGB_RED_PIN, r);
    analogWrite(RGB_GREEN_PIN, g);
    analogWrite(RGB_BLUE_PIN, b);
    
    Serial.printf("Setting PWM - R:%d G:%d B:%d\n", r, g, b);
}