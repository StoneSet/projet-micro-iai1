#include "gestion_Timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_timer.h"
#include "affichagelcd.h"
#include "touch\ili_lcd_general.h"
#include "touch\lcd_api.h"
#include "gestion_i2c.h"
#include "globaldec.h"
#include "gestion_pinsel.h"
#include <stdio.h>
#include <stdlib.h>

// D�finir des constantes pour les param�tres
#define PRECISION 2500
#define DEMI_PERIODE 100
#define EEPROM_I2C_ADDR 0x51

volatile uint8_t compteur = 0;
volatile uint8_t process = 1;
volatile int i2c_processing = 0;
uint16_t addr = 0x01; // Adresse de d�part pour l'EEPROM
uint8_t data_recup = 0;
int nombre = 0;

//================================================
// Fonctions pour �crire des donn�es dans l'EEPROM
//================================================
void writeEEPROM(uint16_t address, uint8_t value) {
    uint8_t data[2];
    data[0] = (uint8_t)address;  // Premi�re partie de l'adresse m�moire o� �crire
    data[1] = value;             // Valeur � �crire

    if (!i2c_processing) {
        i2c_processing = 1;
        i2c_eeprom_write(data, sizeof(data));
        i2c_processing = 0;
    }
}

// Fonction pour effacer la m�moire
void eraseMem(void) {
    writeEEPROM(addr, 0x00);
}

// Fonction pour �crire la valeur actuelle du compteur dans l'EEPROM
void writedata(uint8_t datatowrite) {
    writeEEPROM(addr, datatowrite);
}

//==========================================
// Initialisation du timer
//==========================================
void init_timer() {
    PINSEL_CFG_Type configPinsel;
    TIM_TIMERCFG_Type configTimer;
    TIM_MATCHCFG_Type timermatch;

    // Configuration des broches pour le timer
    configPinsel.Portnum = PINSEL_PORT_1;
    configPinsel.Pinnum = PINSEL_PIN_28;
    configPinsel.Funcnum = PINSEL_FUNC_3;
    configPinsel.Pinmode = 0;
    configPinsel.OpenDrain = PINSEL_PINMODE_NORMAL;
    PINSEL_ConfigPin(&configPinsel);

    // Configuration du timer
    configTimer.PrescaleOption = TIM_PRESCALE_TICKVAL;
    configTimer.PrescaleValue = PRECISION;
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &configTimer);

    // Configuration du match pour le timer
    timermatch.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE;
    timermatch.IntOnMatch = ENABLE;
    timermatch.StopOnMatch = DISABLE;
    timermatch.MatchChannel = 0;
    timermatch.MatchValue = DEMI_PERIODE;
    timermatch.ResetOnMatch = ENABLE;
    TIM_ConfigMatch(LPC_TIM0, &timermatch);

    // Activation de l'interruption du timer
    NVIC_EnableIRQ(TIMER0_IRQn);

    // Lancement du timer
    TIM_Cmd(LPC_TIM0, ENABLE);
}

//==========================================
// Donn�es de d�bogage utilisateur
//==========================================
void afficher_debug(void) {
    uint8_t addr_data[1];
    addr_data[0] = addr; // Remplir le tableau avec l'adresse
    i2c_eeprom_read(addr_data[0], &data_recup, sizeof(data_recup)); // Lire � partir de l'adresse sp�cifi�e
    sprintf(chaine, "[D] mem : %d    ", data_recup);
    LCD_write_english_string(10, 50, chaine, Magenta, White);

    sprintf(chaine, "[D] timer : %d    ", nombre);
    LCD_write_english_string(10, 70, chaine, Magenta, White);
}

//================================================
// Gestionnaire d'interruption du timer
//================================================
void TIMER0_IRQHandler(void) {
    if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)) {
        TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
        if (compteur == 100) {
            if (process == 1 && !i2c_processing) {
                nombre++;
                //writedata(nombre); - Si on veut que �a �crive � chaque fois (pas notre cas)
								afficher_debug();
            }
            compteur = 0;
        } else {
            compteur++;
        }
    }
}

//================================================
// Fonction pour red�marrer le compteur
//================================================
void restartCount(void) {
    process = 1;
    sprintf(chaine, "%s", "Current state : Restart");
    LCD_write_english_string(10, 140, chaine, White, Red);
}

//================================================
// Fonction pour arr�ter le compteur
//================================================
void stopCount(void) {
    if (process == 0) { // Si le timer n'est pas en cours d'ex�cution
        nombre = 0; // R�initialiser le compteur mais pas la m�moire
        sprintf(chaine, "%s", "Current state : Reinit ");
        LCD_write_english_string(10, 140, chaine, White, Red);
        afficher_debug();
    } else { // Si le timer est en cours d'ex�cution
        process = 0;
        sprintf(chaine, "%s", "Current state : Stopped");
        LCD_write_english_string(10, 140, chaine, White, Red);
    }
}

//================================================
// Fonction pour v�rifier l'�tat du bouton
//================================================
void checkButtonPress(void) {
    // V�rifiez l'�tat du bouton
    if (!(LPC_GPIO2->FIOPIN & (1 << BUTTON_PIN))) { // Si P2.10 est � l'�tat bas (bouton press�)
        writedata(nombre);
        while (!(LPC_GPIO2->FIOPIN & (1 << BUTTON_PIN))); // Attendre que le bouton soit rel�ch�
    }
}
