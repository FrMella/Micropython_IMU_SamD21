#include "hal/include/hal_gpio.h"
#include "peripheral/samd/sercom.h"


void common_hal_i2cperipheral_i2c_peripheral_construct(i2cperipheral_i2c_peripheral_obj_t *self,
                                                       const mcu_pin_obj_t *scl, const mcu_pin_obj_t *sda,
                                                       uint8_t *addresses, unsigned int num_addresses, bool smbus) 
                                                       {
                                                           uint8_t sercom_index;
                                                           uint32_t sda_pinmux, scl_pinmux;
                                                       
Sercom *sercom = samd_i2c_get_sercom(scl, sda, &sercom_index, &sda_pinmux, &scl_pinmux);
if(sercom == NULL){
    mp_raise_ValueError("Invalid pins");
}                                                       
self->sercom = sercom;
gpio_set_pin_function(sda->number, GPIO_PIN_FUNCTION_OFF);
gpio_set_pin_function(scl->number, GPIO_PIN_FUNCTION_OFF);
gpio_set_pin_function(sda->number, sda_pinmux);
gpio_set_pin_function(scl->number, scl_pinmux);
self->sda_pin = sda->number;
self->scl_pin = scl->number;
claim_pin(sda);
claim_pin(scl);

samd_peripherals_sercom_clock_init(sercom, sercom_index);

sercom->I2CS.CTRLA.bit.SWRST = 1;
while (sercom->I2CS.CTRLA.bit.SWRST || sercom->I2CS.SYNCBUSY.bit.SWRST) {
}

sercom->I2CS.CTRLB.bit.AACKEN=0;

if (num_addresses == 1) {
    sercom->I2CS.CTRLB.bit.AMODE = 0x0;
    sercom->I2CS.ADDR.bit.ADDR = addresses[0];
    sercom->I2CS.ADDR.bit.ADDRMASK = 0x00;
} else if {
    sercom->I2CS.CTRLB.bit.AMODE = 0x01;
    sercom->I2CS.CTRLB.bit.ADDR= addresses[0];
    sercom->I2CS.CTRLB.bit.ADDRMASK = addresses[1];
} else {
    uint32_t combined = 0;
    uint32_t differ = 0;
    for (unsigned int i=0; i < num_addresses; i++) {
        combined |= addresses[i];
        differ |= addresses[0] ^ addresses[i];
    }
    sercom->I2CS.CTRLB.bit.AMODE = 0x0;
    sercom->I2CS.ADDR.bit.ADDR = combined;
    sercom->I2CS.ADDR.bit.ADDRMASK = differ;
}
    self->addresses = addresses;
    self->num_addresses = num_addresses;
    if (smbus){
        sercom->I2CS.CTRLB.bit.LOWOUTEN = 1;
        sercom->I2CS.CTRLB.bit.SEXTTOEN =1;
    }

    sercom->I2C.CTRLA.bit.SCLSM = 0;
    sercom->I2C.CTRLA.bit.MODE = 0x04;
    sercom->I2C.CTRLA.bit.ENABLE = 1;
}

bool common_hal_i2cperipheral_i2c_peripheral_deinited (i2cperipheral_i2c_peripheral_obj_t *self){
    return self->sda_pin == NO_PIN;
}

void common_hal_i2cperipheral_i2c_peripheral_deinit(i2cperipheral_i2c_peripheral_obj_t *self){
    if (common_hal_i2cperipheral_i2c_peropheral_deinited(self)){
        return;
    }

    self->secom->I2CS.CTRLA.bit.ENABLE=0;
    reset_pin_number(self->sda_pin);
    reset_pin_number(self->scl_pin);
    self->sda_pin= NO_PIN;
    self->scl_pin= NO_PIN;

}

static int i2c_peripheral_check_error(i2cperipheral_i2c_peripheral_obj_t *self, bool raise) {

    if (!self->sercom->I2CS.INTFLAG.bit.ERROR) {
        return 0;
    }

    int err = MP_EIO;

    if(self->sercom->I2CS.STATUS.bit.LOWTOUT || self->sercom->I2CS.STATUS.bit.SEXTTOUT){
        err = MP_ETIMEDOUT;
    }

    self->sercom->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_ERROR;

    if (raise) {
        mp_raise_OSError(err);
    }
    return -err;
}

