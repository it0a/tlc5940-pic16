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
    __TLC_BLANK = __TLC_HIGH;
    __TLC_GSCLK = __TLC_LOW;
    __TLC_SCLK = __TLC_LOW;
    __TLC_SIN = __TLC_LOW;
    __TLC_VPRG = __TLC_HIGH;
    __TLC_XLAT = __TLC_LOW;
    //
    TLC5940_ClockInDC();
}

void TLC5940_ClockInDC(void) {
    unsigned char counter = 0;
    __TLC_SIN = __TLC_HIGH;
    __TLC_VPRG = __TLC_HIGH;
    for(;;) {
        if (counter > __TLC_DC_COUNTER_MAX) {
            __TLC_XLAT = __TLC_HIGH;
            __TLC_XLAT = __TLC_LOW;
            break;
        } else {
            if (dcData[counter]) {
                __TLC_SIN = __TLC_HIGH;
            } else {
                __TLC_SIN = __TLC_LOW;
            }
            __TLC_SCLK = __TLC_HIGH;
            __TLC_SCLK = __TLC_LOW;
            counter++;
        }
    }
}

void TLC5940_SetGS_GW_PWM(void) {
    unsigned char firstCycleFlag = 0;
    unsigned short GSCLK_Counter = 0;
    unsigned char Data_Counter = 0;
    if (__TLC_VPRG == __TLC_HIGH) {
        __TLC_VPRG = __TLC_LOW;
        firstCycleFlag = 1;
    }
    __TLC_BLANK = __TLC_LOW;
    for (;;) {
        if (GSCLK_Counter > __TLC_GSCLK_COUNTER_MAX) {
            __TLC_BLANK = __TLC_HIGH;
            __TLC_XLAT = __TLC_HIGH;
            __TLC_XLAT = __TLC_LOW;
            if (firstCycleFlag) {
                __TLC_SCLK = __TLC_HIGH;
                __TLC_SCLK = __TLC_LOW;
                firstCycleFlag = 0;
            }
            break;
        } else {
            if (!(Data_Counter > __TLC_DATA_COUNTER_MAX)) {
                if (1) {
                    __TLC_SIN = __TLC_HIGH;
                } else {
                    __TLC_SIN = __TLC_LOW;
                }
                __TLC_SCLK = __TLC_HIGH;
                __TLC_SCLK = __TLC_LOW;
                Data_Counter++;
            }
        }
        __TLC_GSCLK = __TLC_HIGH;
        __TLC_GSCLK = __TLC_LOW;
        GSCLK_Counter++;
    }
}

