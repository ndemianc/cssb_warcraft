

//-----------------FM Форварды-----------------------BEGIN {
public fwSetModel(idEntity,szModel[])
{

	new idOwnerModel = pev(idEntity,pev_owner);
	if(!is_user_connected(idOwnerModel)) 
		return FMRES_IGNORED;

	//Для talisman +
	if ( ITEM_Has( idOwnerModel, ITEM_ICE_BLOCK ) > ITEM_NONE || fWorkTalismanPassive(idOwnerModel, arrTalismanActive[idOwnerModel]) == TALISMAN_ITEM_ICE_BLOCK)
	{
		new Float:fDmgTime;
		pev(idEntity,pev_dmgtime,fDmgTime);
		if(fDmgTime == 0.0) return FMRES_IGNORED;
		
		new iTypeGrenade;
		if(szModel[7] == 'w' && szModel[8] == '_' && szModel[9] == 'f')
			iTypeGrenade = NT_FLASHBANG; 
		
		if(!iTypeGrenade) 
			return FMRES_IGNORED;

		if(hasFrostNade[idOwnerModel] == CSW_FLASHBANG)
		{
			hasFrostNade[idOwnerModel] = 0;
			if(get_pcvar_num(CVAR_wc3_iceblock_icon) == ICON_HASNADE)
				show_icon(idOwnerModel, STATUS_HIDE);
		}

		new iTeam = get_user_team(idOwnerModel);
		set_pev(idEntity,pev_team,iTeam);
		set_pev(idEntity,pev_bInDuck,1); // flag it as a frostnade

		new rgb[3], Float:rgbF[3];
		get_rgb_colors(iTeam,rgb);
		IVecFVec(rgb, rgbF);
		
		// Подсветка
		set_pev(idEntity,pev_rendermode,kRenderNormal);
		set_pev(idEntity,pev_renderfx,kRenderFxGlowShell);
		set_pev(idEntity,pev_rendercolor,rgbF);
		set_pev(idEntity,pev_renderamt,16.0);

		set_beamfollow(idEntity,10,10,rgb,100);
	}

	return FMRES_IGNORED;
}

public fwPlayerPreThink(idUser)
{
	if(isFrozen[idUser])
	{
		set_pev(idUser,pev_velocity,Float:{0.0,0.0,0.0}); // stop motion
		
		new Float:gravity;
		pev(idUser,pev_gravity,gravity);
		
		// remember any gravity changes
		if(gravity != 0.000000001 && gravity != 999999999.9)
			oldGravity[idUser] = gravity;

		// if are on the ground and about to jump, set the gravity too high to really do so
		if((pev(idUser,pev_button) & IN_JUMP) && !(pev(idUser,pev_oldbuttons) & IN_JUMP) && (pev(idUser,pev_flags) & FL_ONGROUND))
			set_pev(idUser,pev_gravity,999999999.9);

		// otherwise, set the gravity so low that they don't fall
		else set_pev(idUser,pev_gravity,0.000000001);
	}

	return FMRES_IGNORED;
}

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------FM Форварды-----------------------END }


//-----------------Сообщения-----------------------BEGIN {
public msgDeathMessage(idMessage,iDestMessage,iEntityMessage)
{
	new idVictim = get_msg_arg_int(2);
	if(!is_user_connected(idVictim) || !frostKilled[idVictim]) 
		return PLUGIN_CONTINUE;

	static szWeapon[64];
	get_msg_arg_string(4,szWeapon,charsmax(szWeapon));
	if(equal(szWeapon,"grenade")) set_msg_arg_string(4,"Ice Block");

	return PLUGIN_CONTINUE;
}
//=============================================================================
//=============================================================================
//=============================================================================

//-----------------Сообщения-----------------------END }

//-----------------События-----------------------BEGIN {
public evResetHud(idUser)
{
	if(!is_user_alive(idUser))
		return;

	if(get_pcvar_num(CVAR_wc3_iceblock_icon) == ICON_HASNADE)
	{
		new status = player_has_frostnade(idUser);
		show_icon(idUser, status);
	}

	return;
}

public sm4_4_2_evRoundRestart()//добавлена в cstrike.inl
{
	roundRestarting = true;
}

