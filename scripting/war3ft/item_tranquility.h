

// WC3_PLAYERINFO
enum WC3_PLAYERINFO
{
	Float:fCurrentSpeed,		// Теущая скорость игрока
	bool:bIsOnGround,			// Проверяет находится ли игрок в воздухе
	bool:bCastingSpell,			// Проверяет работает ли задача предмета
	bool:bCastingInterrupted	// Прерывает действие задачи если игрок прыгнул или побежал

}
new arrPlayerInfo[33][WC3_PLAYERINFO]


#define TRANQUILITY_TIME 		get_pcvar_float( CVAR_wc3_tranquility_time )	// Время работы предмета Спокойствие
#define TRANQUILITY_FREQ 		0.5												// Время между повторами эффекта
#define TRANQUILITY_RANGE		get_pcvar_num( CVAR_wc3_tranquility_range )		// Радиус в котором могут лечиться игроки
#define TRANQUILITY_HP			get_pcvar_num( CVAR_wc3_tranquility_hp )		// По сколько HP восстанавливает предмет Спокойствие

#define MAX_CASTING_VELOCITY	40.0	// If players velocity is greater then this, any spellcasting will be interrupted


enum
{
	TASK_GLOBAL_HEARTBEAT = 1000,	// Task numbers starts at 1000
	TASK_CASTINGSPELL[33],			// Used for casting spells
	TASK_CASTINGINTERRUPTCHECK[33],	// Used to check for spellcasting interruptions
	TASK_TRANQUILITY[33]			// Used to give tranquility effect on players
}


// Flags used by PLAYER_CastSpell() function
enum (<<=1)
{
	CAST_IGNORE_INTERRUPT = 1,		// Ignores if the players casting gets interrupted
	CAST_IGNORE_MOVEMENT,			// Skips checking if the player is moving or not
	CAST_SKIP_ENDCAST_SOUND			// Doesn't play the cast sound when a spell goes off
}
