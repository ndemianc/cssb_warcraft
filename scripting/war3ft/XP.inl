/*
*	XP Functions
*/

#define XP_NEARBY_RADIUS		250

// START FROM CSX.INC - This is a hack b/c we can't include CSX in the same plugin
forward bomb_planting(planter);
forward bomb_planted(planter);
forward bomb_defusing(defuser);
forward bomb_defused(defuser);
// END FROM CSX.INC

// Called when a user begins defusing the bomb
public bomb_defusing( defuser )
{
	if ( !WC3_Check() )
	{
		return;
	}

	// Make sure that this isn't called more than once per round
	if ( !bHasBegunPlantingOrDefusing[defuser] )
	{
		new iBonusXP = XP_Give( defuser, DEFUSING_BOMB );

		if ( iBonusXP != 0 )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_DEFUSING_BOMB",
				iBonusXP);	
			
			cssbColoredPrint(defuser, szMessage);
		}
	}

	bHasBegunPlantingOrDefusing[defuser] = true;
}

// Called when a user defuses the bomb
public bomb_defused( defuser )
{
	if ( !WC3_Check() )
	{
		return;
	}

	new iBonusXP = XP_Give( defuser, DEFUSED_BOMB );

	if ( iBonusXP != 0 )
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_DEFUSED_BOMB",
				iBonusXP);	
			
		cssbColoredPrint(defuser, szMessage);
	}

	g_iPlayerRole[defuser] = 0;
}

// Called when a user begins planting the bomb
public bomb_planting( planter )
{
	if ( !WC3_Check() )
	{
		return;
	}

	// Make sure that this isn't called more than once per round
	if ( !bHasBegunPlantingOrDefusing[planter] )
	{
		new iBonusXP = XP_Give( planter, PLANTING_BOMB );

		if ( iBonusXP != 0 )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_PLANTING_BOMB",
				iBonusXP);	
			
			cssbColoredPrint(planter, szMessage);			
		}
	}

	g_iPlayerRole[planter] = PLR_BOMB_PLANTER;
	bHasBegunPlantingOrDefusing[planter] = true;
}

// Called when a user has planted the bomb
public bomb_planted( planter )
{
	if ( !WC3_Check() )
	{
		return;
	}

	new iBonusXP = XP_Give( planter, PLANT_BOMB );

	if ( iBonusXP != 0 )
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_PLANTED_BOMB",
				iBonusXP);	
			
		cssbColoredPrint(planter, szMessage);			
	}

	g_iPlayerRole[planter] = 0;
	p_data_b[planter][PB_ISPLANTING] = false;

	// Serpent Ward Check
	SH_BombPlanted();
}

