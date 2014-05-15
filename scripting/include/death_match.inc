/* G4U DEATH MATCH MODE */

#if defined _death_match_included
  #endinput
#endif
#define _death_match_included

enum
{
	CS_DM_FY = 1,
	CS_DM_DE,
	CS_DM_CS,
	CS_DM_NORMAL,
	CS_DM_AS,
	CS_DM_KA
}

enum
{
	CS_DM_KNIFE = 1,
	CS_DM_PISTOL,
	CS_DM_SNIPER,
	CS_DM_FULL
}

/* Get death match map type */
native g4u_get_dm_mode();
/* Get death match fighting mode */
native g4u_get_dm_type();
native g4u_get_GR_score();
native g4u_get_BL_score();
native g4u_max_round();
native Float:g4u_deathmatch_roundtime()

forward g4u_count_time_down(Minutes, Seconds)
forward g4u_death_match_end(WinTeam)