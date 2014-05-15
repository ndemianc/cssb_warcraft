/* Biohazard functions
*
*  by Cheap_Suit
* 
*  This file is provided as is (no warranties).
*/

#if defined _biohazard_included
	#endinput
#endif
#define _biohazard_included

#pragma reqlib "biohazardf"

/* Returns the mods status. */
stock is_biomod_active()
{
	if(is_plugin_loaded("Biohazard") == -1)
	{
		log_amx("Biohazard is not loaded.")
		return 0
	}
	
	if(!cvar_exists("bh_enabled"))
	{
		log_amx("Cvar: ^"bh_enabled^" does not exist.")
		return 0
	}

	new cvar_pointer = get_cvar_pointer("bh_enabled")
	return get_pcvar_num(cvar_pointer)
}

/* Called when a survivor is infected by a zombie. */
forward event_infect(attacker, victim)

/* Called when a survivor is infected. */
forward event_infect2(index)

/* Called when a game starts. */
forward event_gamestart()

/* Called when a team wins. */
enum BHTeam
{
	BH_TEAM_ZOMBIE 	 = 1,
	BH_TEAM_SURVIVOR = 2
}
forward event_teamwin(BHTeam:team)

/* Returns true if the game is started */
native bool:is_game_started()

/* Makes a user zombie. */
native infect_user(index)

/* Returns 1 if user is a zombie. */
native is_user_zombie(index)

/* Returns 1 if the user is first zombie. */
native is_user_firstzombie(index)

/* Returns the id of the first zombie. */
native firstzombie()