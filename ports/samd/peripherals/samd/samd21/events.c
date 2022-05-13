// ATMEL libraries

#include "samd/events.h"

#include "samd/clocks.h"

#include "py/runtime.h"

#include "hpl/pm/hpl_pm_base.h"

void turn_on_event_system(void) {
    _pm_enable_bus_clock(PM_BUS_APBC, EVSYS);
}

void reset_event_system(void) {
    EVSYS->CTRL.bit.SWRST = true;
    _pm_disable_bus_clock(PM_BUS_APBC, EVSYS);
}

bool event_channel_free(uint8_t channel) {
    uint8_t generator;
    // Explicitly do a byte write so the peripheral knows we're just wanting to read the channel
    // rather than write to it.
    *((uint8_t*) &EVSYS->CHANNEL.reg) = channel;
    generator = (EVSYS->CHANNEL.reg & EVSYS_CHANNEL_EVGEN_Msk) >> EVSYS_CHANNEL_EVGEN_Pos;
    return generator == 0;
}

void disable_event_channel(uint8_t channel_number) {
    EVSYS->CHANNEL.reg = EVSYS_CHANNEL_CHANNEL(channel_number);
}

void disable_event_user(uint8_t user_number) {
    EVSYS->USER.reg = EVSYS_USER_USER(user_number);
}

void connect_event_user_to_channel(uint8_t user, uint8_t channel) {
    EVSYS->USER.reg = EVSYS_USER_USER(user) | EVSYS_USER_CHANNEL(channel + 1);
}

void init_async_event_channel(uint8_t channel, uint8_t generator) {
    EVSYS->CHANNEL.reg = EVSYS_CHANNEL_CHANNEL(channel) |
                         EVSYS_CHANNEL_EVGEN(generator) |
                         EVSYS_CHANNEL_PATH_ASYNCHRONOUS;
}

void init_event_channel_interrupt(uint8_t channel, uint8_t gclk, uint8_t generator) {
    connect_gclk_to_peripheral(gclk, EVSYS_GCLK_ID_0 + channel);
    EVSYS->CHANNEL.reg = EVSYS_CHANNEL_CHANNEL(channel) |
                         EVSYS_CHANNEL_EVGEN(generator) |
                         EVSYS_CHANNEL_PATH_RESYNCHRONIZED |
                         EVSYS_CHANNEL_EDGSEL_RISING_EDGE;
    if (channel >= 8) {
        uint8_t value = 1 << (channel - 8);
        EVSYS->INTFLAG.reg = EVSYS_INTFLAG_EVDp8(value) | EVSYS_INTFLAG_OVRp8(value);
        EVSYS->INTENSET.reg = EVSYS_INTENSET_EVDp8(value) | EVSYS_INTENSET_OVRp8(value);
    } else {
        uint8_t value = 1 << channel;
        EVSYS->INTFLAG.reg = EVSYS_INTFLAG_EVD(value) | EVSYS_INTFLAG_OVR(value);
        EVSYS->INTENSET.reg = EVSYS_INTENSET_EVD(value) | EVSYS_INTENSET_OVR(value);
    }
}

bool event_interrupt_active(uint8_t channel) {
    bool active = false;
    if (channel >= 8) {
        uint8_t value = 1 << (channel - 8);
        active = (EVSYS->INTFLAG.reg & EVSYS_INTFLAG_EVDp8(value)) != 0;
        // Only clear if we know its active, otherwise there is the possibility it becomes active
        // after we check but before we clear.
        if (active) {
            EVSYS->INTFLAG.reg = EVSYS_INTFLAG_EVDp8(value) | EVSYS_INTFLAG_OVRp8(value);
        }
    } else {
        uint8_t value = 1 << channel;
        active = (EVSYS->INTFLAG.reg & EVSYS_INTFLAG_EVD(value)) != 0;
        if (active) {
            EVSYS->INTFLAG.reg = EVSYS_INTFLAG_EVD(value) | EVSYS_INTFLAG_OVR(value);
        }
    }
    return active;
}

bool event_interrupt_overflow(uint8_t channel) {
    bool overflow = false;
    if (channel >= 8) {
        uint8_t value = 1 << (channel - 8);
        overflow = (EVSYS->INTFLAG.reg & EVSYS_INTFLAG_OVRp8(value)) != 0;
    } else {
        uint8_t value = 1 << channel;
        overflow = (EVSYS->INTFLAG.reg & EVSYS_INTFLAG_OVR(value)) != 0;
    }
    return overflow;
}