public sm4_4_2_evNewRound()//добавлена в events.inl
{
	if(roundRestarting)
	{
		roundRestarting = false;
		
		for(new i = 1; i <= get_maxplayers(); i++)
			hasFrostNade[i] = 0;
	}
}
//=============================================================================
//=============================================================================
//=============================================================================

//-----------------Сообщения-----------------------END }


//-----------------HAM Форварды-----------------------BEGIN {
public sm4_4_2_hamPlayerSpawn(idUser)//добавлена в events.inl
{
	nadesBought[idUser] = 0;
	
	if(is_user_alive(idUser))
	{
		if(isChilled[idUser]) task_remove_chill(TASK_REMOVE_CHILL + idUser);
		if(isFrozen[idUser]) task_remove_freeze(TASK_REMOVE_FREEZE + idUser);
	}
	
	return HAM_IGNORED;
}

public sm4_4_2_hamPlayerKilled(idUser)//добавлена в showwin.inl
{
	sm4_4_2_UnfrozenPlayer(idUser);
	
//	return HAM_IGNORED;
}

public fwHamPlayerResetMaxSpeed(idUser)
{
	set_user_chillfreeze_speed(idUser);

	// Set the user's speed
	SHARED_SetSpeed( idUser );

	// Check to see if we should set the player's invisibility
	SHARED_INVIS_Set( idUser );

	return HAM_IGNORED;
}

public fwHamThinkGrenade(idEntity)
{
	if(!pev_valid(idEntity) || !pev(idEntity,pev_bInDuck)) 
		return HAM_IGNORED;
	
	new Float:fDmgTime;
	pev(idEntity,pev_dmgtime,fDmgTime);
	if(fDmgTime > get_gametime()) 
		return HAM_IGNORED;

	frostnade_explode(idEntity);

	return HAM_SUPERCEDE;
}

public fwHamUsePlayerWeaponstrip(idEntity, idUserCaller, idactivator, use_type, Float:value)
{
	if(idUserCaller >= 1 && idUserCaller <= get_maxplayers())
	{
		hasFrostNade[idUserCaller] = 0;

		if(is_user_alive(idUserCaller) && get_pcvar_num(CVAR_wc3_iceblock_icon) == ICON_HASNADE)
		{
			new iStatusShow = player_has_frostnade(idUserCaller);
			show_icon(idUserCaller, iStatusShow);
		}
	}

	return HAM_IGNORED;
}

public fwHamItemDeployFlashBang(idEntity)
{	
	if(pev_valid(idEntity))
	{
		grenade_deployed(get_pdata_cbase(idEntity, m_pPlayer, 4),
		get_pdata_int(idEntity, OFFSET_WEAPON_CSWID, 4));
	}
	
	return HAM_IGNORED;
}

public fwHamItemHolsterFlashBang(idEntity)
{
	if(pev_valid(idEntity))
	{
		grenade_holstered(get_pdata_cbase(idEntity, m_pPlayer, 4),
		get_pdata_int(idEntity, OFFSET_WEAPON_CSWID, 4));
	}
	
	return HAM_IGNORED;
}

public fwHamItemAddToPlayerFlashBang(idEntity, idUser)
{
	if(pev_valid(idEntity))
		grenade_added(idUser, get_pdata_int(idEntity, OFFSET_WEAPON_CSWID, 4));

	return HAM_IGNORED;
}

public fwHamItemAddDuplicateFlashBang(idEntity, idOriginal)
{
	if(pev_valid(idOriginal))
		grenade_added(pev(idOriginal, pev_owner), get_pdata_int(idOriginal, OFFSET_WEAPON_CSWID, 4));
	
	return HAM_IGNORED;
}
//=============================================================================
//=============================================================================
//=============================================================================

//-----------------HAM Форварды-----------------------END }
grenade_deployed(idUser, wid)
{
	// if we should worry about managing my icon now
	if(is_user_alive(idUser) && get_pcvar_num(CVAR_wc3_iceblock_icon) == ICON_HASNADE)
	{
		// if I just switched to a frost grenade
		if( wid == hasFrostNade[idUser] ||  is_wid_in_nadetypes(wid) )
		{
			show_icon(idUser, STATUS_FLASH);
		}
	}
}

