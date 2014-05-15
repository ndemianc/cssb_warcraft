/*
*	Admin Functions
*/

// Advanced Swear Filter and Punishment plugin uses this function
public ADMIN_ServerHandler()
{
	// Make sure WC3 is loaded
	if ( !WC3_Check() )
	{
		return PLUGIN_HANDLED;
	}

	new szCmd[32], szArg1[32], szArg2[32];
	read_argv( 0, szCmd, 31 );
	read_argv( 1, szArg1, 31 );
	read_argv( 2, szArg2, 31 );
	
	new idUser	= str_to_num( szArg1 );
	new iXP = str_to_num( szArg2 );

	if ( equal( szCmd, "amx_takexp" ) )
	{
		iXP *= -1;

		iXP += p_data[idUser][P_XP];
	}

	else if ( equal( szCmd, "changexp" ) )
	{
		iXP += p_data[idUser][P_XP];
		
	//	server_print("changexp ok");		
	}

	ADMIN_SetXP( idUser, iXP );

	return PLUGIN_HANDLED;
}

// This will handle every admin client command
public ADMIN_Handler( idUser )
{
	/*if(!(get_user_flags(idUser) & ADMIN_FLAG))
	{
		format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
		LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
		LANG_PLAYER,"YOU_HAVE_NO_ACCESS");		
		
		client_print( idUser, print_console,szMessage);	
		
		return PLUGIN_HANDLED;
	}*/
		
	new szCmd[32];
	read_argv( 0, szCmd, 31 );

	// Make sure WC3 is loaded
	if ( !WC3_Check() && !equal( szCmd, "wc3_disable" ) && !equal( szCmd, "wc3_enable" ) )
	{
		format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
		LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
		LANG_PLAYER,"CLIENT_PRINT_WC3_CHECK");
	
		ADMIN_Print(idUser,szMessage);
				
		return PLUGIN_HANDLED;
	}
	
	// Make sure the user is an admin
    if ( !( get_user_flags( idUser ) & XP_GetAdminFlag() ) )
	{
		format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
		LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
		LANG_PLAYER,"YOU_HAVE_NO_ACCESS");		
		
		client_print( idUser, print_console,szMessage);

		return PLUGIN_HANDLED;
	}

	// OK we're free to go!!!


	new szArg1[32], szArg2[32];
	read_argv( 1, szArg1, 31 );
	read_argv( 2, szArg2, 31 );

	// Give the user XP
	if ( equal( szCmd, "wc3_givexp" ) || equal( szCmd, "amx_givexp" ) )
	{
		if ( read_argc() < 3 )
		{
			ADMIN_Print(idUser, "Format: %s <name|#id|auth|@TEAM|@ALL> <xp>", szCmd );

			return PLUGIN_HANDLED;
		}

		ADMIN_GiveXP( idUser, szArg1, str_to_num( szArg2 ) );
	}

	// Set the level of a user's race
	else if ( equal( szCmd, "wc3_setlevel" ) )
	{
		if ( read_argc() < 3 )
		{
			ADMIN_Print( idUser, "Format: %s <name|#id|auth|@TEAM|@ALL> <level>", szCmd );

			return PLUGIN_HANDLED;
		}
		
		new iLevel = str_to_num( szArg2 );

		//CSSB
		if ( iLevel < 0 || iLevel >= (MAX_LEVELS+1) )
		{
			format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
			LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
			LANG_PLAYER,"CLIENT_PRINT_LVL_ERROR",
			MAX_LEVELS);	
		
			ADMIN_Print( idUser,szMessage);
			
			return PLUGIN_HANDLED;
		}

		ADMIN_SetLevel( idUser, szArg1, iLevel );
	}

	// Give the user an item
	else if ( equal( szCmd, "wc3_giveitem" ) )
	{
		if ( read_argc() < 3 )
		{
			ADMIN_Print( idUser, "Format: %s <name|#idUser|auth|@TEAM|@ALL> <item id or name>", szCmd );

			return PLUGIN_HANDLED;
		}

		new iItemID;
		if ( is_str_num( szArg2 ) )
		{
			iItemID = str_to_num( szArg2 );
			if ( iItemID > (MAX_SHOPMENU_ITEMS - 1) || iItemID < 0 )//Для shopmenu4
			{
				format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_ITEM_ERROR");
				
				ADMIN_Print( idUser, szMessage );

				return PLUGIN_HANDLED;
			}
		}

		ADMIN_GiveItem( idUser, szArg1, iItemID );
	}
	//Для talisman +
	//Дать игроку талисман
	else if ( equal( szCmd, "wc3_givetalisman" ) )
	{
		if ( read_argc() < 3 )
		{
			ADMIN_Print( idUser, "Format: %s <name|#idUser|auth|@TEAM|@ALL> <talisman ID>", szCmd );
			return PLUGIN_HANDLED;
		}

		new iTalismanID;
		if ( is_str_num( szArg2 ) )
		{
			iTalismanID = str_to_num( szArg2 );

			if(iTalismanID == 999)
			{
				ADMIN_GiveTalisman( idUser, szArg1, iTalismanID );
				return PLUGIN_HANDLED;
			}

			if(!bIsTalisman(iTalismanID))
			{
				formatex(szMessage, charsmax(szMessage), "%L",LANG_PLAYER,"CLIENT_PRINT_TALISMAN_ERROR");
				ADMIN_Print( idUser, szMessage );
				return PLUGIN_HANDLED;
			}
		}

		ADMIN_GiveTalisman( idUser, szArg1, iTalismanID );
	}
	// Enable the plugin
	else if ( equal( szCmd, "wc3_enable" ) )
	{

		// Plugin was already enabled, why re-enable?
		if ( warcraft3 )
		{
			format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_PLUGIN_ALREDY_EN");
			ADMIN_Print( idUser, szMessage );
		}

		// Enable the plugin!
		else
		{
			warcraft3 = true;
			
			format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_PLUGIN_EN");
			
			ADMIN_Print( idUser,szMessage);
		}

	}

	// Disable the plugin
	else if ( equal( szCmd, "wc3_disable" ) )
	{
		// Plugin was already enabled, why re-enable?
		if ( !warcraft3 )
		{
			format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_PLUGIN_ALREDY_DIS");
				
			ADMIN_Print( idUser, szMessage);
		}

		// Disable the plugin!
		else
		{
			// What needs to happen here?
			//    - all skills set to 0 for each player
			//    - call SHARED_SetGravity
			//    - call SHARED_SetInvis
			//    - call SHARED_SetSpeed
			//	  - ULT_ClearIcons( idUser )

			warcraft3 = false;
			
			format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_PLUGIN_DIS");			
			
			ADMIN_Print( idUser, szMessage);
		}
	}
	
	new szArgs[128];
	read_args( szArgs, 127 );

	ADMIN_Log( idUser, szCmd, "%s", szArgs );

	return PLUGIN_HANDLED;
}

