#ifndef __gestion_I2C_H__
#define __gestion_I2C_H__

#include "lpc17xx_timer.h"

void init_i2c_eeprom(void);
void i2c_eeprom_write(void *data, int length);
void i2c_eeprom_read(uint8_t add_case, void *data_recup, int length);

#endif
