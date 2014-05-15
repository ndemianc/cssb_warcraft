/*
*	Race: Frost Mage Functions
*/

#define DAMAGE_BLIZZARD	85		// Повреждения наносимые главной цели супер навыком "Снежная буря"
#define DAMAGE_ICE_BOLT	35		// Повреждения наносимые второстепенной цели от рассыпанных ледяных болтов
#define DAMAGE_BLIZZARD_RADIUS	300	// Радиус повреждений наносимый второстепенной цели от рассыпанных ледяных болтов "Снежная буря"

// ****************************************
// Frost Mage Blizzard Ultimate
// Супер навык "Снежная буря" - Ultimate
// ****************************************
public FM_ULT_Blizzard( iCaster, iTarget, iBodyPart )
{
//	client_print(0,print_chat,"FM_ULT_Blizzard");

	emit_sound(iCaster, CHAN_STATIC, g_szSounds[SOUND_BLIZZARD], 0.5, ATTN_NORM, 0, PITCH_NORM );

	new vOrigin[3];
	get_user_origin( iTarget, vOrigin );

	new ptStartBlizzard[3], ptEndBlizzard[3];
	ptStartBlizzard[0] = vOrigin[0];
	ptStartBlizzard[1] = vOrigin[1];
	ptStartBlizzard[2] = vOrigin[2] + 600;
	ptEndBlizzard[0] = vOrigin[0];
	ptEndBlizzard[1] = vOrigin[1];
	ptEndBlizzard[2] = vOrigin[2] - 600;

	new ptStartIceBolt[3], ptEndIceBolt[3];
	ptStartIceBolt[0] = vOrigin[0];
	ptStartIceBolt[1] = vOrigin[1];
	ptStartIceBolt[2] = vOrigin[2] + 100;
	ptEndIceBolt[0] = vOrigin[0];
	ptEndIceBolt[1] = vOrigin[1];
	ptEndIceBolt[2] = vOrigin[2] + 50;

	WC3_Damage(iTarget, iCaster,DAMAGE_BLIZZARD, CSW_BLIZZARD, iBodyPart );

	Create_TE_BEAMPOINTS(ptStartBlizzard, ptEndBlizzard, g_iSprites[SPR_BLUE_LIGHTNING],0, 0, 15, 255, 0, 255, 255, 255, 255, 0);

	//Ледяные болты
	for(new i = 1; i < 6; i++) 
		Create_TE_SPRITETRAIL(ptStartIceBolt, ptEndIceBolt, g_iSprites[SPR_ICE_FROSTBOLT],50, random_num(27,30), 1, random_num(30,70), 40 )

	new team = get_user_team( iTarget );
	new arrPlayers[32], iNumPlayers, vClosestTargetOrigin[3];
	get_players(arrPlayers, iNumPlayers, "a");

	//Повреждения окружающих рассыпанными ледяными болтами
	for(new i = 0; i < iNumPlayers; i++ )
	{
		if ( is_user_alive( arrPlayers[i] ) )
		{
			if (arrPlayers[i] != iTarget)
			{
				if ( get_user_team( arrPlayers[i] ) == team )
				{
					get_user_origin( arrPlayers[i], vClosestTargetOrigin );

					if ( get_distance( vOrigin, vClosestTargetOrigin ) <= DAMAGE_BLIZZARD_RADIUS )
					{
						WC3_Damage( arrPlayers[i],iCaster, DAMAGE_ICE_BOLT, CSW_BLIZZARD, 0 );
											
						emit_sound(iTarget, CHAN_STATIC, g_szSounds[SOUND_BLIZZARD], 0.5, ATTN_NORM, 0, PITCH_NORM );
					}
				}
			}
		}
	}
	
	return PLUGIN_HANDLED;
}

