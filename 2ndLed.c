#include <reg51.h>   // Header file for 8051 microcontroller
#include <stdio.h>   // Standard I/O library (optional)

// Variable to hold LED status (initially all OFF)
unsigned char led_status = 0x00;

// Delay function (software delay)
void delay() {
    int i, j;
    for(i = 0; i < 1000; i++) {
        for(j = 0; j < 1000; j++) {
            // Do nothing, just delay
        }
    }
}

void main() {
    P1 = led_status;  // Initialize LEDs (all OFF)

    while(1) {        // Infinite loop
        P1 = led_status;   // Output LED pattern

        if(led_status == 0xFF)
            led_status = 0x00;  // Reset if all LEDs ON
        else
            led_status = led_status + 1;  // Increment pattern

        delay();  // Visible delay
    }
}
