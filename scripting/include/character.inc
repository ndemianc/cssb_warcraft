// ******G4U CHARACTER ******//
#if defined _character_included
  #endinput
#endif
#define _character_included

const CSRED_HANDLED = 97

/* Get user character id */
native g4u_get_user_character(id)
/* Get user character health */
native g4u_character_maxhealth(id)
/* Get user character max speed */
native g4u_character_maxspeed(id)
/* Get user character gender */
native g4u_character_gender(id)
/* Set character id to a player */
native g4u_set_user_character_id(id, characterid)
/* Get number of character */
native g4u_get_character_number()
/* Get character id by model */
native g4u_get_characterid_by_model(const model[], len)
/* Get player hud */
native g4u_get_character_hud(id, hud[], len)
/* Get character gravity */
native Float:g4u_character_gravity(id)
/* Get player's hand */
native g4u_character_hand(id)
/* Get hud by id */
native g4u_character_hud_id(id, hud[], len)
/* Get character id by model */
native g4u_character_id_model(const model[], len)
/* Get character cost type */
native g4u_character_cost_type(characterid)
/* Get character cost */
native g4u_character_cost(characterid)
/* Set player character id */
native g4u_set_user_character(id, characterid)

native g4u_set_user_speed_return(id, Speed)
forward character_get_speed(id, speed)
native g4u_set_user_hp_return(id, iHp)
forward character_get_health(id, iHp)
native set_user_gravity_return(id, Float:fGravity)
forward character_get_gravity(id, Float:Gravity)