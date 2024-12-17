// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "infrared.h"
// #include "ultrasonic_sensor.h"
// #include "motorTest.h"
// #include "hc05.h"
// #include "pwm_led.h"
// #include "hardware/pwm.h"
// #include "hardware/uart.h"
// #include "FreeRTOS.h"
// #include "task.h"
// int main() {
//     stdio_init_all();          // Initialize stdio for debugging
//     setup_ultrasonic();        // Setup the ultrasonic sensor
    
//     while (true) {
//         send_trigger_pulse();  // Send a trigger pulse
//         uint32_t echo_time = measure_echo_time();
//         float distance = calculate_distance(echo_time);

//         if (distance < 0) {
//             printf("Measurement error or no object detected\n");
//         } else {
//             printf("Distance: %.2f cm\n", distance);
//         }

//         sleep_ms(100);  // Add delay between measurements
//     }

//     return 0;
// }


// // Define the UART task priority and stack size
// #define UART_TASK_PRIORITY 2
// #define UART_TASK_STACK_SIZE 256

// // Motor task priority and stack size
// #define MOTOR_TASK_PRIORITY 1
// #define MOTOR_TASK_STACK_SIZE 256

// // Pins and other hardware configurations
// #define IR_SENSOR_PIN 10 // Example pin

// // Global variable for UART data
// volatile uint8_t received_data = 0;

// // UART Task: Handles communication and processes commands
// void uart_task(void *pvParameters) {
//     while (1) {
//         if (uart_is_readable(UART_ID)) {
//             received_data = uart_getc(UART_ID);

//             // Process received data
//             switch (received_data) {
//                 case 'f': move_forward(150);
//                 sleep_ms(1000);
//                 move_forward(0);
//                  break;
//                 case 'b': 
//                     move_backward(150);          // move forward
//                     if (!infrared_sensor_is_triggered(IR_SENSOR_PIN)) {
//                     // Do something when the sensor is triggered
//                         printf("Object detected!\n");
//                         // printf("Stopping...\n");
//                         motor_control(0, true);  // Stop motors
//                         lamp_on();     // Turn the lamp on
//                     // sleep_ms(2000);          // Turn for 1 second
//             }
//             else
//             {
//              lamp_off();
//             } 
//             sleep_ms(100); // Wait for a short delay for debouncing
//                 break;
//                 case 'l': motor_control(150, true); 
//                 sleep_ms(750);
//                 motor_control(0, true);
//                 break;
//                 case 'r': motor_control(150, false); 
//                 sleep_ms(750);
//                 motor_control(0,false);
//                 break;
//                 case 's': motor_control(0, true); 
//                 sleep_ms(750);
//                 motor_control(0, true);
//                 break;
//                 default: break;
//             }
//             sleep_ms(500); // Add a delay to simulate work
//         }
//         vTaskDelay(pdMS_TO_TICKS(10)); // Delay to yield control
//     }
// }

// // Motor Task: Handles motor operations and checks sensor states
// void motor_task(void *pvParameters) {
    
//     while (1) {
//         motor_init();
//          if (uart_is_readable(UART_ID)) {
//             received_data = uart_getc(UART_ID);
         
//             if (received_data == 'b') { // Example: Act on 'b' command
//                 printf("NO Object detected! Stopping...\n");
//                 move_backward(150);
//                 lamp_off();
//                 if (!infrared_sensor_is_triggered(IR_SENSOR_PIN)) {
//                     printf("Object detected! Stopping...\n");
//                     lamp_on(); // Turn on the lamp
//                     move_backward(0);
//                     sleep_ms(3000);
//                 } 
//             }
//             vTaskDelay(pdMS_TO_TICKS(100)); // Delay to yield control
//          }
//     }
// }

// // Main Function
// int main() {
//     //Initialize hardware
//     stdio_init_all();
//     motor_init();
//     infrared_sensor_init(IR_SENSOR_PIN);
//     lamp_init();
//     setup_uart();

//     // UART initialization
//     gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
//     gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
//     uart_set_baudrate(UART_ID, BAUD_RATE);
//     uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
//     uart_set_fifo_enabled(UART_ID, false);

