#if defined _single_mode_included
  #endinput
#endif
#define _single_mode_included

enum
{
	CS_SM_NORMAL = 1,
	CS_SM_PISTOL,
	CS_SM_KNIFE,
	CS_SM_SNIPER
}

native g4u_user_kill_score(id);
/* Get state of single mod */
native g4u_get_single_toggle();
/* Get single mode fighting type */
native g4u_get_single_mode();
/* Get max score */
native g4u_get_max_score()
native Float:g4u_get_single_roundtime()
/* Get the highest score */
native g4u_get_highest_score();
/* Called when a round finish */
forward g4u_sm_game_over(winner)

native g4u_get_single_type()
forward g4u_sm_spawn_post(id)