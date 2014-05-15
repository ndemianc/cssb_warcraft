

//-----------------Базовые функции------------------------------------BEGIN {
//Инициализация переменных
public fInitTalisman()//добавлена в war3ft.inl (public WC3_Init())
{
	fTimeSpawnTalismanGlobal = get_pcvar_float( CVAR_wc3_gl_time_spawn_talisman );
	
}


//Обнуление данных когда игрок подключается на сервер
public fPlayerInitTalisman(idUser) //добавлена в war3ft.inl ( WC3_PlayerInit(idUser) )
{
	arrTalismanActive[idUser] = TALISMAN_ITEM_NONE;
	arrTalismanChange[idUser] = TALISMAN_ITEM_NONE;
	arrTalismanNum[idUser] = 0;

	arrPlayerInitTalisman[idUser] = false;	//Загрузка всех талисманов когда игрок зашел на сервер и появилось меню выбора рас (грузится один раз)
	arrTalismanNecklaceNum[idUser] = 0;     // Массив игроков для количества зарядов иммунитета от ульты (Ожерелье)
	arrTalismanHelmNum[idUser] = 0;			// Массив игроков для количества зарядов  (Шлем)
}

//Присвоение данных при респе игрока
public fPostSpawnTalisman(idUser)//добавлена в war3ft.inl ( WC3_PostSpawn(idUser) )
{
	arrTalismanNecklaceNum[idUser] = TALISMAN_ITEM_NECKLACE_NUM;
	arrTalismanHelmNum[idUser] = TALISMAN_ITEM_HELM_NUM;
}

//Когда игрок умирает 
public fTalismanUserDied(idUser)//добавлена в war3ft.inl ( public WC3_Death( idVictim, iKiller, iWeaponID, iHeadshot ) )
{
	if(arrTalismanNecklaceNum[idUser] > 0) 
		arrTalismanNecklaceNum[idUser] = 0;

	if(arrTalismanHelmNum[idUser] > 0) 
		arrTalismanHelmNum[idUser] = 0;
}

//Удаление зарядов
public fTalismanRemoveCharge(idUser)//добавлена в items.inl ( ITEM_RemoveCharge( idUser, iItem ) )
{
	//Ожерелье
	if (arrTalismanNecklaceNum[idUser] > 0 && ITEM_Has( idUser, ITEM_NECKLACE ) == ITEM_NONE) 
	{ 
		arrTalismanNecklaceNum[idUser] -= CHARGE_DISPOSE; 

		if (arrTalismanNecklaceNum[idUser] <= 0 ) 
			arrTalismanNecklaceNum[idUser] = 0; 
	}

	//Шлем
	if (arrTalismanHelmNum[idUser] > 0 && ITEM_Has( idUser, ITEM_HELM ) == ITEM_NONE) 
	{ 
		arrTalismanHelmNum[idUser] -= CHARGE_DISPOSE; 

		if (arrTalismanHelmNum[idUser] <= 0 ) 
			arrTalismanHelmNum[idUser] = 0; 
	}
}

//Выводит кто подобрал талисман и какой для всех игроков кроме подобравшего
public fPrintChatTalismanFound(idUserPickUp,szItemName[])
{
	new szPlayerTouchName[32]
	get_user_name(idUserPickUp, szPlayerTouchName, charsmax(szPlayerTouchName));
	formatex(szMessage, charsmax(szMessage), "%L",LANG_PLAYER,"CLIENT_PRINT_TALISMAN_PICKUP",szPlayerTouchName,szItemName);

	new arrPlayers[32], iNumPlayers;
	get_players( arrPlayers, iNumPlayers );
	for (new i = 0; i < iNumPlayers; i++ )
	{
		new idPlayer = arrPlayers[i];
		
		if(idPlayer == idUserPickUp)
			continue;

		cssbChatColor(idPlayer,"%s%s",fTagWar3ft(),szMessage);
	}

	//Лог кто подобрал и какой талисман
	fTalismanPlayerPickUpLog(idUserPickUp,szItemName);
}

//Считывание всех талисманов игроков из базы данных на выбор рас
public fTalismanInit(idUser)
{
	arrTalismanActive[idUser] = TALISMAN_ITEM_NONE;
	arrTalismanChange[idUser] = TALISMAN_ITEM_NONE;
	arrTalismanNum[idUser] = 0;

	if(MAX_MODE_TALISMAN == 1)
	{
		for(new i = 0; i < MAX_TALISMANS; i++)
			arrListTalisman[idUser][i] = TALISMAN_ITEM_NONE;

		//Считываем из базы данных все талисманы игрока
		DB_GetAllTalisman(idUser);
	}
	else
	{
		//Присвоение всех талисманов игроку
		for(new i = 0; i < MAX_TALISMANS; i++)
			arrListTalisman[idUser][i] = LIST_TALISMANS[i];
	}
}

//Считывание файла респаунов талисмана для конкретной карты
fReadMapSpawnsTalisman()//добавлена в war3ft.inl (public WC3_Init())
{
	new szMap[32], szConfig[32],  szMapFile[256];
	
	get_mapname(szMap, charsmax(szMap));
	get_configsdir(szConfig, charsmax(szConfig));
	format(szMapFile, charsmax(szMapFile), "%s\war3ft\talisman\%s.talisman_spawns.cfg",szConfig, szMap);
	
	iTotalSpawnsTalisman = 0;
	
	if (file_exists(szMapFile)) 
	{
		new szDataFile[124]; 
		new	iLength;
		new iLine = 0;
		new szPos[4][8];
    		
		while(iTotalSpawnsTalisman < MAX_SPAWNS_TALISMAN && (iLine = read_file(szMapFile , iLine , szDataFile ,charsmax(szDataFile), iLength) ) != 0 ) 
		{
			if (strlen(szDataFile) < 2) continue;

			parse(szDataFile, szPos[1], 7, szPos[2], 7, szPos[3], 7);
			
			//Координаты
			arrSpawnOrigin[iTotalSpawnsTalisman][0] = str_to_num(szPos[1]);
			arrSpawnOrigin[iTotalSpawnsTalisman][1] = str_to_num(szPos[2]);
			arrSpawnOrigin[iTotalSpawnsTalisman][2] = str_to_num(szPos[3]);
					
			iTotalSpawnsTalisman++;
		}
	}

	log_amx("Total Spawns Talisman: %d | Map: %s",iTotalSpawnsTalisman,szMap);
}

