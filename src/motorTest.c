#include "motorTest.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Initialize both motors
void motor_init() {
    // Initialize Right Motors (Motor 1)
    gpio_init(MOTOR1_DIR_PIN1);
    gpio_init(MOTOR1_DIR_PIN2);
    gpio_set_dir(MOTOR1_DIR_PIN1, GPIO_OUT);
    gpio_set_dir(MOTOR1_DIR_PIN2, GPIO_OUT);
    gpio_set_function(MOTOR1_PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num1 = pwm_gpio_to_slice_num(MOTOR1_PWM_PIN);
    pwm_set_wrap(slice_num1, 255); // 8-bit resolution
    pwm_set_clkdiv(slice_num1, 4.0f); // Clock divider
    pwm_set_gpio_level(MOTOR1_PWM_PIN, 0); // Start with 0% duty cycle
    pwm_set_enabled(slice_num1, true);

    // Initialize Left Motors (Motor 2)
    gpio_init(MOTOR2_DIR_PIN1);
    gpio_init(MOTOR2_DIR_PIN2);
    gpio_set_dir(MOTOR2_DIR_PIN1, GPIO_OUT);
    gpio_set_dir(MOTOR2_DIR_PIN2, GPIO_OUT);
    gpio_set_function(MOTOR2_PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num2 = pwm_gpio_to_slice_num(MOTOR2_PWM_PIN);
    pwm_set_wrap(slice_num2, 255); // 8-bit resolution
    pwm_set_clkdiv(slice_num2, 4.0f); // Clock divider
    pwm_set_gpio_level(MOTOR2_PWM_PIN, 0); // Start with 0% duty cycle
    pwm_set_enabled(slice_num2, true);
}

// Control both motors
void motor_control(uint16_t speed, bool direction) {
    // Cap speed to 255
    if (speed > 255) {
        speed = 255;
    }

    // Right Motors (Motor 1)
    if (direction) { // Forward
        gpio_put(MOTOR1_DIR_PIN1, 1);
        gpio_put(MOTOR1_DIR_PIN2, 0);
    } else { // Backward
        gpio_put(MOTOR1_DIR_PIN1, 0);
        gpio_put(MOTOR1_DIR_PIN2, 1);
    }
    pwm_set_gpio_level(MOTOR1_PWM_PIN, speed);

    // Left Motors (Motor 2)
    if (direction) { // Forward
        gpio_put(MOTOR2_DIR_PIN1, 1);
        gpio_put(MOTOR2_DIR_PIN2, 0);
    } else { // Backward
        gpio_put(MOTOR2_DIR_PIN1, 0);
        gpio_put(MOTOR2_DIR_PIN2, 1);
    }
    pwm_set_gpio_level(MOTOR2_PWM_PIN, speed);
}

// Turn right
void move_forward(uint16_t speed) {
    // Right Motors (Motor 1) Forward
    gpio_put(MOTOR1_DIR_PIN1, 1);
    gpio_put(MOTOR1_DIR_PIN2, 0);
    pwm_set_gpio_level(MOTOR1_PWM_PIN, speed);

    // Left Motors (Motor 2) Backward
    gpio_put(MOTOR2_DIR_PIN1, 0);
    gpio_put(MOTOR2_DIR_PIN2, 1);
    pwm_set_gpio_level(MOTOR2_PWM_PIN, speed);
}

// Turn left
void move_backward(uint16_t speed) {
    // Right Motors (Motor 1) Backward
    gpio_put(MOTOR1_DIR_PIN1, 0);
    gpio_put(MOTOR1_DIR_PIN2, 1);
    pwm_set_gpio_level(MOTOR1_PWM_PIN, speed);

    // Left Motors (Motor 2) Forward
    gpio_put(MOTOR2_DIR_PIN1, 1);
    gpio_put(MOTOR2_DIR_PIN2, 0);
    pwm_set_gpio_level(MOTOR2_PWM_PIN, speed);
}

