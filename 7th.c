#include <xc.h>
#pragma config FOSC = HS, WDT = OFF, LVP = OFF, PBADEN = OFF
#define _XTAL_FREQ 20000000UL
#define RS LATBbits.LATB0
#define EN LATBbits.LATB1
#define D4 LATBbits.LATB2
#define D5 LATBbits.LATB3
#define D6 LATBbits.LATB4
#define D7 LATBbits.LATB5

static void UART_Init(void) {
    ADCON1 = 0x0F;
    CMCON  = 0x07;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 1;
    SPBRG = 129;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
}

static void UART_Tx(char c) {
    while (!PIR1bits.TXIF);
    TXREG = c;
}

static void UART_Print(const char *s) {
    while (*s) UART_Tx(*s++);
}

static char UART_Rx(void) {
    while (!PIR1bits.RCIF);
    if (RCSTAbits.OERR) {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
    return RCREG;
}

static void pulse(void) {
    EN = 1;
    __delay_us(5);
    EN = 0;
    __delay_us(50);
}

static void lcd_cmd(unsigned char c) {
    RS = 0;
    D4 = (c >> 4) & 1; D5 = (c >> 5) & 1; D6 = (c >> 6) & 1; D7 = (c >> 7) & 1;
    pulse();
    D4 = c & 1; D5 = (c >> 1) & 1; D6 = (c >> 2) & 1; D7 = (c >> 3) & 1;
    pulse();
    __delay_ms(2);
}

static void lcd_dat(unsigned char c) {
    RS = 1;
    D4 = (c >> 4) & 1; D5 = (c >> 5) & 1; D6 = (c >> 6) & 1; D7 = (c >> 7) & 1;
    pulse();
    D4 = c & 1; D5 = (c >> 1) & 1; D6 = (c >> 2) & 1; D7 = (c >> 3) & 1;
    pulse();
    __delay_ms(2);
}

static void lcd_init(void) {
    TRISB = 0x00;
    LATB = 0x00;
    __delay_ms(20);
    // TE (2019 Pattern)/SEM-III/ Microcontroller Lab- 304188 ADYPSOE/E&TC/LAB MANUAL
    lcd_cmd(0x02);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

static void lcd_clear(void) {
    lcd_cmd(0x01);
    __delay_ms(2);
}

static void lcd_pos(unsigned char r, unsigned char c) {
    lcd_cmd((r == 1) ? 0x80 + c - 1 : 0xC0 + c - 1);
}

static void lcd_print(const char *s) {
    while (*s) lcd_dat(*s++);
}

void main(void) {
    UART_Init();
    lcd_init();
    lcd_pos(1, 1);
    lcd_print("Serial Ready");
    UART_Print("PIC18F4550 Serial Test\r\n");
    unsigned long t = 0;
    while (1) {
        if (++t >= 40000UL) {
            UART_Tx('.');
            UART_Tx('\r');
            UART_Tx('\n');
            t = 0;
        }
        if (PIR1bits.RCIF) {
            char c = UART_Rx();
            lcd_clear();
            lcd_pos(1, 1);
            lcd_print("Received:");
            lcd_pos(2, 1);
            lcd_dat(c);
            UART_Print("You typed: ");
            UART_Tx(c);
            UART_Print("\r\n");
        }
        __delay_us(25);
    }
}
