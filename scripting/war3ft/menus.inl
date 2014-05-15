
public MENU_War3Menu( idUser )
{
	static pos, szMenu[256], keys;
	keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<9);
	pos = 0;

	// Add the title
	pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "%L^n^n", LANG_PLAYER, "MENU_WAR3MENU" );
	
	// Add the actual options to the menu
	pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\w1. %L^n", LANG_PLAYER, "SKILLS_MENU" );
	pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\w2. %L^n", LANG_PLAYER, "RACE_MENU" );
	pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\w3. %L^n", LANG_PLAYER, "ITEM_MENU" );
	pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\y4. %L^n", LANG_PLAYER, "HELP" );
	pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\r5. %L^n", LANG_PLAYER, "ADMIN_MENU_TITLE" );
	pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\w6. %L^n", LANG_PLAYER, "MENU_TALISMAN_MENU_NAME" );//Для talisman +
	pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\w7. %L^n", LANG_PLAYER, "MENU_AUTOBUY_MENU_NAME" );//Для авто закупки предметов +
	pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "^n\d0. %L", LANG_PLAYER, "WORD_EXIT" );

	// Display the menu
	show_menu( idUser, keys, szMenu, -1 );

	return;
}

public _MENU_War3Menu( idUser, key )
{

	if ( !WC3_Check() )
	{
		return PLUGIN_HANDLED;
	}

	switch ( key )
	{
		case 0:	menu_Skill_Options( idUser );
		case 1:	menu_Race_Options( idUser );
		case 2:	menu_Item_Options( idUser );
		case 3:	MOTD_War3help( idUser );
		case 4:	menu_Admin_Options( idUser );
		case 5:	fShowMenuTalismans( idUser ); //Для talisman
		case 6:	fShowMenuAutoBuyBase(idUser); //Для авто закупки предметов
	}
	
	return PLUGIN_HANDLED;
}