//Отсчет времени каждые 10 минут (600 секунд)
public taskCounterLoopTalisman()//добавлена в war3ft.inl (public WC3_Init())
{
	if(MAX_MODE_TALISMAN != 1)
		return;

	if(iTotalSpawnsTalisman <= 0)//Если нет респаунов для талисмана
		return;

	new iNumPlayers = get_playersnum();
	if (iNumPlayers < get_pcvar_num( CVAR_wc3_pl_talisman_respawn ) )
		return;

	fTimeSpawnTalismanGlobal += TIME_TASK_SPAWN_TALISMAN;
	set_pcvar_float(CVAR_wc3_gl_time_spawn_talisman,fTimeSpawnTalismanGlobal);

	//client_print(0,print_chat,"TimeSoawnTalismanGlobal: %f",fTimeSpawnTalismanGlobal);	

	if(fTimeSpawnTalismanGlobal >= TIME_SPAWN_TALISMAN)
	{
		fTimeSpawnTalismanGlobal = 0.0;
		set_pcvar_float(CVAR_wc3_gl_time_spawn_talisman,fTimeSpawnTalismanGlobal);

		fCreateTalisman();
	}
}

//Создание модели талисмана
public fCreateTalisman()
{
	if(MAX_MODE_TALISMAN != 1)
		return -1;

	new entTalisman = engfunc(EngFunc_CreateNamedEntity, engfunc(EngFunc_AllocString, "info_target"));
	if (!entTalisman) return -1;
	
	new iSelectPoint = random_num(0,iTotalSpawnsTalisman-1);

	new Float:origin[3];
	arrSpawnOrigin[iSelectPoint][2] -= 15;
	IVecFVec(arrSpawnOrigin[iSelectPoint],origin);//перевод целого вектора в float
	
	set_pev(entTalisman, pev_classname, szClassNameTalisman);
	set_pev(entTalisman, pev_nextthink, get_gametime() + 5.0); 
	set_pev(entTalisman, pev_solid, SOLID_TRIGGER);
//	set_pev(entTalisman, pev_solid, SOLID_SLIDEBOX);
	set_pev(entTalisman, pev_movetype, MOVETYPE_FLY);
//	set_pev(entTalisman,pev_movetype,MOVETYPE_NOCLIP);
	set_pev(entTalisman, pev_sequence, 1);
//	set_pev(entTalisman, pev_flags, pev(entTalisman, pev_flags) & FL_ONGROUND);
	set_pev(entTalisman, pev_frame, 0);
	//set_pev(entTalisman, pev_gravity, 0.5);
	//set_pev(entTalisman, pev_friction, 0.0);
	set_pev(entTalisman,pev_velocity,Float:{0.0,0.0,0.0});
	engfunc(EngFunc_SetModel, entTalisman, szModelTalisman);

	origin[2] -= 25;
	set_pev(entTalisman, pev_origin, origin);

	emit_sound(entTalisman, CHAN_VOICE, g_szSounds[SOUND_REINCARNATION], VOL_NORM, ATTN_NORM, 0, PITCH_NORM);

	//Запись логов
	fTalismanLog(arrSpawnOrigin[iSelectPoint][0],arrSpawnOrigin[iSelectPoint][1],arrSpawnOrigin[iSelectPoint][2]);

	//Сообщение о респауне нового талисмана
	new szShowMsgTalisman[256];
	format(szShowMsgTalisman, charsmax(szShowMsgTalisman), "%L",LANG_PLAYER,"WC3_MSG_RESPAWN_TALISMAN");
	set_dhudmessage(119,136,153,-1.0,-1.0,2,0.0,2.0,0.02,0.02, false);
	show_dhudmessage(0, szShowMsgTalisman);


	return -1;
}
//Вращение талисмана
//register_think("war3ft_talisman", "Talisman_Think");
public Talisman_Think(const entTalisman)//добавлена в cssbWar3ftMod.sma
{
    if(!pev_valid(entTalisman))
        return; 

	static Float:floatvector[3];
	floatvector[0] = 0.0;
	floatvector[1] = 25.0;
	floatvector[2] = 0.0;

	set_pev(entTalisman,pev_avelocity,floatvector);//Задает скорость вращения вокруг своей оси
    set_pev(entTalisman, pev_nextthink, get_gametime() + 5.0);   
}
//Когда игрок прикасается к талисману
public touchPlayerTalisman(idEntityTouched, idUserTouch)//добавлена в loot.inl (public player_Touch(idEntityTouched, idUserTouch))
{

	if (!pev_valid(idEntityTouched) || !is_user_alive(idUserTouch))
		return FMRES_IGNORED;
	
	new classname[32];
	pev(idEntityTouched, pev_classname, classname, sizeof classname - 1);
	
	if (equali(classname, "war3ft_talisman"))
	{
		//Достигнут максимум найденных талисманов 25
		if(arrTalismanNum[idUserTouch] >= MAX_TALISMANS)
		{
			formatex(szMessage, charsmax(szMessage), "%L",LANG_PLAYER,"CLIENT_PRINT_MAX_TALISMANS",MAX_TALISMANS);
			fShowSyncMessageRandomColor(idUserTouch,szMessage,0,0.05, 0.45);

			arrTalismanNum[idUserTouch] = MAX_TALISMANS;

			return FMRES_IGNORED;
		}

		new iSelect = random_num(0,MAX_TALISMANS-1);

		if(!bIsPlayerPresenceTalisman(idUserTouch,LIST_TALISMANS[iSelect]))//Если нет в наличии талисмана
		{
			if (is_valid_ent(idEntityTouched)) 
				engfunc(EngFunc_RemoveEntity, idEntityTouched);

			arrListTalisman[idUserTouch][iSelect] = LIST_TALISMANS[iSelect];

			new szItemName[32];
			LANG_GetItemName(LIST_TALISMANS[iSelect], idUserTouch, szItemName, 31, true );

			formatex(szMessage, charsmax(szMessage), "%L",LANG_PLAYER,"CLIENT_PRINT_TALISMAN_FOUND",szItemName);
			cssbChatColor(idUserTouch,"%s%s",fTagWar3ft(),szMessage);

			fPrintChatTalismanFound(idUserTouch,szItemName);

			arrTalismanNum[idUserTouch]++;

			//Отображаем меню с талисманами при подборе
			fShowMenuTalismans(idUserTouch);
		}
		else //Если есть такой талисман
		{
			
			new iNewTalisman = TALISMAN_ITEM_NONE;
			//Поиск талисмана которого нет у игрока
			for(new i = 0; i < MAX_TALISMANS; i++)
			{
				if(arrListTalisman[idUserTouch][i] == TALISMAN_ITEM_NONE)
				{
					iNewTalisman = LIST_TALISMANS[i];
					arrListTalisman[idUserTouch][i] = iNewTalisman;
					break;
				}
			}

			if(iNewTalisman != TALISMAN_ITEM_NONE)
			{
				if (is_valid_ent(idEntityTouched)) 
					engfunc(EngFunc_RemoveEntity, idEntityTouched);

				new szItemName[32];
				LANG_GetItemName(iNewTalisman, idUserTouch, szItemName, 31, true );

				formatex(szMessage, charsmax(szMessage), "%L 2",LANG_PLAYER,"CLIENT_PRINT_TALISMAN_FOUND",szItemName);
				cssbChatColor(idUserTouch,"%s%s",fTagWar3ft(),szMessage);

				fPrintChatTalismanFound(idUserTouch,szItemName);

				arrTalismanNum[idUserTouch]++;

				//Отображаем меню с талисманами при подборе
				fShowMenuTalismans(idUserTouch);
			}

		}

	}

	return FMRES_IGNORED;
}