//Скил 1 уровня - Осколки льда (Ice splinters)
FM_SkillsOffensive( iAttacker, iVictim, iHitPlace )
{
	if ( !p_data_b[iVictim][PB_ISCONNECTED] || !p_data_b[iAttacker][PB_ISCONNECTED] )
		return;

	static iSkillLevel;
		
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_FROST_MAGE_ICE_SPLINTERS );
	if ( iSkillLevel > 0 && p_data_b[iAttacker][PB_ISFROZENSKILL] == false
		&& ((get_user_team( iAttacker ) != get_user_team( iVictim ))))
	{
		if ( random_float(MIN_PERCENT_ICE_SPLINTERS, MAX_PERCENT_ICE_SPLINTERS ) <= p_ice_splinters[iSkillLevel-1] )
		{
			new vVictimOrigin[3], vAttackerorigin[3];
			get_user_origin( iVictim, vVictimOrigin );
			get_user_origin( iAttacker, vAttackerorigin );
						
			//Create_TE_SPRITETRAIL( vAttackerorigin, vVictimOrigin, g_iSprites[SPR_ICE_SPLINTERS], iSkillLevel, 15, 1, 2, 6 );
			Create_TE_SPRITETRAIL(vAttackerorigin, vVictimOrigin, g_iSprites[SPR_ICE_SPLINTERS],50, random_num(27,30), 1, random_num(30,70), 40 )
			
			emit_sound( iAttacker, CHAN_STATIC, g_szSounds[SOUND_ICE_SPLINTERS], 1.0, ATTN_NORM, 0, PITCH_NORM );
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_ICE_SPLINTERS], 1.0, ATTN_NORM, 0, PITCH_NORM );


			WC3_Damage( iVictim, iAttacker,floatround(p_ice_splinters[iSkillLevel-1]), CSW_ICE_SPLINTERS, iHitPlace );
		}
		else if ( get_pcvar_num( CVAR_wc3_psychostats ) )
		{
			new WEAPON = CSW_ICE_SPLINTERS - CSW_WAR3_MIN;

			iStatsShots[iAttacker][WEAPON]++;
		}
	}
		
}

//Скил 2 уровня - Ледяная стрела (Ice Arrow)
FM_SkillsOffensive_Ice_Arrow(idAttacker, idVictim)
{
	if ( !p_data_b[idVictim][PB_ISCONNECTED] || !p_data_b[idAttacker][PB_ISCONNECTED] )
		return;

	static iSkillLevel;
	iSkillLevel = SM_GetSkillLevel( idAttacker, SKILL_FROST_MAGE_ICE_ARROW );
	if ( iSkillLevel > 0 && p_data_b[idAttacker][PB_ISFROZENSKILL] == false 
		&& ((get_user_team( idAttacker ) != get_user_team( idVictim ))))
	{

		if ( random_float(MIN_PERCENT_ICE_ARROW, MAX_PERCENT_ICE_ARROW ) <= p_ice_arrow[iSkillLevel-1] )
		{
			if ( task_exists( TASK_REMOVE_FROZEN + idVictim ) == 0 )
			{
				//client_print(0,print_chat,"SKILL_FROST_MAGE_ICE_ARROW");

				fCreateIceArrowTrail(idAttacker);

				emit_sound(idAttacker, CHAN_STATIC, g_szSounds[SOUND_ICE_ARROW_MISSILE], 1.0, ATTN_NORM, 0, PITCH_NORM );
			}
						
		}
		
	}
		
}

public fCreateIceArrowTrail(idAttacker)
 {
	if (!is_user_alive(idAttacker)) 
		return PLUGIN_HANDLED;

	new Float: fOrigin[3], Float:fAngle[3],Float: fVelocity[3];
	pev(idAttacker, pev_origin, fOrigin);
	pev(idAttacker, pev_view_ofs, fAngle);
	fm_velocity_by_aim(idAttacker, 2.0, fVelocity, fAngle);
	fAngle[0] *= -1.0;
	
	// create ent
	new entIceArrow = engfunc(EngFunc_CreateNamedEntity, engfunc(EngFunc_AllocString, "info_target"));
	set_pev(entIceArrow, pev_classname, "IceArrow");
	engfunc(EngFunc_SetModel, entIceArrow, "sprites/warcraft3/ice_arrow.spr");
	set_pev(entIceArrow, pev_mins, Float:{-1.0, -1.0, -1.0});
	set_pev(entIceArrow, pev_maxs, Float:{1.0, 1.0, 1.0});
	set_pev(entIceArrow, pev_origin, fOrigin);
	fOrigin[0] += fVelocity[0];
	fOrigin[1] += fVelocity[1];
	fOrigin[2] += fVelocity[2];

	entity_set_float(entIceArrow,EV_FL_framerate,1.0);//framerate
	entity_set_int(entIceArrow, EV_INT_rendermode, 5);//randermode
	entity_set_float(entIceArrow, EV_FL_renderamt, 255.0);//visable

	set_pev(entIceArrow, pev_movetype, MOVETYPE_BOUNCE);
	set_pev(entIceArrow, pev_gravity, 0.01);
	fVelocity[0] *= 1000;
	fVelocity[1] *= 1000;
	fVelocity[2] *= 1000;
	set_pev(entIceArrow, pev_velocity, fVelocity);
	set_pev(entIceArrow, pev_owner, idAttacker);
	set_pev(entIceArrow, pev_angles, fAngle);
	set_pev(entIceArrow, pev_solid, SOLID_BBOX);//store the enitty id
	
	// invisible ent
	//fm_set_rendering(ent, kRenderFxGlowShell, 0, 0, 0, kRenderTransAlpha, 0);
	
	// show trail	
	message_begin( MSG_BROADCAST, SVC_TEMPENTITY );
	write_byte(TE_BEAMFOLLOW);
	write_short(entIceArrow);//entity
	write_short(g_iSprites[SPR_ICE_ARROW_TRAIL]);//model
	write_byte(25);// byte (life in 0.1's)
	write_byte(9);// byte (line width in 0.1's)
	write_byte(42);// byte (color)
	write_byte(170);// byte (color)
	write_byte(255);// byte (color)
	write_byte(255);// byte (brightness)
	message_end();

	return PLUGIN_HANDLED;

}

