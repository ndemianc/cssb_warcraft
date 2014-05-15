

public MOTD_ItemsInfo( idUser, iStart )
{

	static szTmp[256], szTmp2[256], pos, i;
	pos = 0;

	// Add header
	pos += formatex(szTmpMsg[pos], 2047-pos, "<html>");
	pos += formatex(szTmpMsg[pos], 2047-pos, "<meta http-equiv=^"Content-Type^" content=^"text/html; charset=utf-8^" />");
	pos += formatex( szTmpMsg[pos], 2047-pos, "%s", MOTD_header );

	// Add the item information
	for ( i = iStart; i < MAX_ITEMS_PAGE + iStart; i++ )
	{
		LANG_GetItemInfo( i, idUser, szTmp, 255 );
		LANG_GetItemName( i, idUser, szTmp2, 255 );

		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div idUser='s'>%s</div><br>", szTmp, szTmp2 );
	}

	if ( iStart == 0 )//Äëÿ shopmenu1
		formatex( szTmp, 255, "%L", LANG_PLAYER, "MOTD_TITLE_SHOPMENU" );
	if ( iStart == 9 )//Äëÿ shopmenu2
		formatex( szTmp, 255, "%L", LANG_PLAYER, "MOTD_TITLE_SHOPMENU2" );
	if ( iStart == 18 )//Äëÿ shopmenu3
		formatex( szTmp, 255, "%L", LANG_PLAYER, "MOTD_TITLE_SHOPMENU3" );
	if ( iStart == 27 )//Äëÿ shopmenu4
		formatex( szTmp, 255, "%L", LANG_PLAYER, "MOTD_TITLE_SHOPMENU4" );

	pos += formatex(szTmpMsg[pos], 2047-pos, "</html>");

	show_motd( idUser, szTmpMsg, szTmp );
}

public MOTD_PlayerSkills( idUser, bool:bThroughSay )
{

	new iPlayers[32], iNumPlayers, i;
	get_players( iPlayers, iNumPlayers );

	// Lets find out what races are in use
	new bool:bRaceUsed[MAX_RACES+1] = {false}, iRaceID, iTargetID;
	for ( i = 0; i < iNumPlayers; i++ )
	{
		iTargetID	= iPlayers[i];
		iRaceID		= p_data[iTargetID][P_RACE];
		
		bRaceUsed[iRaceID] = true;
	}
	
	new pos = 0, iTeam, k, szTeamColor[2];

	// Then we have a player, and we're not on the server console
	if ( idUser != 0 && bThroughSay )
	{
		pos += formatex(szTmpMsg[pos], 2047-pos, "<html>");
		pos += formatex(szTmpMsg[pos], 2047-pos, "<meta http-equiv=^"Content-Type^" content=^"text/html; charset=utf-8^" />");
		pos += formatex( szTmpMsg[pos], 2047 - pos, "%s", MOTD_header );
	}

	new szTmpName[64], szTeamName[32];
	// Loop through each race
	for ( iRaceID = 1; iRaceID < get_pcvar_num( CVAR_wc3_races ) + 1; iRaceID++ )
	{

		// Make sure this race was used somewhere!
		if ( bRaceUsed[iRaceID] )
		{

			// Get the race's name
			lang_GetRaceName ( iRaceID, idUser, szTmpName, 63 );

			// Player
			if ( idUser != 0 && bThroughSay )
			{
				pos += formatex( szTmpMsg[pos], 2047 - pos, "<b>%s</b><ul>", szTmpName );
			}

			// Server Console
			else
			{
				if ( idUser == 0 )
				{
					server_print( "**** %s ****", szTmpName );
				}
				else
				{
					console_print( idUser, "**** %s ****", szTmpName );
				}
			}


			// Lets show them per team, T, then CT, then Spectators
			for ( iTeam = TEAM_T; iTeam <= TEAM_CT + 1; iTeam++ )
			{
				
				// Default color
				formatex( szTeamColor, 1, "p" );

				if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
				{
					if ( iTeam == TEAM_T )
					{
						formatex( szTeamColor, 1, "t" );
					}
					else if ( iTeam == TEAM_CT )
					{
						formatex( szTeamColor, 1, "c" );
					}
				}
				
				// Loop through all players
				for ( k = 0; k < iNumPlayers; k++ )
				{
					iTargetID = iPlayers[k];

					
					// Make sure the user has this race and is on the proper team
					if ( p_data[iTargetID][P_RACE] == iRaceID && get_user_team( iTargetID ) == iTeam )
					{

						get_user_name( iTargetID, szTmpName, 63 );
						get_user_team( iTargetID, szTeamName, 31 );
						
						// Player
						if ( idUser != 0 && bThroughSay )
						{
							pos += formatex( szTmpMsg[pos], 2047 - pos, "<li id='%s'>(%d) %s</li>", szTeamColor, p_data[iTargetID][P_LEVEL], szTmpName );
						}

						// Server Console
						else
						{
							if ( idUser == 0 )
							{
								server_print( "  [%s]:%d %s", szTeamName, p_data[iTargetID][P_LEVEL], szTmpName );
							}
							else
							{
								console_print( idUser, "  [%s]:%d %s", szTeamName, p_data[iTargetID][P_LEVEL], szTmpName );
							}
						}
					}
				}//end player loop
			}//end team loop

			// Player
			if ( idUser != 0 && bThroughSay )
			{
				pos += formatex( szTmpMsg[pos], 2047 - pos, "</ul>" );
				pos += formatex(szTmpMsg[pos], 2047-pos, "</html>");
			}
		}
	}//end race loop

	// Player
	if ( idUser != 0 && bThroughSay )
	{
		new szTitle[128];
		formatex( szTitle, 255, "%L", LANG_PLAYER, "PLAYER_SKILLS" );

		show_motd( idUser, szTmpMsg, szTitle );
	}

	
	return;
}

