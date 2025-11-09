#include <xc.h>
#include <pic18f4550.h>

// --- CONFIGURATION BITS --- 
// Using an external 20MHz crystal with PLL for 48MHz (96MHz / 2) operation
#pragma config PLLDIV = 5        // Divide by 5 (20MHz/5 = 4MHz input to PLL)
#pragma config CPUDIV = OSC1_PLL2 // Divide clock by 2 (48MHz / 2 = 24MHz CPU Clock)
#pragma config USBDIV = 2        // USB clock source / 2
#pragma config FOSC = HSPLL_HS   // High Speed crystal with PLL enabled
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF        // Internal/External Oscillator Switchover disabled
#pragma config PWRT = OFF        // Power-up Timer disabled
#pragma config BOR = OFF         // Brown-out Reset disabled
#pragma config VREGEN = OFF      // USB voltage regulator disabled
#pragma config MCLRE = ON        // MCLR pin enabled
#pragma config LPT1OSC = OFF     // Low-power Timer1 Oscillator disabled
#pragma config PBADEN = OFF      // PORTB A/D functionality disabled on reset
#pragma config CCP2MX = ON       // CCP2 input/output multiplexed with RC1
#pragma config STVREN = ON       // Stack full/underflow reset enabled
#pragma config LVP = OFF         // Low-Voltage Programming disabled
#pragma config ICPRT = OFF       // In-Circuit Debug/Programming disabled
#pragma config XINST = OFF       // Extended Instruction Set disabled
#pragma config CP0 = OFF, CP1 = OFF, CPB = OFF, CPD = OFF // Code Protection OFF

#define _XTAL_FREQ 48000000 // Primary oscillator frequency for __delay_ms

// --- PIN DEFINITIONS --- 
// LCD Pin Definitions 
#define LCD_DATA_PORT LATB       // Use LATB for writing data
#define LCD_RS LATBbits.LATB4    // Register Select pin (RB4)
#define LCD_E  LATBbits.LATB5    // Enable pin (RB5)

// Keypad Pin Definitions 
#define ROW_LATCH LATD           // LATD0-LATD3 for driving rows (Outputs)
#define COL_PINS PORTD           // PORTD4-PORTD7 for reading columns (Inputs)

// --- FUNCTION PROTOTYPES --- 
void Init_PIC();
void Send_4Bit(unsigned char data);
void LCD_Pulse_E();
void LCD_Write(unsigned char a, unsigned char is_data);
void LCD_Command(unsigned char cmd);
void LCD_Data(unsigned char dat);
void LCD_Init();
void LCD_String(const char *str);
char Keypad_Scan();

// --- MAIN FUNCTION --- 
void main() {
    Init_PIC();
    LCD_Init();
    LCD_String("Keypad Scanner");
    LCD_Command(0xC0); // Go to second line    
    LCD_String("Press a Key!");
    
    while (1) {
        char key = Keypad_Scan();
        if (key != 0) {
            LCD_Command(0x01); // Clear display
            __delay_ms(2);
            LCD_String("Key Pressed:");
            LCD_Command(0xC0);
            LCD_Data(key); // Display the pressed key
            __delay_ms(500); // Wait half a second
            LCD_Command(0x01); // Clear for next input
            __delay_ms(2);
            LCD_String("Keypad Scanner");
            LCD_Command(0xC0);
            LCD_String("Press a Key!");
        }
    }
}

// --- FUNCTION IMPLEMENTATIONS --- 
void Init_PIC() {
    ADCON1 = 0x0F; // Disable all analog inputs (Important for PIC18F4550)
    TRISB = 0b00000000; // PORTB as Output (for LCD control and data)
    TRISD = 0b11110000; // RD0-RD3 (Rows) Output, RD4-RD7 (Columns) Input
    LATB = 0x00;
    LATD = 0x00;
}

// ---------------------- LCD Functions (4-bit mode) ---------------------- 
void Send_4Bit(unsigned char data) {
    LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | (data & 0x0F);
}

void LCD_Pulse_E() {
    LCD_E = 1;
    __delay_us(50);
    LCD_E = 0;
    __delay_us(50); // E pulse width
}

void LCD_Write(unsigned char a, unsigned char is_data) {
    LCD_RS = is_data; // 0 for command, 1 for data
    Send_4Bit(a >> 4); // Send Higher Nibble
    LCD_Pulse_E();
    Send_4Bit(a);      // Send Lower Nibble
    LCD_Pulse_E();
    __delay_ms(2);
}

void LCD_Command(unsigned char cmd) {
    LCD_Write(cmd, 0); // 0 for command
}

void LCD_Data(unsigned char dat) {
    LCD_Write(dat, 1); // 1 for data
}

void LCD_Init() {
    __delay_ms(15);
    LCD_Command(0x02); // Return Home
    LCD_Command(0x28); // 4-bit mode, 2-line display, 5x8 dots
    LCD_Command(0x0C); // Display ON, Cursor OFF, Blink OFF
    LCD_Command(0x06); // Entry Mode Set
    LCD_Command(0x01); // Clear display
    __delay_ms(2);
}

void LCD_String(const char *str) {
    while (*str != '\0') {
        LCD_Data(*str++);
    }
}

// ---------------------- Keypad Scan Function ---------------------- 
char Keypad_Scan() {
    const char keys[4][4] = {
        {'7', '8', '9', '/'},
        {'4', '5', '6', '*'},
        {'1', '2', '3', '-'},
        {'O', '0', '=', '+'}
    };

    for (int row = 0; row < 4; row++) {
        ROW_LATCH |= 0x0F;           // Set all rows high
        ROW_LATCH &= ~(1 << row);    // Drive current row LOW
        __delay_us(10);              // Settle time

        unsigned char col_check = (COL_PINS >> 4) & 0x0F;
        if (!(col_check & 0b0001)) return keys[row][0]; // Col 1 (RD4)
        if (!(col_check & 0b0010)) return keys[row][1]; // Col 2 (RD5)
        if (!(col_check & 0b0100)) return keys[row][2]; // Col 3 (RD6)
        if (!(col_check & 0b1000)) return keys[row][3]; // Col 4 (RD7)
    }

    ROW_LATCH = 0x0F; // Reset all rows high after the scan
    return 0;         // No key pressed
}
