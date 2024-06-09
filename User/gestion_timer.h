#ifndef __gestion_Timer_H__
#define __gestion_Timer_H__

#include "lpc17xx_timer.h"

void init_timer(void);
void TIMER0_IRQHandler(void);
void restartCount(void);
void stopCount(void);
void checkButtonPress(void);
void writedata(uint8_t datatowrite);
void afficher_debug(void);
void eraseMem(void);

#endif