public MOTD_SkillsInfo( idUser )
{

	static iSkillID, bool:bHeaderShown, pos;
	new szTitle[128], szTmpDesc[256], szSkillName[128], szRaceName[64];

	pos = 0;

	pos += formatex(  szTmpMsg[pos], 2047-pos, "<html>");
	pos += formatex(  szTmpMsg[pos], 2047-pos, "<meta http-equiv=^"Content-Type^" content=^"text/html; charset=utf-8^" />");

	pos += formatex( szTmpMsg[pos], 2047-pos, "%s", MOTD_header );

	// Valid race found!
	if ( SM_IsValidRace( p_data[idUser][P_RACE] ) )
	{
		// Get the Race Name
		lang_GetRaceName( p_data[idUser][P_RACE], idUser, szRaceName, 63 );

		// format the title
		pos += formatex( szTmpMsg[pos], 2047-pos, "<div idUser=^"title^">%s</div><br><br>", szRaceName );
		
		// Format the MOTD title
		formatex( szTitle, 255, "%s %L", szRaceName, LANG_PLAYER, "WORD_INFORMATION" );
	}

	// No race selected
	else
	{
		pos += formatex( szTmpMsg[pos], 2047-pos, "<div idUser=^"title^">%L</div><br><br>",LANG_PLAYER,"WC3_NO_RACE_SELECT" );

		
		
		// Format the MOTD title
		formatex(szTitle, sizeof(szTitle) - 1, "%L",LANG_PLAYER,"SKILLS_INFORMATION");
	}


	// Lets get the trainable skills first!
	bHeaderShown = false;
	iSkillID = SM_GetSkillOfType( idUser, SKILL_TYPE_TRAINABLE );

	while ( iSkillID != -1 )
	{
		if ( !bHeaderShown )
		{
			pos += formatex( szTmpMsg[pos], 2047-pos, "<h1>%L</h1>", LANG_PLAYER, "WORD_TRAINABLE_SKILLS" );

			bHeaderShown = true;
		}

		LANG_GetSkillName( iSkillID, idUser, szSkillName, 255, 2 )
		LANG_GetSkillInfo( iSkillID, idUser, szTmpDesc, 255 );

		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><ul>%s</ul><br>", szSkillName, szTmpDesc );

		iSkillID = SM_GetSkillOfType( idUser, SKILL_TYPE_TRAINABLE, iSkillID + 1 );
	}

	// Now add the user's ultimate(s)
	bHeaderShown = false;
	iSkillID = SM_GetSkillOfType( idUser, SKILL_TYPE_ULTIMATE );
	while ( iSkillID != -1 )
	{
		if ( !bHeaderShown )
		{
			pos += formatex( szTmpMsg[pos], 2047-pos, "<h1>%L</h1>", LANG_PLAYER, "WORD_ULTIMATE" );

			bHeaderShown = true;
		}

		LANG_GetSkillName( iSkillID, idUser, szSkillName, 255, 3 )
		LANG_GetSkillInfo( iSkillID, idUser, szTmpDesc, 255 );

		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><ul>%s</ul>", szSkillName, szTmpDesc );

		iSkillID = SM_GetSkillOfType( idUser, SKILL_TYPE_ULTIMATE, iSkillID + 1 );
	}

	// Now add the user's passive ability(s)
	bHeaderShown = false;
	iSkillID = SM_GetSkillOfType( idUser, SKILL_TYPE_PASSIVE );
	while ( iSkillID != -1 )
	{
		if ( !bHeaderShown )
		{
			pos += formatex( szTmpMsg[pos], 2047-pos, "<h1>%L</h1>", LANG_PLAYER, "WORD_HERO_ABILITY" );

			bHeaderShown = true;
		}

		LANG_GetSkillName( iSkillID, idUser, szSkillName, 255, 4 )
		LANG_GetSkillInfo( iSkillID, idUser, szTmpDesc, 255 );

		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><ul>%s</ul><br>", szSkillName, szTmpDesc );

		iSkillID = SM_GetSkillOfType( idUser, SKILL_TYPE_PASSIVE, iSkillID + 1 );
	}

	// Add the footer
	pos += formatex( szTmpMsg[pos], 2047-pos, "<br><div>%L</div>", LANG_PLAYER, "MOTD_MORE_INFO");

	pos += formatex(szTmpMsg[pos], 2047-pos, "</html>");

	show_motd( idUser, szTmpMsg, szTitle );

	return;
}