//     // Create FreeRTOS tasks
//     // xTaskCreate(uart_task, "UART Task", UART_TASK_STACK_SIZE, NULL, UART_TASK_PRIORITY, NULL);
//     xTaskCreate(motor_task, "Motor Task", MOTOR_TASK_STACK_SIZE, NULL, MOTOR_TASK_PRIORITY, NULL);

//     // Start the FreeRTOS scheduler
//     vTaskStartScheduler();

//     // Should never reach here
//     while (1) {
//         tight_loop_contents();
//     }
//     return 0;
    
    
// }

















































/////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
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

// Task priorities and stack sizes
#define MAIN_TASK_PRIORITY 2
#define MAIN_TASK_STACK_SIZE 512

// Pins and thresholds
#define IR_SENSOR_PIN 10
#define DISTANCE_THRESHOLD 10.0 // Threshold distance in cm

// Global variables
volatile uint8_t received_data = 0;
volatile float current_distance = 0.0;

// Combined Task: Handles UART commands, motor control, and sensor checks
// Combined Task: Handles UART commands, motor control, and sensor checks
void main_task(void *pvParameters) {
    while (1) {
        // Check for UART commands
        if (uart_is_readable(UART_ID)) {
            received_data = uart_getc(UART_ID);

            switch (received_data) {
                case 'f': { // Move forward continuously
                    printf("Moving forward...\n");
                    move_forward(200);

                    while (1) {
                        // Ultrasonic sensor logic
                        send_trigger_pulse();
                        uint32_t echo_time = measure_echo_time();
                        current_distance = calculate_distance(echo_time);

                        if (current_distance > 0 && current_distance < DISTANCE_THRESHOLD) {
                            printf("Obstacle detected! Stopping forward movement. Distance: %.2f cm\n", current_distance);
                            move_forward(0);  // Stop motors
                            lamp_on();
                            vTaskDelay(pdMS_TO_TICKS(3000)); // Lamp stays on for 3 seconds
                            lamp_off();
                            break; // Exit forward movement loop
                        }

                        // Check for a new UART command to override forward movement
                        if (uart_is_readable(UART_ID)) {
                            received_data = uart_getc(UART_ID);
                            move_forward(0);  // Stop motors before processing the new command
                            break; // Exit forward movement loop and handle new command
                        }

                        vTaskDelay(pdMS_TO_TICKS(50)); // Small delay for responsiveness
                    }
                    break;
                }

                case 'b': { // Move backward continuously with IR sensor check
                    printf("Moving backward...\n");
                    move_backward(200);

                    while (1) {
                        if (!infrared_sensor_is_triggered(IR_SENSOR_PIN)) {
                            printf("Object detected by IR sensor! Stopping backward movement.\n");
                            move_backward(0);
                            lamp_on();
                            vTaskDelay(pdMS_TO_TICKS(3000)); // Lamp stays on for 3 seconds
                            lamp_off();
                            break;
                        }

                        // Check for a new UART command to override backward movement
                        if (uart_is_readable(UART_ID)) {
                            received_data = uart_getc(UART_ID);
                            move_backward(0); // Stop motors before processing the new command
                            break; // Exit backward movement loop and handle new command
                        }

                        vTaskDelay(pdMS_TO_TICKS(50)); // Small delay for responsiveness
                    }
                    break;
                }

                case 'l': // Turn left
                    printf("Turning left...\n");
                    motor_control(200, true);
                    vTaskDelay(pdMS_TO_TICKS(750));
                    motor_control(0, true);
                    break;

                case 'r': // Turn right
                    printf("Turning right...\n");
                    motor_control(200, false);
                    vTaskDelay(pdMS_TO_TICKS(750));
                    motor_control(0, false);
                    break;

                case 's': // Stop motors
                    printf("Stopped.\n");
                    motor_control(0, true);
                    break;

                default:
                    break;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50)); // Delay to yield control
    }
}


// Main Function
int main() {
    // Initialize hardware
    stdio_init_all();
    motor_init();
    infrared_sensor_init(IR_SENSOR_PIN);
    lamp_init();
    setup_ultrasonic();
    setup_uart();

    // UART initialization
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_baudrate(UART_ID, BAUD_RATE);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);

    // Create FreeRTOS task
    xTaskCreate(main_task, "Main Task", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    // Should never reach here
    while (1) {
        tight_loop_contents();
    }

    return 0;
}