grenade_holstered(idUser, wid)
{	
	// if we should worry about managing my icon now
	if(is_user_alive(idUser) && get_pcvar_num(CVAR_wc3_iceblock_icon) == ICON_HASNADE)
	{
		// if I just holstered a frost grenade		
		if( wid == hasFrostNade[idUser] ||  is_wid_in_nadetypes(wid))
		{
			new status = (player_has_frostnade(idUser) != STATUS_HIDE ? STATUS_SHOW : STATUS_HIDE);
			show_icon(idUser, status);
		}
	}
}

grenade_added(idUser, wid)
{
	// if we should worry about managing my icon now
	if(is_user_alive(idUser) && get_pcvar_num(CVAR_wc3_iceblock_icon) == ICON_HASNADE)
	{
		// if I just got a frost grenade
		if( wid == hasFrostNade[idUser] ||  is_wid_in_nadetypes(wid) )
		{
			new status = player_has_frostnade(idUser);
			show_icon(idUser, status);
		}
	}
}

// a frost grenade explodes
public frostnade_explode(ent)
{
	new nadeTeam = pev(ent,pev_team), owner = pev(ent,pev_owner), Float:nadeOrigin[3];
	pev(ent,pev_origin,nadeOrigin);
	
	// make the smoke
	message_begin_fl(MSG_PVS,SVC_TEMPENTITY,nadeOrigin,0);
	write_byte(TE_SMOKE);
	write_coord_fl(nadeOrigin[0]); // x
	write_coord_fl(nadeOrigin[1]); // y
	write_coord_fl(nadeOrigin[2]); // z
	write_short(g_iSprites[SPR_SMOKE]); // sprite
	write_byte(random_num(30,40)); // scale
	write_byte(5); // framerate
	message_end();
	
	// explosion
	create_blast(nadeTeam,nadeOrigin);
	emit_sound(ent,CHAN_ITEM,g_szSounds[SOUND_ICE_BLOCK_EXPLODE],VOL_NORM,ATTN_NORM,0,PITCH_HIGH);

	// cache our cvars
	new ff = get_cvar_pointer("mp_friendlyfire"),
			bFreezeTeam = get_pcvar_num(CVAR_wc3_iceblock_fteam);

	new ta, Float:targetOrigin[3], Float:distance, tr = create_tr2(), Float:fraction, Float:damage, gotFrozen = 0;
	for(new target = 1; target <= get_maxplayers(); target++)
	{

		// dead, invincible, or self attack that is not allowed
		if(!is_user_alive(target) || pev(target,pev_takedamage) == DAMAGE_NO
//		|| (pev(target,pev_flags) & FL_GODMODE) ||(target == owner && !hitself) ||
		|| (pev(target,pev_flags) & FL_GODMODE) ||(get_user_team(target) == get_user_team(owner) && !bFreezeTeam)
			)
			continue;
		
		// this is a team attack with ff disabled, excluding self attack
		ta = (get_user_team(target) == nadeTeam);
		if(ta && !ff && target != owner) continue;
		
		pev(target,pev_origin,targetOrigin);
		distance = vector_distance(nadeOrigin,targetOrigin);
		
		// too far
		if(distance > FROST_RADIUS) continue;

		// check line of sight - игрок на прямой линии
		nadeOrigin[2] += 2.0;
		engfunc(EngFunc_TraceLine,nadeOrigin,targetOrigin,DONT_IGNORE_MONSTERS,ent,tr);
		nadeOrigin[2] -= 2.0;

		get_tr2(tr,TR_flFraction,fraction);
		if(fraction != 1.0 && get_tr2(tr,TR_pHit) != target) continue;

		// damaged - повреждения
		if(get_pcvar_float(CVAR_wc3_iceblock_damage) > 0.0)
		{
			damage = radius_calc(distance,FROST_RADIUS,1.0,get_pcvar_float(CVAR_wc3_iceblock_damage));
			if(ta) damage /= 2.0;

			if(damage > 0.0)
			{
				frostKilled[target] = 1;
				ExecuteHamB(Ham_TakeDamage,target,ent,owner,damage,DMG_GRENADE);
				if(!is_user_alive(target)) continue; // dead now
				frostKilled[target] = 0;
			}
		}

		// frozen
		if((radius_calc(distance,FROST_RADIUS,100.0,0.0) >= 0.0)
		|| (random_num(1,100) <= floatround(radius_calc(distance,FROST_RADIUS,100.0,100.0))))		
		{
			if(freeze_player(target,nadeTeam))
			{
				gotFrozen = 1;
				emit_sound(target,CHAN_ITEM,g_szSounds[SOUND_ICE_BLOCK_FROZEN],1.0,ATTN_NONE,0,PITCH_LOW);
			}
		}
		
		// chilled - Заморозка если игкрок на краю
		if(random_num(1,100) <= floatround(radius_calc(distance,FROST_RADIUS,100.0,100.0)))
		{
			if(chill_player(target,nadeTeam))
			{
				if(!gotFrozen) emit_sound(target,CHAN_ITEM,g_szSounds[SOUND_ICE_BLOCK_CHILLED],VOL_NORM,ATTN_NORM,0,PITCH_HIGH);
			}
		}
	}

	free_tr2(tr);
	set_pev(ent,pev_flags,pev(ent,pev_flags)|FL_KILLME);
}

