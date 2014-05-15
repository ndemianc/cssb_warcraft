


public sm4_4_9_Tranquility( idUser,iItem )//добавлена в items.inl
{
	new param[2];
	param[0] = idUser;

	fCastSpell( idUser, "_SPELL_Tranquility", param, sizeof(param) );

	sm4_ITEM_DisplayMessage( idUser, iItem );

	arrIsByuItemOneRound[idUser][ITEM_TRANQUILITY]++;

}

//-----------------HAM Forward - Ham_Player_PreThink-----------------------BEGIN {
public fwHamPlayerPreThink( idUser )
{
	new Float:fVelocity, Float:vel[3];

	if( !is_user_connected(idUser) )
		return HAM_IGNORED;
		
	if( !is_user_alive(idUser) )
		return HAM_IGNORED;

	pev( idUser, pev_velocity, vel );
	fVelocity = vector_length( vel );

	arrPlayerInfo[idUser][bIsOnGround] = bool:(pev(idUser, pev_flags) & FL_ONGROUND);
	arrPlayerInfo[idUser][fCurrentSpeed] = _:fVelocity;

	if( fVelocity > MAX_CASTING_VELOCITY && arrPlayerInfo[idUser][bCastingSpell] )
		arrPlayerInfo[idUser][bCastingInterrupted] = true;

	return HAM_IGNORED;
}

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------HAM Forward - Ham_Player_PreThink-----------------------END }


//-----------------Предмет "Спокойствие"-----------------------BEGIN {
// Запускается из fCastSpell
public _SPELL_Tranquility( const param[] )
{
	static param2[6], vOrigin[3];
	new idUser = param[0];

	if( !is_user_alive(idUser))
		return;
	
	get_user_origin( idUser, vOrigin );

	param2[0] = vOrigin[0];
	param2[1] = vOrigin[1];
	param2[2] = vOrigin[2];
	param2[3] = idUser;
	param2[4] = g_iPlayerTeam[idUser];
	
	_SPELL_Tranquility_Task( param2 );
}

