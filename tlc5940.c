// TLC5940.c
// TLC5940 library functions

#include <xc.h>
#include "tlc5940.h"

unsigned char curRow = 0;
unsigned char maxRow = 0;

// void TLC5940_Init(void)
// Initializes the TLC5940 by
// setting the correct outputs.
// Also sets initial values via
// the TLC5940 programming flow
// chart.
void TLC5940_Init(void) {
    // Set as outputs
    __TLC_BLANK_DIR = 0;
    __TLC_GSCLK_DIR = 0;
    __TLC_SCLK_DIR = 0;
    __TLC_SIN_DIR = 0;
    __TLC_VPRG_DIR = 0;
    __TLC_XLAT_DIR = 0;
    TRISD1 = 0;
    // Initial values from TLC5940 programming flow chart
    __TLC_BLANK = 1;
    __TLC_GSCLK = 0;
    __TLC_SCLK = 0;
    __TLC_SIN = 0;
    __TLC_VPRG = 1;
    __TLC_XLAT = 0;

    // Global interrupt enable
    // must be set or no interrupts
    // will occur.
    GIE = 1;

    // Intialize Serial Peripheral Interface
    SPI_Init();
    // Initialize Clock for DC data
    TLC5940_ClockInDC();
    // Initialize Timer2 to use GSCLK
    Timer2_Init();
    // Initialize Timer0 to use XLAT
    Timer0_Init();
}

// void TLC5940_PulseXLAT(void)
// Pulses XLAT signal
void TLC5940_PulseXLAT(void) {
    __TLC_XLAT = 1;
    __TLC_XLAT = 0;
}

// void TLC5940_PulseSCLK(void)
// Pulses SCLK signal
void TLC5940_PulseSCLK(void) {
    __TLC_SCLK = 1;
    __TLC_SCLK = 0;
}

// void TLC5940_ClockInDC(void)
// Reads in Clock DC signal. Calls
// SPI_Write function in order to write
// serial data. Then pulses XLAT to
// write data.
void TLC5940_ClockInDC(void) {
    __TLC_SIN = 1;
    __TLC_VPRG = 1;
    for(dcData_t i = 0; i < __TLC_DC_COUNTER_MAX; i++)
    {
        SPI_Write(dcData[i]);
    }
    TLC5940_PulseXLAT();
}

unsigned char xlatNeedsPulse = 0;


// void interrupt high_isr(void)
// Interrupt-driven function. Whenever GSCLK reaches 4096,
// XLAT pulses. So GSCLK is running at a rate of 230 KHz
// and XLAT at 56 KHz. (230 KHz)/4096 = 56 Hz
void interrupt high_isr(void)
{
    // Restart if at the last row
    if (curRow > 7) {
        curRow = 0;
    }

    // Shift the data to the left by 1
    // on each row. This will address each
    // successive LED
    shiftData(1 << curRow++);

    // Keep track of first cycle
    unsigned char firstCycleFlag = 0;
    __TLC_BLANK = 1;

    if (__TLC_VPRG == 1)
    {
        __TLC_VPRG = 0;
        firstCycleFlag = 1;
    }
    if (firstCycleFlag)
    {
        TLC5940_PulseSCLK();
        firstCycleFlag = 0;
    }
    if (xlatNeedsPulse == 1)
    {
        TLC5940_PulseXLAT();
        xlatNeedsPulse = 0;
    }
    __TLC_BLANK = 0;

    // Write GS data
    for (int i = 0; i < __TLC_DATA_COUNTER_MAX; i++) {
        SPI_Write(gsData[curRow][i]);
    }
    xlatNeedsPulse = 1;
    // Clear interrupt flag so TMR0 can interrupt again
    TMR0IF = 0;
}