freeze_player(idUser,nadeTeam)
{
	if(!isFrozen[idUser])
	{
		pev(idUser,pev_gravity,oldGravity[idUser]);

		// register our forward only when we need it
		if(!iPlayerPreThink)
		{
			iPlayerPreThink = register_forward(FM_PlayerPreThink,"fwPlayerPreThink",0);
		}
	}

	isFrozen[idUser] = nadeTeam;
	
	set_pev(idUser,pev_velocity,Float:{0.0,0.0,0.0});
	set_user_chillfreeze_speed(idUser);
	
	new Float:duration = 4.0, Float:variance = 0.5;
	duration += random_float(-variance,variance);

	remove_task(TASK_REMOVE_FREEZE + idUser);
	set_task(duration,"task_remove_freeze",TASK_REMOVE_FREEZE + idUser);
	
	if(!pev_valid(novaDisplay[idUser])) create_nova(idUser);
	
	if(get_pcvar_num(CVAR_wc3_iceblock_icon) == ICON_ISCHILLED)
	{
		show_icon(idUser, STATUS_FLASH);
	}
	
	return 1;
}

public task_remove_freeze(taskid)
{
	new idUser = taskid - TASK_REMOVE_FREEZE;
	
	if(pev_valid(novaDisplay[idUser]))
	{
		new Float:origin[3];
		pev(novaDisplay[idUser],pev_origin,origin);

		// add some tracers
		message_begin_fl(MSG_PVS,SVC_TEMPENTITY,origin,0);
		write_byte(TE_IMPLOSION);
		write_coord_fl(origin[0]); // x
		write_coord_fl(origin[1]); // y
		write_coord_fl(origin[2] + 8.0); // z
		write_byte(64); // radius
		write_byte(10); // count
		write_byte(3); // duration
		message_end();

		// add some sparks
		message_begin_fl(MSG_PVS,SVC_TEMPENTITY,origin,0);
		write_byte(TE_SPARKS);
		write_coord_fl(origin[0]); // x
		write_coord_fl(origin[1]); // y
		write_coord_fl(origin[2]); // z
		message_end();

		// add the shatter
		message_begin_fl(MSG_PAS,SVC_TEMPENTITY,origin,0);
		write_byte(TE_BREAKMODEL);
		write_coord_fl(origin[0]); // x
		write_coord_fl(origin[1]); // y
		write_coord_fl(origin[2] + 24.0); // z
		write_coord_fl(16.0); // size x
		write_coord_fl(16.0); // size y
		write_coord_fl(16.0); // size z
		write_coord(random_num(-50,50)); // velocity x
		write_coord(random_num(-50,50)); // velocity y
		write_coord_fl(25.0); // velocity z
		write_byte(10); // random velocity
		write_short(mdlGlassSpr); // model
		write_byte(10); // count
		write_byte(25); // life
		write_byte(BREAK_GLASS); // flags
		message_end();

		emit_sound(novaDisplay[idUser],CHAN_ITEM,g_szSounds[SOUND_ICE_BLOCK_UNFROZEN],VOL_NORM,ATTN_NORM,0,PITCH_LOW);
		set_pev(novaDisplay[idUser],pev_flags,pev(novaDisplay[idUser],pev_flags)|FL_KILLME);
	}

	isFrozen[idUser] = 0;
	novaDisplay[idUser] = 0;
	
	// unregister forward if we are no longer using it
	unregister_prethink();

	if(!is_user_connected(idUser)) return;
	
	// restore speed, but then check for chilled
	ExecuteHam(Ham_Player_ResetMaxSpeed, idUser);
	set_user_chillfreeze_speed(idUser);

	set_pev(idUser,pev_gravity,oldGravity[idUser]);
	
	new status = STATUS_HIDE;
	
	// sometimes trail fades during freeze, reapply
	if(isChilled[idUser])
	{
		status = STATUS_SHOW;
		
		new rgb[3];
		get_rgb_colors(isChilled[idUser],rgb);
		set_beamfollow(idUser,30,8,rgb,100);
	}
	
	if(get_pcvar_num(CVAR_wc3_iceblock_icon) == ICON_ISCHILLED)
	{
		show_icon(idUser, status);
	}
}

