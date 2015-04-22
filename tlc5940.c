#include <xc.h>
#include "tlc5940.h"

void TLC5940_Init(void) {
    // Set as outputs
    __TLC_BLANK_DIR = 0;
    __TLC_GSCLK_DIR = 0;
    __TLC_SCLK_DIR = 0;
    __TLC_SIN_DIR = 0;
    __TLC_VPRG_DIR = 0;
    __TLC_XLAT_DIR = 0;
    // Initial values from programming flow chart
    __TLC_BLANK = 1;
    __TLC_GSCLK = 0;
    __TLC_SCLK = 0;
    __TLC_SIN = 0;
    __TLC_VPRG = 1;
    __TLC_XLAT = 0;
    GIE = 1;
    //
    SPI_Init();
    TLC5940_ClockInDC();
    Timer2_Init();
    Timer0_Init();
}

void TLC5940_PulseXLAT(void) {
    __TLC_XLAT = 1;
    __TLC_XLAT = 0;
}

void TLC5940_PulseSCLK(void) {
    __TLC_SCLK = 1;
    __TLC_SCLK = 0;
}

void TLC5940_ClockInDC(void) {
    __TLC_SIN = 1;
    __TLC_VPRG = 1;
    for(dcData_t i = 0; i < __TLC_DC_COUNTER_MAX; i++) {
        SPI_Write(dcData[i]);
    }
    TLC5940_PulseXLAT();
}

unsigned char xlatNeedsPulse = 0;
void interrupt high_isr(void) {
    unsigned char firstCycleFlag = 0;
    __TLC_BLANK = 1;
    if (__TLC_VPRG == 1) {
        __TLC_VPRG = 0;
        firstCycleFlag = 1;
    }
    if (firstCycleFlag) {
        TLC5940_PulseSCLK();
        firstCycleFlag = 0;
    }
    if (xlatNeedsPulse == 1) {
        TLC5940_PulseXLAT();
        xlatNeedsPulse = 0;
    }
    __TLC_BLANK = 0;
    for (int i = 0; i < __TLC_DATA_COUNTER_MAX; i++) {
        SPI_Write(gsData[i]);
    }
    xlatNeedsPulse = 1;
    // Clear interrupt flag so TMR0 can interrupt again
    TMR0IF = 0;
}

// SPI
void SPI_Init(void) {
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

int SPI_Write(unsigned char data) {
    unsigned char TempVar;
    TempVar = SSPSTATbits.BF; // Clears BF
    PIR1bits.SSPIF = 0; // Clear interrupt flag
    SSPBUF = data; // write byte to SSP1BUF register
    while(!PIR1bits.SSPIF) {}; // wait until bus cycle complete
    return ( 0 ); // if WCOL bit is not set return non-negative#
}
//
// Timer2 - Pulses GSCLK
void Timer2_Init(void) {
    PR2 = 0b00000011;
    CCPR1L = 0b00000001;
    CCP1CON = 0b00011100;
    TMR2 = 0x00;
    T2CON = 0b00000100;
    TMR2IF = 0;
    TMR2IE = 0;
}

void Timer0_Init(void) {
    TMR0 = 0x00;
    PSA = 0; // Prescale is assigned to Timer0
    PS2 = 1; // Prescaler Rate Select bits
    PS1 = 0; // Prescaler Rate Select bits
    PS0 = 1; // Prescaler Rate Select bits
    T0SE = 0; // Increment on low-to-high transition on T0CKI pin
    T0CS = 0; // Transition on internal instruction cycle clock
    TMR0IE = 1; // Enable interrupts on Timer0
    T0IF = 0; // Clear interrupt flag for initialization
}

unsigned short ChIdx(unsigned char channel) {
    return sizeof(gsData) - 1 - ((12 * channel)/8);
}

void SetChannel(unsigned char channel, unsigned short brightness) {
    if (channel % 2 == 0) {
        gsData[ChIdx(channel)] = brightness & 0xFF;
        gsData[ChIdx(channel) - 1] = (gsData[ChIdx(channel) - 1] & 0xF0) | ((brightness >> 8) & 0x0F);
    } else {
        gsData[ChIdx(channel)] = ((brightness << 4) & 0xF0) | (gsData[ChIdx(channel)] & 0x0F);
        gsData[ChIdx(channel) - 1] = (brightness >> 4) & 0xFF;
    }
}