// Actually set the user's XP
ADMIN_SetXP( idUser, iXP )
{
/*	if(!(get_user_flags(idUser) & ADMIN_FLAG))
	{
		format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
		LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
		LANG_PLAYER,"YOU_HAVE_NO_ACCESS");		
		
		client_print( idUser, print_console,szMessage);	
		
		return;
	}*/
		
	if ( iXP < 0 )
	{
		iXP = 0;
	}

	//server_print("ADMIN_SetXP ok");	
	
	p_data[idUser][P_XP] = iXP;

	XP_Check( idUser );

	set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + idUser );
}

// Set the user's level
ADMIN_SetLevel( idUser, szTarget[], iLevel )
{
	/*if(!(get_user_flags(idUser) & ADMIN_FLAG))
	{
		format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
		LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
		LANG_PLAYER,"YOU_HAVE_NO_ACCESS");		
		
		client_print( idUser, print_console,szMessage);	
		
		return;
	}*/
		
	new iXP = XP_GetByLevel( iLevel );

	new iTarget = 0, bool:bTargetFound = false;

	// Do this while we continue having a target!
	while ( ( iTarget = FindTarget( iTarget, szTarget ) ) > 0 )
	{
		ADMIN_SetXP( iTarget, iXP );

		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
			LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
			LANG_PLAYER,"CLIENT_PRINT_ADMIN_SET_LEVEL",
			iLevel);	
			
		cssbColoredPrint(iTarget, szMessage);
			
		bTargetFound = true;
	}
	
	// No target found :/
	if ( !bTargetFound )
	{
		ADMIN_NoTargetFound( idUser, szTarget, ( iTarget == -2 ) );
	}
}