//Проверка на наличие талисмана у игрока
bool:bIsPlayerPresenceTalisman(idUser,iTalismanValue)
{
	for(new i = 0; i < MAX_TALISMANS; i++)
	{
		if(arrListTalisman[idUser][i] == iTalismanValue)
			return true;
	}

	return false;
}

// Функция проверяющая доступ к меню талисманов
public fVipTalisman()
{
    new szFlags[24];
    get_pcvar_string(CVAR_wc3_talisman_flag, szFlags, 23 );

    return ( read_flags( szFlags ) );
}

//Проверка на существование талисмана
bool:bIsTalisman(iTalismanValue)
{
	for(new i = 0; i < MAX_TALISMANS; i++)
	{
		if(LIST_TALISMANS[i] == iTalismanValue)
			return true;
	}

	return false;
}
//=============================================================================
//=============================================================================
//=============================================================================
//-----------------Базовые функции-----------------------------------------------END }


//-----------------Функции меню-----------------------BEGIN {
//Вызов отображения меню
fShowMenuTalismans(idUser)
{
	
	if(MAX_MODE_TALISMAN == 1 && arrTalismanNum[idUser] == 0)
	{
		formatex(szMessage, charsmax(szMessage), "%L",LANG_PLAYER,"CLIENT_PRINT_TALISMAN_NO");
		cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		return;
	}

	if(MAX_MODE_TALISMAN == 3)
	{
		if(!(get_user_flags(idUser) & fVipTalisman()))
		{
			formatex(szMessage, charsmax(szMessage), "%L",LANG_PLAYER,"CLIENT_PRINT_TALISMAN_VIP");
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
			return;
		}
	}

	new iNumPlayers = get_playersnum();
	if (iNumPlayers < get_pcvar_num( CVAR_wc3_pl_talisman_activate ) )
	{
		formatex(szMessage, charsmax(szMessage), "%L",LANG_PLAYER,"CLIENT_PRINT_TALISMAN_MAX_PL_ACTIVATE",get_pcvar_num( CVAR_wc3_pl_talisman_activate ));
		cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		return;
	}

	//Присвоение всех талисманов игроку если у игрока максимальное количество талисманов
	if(arrTalismanNum[idUser] >= MAX_TALISMANS)
	{
		for(new i = 0; i < MAX_TALISMANS; i++)
			arrListTalisman[idUser][i] = LIST_TALISMANS[i];
	}

	new idMenu,szBuffer[64],szMenuData[256],szTalismanData[256];
	
	//Название главного меню
	formatex(szMenuData, charsmax(szMenuData), "\y%L", LANG_PLAYER, "MENU_TALISMAN_MENU_NAME");
	idMenu = menu_create(szMenuData, "_Callback_MENU_Talisman");
	
	// Добавление пунктов в меню
	for(new i = 0; i < MAX_TALISMANS; i++)
	{
		if(arrListTalisman[idUser][i] == TALISMAN_ITEM_NONE)
			continue;

		new szItemName[32];
		LANG_GetItemName(arrListTalisman[idUser][i], idUser, szItemName, 31, true );
		
		formatex(szTalismanData, charsmax(szTalismanData), "%L '\y%s'",LANG_PLAYER,"WC3_KEY_TALISMAN", szItemName);
		formatex(szBuffer, charsmax(szBuffer), "%d", arrListTalisman[idUser][i]);

		menu_additem(idMenu,szTalismanData, szBuffer,0,-1);
	}
	
	// Если нет предметов в меню
	if (menu_items(idMenu) <= 0)
	{
		menu_destroy(idMenu);
		return;
	}
			
	//Пункты меню (назад - следующий - выход)
	formatex(szMenuData, charsmax(szMenuData), "\d%L", LANG_PLAYER, "WC3_MENU_BACK");
	menu_setprop(idMenu, MPROP_BACKNAME, szMenuData);
	formatex(szMenuData, charsmax(szMenuData), "\d%L", LANG_PLAYER, "WC3_MENU_NEXT");
	menu_setprop(idMenu, MPROP_NEXTNAME, szMenuData);
	formatex(szMenuData, charsmax(szMenuData), "\d%L", LANG_PLAYER, "WC3_MENU_EXIT");
	menu_setprop(idMenu, MPROP_EXITNAME, szMenuData);
	
	menu_display(idUser, idMenu);
	
}


//Функция выбора пунктов в меню выбора рас		
public _Callback_MENU_Talisman(idUser, idMenu, idItem)
{
	if ( !WC3_Check() )
		return PLUGIN_HANDLED;

	// Меню будет закрыто
	if (idItem == MENU_EXIT)
	{
		menu_destroy(idMenu);
		return PLUGIN_HANDLED;
	}
	
	static _access, szInfoMenu[8], callback;
	menu_item_getinfo(idMenu, idItem, _access, szInfoMenu, sizeof(szInfoMenu) - 1, _, _, callback);
	
	new iNumberTalisman = str_to_num(szInfoMenu);
//	client_print(0,print_chat,"iNumberTalisman: %d |idItem: %d",iNumberTalisman,idItem);

	new szItemName[32];
	LANG_GetItemName(iNumberTalisman, idUser, szItemName, 31, true );

	//Проверка заблокирован ли предмет на карте
	if(isBlockedTalismanMap(iNumberTalisman))
	{
		formatex(szMessage, charsmax(szMessage), "%L",LANG_PLAYER,"CLIENT_PRINT_TALISMAN_DISABLED_ON_MAP",szItemName);
		cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);

		menu_destroy(idMenu);

		return PLUGIN_HANDLED;
	}

	//Если есть талисман
	if(arrTalismanActive[idUser] != TALISMAN_ITEM_NONE)
	{
		arrTalismanChange[idUser] = iNumberTalisman;
		
		formatex(szMessage, charsmax(szMessage), "%L",LANG_PLAYER,"CLIENT_PRINT_TALISMAN_CHANGE",szItemName);
		cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
	}
	else
	{
		//Присваиваем активный талисман
		arrTalismanActive[idUser] = iNumberTalisman;
		
		fActivateTalismanPassive(idUser, iNumberTalisman);

		formatex(szMessage, charsmax(szMessage), "%L",LANG_PLAYER,"CLIENT_PRINT_TALISMAN_ACTIVATE",szItemName);
		cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);

		WC3_ShowBar(idUser);
	}

	menu_destroy(idMenu);

	return PLUGIN_HANDLED;
}