public menu_Skill_Options( idUser )
{

	new pos = 0, menu_body[512]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<8)|(1<<9)
	new szMenuItemSelectSkills[64];
	new szMenuItemSkillInfo[64];
	new szMenuItemReselectSkill[64];
	
	format(szMenuItemSelectSkills,charsmax(szMenuItemSelectSkills),"\w%L",LANG_PLAYER,"SELECT_SKILLS")
	format(szMenuItemSkillInfo,charsmax(szMenuItemSkillInfo),"\y%L",LANG_PLAYER,"SKILLS_INFORMATION")
	format(szMenuItemReselectSkill,charsmax(szMenuItemReselectSkill),"\r%L",LANG_PLAYER,"RESELECT_SKILLS")
	
	pos += format(menu_body[pos], charsmax(menu_body)-pos, "%L^n^n",LANG_PLAYER,"MENU_SKILLS_OPTIONS")

	pos += format(menu_body[pos], charsmax(menu_body)-pos, "\w1. %s^n",szMenuItemSelectSkills)
	pos += format(menu_body[pos], charsmax(menu_body)-pos, "\y2. %s^n",szMenuItemSkillInfo)
	pos += format(menu_body[pos], charsmax(menu_body)-pos, "\r3. %s^n",szMenuItemReselectSkill)

	pos += format(menu_body[pos], charsmax(menu_body)-pos, "^n^n\d9. %L",LANG_PLAYER,"BACK_STRING")
	pos += format(menu_body[pos], charsmax(menu_body)-pos, "^n\d0. %L",LANG_PLAYER,"WORD_EXIT")
	show_menu(idUser,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Skill_Options(idUser,key){
	
	if ( !WC3_Check() )
	{
		return PLUGIN_HANDLED;
	}

	switch (key){
		case 0:	MENU_SelectSkill( idUser );
		case 1:	MOTD_SkillsInfo( idUser );
		case 2:	CMD_Handle( idUser, "resetskills", true );
		case 8: MENU_War3Menu(idUser)
		default: return PLUGIN_HANDLED;
	}

	return PLUGIN_HANDLED;
}

public menu_Race_Options(idUser)
{
	new pos = 0, menu_body[512]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<8)|(1<<9)

	new szMenuItemChangeRace[64];
	new szMenuItemShowLevel[64];
	new szMenuItemResetXP[64];
	new szMenuItemShowPlayerSkills[64];

	format(szMenuItemChangeRace,charsmax(szMenuItemChangeRace),"\w%L",LANG_PLAYER,"CHANGE_RACE")
	format(szMenuItemShowLevel,charsmax(szMenuItemShowLevel),"\y%L",LANG_PLAYER,"SHOW_LEVEL")
	format(szMenuItemResetXP,charsmax(szMenuItemResetXP),"\y%L",LANG_PLAYER,"RESET_XP")
	format(szMenuItemShowPlayerSkills,charsmax(szMenuItemShowPlayerSkills),"\r%L",LANG_PLAYER,"SHOW_PLAYER_SKILLS")

	pos += format(menu_body[pos], charsmax(menu_body)-pos, "%L^n^n",LANG_PLAYER,"MENU_RACE_OPTIONS")
		
	pos += format(menu_body[pos], charsmax(menu_body)-pos, "\w1. %s^n",szMenuItemChangeRace)
	pos += format(menu_body[pos], charsmax(menu_body)-pos, "\y2. %s^n",szMenuItemShowLevel)
	pos += format(menu_body[pos], charsmax(menu_body)-pos, "\y3. %s^n",szMenuItemResetXP)		
	pos += format(menu_body[pos], charsmax(menu_body)-pos, "\r4. %s^n",szMenuItemShowPlayerSkills)		

	pos += format(menu_body[pos], charsmax(menu_body)-pos, "^n^n\d9. %L",LANG_PLAYER,"BACK_STRING")
	pos += format(menu_body[pos], charsmax(menu_body)-pos, "^n\d0. %L",LANG_PLAYER,"WORD_EXIT")
	show_menu(idUser,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Race_Options(idUser,key)
{

	if ( !WC3_Check() )
	{
		return PLUGIN_HANDLED;
	}

	switch (key)
	{
		case 0:	WC3_ChangeRaceStart( idUser );
		case 1:	WC3_ShowRaceInfo( idUser );
		case 2:	MENU_ResetXP( idUser );
		case 3:	MOTD_PlayerSkills( idUser, true );
		case 8: MENU_War3Menu(idUser)
		default: return PLUGIN_HANDLED;
	}

	return PLUGIN_HANDLED;
}

public menu_Item_Options(idUser)
{

	new pos = 0, menu_body[512]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

	new szMenuItemShopOption1[64];
	new szMenuItemShopOption2[64];
	new szMenuItemShopOption3[64];
	new szMenuItemShopOption4[64];
	new szMenuItemShopInfo1[64];
	new szMenuItemShopInfo2[64];
	new szMenuItemShopInfo3[64];
	new szMenuItemShopInfo4[64];
	
	formatex(szMenuItemShopOption1,charsmax(szMenuItemShopOption1),"\r%L",LANG_PLAYER,"SHOPMENU1_OPTION")
	formatex(szMenuItemShopOption2,charsmax(szMenuItemShopOption2),"\r%L",LANG_PLAYER,"SHOPMENU2_OPTION")
	formatex(szMenuItemShopOption3,charsmax(szMenuItemShopOption3),"\r%L",LANG_PLAYER,"SHOPMENU3_OPTION")
	formatex(szMenuItemShopOption4,charsmax(szMenuItemShopOption4),"\r%L",LANG_PLAYER,"SHOPMENU4_OPTION")

	formatex(szMenuItemShopInfo1,charsmax(szMenuItemShopInfo1),"\y%L",LANG_PLAYER,"SHOW_SHOPMENU1_INFO")//Для shopmenu1
	formatex(szMenuItemShopInfo2,charsmax(szMenuItemShopInfo2),"\y%L",LANG_PLAYER,"SHOW_SHOPMENU2_INFO")//Для shopmenu2
	formatex(szMenuItemShopInfo3,charsmax(szMenuItemShopInfo3),"\y%L",LANG_PLAYER,"SHOW_SHOPMENU3_INFO")//Для shopmenu3
	formatex(szMenuItemShopInfo4,charsmax(szMenuItemShopInfo4),"\y%L",LANG_PLAYER,"SHOW_SHOPMENU4_INFO")//Для shopmenu4

	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "%L^n^n",LANG_PLAYER,"MENU_ITEM_OPTIONS")
	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\r1. %s^n",szMenuItemShopOption1)
	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\r2. %s^n",szMenuItemShopOption2)
	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\r3. %s^n",szMenuItemShopOption3)
	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\r4. %s^n",szMenuItemShopOption4)

	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\y5. %s^n",szMenuItemShopInfo1)
	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\y6. %s^n",szMenuItemShopInfo2)
	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\y7. %s^n",szMenuItemShopInfo3)
	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\y8. %s^n",szMenuItemShopInfo4)

	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "^n^n\d9. %L",LANG_PLAYER,"BACK_STRING")
	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "^n\d0. %L",LANG_PLAYER,"WORD_EXIT")
	show_menu(idUser,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}


public _menu_Item_Options(idUser,key)
{

	if ( !WC3_Check() )
	{
		return PLUGIN_HANDLED;
	}

	switch (key)
	{
		case 0:	MENU_Shopmenu( idUser, 0 );
		case 1:	MENU_Shopmenu( idUser, 9 );
		case 2:	MENU_Shopmenu( idUser, 18 );
		case 3:	MENU_Shopmenu( idUser, 27 );
		case 4:	MOTD_ItemsInfo( idUser, 0 );
		case 5:	MOTD_ItemsInfo( idUser, 9 );
		case 6:	MOTD_ItemsInfo( idUser, 18 );
		case 7:	MOTD_ItemsInfo( idUser, 27 );
		case 8: MENU_War3Menu(idUser);
		default: return PLUGIN_HANDLED;
	}

	return PLUGIN_HANDLED;
}

