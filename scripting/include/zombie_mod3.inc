/* #####################################################
	---------------------------------------
	-*- Zombie Mod 3 Includes File -*-
	---------------------------------------
###################################################### */ 

// get mod running
native zb3_get_mod()

// kiem tra xem player co plai la zombie khong
native zb3_get_user_zombie(id)

// tra ve level cua player
native zb3_get_user_level(id)

// tra ve level cua player
native zb3_get_user_zombie_class(id)

// kiem tra xem player co phai la hero khong
native zb3_get_user_hero(id)

// get start health
native zb3_get_user_start_health(id)

// respawn zombie
native zb3_zombie_respawn(id)

// remove weapons in new round? 1 is remove - 2 is no
native zb3_remove_weapons_newround()

// Human kill Zombie
native zb3_human_kill_zombie(killer, victim, headshot)

// show chat text
native zb3_color_saytext(player, const message[], any:...)

// get max level human
native zb3_get_maxlevel_human()

// get weapons ammo
native zb3_get_weapons_ammo(idwpn)

// get damage attack
native zb3_get_user_damage_attack(id)

// set damage attack
native zb3_set_user_damage_attack(id, damage)

// get can take damage
native zb3_get_take_damage()

// get he take damage
native zb3_get_damage_nade()

// register zombie class of Zombie Mod3
native zb3_register_zombie_class(const name[], const model[], Float:gravity, Float:speed, Float:knockback, const sound_death1[], const sound_death2[], const sound_hurt1[], const sound_hurt2[], const sound_heal[], const sound_evolution[])

native zb3_register_extra_item(const name[], cost)

/**
 * Called when a human attack zombie
 *
 * @param attacker	Player index who was attack
 * @param damage	damage fire
 */
forward zb3_human_damage(attacker, damage, type)

/**
 * Called when a player gets infected.
 *
 * @param id		Player index who was infected.
 * @param infector	Player index who infected him (if applicable).
 */
forward zb3_user_infected(id, infector)

/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1034\\ f0\\ fs16 \n\\ par }
*/