bool:isBlockedTalismanMap(iNumberTalisman)
{
	if(iNumberTalisman == TALISMAN_ITEM_GLOVES && g_bGlovesDisabled)
		return true;
	else if(iNumberTalisman == TALISMAN_ITEM_DEVOURER_LIGHT && bDisabledDevourerLight)
		return true;
	else if(iNumberTalisman == TALISMAN_ITEM_ICE_BLOCK && bDisabledIceBlock)
		return true;
	else if(iNumberTalisman == TALISMAN_ITEM_THIEF && bDisabledThief)
		return true;
	else if(iNumberTalisman == TALISMAN_ITEM_MIRROR_ULTIMATE && bDisabledMirrorUltimate)
		return true;

	return false;
}

//=============================================================================
//=============================================================================
//=============================================================================
//-----------------Функции меню-----------------------END }


//-----------------Функции баз данных MYSQL и SQLITE------------------------------------BEGIN {
//Считывание всех талисманов из базы данных (базовая функция)
public DB_GetAllTalisman( idUser )//добавлена в war3ft.inl (WC3_PlayerInit( idUser ))
{
	//Включено сохранение или нет
	if (!get_pcvar_num( CVAR_wc3_save_xp ) || !idUser || MAX_MODE_TALISMAN != 1)
		return;
	
	// Get the Talisman
	switch( g_DBType )
	{
		case DB_MYSQLX:	MYSQLX_GetAllTalisman(idUser);
		case DB_SQLITE:	SQLITE_GetAllTalisman(idUser);
	}

	return;
}

//Считывание из базы данных MYSQL всех талисманов
MYSQLX_GetAllTalisman(idUser)
{
	if ( !MYSQLX_Connection_Available() )
		return;
	
	new iUniqueID = DB_GetUniqueID( idUser );

	if ( iUniqueID <= 0 )
	{
		WC3_Log( true, "[ERROR] Unable to retreive user's Unique ID" );
		return;
	}

	//Запрос количества талиманов для игрока
	new szQueryNumTalisman[256];
	format(szQueryNumTalisman, charsmax(szQueryNumTalisman), "SELECT `talisman_num` FROM `wc3_num_talisman` WHERE (`player_id` = '%d' );", iUniqueID );
	new Handle:hQueryNumTalisman = SQL_PrepareQuery(g_DBConn, szQueryNumTalisman );

	if ( !SQL_Execute( hQueryNumTalisman ) )
	{
		MYSQLX_Error(hQueryNumTalisman, szQueryNumTalisman, 100 );
		return;
	}
	
	new iNumTalismansUser = 0;
	// Цикл считывания из поля 'talisman_num'
	while ( SQL_MoreResults( hQueryNumTalisman ) )
	{
		iNumTalismansUser = SQL_ReadResult( hQueryNumTalisman, 0 );

		SQL_NextRow( hQueryNumTalisman );
	}
	//log_amx("iNumTalismansUser: %d",SQL_MoreResults( hQueryNumTalisman ) );

	//Если у игрока талисамнов максимальное количество не нужно считывать из базы чтобы снизить нагрузку
	if(iNumTalismansUser >= MAX_TALISMANS)
	{
		arrTalismanNum[idUser] = iNumTalismansUser;
		return;
	}

	SQL_FreeHandle( hQueryNumTalisman );

	//Запрос всех талисманов для игрока
	new szQuery[256];
	format(szQuery, 255, "SELECT `talisman_id` FROM `wc3_player_talisman` WHERE (`player_id` = '%d' );", iUniqueID );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error( query, szQuery, 6 );
		return;
	}

	// Обнуляем все талисманы
	for ( new i = 0; i < MAX_TALISMANS; i++ )
		arrListTalisman[idUser][i] = TALISMAN_ITEM_NONE;

	new iTalismanID,iCounter = 0;

	// Цикл считывания из поля 'talisman_id'
	while ( SQL_MoreResults( query ) )
	{
		iTalismanID	= SQL_ReadResult( query, 0 );
		
		if ( iCounter < MAX_TALISMANS )
		{
			for(new i = 0; i < MAX_TALISMANS; i++)
			{
				if(LIST_TALISMANS[i] == iTalismanID)
				{
					arrListTalisman[idUser][i] = LIST_TALISMANS[i];

					arrTalismanNum[idUser]++;
					break;
				}
			}
		}

		SQL_NextRow( query );
		iCounter++;
	}

	SQL_FreeHandle( query );

//	client_print(0,print_chat,"TalismanID MYSQLX_GetAllTalisman");
	
	return;
}

fTalisman_MYSQLX_Save( idUser,iUniqueID )//добавлена в db_sqlite.inl (MYSQLX_Save( idUser ))
{
//	client_print(0,print_chat,"TalismanID MYSQLX_Save:");

	//Удаление записи по ID игрока
	new szQueryDeleteTalisman[256];
	format(szQueryDeleteTalisman, charsmax(szQueryDeleteTalisman), "DELETE FROM `wc3_num_talisman` WHERE (`player_id` = '%d' );",iUniqueID);
	new Handle:hQueryDeleteTalisman = SQL_PrepareQuery(g_DBConn, szQueryDeleteTalisman );

	if ( !SQL_Execute( hQueryDeleteTalisman ) )
	{
		MYSQLX_Error(hQueryDeleteTalisman, szQueryDeleteTalisman, 104 );
		return;
	}
	//SQL_FreeHandle( hQueryDeleteTalisman );

	//Сохранение количества талиманов игрока
	new szQueryNumTalisman[256];
	format(szQueryNumTalisman, charsmax(szQueryNumTalisman), "REPLACE INTO `wc3_num_talisman` ( `player_id` , `talisman_num`) VALUES ( '%d', '%d' );", iUniqueID, arrTalismanNum[idUser]);
	new Handle:hQueryNumTalisman = SQL_PrepareQuery(g_DBConn, szQueryNumTalisman );

	if ( !SQL_Execute( hQueryNumTalisman ) )
	{
		MYSQLX_Error(hQueryNumTalisman, szQueryNumTalisman, 102 );
		return;
	}
//	SQL_FreeHandle( hQueryNumTalisman );

	//Если у игрока талисманов максимальное количество не нужно записывать в базу все талисманы чтобы снизить нагрузку
	if(arrTalismanNum[idUser] >= MAX_TALISMANS)
		return;

	//Сохранение всех талисманов
	for ( new iTalismanID = 0; iTalismanID < MAX_TALISMANS; iTalismanID++ )
	{
		if(arrListTalisman[idUser][iTalismanID] == TALISMAN_ITEM_NONE)
			continue;

		new iTalismanValue = arrListTalisman[idUser][iTalismanID];

		new szQuery[512];
		format( szQuery, 511, "REPLACE INTO `wc3_player_talisman` ( `player_id` , `talisman_id`) VALUES ( '%d', '%d' );", iUniqueID, iTalismanValue);
		new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szQuery, 5 );
			return;
		}
	}

}