public menu_Admin_Options(idUser)
{

    if ( idUser && !( get_user_flags( idUser ) & XP_GetAdminFlag() ) )
	{
		format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"YOU_HAVE_NO_ACCESS");	
				
		client_print( idUser, print_console, szMessage );
		
		return PLUGIN_HANDLED
	}

	new pos = 0, menu_body[512]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<8)|(1<<9)

	new szMenuItemGiveIndXP[64];
	new szMenuItemGiveMultXP[64];
	new szMenuItemSaveAll[64];
	
	format(szMenuItemGiveIndXP,charsmax(szMenuItemGiveIndXP),"\r%L",LANG_PLAYER,"GIVE_IND_XP")
	format(szMenuItemGiveMultXP,charsmax(szMenuItemGiveMultXP),"\r%L",LANG_PLAYER,"GIVE_MULT_XP")
	format(szMenuItemSaveAll,charsmax(szMenuItemSaveAll),"\y%L",LANG_PLAYER,"SAVE_ALL_XP")

	pos += format(menu_body[pos], charsmax(menu_body)-pos, "%L^n^n",LANG_PLAYER,"MENU_ADMIN_MENU")

	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\r1. %s^n",szMenuItemGiveIndXP)
	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\r2. %s^n",szMenuItemGiveMultXP)
	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\y3. %s^n",szMenuItemSaveAll)		

	pos += format(menu_body[pos], charsmax(menu_body)-pos, "^n^n\d9. %L",LANG_PLAYER,"BACK_STRING")
	pos += format(menu_body[pos], charsmax(menu_body)-pos, "^n\d0. %L",LANG_PLAYER,"WORD_EXIT")
	show_menu(idUser,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Admin_Options(idUser,key)
{

	if ( !WC3_Check() )
	{
		return PLUGIN_HANDLED;
	}

	switch (key)
	{
		case 0:
		{
			g_menuOption[idUser] = 1
			g_menuSettings[idUser] = 500
			menu_PlayerXP_Options(idUser,g_menuPosition[idUser] = 0)
		}
		case 1:
		{
			g_menuOption[idUser] = 1
			g_menuSettings[idUser] = 500
			menu_TeamXP_Options(idUser)
		}
		case 2: DB_SaveAll( false );
		case 8: MENU_War3Menu(idUser)
	}

	return PLUGIN_HANDLED;
}

public menu_PlayerXP_Options(idUser,pos)
{
	if (pos < 0)
	{
		menu_Admin_Options(idUser)
		return PLUGIN_CONTINUE
	}

	get_players(g_menuPlayers[idUser],g_menuPlayersNum[idUser])
	new menuBody[512]
	new b = 0
	new i
	new name[32], team[4], title[128], back[16], exitstring[16]
	new start = pos * 7
	if (start >= g_menuPlayersNum[idUser])
		start = pos = g_menuPosition[idUser] = 0
		
	format(title,127,"%L",LANG_PLAYER,"MENU_GIVE_PLAYERS_XP")
	new len = format(menuBody,charsmax(menuBody), "%s\R%d/%d^n\w^n",title,pos+1,(g_menuPlayersNum[idUser] / 7 + ((g_menuPlayersNum[idUser] % 7) ? 1 : 0 )))
	new end = start + 7
	new keys = (1<<9)|(1<<7)

	if (end > g_menuPlayersNum[idUser])
		end = g_menuPlayersNum[idUser]

	for(new a = start; a < end; ++a)
	{
		i = g_menuPlayers[idUser][a]
		get_user_name(i,name,31)
		get_user_team(i,team,3)
		keys |= (1<<b)
		len += format(menuBody[len],charsmax(menuBody)-len,"\w%d. %s^n\w",++b,name)
	}

	format(title,charsmax(title),"%L",LANG_PLAYER,"GIVE")
	len += format(menuBody[len],charsmax(menuBody)-len,"^n8. %s  %d XP^n",title,g_menuSettings[idUser])

	format(back,15,"%L",LANG_PLAYER,"BACK_STRING")

	if (end != g_menuPlayersNum[idUser])
	{
		format(menuBody[len],charsmax(menuBody)-len,"^n9. %L...^n0. %s", LANG_PLAYER,"MORE_STRING", pos ? back : back)
		keys |= (1<<8)
	}
	else
	{
		format(exitstring,charsmax(exitstring),"%L",LANG_PLAYER,"WORD_EXIT")
		format(menuBody[len],charsmax(menuBody)-len,"^n0. %s", pos ? back : exitstring)
	}

	show_menu(idUser,keys,menuBody,-1)
	
	return PLUGIN_CONTINUE

}

public _menu_PlayerXP_Options(idUser,key)
{

	if ( !WC3_Check() )
	{
		return PLUGIN_HANDLED;
	}

	switch(key)
	{
		case 7:
		{
			++g_menuOption[idUser]
			if (g_menuOption[idUser]>6)
			{
				g_menuOption[idUser]=1
			}
			switch(g_menuOption[idUser])
			{
				case 1: g_menuSettings[idUser] = 500
				case 2: g_menuSettings[idUser] = 1000
				case 3: g_menuSettings[idUser] = 5000
				case 4: g_menuSettings[idUser] = 10000
				case 5: g_menuSettings[idUser] = 50000
				case 6: g_menuSettings[idUser] = 100000
			}
			menu_PlayerXP_Options(idUser,g_menuPosition[idUser])
		}
		case 8: menu_PlayerXP_Options(idUser,++g_menuPosition[idUser])
		case 9: return PLUGIN_HANDLED;
		default:
		{
			new idPlayer = g_menuPlayers[idUser][g_menuPosition[idUser] * 7 + key]
			
		//	if((get_user_flags(idPlayer) & ADMIN_FLAG))
		//	{
				format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%d ^x04XP",
					LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
					LANG_PLAYER,"THE_ADMIN_JUST_GAVE_YOU_XP",
					g_menuSettings[idUser]);
				
				cssbColoredPrint(idPlayer, szMessage);	
				
				p_data[idPlayer][P_XP] += g_menuSettings[idUser]

				XP_Check( idPlayer );

				menu_PlayerXP_Options(idUser,g_menuPosition[idUser])
				
				new szUserName[32];
				get_user_name(idPlayer, szUserName, 31 );		
				ADMIN_Log(idUser, "Give XP","%d %s", g_menuSettings[idUser],szUserName);
			//}
		}
	}

	return PLUGIN_HANDLED;
}

