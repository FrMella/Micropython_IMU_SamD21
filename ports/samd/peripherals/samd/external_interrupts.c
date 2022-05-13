

#include "common-hal/pulseio/PulseIn.h"
#include "common-hal/rotaryio/IncrementalEncoder.h"
#include "shared-bindings/microcontroller/__init__.h"
#include "samd/external_interrupts.h"

#include "sam.h"

// Esta estructura se utiliza para compartir almacenamiento por canal entre todos los usuarios de interrupciones externas.
// Sin esto, habría múltiples arreglos aunque no estén unidos porque cada canal tiene un usuario a la vez

static void *channel_data[EIC_EXTINT_NUM];

void external_interrupt_handler(uint8_t channel) {
    shared_eic_handler(channel);
    EIC->INTFLAG.reg = (1 << channel) << EIC_INTFLAG_EXTINT_Pos;
}

void configure_eic_channel(uint8_t eic_channel, uint32_t sense_setting) {
    uint8_t config_index = eic_channel / 8;
    uint8_t position = (eic_channel % 8) * 4;
    #ifdef SAM_D5X_E5X
    eic_set_enable(false);
    #endif
    common_hal_mcu_disable_interrupts();
    uint32_t masked_value = EIC->CONFIG[config_index].reg & ~(0xf << position);
    EIC->CONFIG[config_index].reg = masked_value | (sense_setting << position);
    common_hal_mcu_enable_interrupts();
    #ifdef SAM_D5X_E5X
    eic_set_enable(true);
    #endif
}

void turn_on_eic_channel(uint8_t eic_channel, uint32_t sense_setting) {
    // We do very light filtering using majority voting.
    sense_setting |= EIC_CONFIG_FILTEN0;
    configure_eic_channel(eic_channel, sense_setting);
    uint32_t mask = 1 << eic_channel;
    EIC->INTENSET.reg = mask << EIC_INTENSET_EXTINT_Pos;
    turn_on_cpu_interrupt(eic_channel);
}

void turn_off_eic_channel(uint8_t eic_channel) {
    uint32_t mask = 1 << eic_channel;
    EIC->INTENCLR.reg = mask << EIC_INTENSET_EXTINT_Pos;
    #ifdef SAM_D5X_E5X
    NVIC_DisableIRQ(EIC_0_IRQn + eic_channel);
    NVIC_ClearPendingIRQ(EIC_0_IRQn + eic_channel);
    #endif
    channel_data[eic_channel] = NULL;

    #ifdef SAMD21
    if (EIC->INTENSET.reg == 0) {
        NVIC_DisableIRQ(EIC_IRQn);
        NVIC_ClearPendingIRQ(EIC_IRQn);
    }
    #endif
    // Test if all channels are null and deinit everything if they are.
    if (EIC->EVCTRL.reg == 0 && EIC->INTENSET.reg == 0) {
        turn_off_external_interrupt_controller();
    }
}

void* get_eic_channel_data(uint8_t eic_channel) {
    return channel_data[eic_channel];
}

void set_eic_channel_data(uint8_t eic_channel, void* data) {
    channel_data[eic_channel] = data;
}