// Called on certain player events
public on_PlayerAction()
{
	if ( !WC3_Check() )
	{
		return;
	}

	new sArg[64], sAction[64], sName[64];
	new iUserId, idUser;

	read_logargv( 0,sArg,64 );
	read_logargv( 2,sAction,64 );
	parse_loguser( sArg,sName,64,iUserId );
	idUser = find_player( "k", iUserId );

	if ( idUser == 0 )
	{
		return;
	}

	// If the round is over do not award XP!
	if ( g_EndRound )
	{
		return;
	}

	// Spawned with the bomb 
	if (equal(sAction,"Spawned_With_The_Bomb"))
	{
		g_iPlayerRole[idUser] = PLR_BOMB_CARRIER;
		new iBonusXP = XP_Give( idUser, SPAWN_BOMB );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_SPAWNING_WITH_BOMB",
				iBonusXP);	
			
			cssbColoredPrint(idUser, szMessage);			
		}
	}
	// Dropped bomb 
	else if (equal(sAction,"Dropped_The_Bomb"))
	{
		g_iPlayerRole[idUser] = 0;
		new iBonusXP = XP_Give( idUser, -1 * BOMB_PICKUP );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_LOST_BOMB",
				-1 * iBonusXP);	
			
			cssbColoredPrint(idUser, szMessage);			
		}
	}
	// Picked up bomb 
	else if (equal(sAction,"Got_The_Bomb"))
	{ 
		g_iPlayerRole[idUser] = PLR_BOMB_CARRIER;
		new iBonusXP = XP_Give( idUser, BOMB_PICKUP );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_PICKING_UP_BOMB",
				iBonusXP);	
			
			cssbColoredPrint(idUser, szMessage);			
		}
	}
	// Hostage touched 
	else if (equal(sAction,"Touched_A_Hostage"))
	{
		g_iPlayerRole[idUser] = PLR_HOSTAGE_RESCUER;
		new iBonusXP = XP_Give( idUser, TOUCH_HOSTAGE );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_BEGINNING_HOSTAGE_RESCUE",
				iBonusXP);	
			
			cssbColoredPrint(idUser, szMessage);			
		}
	}
	// Hostage rescued 
	else if (equal(sAction,"Rescued_A_Hostage"))
	{
		new iBonusXP = XP_Give( idUser, RESCUE_HOSTAGE );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_RESCUING_HOSTAGE",
				iBonusXP);	
			
			cssbColoredPrint(idUser, szMessage);			
		}
	}
	// Hostage killed 
	else if (equal(sAction,"Killed_A_Hostage"))
	{
		new iBonusXP = XP_Give( idUser, -1 * KILL_HOSTAGE );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_KILLING_HOSTAGE",
				-1 * iBonusXP);	
			
			cssbColoredPrint(idUser, szMessage);
		}
	}
	// VIP spawn 
	else if (equal(sAction,"Became_VIP"))
	{ 
		g_iPlayerRole[idUser] = PLR_VIP;
		new iBonusXP = XP_Give( idUser, SPAWN_VIP );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_SPAWN_AS_VIP",
				iBonusXP);	
			
			cssbColoredPrint(idUser, szMessage);
		}
	}
	// VIP escaped 
	else if (equal(sAction,"Escaped_As_VIP"))
	{
		new iBonusXP = XP_Give( idUser, ESCAPE_VIP );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_ESCAPED_VIP",
				iBonusXP);	
			
			cssbColoredPrint(idUser, szMessage);
		}
	}
	
}

