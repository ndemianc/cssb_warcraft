
stock fTagWar3ft()
{
	new szTag[64];
	formatex(szTag, sizeof(szTag) - 1, "!y%L!t%L!y%L ",LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT");
	return szTag;
}

//-----------------Базовые функции добавленные в cssbWar3ftMod.sma-----------------------BEGIN {
public sm4_PluginPrecache()
{
	precache_model(szModelFrozen);
}

public sm4_ClientPutinserver(idUser)
{
	isChilled[idUser] = 0;
	isFrozen[idUser] = 0;
	frostKilled[idUser] = 0;
	novaDisplay[idUser] = 0;
	hasFrostNade[idUser] = 0;
	
}

public sm4_ClientDisconnect(idUser)
{
	if(isChilled[idUser]) task_remove_chill(TASK_REMOVE_CHILL + idUser);
	if(isFrozen[idUser]) task_remove_freeze(TASK_REMOVE_FREEZE + idUser);

	sm4_RemoveSkeleton(idUser);

	fSleepRemove( idUser + TASK_SLEEP_WEAPON );
}
//=============================================================================
//=============================================================================
//=============================================================================

//-----------------Базовые функции добавленные в cssbWar3ftMod.sma-----------------------END }


//-----------------Основные функции-----------------------BEGIN {

public sm4_CVAR_Init()//добавлена в cvar.inl
{
	//Настройки предметов из shopmenu4:
	CVAR_wc3_light_charges  = register_cvar("wc3_light_charges", "3");						//Количество зарядов пожирателя змей
	CVAR_wc3_iceblock_icon = register_cvar("wc3_iceblock_icon","1");						//Включить/выключить иконку ледяной глыбы
	CVAR_wc3_iceblock_color = register_cvar("wc3_iceblock_color","0 206 209");				//Цвет ледяной глыбы
	CVAR_wc3_iceblock_fteam = register_cvar("wc3_iceblock_fteam","0");						//Будет ли замораживать своих по команде
	CVAR_wc3_iceblock_damage = register_cvar("wc3_iceblock_damage","20.0");					//Повреждения от ледяной глыбы
	CVAR_wc3_iceblock_timer = register_cvar("wc3_iceblock_timer","5");						//Таймер до следующей ледяной глыбы
	CVAR_wc3_thief_time = register_cvar("wc3_thief_time","5");								//Время через которое прибавляются деньги
	CVAR_wc3_thief_money = register_cvar("wc3_thief_money","20");							//Сколько денег прибавлять (учесть умножение на кол-во игроков)
	CVAR_wc3_mirror_ult_charges  = register_cvar("wc3_mirror_ult_charges", "3");			//Количество зарядов отражателя супер навыка
	CVAR_wc3_deathcoil_charges  = register_cvar("wc3_deathcoil_charges", "2");				//Количество зарядов Лик смерти
	CVAR_wc3_deathcoil_damage  = register_cvar("wc3_deathcoil_damage", "30");				//Количество повреждений наносимые Лик смерти ( расчет wc3_deathcoil_damage*(0.7) )
	CVAR_wc3_deathcoil_percent  = register_cvar("wc3_deathcoil_percent", "100");			//Процент вероятности сраатывания Лик смерти
	CVAR_wc3_impale_damage  = register_cvar("wc3_impale_damage", "30");						//Повреждения при подбросе
	CVAR_wc3_impale_armordmg  = register_cvar("wc3_impale_armordmg", "15");					//Повреждения брони при подбросе
	CVAR_wc3_impale_colldmg  = register_cvar("wc3_impale_colldmg", "30");					//Повреждения при подбросе (при столкновении с потолком)
	CVAR_wc3_impale_charges  = register_cvar("wc3_impale_charges", "2");					//Количество зарядов Пронзающий Шип
	CVAR_wc3_impale_percent  = register_cvar("wc3_impale_percent", "100");					//Процент вероятности срабатывания Пронзающий Шип
	CVAR_wc3_sleep_charges  = register_cvar("wc3_sleep_charges", "1");						//Количество зарядов Сонная аура
	CVAR_wc3_sleep_percent  = register_cvar("wc3_sleep_percent", "100");					//Процент вероятности срабатывания Сонная аура
	CVAR_wc3_sleep_duration  = register_cvar("wc3_sleep_duration", "4.0");					//Продолжительность сна при воздействии Сонная аура
	CVAR_wc3_book_dead_charges  = register_cvar("wc3_book_dead_charges", "1");				//Количество вызываемых скелетов Книга мертвых
	CVAR_wc3_buy_round_book_dead  = register_cvar("wc3_buy_round_book_dead", "2");			//Сколько раз можно покупать предмет Книга мертвых за один раунд
	CVAR_wc3_percent_hp_book_dead  = register_cvar("wc3_percent_hp_book_dead", "30");		//Процент HP от здоровья вызвавшего, дающийся скелету 
	CVAR_wc3_tranquility_time  = register_cvar("wc3_tranquility_time", "15.0");				//Время работы предмета Спокойствие
	CVAR_wc3_tranquility_range  = register_cvar("wc3_tranquility_range", "250");			//Радиус в котором могут лечиться игроки
	CVAR_wc3_buy_round_tranquility  = register_cvar("wc3_buy_round_tranquility", "2");		//Сколько раз можно покупать предмет Спокойствие за один раунд
	CVAR_wc3_tranquility_hp  = register_cvar("wc3_tranquility_hp", "2");					//По сколько HP восстанавливает предмет Спокойствие

	//Цены на предметы shopmenu4:
	CVAR_wc3_cost_devourer_light  = register_cvar("wc3_cost_devourer_light", "600");		//Devourer of lightnings - Пожиратель змей
	CVAR_wc3_cost_ice_block  = register_cvar("wc3_cost_ice_block", "1100");					//Ice Block - Ледяная глыба
	CVAR_wc3_cost_thief  = register_cvar("wc3_cost_thief", "2200");							//Thief of money - Похититель денег
	CVAR_wc3_cost_mirror_ult  = register_cvar("wc3_cost_mirror_ult", "950");				//Mirror Ultimate - Отражатель супер навыка
	CVAR_wc3_cost_death_coil  = register_cvar("wc3_cost_death_coil", "1500");				//Death Coil - Лик смерти
	CVAR_wc3_cost_impale  = register_cvar("wc3_cost_impale", "2000");						//Impale - Пронзающий Шип
	CVAR_wc3_cost_sleep  = register_cvar("wc3_cost_sleep", "3000");							//Sleep - Сонная аура
	CVAR_wc3_cost_book_dead  = register_cvar("wc3_cost_book_dead", "5000");					//Book of The Dead - Книга мертвых
	CVAR_wc3_cost_tranquility  = register_cvar("wc3_cost_tranquility", "6000");				//Спокойствие - Tranquility

}
//Сброс всех счетчиков на ограничение покупок
sm4_ITEM_LimitOneRound(idUser)//добавлена в items.inl
{
	arrIsByuItemOneRound[idUser][ITEM_DEVOURER_LIGHT]	    = 0;	// 
	arrIsByuItemOneRound[idUser][ITEM_ICE_BLOCK]			= 0;	// 
	arrIsByuItemOneRound[idUser][ITEM_THIEF]				= 0;	// 
	arrIsByuItemOneRound[idUser][ITEM_MIRROR_ULTIMATE]		= 0;	// 
	arrIsByuItemOneRound[idUser][ITEM_DEATHCOIL]			= 0;	// 
	arrIsByuItemOneRound[idUser][ITEM_IMPALE]				= 0;	//
	arrIsByuItemOneRound[idUser][ITEM_SLEEP]				= 0;	// 
	arrIsByuItemOneRound[idUser][ITEM_BOOK_DEAD]			= 0;	// 
	arrIsByuItemOneRound[idUser][ITEM_TRANQUILITY]			= 0;	// 
}
//Цены на предметы
sm4_ITEM_InitCost()//добавлена в items.inl
{
	ITEM_COST[ITEM_DEVOURER_LIGHT]	    = get_pcvar_num( CVAR_wc3_cost_devourer_light );
	ITEM_COST[ITEM_ICE_BLOCK]			= get_pcvar_num( CVAR_wc3_cost_ice_block );
	ITEM_COST[ITEM_THIEF]				= get_pcvar_num( CVAR_wc3_cost_thief );
	ITEM_COST[ITEM_MIRROR_ULTIMATE]		= get_pcvar_num( CVAR_wc3_cost_mirror_ult );
	ITEM_COST[ITEM_DEATHCOIL]			= get_pcvar_num( CVAR_wc3_cost_death_coil );
	ITEM_COST[ITEM_IMPALE]				= get_pcvar_num( CVAR_wc3_cost_impale );
	ITEM_COST[ITEM_SLEEP]				= get_pcvar_num( CVAR_wc3_cost_sleep );
	ITEM_COST[ITEM_BOOK_DEAD]			= get_pcvar_num( CVAR_wc3_cost_book_dead );
	ITEM_COST[ITEM_TRANQUILITY]			= get_pcvar_num( CVAR_wc3_cost_tranquility );
	
}
//Сообщения выводимые при покупке предмета
sm4_ITEM_DisplayMessage( idUser, iItem )//добавлена в items.inl
{
	switch ( iItem )
	{
		case ITEM_DEVOURER_LIGHT:
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"INFO_SHOPMENU4_1",DEVOURER_LIGHT_CHARGES);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);

		}
		case ITEM_ICE_BLOCK:
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"INFO_SHOPMENU4_2",get_pcvar_num( CVAR_wc3_iceblock_timer ));
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);


		}
		case ITEM_THIEF:
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"INFO_SHOPMENU4_3",THIEF_TIME,THIEF_MONEY);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);

		}
		case ITEM_MIRROR_ULTIMATE:
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"INFO_SHOPMENU4_4",MIRROR_ULTIMATE_CHARGES);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}
		case ITEM_DEATHCOIL:
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"INFO_SHOPMENU4_5",DEATHCOIL_CHARGES);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}
		case ITEM_IMPALE:
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"INFO_SHOPMENU4_6",IMPALE_CHARGES);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}
		case ITEM_SLEEP:
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"INFO_SHOPMENU4_7",SLEEP_CHARGES);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}
		case ITEM_BOOK_DEAD:
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"INFO_SHOPMENU4_8",BOOK_DEAD_CHARGES);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}
		case ITEM_TRANQUILITY:
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"INFO_SHOPMENU4_9");
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}

	}

}
//Удаление предмета
sm4_ITEM_Remove( idUser,iItem)//добавлена в items.inl
{
	switch( iItem )
	{
		case ITEM_DEVOURER_LIGHT:
		{
			arriTotalRingsDevourerLight[idUser] = 0;
		}
		case ITEM_ICE_BLOCK:
		{
			if ( task_exists( TASK_ITEM_ICE_BLOCK + idUser ) )
			{
				remove_task( TASK_ITEM_ICE_BLOCK + idUser );
			}
		}
		case ITEM_THIEF:
		{
			if ( task_exists( TASK_ITEM_THIEF + idUser ) )
			{
				remove_task( TASK_ITEM_THIEF + idUser );
			}
		}
		case ITEM_MIRROR_ULTIMATE:
		{
			arriTotalChargeMirrorUltimate[idUser] = 0;
		}
		case ITEM_DEATHCOIL:
		{
			arriTotalChargeDeathCoil[idUser] = 0;
		}
		case ITEM_IMPALE:
		{
			arriTotalChargeImpale[idUser] = 0;
			
		}
		case ITEM_SLEEP:
		{
			arriTotalChargeSleep[idUser] = 0;
		}
		case ITEM_BOOK_DEAD:
		{
		}
		case ITEM_TRANQUILITY:
		{
		}
		
	}

}
//Удаление колец и зарядов
sm4_ITEM_RemoveCharge(idUser,iItem )//добавлена в items.inl
{
	switch ( iItem )
	{
		case ITEM_DEVOURER_LIGHT:
		{
			arriTotalRingsDevourerLight[idUser] -= CHARGE_DISPOSE;
			
			if (arriTotalRingsDevourerLight[idUser] <= 0 )
			{
				ITEM_RemoveID( idUser, iItem );
			}
		}
		case ITEM_MIRROR_ULTIMATE:
		{
			arriTotalChargeMirrorUltimate[idUser] -= CHARGE_DISPOSE;
			
			if (arriTotalChargeMirrorUltimate[idUser] <= 0 )
			{
				ITEM_RemoveID( idUser, iItem );
			}
		}
		case ITEM_DEATHCOIL:
		{
			arriTotalChargeDeathCoil[idUser] -= CHARGE_DISPOSE;
			
			if (arriTotalChargeDeathCoil[idUser] <= 0 )
			{
				ITEM_RemoveID( idUser, iItem );
			}
		}
		case ITEM_IMPALE:
		{
			arriTotalChargeImpale[idUser] -= CHARGE_DISPOSE;
			
			if (arriTotalChargeImpale[idUser] <= 0 )
			{
				ITEM_RemoveID( idUser, iItem );
			}
		}
		case ITEM_SLEEP:
		{
			arriTotalChargeSleep[idUser] -= CHARGE_DISPOSE;
			
			if (arriTotalChargeSleep[idUser] <= 0 )
			{
				ITEM_RemoveID( idUser, iItem );
			}
		}
		

		
	}

}
////Дает Бонус (предмет) сразу при покупке
sm4_ITEM_GiveBonuses( idUser, iItem )//добавлена в items.inl
{
	switch ( iItem )
	{
		case ITEM_DEVOURER_LIGHT:
		{
			arriTotalRingsDevourerLight[idUser] += DEVOURER_LIGHT_CHARGES;
		}
		case ITEM_ICE_BLOCK:
		{
			ITEM_IceBlock( idUser );
		}
		case ITEM_THIEF:
		{
			ITEM_Thief( idUser );
		}
		case ITEM_MIRROR_ULTIMATE:
		{
			arriTotalChargeMirrorUltimate[idUser] += MIRROR_ULTIMATE_CHARGES;
		}
		case ITEM_DEATHCOIL:
		{
			arriTotalChargeDeathCoil[idUser] += DEATHCOIL_CHARGES;
		}
		case ITEM_IMPALE:
		{
			arriTotalChargeImpale[idUser] += IMPALE_CHARGES;
		}
		case ITEM_SLEEP:
		{
			arriTotalChargeSleep[idUser] += SLEEP_CHARGES;
		}
		case ITEM_BOOK_DEAD:
		{
		}
		case ITEM_TRANQUILITY:
		{
			
		}
	}
}