public menu_TeamXP_Options(idUser)
{

	new pos = 0, menu_body[512], give[16]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<7)|(1<<8)|(1<<9)

	new szMenuItemTerrorist[64];
	new szMenuItemCT[64];
	new szMenuItemEveryone[64];
	
	format(szMenuItemTerrorist,charsmax(szMenuItemTerrorist),"\y%L",LANG_PLAYER,"TERRORISTS")
	format(szMenuItemCT,charsmax(szMenuItemCT),"\y%L",LANG_PLAYER,"CT")
	format(szMenuItemEveryone,charsmax(szMenuItemEveryone),"\r%L",LANG_PLAYER,"EVERYONE")

	pos += format(menu_body[pos], charsmax(menu_body)-pos, "%L^n^n",LANG_PLAYER,"MENU_TEAM_XP")

	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\y1. %s^n",szMenuItemTerrorist)
	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\y2. %s^n",szMenuItemCT)
	pos += formatex(menu_body[pos], charsmax(menu_body)-pos, "\r3. %s^n",szMenuItemEveryone)				

	format(give,charsmax(give),"\w%L",LANG_PLAYER,"GIVE")
	pos += format(menu_body[pos], charsmax(menu_body)-pos,"^n\w8. %s  %d XP^n",give,g_menuSettings[idUser])
	pos += format(menu_body[pos], charsmax(menu_body)-pos, "^n^n\d9. %L",LANG_PLAYER,"BACK_STRING")
	pos += format(menu_body[pos], charsmax(menu_body)-pos, "^n\d0. %L",LANG_PLAYER,"WORD_EXIT")
	show_menu(idUser,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_TeamXP_Options(idUser,key){

	if ( !WC3_Check() )
	{
		return PLUGIN_HANDLED;
	}

	switch(key)
	{
		case 0:
		{
			ADMIN_GiveXP( idUser, "@T", g_menuSettings[idUser] );
			ADMIN_Log(idUser, "Give XP @T","%d", g_menuSettings[idUser]);
			menu_TeamXP_Options(idUser)
		}
		case 1:
		{
			ADMIN_GiveXP( idUser, "@CT", g_menuSettings[idUser] );
			ADMIN_Log(idUser, "Give XP @CT","%d", g_menuSettings[idUser]);
			menu_TeamXP_Options(idUser)
		}
		case 2:
		{
			ADMIN_GiveXP( idUser, "@ALL", g_menuSettings[idUser] );
			ADMIN_Log(idUser, "Give XP @ALL","%d", g_menuSettings[idUser]);
			menu_TeamXP_Options(idUser)
		}
		case 7:
		{
			++g_menuOption[idUser]
			if (g_menuOption[idUser]>6)
			{
				g_menuOption[idUser]=1
			}
			switch(g_menuOption[idUser])
			{
				case 1: g_menuSettings[idUser] = 500
				case 2: g_menuSettings[idUser] = 1000
				case 3: g_menuSettings[idUser] = 5000
				case 4: g_menuSettings[idUser] = 10000
				case 5: g_menuSettings[idUser] = 50000
				case 6: g_menuSettings[idUser] = 100000
			}
			menu_TeamXP_Options(idUser)
		}
		case 8: menu_Admin_Options(idUser)
	}

	return PLUGIN_HANDLED;
}

public MENU_ResetXP(idUser)
{
	if(!(get_user_flags(idUser) & ADMIN_FLAG))
	{
		format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
		LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
		LANG_PLAYER,"YOU_HAVE_NO_ACCESS");		
		
		client_print( idUser, print_console,szMessage);	
		cssbColoredPrint(idUser, szMessage);
		
		return;
	}

	static szMenu[128];
	new keys = (1<<0)|(1<<1)|(1<<9);

	formatex(szMenu, sizeof(szMenu) - 1, "%L^n^n\w1. %L^n\w2. %L",
	LANG_PLAYER,"MENU_RESET_XP",
	LANG_PLAYER,"WC3_YES",
	LANG_PLAYER,"WC3_NO");
	
	show_menu( idUser, keys, szMenu, -1 );

	return;
}

public _MENU_ResetXP( idUser, key )
{
	
	if ( !WC3_Check() )
	{
		return PLUGIN_HANDLED;
	}

	// User selected yes
	if ( key == 0 )
	{
		XP_Reset( idUser );
	}
	
	return PLUGIN_HANDLED;
}