// void SPI_Init(void)
// Initializes Serial Peripheral Interface
// on the PIC16F877 in order to write serial
// data to the TLC5940.
void SPI_Init(void)
{
    SPI_CLK = 0; //output
    SPI_SDI = 1; //input
    SPI_SDO = 0; //output
    SPI_SS = 0; //output
    SSPCONbits.SSPEN = 0;
    SSPSTATbits.CKE = 1;    //data transfer on rising edge clk
    SSPSTATbits.SMP = 1;
    SSPCONbits.SSPM = 0;    //SPI Master mode, clock = FOSC/4
    SSPCONbits.CKP = 0; //Idle state for clock is a low level (Microwire alternate)
    SSPCONbits.SSPEN = 1;
    PIR1bits.SSPIF = 0; // Clear interrupt flag
}

// int SPI_Write(unsigned char data)
// Writes data via the Serial Peripheral Interface
// on the PIC16F877.
int SPI_Write(unsigned char data)
{
    unsigned char TempVar;
    TempVar = SSPSTATbits.BF; // Clears BF
    PIR1bits.SSPIF = 0; // Clear interrupt flag
    SSPBUF = data; // write byte to SSP1BUF register
    while(!PIR1bits.SSPIF) {}; // wait until bus cycle complete
    return ( 0 ); // if WCOL bit is not set return non-negative#
}

// void Timer2_Init(void)
// Initializes Timer2 to
// pulse GSCLK at a rate of 230 KHz.
void Timer2_Init(void)
{
    PR2 = 0b00000011; // Timer2 period for use in PWM
                      // Equation is as follows:
                      // PWM period = ((PR2+1)*4*Tosc*TMR2_Prescale_Value)
    CCPR1L = 0b00000001;
    CCP1CON = 0b00011100;
    TMR2 = 0x00;
    T2CON = 0b00000100; // Timer2 has a prescale of 1
                        // from setting T2CKPS1:T2CKPS0
                        // to 0:0
    TMR2IF = 0; // Disable interrupts on Timer2
    TMR2IE = 0; // Clear interrupt flag for initialization
}

// void Timer0_Init(void)
// Initializes Timer0 to
// pulse XLAT at a rate of 56 Hz
void Timer0_Init(void) {
    TMR0 = 0x00;
    PSA = 0; // Prescale is assigned to Timer0
    PS2 = 0; // Prescaler Rate Select bits
    PS1 = 1; // Prescaler Rate Select bits
    PS0 = 0; // Prescaler Rate Select bits
    T0SE = 0; // Increment on low-to-high transition on T0CKI pin
    T0CS = 0; // Transition on internal instruction cycle clock
    TMR0IE = 1; // Enable interrupts on Timer0
    T0IF = 0; // Clear interrupt flag for initialization
}

// unsigned short ChIdx(unsigned char channel)
// Returns the number of bytes of the grayscale data
// corresponding to a channel on the LED matrix
unsigned short ChIdx(unsigned char channel)
{
    return sizeof(gsData) - 1 - ((12 * channel)/8);
}

// void SetChannel(unsigned char channel, unsigned short brightness)
// Set appropriate channel to the appropriate brightness
void PutPixel(unsigned char x, unsigned char y, unsigned short brightness)
{
    if (x % 2 == 0)
    {
        gsData[y][ChIdx(x)] = brightness & 0xFF;
        gsData[y][ChIdx(x) - 1] = (gsData[y][ChIdx(x) - 1] & 0xF0) | ((brightness >> 8) & 0x0F);
    }
    else
    {
        gsData[y][ChIdx(x)] = ((brightness << 4) & 0xF0) | (gsData[y][ChIdx(x)] & 0x0F);
        gsData[y][ChIdx(x) - 1] = (brightness >> 4) & 0xFF;
    }
}

// MIC5891 functions

// void strobe()
// Pulses strobe on the MIC5891
 void strobe()
 {
     STROBE_MIC = 1;
     STROBE_MIC = 0;
 }

 // void shiftData(unsigned char shiftRegister)
 // Writes and shifts data via the Serial Peripheral
 // Interface on the PIC16F877. Strobe must be pulsed
 // at the end of the write in order for data to get
 // clocked in.
 void shiftData(unsigned char shiftRegister)
{
    SPI_Write(shiftRegister);
    strobe();
}