fm_velocity_by_aim(iIndex, Float:fDistance, Float:fVelocity[3], Float:fViewAngle[3])
{
	pev(iIndex, pev_v_angle, fViewAngle);
	fVelocity[0] = floatcos(fViewAngle[1], degrees) * fDistance;
	fVelocity[1] = floatsin(fViewAngle[1], degrees) * fDistance;
	fVelocity[2] = floatcos(fViewAngle[0]+90.0, degrees) * fDistance;
	return 1;
}

//Создание взрыва при прикосновении стрелы с игроком или поверхностью
public fCreateExplodeIceArrow(entIceArrow)
{
	new Float:fOrigin[3];
	new iOrigin[3];
	entity_get_vector(entIceArrow, EV_VEC_origin, fOrigin);
	FVecIVec(fOrigin, iOrigin);

	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(TE_GLOWSPRITE);
	write_coord(iOrigin[0]);
	write_coord(iOrigin[1]);
	write_coord(iOrigin[2]);
	write_short(g_iSprites[SPR_ICE_EXPLODE]);// model
	write_byte(3);// life 0.x sec
	write_byte(6);	// size
	write_byte(110);// brightness
	message_end();

	emit_sound(entIceArrow, CHAN_AUTO, g_szSounds[SOUND_ICE_EXPLODE], 1.0, ATTN_NORM, 0, PITCH_NORM);

	if (is_valid_ent(entIceArrow)) 
		remove_entity(entIceArrow);
}


public touchIceArrow(entIceArrow,idWorldPlayer) 
{
	//Если стрела попадает в какое-нибудь место (стена,пол и т.д.)
	if(idWorldPlayer == 0) 
	{
		fCreateExplodeIceArrow(entIceArrow);
	}
	//Если стрела попадает в игрока
	else if(is_user_connected(idWorldPlayer) && p_data_b[idWorldPlayer][PB_ISFROZENPLAYER] == false)
	{
		//Если стрела попадает в игрока и он не из твоей команды
		new idAttacker = entity_get_edict(entIceArrow,EV_ENT_owner);
		if(get_user_team(idWorldPlayer) != get_user_team(idAttacker))
		{
			//Создание эффекта заморозки
			message_begin(MSG_ONE, get_user_msgid("ScreenFade"), _, idWorldPlayer);
			write_short(0);// duration
			write_short(0);// hold time
			write_short(FFADE_STAYOUT);// fade type
			write_byte(0);// red
			write_byte(50);// green
			write_byte(200);// blue
			write_byte(100);// alpha
			message_end();

			fm_set_rendering(idWorldPlayer, kRenderFxGlowShell, 0, 100, 200, kRenderNormal, 25);

			// Prevent from jumping
			if (pev(idWorldPlayer, pev_flags) & FL_ONGROUND)
				set_pev(idWorldPlayer, pev_gravity, 999999.9);// set really high
			else
				set_pev(idWorldPlayer, pev_gravity, 0.000001);// no gravity

			set_pev(idWorldPlayer, pev_velocity, Float:{0.0,0.0,0.0});// stop motion
			set_pev(idWorldPlayer, pev_maxspeed, 1.0);// prevent from moving

			//set_user_maxspeed(idWorldPlayer, 1.0 );
				
			fCreateExplodeIceArrow(entIceArrow);

			//if ( task_exists( TASK_GLOW_SHELL + idWorldPlayer ) )
			//	return;

			p_data_b[idWorldPlayer][PB_ISFROZENPLAYER] = true;

			static iSkillLevel;
			iSkillLevel = SM_GetSkillLevel(idAttacker, SKILL_FROST_MAGE_ICE_ARROW );
			set_task(p_ice_arrow[iSkillLevel-1], "fRemoveFreeze", TASK_REMOVE_FROZEN + idWorldPlayer );

		}
		//Если стрела попадает в игрока и он из твоей команды
		else 
		{
			fCreateExplodeIceArrow(entIceArrow)
		}
	} 
	//Если игрок запустил стрелу и вышел с сервера
	else
	{
		fCreateExplodeIceArrow(entIceArrow);
	}
}

