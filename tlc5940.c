#include <xc.h>
#include "tlc5940.h"

void TLC5940_Init(void) {
    // Set as outputs
    __TLC_BLANK_DIR = __TLC_OUTPUT;
    __TLC_GSCLK_DIR = __TLC_OUTPUT;
    __TLC_SCLK_DIR = __TLC_OUTPUT;
    __TLC_SIN_DIR = __TLC_OUTPUT;
    __TLC_VPRG_DIR = __TLC_OUTPUT;
    __TLC_XLAT_DIR = __TLC_OUTPUT;
    // Initial values from programming flow chart
    __TLC_BLANK =__TLC_HIGH;
    __TLC_GSCLK =__TLC_LOW;
    __TLC_SCLK =__TLC_LOW;
    __TLC_SIN =__TLC_LOW;
    __TLC_VPRG =__TLC_HIGH;
    __TLC_XLAT =__TLC_LOW;
}

