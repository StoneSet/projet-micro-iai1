//===========================================================//
// Projet Micro - INFO1 - ENSSAT - S2 2018							 //
//===========================================================//
// File                : Programme de départ
// Hardware Environment: Open1768	
// Build Environment   : Keil µVision
//===========================================================//

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_timer.h"
#include "touch\ili_lcd_general.h"
#include "touch\lcd_api.h"
#include "affichagelcd.h"
#include "touch\touch_panel.h"


#include "globaldec.h" // fichier contenant toutes les déclarations de variables globales
#include <stdio.h>

	

//===========================================================//
// Function: Main
//===========================================================//
int main(void)
{	  
	  int n; 
	
	  // Init(); // init variables globales et pinsel pour IR => à faire
	
	  lcd_Initializtion(); // init pinsel ecran et init LCD
		// affichage sur l'écran d'un exemple de 4 carrés de couleur et d'une chaine de caractère
	  n=sprintf(chaine,"Ceci est un exemple      ");
	  LCD_write_english_string (32,30,chaine,White,Blue);
	  dessiner_rect(10,60,110,110,2,1,Black,Yellow);
	  dessiner_rect(120,60,110,110,2,1,Black,Green);
	  dessiner_rect(10,170,110,110,2,1,Black,Blue);
	  dessiner_rect(120,170,110,110,2,1,Black,Red);
		
	  touch_init(); // init pinsel tactile et init tactile; à ne laisser que si vous utilisez le tactile
	  
    while(1)  ; 
	// pour l'instant, le main fait juste quelques inits ... à vous d'écrire le reste
	 
		
		
		
		
	}

//---------------------------------------------------------------------------------------------	
#ifdef  DEBUG
void check_failed(uint8_t *file, uint32_t line) {while(1);}
#endif
