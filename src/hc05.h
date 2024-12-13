#ifndef HC05_H
#define HC05_H

#include "pico/stdlib.h"
#include "hardware/uart.h"

//#include "hardware/gpio.h"
// // UART and GPIO configuration
// #define UART_ID uart1
// #define BAUD_RATE 9600
// #define TX_PIN 12   // UART TX pin
// #define RX_PIN 13   // UART RX pin


#define UART_ID uart0
#define BAUD_RATE 9600 // 
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE
#define UART_TX_PIN 12
#define UART_RX_PIN 13

// Function prototypes
void setup_uart(void);


#endif // HC05_H