XP_onDeath( iVictim, iAttacker, iWeaponIndex, iHeadshot )
{

	// We don't want to give XP to the world
	if ( iAttacker == 0 )
	{
		return;
	}

	// We don't want to give XP to suiciders
	else if ( iAttacker == iVictim )
	{
		return;
	}
	
	new iLevel = p_data[iVictim][P_LEVEL]
	new iBonusXP, iXP = floatround( XP_GivenByLevel( iLevel ) * fWpnXPMultiplier[iWeaponIndex] );

	// Check for a team kill
	if ( get_user_team( iAttacker ) == get_user_team( iVictim ) )
	{
		// Remove XP since he killed his teammate
		iBonusXP = XP_Give( iAttacker, -1 * iXP );

		// This message should be displayed no matter what XP_kill_objectives is, b/c it's a no-no
		if ( iBonusXP != 0 )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_TEAM_KILL",
				 -1 * iBonusXP);	
			
			cssbColoredPrint(iAttacker, szMessage);			
		}

	}

	// Otherwise the player killed the other team
	else
	{
		// Award XP for a kill
		iBonusXP = XP_Give( iAttacker, iXP );
		
		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"AWARD_FOR_ENEMY_KILL",
				 iBonusXP);	
			
			cssbColoredPrint(iAttacker, szMessage);			
		}

		// User had a headshot?  Give bonus XP!
		if ( iHeadshot )
		{
			iBonusXP = XP_Give( iAttacker, KILL_HEADSHOT );
			
			if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
			{
				format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
					LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
					LANG_PLAYER,"AWARD_FOR_KILL_HEADSHOT",
					 iBonusXP);
				
				cssbColoredPrint(iAttacker, szMessage);
			}
		}

		new iAssistLevel, iAssistXP, iVictimMaxHealth;
		new Float:fMultiplier;

		// Award XP for other people doing damage to this victim
		for ( new i = 0; i < MAXPLAYERS; i++ )
		{
			// Then this player dealt some damage to this player this round
			if ( g_iDamageDealt[i][iVictim] > 0 && iAttacker != i )
			{
				iVictimMaxHealth = get_user_maxhealth( iVictim );
				fMultiplier = float( g_iDamageDealt[i][iVictim] ) / float( iVictimMaxHealth );
				
				iAssistLevel = p_data[i][P_LEVEL];
				iAssistXP = XP_GivenByLevel( iAssistLevel );

				// Need a ratio of XP to award to person who dealt damage
				iBonusXP = XP_Give( i, ( floatround( float( iAssistXP ) * fMultiplier ) / 2 ) );

				if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
						LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
						LANG_PLAYER,"AWARD_FOR_KILL_ASSIST",
						iBonusXP);
				
					cssbColoredPrint(i, szMessage);					
				}

				// victim may respawn, so reset the counter
				g_iDamageDealt[i][iVictim] = 0;
			}
		}

		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			
			// User killed a hostage rescuer
			if ( g_iPlayerRole[iVictim] == PLR_HOSTAGE_RESCUER )
			{
				iBonusXP = XP_Give( iAttacker, KILL_HOSTAGE_SAVER );
				
				if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
						LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
						LANG_PLAYER,"AWARD_FOR_KILL_HOSTAGE_RESCUER",
						iBonusXP);
				
					cssbColoredPrint(iAttacker, szMessage);
				}
			}
			// User killed the bomb defuser
			else if ( g_iPlayerRole[iVictim] == PLR_BOMB_DEFUSER )
			{
				iBonusXP = XP_Give( iAttacker, KILL_DEFUSER );
				
				if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
						LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
						LANG_PLAYER,"AWARD_FOR_KILL_DEFUSER",
						iBonusXP);
				
					cssbColoredPrint(iAttacker, szMessage);					
				}
			}
			// User killed the bomb planter
			else if ( g_iPlayerRole[iVictim] == PLR_BOMB_PLANTER )
			{
				iBonusXP = XP_Give( iAttacker, KILL_PLANTER );
				
				if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
						LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
						LANG_PLAYER,"AWARD_FOR_KILL_PLANTER",
						iBonusXP);
				
					cssbColoredPrint(iAttacker, szMessage);						
				}
			}
			// User killed the bomb carrier
			else if ( g_iPlayerRole[iVictim] == PLR_BOMB_CARRIER )
			{
				iBonusXP = XP_Give( iAttacker, KILL_BOMB_CARRIER );
				
				if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
						LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
						LANG_PLAYER,"AWARD_FOR_KILL_BOMB_CARRIER",
						iBonusXP);
				
					cssbColoredPrint(iAttacker, szMessage);						
				}
			}
			// user killed the VIP
			else if ( g_iPlayerRole[iVictim] == PLR_VIP )
			{
				iBonusXP = XP_Give( iAttacker, KILL_VIP );
				
				if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
				{
					format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
						LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
						LANG_PLAYER,"AWARD_FOR_KILL_VIP",
						iBonusXP);
				
					cssbColoredPrint(iAttacker, szMessage);					
				}
			}
		}
		
		// Player died, so lets reset their data
		g_iPlayerRole[iVictim] = 0;
	}
}


XP_WinRound( iWinner )
{
	
	new players[32], numberofplayers, i, idUser;
	get_players( players, numberofplayers );

	for ( i = 0; i < numberofplayers; i++ )
	{
		idUser = players[i];

		// Is the user on the correct team?
		if ( get_user_team( idUser ) == iWinner )
		{

			new iBonusXP = XP_Give( idUser, WIN_ROUND );

			if ( get_pcvar_num( CVAR_wc3_show_objectives ) )
			{				
				format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
						LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
						LANG_PLAYER,"AWARD_FOR_WINNING_ROUND",
						iBonusXP);
				
				cssbColoredPrint(idUser, szMessage);
			}
		}
	}
}

XP_GetByLevel( iLevel )
{
	//CSSB
	if ( iLevel < 0 || iLevel > MAX_LEVELS )
	{
		return 0;
	}

	// Change based on XP multiplier!
	new Float:fXPMult = get_pcvar_float( CVAR_wc3_xp_multiplier );

	if ( get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return floatround( iXPLevelSaved[iLevel] * fXPMult );
	}

	return floatround( iXPLevelShortTerm[iLevel] * fXPMult );
}


