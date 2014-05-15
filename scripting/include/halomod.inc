#if !defined _fakemeta_included
#include <fakemeta>
#endif

#if !defined _hamsandwich_included
#include <hamsandwich>
#endif

#if defined _halomod_included
#endinput
#endif
#define _halomod_included

#define fm_create_entity(%1) engfunc(EngFunc_CreateNamedEntity, engfunc(EngFunc_AllocString, %1))

#define HALOVERSION "3.4"

#define HALOWEAPS 6

#define HALO_TEAM_SLAYER 0
#define HALO_SLAYER 1
#define HALO_JUGGERNAUT 2

#define t 1
#define ct 2


public is_halo_on()
	return get_cvar_num("halo_on")

public get_halo_gravity()
	return get_cvar_num("halo_gravity")

public get_halo_type()
	return get_cvar_num("halo_type")

public play_sound(id, const sound[])
{
	if(is_halo_on())
		client_cmd(id,"mp3 play sound/halomodv3/%s.mp3", sound)
}

stock gWeapId[] = {
	CSW_FAMAS,
	CSW_P90,
	CSW_M3,
	CSW_DEAGLE,
	CSW_KNIFE,
	CSW_HEGRENADE
}	

stock fm_find_ent_by_owner(index, const classname[], owner, jghgtype = 0) {
	new strtype[11] = "classname", ent = index
	switch (jghgtype) {
		case 1: strtype = "target"
			case 2: strtype = "targetname"
		}
	
	while ((ent = engfunc(EngFunc_FindEntityByString, ent, strtype, classname)) && pev(ent, pev_owner) != owner) {}
	
	return ent
}

stock fm_strip_user_weapons(index) {
	new ent = fm_create_entity("player_weaponstrip")
	if (!pev_valid(ent))
		return 0
	
	dllfunc(DLLFunc_Spawn, ent)
	dllfunc(DLLFunc_Use, ent, index)
	engfunc(EngFunc_RemoveEntity, ent)
	
	return 1
}

stock bacon_give_weapon(index, weapon[]) {
	if(!equal(weapon,"weapon_", 7) || user_has_weapon(index, get_weaponid(weapon))) return 0
	
	static ent; ent = engfunc(EngFunc_CreateNamedEntity, engfunc(EngFunc_AllocString, weapon))
	if(!pev_valid(ent)) return 0
	
	set_pev(ent, pev_spawnflags, SF_NORESPAWN)
	dllfunc(DLLFunc_Spawn, ent)
	
	if(!ExecuteHamB(Ham_AddPlayerItem, index, ent))
	{
		if(pev_valid(ent)) set_pev(ent, pev_flags, pev(ent, pev_flags) | FL_KILLME)
		return 0
	}
	ExecuteHamB(Ham_Item_AttachToPlayer, ent, index)
	
	return 1
}
