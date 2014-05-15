/*
*	Counter-Strike and Condition Zero only functions
*/

public on_EndRound()
{

	if ( !WC3_Check() )
	{
		return;
	}

	g_EndRound = true;


	// Save XP at the end of the round?
	if ( get_pcvar_num( CVAR_wc3_save_end_round ) )
	{
		// Threaded saves on end round!
		DB_SaveAll( true );
	}
	
	//Отображение победителя в конце раунда
//	fShowWinnPlayer();

	return;
}

// Called when freezetime is over
public on_FreezeTimeComplete()
{

	if ( !WC3_Check() )
	{
		return;
	}

	g_freezeTime = false;
	/*
	new iPlayers[32], iNumPlayers, i;
	get_players( iPlayers, iNumPlayers, "a" );
	
	// Loop through all players
	for ( i = 0; i < iNumPlayers; i++ )
	{
		
		// Show them their race/xp/item bar
		WC3_ShowBar( iPlayers[i] );

		// Award speed bonuses (if any)
		set_task( 0.1, "SHARED_SetSpeed", TASK_UNHOLYSPEED + iPlayers[i] );
	}*/

	return;
}

public on_TerroristWin()
{

	if ( !WC3_Check() )
	{
		return;
	}

	XP_WinRound( TEAM_T );

	//Отображение победителя в конце раунда
	fShowWinnPlayer(TEAM_T);

	return;
}

public on_CTWin()
{
	if ( !WC3_Check() )
	{
		return;
	}

	XP_WinRound( TEAM_CT );

	//Отображение победителя в конце раунда
	fShowWinnPlayer(TEAM_CT);

	return;
}


public _CS_MinModelsLoop()
{
	if ( !WC3_Check() )
	{
		return;
	}

	if ( get_pcvar_num( CVAR_wc3_query_client ) )
	{
		new iPlayers[32], iNumPlayers, i;
		get_players( iPlayers, iNumPlayers, "c" );
		
		for ( i = 0; i < iNumPlayers; i++ )
		{
			query_client_cvar( iPlayers[i], "cl_minmodels", "_CS_CheckMinModelsValue" );
		}
	}
}

public _CS_CheckMinModelsValue( idUser, const cvar[], const value[] )
{
	if ( equali( cvar,"cl_minmodels" ) && strcmp( value, "0" ) )
	{
		client_cmd( idUser, "echo ^"======== Warcraft 3 Frozen Throne ========^"" );
		format(szMessage, sizeof(szMessage) - 1, "echo ^"%L^"",LANG_PLAYER,"CLIENT_KICK_CL_MODELS_1");
		client_cmd( idUser, szMessage);
		format(szMessage, sizeof(szMessage) - 1, "echo ^"%L^"",LANG_PLAYER,"CLIENT_KICK_CL_MODELS_2");
		client_cmd( idUser, szMessage);
		client_cmd( idUser, "echo ^"=========================================^"" );
		format(szMessage, sizeof(szMessage) - 1, "kick #%d ^"%L^"",get_user_userid( idUser ),LANG_PLAYER,"CLIENT_KICK_CL_MODELS_3");	
		server_cmd( szMessage );
	} 
}

// Function called when a user's armor changes
public on_Battery( idUser )
{	
	if ( !WC3_Check() )
	{
		return;
	}
	
	if ( !bIgnoreArmorSet[idUser] )
	{
		// Store the previous value (we do this b/c the Battery event is triggered (and set to 0) before weapon reincarnation)
		p_data[idUser][P_LASTARMOR] = p_data[idUser][P_ARMORONDEATH];

		p_data[idUser][P_ARMORONDEATH] = read_data( 1 );
	}

	return;
}

public on_ArmorType(idUser)
{

	if ( !WC3_Check() )
	{
		return;
	}
	
	// Save user's armor
	p_data[idUser][P_ARMORONDEATH] = cs_get_user_armor( idUser, g_ArmorType[idUser] );

	return;
}

