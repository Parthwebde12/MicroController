#include <reg51.h>    // Header file for 8051 microcontroller
#include <stdio.h>    // Standard I/O library (not mandatory here)

// Variable to hold LED status (initially all OFF)
unsigned char led_status = 0x00;

// Delay function (software delay using nested loops)
void delay() {
    int i, j;
    for(i = 0; i < 1000; i++) {
        for(j = 0; j < 1000; j++) {
            // Do nothing, just waste time
        }
    }
}

void main() {
    P1 = led_status; // Send initial LED status (all OFF) to Port 1

    while(1) { // Infinite loop (program runs continuously)
        P1 = led_status; // Output led_status on Port 1 (LED pattern)

        // If all LEDs are ON (0xFF), reset to 0x00 (all OFF)
        if(led_status == 0xFF)
            led_status = 0x00;
        else
            led_status = led_status + 1; // Increment LED pattern step by step

        delay(); // Call delay to make LED change visible
    }
}
