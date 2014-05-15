
#if defined _defender_mode_included
  #endinput
#endif
#define _defender_mode_included


/* NATIVES */
native df_is_user_human_hero(id)
native df_is_user_ghost(id)
native df_is_user_ghost_hero(id)
native df_is_user_human(id)
native df_get_human_hero_health()
native df_get_ghost_hero_health()
native Float:df_get_human_hero_gravity()
native Float:df_get_ghost_hero_gravity()
native Float:df_get_human_hero_speed()
native Float:df_get_ghost_hero_speed()
native df_get_user_bonus_point(id)
native Float:df_get_user_revenge_point(id)
native df_is_mode_on()
native Float:df_get_round_time()
native df_get_human_score()
native df_get_ghost_score()
native df_get_human_kill()
native df_get_ghost_kill()
native df_get_max_round()
native df_get_max_bonus_point()
/* Forwards */

forward df_become_human_hero_pre(id)
forward df_become_human_hero_post(id)
forward df_become_ghost_hero_pre(id)
forward df_become_ghost_hero_post(id)
forward df_human_hero_killed(id, killer)
forward df_ghost_hero_killed(id, killer)
forward df_human_respawn_pre(id)
forward df_human_respawn_post(id)
forward df_ghost_respawn_pre(id)
forward df_ghost_respawn_post(id)
forward df_hero_human_rehealth(id)
forward df_hero_ghost_rehealth(id)
forward df_human_win(iHumanScore, iHumanKill)
forward df_ghost_win(iGhostScore, iGhostKill)
forward df_bonus_point_added(id, iBonusPoint)
forward df_match_end_human_win()
forward df_match_end_ghost_win()