// Function will display the changerace menu
public MENU_ChangeRace( idUser, iRaceXP[MAX_RACES] )
{
	
	new szRaceName[MAX_RACES+1][64], i, pos, iKeys = 0, szMenu[512], szXP[16];

	// Get our race names
	for ( i = 0; i < get_pcvar_num( CVAR_wc3_races ); i++ )
	{
		lang_GetRaceName( i + 1, idUser, szRaceName[i], 63 );
	}

	pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "%L", LANG_PLAYER, "MENU_SELECT_RACE" );

	// Then add the experience column
	if ( get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\R%L^n^n", LANG_PLAYER, "MENU_WORD_EXPERIENCE" );
	}
	else
	{
		//pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "^n^n" );
		pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "^n" );
	}

	// Build the changerace menu (for every race)
	for ( i = 0; i < get_pcvar_num( CVAR_wc3_races ); i++ )
	{
		new iRaceNum = i;

		num_to_str( iRaceXP[i], szXP, 15 );
		
		// Add the "Select a Hero" message if necessary
		if ( i == 4 )
			pos += format( szMenu[pos], charsmax(szMenu)-pos, "%L", LANG_PLAYER, "SELECT_HERO" );
		
		// User's current race
		if ( i == p_data[idUser][P_RACE] - 1 )
		{
			/*
			if(i == 9)
			{
				pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\d%d. %s\d\R%s^n", 0, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
				iKeys |= (1<<9);
			}
			else
			{
				pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\d%d. %s\d\R%s^n", i + 1, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
				iKeys |= (1<<i);
			}*/

			if(i == 8 && get_pcvar_num(CVAR_wc3_vip_chameleon) == 1 || i == 9 && get_pcvar_num(CVAR_wc3_vip_ice_magician) == 1)
			{
				if(i == 9)
					iRaceNum = -1;

				pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\r%d. %s (VIP)\r\R%s^n", iRaceNum + 1, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
			}
			else
			{
				if(i == 9)
					iRaceNum = -1;

				pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\d%d. %s\d\R%s^n", iRaceNum + 1, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );			
			}

			if(i == 9)
				iKeys |= (1<<9);
			else
				iKeys |= (1<<i);
		}

		// Race the user wants to change to
		else if ( i == p_data[idUser][P_CHANGERACE] - 1 )
		{
			if(i == 9)
			{
				pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\r%d. %s\r\R%s^n", 0, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
				iKeys |= (1<<9);
			}
			else
			{
				pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\r%d. %s\r\R%s^n", i + 1, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
				iKeys |= (1<<i);
			}
		}

		// All other cases
		else
		{
			/*
			new iRaceLimit = get_pcvar_num( CVAR_wc3_race_limit );
			new bool:bAllowRace = true;

			if ( iRaceLimit > 0 )
			{
				new iTotal[MAX_RACES];

				// Get how many people are using each race
				new iPlayers[32], iNumPlayers, i, iTarget;
				get_players( iPlayers, iNumPlayers );

				for ( i = 0; i < iNumPlayers; i++ )
				{
					iTarget = iPlayers[i];

					if ( iTarget != idUser && p_data[iTarget][P_RACE] > 0 && p_data[iTarget][P_RACE] <= get_pcvar_num( CVAR_wc3_races ) )
					{
						iTotal[p_data[iTarget][P_RACE]]++;
					}
				}
				
				// Now if we have more races selected than iRaceLimit provides us with, then we need to increase iRaceLimit
				while ( HLPR_TotalUsingRaces( iTotal ) > iRaceLimit * get_playersnum() )
				{
					iRaceLimit++;
				}

				// Check to see if there was an increase that was necessary
				if ( iRaceLimit > get_pcvar_num( CVAR_wc3_race_limit ) )
				{
					WC3_Log( true, "Error, increase wc3_race_limit to at least %d", iRaceLimit );
				}

				if ( iTotal[i+1] >= iRaceLimit )
				{
					bAllowRace = false;

				}
			}*/

			new bool:bAllowRace = true;

			// Check to see if the user can choose this race (are there too many of this race?)
			if ( bAllowRace )
			{/*
				if(i == 9)
				{
					pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\w%d. %s\y\R%s^n", 0, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
					iKeys |= (1<<9);
				}
				else
				{
					pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\w%d. %s\y\R%s^n", i + 1, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
					iKeys |= (1<<i);
				}*/

				if(i == 8 && get_pcvar_num(CVAR_wc3_vip_chameleon) == 1 || i == 9 && get_pcvar_num(CVAR_wc3_vip_ice_magician) == 1)
				{
					if(i == 9)
						iRaceNum = -1;

					pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\r%d. %s (VIP)\r\R%s^n", iRaceNum + 1, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
				}
				else
				{

					if(i == 9)
						iRaceNum = -1;

					pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\w%d. %s\y\R%s^n", iRaceNum + 1, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
				}

				if(i == 9)
					iKeys |= (1<<9);
				else
					iKeys |= (1<<i);
			}

			// If not, display the race, but don't give them a key to press
			else
			{
				if(i == 9)
					iRaceNum = -1;

				pos += formatex( szMenu[pos], charsmax(szMenu)-pos, "\d%d. %s\y\R%s^n", iRaceNum + 1, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
			}
		}

	}

	/*iKeys |= (1<<i);
	
	// This is needed so we can make the Auto-Select option "0" if the number of races is 9
	if ( get_pcvar_num( CVAR_wc3_races ) == 10 )
	{
		i = -1;
	}

	pos += format( szMenu[pos], charsmax(szMenu)-pos, "%L", idUser, "SELECT_RACE_FOOTER", i + 1 );
	
	// Add a cancel button to the bottom
	if ( get_pcvar_num( CVAR_wc3_races ) != 9 )
	{
		iKeys |= (1<<9);

		pos += format( szMenu[pos], charsmax(szMenu)-pos, "^n\d0. %L", idUser, "WORD_CANCEL" );
	}
	*/
	
	// Show the menu to the user!
	show_menu( idUser, iKeys, szMenu, -1 );

	return;
}

/*HLPR_TotalUsingRaces( iTotalRaces[MAX_RACES] )
{
	new iTotal = 0;
	for ( new i = 1; i <= get_pcvar_num( CVAR_wc3_races ); i++ )
	{
		WC3_Log( true, "%d", i );
		iTotal += iTotalRaces[i];
	}

	return iTotal;
}*/


