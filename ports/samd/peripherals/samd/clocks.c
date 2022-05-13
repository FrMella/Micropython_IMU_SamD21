/*Atmel definitions access for ATMEL libraries implement micropython
 *Cawthron Institute 2022
 *FrMella
*/

#include "clocks.h"

#include "hpl_gclk_config.h"

#include "shared-bindings/microcontroller/__init__.h"

#include "py/runtime.h"

uint8_t find_free_gclk(uint16_t divisor) {
    if (divisor > 0xff) {
        if (gclk_enabled(1)) {
            return 0xff;
        }
        return 1;
    }
    uint8_t first_8_bit = 2;
    #ifdef SAMD21
    first_8_bit = 3;
    if (divisor <= (1 << 5) && !gclk_enabled(2)) {
        return 2;
    }
    #endif
    for (uint8_t i = first_8_bit; i < GCLK_GEN_NUM; i++) {
        if (!gclk_enabled(i)) {
            return i;
        }
    }
    return 0xff;
}

static uint8_t last_static_clock = 0;

void init_dynamic_clocks(void) {
    for (uint8_t i = 0; i < GCLK_GEN_NUM; i++) {
        if (gclk_enabled(i)) {
            last_static_clock = i;
        }
    }
}

void reset_gclks(void) {
    for (uint8_t i = last_static_clock + 1; i < GCLK_GEN_NUM; i++) {
        disable_gclk(i);
    }
}