XP_GivenByLevel( iLevel )
{
	//CSSB
	if ( iLevel < 0 || iLevel > MAX_LEVELS )
	{
		return 0;
	}

	if ( get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return iXPGivenSaved[iLevel];
	}

	return iXPGivenShortTerm[iLevel];
}

bool:XP_MinPlayers()
{
	new iNum = 0;

	// Don't ignore bots
	if ( get_pcvar_num( CVAR_wc3_ignore_bots ) == 0 )
	{
		iNum = get_playersnum();
	}

	// Ignore bots, spectators, and HLTV
	else
	{
		new players[32];
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			new iNumCT;
			get_players( players, iNum, "ce", "TERRORIST" );
			get_players( players, iNumCT, "ce", "CT" );
			iNum += iNumCT;
		}
	}

	if ( iNum < get_pcvar_num( CVAR_wc3_min_players ) )
	{
		return false;
	}

	return true;
}

// Function will verify if the user's level/information is correct and checks to see if the user has gained a level
XP_Check( idUser, bShowGained = true )
{
	new iOldLevel = p_data[idUser][P_LEVEL], i;
	
	// Make sure the user doesn't have negative experience
	if ( p_data[idUser][P_XP] < 0 )
	{
		p_data[idUser][P_XP] = 0;
	}

	// Determine what the user's level should be
	for ( i = 0; i <= MAX_LEVELS; i++ )
	{
		// User has enough XP to advance to the next level
		if ( p_data[idUser][P_XP] >= XP_GetByLevel( i ) )
		{
			p_data[idUser][P_LEVEL] = i;
		}

		// On this increment the user doesn't have enough XP to advance to the next level
		else
		{
			break;
		}
	}

	// User gained a level
	if ( p_data[idUser][P_LEVEL] > iOldLevel && p_data[idUser][P_RACE] != 0 && bShowGained )
	{
		WC3_StatusText( idUser, TXT_TOP_CENTER, "%L", LANG_PLAYER, "YOU_GAINED_A_LEVEL" );
		
		// Only play the sound if the user is alived!!
		if ( is_user_alive( idUser ) )
		{
			emit_sound( idUser, CHAN_STATIC, g_szSounds[SOUND_LEVELUP], 1.0, ATTN_NORM, 0, PITCH_NORM );
		}
	}

	// We might need to lower the skills the user has ( can occur if you load XP info from a database and the XP multiplier has changed )
	new iSkillsUsed = SM_TotalSkillPointsUsed( idUser );
	new iSkillID, iSkillLevel;
	new bool:bSkillRemoved = false;

	while ( iSkillsUsed > p_data[idUser][P_LEVEL] )
	{
		// We might need to remove the ult first!
		if ( p_data[idUser][P_LEVEL] < MIN_ULT_LEVEL )
		{
			new iUltimateID = SM_GetSkillOfType( idUser, SKILL_TYPE_ULTIMATE );
			
			// Then we need to remove this!
			if ( SM_GetSkillLevel( idUser, iUltimateID, 0 ) == 1 )
			{
				// Remove the ult
				SM_SetSkillLevel( idUser, iUltimateID, 0, 0 );

				bSkillRemoved = true;

				// Clear ult icon!
				ULT_ClearIcons( idUser );

				// Get our current skills used
				iSkillsUsed = SM_TotalSkillPointsUsed( idUser );

				// Restart the loop!
				continue;
			}
		}

		iSkillID = SM_GetRandomSkill( idUser );
		iSkillLevel = SM_GetSkillLevel( idUser, iSkillID, 1 );

		// We don't want a passive skill or a skill that is level 0 so keep trying
		while ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_PASSIVE || iSkillLevel == 0 )
		{
			iSkillID = SM_GetRandomSkill( idUser );
			iSkillLevel = SM_GetSkillLevel( idUser, iSkillID, 2 );
		}
		
		// OK at this point we have a valid skill, lets remove a level!
		SM_SetSkillLevel( idUser, iSkillID, iSkillLevel - 1, 1 );

		bSkillRemoved = true;

		// Get our current skills used
		iSkillsUsed = SM_TotalSkillPointsUsed( idUser );
	}
	
	// Need to reset all skill data for the given race if we've removed a skill
	if ( bSkillRemoved )
	{
		WC3_InitPlayerSkills( idUser );
	}

	// OK lets check the total skills the user has, and maybe show them the selectskills menu
	new iTotalSkillsUsed = SM_TotalSkillPointsUsed( idUser );

	if ( iTotalSkillsUsed < p_data[idUser][P_LEVEL] )
	{
		MENU_SelectSkill( idUser );
	}

	WC3_ShowBar( idUser );
}