fTalisman_MYSQLX_Save_T( idUser,iUniqueID )//добавлена в db_sqlite.inl (MYSQLX_Save_T( idUser ))
{
//	client_print(0,print_chat,"TalismanID MYSQLX_Save_T:");

	//Удаление записи по ID игрока
	new szQueryDeleteTalisman[256];
	format(szQueryDeleteTalisman, charsmax(szQueryDeleteTalisman), "DELETE FROM `wc3_num_talisman` WHERE (`player_id` = '%d' );",iUniqueID);
	SQL_ThreadQuery(g_DBTuple,"_MYSQLX_Save_T", szQueryDeleteTalisman );

	//Сохранение количества талиманов игрока
	new szQueryNumTalisman[256];
	format(szQueryNumTalisman, charsmax(szQueryNumTalisman), "REPLACE INTO `wc3_num_talisman` ( `player_id` , `talisman_num`) VALUES ( '%d', '%d' );", iUniqueID, arrTalismanNum[idUser]);
	SQL_ThreadQuery(g_DBTuple,"_MYSQLX_Save_T", szQueryNumTalisman );

	//Если у игрока талисманов максимальное количество не нужно записывать в базу все талисманы, чтобы снизить нагрузку
	if(arrTalismanNum[idUser] >= MAX_TALISMANS)
		return;

	//Сохранение всех талисманов
	for ( new iTalismanID = 0; iTalismanID < MAX_TALISMANS; iTalismanID++ )
	{
		if(arrListTalisman[idUser][iTalismanID] == TALISMAN_ITEM_NONE)
			continue;

		new iTalismanValue = arrListTalisman[idUser][iTalismanID];

		new szQuery[512];
		format( szQuery, 511, "REPLACE INTO `wc3_player_talisman` ( `player_id` , `talisman_id`) VALUES ( '%d', '%d' );", iUniqueID, iTalismanValue);
		SQL_ThreadQuery( g_DBTuple, "_MYSQLX_Save_T", szQuery );
	}
}


//Считывание из базы данных SQLITE всех талисманов
SQLITE_GetAllTalisman(idUser)
{
	if ( !SQLITE_Connection_Available() )
		return;
	
	new iUniqueID = DB_GetUniqueID( idUser );

	if ( iUniqueID <= 0 )
	{
		WC3_Log( true, "[ERROR] Unable to retreive user's Unique ID" );
		return;
	}

	//Запрос количества талисманов для игрока
	new szQueryNumTalisman[256];
	format(szQueryNumTalisman, charsmax(szQueryNumTalisman), "SELECT `talisman_num` FROM `wc3_num_talisman` WHERE (`player_id` = '%d' );", iUniqueID );
	new Handle:hQueryNumTalisman = SQL_PrepareQuery(g_DBConn, szQueryNumTalisman );

	if ( !SQL_Execute( hQueryNumTalisman ) )
	{
		SQLITE_Error(hQueryNumTalisman, szQueryNumTalisman, 100 );
		return;
	}
	
	new iNumTalismansUser = 0;
	// Цикл считывания из поля 'talisman_num'
	while ( SQL_MoreResults( hQueryNumTalisman ) )
	{
		iNumTalismansUser = SQL_ReadResult( hQueryNumTalisman, 0 );

		SQL_NextRow( hQueryNumTalisman );
	}

	SQL_FreeHandle( hQueryNumTalisman );
	
	//Если у игрока талисманов максимальное количество не нужно считывать из базы чтобы снизить нагрузку
	if(iNumTalismansUser == MAX_TALISMANS)
	{
		arrTalismanNum[idUser] = iNumTalismansUser;
		return;
	}

	//Запрос всех талисманов для игрока
	new szQuery[256];
	format(szQuery, charsmax(szQuery), "SELECT `talisman_id` FROM `wc3_player_talisman` WHERE (`player_id` = '%d' );", iUniqueID );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, szQuery, 101 );
		return;
	}

	// Обнуляем все талисманы
	for ( new i = 0; i < MAX_TALISMANS; i++ )
		arrListTalisman[idUser][i] = TALISMAN_ITEM_NONE;

	new iTalismanID,iCounter = 0;

	// Цикл считывания из поля 'talisman_id'
	while ( SQL_MoreResults( query ) )
	{
		iTalismanID	= SQL_ReadResult( query, 0 );
		
		if ( iCounter < MAX_TALISMANS )
		{
			for(new i = 0; i < MAX_TALISMANS; i++)
			{
				if(LIST_TALISMANS[i] == iTalismanID)
				{
					arrListTalisman[idUser][i] = LIST_TALISMANS[i];

					arrTalismanNum[idUser]++;
					break;
				}
			}
		}

		SQL_NextRow( query );
		iCounter++;
	}

	SQL_FreeHandle( query );

//	client_print(0,print_chat,"TalismanID SQLITE_GetAllTalisman");

	return;
}

fTalisman_SQLITE_Save( idUser,iUniqueID )//добавлена в db_sqlite.inl (SQLITE_Save( idUser ))
{
//	client_print(0,print_chat,"TalismanID SQLITE_Save:");
//	log_amx("TalismanID fTalisman_SQLITE_Save");

	//Удаление записи по ID игрока
	new szQueryDeleteTalisman[256];
	format(szQueryDeleteTalisman, charsmax(szQueryDeleteTalisman), "DELETE FROM `wc3_num_talisman` WHERE (`player_id` = '%d' );",iUniqueID);
	new Handle:hQueryDeleteTalisman = SQL_PrepareQuery(g_DBConn, szQueryDeleteTalisman );

	if ( !SQL_Execute( hQueryDeleteTalisman ) )
	{
		SQLITE_Error(hQueryDeleteTalisman, szQueryDeleteTalisman, 104 );
		return;
	}
	//SQL_FreeHandle( hQueryDeleteTalisman );

	//Сохранение количества талисманов игрока
	new szQueryNumTalisman[256];
	format(szQueryNumTalisman, charsmax(szQueryNumTalisman), "REPLACE INTO `wc3_num_talisman` ( `player_id` , `talisman_num`) VALUES ( '%d', '%d' );", iUniqueID, arrTalismanNum[idUser]);
	new Handle:hQueryNumTalisman = SQL_PrepareQuery(g_DBConn, szQueryNumTalisman );

	if ( !SQL_Execute( hQueryNumTalisman ) )
	{
		SQLITE_Error(hQueryNumTalisman, szQueryNumTalisman, 102 );
		return;
	}
//	SQL_FreeHandle( hQueryNumTalisman );

	//Если у игрока талисманов максимальное количество не нужно записывать в базу все талисманы чтобы снизить нагрузку
	if(arrTalismanNum[idUser] >= MAX_TALISMANS)
		return;

	//Сохранение всех талисманов
	for ( new iTalismanID = 0; iTalismanID < MAX_TALISMANS; iTalismanID++ )
	{
		if(arrListTalisman[idUser][iTalismanID] == TALISMAN_ITEM_NONE)
			continue;

		new iTalismanValue = arrListTalisman[idUser][iTalismanID];

		new szQuery[512];
		format( szQuery, charsmax(szQuery), "REPLACE INTO `wc3_player_talisman` ( `player_id` , `talisman_id`) VALUES ( '%d', '%d' );", iUniqueID, iTalismanValue);
		new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			SQLITE_Error( query, szQuery, 103 );
			return;
		}
	//	SQL_FreeHandle( query );
	}
}

