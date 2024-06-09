#ifndef GESTION_PINSEL_H
#define GESTION_PINSEL_H

#include "lpc17xx_pinsel.h"

#define I2C_SDA_PIN 27
#define I2C_SCL_PIN 28
#define BUTTON_PIN 10
#define BUTTON_PORT 2

void pin_init(void);

#endif
