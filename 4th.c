#include <xc.h>

#pragma config FOSC = HS, WDT = OFF, LVP = OFF

void __interrupt(high_priority) timer_isr(void);

void main(void)
{
    ADCON1 = 0x0F;
    TRISBbits.TRISB0 = 0;
    PORTBbits.RB0 = 0;

    T0CON = 0b00000111;   // Timer0 ON, 8-bit mode, prescaler 1:256
    TMR0H = 0xFF;
    TMR0L = 0xEC;

    T0CONbits.TMR0ON = 1;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.GIE = 1;

    while (1) { }
}

void __interrupt(high_priority) timer_isr(void)
{
    if (INTCONbits.TMR0IF)
    {
        TMR0H = 0xFF;
        TMR0L = 0xEC;
        PORTBbits.RB0 = ~PORTBbits.RB0;
        INTCONbits.TMR0IF = 0;
    }
}

/*
--------------------------------------------
   🧠 LOGIC ANALYZER SETUP (MPLAB SIMULATOR)
--------------------------------------------

1. Open MPLAB X IDE.
2. Select "Simulator" as the tool in Project Properties.
3. Build your project (F11) to compile.
4. Go to: Window → Debugging → New Logical Analyzer.
5. In Logic Analyzer → Add Channel → choose PORTB → select bit 0 (RB0).
6. Start simulation:
      • Click Debug Project (F5)
      • Then click Run (F6)
7. In Logic Analyzer window:
      • You’ll see RB0 waveform (square wave)
      • Zoom out if waveform looks flat (it’s toggling fast)
8. Each Timer0 interrupt toggles RB0 → forms a square wave.
9. If nothing appears:
      • Make sure INTCONbits.PEIE = 1;  (Peripheral interrupt enable)
      • Ensure INTCONbits.GIE = 1;      (Global interrupt enable)
      • Confirm TMR0ON = 1;             (Timer running)
10. For slower visible waveform:
      • Increase delay (change preload values TMR0H/TMR0L)

--------------------------------------------
*/