//Удаление заморозки
public fRemoveFreeze( idUser )
{
	if ( idUser >= TASK_REMOVE_FROZEN )
		idUser -= TASK_REMOVE_FROZEN;

	if ( !p_data_b[idUser][PB_ISCONNECTED] )
		return;

	p_data_b[idUser][PB_ISFROZENPLAYER] = false;

//	client_print(0,print_chat,"fRemoveFreeze");

	SHARED_SetGravity( idUser );
	
	SHARED_SetSpeed( idUser );

	fm_set_rendering(idUser);

	// Gradually remove screen's blue tint
	message_begin(MSG_ONE, get_user_msgid("ScreenFade"), _, idUser);
	write_short(UNIT_SECOND);// duration
	write_short(0);// hold time
	write_short(FFADE_IN);// fade type
	write_byte(0);// red
	write_byte(50);// green
	write_byte(200);// blue
	write_byte(100);// alpha
	message_end();
	
	static origin2[3];
	get_user_origin(idUser, origin2);
	
	// Glass shatter
	message_begin(MSG_PVS, SVC_TEMPENTITY, origin2);
	write_byte(TE_BREAKMODEL);
	write_coord(origin2[0]); // x
	write_coord(origin2[1]); // y
	write_coord(origin2[2]+24); // z
	write_coord(16);// size x
	write_coord(16);// size y
	write_coord(16);// size z
	write_coord(random_num(-50, 50));// velocity x
	write_coord(random_num(-50, 50));// velocity y
	write_coord(25);// velocity z
	write_byte(10);// random velocity
	write_short(mdlGlassSpr);// model
	write_byte(10);// count
	write_byte(25);// life
	write_byte(BREAK_GLASS); // flags
	message_end();

	return;
}

//Скил 3 уровня - Конус холода (Cold cone)
FM_SkillsOffensive_Cone_Cold(idAttacker, idVictim)
{
	if ( !p_data_b[idVictim][PB_ISCONNECTED] || !p_data_b[idAttacker][PB_ISCONNECTED] )
		return;

	static iSkillLevel;
	iSkillLevel = SM_GetSkillLevel( idAttacker, SKILL_FROST_MAGE_CONE_OF_COLD );
	if ( iSkillLevel > 0 && p_data_b[idAttacker][PB_ISFROZENSKILL] == false
		&& ((get_user_team( idAttacker ) != get_user_team( idVictim ))))
	{
		if ( random_float(MIN_PERCENT_CONE_COLD, MAX_PERCENT_CONE_COLD ) <= p_cone_cold[iSkillLevel-1] )
		{
			if ( task_exists( TASK_FROZEN_SKILL + idVictim ) == 0 )
			{
				//client_print(0,print_chat,"SKILL_FROST_MAGE_CONE_OF_COLD");

				new vVictimOrigin[3],vAxisOrigin[3];
				get_user_origin( idVictim, vVictimOrigin );

				vAxisOrigin[2] += vVictimOrigin[2] + 100;
				Create_TE_BEAMCYLINDER( vVictimOrigin, vVictimOrigin, vAxisOrigin, g_iSprites[SPR_SHOCKWAVE], 0, 0, 4, 60, 0, 41, 138, 255, 200, 0 );

				p_data_b[idVictim][PB_ISFROZENSKILL] = true;

				set_task(p_cone_cold[iSkillLevel-1], "fRemoveFrozenSkill", TASK_FROZEN_SKILL + idVictim );

				emit_sound(idAttacker, CHAN_STATIC, g_szSounds[SOUND_COLD_CONE_S], 1.0, ATTN_NORM, 0, PITCH_NORM );
				emit_sound(idVictim, CHAN_STATIC, g_szSounds[SOUND_COLD_CONE_S], 1.0, ATTN_NORM, 0, PITCH_NORM );
			}
						
		}
		
	}
}

//Удаление заморозки скилов
public fRemoveFrozenSkill( idUser )
{
	if ( idUser >= TASK_FROZEN_SKILL )
		idUser -= TASK_FROZEN_SKILL;

	if ( !p_data_b[idUser][PB_ISCONNECTED] )
		return;

	//client_print(0,print_chat,"fRemoveFrozenSkill");

	p_data_b[idUser][PB_ISFROZENSKILL] = false;

	emit_sound(idUser, CHAN_STATIC, g_szSounds[SOUND_COLD_CONE_E], 1.0, ATTN_NORM, 0, PITCH_NORM );

	return;
}