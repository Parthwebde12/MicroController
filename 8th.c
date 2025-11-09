#include <xc.h>

#define in1 LATDbits.LATD1
#define in2 LATDbits.LATD2

void delay(unsigned int time);
void set_speed(unsigned char duty);

void main(void) {
    TRISD = 0x00;
    LATD = 0x00;
    TRISCbits.TRISC2 = 0;

    PR2 = 249;
    CCP1CON = 0x0C;
    T2CON = 0x04;

    while (1) {
        in1 = 1;
        in2 = 0;
        set_speed(64);
        delay(200);
        set_speed(128);
        delay(200);
        set_speed(200);
        delay(200);

        // TE (2019 Pattern)/SEM-III/ Microcontroller Lab- 304188 ADYPSOE/E&TC/LAB MANUAL

        set_speed(0);
        delay(100);

        in1 = 0;
        in2 = 1;
        set_speed(64);
        delay(200);
        set_speed(128);
        delay(200);
        set_speed(200);
        delay(200);
        set_speed(0);
        delay(200);
    }
}

void set_speed(unsigned char duty) {
    CCPR1L = duty >> 2;
    CCP1CONbits.DC1B = duty & 0x03;
}

void delay(unsigned int time) {
    unsigned int i, j;
    for (i = 0; i < time; i++)
        for (j = 0; j < 1257; j++);
}

