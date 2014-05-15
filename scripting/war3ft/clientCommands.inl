#define ULT_SEARCHTIME		50			// Translates to 5 seconds

// This isn't actually called when they join spectator :/
public cmd_Teamselect( idUser, key )
{
	if ( !WC3_Check() )
	{
		return;
	}

	p_data_b[idUser][PB_CHANGINGTEAM] = true;
}

public cmd_Jointeam( idUser )
{
	if ( !WC3_Check() )
	{
		return;
	}

	p_data_b[idUser][PB_CHANGINGTEAM] = true;
}

// This is here to prevent the user from forcing a ResetHUD event
public cmd_fullupdate()
{
	if ( !WC3_Check() )
	{
		return PLUGIN_CONTINUE;
	}

	return PLUGIN_HANDLED;
}

// Called when the user buys a he grenade
public cmd_hegren( idUser )
{ 
	if ( !WC3_Check() )
	{
		return PLUGIN_CONTINUE;
	}

	// Make sure we even have grenade protection on
	if ( !get_pcvar_num( CVAR_wc3_grenade_protection ) )
	{
		return PLUGIN_CONTINUE;
	}

	// Then they shouldn't be buying a grenade anyway
	if ( !cs_get_user_buyzone( idUser ) )
	{
		return PLUGIN_HANDLED;
	}


	// User can only buy one grenade per round
	if ( p_data[idUser][P_HECOUNT] > 0 )
	{ 
		client_print( idUser, print_center, "%L", LANG_PLAYER, "ONLY_ONE_GRENADE_PER_ROUND" );

		return PLUGIN_HANDLED;
	}
	
	// Increment since they bought their grenade
	else
	{
		++p_data[idUser][P_HECOUNT];

		return PLUGIN_CONTINUE;
	}

	return PLUGIN_CONTINUE;
} 

