#include <reg51.h>

void delay(void);

void main(void) {
    unsigned char bnum[16] = {
        0x00, 0x01, 0x04, 0x05,
        0x10, 0x11, 0x14, 0x15,
        0x40, 0x41, 0x44, 0x45,
        0x50, 0x51, 0x54, 0x55
    };

    while(1) {
        int i;
        for(i = 0; i < 16; i++) {
            P1 = bnum[i];  // Output HEX pattern
            delay();       // Small delay
        }
    }
}

void delay(void) {
    int i;
    for(i = 0; i < 10000; i++);  // Simple delay loop
}