// Event triggered when you look at another player
public on_ShowStatus( idUser )
{

	if ( !WC3_Check() )
	{
		return;
	}

	// Lets not show any player info during freezetime! - don't want them to find moles (unless they have mole protectant) :P
/*	if ( g_freezeTime && ITEM_Has( idUser, ITEM_PROTECTANT ) == ITEM_NONE )
	{
		Create_StatusText( idUser, 0, "" );

		return;
	}*/

	new idTarget = read_data( 2 );
	
	if(is_user_alive(idUser) && is_user_connected(idUser) &&
		is_user_alive(idTarget) && is_user_connected(idTarget))
		{

			new iTargetTeam = get_user_team( idTarget );
			new iViewerTeam = get_user_team( idUser );

			// Same team check
			if ( iViewerTeam == iTargetTeam )
			{
				// Check if your teammate looks like the enemy!
				if ( p_data_b[idTarget][PB_SKINSWITCHED] )
				{
					//client_print( idUser, print_center, "%L", idUser, "HES_ON_YOUR_TEAM_DONT_SHOOT" );

					new szMessageDontShot[256];
					format(szMessageDontShot, charsmax(szMessageDontShot),
						"%L",LANG_PLAYER, "HES_ON_YOUR_TEAM_DONT_SHOOT");

//					set_hudmessage(0,255,0,-1.0,0.60, 0, 0.01, 3.0, 0.01, 0.01, HUD_SHOWSTATUS );
					set_dhudmessage(79, 79, 79 ,-1.0,0.30,0, 0.01, 1.0, 0.01, 0.01, false);
					show_dhudmessage(idUser, szMessageDontShot);
					
					client_cmd( idUser, "speak %s", g_szSounds[SOUND_ANTEND] );
				}
			}

		// Отображение информации на экране при наведении прицелом
	//	if ( !g_freezeTime && get_pcvar_num( CVAR_wc3_show_player ) )
		if ( get_pcvar_num( CVAR_wc3_show_player ) )
		{
				new szTargetName[32];
				get_user_name(idTarget, szTargetName,charsmax(szTargetName));
							
				if(!SM_IsValidRace( p_data[idTarget][P_RACE] ))
					return;
				
				new szShortRaceNameTarget[32];
				lang_GetRaceName(p_data[idTarget][P_RACE], idUser, szShortRaceNameTarget,charsmax(szShortRaceNameTarget),true);
			
				new szMessageStatus[256];

				new iColorRGB_TEAM_T[3];
				iColorRGB_TEAM_T[0] = 255;
				iColorRGB_TEAM_T[1] = 69;
				iColorRGB_TEAM_T[2] = 0;

				new iColorRGB_TEAM_CT[3];
				iColorRGB_TEAM_CT[0] = 58;
				iColorRGB_TEAM_CT[1] = 95;
				iColorRGB_TEAM_CT[2] = 205;
				
				if(iViewerTeam == TEAM_T)//Если наводящий курсор из команды террористов
				{
					if(iTargetTeam == TEAM_T)
					{
						format(szMessageStatus, charsmax(szMessageStatus), 
						"%s ^n%L %d %L %d ^n%L %s ^n%L %d",
						szTargetName,
						LANG_PLAYER, "WC3_HEALTH",
						get_user_health(idTarget),
						LANG_PLAYER, "WC3_ARMOR",
						get_user_armor(idTarget),
						LANG_PLAYER, "WC3_RACE",
						szShortRaceNameTarget,				
						LANG_PLAYER, "WORD_LEVEL",
						p_data[idTarget][P_LEVEL]);
					
						set_hudmessage(iColorRGB_TEAM_T[0],iColorRGB_TEAM_T[1],iColorRGB_TEAM_T[2], -1.0, 0.60, 0, 0.01, 3.0, 0.01, 0.01, HUD_SHOWSTATUS );
						show_hudmessage(idUser,szMessageStatus);
					}
					if(iTargetTeam == TEAM_CT)
					{
						format(szMessageStatus, charsmax(szMessageStatus), 
						"%s  ^n%L %s ^n%L %d",
						szTargetName,
						LANG_PLAYER, "WC3_RACE",
						szShortRaceNameTarget,				
						LANG_PLAYER, "WORD_LEVEL",
						p_data[idTarget][P_LEVEL]);
						
						set_hudmessage(iColorRGB_TEAM_CT[0],iColorRGB_TEAM_CT[1],iColorRGB_TEAM_CT[2], -1.0, 0.60, 0, 0.01, 3.0, 0.01, 0.01, HUD_SHOWSTATUS );
						show_hudmessage(idUser,szMessageStatus);
					}
				}

				if(iViewerTeam == TEAM_CT)//Если наводящий курсор из команды коунтер-террористов
				{
					if(iTargetTeam == TEAM_T)
					{
						format(szMessageStatus, charsmax(szMessageStatus), 
						"%s  ^n%L %s ^n%L %d",
						szTargetName,
						LANG_PLAYER, "WC3_RACE",
						szShortRaceNameTarget,				
						LANG_PLAYER, "WORD_LEVEL",
						p_data[idTarget][P_LEVEL]);
						
						set_hudmessage(iColorRGB_TEAM_T[0],iColorRGB_TEAM_T[1],iColorRGB_TEAM_T[2], -1.0, 0.60, 0, 0.01, 3.0, 0.01, 0.01, HUD_SHOWSTATUS );
						show_hudmessage(idUser,szMessageStatus);
					}
					if(iTargetTeam == TEAM_CT)
					{
					
						format(szMessageStatus, charsmax(szMessageStatus), 
						"%s ^n%L %d %L %d ^n%L %s ^n%L %d",
						szTargetName,
						LANG_PLAYER, "WC3_HEALTH",
						get_user_health(idTarget),
						LANG_PLAYER, "WC3_ARMOR",
						get_user_armor(idTarget),
						LANG_PLAYER, "WC3_RACE",
						szShortRaceNameTarget,				
						LANG_PLAYER, "WORD_LEVEL",
						p_data[idTarget][P_LEVEL]);
					
						set_hudmessage(iColorRGB_TEAM_CT[0],iColorRGB_TEAM_CT[1],iColorRGB_TEAM_CT[2], -1.0, 0.60, 0, 0.01, 3.0, 0.01, 0.01, HUD_SHOWSTATUS );
						show_hudmessage(idUser,szMessageStatus);
					}
				}


/*
		new iRed = 0, iBlue = 0, bool:bShowAsTeammate = false;
				
		// The target looks like the enemy o.O
		//Если цель на которую наводишь прицел переоделась в скин твоей команды
		if ( p_data_b[idTarget][PB_SKINSWITCHED] )
		{

			// Then the viewer should see the user's true color!
			//Если куплен предмет Mole
			if ( ITEM_Has( idUser, ITEM_PROTECTANT ) > ITEM_NONE )
			{
				( iTargetTeam == TEAM_T )	? ( iRed = 255 ) : 0;
				( iTargetTeam == TEAM_CT )	? ( iBlue = 255 ) : 0;
			}

			// Lets "camouflage" them
			else
			{
				( iTargetTeam == TEAM_T )	? ( iBlue = 255 ) : 0;
				( iTargetTeam == TEAM_CT )	? ( iRed = 255 ) : 0;

				bShowAsTeammate = true;
			}
		}
		
		// Lets just set the correct colors
		//Если цель на которую наводишь прицел имеет такой же скин твоей команды
		else
		{
			( iTargetTeam == TEAM_T )	? ( iRed = 255 ) : 0;
			( iTargetTeam == TEAM_CT )	? ( iBlue = 255 ) : 0;
		}
		
		// Set up the hud message
		set_hudmessage(iRed, 50, iBlue, -1.0, 0.60, 1, 0.01, 3.0, 0.01, 0.01, HUD_SHOWSTATUS );

		// Teammate or should look like teammate
		if ( iViewerTeam == iTargetTeam || bShowAsTeammate )
		{
			new iClip, iAmmo;
			new iWeaponID = get_user_weapon( iTarget, iClip, iAmmo );
			new szWeaponName[32];

			// Only get the weapon name if our weapon idUser is valid
			if ( iWeaponID > 0 )
			{
				get_weaponname( iWeaponID, szWeaponName, 31 );
			}
			
			show_hudmessage( idUser, "%s -- %d HP / %d AP%s%s", szTargetName, get_user_health( iTarget ), get_user_armor( iTarget ), ( ( iWeaponID > 0 ) ? " / " : "" ), szWeaponName );
			
	
		} 

		// Enemy
		else
		{
			show_hudmessage( idUser, szTargetName );
		}*/

		

		}//if ( get_pcvar_num( CVAR_wc3_show_player ) )

	 }//if(is_user_alive(idUser) && is_user_connected(idUser)

	return;
}

