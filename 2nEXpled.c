#include <stdio.h>
#include <reg51.h>

unsigned char led_status = 0x00;

void delay() {
    int i, j;
    for (i = 0; i < 1000; i++) {
        for (j = 0; j < 1000; j++) {
            // simple software delay
        }
    }
}

void main() {
    while (1) {
        P1 = led_status;           // send data to Port 1
        if (led_status == 0xFF)
            led_status = 0x00;     // reset if all LEDs are ON
        else
            led_status = led_status + 1; // increment pattern
        delay();                   // ? semicolon added
    }
}