fTalisman_SQLITE_Save_T(idUser,iUniqueID )//добавлена в db_sqlite.inl (SQLITE_Save_T( idUser ))
{
//	client_print(0,print_chat,"TalismanID SQLITE_Save_T:");
//	log_amx("TalismanID SQLITE_Save_T");

	//Удаление записи по ID игрока
	new szQueryDeleteTalisman[256];
	format(szQueryDeleteTalisman, charsmax(szQueryDeleteTalisman), "DELETE FROM `wc3_num_talisman` WHERE (`player_id` = '%d' );",iUniqueID);
	SQL_ThreadQuery(g_DBTuple,"_SQLITE_Save_T", szQueryDeleteTalisman );

	//Сохранение количества талисманов игрока
	new szQueryNumTalisman[256];
	format(szQueryNumTalisman, charsmax(szQueryNumTalisman), "REPLACE INTO `wc3_num_talisman` ( `player_id` , `talisman_num`) VALUES ( '%d', '%d' );", iUniqueID, arrTalismanNum[idUser]);
	SQL_ThreadQuery(g_DBTuple,"_SQLITE_Save_T", szQueryNumTalisman );

	//Если у игрока талисманов максимальное количество не нужно записывать в базу все талисманы, чтобы снизить нагрузку
	if(arrTalismanNum[idUser] >= MAX_TALISMANS)
		return;

	//Сохранение всех талисманов
	for ( new iTalismanID = 0; iTalismanID < MAX_TALISMANS; iTalismanID++ )
	{
		if(arrListTalisman[idUser][iTalismanID] == TALISMAN_ITEM_NONE)
			continue;

		new iTalismanValue = arrListTalisman[idUser][iTalismanID];

		new szQuery[512];
		format(szQuery,charsmax(szQuery), "REPLACE INTO `wc3_player_talisman` ( `player_id` , `talisman_id`) VALUES ( '%d', '%d' );", iUniqueID, iTalismanValue);
		SQL_ThreadQuery( g_DBTuple, "_SQLITE_Save_T", szQuery );
	}
}

//=============================================================================
//=============================================================================
//=============================================================================
//-----------------Функции баз данных MYSQL и SQLITE-------------------------------------END }


//-----------------Функции талисмана как пассивного навыка------------------------------------BEGIN {

public fActivateTalismanPassive(idUser, idTalismanActive)//добавлена [items.inl - ITEM_GiveAllBonuses( idUser )]
{

	switch(idTalismanActive)
	{
		case TALISMAN_ITEM_BOOTS:	// Boots of Speed
		{
			SHARED_SetSpeed( idUser );
		}
		case TALISMAN_ITEM_CLOAK:	// Cloak of Shadows
		{
			SHARED_INVIS_Set(idUser);
		}
		case TALISMAN_ITEM_HEALTH:	// Periapt of Health
		{
			new iHealth = get_pcvar_num( CVAR_wc3_health );
			iHealth += get_user_health( idUser );

			if (iHealth > 255)
				set_user_health(idUser, 255 );
			else
				set_user_health( idUser, iHealth );
		}
		case TALISMAN_ITEM_SOCK:	// Sock of the Feather
		{
			SHARED_SetGravity(idUser);
		}
		case TALISMAN_ITEM_GLOVES:	// Flaming Gloves of Warmth
		{
			ITEM_Gloves( idUser );
		}
		case TALISMAN_ITEM_RING:	// Ring of Regeneration
		{
			if ( !task_exists( TASK_ITEM_RING + idUser ) )
			{
				_ITEM_Ring( idUser );
			}
		}
		case TALISMAN_ITEM_CHAMELEON:	// Chameleon
		{
			SHARED_ChangeSkin( idUser, SKIN_SWITCH );
		}
		case TALISMAN_ITEM_ESP:		// ESP
		{
			fEnableItemESP(idUser);
		}
		case TALISMAN_ITEM_ICE_BLOCK:	//Ледяная глыба [shopmenu4.inl - sm4_ITEM_IceBlock_Begin(idUser)]
		{
			ITEM_IceBlock( idUser );
		}
		case TALISMAN_ITEM_THIEF:	//Похититель денег [shopmenu4.inl - ITEM_Thief_Begin(idUser)]
		{
			ITEM_Thief( idUser );
		}
	}

}

