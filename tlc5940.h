#ifndef TLC5940_H
#define TLC5940_H

#include <xc.h>

// Number of TLC5940s to be used (default: 1)
#ifndef TLC5940_N
#define TLC5940_N 1
#endif

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

#define __TLC_OUTPUT 0
#define __TLC_INPUT 1
#define __TLC_LOW 0
#define __TLC_HIGH 1

void TLC5940_Init(void);

#endif