public cmd_Ultimate(idUser)
{

	if ( !WC3_Check( idUser ) )
	{
		return PLUGIN_HANDLED;
	}

	// User can't use ultimate when dead
	if ( !is_user_alive( idUser ) )
	{
		return PLUGIN_HANDLED;
	}

	new iSkillID = SM_GetSkillOfType( idUser, SKILL_TYPE_ULTIMATE );
	new iSkillLevel = SM_GetSkillLevel( idUser, iSkillID, 3 );

	// User can't use their ultimate when they're hexed
	if ( p_data_b[idUser][PB_HEXED] )
	{
		WC3_StatusText( idUser, TXT_ULTIMATE, "%L", LANG_PLAYER, "HEX_NO_ABILITY" );

		client_cmd( idUser, "speak %s", g_szSounds[SOUND_ERROR] );

		return PLUGIN_HANDLED;
	}
	
	// User has no ultimate!
	else if ( iSkillLevel == 0 )
	{
		WC3_StatusText( idUser, TXT_ULTIMATE, "%L", LANG_PLAYER, "ULTIMATE_NOT_FOUND" );

		client_cmd( idUser, "speak %s", g_szSounds[SOUND_ERROR] );

		return PLUGIN_HANDLED;
	}
	
/*	// We're still in the global delay :/
	else if ( g_iUltimateDelay > 0 )
	{
		WC3_StatusText( idUser, TXT_ULTIMATE, "%L", LANG_PLAYER, "ULTIMATE_NOT_READY", g_iUltimateDelay );
		
		client_cmd( idUser, "speak %s", g_szSounds[SOUND_ERROR] );

		return PLUGIN_HANDLED;
	}*/

	// Ultimate is used or not ready yet
	else if ( p_data[idUser][P_ULTIMATEDELAY] > 0 )
	{
		WC3_StatusText( idUser, TXT_ULTIMATE, "%L", LANG_PLAYER, "ULTIMATE_NOT_READY", p_data[idUser][P_ULTIMATEDELAY] );

		client_cmd( idUser, "speak %s", g_szSounds[SOUND_ERROR] );

		return PLUGIN_HANDLED;
	}

	// If we got here, then we can cast the user's ultimate
	switch ( iSkillID )
	{
		
		// UNDEAD - Suicide Bomber
		case ULTIMATE_SUICIDE:
		{
			// User has already had their warning - kill them!
			if ( p_data_b[idUser][PB_SUICIDEATTEMPT] )
			{
				WC3_KillUser( idUser, 0, 0 );
			
				p_data_b[idUser][PB_SUICIDEATTEMPT] = false
			}

			// Give the user his/her warning
			else
			{
				// Flash the user's ultimate icon
				ULT_Icon( idUser, ICON_FLASH );

				p_data_b[idUser][PB_SUICIDEATTEMPT] = true

				WC3_StatusText( idUser, TXT_BLINK_CENTER, "%L", LANG_PLAYER, "SUICIDE_BOMB_ARMED" );
			}
		}

		// HUMAN ALLIANCE - Blink
		case ULTIMATE_BLINK:
		{
			HU_ULT_Blink( idUser );
		}

		// ORCISH HORDE - Chain Lightning
		case ULTIMATE_CHAINLIGHTNING:
		{
			if ( !p_data_b[idUser][PB_ISSEARCHING] )
			{
				p_data_b[idUser][PB_ISSEARCHING] = true;

				// Don't continue if task already exists...
				if ( !task_exists( TASK_ULTPING + idUser ) )
				{
					new parm[2];
					parm[0] = idUser;
					parm[1] = 5;
					_ULT_Ping( parm );
				}
			}
		}

		// NIGHT ELF - Entangling Roots
		case ULTIMATE_ENTANGLE:
		{
			if ( !p_data_b[idUser][PB_ISSEARCHING] )
			{
				p_data_b[idUser][PB_ISSEARCHING] = true;

				// Don't continue if task already exists...
				if ( !task_exists( TASK_ULTPING + idUser ) )
				{
					new parm[2];
					parm[0] = idUser;
					parm[1] = 5;
					_ULT_Ping( parm );
				}
			}
		}

		// BLOOD MAGE - Immolate
		case ULTIMATE_IMMOLATE:
		{
		//	if(p_data[idUser][P_RACE] == RACE_BLOOD)
		//	{
				p_data_b[idUser][PB_ISSEARCHING] = true;

				// Don't continue if task already exists...
				if ( !task_exists( TASK_ULTPING + idUser ) )
				{
					new parm[2];
					parm[0] = idUser;
					parm[1] = 5;
					_ULT_Ping( parm );
				}
		//	}

		}

		// SHADOW HUNTER - Big Bad Voodoo
		case ULTIMATE_BIGBADVOODOO:
		{
			SH_Ult_BigBadVoodoo( idUser );
		}

		// WARDEN - Vengeance
		case ULTIMATE_VENGEANCE:
		{
			WA_ULT_Vengeance( idUser );
		}

		// CRYPT LORD - Locust Swarm
		case ULTIMATE_LOCUSTSWARM:
		{

			if ( get_pcvar_num( CVAR_wc3_psychostats ) )
			{
				new WEAPON = CSW_LOCUSTS - CSW_WAR3_MIN;

				iStatsShots[idUser][WEAPON]++;
			}

			CL_ULT_LocustSwarm( idUser );
		}
		// Frost Mage - Blizzard - Снежная буря
		case ULTIMATE_FROST_MAGE_BLIZZARD:
		{
			p_data_b[idUser][PB_ISSEARCHING] = true;

			// Don't continue if task already exists...
			if ( !task_exists( TASK_ULTPING + idUser ) )
			{
				new parm[2];
				parm[0] = idUser;
				parm[1] = 5;
				_ULT_Ping( parm );
			}
		}

		// Chameleon - Flame Strike
		case ULTIMATE_CHAM_FLAME_STRIKE:
		{
			if(p_data[idUser][P_RACE] == RACE_CHAMELEON)
			{
				Ultimate_FlameStrike(idUser);
				p_data[idUser][P_FLAMECOUNT]++;
				if(p_data[idUser][P_FLAMECOUNT]>5)
				{
					ULT_Icon( idUser, ICON_HIDE );
					p_data[idUser][P_FLAMECOUNT]=0;

				}
			}

		}
		
	}

	return PLUGIN_HANDLED;
}