chill_player(idUser,nadeTeam)
{
	// we aren't already been chilled
	if(!isChilled[idUser])
	{
		oldRenderFx[idUser] = pev(idUser,pev_renderfx);
		pev(idUser,pev_rendercolor,oldRenderColor[idUser]);
		oldRenderMode[idUser] = pev(idUser,pev_rendermode);
		pev(idUser,pev_renderamt,oldRenderAmt[idUser]);

		isChilled[idUser] = nadeTeam;
	}

	isChilled[idUser] = nadeTeam;
	
	set_user_chillfreeze_speed(idUser);
	
	new Float:duration = 7.0, Float:variance = 1.0;
	duration += random_float(-variance,variance);

	remove_task(TASK_REMOVE_CHILL + idUser);
	set_task(duration,"task_remove_chill",TASK_REMOVE_CHILL + idUser);

	new rgb[3];
	get_rgb_colors(nadeTeam,rgb);
	
	IVecFVec(rgb, glowColor[idUser]);
	
	// glowshell
	set_user_rendering(idUser, kRenderFxGlowShell, rgb[0], rgb[1], rgb[2], kRenderNormal, floatround(GLOW_AMOUNT));

	set_beamfollow(idUser,30,8,rgb,100);

	// I decided to let the frostnade tint override a flashbang,
	// because if you are frozen, then you have much bigger problems.

	// add a blue tint to their screen
	message_begin(MSG_ONE,gmsgScreenFade,_,idUser);
	write_short(floatround(4096.0 * duration)); // duration
	write_short(floatround(3072.0 * duration)); // hold time (4096.0 * 0.75)
	write_short(FFADE_IN); // flags
	write_byte(rgb[0]); // red
	write_byte(rgb[1]); // green
	write_byte(rgb[2]); // blue
	write_byte(100); // alpha
	message_end();
	
	if(get_pcvar_num(CVAR_wc3_iceblock_icon) == ICON_ISCHILLED && !isFrozen[idUser])
	{
		show_icon(idUser, STATUS_SHOW);
	}

	return 1;
}

