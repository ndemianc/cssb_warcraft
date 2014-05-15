new szSpriteItemWhite[64];

new idSpriteItemWhite;


new bool:bIsUserESP[33];

#define	TASK_ESP	97854

#define	FREQUENCY_ESP	get_pcvar_float(CVAR_wc3_frequency_esp)	//Частота срабатывания детектора
#define	RADIUS_ESP	get_pcvar_num( CVAR_wc3_radius_esp )		//Радиус детектора