//=============================================================================
//=============================================================================
//=============================================================================

//-----------------Основные функции-----------------------END }

//-----------------Предмет "Ледяная глыба"-----------------------BEGIN {
ITEM_IceBlock( idUser )
{
	if ( !WC3_Check() )
		return;

	if ( !SHARED_HasIceBlock( idUser ) )
	{
		arriIceBlockTimer[idUser] = 0;

		_ITEM_IceBlock_Give( idUser );
	}

	return;
}

sm4_ITEM_IceBlock_Begin(idUser)//добавлена в events.inl
{
	//Для talisman +
	// Предмет "Ледяная глыба"
	if ( ITEM_Has(idUser, ITEM_ICE_BLOCK ) > ITEM_NONE || fWorkTalismanPassive(idUser, arrTalismanActive[idUser]) == TALISMAN_ITEM_ICE_BLOCK)
	{
		// Then lets start a timer to give them a grenade!
		arriIceBlockTimer[idUser] = get_pcvar_num( CVAR_wc3_iceblock_timer );

		//format(szMessage, sizeof(szMessage) - 1, "%d %L",arriIceBlockTimer[idUser],LANG_PLAYER,"UNTIL_YOUR_NEXT_ICE_BLOCK");
		//WC3_StatusText( idUser, ICE_BLOCK_TIMER,szMessage);

		arriIceBlockTimer[idUser]--;

		set_task( 1.0, "_ITEM_IceBlock_Give", TASK_ITEM_ICE_BLOCK + idUser );
	}
}