public MOTD_War3help(idUser)
{

	if ( !WC3_Check( idUser ) )
	{
		return;
	}


	// Get the game title
	new szGame[32];
	get_modname( szGame, 31 );

	new pos = 0;

	// Add header
	pos += formatex( szTmpMsg[pos], 2047-pos, "<html>");
	pos += formatex( szTmpMsg[pos], 2047-pos, "<meta http-equiv=^"Content-Type^" content=^"text/html; charset=utf-8^" />");

	pos += formatex( szTmpMsg[pos], 2047-pos, "%s", MOTD_header );

	// Add the content
	pos += formatex( szTmpMsg[pos], 2047-pos, "%L<br>"	, LANG_PLAYER, "MOTD_DEVELOPED_BY" );
	pos += formatex( szTmpMsg[pos], 2047-pos, "%L<br>"	, LANG_PLAYER, "MOTD_CLICK_FOR_LATEST" );
	pos += formatex( szTmpMsg[pos], 2047-pos, "%L<p>"	, LANG_PLAYER, "MOTD_MORE_INFO", szGame );
	pos += formatex( szTmpMsg[pos], 2047-pos, "%L"		, LANG_PLAYER, "WAR3HELP" );

	pos += formatex(szTmpMsg[pos], 2047-pos, "</html>");
	
	// Set up the title
	new szTitle[128];
	formatex( szTitle, 255, "%L", LANG_PLAYER, "WAR3HELP_TITLE" );

	show_motd( idUser, szTmpMsg, szTitle );

	return;
}



public MOTD_XP_Kach(idUser)
{
	if ( !WC3_Check( idUser ) )
		return;

	show_motd(idUser, "addons/amxmodx/configs/war3ft/motd/kach.txt", "++XP");

	return;
}