public CMD_Handler( idUser )
{

	new szCmd[32];

	read_argv( 0, szCmd, 31 );

	CMD_Handle( idUser, szCmd, false );

	return PLUGIN_HANDLED;
}

public cmd_Say( idUser )
{
	
	new szSaid[32];
	read_args( szSaid, 31 );

	remove_quotes( szSaid );

	CMD_Handle( idUser, szSaid, true );

	return;
}

// Command handler
CMD_Handle( idUser, szCmd[], bool:bThroughSay )
{
	// Change the user's race
	if ( CMD_Equal( idUser,  szCmd, "changerace" ) )
	{
		WC3_ChangeRaceStart(idUser);
	}
	/*//Команда для выбора расы хамелиона
	else if ( CMD_Equal( idUser, szCmd, "cham" ) )
	{
		if (get_user_flags(idUser) & ADMIN_LEVEL_H)
		{
			WC3_SetRace(idUser, 9);
		}
	} */

	// Display select skill menu
	else if ( CMD_Equal( idUser,  szCmd, "selectskills" ) || CMD_Equal( idUser,  szCmd, "selectskill" ) )
	{
		MENU_SelectSkill( idUser );
	}

	else if ( CMD_Equal( idUser,  szCmd, "playerskills" ) )
	{
		MOTD_PlayerSkills( idUser, bThroughSay );
	}

	else if ( CMD_Equal( idUser,  szCmd, "ms" ) || CMD_Equal( idUser,  szCmd, "movespeed" ) )
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%0.0f",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_MOVE_SPEED",
				get_user_maxspeed( idUser ));
			
		cssbColoredPrint(idUser, szMessage);
	}

	else if ( CMD_Equal( idUser,  szCmd, "skillsinfo" ) )
	{
		MOTD_SkillsInfo( idUser );
	}

	else if ( CMD_Equal( idUser,  szCmd, "war3help" ) )
	{
		MOTD_War3help(idUser)
	}
	else if ( CMD_Equal( idUser, szCmd, "debug" ) )
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%d ^x04%L ^x03%d",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_DEBUG_ULT_COOL",
				g_iUltimateDelay,
				LANG_PLAYER,"CLIENT_PRINT_DEBUG_PLAYER",
				p_data[idUser][P_ULTIMATEDELAY] );
			
		cssbColoredPrint(idUser, szMessage);
	}

	else if ( CMD_Equal( idUser, szCmd, "levitation" ) )
	{
		if ( SM_GetSkillLevel( idUser, SKILL_LEVITATION ) <= 0 )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_LEVITATION_0");
			
			cssbColoredPrint(idUser, szMessage);

			return;
		}

		if ( g_bLevitation[idUser] )
		{
			g_bLevitation[idUser] = false;
		
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_LEVITATION_1");
			
			cssbColoredPrint(idUser, szMessage);
		}
		else
		{
			g_bLevitation[idUser] = true;
		
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_LEVITATION_2");
			
			cssbColoredPrint(idUser, szMessage);
		}

		SHARED_SetGravity( idUser );
	}

	else if ( CMD_Equal( idUser,  szCmd, "level" ) )
	{
		WC3_ShowRaceInfo( idUser );
		WC3_ShowBar( idUser );
	}

	else if ( CMD_Equal( idUser,  szCmd, "shopmenu" ) )
	{
		MENU_Shopmenu( idUser, 0 );
	}

	else if ( CMD_Equal( idUser,  szCmd, "resetxp" ) )
	{
		if((get_user_flags(idUser) & ADMIN_FLAG))
		{
			MENU_ResetXP( idUser );
		}
		else
		{
			format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
			LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
			LANG_PLAYER,"YOU_HAVE_NO_ACCESS");		
			
			client_print( idUser, print_console,szMessage);
			
			return;
		}
	}

	else if ( CMD_Equal( idUser,  szCmd, "itemsinfo" ) )
	{
		MOTD_ItemsInfo( idUser, 0 )
	}
	else if ( CMD_Equal( idUser,  szCmd, "war3menu" ) )
	{
		MENU_War3Menu( idUser );
	}
	else if ( CMD_Equal( idUser,  szCmd, "savexp" ) )
	{
	   format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_SAVE_XP");
			
			cssbColoredPrint(idUser, szMessage);
	}

	else if ( CMD_Equal( idUser,  szCmd, "resetskills" ) )
	{
		// Special message for csdm
		if ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 )
		{
			format(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_RESET_SKILLS");
			client_print( idUser, print_center,szMessage);
		}
		else
		{
			format(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"SKILLS_RESET_NEXT_ROUND");
			client_print( idUser, print_center,szMessage);
		}
		
		p_data_b[idUser][PB_RESETSKILLS] = true;
	}
	// Cheat command if it's enabled
	else if ( get_pcvar_num( CVAR_wc3_cheats ) && ( CMD_Equal( idUser,  szCmd, "level10" ) || CMD_Equal( idUser,  szCmd, "lvl10" ) ) )
	{
		new iRaceID = p_data[idUser][P_RACE];

		// They haven't been given free XP for this race yet
		if ( !g_bGivenLevel10[idUser][iRaceID] )
		{
			
			// Save their XP now, b/c we're not going to later
			DB_SaveXP( idUser, true );
	
			// OK give them level 10
			p_data[idUser][P_XP] = XP_GetByLevel( 10 );

			XP_Check( idUser );
			
			// Make sure we set that we've given them XP already!
			g_bGivenLevel10[idUser][iRaceID] = true;

			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_CHEATER");
			
			cssbColoredPrint(idUser, szMessage);
		}
		// We've already given them XP!!!
		else
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_ALREADY_GIVEN_LVL");
			
			cssbColoredPrint(idUser, szMessage);
		}
	}
	
	// Ability to buy items through console commands
	else if ( CMD_Equal( idUser, szCmd, "ankh" ) )
	{
		if ( ITEM_MenuCanBuyCheck(idUser) )ITEM_Buy( idUser, ITEM_ANKH );
	}

	else if ( CMD_Equal( idUser, szCmd, "boots" ) )
	{
		if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_BOOTS );
	}

	else if ( CMD_Equal( idUser, szCmd, "claws" ) )
	{
		if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_CLAWS );
	}

	else if ( CMD_Equal( idUser, szCmd, "cloak" ) )
	{
		if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_CLOAK );
	}

	else if ( CMD_Equal( idUser, szCmd, "mask" ) )
	{
		if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_MASK );
	}

	else if ( CMD_Equal( idUser, szCmd, "necklace" ) )
	{
		if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_NECKLACE );
	}

	else if ( CMD_Equal( idUser, szCmd, "frost" ) )
	{
		if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_FROST );
	}

	else if ( CMD_Equal( idUser, szCmd, "health" ) )
	{
		if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_HEALTH );
	}

	else if ( CMD_Equal( idUser, szCmd, "tome" ) )
	{
		if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_TOME );
	}

	if ( get_pcvar_num( CVAR_wc3_races ) > 4 )
	{

		if ( CMD_Equal( idUser,  szCmd, "itemsinfo2" ) )
		{
			MOTD_ItemsInfo( idUser, 9 );
		}
		else if ( CMD_Equal( idUser, szCmd, "scroll" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_SCROLL );
		}

		else if ( CMD_Equal( idUser, szCmd, "helm" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_HELM );
		}

		else if ( CMD_Equal( idUser, szCmd, "amulet" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_AMULET );
		}

		else if ( CMD_Equal( idUser, szCmd, "socks" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_SOCK );
		}

		else if ( CMD_Equal( idUser, szCmd, "gloves" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_GLOVES );
		}

		else if ( CMD_Equal( idUser,  szCmd, "rings" ) )
		{
			if ( ITEM_MenuCanBuyCheck( idUser ) ) ITEM_BuyRings( idUser );
		}

		else if ( CMD_Equal( idUser, szCmd, "chameleon" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_CHAMELEON );
		}

		else if ( CMD_Equal( idUser, szCmd, "mole" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_MOLE );
		}

		else if ( CMD_Equal( idUser,  szCmd, "ability" ) )
		{
			SH_PlaceSerpentWard( idUser );
		}

		else if ( CMD_Equal( idUser,  szCmd, "shopmenu2" ) )
		{
			MENU_Shopmenu( idUser, 9 );
		}
		//Для shopmenu3
		else if ( CMD_Equal( idUser,  szCmd, "shopmenu3" ) )
		{
			MENU_Shopmenu( idUser, 18 );
		}
		else if ( CMD_Equal( idUser,  szCmd, "itemsinfo3" ) )
		{
			MOTD_ItemsInfo( idUser, 18 );
		}
		else if ( CMD_Equal( idUser, szCmd, "brain" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_INTELLECT );
		}
		else if ( CMD_Equal( idUser, szCmd, "nogren" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_GRENADE_PROTECTION );
		}
		else if ( CMD_Equal( idUser, szCmd, "mirror" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_MIRROR_SHIELD );
		}
		else if ( CMD_Equal( idUser, szCmd, "esp" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_ESP );
		}
		else if ( CMD_Equal( idUser, szCmd, "farmor" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_FROST_ARMOR );
		}
		else if ( CMD_Equal( idUser, szCmd, "deathtouch" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_DEATH_TOUCH );
		}
		else if ( CMD_Equal( idUser, szCmd, "hpp" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_HEALTH_POTION );
		}
		else if ( CMD_Equal( idUser, szCmd, "ulttimediv" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_ULTIMATE_TIMER_DIVIDER );
		}
		else if ( CMD_Equal( idUser, szCmd, "steelskin" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_STEEL_SKIN );
		}

		//Для shopmenu4
		else if ( CMD_Equal( idUser,  szCmd, "shopmenu4" ) )
		{
			MENU_Shopmenu( idUser, 27 );
		}
		else if ( CMD_Equal( idUser,  szCmd, "itemsinfo4" ) )
		{
			MOTD_ItemsInfo( idUser, 27 );
		}
		else if ( CMD_Equal( idUser, szCmd, "devlight" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_DEVOURER_LIGHT );
		}
		else if ( CMD_Equal( idUser, szCmd, "iceblock" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_ICE_BLOCK );
		}
		else if ( CMD_Equal( idUser, szCmd, "thief" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_THIEF );
		}
		else if ( CMD_Equal( idUser, szCmd, "mirrorult" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_MIRROR_ULTIMATE );
		}
		else if ( CMD_Equal( idUser, szCmd, "deathcoil" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_DEATHCOIL );
		}
		else if ( CMD_Equal( idUser, szCmd, "impale" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_IMPALE );
		}
		else if ( CMD_Equal( idUser, szCmd, "sleep" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_SLEEP );
		}
		else if ( CMD_Equal( idUser, szCmd, "skeletons" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_BOOK_DEAD );
		}
		else if ( CMD_Equal( idUser, szCmd, "tranquility" ) )
		{
			if (ITEM_MenuCanBuyCheck(idUser)) ITEM_Buy( idUser, ITEM_TRANQUILITY );
		}
		//Для talisman +
		else if ( CMD_Equal( idUser, szCmd, "talisman" ) )
		{
			fShowMenuTalismans(idUser);
		}
		//Для авто закупки предметов
		else if ( CMD_Equal( idUser, szCmd, "autobuyitems" ) )
		{
			fShowMenuAutoBuyBase(idUser);
		}
		
	}
	
	return;
}

// Function will check if the first string is equal to the second (checks for NAME or /NAME)
CMD_Equal( idUser,  szCmd[], szCorrectCmd[] )
{

	new szTmp[64];
	formatex( szTmp, 63, "/%s", szCorrectCmd );

	new bool:bValid = equali( szCmd, szTmp ) || equali( szCmd, szCorrectCmd );
	
	if ( !WC3_Check() )
	{
		
		// Only print a message if the command was valid
		if ( bValid )
		{
			WC3_Check( idUser );
		}
	
		return false;
	}


	return bValid;
}