// This is called when the user is no longer viewing the player
public on_HideStatus( idUser )
{
	if ( !WC3_Check() )
	{
		return;
	}

	if ( get_pcvar_num( CVAR_wc3_show_player ) && !g_freezeTime )
	{
		UTIL_ClearHudChannel( idUser, HUD_SHOWSTATUS );
	}

	ClearSyncHud(idUser, msgSyncObj);
}

// Function is called when buytime is over
public _CS_BuyTimeOver()
{
	g_buyTime = false;
}

public on_WeapPickup( idUser )
{ 

	if ( !WC3_Check() )
	{
		return;
	}

	SHARED_SaveWeapons( idUser );

	return;
}

public on_GameRestart()
{

	if ( !WC3_Check() )
	{
		return;
	}
	
	// Save all XP on game restart - do not do this threaded or it might take 300 years
	DB_SaveAll( false );

	g_GameRestarting = true;

	sm4_4_2_evRoundRestart();

	return;
}

// From war3x
public on_Zoom( idUser )
{

	if ( !WC3_Check() )
	{
		return;
	}

	if ( read_data( 1 ) < 90 )
	{
		g_bPlayerZoomed[idUser] = true;
	}

	else
	{
		g_bPlayerZoomed[idUser] = false;
	}

	// We will need to change the user's speed on zooming
	SHARED_SetSpeed( idUser );

	return;
}

public on_BombPlanting( idUser )
{
	p_data_b[idUser][PB_ISPLANTING] = true;
}

public on_BombStopPlanting( idUser )
{
	p_data_b[idUser][PB_ISPLANTING] = false;
}

public CZ_BotHookHam( idUser )
{
    // Thx to Avalanche and GunGame of which this method is based off of
    if ( g_bCZBotRegisterHam || !is_user_connected(idUser) || g_MOD != GAME_CZERO )
		return;


    // Recheck for safety.
    if ( (pev(idUser, pev_flags) & FL_FAKECLIENT) && get_pcvar_num(CVAR_bot_quota) > 0 )
	{
        // Post spawn fix for cz bots, since RegisterHam does not work for them
        RegisterHamFromEntity( Ham_TakeDamage, idUser, "EVENT_TakeDamage", 1 );
		RegisterHamFromEntity( Ham_Spawn, idUser, "EVENT_Spawn", 1);

        // Only needs to run once after ham is registed ignore.
        g_bCZBotRegisterHam = true;
    }
}