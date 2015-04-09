#ifndef TLC5940_H
#define TLC5940_H

#include <p16f877.h>

// GSCLK requires CCP1.
// CCP1 maps to RC2 (See datasheet)
// GSCLK must be mapped to RC2.
#define GSCLK_PORT  PORTCbits.RC2

// SCLK requires SCK
// SCK maps to RC3 (See datasheet)
// SCLK must be mapped to RC3.
#define SCLK_PORT   PORTCbits.RC3

// SIN requires SDO
// SDO maps to RC3 (See datasheet)
// SCLK must be mapped to RC5.
#define SIN_PORT    PORTCbits.RC5

// XLAT has no requirements.
// XLAT can be remapped if necessary.
#define XLAT_PORT	PORTAbits.RA0

// BLANK has no requirements.
// BLANK can be remapped if necessary.
#define BLANK_PORT	PORTAbits.RA1

// VPRG has no requirements.
// VPRG can be remapped if necessary.
#define VPRG_PORT	PORTAbits.RA2

void TLC5940_Init(void);

#endif