public _ITEM_IceBlock_Give( idUser )
{
	if ( !WC3_Check() )
		return;

	if ( idUser >= TASK_ITEM_ICE_BLOCK )
		idUser -= TASK_ITEM_ICE_BLOCK;

	if ( !p_data_b[idUser][PB_ISCONNECTED] || !is_user_alive( idUser ) )
		return;

	// Only need to save once! - this is b/c I'm not sure when the loss of a grenade is updated - and I wanted SHARED_HasIceBlock to work @ all times!
	if ( arriIceBlockTimer[idUser] == get_pcvar_num( CVAR_wc3_iceblock_timer ) - 1 )
		SHARED_SaveWeapons( idUser );

	// Lets do CS/CZ only
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Already have a grenade!!
		if ( SHARED_HasIceBlock( idUser ) )
		{
			arriIceBlockTimer[idUser] = 0;

			return;
		}
	}

	if ( arriIceBlockTimer[idUser] > 0 )
	{
		//format(szMessage, sizeof(szMessage) - 1, "%d %L",arriIceBlockTimer[idUser],LANG_PLAYER,"UNTIL_YOUR_NEXT_ICE_BLOCK");
		//WC3_StatusText( idUser, ICE_BLOCK_TIMER,szMessage);

		arriIceBlockTimer[idUser]--;

		set_task( 1.0, "_ITEM_IceBlock_Give", TASK_ITEM_ICE_BLOCK + idUser );

		return;
	}

	// Counter-Strike or Condition Zero grenade
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		give_item( idUser, "weapon_flashbang" );
	}
	// Display a message to the user
	//WC3_StatusText( idUser, ICE_BLOCK_TIMER, "%L", idUser, "ENJOY_ICE_BLOCK" )

	return;
}
//=============================================================================
//=============================================================================
//=============================================================================

