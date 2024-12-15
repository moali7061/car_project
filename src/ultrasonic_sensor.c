#include "ultrasonic_sensor.h"
void setup_ultrasonic() {
    // Set TRIGGER_PIN as output
    gpio_init(TRIGGER_PIN);
    gpio_set_dir(TRIGGER_PIN, GPIO_OUT);
    gpio_put(TRIGGER_PIN,0) ; 
    // Set ECHO_PIN as input
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}


void send_trigger_pulse() {
    gpio_put(TRIGGER_PIN, 1);  // Set Trigger high
    sleep_us(10);               // Wait for 10µs
    gpio_put(TRIGGER_PIN, 0);  // Set Trigger low
}

uint32_t measure_echo_time() {
    uint64_t start_time = to_us_since_boot(get_absolute_time());
    while (gpio_get(ECHO_PIN) == 0) {
        if (to_us_since_boot(get_absolute_time()) - start_time > 50000) {
            printf("Timeout waiting for echo high\n");
            return 0; // Timeout
        }
    }

    start_time = to_us_since_boot(get_absolute_time());
    while (gpio_get(ECHO_PIN) == 1) {
        if (to_us_since_boot(get_absolute_time()) - start_time > 50000) {
            printf("Timeout waiting for echo low\n");
            return 0; // Timeout
        }
    }

    uint64_t end_time = to_us_since_boot(get_absolute_time());
    return (uint32_t)(end_time - start_time); // Time in microseconds
}


float calculate_distance(uint32_t echo_time) {
    if (echo_time == 0) {
        return -1; // Invalid measurement
    }
    const float speed_of_sound = 0.0343;
    return (echo_time * speed_of_sound) / 2.0;
}