public task_remove_chill(taskid)
{
	new idUser = taskid - TASK_REMOVE_CHILL;

	isChilled[idUser] = 0;
	
	// unregister forward if we are no longer using it
	//unregister_prethink();

	if(!is_user_connected(idUser)) return;
	
	// set speed to normal, then check for frozen
	ExecuteHam(Ham_Player_ResetMaxSpeed, idUser);
	set_user_chillfreeze_speed(idUser);

	// reset rendering
	set_user_rendering(idUser, oldRenderFx[idUser], floatround(oldRenderColor[idUser][0]), floatround(oldRenderColor[idUser][1]),
		floatround(oldRenderColor[idUser][2]), oldRenderMode[idUser], floatround(oldRenderAmt[idUser]));

	clear_beamfollow(idUser);

	// calculate end of flashbang
	new Float:flashedUntil = get_pdata_float(idUser,m_flFlashedUntil),
			Float:flashHoldTime = get_pdata_float(idUser,m_flFlashHoldTime),
			Float:endOfFlash = flashedUntil + (flashHoldTime * 0.67);
	
	// not blinded
	if(get_gametime() >= endOfFlash)
	{
		// clear tint
		message_begin(MSG_ONE,gmsgScreenFade,_,idUser);
		write_short(0); // duration
		write_short(0); // hold time
		write_short(FFADE_IN); // flags
		write_byte(0); // red
		write_byte(0); // green
		write_byte(0); // blue
		write_byte(255); // alpha
		message_end();
	}
	
	if(get_pcvar_num(CVAR_wc3_iceblock_icon) == ICON_ISCHILLED && !isFrozen[idUser])
	{
		show_icon(idUser, STATUS_HIDE);
	}
}

// make a frost nova at a player's feet
create_nova(idUser)
{
	new nova = engfunc(EngFunc_CreateNamedEntity,engfunc(EngFunc_AllocString,"info_target"));

	engfunc(EngFunc_SetSize,nova,Float:{-8.0,-8.0,-4.0},Float:{8.0,8.0,4.0});
	engfunc(EngFunc_SetModel,nova,szModelFrozen);

	// random orientation
	new Float:angles[3];
	angles[1] = random_float(0.0,360.0);
	set_pev(nova,pev_angles,angles);

	// put it at their feet
	new Float:novaOrigin[3];
	pev(idUser,pev_origin,novaOrigin);
	engfunc(EngFunc_SetOrigin,nova,novaOrigin);

	// make it translucent
	new rgb[3];
	get_rgb_colors(isFrozen[idUser], rgb);
	IVecFVec(rgb, angles); // let's just use angles

	set_pev(nova,pev_rendercolor,angles); // see above
	set_pev(nova,pev_rendermode,kRenderTransAlpha);
	set_pev(nova,pev_renderfx,kRenderFxGlowShell);
	set_pev(nova,pev_renderamt,128.0);

	novaDisplay[idUser] = nova;

}

/****************************************
* UTILITY FUNCTIONS
****************************************/

// check if prethink is still being used, if not, unhook it
unregister_prethink()
{
	if(iPlayerPreThink)
	{
		new i;
		for(i = 1; i <= get_maxplayers();i++) 
			if(isFrozen[i]) break;
			
		if(i > get_maxplayers())
		{
			unregister_forward(FM_PlayerPreThink,iPlayerPreThink,0);
			iPlayerPreThink = 0;
		}
	}
}