//Повторяется отрисовка эффекта
public _SPELL_Tranquility_Task( param[6] )
{
	static Float:fCounter[33], Float:fScalar;
	static idUser, vOrigin[3];
	static vPosition[3], vTargetOrigin[3], iDistance, iTargetID, iTeamPlayer;
	new r,g,b;
	new const Float:fStepping = 100.0 / (TRANQUILITY_TIME / TRANQUILITY_FREQ);

	vOrigin[0] 	= param[0];
	vOrigin[1] 	= param[1];
	vOrigin[2] 	= param[2];
	idUser 		= param[3];
	iTeamPlayer = param[4];
	
	//client_print(idUser,print_chat,"_SPELL_Tranquility_Task");

	if( !task_exists( TASK_TRANQUILITY + idUser ) )
	{
		fCounter[idUser] = 100.0;

		emit_sound( idUser, CHAN_STATIC, g_szSounds[SOUND_TRANQUILITY], VOL_NORM, ATTN_NORM, 0, PITCH_NORM );

		if(iTeamPlayer == TEAM_T)
		{
			r = 205;
			g = 92;
			b = 92;
		}
		if(iTeamPlayer == TEAM_CT)
		{
			r = 30;
			g = 144;
			b = 255;
		}

		new radius = floatround( 65.0 / 5.0 * 5 );
		new life = floatround( 170.0 / 5.0 * 5 );
		
		Create_TE_DLIGHT( vOrigin, radius, r, g, b, life, 4 );

		set_task( TRANQUILITY_FREQ, "_SPELL_Tranquility_Task", TASK_TRANQUILITY + idUser, param, sizeof(param), "a", \
					floatround((TRANQUILITY_TIME-TRANQUILITY_FREQ)/TRANQUILITY_FREQ) );
	}

	for(new iE = 0 ; iE < floatround(1+fCounter[idUser]/12.0) ; iE++ )
	{
		vPosition[0] = vOrigin[0] - random_num( -450, 450 );
		vPosition[1] = vOrigin[1] - random_num( -450, 450 );
		vPosition[2] = vOrigin[2] - random_num( -100, 100 );
		Create_TE_IMPLOSION(vPosition, 100, 5, random_num( 2, 20 ) );
	}

	for(new iC = 0 ; iC < floatround(1+fCounter[idUser]/12.0) ; iC++ )
	{
		vPosition[0] = vOrigin[0] - random_num( -450, 450 );
		vPosition[1] = vOrigin[1] - random_num( -450, 450 );
		vPosition[2] = vOrigin[2] - random_num( -100, 100 );
		Create_TE_EXPLOSION( vPosition, vPosition, g_iSprites[SPR_SHADOWSTRIKE], random_num( 1, 4 ), 24, 4 );
	}

	vPosition[0] = vOrigin[0];
	vPosition[1] = vOrigin[1];
	vPosition[2] = vOrigin[2] + TRANQUILITY_RANGE;
	fScalar =  3.5;
	Create_TE_BEAMCYLINDER( vOrigin, vOrigin, vPosition, g_iSprites[SPR_SHOCKWAVE], 0, 0, 10, 55, 100, 188, 220, 255, 1 + floatround(fCounter[idUser]/fScalar), 0 );

	fCounter[idUser] -= fStepping;
	
	if( fCounter[idUser] < 0.0 )
		fCounter[idUser] = 0.0;


	new arrPlayers[32], iNumPlayers;
	new iTeam = get_user_team( idUser );
	
	get_players( arrPlayers, iNumPlayers, "a" );

	for( new i = 0 ; i < iNumPlayers; i++ )
	{
		iTargetID = arrPlayers[i];

		if ( p_data_b[iTargetID][PB_ISCONNECTED] && get_user_team( iTargetID ) == iTeam )
		{
			if( is_user_alive(iTargetID) )
			{
				get_user_origin( iTargetID, vTargetOrigin );
				iDistance = get_distance( vOrigin, vTargetOrigin );
		
				if( iDistance <= TRANQUILITY_RANGE && !p_data_b[iTargetID][PB_HEXED])
				{
					if(sm4_IsPlayerSkeleton(iTargetID) == false)//Если игрок скелет,лечиться не может
						fHeal( iTargetID, TRANQUILITY_HP );
				}
			}
		}
	}

}
//=============================================================================
//=============================================================================
//=============================================================================

//-----------------Предмет "Спокойствие"-----------------------END }


//-----------------Вспомогательные функции-----------------------BEGIN {
bool:fCastSpell( idUser, const szTaskString[]="", const sParam[]="", const sParamSize=0, const Flags=0 )
{
	if( arrPlayerInfo[idUser][bCastingSpell])
		return false;

	if(!(Flags & CAST_IGNORE_MOVEMENT) )
	{
		if( arrPlayerInfo[idUser][fCurrentSpeed] > MAX_CASTING_VELOCITY )
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_NO_MOVING_TRANQUILITY");
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);

			new iItemMoney = ITEM_Cost( idUser, ITEM_TRANQUILITY );
			SHARED_SetUserMoney(idUser,SHARED_GetUserMoney( idUser ) + iItemMoney, 0 );

			return false;
		}
	}
	
	new param[128];
	
	param[0] = idUser;
	param[1] = Flags;
	copy( param[2], 31, szTaskString );
	param[38] = sParamSize;
	for( new i = 0 ; i < sParamSize ; i++ )
		param[i+39] = sParam[i];
		
	if( !(Flags & CAST_IGNORE_MOVEMENT) )
	{
		if( arrPlayerInfo[idUser][fCurrentSpeed] > MAX_CASTING_VELOCITY )
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_NO_MOVING_TRANQUILITY");
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);

			return false;
		}
	}

	arrPlayerInfo[idUser][bCastingSpell] = true;
	arrPlayerInfo[idUser][bCastingInterrupted] = false;

	Create_BarTime( idUser, floatround(1.0), 0 );
	SHARED_Glow( idUser, 0, 50, 255,0);
		
	new param2[2];
	param2[0] = idUser;
	param2[1] = Flags;
	set_task( 0.1, "_PLAYER_CheckCasting", TASK_CASTINGINTERRUPTCHECK + idUser, param2, sizeof(param2), "b" );

	emit_sound( idUser, CHAN_VOICE, g_szSounds[SOUND_PRECAST_HOLY], VOL_NORM, ATTN_NORM, 0, PITCH_NORM );
	set_task( 1.0, "_PLAYER_EndCasting", TASK_CASTINGSPELL + idUser, param, sizeof(param) );
	
	return true;
}