XP_Configure()
{

	// Configure based on weapon multiplier

	// Counter-Strike and Condition Zero weapons
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		fWpnXPMultiplier[CSW_USP			]	= 2.5;
		fWpnXPMultiplier[CSW_DEAGLE			]	= 2.5;
		fWpnXPMultiplier[CSW_GLOCK18		]	= 2.5;
		fWpnXPMultiplier[CSW_ELITE			]	= 3.5;
		fWpnXPMultiplier[CSW_P228			]	= 2.5;
		fWpnXPMultiplier[CSW_FIVESEVEN		]	= 2.5;

		fWpnXPMultiplier[CSW_XM1014			]	= 1.25;
		fWpnXPMultiplier[CSW_M3				]	= 1.5;

		fWpnXPMultiplier[CSW_MP5NAVY		]	= 1.0;
		fWpnXPMultiplier[CSW_UMP45			]	= 1.25;
		fWpnXPMultiplier[CSW_P90			]	= 1.25;
		fWpnXPMultiplier[CSW_TMP			]	= 1.5;
		fWpnXPMultiplier[CSW_MAC10			]	= 1.5;
		fWpnXPMultiplier[CSW_GALIL			]	= 1.15;
		fWpnXPMultiplier[CSW_FAMAS			]	= 1.15;

		fWpnXPMultiplier[CSW_AWP			]	= 1.0;
		fWpnXPMultiplier[CSW_M4A1			]	= 1.0;
		fWpnXPMultiplier[CSW_AK47			]	= 1.0;
		fWpnXPMultiplier[CSW_AUG			]	= 1.0;
		fWpnXPMultiplier[CSW_SG552			]	= 1.0;
		fWpnXPMultiplier[CSW_G3SG1			]	= 1.0;
		fWpnXPMultiplier[CSW_SG550			]	= 1.0;
		fWpnXPMultiplier[CSW_M249			]	= 1.25;
		fWpnXPMultiplier[CSW_SCOUT			]	= 3.0;

		fWpnXPMultiplier[CSW_HEGRENADE		]	= 1.75;
		fWpnXPMultiplier[CSW_KNIFE			]	= 6.0;

		fWpnXPMultiplier[CSW_C4				]	= 2.0;
		fWpnXPMultiplier[CSW_SMOKEGRENADE	]	= 1.0;
		fWpnXPMultiplier[CSW_FLASHBANG		]	= 1.0;
	}

	fWpnXPMultiplier[CSW_WORLDSPAWN		]	= 0.0;
	fWpnXPMultiplier[CSW_LIGHTNING		]	= 1.0;
	fWpnXPMultiplier[CSW_SUICIDE		]	= 1.0;
	fWpnXPMultiplier[CSW_IMMOLATE		]	= 1.0;
	fWpnXPMultiplier[CSW_LOCUSTS		]	= 1.0;
	fWpnXPMultiplier[CSW_SERPENTWARD	]	= 1.0;
	fWpnXPMultiplier[CSW_SHADOW			]	= 1.0;	
	fWpnXPMultiplier[CSW_THORNS			]	= 1.0;
	fWpnXPMultiplier[CSW_CARAPACE		]	= 1.0;
	fWpnXPMultiplier[CSW_CARRION		]	= 1.0;
	fWpnXPMultiplier[CSW_ORB			]	= 1.0;
	fWpnXPMultiplier[CSW_CONCOCTION		]	= 1.0;
	fWpnXPMultiplier[CSW_BANISH			]	= 1.0;
	fWpnXPMultiplier[CSW_BLIZZARD		]	= 1.0;
	fWpnXPMultiplier[CSW_ICE_SPLINTERS	]	= 1.0;
	fWpnXPMultiplier[CSW_CHAM_FLAME_STRIKE	]	= 1.0;
	fWpnXPMultiplier[CSW_DEATHCOIL	]		= 2.0; //sm4
	
}

