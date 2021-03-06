// TLC5940.h
// Author: Jordan Gonzalez
// TLC5940 library function
// prototypes and definitions

#ifndef TLC5940_H
#define TLC5940_H

#include <xc.h>

// Number of TLC5940s to be used (default: 1)
#ifndef TLC5940_N
#define TLC5940_N 1
#endif

#define __TLC_LED_N 16 * TLC5940_N

// PIC16F877 Serial Peripheral Interface pins
#define SPI_CLK         TRISC3  // RC3
#define SPI_SDI         TRISC4  // RC4
#define SPI_SDO         TRISC5  // RC5
#define SPI_SS          TRISA5  // RA5


// GSCLK requires CCP1.
// CCP1 maps to RC2 (See datasheet)
// GSCLK must be mapped to RC2.
#define __TLC_GSCLK_DIR  TRISC2
#define __TLC_GSCLK      RC2

// SCLK requires SCK
// SCK maps to RC3 (See datasheet)
// SCLK must be mapped to RC3.
#define __TLC_SCLK_DIR   TRISC3
#define __TLC_SCLK       RC3

// SIN requires SDO
// SDO maps to RC5 (See datasheet)
// SIN must be mapped to RC5.
#define __TLC_SIN_DIR    TRISC5
#define __TLC_SIN        RC5

// XLAT has no requirements.
// XLAT can be remapped if necessary.
#define __TLC_XLAT_DIR	TRISB0
#define __TLC_XLAT	    RB0

// BLANK has no requirements.
// BLANK can be remapped if necessary.
#define __TLC_BLANK_DIR	TRISB1
#define __TLC_BLANK	    RB1

// VPRG has no requirements.
// VPRG can be remapped if necessary.
#define __TLC_VPRG_DIR	TRISB2
#define __TLC_VPRG	    RB2

#if (12 * TLC5940_N > 255)
#define dcData_t unsigned short
#else
#define dcData_t unsigned char
#endif

#if (24 * TLC5940_N > 255)
#define gsData_t unsigned short
#else
#define gsData_t unsigned char
#endif

#define __TLC_DC_SIZE 12
#define __TLC_GS_SIZE 24
#define __TLC_DC_COUNTER_MAX (__TLC_DC_SIZE * TLC5940_N)
#define __TLC_DATA_COUNTER_MAX (12)

// MIC5891 mapping
#define SOUT_MIC RD2    // Serial out MIC5891
#define CLK_MIC RD0     // Clock MIC5891
#define STROBE_MIC RD1  // Strobe MIC5891

/// MSB--------->LSB
unsigned char dcData[__TLC_DC_SIZE * TLC5940_N] = {
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111
};



// Channel 0 - 1111 1111 1111
// Channel 1 - 1000 0000 0000
// Channel 2 - 0100 0000 0000
// Channel 3 - 0010 0000 0000
unsigned char gsData[8][__TLC_DATA_COUNTER_MAX] = {
    {
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000
    }, {
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000
    }, {
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000
    }, {
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000
    }, {
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000
    }, {
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000
    }, {
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000
    }, {
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000,
        0b00000000, 0b00000000
    }
};

void TLC5940_Init(void);
void TLC5940_ClockInDC(void);
void TLC5940_PulseXLAT(void);
void TLC5940_PulseSCLK(void);

// PIC16F877 SPI - Serial Peripheral Interface
void SPI_Init(void);
int SPI_Write(unsigned char data);
//

// GSCLK Timer Interrupt
void Timer2_Init(void);
// XLAT Pulse Interrupt
void Timer0_Init(void);

unsigned short ChIdx(unsigned char channel);
void PutPixel(unsigned char y, unsigned char x, unsigned short value);
void ClearScreen(void);

// MIC5891 functions
void strobe();
void shiftData(unsigned char shiftRegister);

unsigned char maxRow;

#endif