public _MENU_ChangeRace( idUser, key )
{
	/*
	key == 0 - Undead Scourge
	key == 1 - Human Alliance
	key == 2 - Orcish Horde
	key == 3 - Night Elves of Kalimdor
	key == 4 - Blood Mage
	key == 5 - Shadow Hunter
	key == 6 - Warden
	key == 7 - Crypt Lord
	key == 8 - Chameleon
	key == 9 - Frost Mage
	*/

//	client_print(0,print_chat,"key: %d",key);

	if ( !WC3_Check() )
	{
		return PLUGIN_HANDLED;
	}
	
	// User pressed 0 (cancel)
	if ( get_pcvar_num( CVAR_wc3_races ) < 10 && key - 1 == get_pcvar_num( CVAR_wc3_races ) )
	{
		return PLUGIN_HANDLED;
	}

	// Save the current race data before we change
	DB_SaveXP( idUser, false );

	new iRace, iAutoSelectKey = KEY_0;
	
	if ( get_pcvar_num( CVAR_wc3_races ) != 10 )
	{
//		iAutoSelectKey = get_pcvar_num( CVAR_wc3_races )
	}
	
	// Auto select a race
	if ( key == iAutoSelectKey )
	{
//		iRace = random_num( 1, get_pcvar_num( CVAR_wc3_races ) );
	}

	// Otherwise race is set
	else
	{
	//	iRace = key + 1;
	}

	iRace = key + 1;


//Расы для V.I.P.
	if(iRace == 9 && get_pcvar_num(CVAR_wc3_vip_chameleon) == 1)
	{
	   if(!(get_user_flags(idUser) & fVipChameleon()))
	   {
			format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
			LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
			LANG_PLAYER,"YOU_HAVE_NO_ACCESS");    
			  
			client_print( idUser, print_console,szMessage);   
			cssbColoredPrint(idUser, szMessage);
			  
			// Get the race's name
			new szRaceName[64];
			lang_GetRaceName( iRace, idUser, szRaceName, 63 );

			show_motd(idUser, "addons/amxmodx/configs/war3ft/motd/vip_chameleon.txt", szRaceName);

			WC3_ChangeRaceStart(idUser);

			return PLUGIN_HANDLED;
	   }
	}

	if(iRace == 10 && get_pcvar_num(CVAR_wc3_vip_ice_magician) == 1)
	{
	   if(!(get_user_flags(idUser) & fVipIceMagician()))
	   {
			format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
			LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
			LANG_PLAYER,"YOU_HAVE_NO_ACCESS");    
			  
			client_print( idUser, print_console,szMessage);   
			cssbColoredPrint(idUser, szMessage);
			 
			// Get the race's name
			new szRaceName[64];
			lang_GetRaceName( iRace, idUser, szRaceName, 63 );

			show_motd(idUser, "addons/amxmodx/configs/war3ft/motd/vip_ice_magician.txt", szRaceName);

			WC3_ChangeRaceStart( idUser );

			return PLUGIN_HANDLED;
	   }
	}


	// User currently has a race
	if ( p_data[idUser][P_RACE] != 0 )
	{

		// Change the user's race at the start of next round
		if ( iRace != p_data[idUser][P_RACE] )
		{
			
			// Special message for csdm
			if ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 )
			{
				format(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_MENU_CHANGE_RACE");
				client_print( idUser, print_center,szMessage);
				
			}	
			else
			{
				format(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CENTER_CHANGED_NEXT");
				client_print( idUser, print_center,szMessage);				
			}

			p_data[idUser][P_CHANGERACE] = iRace;
		}

		// Do nothing
		else
		{
			p_data[idUser][P_CHANGERACE] = 0;
		}
	}

	// User doesn't have a race so give it to him!!!
	else
	{
		WC3_SetRace( idUser, iRace );
	}

	return PLUGIN_HANDLED;
}

public MENU_ReplaceItem( idUser )
{

	new szMenu[512] = "", pos = 0;
	new iKeys = (1<<9)|(1<<0)|(1<<1)|(1<<2);

	// Add the menu header
	pos += format( szMenu[pos], charsmax(szMenu)-pos, "%L^n^n", LANG_PLAYER, "MENU_REPLACE_ITEM" );

	new szItemName[64], szItemName2[64],szItemName3[64];
	LANG_GetItemName( g_iShopMenuItems[idUser][ITEM_SLOT_ONE], idUser, szItemName, charsmax(szItemName) );
	LANG_GetItemName( g_iShopMenuItems[idUser][ITEM_SLOT_TWO], idUser, szItemName2, charsmax(szItemName2) );
	LANG_GetItemName( g_iShopMenuItems[idUser][ITEM_SLOT_THREE], idUser, szItemName3, charsmax(szItemName3) );

	// Add the items
	pos += format( szMenu[pos], charsmax(szMenu)-pos, "\y1. %s^n", szItemName );
	pos += format( szMenu[pos], charsmax(szMenu)-pos, "\y2. %s^n", szItemName2 );
	pos += format( szMenu[pos], charsmax(szMenu)-pos, "\y3. %s^n", szItemName3 );

	// Add the exit option
	pos += format( szMenu[pos], charsmax(szMenu)-pos, "^n\d0. %L", LANG_PLAYER, "WORD_EXIT" );

	// Show the menu
	show_menu( idUser, iKeys, szMenu, -1 );

	return;
}

