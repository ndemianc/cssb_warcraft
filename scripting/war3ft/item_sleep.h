
// - Sleep ------------------------------------------------ //
#define SLEEP_DURATION	get_pcvar_float( CVAR_wc3_sleep_duration )   //Время сна 
#define SLEEP_MOVETIME	0.8    // (  float) duration after sleep is done that player is unattackable

new bool:g_bPlayerSleeping[33];
new bool:g_bPlayerCantMove[33];


#define SPEED_IMMOBILIZE	0.1    // Скорость при воздействии Сонная аура

#define TASK_SLEEP				657843
#define TASK_SLEEP_WEAPON		658844
#define TASK_SLEEP_DURATION		659845
#define TASK_SLEEP_FINISH		661846
#define TASK_SLEEP_TEMPENTITY	677843


new Float:arrfDuration[33];