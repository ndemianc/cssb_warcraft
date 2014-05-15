// - Death Coil ------------------------------------------- //
#define DEATHCOIL_DAMAGE	get_pcvar_num( CVAR_wc3_deathcoil_damage )//Количество повреждений наносимые Лик смерти ( расчет wc3_deathcoil_damage*(0.7) )
#define DEATHCOIL_VELOCITY	800.0    // (  float) velocity of death coil towards target
#define DEATHCOIL_DURATION	3.0    // (  float) life of deathcoil before removal

new DEATHCOIL_SHELL_RGB[3]	=	{160,255,0};  // (integer) RGB of glow shell over skull
new DEATHCOIL_TRAIL_RGB[3]	=	{96,224,0};  // (integer) RGB of beamfollow trail from skull


#define GLOW_R                         0    // Glow reference values
#define GLOW_G                         1
#define GLOW_B                         2
