// ATMEL libraries


#include <stdbool.h>
#include <stdint.h>

#include "samd/timers.h"

#include "timer_handler.h"

#include "hpl/gclk/hpl_gclk_base.h"

const uint8_t tcc_cc_num[3] = {4, 2, 2};
const uint8_t tc_gclk_ids[TC_INST_NUM] = {TC3_GCLK_ID,
               TC4_GCLK_ID,
               TC5_GCLK_ID,
#ifdef TC6_GCLK_ID
               TC6_GCLK_ID,
#endif
#ifdef TC7_GCLK_ID
               TC7_GCLK_ID,
#endif
            };
const uint8_t tcc_gclk_ids[3] = {TCC0_GCLK_ID, TCC1_GCLK_ID, TCC2_GCLK_ID};

void turn_on_clocks(bool is_tc, uint8_t index, uint32_t gclk_index) {
    uint8_t gclk_id;
    if (is_tc) {
        gclk_id = tc_gclk_ids[index];
    } else {
        gclk_id = tcc_gclk_ids[index];
    }
    // Determine the clock slot on the APBC bus. TCC0 is the first and 8 slots in.
    uint8_t clock_slot = 8 + index;
    // We index TCs starting at zero but in memory they begin at three so we have to add three.
    if (is_tc) {
        clock_slot += 3;
    }
    PM->APBCMASK.reg |= 1 << clock_slot;
    _gclk_enable_channel(gclk_id, gclk_index);
}

void tc_set_enable(Tc* tc, bool enable) {
    tc->COUNT16.CTRLA.bit.ENABLE = enable;
    while (tc->COUNT16.STATUS.bit.SYNCBUSY != 0) {
        /* Wait for sync */
    }
}

void tc_wait_for_sync(Tc* tc) {
    while (tc->COUNT16.STATUS.bit.SYNCBUSY != 0) {}
}