// Give the user some XP
ADMIN_GiveXP( idUser, szTarget[], iXP )
{
	/*if(!(get_user_flags(idUser) & ADMIN_FLAG))
	{
		format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
		LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
		LANG_PLAYER,"YOU_HAVE_NO_ACCESS");		
		
		client_print( idUser, print_console,szMessage);	
		
		return;
	}*/
		
	new iTarget = 0, bool:bTargetFound = false;
	
	// Do this while we continue having a target!
	while ( ( iTarget = FindTarget( iTarget, szTarget ) ) > 0 )
	{
		//if((get_user_flags(iTarget) & ADMIN_FLAG))
		//{
			ADMIN_SetXP( iTarget, p_data[iTarget][P_XP] + iXP );
		
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
					LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
					LANG_PLAYER,"THE_ADMIN_GAVE_YOU_EXPERIENCE",
					iXP);	
				
			cssbColoredPrint(iTarget, szMessage);

			bTargetFound = true;
		//}
	}

	// No target found :/
	if ( !bTargetFound )
	{
		ADMIN_NoTargetFound( idUser, szTarget, ( iTarget == -2 ) );
	}
}

// Give the user an item
ADMIN_GiveItem( idUser, szTarget[], iItemID )
{/*
	if(!(get_user_flags(idUser) & ADMIN_FLAG))
	{
		format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
		LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
		LANG_PLAYER,"YOU_HAVE_NO_ACCESS");		
		
		client_print( idUser, print_console,szMessage);	
		
		return;
	}*/
		
	new szItemName[32];
	LANG_GetItemName ( iItemID, idUser, szItemName, 31 );

	new iTarget = 0, bool:bTargetFound = false;

	// Do this while we continue having a target!
	while ( ( iTarget = FindTarget( iTarget, szTarget ) ) > 0 )
	{
		/*format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03'%s'",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_ADMIN_GIVE_ITEM",
				szItemName);	
			
		cssbColoredPrint(iTarget, szMessage);*/
			
		//ITEM_GiveItem( iTarget, iItemID );
		ADMIN_ITEM_Buy( iTarget, iItemID );

		bTargetFound = true;
	}

	// No target found :/
	if ( !bTargetFound )
	{
		ADMIN_NoTargetFound( idUser, szTarget, ( iTarget == -2 ) );
	}
}

// Find a user based on szTarget
FindTarget( iLastID, szTarget[] )
{
	new iTarget = -1;

	// Then we want to basically return everyone!
	if ( equali( szTarget, "@ALL" ) )
	{
		new players[32], iTotalPlayers, i;
		get_players( players, iTotalPlayers );
		
		// Loop through and search for the next target
		for ( i = 0; i < iTotalPlayers; i++ )
		{
			// Target found, so lets return the next one (if possible)!!
			if ( players[i] == iLastID && i + 1 != iTotalPlayers )
			{
				iTarget = players[i+1];
			}
		}

		// No target was found so return the first one
		if ( iTotalPlayers > 0 && iLastID == 0 )
		{
			iTarget = players[0];
		}
	}

	// Find a target based on the team
	else if ( szTarget[0] == '@' )
	{
		new iTeam = -1;
		
		// Counter-Strike and Condition Zero Checks
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			if ( equali( szTarget, "@T" ) )
			{
				iTeam = TEAM_T;
			}
			else if ( equali( szTarget, "@CT" ) )
			{
				iTeam = TEAM_CT;
			}
		}
	
		// Make sure a team was found
		if ( iTeam != -1 )
		{
			new players[32], iTotalPlayers, i, iFirstPlayer = -1, bool:bSaveNext = false;
			get_players( players, iTotalPlayers );

			// Loop through and search for the next target
			for ( i = 0; i < iTotalPlayers; i++ )
			{			
				// Make sure they're on the same team
				if ( iTeam == get_user_team( players[i] ) )
				{
					
					// This is the next available player
					if ( bSaveNext )
					{
						iTarget = players[i];
						break;
					}

					// If this is the previous target, we need to get the next one!
					if ( players[i] == iLastID )
					{
						bSaveNext = true;
					}
					
					// Save the FIRST player on this team
					if ( iFirstPlayer == -1 )
					{
						iFirstPlayer = players[i];
					}
				}
			}

			// No target was found so return the first one that matches the team (the target could still be -1 if iFirstPlayer wasn't found)
			if ( iLastID == 0 )
			{
				iTarget = iFirstPlayer;
			}
		}
	}

	// Otherwise search for a player
	else
	{
		// Initial search is by player name
		new iPlayer = find_player( "a", szTarget );
		
		// If not found, search by partial match
		if ( !iPlayer )
		{
			iPlayer = find_player( "bl", szTarget );

			if ( iPlayer )
			{

				// Then Multiple clients found
				if ( iPlayer != find_player( "blj", szTarget ) )
				{
					return -2;
				}
			}
		}

		// If not found, search by auth idUser
		if ( !iPlayer )
		{
			iPlayer = find_player( "c" , szTarget );
		}

		// If not found, search by user idUser
		if ( !iPlayer )
		{
			// Make sure we have a user idUser
			if ( szTarget[0] == '#' && szTarget[1] )
			{
				iPlayer = find_player( "k", str_to_num( szTarget[1] ) )
			}
		}
		
		// Yay we have a match!!!
		if ( iPlayer && iLastID != iPlayer )
		{
			iTarget = iPlayer;
		}
	}

	return iTarget;
}

