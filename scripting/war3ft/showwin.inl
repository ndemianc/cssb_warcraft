

//Отображение победителя в конце раунда
public fShowWinnPlayer(iTeamWin)
{
	if(get_pcvar_num(CVAR_wc3_show_winner) != 1)
		return;

	new arrPlayersID[32],iPlayersNum, iMaxDmgId = 0, iMaxKillsId = 0;
	new szMessageBest[256], iLengthText;
	new szMessageBestDmg[256];
	new szMaxDmgName[32], szMaxKillsName[32];
	
//	get_players(arrPlayersID, iPlayersNum, "c");
	get_players(arrPlayersID, iPlayersNum);
	
	for (new iStats = 0; iStats < iPlayersNum; iStats++)
	{
		if (arrStatsInfo[arrPlayersID[iStats]][ME_DMG] > arrStatsInfo[arrPlayersID[iMaxDmgId]][ME_DMG])
			iMaxDmgId = iStats;
		if (arrStatsInfo[arrPlayersID[iStats]][ME_KILLS] > arrStatsInfo[arrPlayersID[iMaxKillsId]][ME_KILLS])
			iMaxKillsId = iStats;
	}

	get_user_name(arrPlayersID[iMaxKillsId], szMaxKillsName, 31);
	get_user_name(arrPlayersID[iMaxDmgId], szMaxDmgName, 31);
			
	// Get the target's race name
	new szShortRaceNameMaxKills[32];
	lang_GetRaceName(p_data[arrPlayersID[iMaxKillsId]][P_RACE], arrPlayersID[iMaxKillsId], szShortRaceNameMaxKills,31,true);

	new szShortRaceNameMaxDmg[32];
	lang_GetRaceName( p_data[arrPlayersID[iMaxDmgId]][P_RACE], arrPlayersID[iMaxDmgId], szShortRaceNameMaxDmg,31,true);

	if (arrStatsInfo[arrPlayersID[iMaxKillsId]][ME_KILLS] ||
		arrStatsInfo[arrPlayersID[iMaxDmgId]][ME_DMG])
	{
		for (new i = 0; i < iPlayersNum; i++)
		{
			iLengthText = format(szMessageBest, charsmax(szMessageBest), "%L^n", arrPlayersID[i], "WC3_BEST_TITLE");

			if (arrStatsInfo[arrPlayersID[iMaxKillsId]][ME_KILLS])
				iLengthText += format(szMessageBest[iLengthText], charsmax(szMessageBest) - iLengthText, 
				"^n%L: %L: %s %L: %d",LANG_PLAYER, "WC3_BEST_KILLS",
										szMaxKillsName,
										arrStatsInfo[arrPlayersID[iMaxKillsId]][ME_KILLS],
										LANG_PLAYER, "WC3_RACE",
										szShortRaceNameMaxKills,
										LANG_PLAYER, "WORD_LEVEL",
										p_data[arrPlayersID[iMaxKillsId]][P_LEVEL]);
			
			if (arrStatsInfo[arrPlayersID[iMaxDmgId]][ME_DMG])
					format(szMessageBestDmg, charsmax(szMessageBestDmg), 
					"^n%L: %L: %s %L: %d",LANG_PLAYER, "WC3_BEST_DMG",
											szMaxDmgName,
											arrStatsInfo[arrPlayersID[iMaxDmgId]][ME_DMG],
											LANG_PLAYER, "WC3_RACE",
											szShortRaceNameMaxDmg,
											LANG_PLAYER, "WORD_LEVEL",
											p_data[arrPlayersID[iMaxDmgId]][P_LEVEL]);

			/*
			if (arrStatsInfo[arrPlayersID[iMaxKillsId]][ME_KILLS])
				iLengthText += format(szMessageBest[iLengthText], charsmax(szMessageBest) - iLengthText, 
				"^n%L: |%L: %s|%L: %d",	LANG_PLAYER, "WC3_BEST_KILLS",
										szMaxKillsName,
										arrStatsInfo[arrPlayersID[iMaxKillsId]][ME_KILLS],
										LANG_PLAYER, "WC3_RACE",
										szShortRaceNameMaxKills,
										LANG_PLAYER, "WORD_LEVEL",
										p_data[arrPlayersID[iMaxKillsId]][P_LEVEL]);
			
			if (arrStatsInfo[arrPlayersID[iMaxDmgId]][ME_DMG])
				iLengthText += format(szMessageBest[iLengthText], charsmax(szMessageBest) - iLengthText, 
					"^n%L: |%L: %s|%L: %d",	LANG_PLAYER, "WC3_BEST_DMG",
											szMaxDmgName,
											arrStatsInfo[arrPlayersID[iMaxDmgId]][ME_DMG],
											LANG_PLAYER, "WC3_RACE",
											szShortRaceNameMaxDmg,
											LANG_PLAYER, "WORD_LEVEL",
											p_data[arrPlayersID[iMaxDmgId]][P_LEVEL]);
*/
			if(iTeamWin == TEAM_T)
			{
				set_dhudmessage(205,0,0,0.05, 0.55, 0, 10.0, 7.0, 2.0, 3.0, false);
				show_dhudmessage(arrPlayersID[i], szMessageBest);

				set_dhudmessage(205,0,0,0.05, 0.63, 0, 10.0, 7.0, 2.0, 3.0, false);
				show_dhudmessage(arrPlayersID[i], szMessageBestDmg);
			}
			if(iTeamWin == TEAM_CT)
			{
				set_dhudmessage(58, 95, 205 , 0.05, 0.55, 0, 10.0, 7.0, 2.0, 3.0, false);
				show_dhudmessage(arrPlayersID[i], szMessageBest);

				set_dhudmessage(58, 95, 205 ,0.05, 0.63, 0, 10.0, 7.0, 2.0, 3.0, false);
				show_dhudmessage(arrPlayersID[i], szMessageBestDmg);
			}

			
		}
	}

	
}

//Форварды
//Отображение победителя в конце раунда
public fwHamKilled(idUser, idAttacker, shouldgib)
{
	if (is_user_alive(idAttacker) && get_pcvar_num(CVAR_wc3_show_winner) == 1)
	{
		if (is_user_connected(idAttacker))
			arrStatsInfo[idAttacker][ME_KILLS]++;
	}

	p_data_b[idUser][PB_DIEDLASTROUND] = true;

	sm4_4_2_hamPlayerKilled(idUser);

}

public fwRoundRespawn(idUser)
{
	if(get_pcvar_num(CVAR_wc3_show_winner) == 1)
	{
		for (new i = 0; i < ME_NUM; i++)
			arrStatsInfo[idUser][i] = 0;
	}
	
}

public fwTraceAttack(idUser, idAttacker, Float:damage, Float:direction[3], traceresult, damagebits)
{
	if (is_user_alive(idAttacker) && get_pcvar_num(CVAR_wc3_show_winner) == 1)
	{
		new hit = get_tr2(traceresult, TR_iHitgroup);
		arrStatsInfo[idAttacker][ME_DMG] += floatround(damage);
		arrStatsInfo[idAttacker][ME_HIT] = hit;
	}
}