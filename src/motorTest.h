#ifndef MOTORTEST_H
#define MOTORTEST_H

#include <stdint.h>
#include <stdbool.h>

// Right Motor (Motor 1) Pin Definitions
#define MOTOR1_PWM_PIN 0    // PWM pin for Motor 1
#define MOTOR1_DIR_PIN1 16  // Direction pin 1 for Motor 1
#define MOTOR1_DIR_PIN2 17  // Direction pin 2 for Motor 1

// Left Motor (Motor 2) Pin Definitions
#define MOTOR2_PWM_PIN 1    // PWM pin for Motor 2
#define MOTOR2_DIR_PIN1 18  // Direction pin 1 for Motor 2
#define MOTOR2_DIR_PIN2 19  // Direction pin 2 for Motor 2

// Function Declarations
void motor_init();                            // Initializes motor GPIOs and PWM
void motor_control(uint16_t speed, bool direction); // Controls both motors (speed and direction)
void move_backward(uint16_t speed);              // Turns the car to the right
void move_forward(uint16_t speed);               // Turns the car to the left

#endif // MOTORTEST_H
