#include "gestion_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"

//fonction d'initialisation des broches I/O
void pin_init(void) {
    PINSEL_CFG_Type config_broche_i2c; 

    // Initialisation GPIO2.10 en entrée pour le bouton
    GPIO_SetDir(BUTTON_PORT, (1 << BUTTON_PIN), 0);

    // Initialisation des broches I2C (P0.27 et P0.28)
    config_broche_i2c.Portnum = PINSEL_PORT_0;
    config_broche_i2c.Pinnum = I2C_SDA_PIN;
    config_broche_i2c.Funcnum = PINSEL_FUNC_1;
    config_broche_i2c.Pinmode = PINSEL_PINMODE_PULLUP;
    config_broche_i2c.OpenDrain = PINSEL_PINMODE_NORMAL;
    PINSEL_ConfigPin(&config_broche_i2c);

    config_broche_i2c.Pinnum = I2C_SCL_PIN;
    PINSEL_ConfigPin(&config_broche_i2c);
}
