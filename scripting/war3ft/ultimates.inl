/*
*	Common Ultimate Functions
*/

// This is ran every second...
public _ULT_Delay()
{
	
	//Запуск счетчика отсчета на удаление предмета 
	fRunCalcRemoveLootItem();

	// Decrement the global counter
	g_iUltimateDelay--;
	
	// Now we need to loop through all players and decrement their ultimate delay
	new players[32], numplayers, i, idUser;
	get_players( players, numplayers );

	for ( i = 0; i < numplayers; i++ )
	{
		idUser = players[i];

		// Reduce the user's ultimate delay

		//Для talisman +
		if ( ITEM_Has( idUser, ITEM_ULTIMATE_TIMER_DIVIDER ) > ITEM_NONE || fWorkTalismanPassive(idUser, arrTalismanActive[idUser]) == TALISMAN_ITEM_ULTIMATE_TIMER_DIVIDER)
		{
				if(p_data[idUser][P_ULTIMATEDELAY] >= RESET_ULTIMATE_TIMER)
					p_data[idUser][P_ULTIMATEDELAY] -= RESET_ULTIMATE_TIMER;
				else
					p_data[idUser][P_ULTIMATEDELAY]--;
		}	
		else
		{
			p_data[idUser][P_ULTIMATEDELAY]--;
		}

		// Then the user's ultimate is ready
		if ( p_data[idUser][P_ULTIMATEDELAY] == 0 )
		{
			ULT_IconHandler( idUser );
		}
/*
		//Дает XP 50000 в 18:00:00
		new szName[32];
		get_user_name(idUser, szName, 31 );

		new szCurrentTime[64];
		get_time("%H:%M:%S", szCurrentTime,63 );

//		client_print(idUser,print_chat,"Time is: %s",szCurrentTime)

		if(equal(szCurrentTime,"18:00:00"))
			ADMIN_GiveXP( idUser,szName, 50000 );

*/
	}



}

ULT_ResetCooldown( idUser, iTime, iHideIcon = true )
{
	p_data[idUser][P_ULTIMATEDELAY]	= iTime;


	// Hide the user's ultimate icon
	if ( iHideIcon )
	{
		ULT_Icon( idUser, ICON_HIDE );
	}
}

// This function will handle ultimate icons - basically shows if appropriate or hides if appropriate
ULT_IconHandler( idUser )
{
	// WC3 isn't running... shux
	if ( !WC3_Check() )
	{
		return;
	}
	
	new bool:bShowIcon = true;

	// User has no ultimate!
	new iUltimateID = SM_GetSkillOfType( idUser, SKILL_TYPE_ULTIMATE );
	if ( iUltimateID > -1 && SM_GetSkillLevel( idUser, iUltimateID, 6 ) <= 0 )
	{
		bShowIcon = false;
	}

	/* In global delay - can't show icon!
	else if ( g_iUltimateDelay > 0)
	{
		bShowIcon = false;
	}*/
	
	// User's delay is longer!
	else if ( p_data[idUser][P_ULTIMATEDELAY] > 0 )
	{
		bShowIcon = false;
	}

	// User is dead - don't show icon!
	else if ( !is_user_alive( idUser ) )
	{
		bShowIcon = false;
	}

	// User isn't connected! - don't show icon!
	else if ( !p_data_b[idUser][PB_ISCONNECTED] )
	{
		bShowIcon = false;
	}


	// We need to hide the user's icon!
	if ( !bShowIcon )
	{
		// In theory I could remember what was displayed when - but easy way out is this
		ULT_ClearIcons( idUser );
	}

	// Show the user's icon!
	else
	{
		// Play the ultimate ready sound
		client_cmd( idUser, "speak %s", g_szSounds[SOUND_ULTIMATEREADY] )
		
		// Give the user a graphical message that their ultimate is ready
		WC3_StatusText( idUser, TXT_ULTIMATE, "%L", LANG_PLAYER, "ULTIMATE_READY" );
		
		// Show their ultimate icon
		ULT_Icon( idUser, ICON_SHOW );
	}
}

