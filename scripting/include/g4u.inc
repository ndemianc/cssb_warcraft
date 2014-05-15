/*================================================================================
	G4U INCLUDE FILE
	Contact with level_up.amxx , gcoin.amxx, g4u_character.amxx
=================================================================================*/
#if defined _g4u_included
  #endinput
#endif
#define _g4u_included

/* Returns player's coin */
native g4u_get_user_coin(id)

/* Set player's coin */
native g4u_set_user_coin(id, amount)
native g4u_get_level(id)
/* Returns defusing bomb point of a player */
native g4u_get_defuse_bomb_point(id)
/* Returns planting bomb point of a player */
native g4u_get_plant_bomb_point(id)
/* Returns knife killing point of a player */
native g4u_get_knife_point(id)
/* Returns headshot points of a player */
native g4u_get_headshot_point(id)
/* Returns infecting point of a player */
native g4u_get_infect_point(id)
/* Returns normal killing point of a player */
native g4u_get_normal_kill_point(id)
/* Returns zombie killing point of a player */
native g4u_get_zombie_point(id)
/* Set player knife kill */
native g4u_set_knife_kill(killer, headshot)
/* Set player grenade kill */ 
native g4u_set_grenade_kill(killer)
/* Get the name of level */
native g4u_get_level_name(levelid, output[], len)
/* Get max level */
native g4u_get_max_level()
/* Set escape point for a player */
native g4u_set_escape_point(id)
/* Get user's gp */
native g4u_get_user_gp(id)
/* Set user's gp */
native g4u_set_user_gp(id, amount)
/* Save user level */
native g4u_save_user_level(id, const name[])
/* Load user level */
native g4u_load_user_level(id, const name[])
/* Load admin level */
native g4u_load_admin_level(id)
/* Get all points of an user */
native g4u_get_total_point(id)
/* Get level's point */
native g4u_get_level_point(levelid)

native g4u_get_awarded_coin(id)
native g4u_get_awarded_gp(id)
native g4u_is_user_promoted(id)
native g4u_set_user_AccName(id, const AccName[])