//===========================================================//
// Projet Micro - INFO1 - ENSSAT - S2 2018							 //
//===========================================================//
// File                : Programme de départ
// Hardware Environment: Open1768	
// Build Environment   : Keil µVision
//===========================================================//

#include "touch\ili_lcd_general.h"
#include "touch\lcd_api.h"
#include "affichagelcd.h"
#include "touch\touch_panel.h"

#include "global.h"
#include "gestion_i2c.h"
#include "gestion_timer.h"

#include "gestion_pinsel.h"
#include <stdio.h>


//===========================================================//
// Check if someone have touched the screen
//===========================================================//
// Définition des constantes pour les zones tactiles
#define START_X_MIN 460
#define START_X_MAX 1800
#define START_Y_MIN 1000
#define START_Y_MAX 4000

#define STOP_X_MIN 2100
#define STOP_X_MAX 3500
#define STOP_Y_MIN 1000
#define STOP_Y_MAX 4000

#define RESET_MEM_X_MIN 0
#define RESET_MEM_X_MAX 3800
#define RESET_MEM_Y_MIN 1263
#define RESET_MEM_Y_MAX 1600

//vérifier si les coordonnées sont dans la zone "START"
int isStartZone(int x, int y) {
    return (x >= START_X_MIN && x <= START_X_MAX && y >= START_Y_MIN && y <= START_Y_MAX);
}

//vérifier si les coordonnées sont dans la zone "STOP"
int isStopZone(int x, int y) {
    return (x >= STOP_X_MIN && x <= STOP_X_MAX && y >= STOP_Y_MIN && y <= STOP_Y_MAX);
}

//vérifier si les coordonnées sont dans la zone "RESET MEM"
int isResetMemZone(int x, int y) {
    return (x >= RESET_MEM_X_MIN && x <= RESET_MEM_X_MAX && y >= RESET_MEM_Y_MIN && y <= RESET_MEM_Y_MAX);
}

void screenPat() {
    touch_read();

    if (isStartZone(touch_x, touch_y)) {
        restartCount();
    } 

    else if (isStopZone(touch_x, touch_y)) {
        stopCount();
    } 

    else if (isResetMemZone(touch_x, touch_y)) {
        eraseMem();
    }
}

void init_ui() {
		lcd_clear(Grey);
		//text init
		sprintf(chaine,"%s", "TIMER 2000 V4");
		LCD_write_english_string (65,10,chaine,Red,Grey);

		//draw buttons and text
		dessiner_rect(70,100,80,31,2,1,Red,Yellow);
		sprintf(chaine,"%s", "RESET MEM");
		LCD_write_english_string (74,107,chaine,Black,Yellow);
		////
		dessiner_rect(10,170,100,100,2,1,Black,Green);
		sprintf(chaine,"%s", "START");
		LCD_write_english_string (40,210,chaine,White,Green);
		////
		dessiner_rect(130,170,100,100,2,1,Black,Red);
		sprintf(chaine,"%s", "STOP");
		LCD_write_english_string (165,210,chaine,White,Red);
		////
		sprintf(chaine,"%s", "Current state : Playing");
		LCD_write_english_string (10,140,chaine,White,Red);
	
}

//===========================================================//
// Function: Main
//===========================================================//
int main(void)
{	   
	//Tests de lecture / écriture dans la mémoire
	/*uint8_t reg = 0x01;
	uint8_t data[2] = {0x01,0x30};
	uint8_t dataRecupere;
	i2c_eeprom_write(&data, sizeof(data));
  i2c_eeprom_read(0x01, &dataRecupere, 1);
	sprintf(chaine, "Read: 0x%02X", dataRecupere);
  LCD_write_english_string(32, 50, chaine, White, Red);
	*/
	  //Initialisation des différents périphériques
	  lcd_Initializtion();
		touch_init();
		pin_init();
		init_timer();
		init_i2c_eeprom();
	
		// dessin de l'ui
		init_ui();
	
    while(1) {
			touch_debug();
			checkButtonPress();   ///vérifier l'état du bouton pour write le timer
			
			//// pour detecter si l'écran est pressé (assez approximatif...)
			if(TP_DOWN() && lcdPress == 0){
				lcdPress = 1; // ecran est appuyé
				screenPat();
			} else if (!TP_DOWN() && lcdPress == 1) {
				lcdPress = 0;
			}
		};
	};
	
//---------------------------------------------------------------------------------------------	
#ifdef  DEBUG
void check_failed(uint8_t *file, uint32_t line) {while(1);}
#endif
