#include "pwm_led.h"

// Initialize the GPIO pin for the lamp
void lamp_init() {
    gpio_init(LAMP_PIN);        // Initialize the GPIO
    gpio_set_dir(LAMP_PIN, GPIO_OUT); // Set as output
    gpio_put(LAMP_PIN, 0);      // Ensure the lamp is off initially
}

// Turn the lamp on
void lamp_on() {
    gpio_put(LAMP_PIN, 1);  // Set GPIO high
}

// Turn the lamp off
void lamp_off() {
    gpio_put(LAMP_PIN, 0);  // Set GPIO low
}

// Toggle the lamp state
void lamp_toggle() {
    gpio_put(LAMP_PIN, !gpio_get(LAMP_PIN));  // Toggle the state
}
