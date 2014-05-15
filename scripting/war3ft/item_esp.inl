

public fEnableItemESP(idUser)
{
	if(task_exists(idUser + TASK_ESP))
		remove_task(idUser + TASK_ESP);

	bIsUserESP[idUser] = true;
	set_task(FREQUENCY_ESP, "fESPLoop", idUser + TASK_ESP, _, _, "b");
	
}

public fDisableItemESP(idUser)
{
	bIsUserESP[idUser] = false;

	if(task_exists(idUser + TASK_ESP))
		remove_task(idUser + TASK_ESP);
}

public fESPLoop(idUser)
{	
	if (idUser >= TASK_ESP )
			idUser -= TASK_ESP;

	if(!is_user_connected(idUser) || !is_user_alive(idUser) || !bIsUserESP[idUser]) 
	{
		if(task_exists(idUser + TASK_ESP))
			remove_task(idUser + TASK_ESP);

		return PLUGIN_CONTINUE;
	}
	
	new arrPlayers[32], iNum;
	get_players(arrPlayers, iNum, "a");
	
	for(new i = 0; i < iNum; ++i)
	{
		new iLoopIdPlayer = arrPlayers[i];
		new iTeamCurrentUser = get_user_team(idUser);
		new iTeamLoopUser = get_user_team(iLoopIdPlayer);

		if(iTeamCurrentUser == iTeamLoopUser || idUser == iLoopIdPlayer) 
			continue;

		if(!p_data[iLoopIdPlayer][P_RACE] || p_data[iLoopIdPlayer][P_RACE] != RACE_HUMAN)
			continue;

		new iOrigin[3];
		get_user_origin(iLoopIdPlayer, iOrigin, 0);
			
		message_begin(MSG_ONE, SVC_TEMPENTITY, iOrigin, idUser);
		write_byte(21);
		write_coord(iOrigin[0]);
		write_coord(iOrigin[1]);
		write_coord(iOrigin[2] - 35);
		write_coord(iOrigin[0]);
		write_coord(iOrigin[1]);
		write_coord(iOrigin[2] + RADIUS_ESP);
		write_short(idSpriteItemWhite);
		write_byte(0);
		write_byte(1);
		write_byte(6); 
		write_byte(8);
		write_byte(1);
		write_byte(100);
		write_byte(100);
		write_byte(255);
		write_byte(192);
		write_byte(0);
		message_end();
	}

	return PLUGIN_CONTINUE;
}