ADMIN_NoTargetFound( idUser, szTarget[], bool:bMulti )
{
	// Multiple clients found
	if ( bMulti )
	{
		ADMIN_Print( idUser, "There is more than one client matching '%s'", szTarget );
	}

	else
	{
		ADMIN_Print( idUser, "Unable to find target(s) '%s'", szTarget );
	}

	// List what the available targets are
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		ADMIN_Print( idUser, "Available targets are: @ALL, @CT, @T or the player's name/auth/#idUser");
	}

}

// Function will print to server console or client console based on the ID number
ADMIN_Print( idUser, text[], {Float,_}:...)
{    
	// format the text as needed

	new szFormattedText[128];
	format_args( szFormattedText, 127, 1 );

	if ( idUser == 0 )
	{
		server_print( szFormattedText );
	}
	else
	{
		client_print( idUser, print_console, szFormattedText );
	}

	// Gets rid of compiler warning
	if ( text[0] == 0 )
	{
		return;
	}
}  

// Adapted from war3x's log file (I was lazy)
ADMIN_Log( idUser, szCommand[], {Float,_}:... )
{

	new szLogFile[128];
	get_configsdir( szLogFile, 127 );
	formatex( szLogFile, 127, "%s/war3ft/wc3_admin.log", szLogFile );

	new szFormattedText[128];
	format_args( szFormattedText, 127, 2 );

	if ( !file_exists( szLogFile ) )
	{
		write_file( szLogFile, "WC3 : Frozen Throne", -1 );
		write_file( szLogFile, "Logging of admin commands", -1 );
		write_file( szLogFile, " ", -1 );
	}

	new szAdminName[32], szSteamID[32], szTeam[13];
	if ( idUser > 0 )
	{
		get_user_name( idUser, szAdminName, 31 );
		get_user_authid( idUser, szSteamID, 31 );
		get_user_team( idUser, szTeam, 12 );
	}
	else
	{
		copy( szAdminName, 31, "SERVER" );
		copy( szSteamID, 31, "SERVER" );
	}

	new szCurrentTime[64];
	get_time( "L %m/%d/%Y - %H:%M:%S", szCurrentTime,63 );

	new szLogEntry[256];
	formatex( szLogEntry, 255, "%s Cmd: ^"%s<%d><%s><%s>^" '%s %s'", szCurrentTime, szAdminName, get_user_userid( idUser ), szSteamID, szTeam, szCommand, szFormattedText );

	write_file( szLogFile, szLogEntry, -1 );

	// Gets rid of compiler warning
	if ( szCommand[0] == 0 )
	{
		return;
	}
}


