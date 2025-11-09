#include <reg51.h>
#include <stdio.h>

unsigned char led_status = 0x00;

void delay() {
    int i, j;
    for(i = 0; i < 1000; i++) {
        for(j = 0; j < 1000; j++) {
            // Software delay
        }
    }
}

void main() {
    P1 = led_status;  // Initialize LEDs

    while(1) {
        P1 = led_status;   // Output LED pattern

        if(led_status == 0xFF)
            led_status = 0x00;
        else
            led_status = led_status + 1;

        delay();  // Delay between increments
    }
}
