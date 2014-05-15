#if defined _blood_included
	#endinput
#endif
#define _blood_included

#include <amxmodx>
#include <fakemeta>
#include <xs>

#define	DONT_BLEED		-1
#define	BLOOD_COLOR_RED		247
#define	BLOOD_COLOR_YELLOW	195
#define	BLOOD_COLOR_GREEN	BLOOD_COLOR_YELLOW

new const GUNSHOT_DECALS[] = { 41, 42, 43, 44, 45 }
new const BLOOD_DECALS[] = { 27, 28, 29, 30, 31, 32 }
//new const YBLOOD_DECALS[] = { 216, 217, 218, 219, 220, 221 }

stock UTIL_TraceLine(const Float:vecStart[3], const Float:vecEnd[3], ignore, pentIgnore)
{
	new ptr
	engfunc(EngFunc_TraceLine, vecStart, vecEnd, ignore, pentIgnore, ptr);
	return ptr;
}

stock bool:UTIL_ShouldShowBlood(color)
{
	if(color != DONT_BLEED)
	{
		if(color == BLOOD_COLOR_RED)
		{
			if(get_cvar_float("violence_hblood") != 0.0)
				return true;
		}
		else
		{
			if(get_cvar_float("violence_ablood") != 0.0)
				return true;
		}
	}
	return false;
}

stock UTIL_BloodDrips(const Float:origin[3], color, amount, g_sModelIndexBloodSpray, g_sModelIndexBloodDrop)
{
	if ( !UTIL_ShouldShowBlood( color ) )
		return 0;

	if ( color == DONT_BLEED || amount == 0 )
		return 0;

	amount *= 2;

	if ( amount > 255 )
		amount = 255;
		
	new origin2[3]
	origin2[0] = floatround(origin[0])
	origin2[1] = floatround(origin[1])
	origin2[2] = floatround(origin[2])

	message_begin(MSG_PVS, SVC_TEMPENTITY, origin2);
	write_byte(TE_BLOODSPRITE);
	write_coord(floatround(origin[0]));								// pos
	write_coord(floatround(origin[1]));
	write_coord(floatround(origin[2]));
	write_short(g_sModelIndexBloodSpray);				// initial sprite model
	write_short(g_sModelIndexBloodDrop);				// droplet sprite models
	write_byte(color);								// color index into host_basepal
	write_byte(min(max(3, amount / 10), 16));		// size
	message_end();
	
	return 1;
}

stock UTIL_BloodDecalTrace(pTrace, color)
{
	if(UTIL_ShouldShowBlood(color))
	{
		UTIL_DecalTrace(pTrace, BLOOD_DECALS[random_num(0, sizeof BLOOD_DECALS -1)])
		return 1;
	}
	return 0;
}

stock UTIL_DecalTrace(pTrace, index)
{
	new entityIndex;
	new message;
	new id;

	if(index < 0)
		return 0;

	if(get_tr2(pTrace, TR_flFraction) == 1.0)
		return 0;

	id = get_tr2(pTrace, TR_pHit)
	
	// Only decal BSP models
	if(id)
	{
		//if (!IsBSPModel(id))
		//	return 0;
		entityIndex = id;
	}
	else
	{
		entityIndex = 0;
	}

	message = TE_DECAL;
	
	if(entityIndex != 0)
	{
		if(index > 255)
		{
			message = TE_DECALHIGH;
			index -= 256;
		}
	}
	else
	{
		message = TE_WORLDDECAL;
		if(index > 255)
		{
			message = TE_WORLDDECALHIGH;
			index -= 256;
		}
	}
	
	new Float:vecEndPos[3]
	get_tr2(pTrace, TR_vecEndPos, vecEndPos)
	
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(message);
	write_coord(floatround(vecEndPos[0]));
	write_coord(floatround(vecEndPos[1]));
	write_coord(floatround(vecEndPos[2]));
	write_byte(index);
	if(entityIndex)
	{
		write_short(entityIndex);
	}
	message_end();
	
	return 1;
}

stock bool:IsBSPModel(id)
{
	return pev(id, pev_solid) == SOLID_BSP || pev(id, pev_movetype) == MOVETYPE_PUSHSTEP;
}

stock SpawnBlood(Float:vecSpot[3], bloodColor, Float:flDamage, g_sModelIndexBloodSpray, g_sModelIndexBloodDrop)
{
	UTIL_BloodDrips(vecSpot, bloodColor, floatround(flDamage), g_sModelIndexBloodSpray, g_sModelIndexBloodDrop);
	return 1;
}

stock TraceBleed(Float:flDamage, Float:vecDir[3], ptr, bitsDamageType, id, color)
{
	if(color == DONT_BLEED)
		return 0;
	
	if (flDamage == 0.0)
		return 0;

	if(!(bitsDamageType & (DMG_CRUSH | DMG_BULLET | DMG_SLASH | DMG_BLAST | DMG_CLUB | DMG_MORTAR)))
		return 0;
	
	// make blood decal on the wall! 
	new Bloodtr;
	new Float:vecTraceDir[3]; 
	new Float:flNoise;
	new cCount;
	new Float:vecEnd[3];
	new Float:vecEndPos[3];
	get_tr2(ptr, TR_vecEndPos, vecEndPos)

	if (flDamage < 10.0)
	{
		flNoise = 0.1;
		cCount = 1;
	}
	else if (flDamage < 25.0)
	{
		flNoise = 0.2;
		cCount = 2;
	}
	else
	{
		flNoise = 0.3;
		cCount = 4;
	}

	for(new i = 0; i < cCount; i++)
	{
		xs_vec_mul_scalar(vecDir, -1.0, vecTraceDir)

		vecTraceDir[0] += random_float(-flNoise, flNoise);
		vecTraceDir[1] += random_float(-flNoise, flNoise);
		vecTraceDir[2] += random_float(-flNoise, flNoise);
		
		xs_vec_mul_scalar(vecTraceDir, -172.0, vecTraceDir)
		xs_vec_add(vecEndPos, vecTraceDir, vecEnd)

		Bloodtr = UTIL_TraceLine(vecEndPos, vecEnd, 0, id);
		UTIL_BloodDecalTrace(Bloodtr, color);
	}
	
	return 1;
}

stock DecalGunshot(pTrace)
{
	new pent = get_tr2(pTrace, TR_pHit)
	
	if(IsBSPModel(pent))
	{
		UTIL_GunshotDecalTrace(pTrace, GUNSHOT_DECALS[random_num(0, sizeof GUNSHOT_DECALS -1)]);
		return 1;
	}
	
	return 0;
}

stock UTIL_GunshotDecalTrace(pTrace, index)
{
	if(index < 0)
		return 0;

	if(get_tr2(pTrace, TR_flFraction) == 1.0)
		return 0;
		
	new Float:vecEndPos[3]
	get_tr2(pTrace, TR_vecEndPos, vecEndPos)

	message_begin(MSG_PAS, SVC_TEMPENTITY, pTrace->vecEndPos);
	write_byte(TE_GUNSHOTDECAL);
	write_coord(floatround(vecEndPos[0]));
	write_coord(floatround(vecEndPos[1]));
	write_coord(floatround(vecEndPos[2]));
	write_short(get_tr2(pTrace, TR_pHit));
	write_byte(index);
	message_end();
	
	return 1;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1066\\ f0\\ fs16 \n\\ par }
*/