//-----------------Предмет "Ледяная глыба"-----------------------END }
 

//-----------------Предмет "Похититель денег"-----------------------BEGIN {
ITEM_Thief( idUser )
{
	if ( !WC3_Check() )
		return;

	arriThiefTimer[idUser] = 0;

	_ITEM_Thief_Give( idUser );

	ITEM_Thief_Begin(idUser);

	return;
}

ITEM_Thief_Begin(idUser)
{
	//Для talisman +
	// Предмет "Похититель денег"
	if ( ITEM_Has(idUser, ITEM_THIEF ) > ITEM_NONE || fWorkTalismanPassive(idUser, arrTalismanActive[idUser]) == TALISMAN_ITEM_THIEF)
	{
		if(task_exists( TASK_ITEM_THIEF + idUser ))
			remove_task( TASK_ITEM_THIEF + idUser );

		arriThiefTimer[idUser] = THIEF_TIME;

		arriThiefTimer[idUser]--;

		set_task( 1.0, "_ITEM_Thief_Give", TASK_ITEM_THIEF + idUser );
	}
}

public _ITEM_Thief_Give( idUser )
{
	if ( !WC3_Check() )
		return;

	if ( idUser >= TASK_ITEM_THIEF )
		idUser -= TASK_ITEM_THIEF;

	if ( !p_data_b[idUser][PB_ISCONNECTED] || !is_user_alive( idUser ) )
		return;

	if ( arriThiefTimer[idUser] > 0 )
	{
		arriThiefTimer[idUser]--;

		set_task( 1.0, "_ITEM_Thief_Give", TASK_ITEM_THIEF + idUser );

		return;
	}

	//Начисление денег
	new players[32], numplayers;
	get_players( players, numplayers );

	new iMoney;
	iMoney = SHARED_GetUserMoney( idUser );
	
	new iNewMoney = iMoney + (THIEF_MONEY * numplayers);

	SHARED_SetUserMoney(idUser, iNewMoney, 1 );

//	client_print(idUser,print_chat,"iNewMoney: %d",iNewMoney);

	ITEM_Thief_Begin(idUser);


}
//=============================================================================
//=============================================================================
//=============================================================================

