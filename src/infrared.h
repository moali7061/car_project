#ifndef INFRARED_SENSOR_H 
#define INFRARED_SENSOR_H
#include "pico/stdlib.h"
// Define the default pin for the infrared sensor
#ifndef IR_SENSOR_PIN
// Default GPIO pin for infrared sensor
#define IR_SENSOR_PIN 10
#endif
// Function to initialize the infrared sensor
void infrared_sensor_init(uint sensor_pin);
// Function to read the state of the infrared sensor
bool infrared_sensor_is_triggered(uint sensor_pin);
#endif // INFRARED_SENSOR_H