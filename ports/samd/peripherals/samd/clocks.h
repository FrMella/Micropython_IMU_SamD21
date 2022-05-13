/*Atmel definitions access for ATMEL libraries implement micropython
 *Cawthron Institute 2022
 *FrMella 
 *MICROPY_INCLUDED_ATMEL_SAMD_SAMD51_PERIPHERALS_H
*/
#ifndef MICROPY_INCLUDED_ATMEL_SAMD_CLOCKS_H
#define MICROPY_INCLUDED_ATMEL_SAMD_CLOCKS_H

#include <stdbool.h>
#include <stdint.h>

#include "include/sam.h"

#ifdef SAM_D5X_E5X
#define CLOCK_48MHZ GCLK_GENCTRL_SRC_DFLL_Val
#endif
#ifdef SAMD21
#define CLOCK_48MHZ GCLK_GENCTRL_SRC_DFLL48M_Val
#endif

#define DEFAULT_DFLL48M_FINE_CALIBRATION 512

#define CORE_GCLK 0

uint8_t find_free_gclk(uint16_t divisor);

bool gclk_enabled(uint8_t gclk);
void disable_gclk(uint8_t gclk);
void reset_gclks(void);

void connect_gclk_to_peripheral(uint8_t gclk, uint8_t peripheral);
void disconnect_gclk_from_peripheral(uint8_t gclk, uint8_t peripheral);

void enable_clock_generator(uint8_t gclk, uint32_t source, uint16_t divisor);
void disable_clock_generator(uint8_t gclk);

void clock_init(bool has_crystal, uint32_t dfll48m_fine_calibration);
void init_dynamic_clocks(void);

bool clock_get_enabled(uint8_t type, uint8_t index);
bool clock_get_parent(uint8_t type, uint8_t index, uint8_t *p_type, uint8_t *p_index);
uint32_t clock_get_frequency(uint8_t type, uint8_t index);
uint32_t clock_get_calibration(uint8_t type, uint8_t index);
int clock_set_calibration(uint8_t type, uint8_t index, uint32_t val);

#endif  // MICROPY_INCLUDED_ATMEL_SAMD_CLOCKS_H