// This function will display/flash/hide the race's ultimate icon on the screen
ULT_Icon( idUser, flag )
{
	
	// If they mysteriously lost a level - we shouldn't display the icon (i.e. admin command)
	if ( p_data[idUser][P_LEVEL] < MIN_ULT_LEVEL && flag != ICON_HIDE )
	{
		flag = ICON_HIDE;
	}

	new iRaceID = p_data[idUser][P_RACE];
	
	// If we're chameleon we need to display the icon of the ultimate we have
	if ( p_data[idUser][P_RACE] == RACE_CHAMELEON )
	{
		//new iChamUltimate = g_ChamSkills[3];
		//iRaceID = g_SkillOwner[iChamUltimate];
		iRaceID = 9;
	}
	
	//client_print(0,print_chat,"iRaceID: %d",iRaceID);

	
	// We should only do this if we have a valid race ID
	if ( iRaceID )
	{
		new r, g, b;

		// Each race has its on color for its icon...
		switch ( iRaceID )
		{
			case 1: r=255,	g=0,	b=0;		// Undead
			case 2: r=0,	g=120,	b=120;		// Human
			case 3: r=255,	g=255,	b=255;		// Orc
			case 4: r=0,	g=0,	b=255;		// Night Elf
			case 5: r=255,	g=0,	b=0;		// Blood Mage
			case 6: r=0,	g=200,	b=200;		// Shadow Hunter
			case 7: r=255,	g=0,	b=0;		// Warden
			case 8: r=0,	g=255,	b=0;		// Crypt Lord
			case 9: r=218,	g=165,	b=32;		// Chameleon
			case 10: r=30,	g=144,	b=255;		// Frost Mage
		}
			
		// Special circumstances should be done here
		switch ( flag )
		{
			case ICON_FLASH:
			{

				// Change colors for Suicide Bomber or Immolate
				if ( SM_GetSkillLevel( idUser, ULTIMATE_SUICIDE ) > 0 || SM_GetSkillLevel( idUser, ULTIMATE_IMMOLATE ) > 0 )
				{
					r=255, g=255, b=255;
				}
			}
		}

		// Create the status icon
		Create_StatusIcon( idUser, flag, g_UltimateIcons[iRaceID - 1], r, g, b );

		// Remember what icon we displayed
		if ( flag == ICON_SHOW || flag == ICON_FLASH )
		{
			g_ULT_iLastIconShown[idUser] = iRaceID;
		}

		// No icon shown
		else if ( flag == ICON_HIDE )
		{
			g_ULT_iLastIconShown[idUser] = 0;
		}
	}
}

ULT_ClearIcons( idUser )
{
	// Only have icons for CS/CZ
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Remove the last icon that was shown!
		if ( g_ULT_iLastIconShown[idUser] > 0 )
		{
			Create_StatusIcon( idUser, ICON_HIDE, g_UltimateIcons[g_ULT_iLastIconShown[idUser]-1], 0, 0, 0 );
		}
		
		/*
		// Loop through all possible icons and remove them
		for ( new i = 0; i < MAX_RACES - 1; i++ )
		{
			Create_StatusIcon( idUser, ICON_HIDE, g_UltimateIcons[i], 0, 0, 0 )
		}*/
	}
}

ULT_Available( idUser )
{

	// User needs ult + can't have it used + can't have a delay + can't have a global delay
	//if ( p_data[idUser][P_ULTIMATEDELAY] <= 0 && g_iUltimateDelay <= 0 )
	if ( p_data[idUser][P_ULTIMATEDELAY] <= 0 )
	{
		new iSkillID = SM_GetSkillOfType( idUser, SKILL_TYPE_ULTIMATE );
		new iSkillLevel = SM_GetSkillLevel( idUser, iSkillID, 7 );
		
		if ( iSkillLevel > 0 )
		{
			return true;
		}
	}

	return false;
}

