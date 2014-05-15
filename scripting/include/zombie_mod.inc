/* This file contacts with zombiemod.amxx */
#if defined _zombie_mod_included
  #endinput
#endif
#define _zombie_mod_included

enum
{
	WIN_NONE = 0,
	WIN_HUMAN,
	WIN_ZOMBIE
}

const CSRED_ZM_HANDLED = 97
/* --------------[NATIVES]-----------------*/
/* Check if user is a zombie */
native g4u_get_user_zombie(id)
/* Check zombie level */
native g4u_get_zombie_level(id)
/* Check zombie knock back */
native Float:g4u_get_zombie_knockback(id)
/* Check zombie max health */
native Float:g4u_get_zombie_maxhealth(id)
/* Check zombie class */
native g4u_get_zombie_class(id)
/* Check if mode is on */
native g4u_get_zombie_toggle()
/* Get zombie gravity */
native Float:g4u_get_zombie_gravity(id)
/* Get damage count human does */
native Float:g4u_get_human_damage(id)
/* Set zombie classid to a player */
native g4u_set_user_classid(id, classid)
/* Get zombie classid by checking first level model */
native g4u_get_zombieid_by_model(const model[], len)
/* Get zombie weapon hud */
native g4u_get_zombie_weapon_hud(zombieid, hud[], len)
/* Get zombie hud health */
native g4u_get_zombie_health_hud(zombieid, hud[], len)
native zm3_check_round_end()
native zm3_set_cant_spawn(id)
/*--------------[FORWARDS]-------------------*/
/* Called before a player is infected */
forward g4u_infected_pre(victim, infector)
/* Called after a player is infected */
forward g4u_infected_post(victim, infector)
/* Called before player do damage to zombie */
forward g4u_hm_atk_zb_pre(human, zombie, Float:damage, Float:total_damage)
/* Called after player do damage to zombie */
forward g4u_hm_atk_zb_post(human, zombie, Float:damage, Float:total_damage)
/* Called before player spawn */
forward g4u_user_respawn_pre(id)
/* Called after player spawn */
forward g4u_user_respawn_post(id)
/* Called before zombie appears */
forward g4u_zombie_appear_pre(zombieid)
/* Called after zombie appears */
forward g4u_zombie_appear_post(zombieid)
/* Called before a zombie level up */
forward g4u_zombie_lv_up_pre(id, level)
/* Called after a zombie level up */
forward g4u_zombie_lv_up_post(id, level)
/* Called when a round starts */
forward g4u_round_start()
/* Called when a round finish */
forward g4u_zombie_round_finish(winteam, human_score, zombie_score)
/* Called when zombie destroy armor of HUMAN */
forward g4u_zombie_damage_armor(attacker, victim)
/* Caleed when zombie's armor is destroyed */
forward g4u_zombie_defend_fail(attacker, victim)
forward g4u_user_become_zombie(id)
forward g4u_become_hero_pre(id)
forward g4u_become_hero_post(id)
forward g4u_zombie_quitgame()
forward g4u_zombie_hero_death(killer, victim)
forward zm3_zombie_model_change(id)
forward zm3_zombie_touch_weapon(id, ent)
forward zm3_zombie_strip_weapon(id, weaponid)
native g4u_zb_get_zombie_score();
native g4u_zb_get_human_score();
native g4u_zombie_max_round();

native g4u_create_zombie_class(const name[])
native g4u_set_class_model(classid, const fmodel[], const smodel[], const tmodel[])
native g4u_set_zombie_hand(classid, const fmodel[], const smodel[], const tmodel[])
native g4u_set_zombie_hud(classid, const hudkill[], const HealthHud[])
native g4u_set_zombie_health(classid, Float:fHealth, Float:sHealth, Float:tHealth)
native g4u_set_zombie_gravity(classid, Float:fGravity, Float:sGravity, Float:tGravity)
native g4u_set_zombie_speed(classid, Float:fSpeed, Float:sSpeed, Float:tSpeed)
native g4u_set_zombie_damage(classid, Float:fDmg, Float:sDmg, Float:tDmg)
native g4u_set_zombie_sound(classid, const directory[])
native g4u_get_zombie_cost_type(classid)
native g4u_get_zombie_cost(classid)
native Float:g4u_zombie_get_roundtime()
native g4u_is_user_hero(id)
native Float:g4u_get_hero_health()
native g4u_get_human_hope()
native g4u_set_zombie_price(iClass, cost_type, cost)
native g4u_set_zombie_knockback(iClass, Float:fKnockBack, Float:sKnockBack, Float:tKnockBack)
forward zm3_zombie_set_hand(id)
native zm3_update_zombie_model(id)
forward g4u_user_skill_post(id, iClass)