public _menu_ReplaceItem( idUser, iKey )
{
	if ( !WC3_Check() || iKey == 9 )
	{
		return PLUGIN_HANDLED;
	}

	// Remove item from item slot one
	if ( iKey == 0 )
	{
		ITEM_Remove( idUser, ITEM_SLOT_ONE )
	}

	// Remove item from itemslot two
	if ( iKey == 1 )
	{
		ITEM_Remove( idUser, ITEM_SLOT_TWO )
	}
	// Remove item from itemslot two
	if ( iKey == 2 )
	{
		ITEM_Remove( idUser, ITEM_SLOT_THREE )
	}

	// Then the user typed "rings"
	if ( g_iFutureItem[idUser] == -3 )
	{
		ITEM_BuyRings( idUser );
	}
	else
	{
		ITEM_Buy( idUser, g_iFutureItem[idUser] );
	}

	return PLUGIN_HANDLED;
}

//Меню магазинов
public MENU_Shopmenu( idUser, iStart )
{

	if ( !ITEM_MenuCanBuyCheck( idUser ) )
	{
		return;
	}

	// Shopmenu2 can't display if we don't have the correct number of races!
	if ( iStart == MAX_PAGE_ITEMS && get_pcvar_num( CVAR_wc3_races ) <= 4 )
	{
		return;
	}

	new szMenu[512], szItemName[64], pos = 0, i, iItemID;
	new iKeys = (1<<9);

	// Add the header
	if ( iStart == 0 )
	{
		pos += format( szMenu[pos], charsmax(szMenu)-pos, "%L", LANG_PLAYER, "MENU_BUY_ITEM" );
	}

	// "Shopmenu 2"
	if ( iStart == MAX_PAGE_ITEMS )
	{
		pos += format( szMenu[pos], charsmax(szMenu)-pos, "%L", LANG_PLAYER, "MENU_BUY_ITEM2" );
	}
	// "Shopmenu 3"
	if ( iStart == 18 )
	{
		pos += format( szMenu[pos], charsmax(szMenu)-pos, "%L", LANG_PLAYER, "MENU_BUY_ITEM3" );
	}
	//Для shopmenu4
	if ( iStart == 27 )
	{
		pos += format( szMenu[pos], charsmax(szMenu)-pos, "%L", LANG_PLAYER, "MENU_BUY_ITEM4" );
	}
	
	// Lets add the items to the menu!
	for ( i = 0; i < MAX_PAGE_ITEMS; i++ )
	{
		iItemID = iStart + i;

		LANG_GetItemName( iItemID, idUser, szItemName,charsmax(szItemName));

		new iItemMoney = ITEM_Cost( idUser, iItemID );
		new iUserMoney = SHARED_GetUserMoney( idUser );

		if(iUserMoney < iItemMoney)
			pos += format( szMenu[pos], charsmax(szMenu)-pos, "\d%d. %s\R%d^n", i + 1, szItemName, ITEM_Cost( idUser, iItemID ) );
		else
			pos += format( szMenu[pos], charsmax(szMenu)-pos, "\y%d. %s\r\R%d^n", i + 1, szItemName, ITEM_Cost( idUser, iItemID ) );

		iKeys |= (1<<i);
	}

	pos += format( szMenu[pos], charsmax(szMenu)-pos, "^n\d0. %L", LANG_PLAYER, "WORD_EXIT" );

	show_menu( idUser, iKeys, szMenu, -1 );
}
//Для shopmenu1
public _MENU_Shopmenu1( idUser, iKey )
{
	if ( !WC3_Check() || iKey == 9 )
	{
		return PLUGIN_HANDLED;
	}

	new iItemMoney = ITEM_Cost( idUser, iKey );
	new iUserMoney = SHARED_GetUserMoney( idUser );

	if(iUserMoney < iItemMoney)
		MENU_Shopmenu( idUser, 0 );


	ITEM_Buy( idUser, iKey );

	return PLUGIN_HANDLED;
}
//Для shopmenu2
public _MENU_Shopmenu2( idUser, iKey )
{
	if ( !WC3_Check() || iKey == 9 )
	{
		return PLUGIN_HANDLED;
	}

	iKey += MAX_PAGE_ITEMS;
	
	new iItemMoney = ITEM_Cost( idUser, iKey );
	new iUserMoney = SHARED_GetUserMoney( idUser );

	if(iUserMoney < iItemMoney)
		MENU_Shopmenu( idUser, 9 );


	ITEM_Buy( idUser, iKey );

	return PLUGIN_HANDLED;
}

//Для shopmenu3
public _MENU_Shopmenu3( idUser, iKey )
{
	if ( !WC3_Check() || iKey == 9 )
		return PLUGIN_HANDLED;

	iKey += 18;
	
	new iItemMoney = ITEM_Cost( idUser, iKey );
	new iUserMoney = SHARED_GetUserMoney( idUser );

	if(iUserMoney < iItemMoney)
		MENU_Shopmenu( idUser, 18 );


	ITEM_Buy( idUser, iKey );

	return PLUGIN_HANDLED;
}

//Для shopmenu4
public _MENU_Shopmenu4( idUser, iKey )
{
	if ( !WC3_Check() || iKey == 9 )
		return PLUGIN_HANDLED;

	iKey += 27;

	new iItemMoney = ITEM_Cost( idUser, iKey );
	new iUserMoney = SHARED_GetUserMoney( idUser );

	if(iUserMoney < iItemMoney)
		MENU_Shopmenu( idUser, 27 );


	ITEM_Buy( idUser, iKey );

	return PLUGIN_HANDLED;
}