public _PLAYER_CheckCasting( const param[] )
{
	new idUser, Flags;
	new bool:bRemoveTasks = false;
	
	idUser = param[0];
	Flags = param[1];
	
	if( !is_user_alive(idUser) )
		bRemoveTasks = true;
	
	else if( !arrPlayerInfo[idUser][bCastingSpell] )
		bRemoveTasks = true;

	// Check to see if the cast was interrupted
	else if( arrPlayerInfo[idUser][bCastingInterrupted] && !(Flags & CAST_IGNORE_INTERRUPT) )
	{
		bRemoveTasks = true;

		// Remove the castbar
		Create_BarTime( idUser, 0, 0 );

		format(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_DEACTIVATION_TRANQUILITY");
		WC3_StatusText( idUser,TXT_BLINK_CENTER,szMessage);

		new iItemMoney = ITEM_Cost( idUser, ITEM_TRANQUILITY );
		SHARED_SetUserMoney(idUser,SHARED_GetUserMoney( idUser ) + iItemMoney, 0 );

		emit_sound( idUser, CHAN_VOICE, g_szSounds[SOUND_ERROR], 0.0, ATTN_NORM, 0, PITCH_NORM );
	}

	if( bRemoveTasks )
		sm4_4_9_fRemoveSpellCasting( idUser );
}

public _PLAYER_EndCasting( const param[] )
{
	new idUser, Flags, szTaskString[32];
	new sParam[64], sParamSize;

	idUser = param[0];
	Flags = param[1];
	copy( szTaskString, 31, param[2] );
	sParamSize = param[38];
	for( new i = 0 ; i < sParamSize ; i++ )
		sParam[i] = param[i+39];

	sm4_4_9_fRemoveSpellCasting( idUser );

	if( !(Flags & CAST_SKIP_ENDCAST_SOUND) )
		emit_sound( idUser, CHAN_VOICE, g_szSounds[SOUND_CAST_HOLY], VOL_NORM, ATTN_NORM, 0, PITCH_NORM );

	set_task( 0.0, szTaskString, _, sParam, sParamSize );
}



//Остановка работы предмета Спокойствие
//добавлена в war3ft.inl (в двух местах)
//добавлена в events.inl (в функцию EVENT_NewRound)
sm4_4_9_fRemoveSpellCasting( idUser )
{
	arrPlayerInfo[idUser][bCastingSpell] = false;
	arrPlayerInfo[idUser][bCastingInterrupted] = false;
	
	remove_task( TASK_CASTINGSPELL + idUser );
	remove_task( TASK_CASTINGINTERRUPTCHECK + idUser );
}

fHeal( idUser, iHealth)
{
	new vOrigin[3], Float:fCurrentHealth;

	if( !is_user_alive( idUser ) )
		return 0;

	pev( idUser, pev_health, fCurrentHealth );

	new Float:fMaxHealth = float( get_user_maxhealth( idUser ) );
	new Float:fHealth = float( iHealth );

	if( fCurrentHealth < fMaxHealth )
	{
		if( fCurrentHealth+fHealth > fMaxHealth )
		{
			set_pev( idUser, pev_health, fMaxHealth );
			iHealth = floatround( fMaxHealth - fCurrentHealth );
		}
		else
			set_pev( idUser, pev_health, fCurrentHealth+fHealth );
		
		get_user_origin( idUser, vOrigin );
		
		Create_TE_IMPLOSION(vOrigin, 100, 8,1 );

		return iHealth;
	}
	return 0;
}



//=============================================================================
//=============================================================================
//=============================================================================

//-----------------Вспомогательные функции-----------------------END }