// make the explosion effects
create_blast(team,Float:origin[3])
{
	new rgb[3];
	get_rgb_colors(team,rgb);

	// smallest ring
	message_begin_fl(MSG_PVS,SVC_TEMPENTITY,origin,0);
	write_byte(TE_BEAMCYLINDER);
	write_coord_fl(origin[0]); // x
	write_coord_fl(origin[1]); // y
	write_coord_fl(origin[2]); // z
	write_coord_fl(origin[0]); // x axis
	write_coord_fl(origin[1]); // y axis
	write_coord_fl(origin[2] + 385.0); // z axis
	write_short(g_iSprites[SPR_SHOCKWAVE]); // sprite
	write_byte(0); // start frame
	write_byte(0); // framerate
	write_byte(4); // life
	write_byte(60); // width
	write_byte(0); // noise
	write_byte(rgb[0]); // red
	write_byte(rgb[1]); // green
	write_byte(rgb[2]); // blue
	write_byte(100); // brightness
	write_byte(0); // speed
	message_end();

	// medium ring
	message_begin_fl(MSG_PVS,SVC_TEMPENTITY,origin,0);
	write_byte(TE_BEAMCYLINDER);
	write_coord_fl(origin[0]); // x
	write_coord_fl(origin[1]); // y
	write_coord_fl(origin[2]); // z
	write_coord_fl(origin[0]); // x axis
	write_coord_fl(origin[1]); // y axis
	write_coord_fl(origin[2] + 470.0); // z axis
	write_short(g_iSprites[SPR_SHOCKWAVE]); // sprite
	write_byte(0); // start frame
	write_byte(0); // framerate
	write_byte(4); // life
	write_byte(60); // width
	write_byte(0); // noise
	write_byte(rgb[0]); // red
	write_byte(rgb[1]); // green
	write_byte(rgb[2]); // blue
	write_byte(100); // brightness
	write_byte(0); // speed
	message_end();

	// largest ring
	message_begin_fl(MSG_PVS,SVC_TEMPENTITY,origin,0);
	write_byte(TE_BEAMCYLINDER);
	write_coord_fl(origin[0]); // x
	write_coord_fl(origin[1]); // y
	write_coord_fl(origin[2]); // z
	write_coord_fl(origin[0]); // x axis
	write_coord_fl(origin[1]); // y axis
	write_coord_fl(origin[2] + 555.0); // z axis
	write_short(g_iSprites[SPR_SHOCKWAVE]); // sprite
	write_byte(0); // start frame
	write_byte(0); // framerate
	write_byte(4); // life
	write_byte(60); // width
	write_byte(0); // noise
	write_byte(rgb[0]); // red
	write_byte(rgb[1]); // green
	write_byte(rgb[2]); // blue
	write_byte(100); // brightness
	write_byte(0); // speed
	message_end();

	// light effect
	message_begin_fl(MSG_PAS,SVC_TEMPENTITY,origin,0);
	write_byte(TE_DLIGHT);
	write_coord_fl(origin[0]); // x
	write_coord_fl(origin[1]); // y
	write_coord_fl(origin[2]); // z
	write_byte(floatround(FROST_RADIUS/5.0)); // radius
	write_byte(rgb[0]); // r
	write_byte(rgb[1]); // g
	write_byte(rgb[2]); // b
	write_byte(8); // life
	write_byte(60); // decay rate
	message_end();
}

// give an entity a beam trail
set_beamfollow(ent,life,width,rgb[3],brightness)
{
	clear_beamfollow(ent);

	message_begin(MSG_BROADCAST,SVC_TEMPENTITY);
	write_byte(TE_BEAMFOLLOW);
	write_short(ent); // entity
	write_short(g_iSprites[SPR_ICE_BLOCK_TRAIL]); // sprite
	write_byte(life); // life
	write_byte(width); // width
	write_byte(rgb[0]); // red
	write_byte(rgb[1]); // green
	write_byte(rgb[2]); // blue
	write_byte(brightness); // brightness
	message_end();
}

// removes beam trails from an entity
clear_beamfollow(ent)
{
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY);
	write_byte(TE_KILLBEAM);
	write_short(ent); // entity
	message_end();
}

// gets the appropriate color and displays the frostnade icon to the player with the given status
show_icon(idUser, status)
{
	//Для talisman +
	if ( ITEM_Has(idUser, ITEM_ICE_BLOCK ) > ITEM_NONE || fWorkTalismanPassive(idUser, arrTalismanActive[idUser]) == TALISMAN_ITEM_ICE_BLOCK)
	{
		static rgb[3];
		if(status) get_rgb_colors(get_user_team(idUser), rgb); // only get colors if we need to
		
		message_begin(MSG_ONE,gmsgStatusIcon,_,idUser);
		write_byte(status); // status (0=hide, 1=show, 2=flash)
//		write_string("suithelmet_empty"); // sprite name
//		write_string("smallskull"); // sprite name
		write_string("d_skull"); // sprite name


		write_byte(rgb[0]); // red
		write_byte(rgb[1]); // green
		write_byte(rgb[2]); // blue
		message_end();
	}
}

is_wid_in_nadetypes(wid)
{
	new types = NT_FLASHBANG;
	
	return ((wid == CSW_FLASHBANG && (types & NT_FLASHBANG)));
}

