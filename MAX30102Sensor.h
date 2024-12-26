#ifndef MAX30102SENSOR_H
#define MAX30102SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_MAX3010x.h>

// Functions to initialize and read from the MAX30102.
bool setupMAX30102();
float getHeartRate(); 
float getSpO2();

#endif
