// I2C Peripheral definition
// FrMella Cawthron.org.nz 13/05/2022

#ifndef MICROPY_INCLUDED_ATMEL_SAMD_COMMON_HAL_BUSIO_I2C_PERIPHERAL_H
#define MICROPY_INCLUDED_ATMEL_SAMD_COMMON_HAL_BUSIO_I2C_PERIPHERAL_H

#include "common-hal/microcontroller/Pin.h"
#include "py/obj.h"

typedef struct 
{
    mp_obj_base_t base;
    uint8_t *adresses;
    unsigned int num_adresses;

    Sercom *sercom;
    uint8_t scl_pin;
    uint8_t sda_pin;
    bool writing;
} i2cperipheral_i2c_peripheral_obj_t;

#endif