public ADMIN_ITEM_CanBuy( idUser, iItem )
{
		
	// User already owns the item and it's not a chargeable item!
	if ( ITEM_Has( idUser, iItem ) > ITEM_NONE && !ITEM_CheckFlag( iItem, ITEM_CHARGEABLE ) )
	{
		client_print( idUser, print_center, "%L", LANG_PLAYER, "ALREADY_OWN_THAT_ITEM" );

		return false;
	}
	
	// Make sure these items can be bought if the user is dead
	else if ( !is_user_alive( idUser ) && !ITEM_CheckFlag( iItem, ITEM_BUYWHENDEAD ) ) 
	{
		client_print( idUser, print_center, "%L", LANG_PLAYER, "NOT_PURCHASE_WHEN_DEAD" );

		return false;
	}
	
	// User has necklace + blink, they don't need a necklace
	else if ( iItem == ITEM_NECKLACE && p_data_b[idUser][PB_WARDENBLINK] )
	{
		client_print( idUser, print_center, "%L", LANG_PLAYER, "CLIENT_PRINT_ITEM_CAN_BUY_0" );

		return false;
	}

	// User doesn't need an ankh if they're going to reincarnate
	else if ( iItem == ITEM_ANKH && SM_GetSkillLevel( idUser, SKILL_REINCARNATION ) == 3 && !p_data[idUser][P_CHANGERACE] && p_data[idUser][P_RACE] != RACE_CHAMELEON )
	{
	
		client_print( idUser, print_center, "%L", LANG_PLAYER, "CLIENT_PRINT_ITEM_CAN_BUY_1" );

		return false;
	}
	
	// User has purchased the maximum allowed rings
	else if ( g_iTotalRings[idUser] >= 5 && iItem == ITEM_RING )
	{
		client_print( idUser, print_center, "%L", LANG_PLAYER, "NOT_PURCHASE_MORE_THAN_FIVE_RINGS" );

		return false;
	}

	// User has purchased gloves when they're disabled on this map
	else if ( iItem == ITEM_GLOVES && g_bGlovesDisabled )
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_ITEM_CAN_BUY_2");	
			
		cssbColoredPrint(idUser, szMessage);	

		return false;
	}

	// User is already going to reincarnate weapons because they bought an ankh earlier (probably lost it when died)
	else if ( ( iItem == ITEM_ANKH && g_bPlayerBoughtAnkh[idUser] ) || ( iItem == ITEM_MOLE && g_bPlayerBoughtMole[idUser] ) )
	{
		client_print( idUser, print_center, "%L", LANG_PLAYER, "ALREADY_OWN_THAT_ITEM" );

		return false;
	}

	//Ограничение на покупку предмета Прикосновение смерти
	else if ( ( iItem == ITEM_DEATH_TOUCH && arrIsByuItemOneRound[idUser][iItem] == BUY_ITEM_ONE_ROUND_DEATH_TOUCH ))
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
					LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
					LANG_PLAYER,"CLIENT_PRINT_LIMIT_BUY_ONE_ROUND",
					BUY_ITEM_ONE_ROUND_DEATH_TOUCH);

		cssbColoredPrint(idUser, szMessage);

		return false;
	}

	//Ограничение на покупку предмета Микстура здоровья
	else if ( ( iItem == ITEM_HEALTH_POTION && arrIsByuItemOneRound[idUser][iItem] == BUY_ITEM_ONE_ROUND_HEALTH_POTION ))
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
					LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
					LANG_PLAYER,"CLIENT_PRINT_LIMIT_BUY_ONE_ROUND",
					BUY_ITEM_ONE_ROUND_HEALTH_POTION);

		cssbColoredPrint(idUser, szMessage);

		return false;
	}

	//Ограничение на покупку предмета Стальная кожа
	else if ( ( iItem == ITEM_STEEL_SKIN && arrIsByuItemOneRound[idUser][iItem] == BUY_ITEM_ONE_ROUND_STEEL_SKIN ))
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
					LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
					LANG_PLAYER,"CLIENT_PRINT_LIMIT_BUY_ONE_ROUND",
					BUY_ITEM_ONE_ROUND_STEEL_SKIN);

		cssbColoredPrint(idUser, szMessage);

		return false;
	}

	return true;
}

public ADMIN_ITEM_Buy( idUser, iItem )
{
	
	// If the user can buy this item...
	if ( ADMIN_ITEM_CanBuy( idUser, iItem ) )
	{

		// User's items are full
		if ( ITEM_GetSlot( idUser ) == ITEM_SLOT_FULL && !ITEM_CheckFlag( iItem, ITEM_USEONBUY ) )
		{

			// We only care about items being full if this item isn't a use on buy
			if ( !ITEM_CheckFlag( iItem, ITEM_USEONBUY ) )
			{

				new bool:bShowReplaceMenu = false;

				// One time use...
				if ( !ITEM_CheckFlag( iItem, ITEM_CHARGEABLE ) )
				{
					//client_print( idUser, print_chat, "[DEBUG] Item is not chargeable" );

					bShowReplaceMenu = true;
				}

				// We also need to replace it if the item is chargeable but they don't own that item
				if ( ITEM_Has( idUser, iItem ) == ITEM_NONE && ITEM_CheckFlag( iItem, ITEM_CHARGEABLE ) )
				{
					//client_print( idUser, print_chat, "[DEBUG] Doesn't have item and new item is chargeable" );

					bShowReplaceMenu = true;
				}

				if ( bShowReplaceMenu )
				{
					g_iFutureItem[idUser] = iItem;

					MENU_ReplaceItem( idUser );

					return;
				}
			}
		}
		
		ITEM_GiveItem( idUser, iItem );
	}

	return;
}