//-----------------Предмет "Похититель денег"-----------------------END }


//-----------------Предмет "Отражатель супер навыка"-----------------------BEGIN {
sm4_EffectMirrorUltimate(idUser)//добавлен в events.inl
{
	if (!is_user_alive(idUser)) 
		return;
	
	static Float:origin[3];
	pev(idUser,pev_origin,origin);

	message_begin(MSG_BROADCAST,SVC_TEMPENTITY); 
	write_byte(TE_EXPLOSION); // TE_EXPLOSION
	write_coord(floatround(origin[0])); // origin x
	write_coord(floatround(origin[1])); // origin y
	write_coord(floatround(origin[2])); // origin z
	write_short(g_iSprites[SPR_MIRROR_ULTIMATE]); // sprites
	write_byte(15); // scale in 0.1's
	write_byte(12); // framerate
	write_byte(14); // flags 
	message_end(); // message end
	
	message_begin(MSG_ONE_UNRELIABLE, gmsgScreenFade,_, idUser);
	write_short(1<<10);
	write_short(1<<10);
	write_short(0x0000);
	write_byte(238);//r
	write_byte(232);  //g
	write_byte(170);  //b
	write_byte(75);
	message_end();

}
//=============================================================================
//=============================================================================
//=============================================================================

//-----------------Предмет "Отражатель супер навыка"-----------------------END }


