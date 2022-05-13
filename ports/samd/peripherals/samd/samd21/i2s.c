// ATMEL libraries
#include "samd/i2s.h"

#include "samd/clocks.h"

#include "hpl/gclk/hpl_gclk_base.h"
#include "hpl/pm/hpl_pm_base.h"

void turn_on_i2s(void) {
    _pm_enable_bus_clock(PM_BUS_APBC, I2S);
}

void i2s_set_serializer_enable(uint8_t serializer, bool enable) {
    while ((I2S->SYNCBUSY.vec.SEREN & (1 << serializer)) != 0) {}
    if (enable) {
        I2S->CTRLA.vec.SEREN = 1 << serializer;
    } else {
        I2S->CTRLA.vec.SEREN &= ~(1 << serializer);
    }
    while ((I2S->SYNCBUSY.vec.SEREN & (1 << serializer)) != 0) {}
}
