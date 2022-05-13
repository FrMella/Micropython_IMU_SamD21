

#ifndef MICROPY_INCLUDED_ATMEL_SAMD_PERIPHERALS_DMA_H
#define MICROPY_INCLUDED_ATMEL_SAMD_PERIPHERALS_DMA_H

#include <stdbool.h>
#include <stdint.h>

#include "include/sam.h"

// We allocate DMA resources for the entire lifecycle of the board (not the
// vm) because the general_dma resource will be shared between the REPL and SPI
// flash. Both uses must block each other in order to prevent conflict.
#define AUDIO_DMA_CHANNEL_COUNT 3
#define DMA_CHANNEL_COUNT (AUDIO_DMA_CHANNEL_COUNT + 2)
#define SHARED_TX_CHANNEL (DMA_CHANNEL_COUNT - 2)
#define SHARED_RX_CHANNEL (DMA_CHANNEL_COUNT - 1)

void init_shared_dma(void);

uint8_t sercom_index(Sercom* sercom);

int32_t sercom_dma_write(Sercom* sercom, const uint8_t* buffer, uint32_t length);
int32_t sercom_dma_read(Sercom* sercom, uint8_t* buffer, uint32_t length, uint8_t tx);
int32_t sercom_dma_transfer(Sercom* sercom, const uint8_t* buffer_out, uint8_t* buffer_in, uint32_t length);

void dma_configure(uint8_t channel_number, uint8_t trigsrc, bool output_event);
void dma_enable_channel(uint8_t channel_number);
void dma_disable_channel(uint8_t channel_number);
void dma_suspend_channel(uint8_t channel_number);
void dma_resume_channel(uint8_t channel_number);
bool dma_channel_free(uint8_t channel_number);
bool dma_channel_enabled(uint8_t channel_number);
uint8_t dma_transfer_status(uint8_t channel_number);
DmacDescriptor* dma_descriptor(uint8_t channel_number);
DmacDescriptor* dma_write_back_descriptor(uint8_t channel_number);

#endif  // MICROPY_INCLUDED_ATMEL_SAMD_PERIPHERALS_DMA_H