public MENU_SelectSkill( idUser )
{

	// User has no race, how can we select skills?!?
	if ( p_data[idUser][P_RACE] == 0 )
	{
		//set_hudmessage(200, 100, 0, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 2)
		WC3_StatusText( idUser, TXT_TOP_CENTER, "%L", LANG_PLAYER, "SELECT_RACE_BEFORE_SKILLS" );

		return;
	}

	// They don't choose skills when it's random
	else if ( p_data[idUser][P_RACE] == RACE_CHAMELEON && get_pcvar_num( CVAR_wc3_cham_random ) )
	{
		//WC3_StatusText( idUser, TXT_TOP_CENTER, "%s", "Chameleons can't select skills!" );
		CHAM_ConfigureSkills( idUser );

		return;
	}

	// Lets make sure the user has some available skill points
	new iSkillsUsed = SM_TotalSkillPointsUsed( idUser );
	if ( iSkillsUsed >= p_data[idUser][P_LEVEL] )
	{

		//set_hudmessage(200, 100, 0, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 2)
		WC3_StatusText( idUser, TXT_TOP_CENTER, "%L", LANG_PLAYER, "ALREADY_SELECTED_SKILL_POINTS" );

		return;
	}


//-----------------------------------------------------------------------

	// Bots don't need a menu now do they??
	if ( p_data[idUser][P_RACE] == RACE_CHAMELEON || is_user_bot( idUser ))
	{
		// No race has been selected yet!!
		if ( !SM_SkillAvailable( idUser ) )
		{
			return;
		}
		
		// Keep giving the bot a random ID until we are full!
		while ( iSkillsUsed < p_data[idUser][P_LEVEL] )
		{
			SM_GiveRandomSkillPoint( idUser );

			iSkillsUsed = SM_TotalSkillPointsUsed( idUser );
		}

		return;
	}

//--------------------------------------------------------------------------
	// OK set up a menu!!!

	new szMsg[512], pos = 0, szSkillName[64];
	new iSkillCounter = 0, iSkillID = 0, iKeys = (1<<9), iSkillLevel;

	
	// Add the menu header
	pos += formatex( szMsg[pos], charsmax(szMsg)-pos, "%L", LANG_PLAYER, "MENU_SELECT_SKILL" );

	iSkillID = SM_GetSkillByPos( idUser, iSkillCounter );

	while ( iSkillID != -1 )
	{
		iSkillLevel = SM_GetSkillLevel( idUser, iSkillID, 4 );

		LANG_GetSkillName( iSkillID , idUser, szSkillName, 63, 1 );
		
		// Add the trainable skills to the menu
		if ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_TRAINABLE )
		{

			// Only add it to the menu if they don't have level 3 already!
			if ( iSkillLevel < MAX_SKILL_LEVEL )
			{

				// User isn't high enough of a level to select this skill yet
				if ( p_data[idUser][P_LEVEL] <= 2 * iSkillLevel )
				{
					pos += formatex( szMsg[pos], charsmax(szMsg)-pos, "\d" );
				}

				// Then the user can choose it!
				else
				{
					iKeys |= (1<<iSkillCounter);
				}

				pos += formatex( szMsg[pos], charsmax(szMsg)-pos, "^n%d. %s %L %d\w", iSkillCounter+1, szSkillName, LANG_PLAYER, "WORD_LEVEL", iSkillLevel + 1 );
			}
		}
		
		// Add the ultimate to the menu
		else if ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_ULTIMATE )
		{

			if ( iSkillLevel < MAX_ULTIMATE_LEVEL )
			{
				// User can't choose ultimate yet :/
				if ( p_data[idUser][P_LEVEL] <= 5 )
				{
					pos += formatex( szMsg[pos], charsmax(szMsg)-pos, "\d" );
				}

				// Then the user is level 6 or above and can select their ultimate!
				else
				{
					iKeys |= (1<<iSkillCounter);
				}

				pos += formatex( szMsg[pos], charsmax(szMsg)-pos, "^n%d. %L: %s\w", iSkillCounter+1, LANG_PLAYER, "WORD_ULTIMATE", szSkillName );
			}
		}

		iSkillCounter++;
		iSkillID = SM_GetSkillByPos( idUser, iSkillCounter );
	}

	// Add the cancel button to the menu
	pos += formatex( szMsg[pos], charsmax(szMsg)-pos, "^n^n\d0. %L", LANG_PLAYER, "WORD_CANCEL" );

	// Show the menu!
	show_menu( idUser, iKeys, szMsg, -1 );

	return;
}

public _MENU_SelectSkill( idUser, iKey )
{

	if ( !WC3_Check() || iKey == 9 )
	{
		return PLUGIN_HANDLED;
	}

	// Determine which key was just selected
	new iSkillID = SM_GetSkillByPos( idUser, iKey );

	// Set up the skill!
	SM_SetSkill( idUser, iSkillID );
	
	new iSkillsUsed = SM_TotalSkillPointsUsed( idUser );
	
	// Then they have another skill to select!!
	if ( iSkillsUsed < p_data[idUser][P_LEVEL] )
	{
		MENU_SelectSkill( idUser );
	}

	// No more, lets show them their latest level/XP
	else
	{
		WC3_ShowBar( idUser );
	}

	return PLUGIN_HANDLED;
}