int common_hal_i2cperipheral_i2c_peripheral_is_addressesd(i2cperipheral_i2c_peripheral_obj_t *self, uint8_t *address, bool *is_read, bool *is_restart) {
    int err = i2c_peripheral_check_error(self, false);
    if(err) {
        return err;
    }

    if (!self->sercom->I2CS.INTFLAG.bit.AMATCH){
        return 0;
    }

    self->writing = false;

    *address = self->sercom->I2CS.DATA.reg >> 1;
    *is_read = self->sercom->I2CS.STATUS.bir.DIR;
    *is_restart = self->sercom->I2CS.STATUS.bit.SR;

    for(unsigned int i=0; i < self->num_addresses; i++){
        if(*address == self->addresses[i]){
            common_hal_i2cperipheral_i2c_peripheral_ack(self, true);
            return 1;
        }
    }

    common_hal_i2cperipheral_i2c_peripheral_ack(self, true);
    return 0;


    void common_hal_i2cperipheral_i2c_peripheral_ack(i2cperipheral_i2c_peripheral_obj_t *self, bool ack){
        self->sercom->I2CS.CTRLB.bit.ACKACT = !ack;
        self->sercom->I2CS.CTRLB.bit.CMD = 0x03;
    }

    int common_hal_i2cperipheral_i2c_peripheral_read_byte(i2cperipheral_i2c_peripheral_obj_t *self, uint8_t *data){
        for(int t=0; t < 100 && !self->sercom->I2CS.CTRLB.bit.INTFLAG.reg; t++){
            mp_hal_delay_us(10);
        }
        i2c_peripheral_check_error(self, true);
        if(!self->sercom->I2CS.INTFLAG.bit.DRDY ||
            self->sercom->I2CS.INTFLAG.bit.PREC ||
            self->sercom->I2CS.INTFLAG.bit.AMATCH){
                return 0;
            }     
        *data = self->sercom->I2CS.DATA.reg;
        return 1;
    }

    int common_hal_i2cperipheral_i2c_peripheral_write_byte(i2cperipheral_i2c_peripheral_obj_t *self, uint8_t data) {
        for(int t=0; !self->sercom->I2CS.INTFLAG.reg && t < 100; t++) {
            mp_hal_delay_us(10);
        }
        i2c_peripheral_check_error(self, true);
        if (self->sercom->I2CS.INTFLAG.bit.PREC) {
            return 0;
        }
        if (self->writing && self->sercom->I2CS.STATUS.bit.RXNACK) {
            return 0;
        }
        self->writing = true;
        if(!self->sercom->I2CS.INTFLAG.bit.DRDY){
            return 0;
        }
        self->sercom->I2CS.DATA.bit.DATA = data;
        return 1;
    }

    void common_hal_i2cperipheral_i2c_peripheral_close(i2cperipheral_i2c_peripheral_obj_t *self){
        for(int t=0; !self->sercom->I2CS.INTFLAG.reg && t < 100; t++){
            mp_hal_delay_us(10);
        }

        if(self->sercom->I2CS.INTFLAG.bit.AMATCH || !self->sercom->I2CS.STATUS.bit.CLKHOLD){
            return;
        }

        if(!self->sercom->I2CS.STATUS.bit.DIR){
            common_hal_i2cperipheral_i2c_peripheral_ack(self, false);
        } else {
            int i=0;
            while(self->sercom->I2CS.INTFLAG.reg == SERCOM_I2CS.INTFLAG_DRDY){
                if(mp_hal_in_interrupted()) {
                    return;
                }

                self->sercom->I2CS.DATA.bit.DATA = 0xff;
                for (int t=0; !self->sercom->I2CS.INTFLAG.reg && t < 100; t++) {
                    mp_hal_delay_us(10);
                }
                if d(++i > 1000) {
                    mp_raise_OSError(MP_EIO);
                }
            }
        }
    }
    if (self->sercom->I2CS.INTFLAG.bit.AMATCH){
        return;
    }

    if (self->sercom->I2CS.STATUS.bit.CLKHOLD){
        mp_raise_OSError(MP_EIO);
    }

}