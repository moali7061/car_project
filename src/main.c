#include <stdio.h>
#include "pico/stdlib.h"
#include "infrared.h"
#include "ultrasonic_sensor.h"
#include "motorTest.h"
#include "hc05.h"
#include "pwm_led.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "FreeRTOS.h"
#include "task.h"

// Define the UART task priority and stack size
#define UART_TASK_PRIORITY 2
#define UART_TASK_STACK_SIZE 256

// Motor task priority and stack size
#define MOTOR_TASK_PRIORITY 1
#define MOTOR_TASK_STACK_SIZE 256

// Pins and other hardware configurations
#define IR_SENSOR_PIN 15 // Example pin

// Global variable for UART data
volatile uint8_t received_data = 0;

// UART Task: Handles communication and processes commands
void uart_task(void *pvParameters) {
    while (1) {
        if (uart_is_readable(UART_ID)) {
            received_data = uart_getc(UART_ID);

            // Process received data
            switch (received_data) {
                case 'f': move_forward(150); break;
                case 'b': move_backward(150); break;
                case 'r': motor_control(150, true); break;
                case 'l': motor_control(150, false); break;
                default: break;
            }
            sleep_ms(500); // Add a delay to simulate work
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Delay to yield control
    }
}

// Motor Task: Handles motor operations and checks sensor states
void motor_task(void *pvParameters) {
    while (1) {
        if (received_data == 'b') { // Example: Act on 'b' command
            if (!infrared_sensor_is_triggered(IR_SENSOR_PIN)) {
                printf("Object detected! Stopping...\n");
                motor_control(0, true); // Stop motors
                lamp_on(); // Turn on the lamp
            } else {
                lamp_off(); // Turn off the lamp
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay to yield control
    }
}

// Main Function
int main() {
    // Initialize hardware
    stdio_init_all();
    motor_init();
    infrared_sensor_init(IR_SENSOR_PIN);
    lamp_init();
    setup_uart();

    // UART initialization
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_baudrate(UART_ID, BAUD_RATE);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);

    // Create FreeRTOS tasks
    xTaskCreate(uart_task, "UART Task", UART_TASK_STACK_SIZE, NULL, UART_TASK_PRIORITY, NULL);
    xTaskCreate(motor_task, "Motor Task", MOTOR_TASK_STACK_SIZE, NULL, MOTOR_TASK_PRIORITY, NULL);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    // Should never reach here
    while (1) {
        tight_loop_contents();
    }
    return 0;
}
