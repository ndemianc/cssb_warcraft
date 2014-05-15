/* This file is contact with g4u_pistol.amxx, g4u_riffle.amxx, g4u_hegrenade.amxx, ... */

#if defined _g4u_wpn_included
  #endinput
#endif
#define _g4u_wpn_included



enum
{
	RIFLE_LOAD_NONE = 0,
	RIFLE_LOAD_FULL,
	RIFLE_LOAD_SNIPER
}

enum
{
	PISTOL_LOAD_NONE,
	PISTOL_LOAD_NORMAL_PISTOL,
	PISTOL_LOAD_ALL
}

enum
{
	SHOTGUN_LOAD_NONE = 0,
	SHOTGUN_LOAD_FULL
}

enum
{
	GRE_LOAD_NONE = 0,
	GRE_LOAD_FULL
}
/* Get new riffle weapon id */
native g4u_get_user_riffle(id)
/* Force player to buy a new weapon*/
native g4u_force_user_buy_riffle(id, weaponid)
/* Force a player drop his own new riffle */
native g4u_force_user_drop_riffle(id)
/* Force a player buy grenade for his new riffle */
native g4u_force_user_buy_rnade(id)
/* Get the id of the weapon which is replaced by this plugin (riffle) */
native g4u_get_riffle_wpnchange(wpnid)
/* Get weapon count */
native g4u_get_weapon_count()
/* Give weapon bpa */
native g4u_give_weapon_bpa(id, weaponid)
/* Check if player has smoke grenade */
native g4u_user_has_smokegrenade(id)
/* Get riffle id */
native g4u_user_has_riffle(id)
/* Equip riffle to a player */
native g4u_equip_riffle(id, weaponid, client_msg, server_msg)
/* Get amount of rifles */
native g4u_get_rifle_amount()
/* Check riffle hud */
native g4u_get_riffle_hud(weaponid, hud[], len)
/* Give to player a grenade */
native g4u_equip_nade(id, weaponid, message)
/* Call when player equip */
forward g4u_player_equip(id, weaponid)
/* Force a player to buy a grenade */
native g4u_force_buy_nade(id, weaponid)
/* Get riffle name */
native g4u_get_riffle_name(wpnid, name[], len)
/* Strip riffle */
native g4u_strip_user_riffle(id)
/* Get nade number */
native g4u_get_nade_count()
/* Get nade model */
native g4u_get_hegrenade_model(weaponid, input[], len)
/* Strip a grenade of a player */
native g4u_strip_user_grenade(id)
/* Get numbers of pistol */
native g4u_get_pistol_amount()
/* Check if user has a pistol */
native g4u_user_has_pistol(id)
/* Equip a pistol to a player */
native g4u_equip_pistol(id, weaponid, showmessage)
/* Get pistol weapon change */
native g4u_get_pistol_weapon_change(weaponid)
/* Get pistol hud */
native g4u_get_pistol_hud(weaponid, hud[], len)
/* Force an user buy a pistol */
native g4u_force_user_buy_pistol(id, weaponid)
/* Get pistol name */
native g4u_get_pistol_name(weaponid, name[], len)
/* Strip pistol of a player */
native g4u_strip_user_pistol(id)
/* Get smoke grenade name */
native g4u_get_smokegrenade_name(weaponid, name[], len)
/* Strip user smoke grenade */
native g4u_strip_smokegrenade(id)
/* Get smokegrenade hud */
native g4u_get_smokegrenade_hud(weaponid, hud[], len)
/* Equip pistol but check the recipient's level */
native g4u_equip_pistol_level(id, model[], len)
/* Equip a smoke grenade to a player */
native g4u_equip_user_smokegrenade(id, weaponid, message)
/* Get hegrenade name */
native g4u_get_agrenade_name(weaponid, name[], len)
/* Get hegrenade hud */
native g4u_get_hegrenade_hud(weaponid, hud[], len)
/* Check if user has hegrenade */
native g4u_get_user_hegrenade(id)
/* Call when a Rifle successfully loaded its information */
forward g4u_rifle_successfully_loaded(weaponid)
/* Get rifles weight */
native Float:g4u_current_rifle_weight(id)
/* Call when player buy a pistol */
forward g4u_player_equip_pistol(id, weaponid)
/* Call when a pistol successfully loads its information */
forward g4u_pistol_successfully_loaded(weaponid)
/* Give a player a knife */
native g4u_equip_knife(id, weaponid, message)
/* Give a player shotgun but check his level */
native g4u_equip_shotgun_level(id, model[], len)
/* Get hud string of knife */
native g4u_get_knife_hud(weaponid , hud[], len)
/* Force an user to buy a knife */
native g4u_force_user_buy_knife(id, weaponid)
/* Get cgrenade amount */
native g4u_get_cnade_number()
/* Check if a player has a knife */
native g4u_user_has_knife(id)
/* Get the name of knife */
native g4u_get_knife_name(weaponid, name[], len)
/* Strip knives of user */
native g4u_strip_user_knife(id)
/* Get amount of knives */
native g4u_get_knife_amount()
/* Give knife but check level */
native g4u_equip_knife_level(id, model[], len)
/* Call when a player successfully buy a knife */
forward g4u_player_equip_knife(id, weaponid)
/* Give a shotgun to a player */
native g4u_equip_shotgun(id, weaponid, show_message)
/* Get number of shotguns */
native g4u_get_shotgun_amount()
/* Get hud string of shotgun */
native g4u_get_shotgun_hud(weaponid, hud[], len)
/* Force a player to buy a shotgun */
native g4u_force_user_buy_shotgun(id, weaponid)
/* Check if an user has a shotgun */
native g4u_user_has_shotgun(id)
/* Give rifle to a player but check his level */
native g4u_equip_rifle_level(id, const model[], len)
/* Get original shotgun id */
native g4u_get_shotgun_weapon_change(weaponid)
/* Returns name of shotgun */
native g4u_get_shotgun_name(weaponid, name[], len)
/* Strip user's shotgun */
native g4u_strip_user_shotgun(id)
/* Call when a player equip a shotgun */
forward g4u_player_equip_shotgun(id, weaponid)
/* Call when a launcher grenade kills someone */
forward g4u_launcher_nade_kill(victim, killer)
/* Call when a grenade kills someone */
forward g4u_grenade_kill(victim, killer)
/* Call when a shotgun kills someone */
forward g4u_shotgun_kill(victim, killer, headshot)
/* Call when player use secondary attack to kill his enermy */
forward g4u_secondary_kill(victim, killer, headshot)
/* Refill ammo for rifle */
native g4u_set_rifle_full_ammo(id,  client_message, server_message)
/* Refill ammo for pistol */
native g4u_set_pistol_full_ammo(id,  client_message, server_message)
/* Called when a player drop a A Type grenade */
forward g4u_hegrenade_drop(ent, id)
/* Called when a player drop C type grenade */
forward g4u_smokegrenade_drop(ent, id)
/* Set rifle full launcher nade */
native g4u_set_rifle_full_grenade(id, client_msg, server_msg)
/* Call when primary weapon special function is actived */
forward g4u_rifle_special_function(ent)
/* Call when primary weapon is reloaded*/
forward g4u_rifle_reload(ent)
/* Call when a rifle drops to floor */
forward g4u_rifle_drop(id)
/* Call when a pistol reload */
forward g4u_pistol_reload(id, ent)
/* Call when a pistol actives it's special function */
forward g4u_pistol_special_function(id, ent)
/* Call when a pistol drops to floor */
forward g4u_pistol_drop(id, ent)
/* Call when rifle send an animation */
forward g4u_rifle_play_animation(id, animation)
/* Call when pistol sends an animation */
forward g4u_pistol_play_animation(id, animation)
/* Get rifle id by w model */
native g4u_rifle_id_by_model(const model[], len)
/* Get normal rifle by w model */
native g4u_nrifle_id_by_model(const model[], len)
/* Get shotgun id by w model */
native g4u_shotgun_id_by_model(const model[], len)
/* Get normal shotgun by w model */
native g4u_nshotgun_id_by_model(const model[], len)
/* Get pistol id by w model */
native g4u_pistol_id_by_model(const model[], len)
/* Get normal pistol by w model */
native g4u_npistol_id_by_model(const model[], len)
/* Get agrenade id by w model */
native g4u_agrenade_id_by_model(const model[], len)
/* Get smk id by w model */
native g4u_smk_id_by_model(const model[], len)
/* Get melee id by w model */
native g4u_knifeid_by_model(const model[], len)
/* Get decrease speed of RIFLE */
native Float:g4u_current_rifle_dspeed(id)
/* Give smk but check level */
native g4u_give_smk_level(id, model[], len)
/* Give hegrenade but check level */
native g4u_equip_grenade_level(id, model[], len)
/* Get SHOTGUN PRICE */
native g4u_shotgun_cost(wid)
/* Get SHOTGUN cost type */
native g4u_shotgun_cost_type(wid)
/* Get RIFLE PRICE */
native g4u_rifle_cost(wid)
/* Get RIFLE cost type */
native g4u_rifle_cost_type(wid)
/* Get PISTOL PRICE */
native g4u_pistol_cost(wid)
/* Get PISTOL Cost Type */
native g4u_pistol_cost_type(wid)
/* Get HEGRENADE PRICE */
native g4u_hegrenade_cost(wid)
/* Get HEGRENADE cost type */
native g4u_hegrenade_cost_type(wid)
/* Get SMOKEGRENADE PRICE */
native g4u_smokegrenade_cost(wid)
/* Get SMOKEGRENADE cost type */
native g4u_smokegrenade_cost_type(wid)
/* Get smokegrenade - hegrenade level */
native g4u_smokegrenade_level(id)
native g4u_hegrenade_level(wid)
native g4u_get_knife_cost(wid)
native g4u_get_knife_cost_type(wid)
native g4u_get_shotgun_level(wid)
native g4u_get_knife_level(wid)
/************ FORWARD - USEFUL FOR SPRITE CONFIGURATIONS *******************/
/* Called when rifle is selected */
forward g4u_rifle_selected(id, weaponid, weapon_name[], weapon_hud[], weapon_file[])
/* Called when normal rifle is selected - The id is contact with CSW_ */
forward g4u_normal_rifle_selected(id, weaponid, weapon_hud[])
/* Called when NEW hegrenade is selected */
forward g4u_new_hegrenade_selected(id, weaponid, weapon_name[], weapon_hud[], weapon_file[])
/* Called when NORMAL HEGRENADE is selected */
forward g4u_hegrenade_selected(id)
/* Called when NEW smoke grenade is selected */
forward g4u_new_newsmokegrn_selected(id, weaponid, weapon_name[], weapon_hud[], weapon_file[])
/* Called when NORMAL Smoke grenade is selected */
forward g4u_smokegrenade_selected(id)
/* Called when FLASH BANG is selected */
forward g4u_flashbang_selected(id)
/* Called when pistol is selected */
forward g4u_pistol_selected(id, weaponid, weapon_name[], weapon_hud[], weapon_file[], Float:speed, Float:weight)
/* Called when normal pistol is selected - The id is contact with CSW_ */
forward g4u_normal_pistol_selected(id, weaponid, weapon_hud[])
/* Called when NORMAL KNIFE is selected */
forward g4u_normal_knife_selected(id)
/* Called when NEW KNIFE is selected */
forward g4u_new_knife_selected(id, weaponid, weapon_name[], weapon_hud[], weapon_file[])
/* Called when shotgun is selected */
forward g4u_shotgun_selected(id, weaponid, weapon_name[], weapon_hud[], weapon_file[])
/* Called when normal shotgun is selected - The id is contact with CSW_ */
forward g4u_normal_shotgun_selected(id, weaponid, weapon_hud[])
/* Called when a knife hit somebody */
forward g4u_knife_hit(id, target, hitplace)
/* Called when a grenade of pistol kills someone */
forward g4u_pistol_grenade_kill(victim, killer)