// This will simply CHECK if a user can block an ultimate!
bool:ULT_CanUserBlockUlt( idUser )
{
	new bool:bCanBlock = false;

	if ( p_data_b[idUser][PB_WARDENBLINK] && !p_data_b[idUser][PB_HEXED] )
	{
		bCanBlock = true;
	}

	//Для talisman
	else if ( ITEM_Has( idUser, ITEM_NECKLACE ) > ITEM_NONE || 
		(fWorkTalismanPassive(idUser, arrTalismanActive[idUser]) == TALISMAN_ITEM_NECKLACE && arrTalismanNecklaceNum[idUser] > 0)
		)
	{
		bCanBlock = true;
	}

	return bCanBlock;
}

ULT_RemoveCharge(idUser)
{
	//Для talisman
	if ( ITEM_Has( idUser, ITEM_NECKLACE ) > ITEM_NONE || arrTalismanNecklaceNum[idUser] > 0)
	{
		ITEM_RemoveCharge( idUser, ITEM_NECKLACE );
	}
	else if ( p_data_b[idUser][PB_WARDENBLINK] )
	{}
	
	format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_ULT_REMOVE_CHARGE");	
			
	cssbColoredPrint(idUser, szMessage);

	// Play ultimate blocked sound
	emit_sound( idUser, CHAN_STATIC, g_szSounds[SOUND_SPELLSHIELD], 1.0, ATTN_NORM, 0, PITCH_NORM );
}

ULT_Blocked( idUser )
{
	// Maybe play a sound here instead?
	format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_ULT_BLOCKED");	
			
	cssbColoredPrint(idUser, szMessage);
	
	// Play ultimate blocked sound
	emit_sound( idUser, CHAN_STATIC, g_szSounds[SOUND_DISPELL], 1.0, ATTN_NORM, 0, PITCH_NORM );
}

public _ULT_Ping( parm[] )
{
	new idUser = parm[0];
	new iTimeLeft = parm[1];
	
	// Decrement our timer
	parm[1]--;
	
	// User died or diconnected
	if ( !p_data_b[idUser][PB_ISCONNECTED] || !is_user_alive( idUser ) )
	{
		p_data_b[idUser][PB_ISSEARCHING] = false;
	}
	
	// This is the last "playing" of the sound, no target was found :/
	if ( iTimeLeft == 0 )
	{
		p_data_b[idUser][PB_ISSEARCHING] = false;
		
		if ( ULT_Available( idUser ) )
		{
			ULT_Icon( idUser, ICON_SHOW );
		}
	}
	
	// Then we need to play the sound + flash their icon!
	if ( p_data_b[idUser][PB_ISSEARCHING] )
	{
		// Flash their ultimate icon
		ULT_Icon( idUser, ICON_FLASH );

		// Play the ping sound
		emit_sound( idUser, CHAN_STATIC, g_szSounds[SOUND_ULTIMATESCAN], 1.0, ATTN_NORM, 0, PITCH_NORM );

		set_task( 1.0, "_ULT_Ping", TASK_ULTPING + idUser, parm, 2 );
	}

	return;
}

// Function called before user's spawn
ULT_Reset( idUser )
{
	// Remove any explosions that are occuring, we don't want them carried over to the next round
	task_exists( TASK_EXPLOSION + idUser )		? remove_task( TASK_EXPLOSION + idUser ) : 0;
	task_exists( TASK_BEAMCYLINDER + idUser )	? remove_task( TASK_BEAMCYLINDER + idUser ) : 0;

	// Stop Crypt Lord's Ultimate
	task_exists( TASK_FUNNELS + idUser )		? remove_task( TASK_FUNNELS + idUser ) : 0;

	// Stop the ultimate cooldowns since they will reset at round start
	task_exists( TASK_UDELAY + idUser )			? remove_task( TASK_UDELAY + idUser ) : 0;
	
	// Reset Big Bad Voodoo
	if ( task_exists( TASK_RESETGOD + idUser ) )
	{
		remove_task( TASK_RESETGOD + idUser );

		SH_Ult_Remove( idUser );
	}
	
	// Set this to false to stop searching ultimates (used by NE + ORC + BM ultimates)
	p_data_b[idUser][PB_ISSEARCHING] = false;

	// Hide their ultimate icon
	ULT_Icon( idUser, ICON_HIDE );
}