//-----------------Предмет "Книга мертвых"-----------------------BEGIN {

sm4_ITEM_Book_Dead(idUser,iItem)//добавлен в items.inl
{

	if ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 )
		return;

	if (!is_user_alive(idUser)) 
		return;
	
	new iTeamCurrentUser;
	iTeamCurrentUser = get_user_team(idUser);

	new iNewHealthSkeleton = floatround(float(SHARED_GetUserHealth(idUser) * BOOK_DEAD_PERCENT_HEALTH / 100));

	if(iNewHealthSkeleton == 0)
		return;

	new arrPlayers[32], iNumPlayers;

	if(iTeamCurrentUser == TEAM_T)
		get_players( arrPlayers, iNumPlayers, "be","TERRORIST");
	if(iTeamCurrentUser == TEAM_CT)
		get_players( arrPlayers, iNumPlayers, "be","CT");
		
		
	//Не хватает мертвых игроков для оживления
	if(iNumPlayers < BOOK_DEAD_CHARGES)
	{
		formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_NO_DEAD_PLAYERS",BOOK_DEAD_CHARGES);
		cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);

		new iItemMoneyBookDead = ITEM_Cost( idUser, ITEM_BOOK_DEAD );
		SHARED_SetUserMoney(idUser,SHARED_GetUserMoney( idUser ) + iItemMoneyBookDead, 0 );

		return;
	}

	sm4_ITEM_DisplayMessage( idUser, iItem );

	arrIsByuItemOneRound[idUser][ITEM_BOOK_DEAD]++;

	emit_sound(idUser, CHAN_STATIC, g_szSounds[SOUND_BOOK_DEAD_RAISE], 1.0, ATTN_NORM, 0, PITCH_NORM );

	for (new iBookDeadCount = 0; iBookDeadCount < BOOK_DEAD_CHARGES; iBookDeadCount++ )
	{
		new iAliveSkeleton = random_num(0,iNumPlayers - 1);

		new iSkeletonId = -1;
		for (new i = 0; i < iNumPlayers; i++ )
		{
			iSkeletonId = arrPlayers[i];

			if(iSkeletonId == idUser)
				continue;

			if(iAliveSkeleton == i)
			{
				new ent = SHARED_FindFreeSpawn( iSkeletonId, true );
				if ( ent < 0 )
				{
					WC3_StatusText( idUser, 0, "%L", LANG_PLAYER, "NO_FREE_SPAWN_FOUND" );
					return;
				}
				else
				{
					
					ArrayPushCell(arrIntSkeletons, iSkeletonId);

					SHARED_CopySavedWeapons( iSkeletonId );
					ExecuteHamB(Ham_CS_RoundRespawn,iSkeletonId);
					SHARED_ChangeSkin( iSkeletonId, SKIN_RESET );

					SHARED_SetUserHealth(iSkeletonId,iNewHealthSkeleton);

					set_task(0.4, "_SHARED_CS_GiveWeapons", TASK_GIVEITEMS + iSkeletonId );

					formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_RESPAWN_SKELETON",BOOK_DEAD_CHARGES);
					cssbChatColor(iSkeletonId,"%s%s",fTagWar3ft(),szMessage);

					new vOrigin[3], vNewOrigin[3], Float:fSpawnOrigin[3];
					
					get_user_origin( iSkeletonId, vOrigin );
					entity_get_vector( ent, EV_VEC_origin, fSpawnOrigin );
					FVecIVec( fSpawnOrigin, vNewOrigin );
					set_user_origin( iSkeletonId, vNewOrigin );
					Create_TE_TELEPORT( vNewOrigin );
															
					emit_sound(iSkeletonId, CHAN_STATIC, g_szSounds[SOUND_BOOK_DEAD_WARLOCK], 1.0, ATTN_NORM, 0, PITCH_NORM );

				}



				break;
			}
			
		}
	}
		
}

