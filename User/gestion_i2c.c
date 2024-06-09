#include "lpc17xx_i2c.h"

#define EEPROM_I2C_ADDR 0x51

// initialisation de la mémoire i2c
void init_i2c_eeprom(void) {
	I2C_Init(LPC_I2C0, 500000); //init a 500khz
	I2C_Cmd(LPC_I2C0, ENABLE);
}

// write dans la mémoire i2c
void i2c_eeprom_write(void *data, int length)
{
	I2C_M_SETUP_Type i2cSetup;

	i2cSetup.sl_addr7bit = EEPROM_I2C_ADDR;
	i2cSetup.tx_data = (uint8_t*)data;
  i2cSetup.tx_length = length; //longueur des données
  i2cSetup.rx_data = NULL;
  i2cSetup.rx_length = 0;
	i2cSetup.retransmissions_max = 10;
	i2cSetup.callback = NULL;

	I2C_MasterTransferData(LPC_I2C0, &i2cSetup, I2C_TRANSFER_POLLING);
}

// read dans la mémoire i2c
void i2c_eeprom_read(uint8_t add_case, void *data_recup, int length)
{
    I2C_M_SETUP_Type setupi2c;
    
    setupi2c.sl_addr7bit = EEPROM_I2C_ADDR; 
    setupi2c.tx_data = &add_case; 
    setupi2c.tx_length = 1;        
    setupi2c.rx_data = data_recup;
    setupi2c.rx_length = length;  
    setupi2c.retransmissions_max = 10;
    setupi2c.retransmissions_count = 0;
    setupi2c.tx_count = 0;
    setupi2c.rx_count = 0;
        
    setupi2c.callback = NULL;
    
    I2C_MasterTransferData(LPC_I2C0, &setupi2c, I2C_TRANSFER_POLLING); //mode POLLING
}
