#if defined _deathmatch_included
  #endinput
#endif
#define _deathmatch_included

enum
{
	CS_TDM_FULL = 1,
	CS_TDM_PISTOL,
	CS_TDM_SNIPER,
	CS_TDM_KNIFE
}
	
/* Check the state of deathmatch mode */
native g4u_deathmatch_toggle()
/* Get game mode of deathmatch */
native g4u_deathmatch_mode()
/* Get BL score */
native g4u_deathmatch_bl_score()
/* Get GR score */
native g4u_deathmatch_gr_score()
/* Get max score */
native g4u_deathmatch_max_score()
/* Called when GR win */
forward g4u_tdm_gr_win()
/* Called when BL win */
forward g4u_tdm_bl_win()

native g4u_deathmatch_fighttype()