// Reset the user's XP to 0
public XP_Reset(idUser)
{

	p_data[idUser][P_LEVEL]		= 0;
	p_data[idUser][P_XP]		= 0;
	SM_ResetSkillLevels( idUser );

	// Save after resetting the user's XP
	DB_SaveXP( idUser, true );

	WC3_ShowBar( idUser );

	format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
			LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
			LANG_PLAYER,"YOUR_XP_HAS_BEEN_RESET");
				
	cssbColoredPrint(idUser, szMessage);		

	// Reset the skills...

	SHARED_INVIS_Set( idUser );

	// Undead's Unholy Aura
	SHARED_SetGravity( idUser );

	// Set the user's speed
	SHARED_SetSpeed( idUser );

	return PLUGIN_CONTINUE;
}	

// Function from war3x thanks ryan!!!
public XP_GetAdminFlag()
{

    new szFlags[24];
    get_pcvar_string( CVAR_wc3_admin_flag, szFlags, 23 );

    return ( read_flags( szFlags ) );
}

stock XP_Give( idUser, iBonusXP )
{

	if ( !WC3_Check() || !idUser )
	{
		return 0;
	}

	// Make sure we have the minimum amount of players
	if ( !XP_MinPlayers() )
	{
		return 0;
	}

	// Bonus calculated by:
	// Bonus XP * (lvl of player/float(MAX_LEVELS) + 1.0)
	// I.E. if Player is level MAX_LEVELS, then it will be Bonus XP * 2.0
	
	if ( iBonusXP != 0 )
	{
		new Float:fCurrentLevel = float( p_data[idUser][P_LEVEL] );
		new Float:iLevelMultiplier = ( fCurrentLevel / float(MAX_LEVELS) ) + 1.0;
		
		new iRealBonusXP = floatround(iLevelMultiplier * iBonusXP);

		//Для talisman +
		// Intellect
		if (ITEM_Has(idUser, ITEM_INTELLECT ) > ITEM_NONE || fWorkTalismanPassive(idUser, arrTalismanActive[idUser]) == TALISMAN_ITEM_INTELLECT)
		{	
			iRealBonusXP = floatround(iLevelMultiplier * iBonusXP * BONUS_INTELLECT);

			if(!is_user_bot(idUser))
			{
				new szShowMessageXPBonus[256];
				format(szShowMessageXPBonus, charsmax(szShowMessageXPBonus), "+%d*XP: %d",BONUS_INTELLECT,iRealBonusXP);
				//fShowMsgPickUpItem(idUser,szShowMessageXPBonus,205,92,92);
				fShowMessagePickUpItem(idUser,szShowMessageXPBonus,205,92,92,enOther);
				
			}
		}

		//Если игрок имеет флаг 'p'
		if((get_user_flags(idUser) & fBonusXP()))
			p_data[idUser][P_XP] += floatround(iRealBonusXP * get_pcvar_float( CVAR_wc3_bonus_xp ));
		else
			p_data[idUser][P_XP] += iRealBonusXP;

		XP_Check( idUser );

		return iRealBonusXP;
	}

	return 0;
}


stock XP_GetLevelByXP( iXP )
{
    if ( iXP < 0 || iXP == 0 )
     return 0;

    new Float:fXPMult = get_pcvar_float( CVAR_wc3_xp_multiplier );

    new iResultLevel = 0;
    for (new iLevel = 0; iLevel <= MAX_LEVELS; iLevel++ )
    {
     if ( get_pcvar_num( CVAR_wc3_save_xp ) == 1 )
     {
      if(iXP >= floatround(iXPLevelSaved[iLevel] * fXPMult ))
       iResultLevel = iLevel;
     }

     if ( get_pcvar_num( CVAR_wc3_save_xp ) == 0 )
     {
      if(iXP >= floatround(iXPLevelShortTerm[iLevel] * fXPMult ))
       iResultLevel = iLevel;
     }

    }

    return iResultLevel;
}


