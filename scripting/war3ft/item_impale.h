
// - Impale ----------------------------------------------- //

#define IMPALE_DAMAGE			get_pcvar_num( CVAR_wc3_impale_damage ) // (integer) initial impale damage
#define IMPALE_ARMORDAMAGE		get_pcvar_num( CVAR_wc3_impale_armordmg )    // (integer) initial impale damage
#define IMPALE_COLLISIONDAMAGE	get_pcvar_num( CVAR_wc3_impale_colldmg )    // (integer) damage dealt for collision with ceiling
#define IMPALE_VELOCITY			630    // (integer) upward velocity (620 ~ 30 damage)
#define IMPALE_MINDISTANCE		200    // (integer) any impale that doesnt span this distance will assume ceiling collision



new g_PlayerImpaled[33];