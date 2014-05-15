#define TASK_ITEM_ICE_BLOCK 46752

new arriIceBlockTimer[33];	// Отслеживает,через сколько времени дать игроку флеш гранату


#define message_begin_fl(%1,%2,%3,%4) engfunc(EngFunc_MessageBegin, %1, %2, %3, %4)

#define write_coord_fl(%1) engfunc(EngFunc_WriteCoord, %1)

#define m_pPlayer			41
#define m_pActiveItem		373
#define m_flFlashedUntil	514
#define m_flFlashHoldTime	517
#define OFFSET_WEAPON_CSWID	43
#define Ham_Player_ResetMaxSpeed Ham_Item_PreFrame

#define MAX_WEAPONS		32
#define AMMO_FLASHBANG		11
#define AMMO_HEGRENADE		12
#define AMMO_SMOKEGRENADE	13
#define DMG_GRENADE		(1<<24)
#define FFADE_IN			0x0000 // just here so we don't pass 0 into the function
#define BREAK_GLASS		0x01
#define STATUS_HIDE		0
#define STATUS_SHOW		1
#define STATUS_FLASH		2

#define GLOW_AMOUNT		1.0
#define FROST_RADIUS	240.0

#define NT_FLASHBANG		(1<<0) // 1; CSW:25

#define ICON_HASNADE		1
#define ICON_ISCHILLED		2

#define TASK_REMOVE_CHILL	27226
#define TASK_REMOVE_FREEZE	27227

new const szModelFrozen[]	= "models/pi_shrub.mdl";

new  iPlayerPreThink ,bool:roundRestarting;

new isChilled[33], isFrozen[33], frostKilled[33], novaDisplay[33], Float:glowColor[33][3], Float:oldGravity[33], oldRenderFx[33],
		Float:oldRenderColor[33][3], oldRenderMode[33], Float:oldRenderAmt[33], hasFrostNade[33], nadesBought[33];