stock fWorkTalismanPassive(idUser, idTalismanActive)
{
	switch(idTalismanActive)
	{
		case TALISMAN_ITEM_ANKH:	// Ankh of Reincarnation [shared.inl - public SHARED_CS_Reincarnation( idUser )]
		{
			return TALISMAN_ITEM_ANKH;
		}
		case TALISMAN_ITEM_BOOTS:	// Boots of Speed [shared.inl - public SHARED_SetSpeed( idUser )]
		{
			return TALISMAN_ITEM_BOOTS;
		}
		case TALISMAN_ITEM_CLAWS:	// Claws of Attack [items.inl - ITEM_Offensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace )]
		{
			return TALISMAN_ITEM_CLAWS;
		}
		case TALISMAN_ITEM_CLOAK:	// Cloak of Shadows [shared.inl - public SHARED_INVIS_Set( idUser )]
		{
			return TALISMAN_ITEM_CLOAK;
		}
		case TALISMAN_ITEM_MASK:	// Mask of Death [items.inl - ITEM_Offensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace )]
		{
			return TALISMAN_ITEM_MASK;
		}
		case TALISMAN_ITEM_NECKLACE:	// Necklace of Immunity [ultimates.inl - bool:ULT_CanUserBlockUlt( idUser )]
		{
			return TALISMAN_ITEM_NECKLACE;
		}
		case TALISMAN_ITEM_FROST:	// Orb of Frost [items.inl - ITEM_Offensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace )]
		{
			return TALISMAN_ITEM_FROST;
		}
		case TALISMAN_ITEM_HEALTH:	// Periapt of Health [stocks.inl - stock get_user_maxhealth( idUser )]
		{
			return TALISMAN_ITEM_HEALTH;
		}
		case TALISMAN_ITEM_PROTECTANT:	// Mole Protectant [events.inl - public TRIGGER_TraceLine( Float:v1[3], Float:v2[3], noMonsters, pentToSkip )]
		{
			return TALISMAN_ITEM_PROTECTANT;
		}
		case TALISMAN_ITEM_HELM:	// Helm of Excellence [events.inl - public TRIGGER_TraceLine( Float:v1[3], Float:v2[3], noMonsters, pentToSkip )]
		{
			if(ITEM_HasItem( idUser, ITEM_HELM ) != ITEM_HELM)
					return TALISMAN_ITEM_HELM;
		}
		case TALISMAN_ITEM_AMULET:	// Amulet of the Cat [cssbWar3ftMod.sma - public client_PreThink( id )]
		{
			if(ITEM_HasItem( idUser, ITEM_AMULET ) != ITEM_AMULET)
				return TALISMAN_ITEM_AMULET;
		}
		case TALISMAN_ITEM_SOCK:	// Sock of the Feather [shared.inl - public SHARED_SetGravity( idUser )]
		{
			return TALISMAN_ITEM_SOCK;
		}
		case TALISMAN_ITEM_GLOVES:	// Flaming Gloves of Warmth [events.inl - public grenade_throw( idUser, greindex, wId )]
		{
			if(ITEM_HasItem( idUser, ITEM_GLOVES ) != ITEM_GLOVES)
				return TALISMAN_ITEM_GLOVES;
		}
		case TALISMAN_ITEM_RING:	// Ring of Regeneration + 1 [items.inl - public _ITEM_Ring( idUser )]
		{
			if(ITEM_HasItem( idUser, ITEM_RING ) != ITEM_RING)
				return TALISMAN_ITEM_RING;
		}
		case TALISMAN_ITEM_CHAMELEON:	// Chameleon [race_vip_models.inl - public fwPlayerPostThink(idUser)]
		{
			if(ITEM_HasItem( idUser, ITEM_CHAMELEON ) != ITEM_CHAMELEON)
				return TALISMAN_ITEM_CHAMELEON;
		}
		case TALISMAN_ITEM_INTELLECT:	// Intellect [XP.inl - stock XP_Give( idUser, iBonusXP )]
		{
			if(ITEM_HasItem( idUser, ITEM_INTELLECT ) != ITEM_INTELLECT)
				return TALISMAN_ITEM_INTELLECT;
		}
		case TALISMAN_ITEM_GRENADE_PROTECTION:	// Grenade Protection [events.inl - public EVENT_TakeDamage( iVictim, iWeapon, iAttacker, Float:f_Damage, damagetype )]
		{
			if(ITEM_HasItem( idUser, ITEM_GRENADE_PROTECTION ) != ITEM_GRENADE_PROTECTION)
				return TALISMAN_ITEM_GRENADE_PROTECTION;
		}
		case TALISMAN_ITEM_MIRROR_SHIELD:	// Mirror Shield [events.inl - public client_damage( iAttacker, iVictim, iDamage, iWeapon, iHitPlace, TA )]
		{
			if(ITEM_HasItem( idUser, ITEM_GRENADE_PROTECTION ) != ITEM_GRENADE_PROTECTION)
				return TALISMAN_ITEM_MIRROR_SHIELD;
		}
		case TALISMAN_ITEM_ESP:		// ESP
		{
			if(ITEM_HasItem( idUser, ITEM_ESP ) != ITEM_ESP)
				return TALISMAN_ITEM_ESP;
		}
		case TALISMAN_ITEM_FROST_ARMOR:		// Frost Armor [events.inl - public client_damage( iAttacker, iVictim, iDamage, iWeapon, iHitPlace, TA )]
		{
			if(ITEM_HasItem( idUser, ITEM_FROST_ARMOR ) != ITEM_FROST_ARMOR)
				return TALISMAN_ITEM_FROST_ARMOR;			
		}
		case TALISMAN_ITEM_ULTIMATE_TIMER_DIVIDER:	// Ultimate Timer Divider [ultimates.inl - public _ULT_Delay()]
		{
			if(ITEM_HasItem( idUser, ITEM_ULTIMATE_TIMER_DIVIDER ) != ITEM_ULTIMATE_TIMER_DIVIDER)
					return TALISMAN_ITEM_ULTIMATE_TIMER_DIVIDER;
		}
		case TALISMAN_ITEM_DEVOURER_LIGHT:	//Пожиратель змей [race_shadow.inl - public _SH_DrawSerpentWard( parm[5] )]
		{
			if(ITEM_HasItem( idUser, ITEM_DEVOURER_LIGHT ) != ITEM_DEVOURER_LIGHT)
				return TALISMAN_ITEM_DEVOURER_LIGHT;
		}
		case TALISMAN_ITEM_ICE_BLOCK:	//Ледяная глыба [shopmenu4.inl - sm4_ITEM_IceBlock_Begin(idUser)]
													//	[item_ice_block.inl - public fwSetModel(idEntity,szModel[]) и show_icon(idUser, status)]
		{
			if(ITEM_HasItem( idUser, ITEM_ICE_BLOCK ) != ITEM_ICE_BLOCK)
				return TALISMAN_ITEM_ICE_BLOCK;

		}
		case TALISMAN_ITEM_THIEF:	//Похититель денег [shopmenu4.inl - ITEM_Thief_Begin(idUser)]
		{
			if(ITEM_HasItem( idUser, ITEM_THIEF ) != ITEM_THIEF)
				return TALISMAN_ITEM_THIEF;
		}
		case TALISMAN_ITEM_MIRROR_ULTIMATE:		//Отражатель супер навыка [events.inl - public TRIGGER_TraceLine( Float:v1[3], Float:v2[3], noMonsters, pentToSkip )]
		{															   // (не добавлять)[race_crypt.inl - CL_ULT_LocustSwarm( iAttacker )]

			if (random_float(0.0, 1.0 ) <= 0.75 )
			{
				if(ITEM_HasItem( idUser, ITEM_MIRROR_ULTIMATE ) != ITEM_MIRROR_ULTIMATE)
					return TALISMAN_ITEM_MIRROR_ULTIMATE;
			}
		}
	}

	return TALISMAN_ITEM_NONE;
}

//Какой талисман является активным у игрока
//добавлена [shared.inl - SHARED_Glow( idUser, iRed, iGreen, iBlue, iAll ),
//						  SHARED_GlowShell(idUser, iRed, iGreen, iBlue,Float:fTime,iAmountGlow = 16)]
bool:bIsActiveTalisman(idUser,idTalisman)
{
	if(arrTalismanActive[idUser] == idTalisman)
		return true;

	return false;
}


//=============================================================================
//=============================================================================
//=============================================================================
//-----------------Функции талисмана как пассивного навыка-------------------------------------END }


//-----------------Функции отображения Sync HUD------------------------------------BEGIN {