player_has_frostnade(idUser)
{
	new retVal = STATUS_HIDE, curwpn = get_user_weapon(idUser);

	// no override, variable explicitly set
	if(hasFrostNade[idUser])
	{
		retVal = (curwpn == hasFrostNade[idUser] ? STATUS_FLASH : STATUS_SHOW);
	}
	
	// override enabled, and I'm on the right team
	else
	{		
		new types = NT_FLASHBANG;

		if(retVal != STATUS_FLASH && (types & NT_FLASHBANG) && cs_get_user_bpammo(idUser, CSW_FLASHBANG) > 0)
		{
			retVal = (curwpn == CSW_FLASHBANG ? STATUS_FLASH : STATUS_SHOW);
		}
	}
	
	return retVal;
}

// gets RGB colors from the cvar
get_rgb_colors(team,rgb[3])
{
	static color[12], parts[3][4];
	get_pcvar_string(CVAR_wc3_iceblock_color,color,11);
	
	// if cvar is set to "team", use colors based on the given team
	if(equali(color,"team",4))
	{
		if(team == 1)
		{
			rgb[0] = 150;
			rgb[1] = 0;
			rgb[2] = 0;
		}
		else
		{
			rgb[0] = 0;
			rgb[1] = 0;
			rgb[2] = 150;
		}
	}
	else
	{
		parse(color,parts[0],3,parts[1],3,parts[2],3);
		rgb[0] = str_to_num(parts[0]);
		rgb[1] = str_to_num(parts[1]);
		rgb[2] = str_to_num(parts[2]);
	}
}
// EXAMPLE: fDistance = 60.0, fRadius = 240.0, maxVal = 100.0, minVal = 20.0
// we are 0.75 (1.0-(60.0/240.0)) of the way to the radius, so scaled with our
// values, it comes out to 80.0 (20.0 + (0.75 * (100.0 - 20.0)))
Float:radius_calc(Float:fDistance,Float:fRadius,Float:fMaxValue,Float:fMinValue)
{
	if(fMaxValue <= 0.0) return 0.0;
	if(fMinValue >= fMaxValue) return fMinValue;
	return fMinValue + ((1.0 - (fDistance / fRadius)) * (fMaxValue - fMinValue));
}

// sets a user's chilled/frozen speed if applicable
// (NOTE: does NOT reset his maxspeed if he is not chilled/frozen)
set_user_chillfreeze_speed(idUser)
{
	if(isFrozen[idUser])
	{
		set_user_maxspeed(idUser, 1.0);
	}
	else if(isChilled[idUser])
	{
		set_user_maxspeed(idUser, get_default_maxspeed(idUser)* 0.6);
	}
}

// gets the maxspeed a user should have, given his current weapon
stock Float:get_default_maxspeed(idUser)
{	
	new wEnt = get_pdata_cbase(idUser, m_pActiveItem), Float:result = 250.0;

	if(pev_valid(wEnt))
	{
		ExecuteHam(Ham_CS_Item_GetMaxSpeed, wEnt, result);
	}
	
	return result;
}



//Разморозка игрока
public sm4_4_2_UnfrozenPlayer(idUser)//добавлена в events.inl
{
	hasFrostNade[idUser] = 0;

	if(get_pcvar_num(CVAR_wc3_iceblock_icon) == ICON_HASNADE)
			show_icon(idUser, STATUS_HIDE);

	if(isChilled[idUser]) task_remove_chill(TASK_REMOVE_CHILL + idUser);
	if(isFrozen[idUser]) task_remove_freeze(TASK_REMOVE_FREEZE + idUser);
	
	return HAM_IGNORED;
}


//------------------SHARED----------------------BEGIN {
SHARED_HasIceBlock( idUser )
{
	new i, bool:bIceBlockFound = false;
	new iCountFlashBang;
	
	// Loop through all weapons and search for a grenade
	while ( g_PlayerWeapons[idUser][i] && !bIceBlockFound )
	{
		if ( SHARED_IsIceBlock( g_PlayerWeapons[idUser][i] ) )
		{
			iCountFlashBang++;

			if(iCountFlashBang == 2)
			{
				bIceBlockFound = true;
				iCountFlashBang = 0;
				break;
			}
		}

		i++;
	}

	return bIceBlockFound;
}

bool:SHARED_IsIceBlock( iWeapon )
{
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( iWeapon == CSW_FLASHBANG )
		{
			return true;
		}
	}

	return false;
}
//=============================================================================
//=============================================================================
//=============================================================================

//-----------------SHARED-----------------------END }