native Float:g4u_pistol_dspeed(wid)
native Float:g4u_pistol_gravity(wid)
native Float:g4u_get_rifle_weight(wid)
native Float:g4u_get_rifle_dspeed(wid)
native Float:g4u_get_knife_weight(wid)
native Float:g4u_get_knife_dspeed(wid)
native Float:g4u_get_shotgun_dspeed(wid)
native Float:g4u_get_shotgun_weight(wid)
native g4u_set_shotgun_full_am(id, client_message, server_message)
native g4u_rifle_get_bpammo(wid)
native g4u_shotgun_get_bpa(wid)
native g4u_pistol_get_bpa(wid)


forward g4u_knife_first_atk(id)
forward g4u_knife_second_atk(id)

native g4u_execute_rifle_file(const file[], trash, const cfgdir[], const mapname[], const txt[])


forward g4u_rifle_load_data()
forward g4u_pistol_load_data()
forward g4u_shotgun_preload()
forward g4u_hegrenade_preload()
forward g4u_smk_preload()
forward g4u_smokegrenade_kill(victim, killer)
native g4u_set_shotgun_load_type(loadtype)
native g4u_set_rifle_load_type(loadtype)
native g4u_set_hegrenade_load_type(loadtype)
native g4u_set_pistol_load_type(loadtype)
native g4u_set_smk_load_type(loadtype)