public fShowSyncMessageRandomColor(idUser,szHUDMessage[],iMessageEffect,Float:fX,Float:fY)
{
	new rgbRed, rgbGreen, rgbBlue;

	rgbRed = random_num(0, 255);
	rgbGreen = random_num(0, 255);
	rgbBlue = random_num(0, 255);
			
	new iEffect, Float:iFadeIn, Float:iFadeOut, Float:iHoldTime;

	switch(iMessageEffect)
	{
		case 1:
		{
			iEffect = 1;
			iFadeIn = 0.2;
			iFadeOut = 0.2;
			iHoldTime = 7.0;
		}
		case 2:
		{
			iEffect = 2;
			iFadeIn = 0.05;
			iFadeOut = 0.5;
			iHoldTime = 7.0;
		}
		default:
		{
			iEffect = 0;
			iFadeIn = 0.5;
			iFadeOut = 0.5;
			iHoldTime = 7.0;
		}
	}

	set_hudmessage(rgbRed, rgbGreen, rgbBlue,fX,fY, iEffect, 0.1, iHoldTime, iFadeIn, iFadeOut, 3);
	ShowSyncHudMsg(idUser, stSyncHudCreate ,"%s", szHUDMessage);
}

//=============================================================================
//=============================================================================
//=============================================================================
//-----------------Функции отображения Sync HUD-------------------------------------END }


//-----------------Функции администратора-------------------------------------BEGIN {
// Give the user an talisman
ADMIN_GiveTalisman( idUser, szTarget[], iItemID )//добавлена в admin.inl (public ADMIN_Handler( idUser ))
{
/*	if(!(get_user_flags(idUser) & ADMIN_FLAG))
	{
		formatex(szMessage, charsmax(szMessage), "%L",LANG_PLAYER,"YOU_HAVE_NO_ACCESS");		
		client_print( idUser, print_console,szMessage);	
		return;
	}*/
		
	new szItemName[32];
	LANG_GetItemName ( iItemID, idUser, szItemName, 31 );

	new iTarget = 0, bool:bTargetFound = false;

	//Дать все талисманы
	if(iItemID == 999)
	{
		while ( ( iTarget = FindTarget( iTarget, szTarget ) ) > 0 )
		{
			ADMIN_Talisman_Buy_All(iTarget);
			bTargetFound = true;
		}

		if ( !bTargetFound )
			ADMIN_NoTargetFound( idUser, szTarget, ( iTarget == -2 ) );

		return;
	}

	//Дать один талисман
	while ( ( iTarget = FindTarget( iTarget, szTarget ) ) > 0 )
	{
		ADMIN_Talisman_Buy( iTarget, iItemID );
		bTargetFound = true;
	}
	if ( !bTargetFound )
	{
		ADMIN_NoTargetFound( idUser, szTarget, ( iTarget == -2 ) );
	}
}

public ADMIN_Talisman_Buy( idUser, iTalismanValue )
{
	if(!bIsPlayerPresenceTalisman(idUser,iTalismanValue))//Если нет в наличии талисмана
	{
		for(new i = 0; i < MAX_TALISMANS; i++)
		{
			if(LIST_TALISMANS[i] == iTalismanValue)
			{
				arrListTalisman[idUser][i] = iTalismanValue;

				arrTalismanNum[idUser]++;

				fShowMenuTalismans(idUser);

				break;
			}
		}
	}
}

public ADMIN_Talisman_Buy_All( idUser )
{
	for(new i = 0; i < MAX_TALISMANS; i++)
		arrListTalisman[idUser][i] = LIST_TALISMANS[i];

	arrTalismanNum[idUser] = MAX_TALISMANS;

	fShowMenuTalismans(idUser);
}

//=============================================================================
//=============================================================================
//=============================================================================
//-----------------Функции администратора-------------------------------------END }


//-----------------Функции логов-------------------------------------BEGIN {

//Лог появления талисмана для проверки что все работает
fTalismanLog(ptX,ptY,ptZ)
{

	new szLogFile[128];
	get_configsdir( szLogFile, charsmax(szLogFile) );
	formatex( szLogFile, charsmax(szLogFile), "%s/war3ft/wc3_talisman.log", szLogFile );

	if (!file_exists( szLogFile ) )
	{
		write_file( szLogFile, "WC3: Frozen Throne", -1 );
		write_file( szLogFile, "Logging of respawn talismans", -1 );
		write_file( szLogFile, " ", -1 );
	}

	new szCvarNetAddress[64];
    get_cvar_string("net_address",szCvarNetAddress,charsmax(szCvarNetAddress) );
	
	new szCvarIP[64];
    get_cvar_string("ip",szCvarIP,charsmax(szCvarIP) );	

	new szMap[32];
	get_mapname(szMap, charsmax(szMap));

	new szCurrentTime[64];
	get_time( "L %m/%d/%Y - %H:%M:%S", szCurrentTime,charsmax(szCurrentTime));

	new szLogEntry[256];
	formatex(szLogEntry, 255, "%s Spawn: ^"<%d><%d><%d>^" Map: ^"<%s>^" IP: ^"<%s><%s>^" ", szCurrentTime,ptX,ptY,ptZ,szMap,szCvarNetAddress,szCvarIP);

	write_file( szLogFile, szLogEntry, -1 );
}

//Лог появления талисмана для проверки что все работает
fTalismanPlayerPickUpLog(idUserPickUp,szItemName[])
{
	new szLogFile[128];
	get_configsdir( szLogFile, charsmax(szLogFile) );
	formatex( szLogFile, charsmax(szLogFile), "%s/war3ft/wc3_talisman_pickup.log", szLogFile );

	if (!file_exists( szLogFile ) )
	{
		write_file( szLogFile, "WC3: Frozen Throne", -1 );
		write_file( szLogFile, "Logging of pick up player talismans", -1 );
		write_file( szLogFile, " ", -1 );
	}

	new szUserIP[32], szUserAuthid[32],szUserName[32];
	get_user_ip(idUserPickUp, szUserIP, charsmax(szUserIP), 1);
	get_user_authid(idUserPickUp, szUserAuthid, charsmax(szUserAuthid));
	get_user_name(idUserPickUp, szUserName, charsmax(szUserName));
	
	new szMap[32];
	get_mapname(szMap, charsmax(szMap));

	new szCurrentTime[64];
	get_time( "L %m/%d/%Y - %H:%M:%S", szCurrentTime,charsmax(szCurrentTime));

	new szLogEntry[256];
	formatex(szLogEntry, 255, "%s Name: ^"<%s>^" StemID: ^"<%s>^" IP: ^"<%s>^" Map: ^"<%s>^" Talisman: ^"<%s>^" ", szCurrentTime,szUserName,szUserAuthid,szUserIP,szMap,szItemName);

	write_file( szLogFile, szLogEntry, -1 );
}

//=============================================================================
//=============================================================================
//=============================================================================
//-----------------Функции логов-------------------------------------END }