//Удаление скелета если игрок вышел с сервара
stock sm4_RemoveSkeleton(idCurUser)//добавлена в items.inl
{
	for (new i = 0; i < ArraySize(arrIntSkeletons); i++)
	{
		new idCurSkeleton = ArrayGetCell(arrIntSkeletons, i);
		if(idCurUser == idCurSkeleton)
		{
			ArrayDeleteItem(arrIntSkeletons,i);
			break;
		}
	}
}

bool:sm4_IsPlayerSkeleton(idUser)//добавлена в items.inl и race_shadow.inl
{
	new bool:bIsPlayerSkeleton = false;
	for (new i = 0; i < ArraySize(arrIntSkeletons); i++)
	{
		new idCurSkeleton = ArrayGetCell(arrIntSkeletons, i);
		if(idUser == idCurSkeleton)
			bIsPlayerSkeleton = true;
	}

	return bIsPlayerSkeleton;
}

//Обнуление всех массивов Книга мертвых
//(в данном случае запускается при старте нового раунда
//файл events.inl - функция public EVENT_NewRound()  )
sm4_fRemoveAllArrayBookDead()
{
	ArrayClear(arrIntSkeletons);
}
//=============================================================================
//=============================================================================
//=============================================================================

//-----------------Предмет "Книга мертвых"-----------------------END }


//-----------------Блокировка предметов на определенных картах-----------------------BEGIN {
bool:sm4_DisabledItemOnMap(idUser,iItem,szItemName[])
{
	LANG_GetItemName( iItem, idUser, szItemName, 31, true );

	switch ( iItem )
	{
		case ITEM_DEVOURER_LIGHT:
			return bDisabledDevourerLight;	//Пожиратель змей
		case ITEM_ICE_BLOCK:
			return bDisabledIceBlock;		//Ледяная глыба
		case ITEM_THIEF:
			return bDisabledThief;			//Похититель денег
		case ITEM_MIRROR_ULTIMATE:
			return bDisabledMirrorUltimate;	//Отражатель супер навыка
		case ITEM_DEATHCOIL:
			return bDisabledDeathcoil;		//Лик смерти
		case ITEM_IMPALE:
			return bDisabledImpale;			//Пронзающий шип			
		case ITEM_SLEEP:
			return bDisabledSleep;			//Сонная аура			
		case ITEM_BOOK_DEAD:
			return bDisabledBookDead;		//Книга мертвых
		case ITEM_TRANQUILITY:
			return bDisabledTranquility;	//Спокойствие
	}

	return false;
}
//=============================================================================
//=============================================================================
//=============================================================================

//-----------------Блокировка предметов на определенных картах-----------------------END }

