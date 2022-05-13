/*Atmel definitions access for ATMEL libraries implement micropython
 *Cawthron Institute 2022
 *FrMella
 *MICROPY_INCLUDED_ATMEL_SAMD_PERIPHERALS_ADC_H 
*/
#ifndef MICROPY_INCLUDED_ATMEL_SAMD_PERIPHERALS_ADC_H
#define MICROPY_INCLUDED_ATMEL_SAMD_PERIPHERALS_ADC_H

#include "include/sam.h"
#include "hal/include/hal_adc_sync.h"

void samd_peripherals_adc_setup(struct adc_sync_descriptor *adc, Adc *instance);

#endif  
