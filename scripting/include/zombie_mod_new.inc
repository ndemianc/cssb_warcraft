/* This file contacts with zombiemod.amxx */
#if defined _zombie_mod_included
  #endinput
#endif
#define _zombie_mod_included

/* Check a player if he/she is a zombie or not */
native g4u_get_user_zombie(id)
/* Get the id class of a player */
native g4u_get_zombie_class(id)
/* Get level of a player (only if he is a zombie) */
native g4u_get_zombie_level(id)
/* Check zombie mod state */
native g4u_get_zombie_toggle()
/* Register an item to equipment menu */
native g4u_register_equipment(const name[])
/* Register an item to secondary weapon menu */
native g4u_register_secondary_weapon(const name[])
/* Register an item to primary weapon menu */
native g4u_register_primary_weapon(const name[])
/* Register an item to extra menu */
native g4u_register_extra_item(const name[])
/* Register an object which will be removed when zombie mode is on */
native g4u_register_map_object(const name[])
/************** FORWARD *****************/
/* Usually called when a player become zombie */
forward g4u_player_infected(victim, infector)
/* Usually called when a player buy an extra item */
forward g4u_player_buy_item(id, itemid)
/* Usually called when player choose an equipment */
forward g4u_equipment_selected(id, itemid)
/* Usually called when player choose a secondary weapon */
forward g4u_secondary_weapon_selected(id, itemid)
/* Usually called when a primary is selected */
forward g4u_primary_weapon_selected(id, itemid)
