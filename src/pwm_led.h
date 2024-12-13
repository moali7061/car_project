#ifndef LAMP_H
#define LAMP_H

#include "pico/stdlib.h"

// Define the GPIO pin for the lamp
#define LAMP_PIN 8

// Function prototypes
void lamp_init();
void lamp_on();
void lamp_off();
void lamp_toggle();

#endif // LAMP_H
