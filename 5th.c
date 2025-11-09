#include <xc.h> // Configuration bits 
#pragma config WDT = OFF // Watchdog Timer disabled 
#pragma config LVP = OFF // Low-Voltage Programming disabled 

// Define LCD connections 
#define LCD_DATA PORTD 
#define ctrl PORTE 
#define rs PORTEbits.RE0 
#define rw PORTEbits.RE1 
#define en PORTEbits.RE2 

// Function Prototypes 
void init_LCD(void);
void LCD_command(unsigned char cmd);
void LCD_data(unsigned char data);
void LCD_write_string(char *str);
void msdelay(unsigned int time);

void main(void) {
    char var1[] = "WEL-COME";
    char var2[] = "TE-ENTC";
    ADCON1 = 0x0F; // Configure all pins as digital 
    TRISD = 0x00; // PORTD as output (Data bus)
    TRISE = 0x00; // PORTE as output (Control signals)
    init_LCD(); // Initialize LCD
    msdelay(50);
    LCD_write_string(var1); // Write first string
    msdelay(150);
    LCD_command(0xC0); // Move cursor to second line
    LCD_write_string(var2); // Write second string
    while (1); // Infinite loop
}

// ------------------ Delay Function ------------------ 
void msdelay(unsigned int time) {
    unsigned int i, j;
    for (i = 0; i < time; i++) {
        for (j = 0; j < 710; j++); // Adjust for accurate delay (depends on Fosc)
    }
}

// ------------------ LCD Initialization ------------------ 
void init_LCD(void) {
    LCD_command(0x38); // 8-bit, 2-line, 5x7 dots 
    msdelay(15);
    LCD_command(0x01); // Clear display
    msdelay(15);
    LCD_command(0x0C); // Display ON, Cursor OFF
    msdelay(15);
    LCD_command(0x80); // Set cursor at beginning of 1st line
    msdelay(15);
}

// ------------------ Send Command ------------------ 
void LCD_command(unsigned char cmd) {
    LCD_DATA = cmd; // Send command to PORTD
    rs = 0; // RS=0 → Command register
    rw = 0; // RW=0 → Write operation
    en = 1; // Enable pulse
    msdelay(15);
    en = 0;
}

// ------------------ Send Data (Character) ------------------ 
void LCD_data(unsigned char data) {
    LCD_DATA = data; // Send data to PORTD
    rs = 1; // RS=1 → Data register
    rw = 0; // RW=0 → Write operation
    en = 1; // Enable pulse
    msdelay(15);
    en = 0;
}

// ------------------ Write String ------------------ 
void LCD_write_string(char *str) {
    int i = 0;
    while (str[i] != '\0') { 
        LCD_data(str[i]); // Send each character
        msdelay(15);
        i++;
    } 
}
