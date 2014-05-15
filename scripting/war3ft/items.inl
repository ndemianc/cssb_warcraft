/*
*	Item Functions
*/
//Цены на предметы
ITEM_InitCost()
{
	ITEM_COST[ITEM_ANKH]	    = get_pcvar_num( CVAR_wc3_cost_ankh );	// Ankh of Reincarnation
	ITEM_COST[ITEM_BOOTS]       = get_pcvar_num( CVAR_wc3_cost_boots_of_speed );			// Boots of Speed
	ITEM_COST[ITEM_CLAWS]	    = get_pcvar_num( CVAR_wc3_cost_claws_of_attack );			// Claws of Attack
	ITEM_COST[ITEM_CLOAK]	    = get_pcvar_num( CVAR_wc3_cost_cloak_of_shadows );			// Cloak of Shadows
	ITEM_COST[ITEM_MASK]	    = get_pcvar_num( CVAR_wc3_cost_mask_of_death );			// Mask of Death
	ITEM_COST[ITEM_NECKLACE]	= get_pcvar_num( CVAR_wc3_cost_necklace );			// Necklace of Immunity
	ITEM_COST[ITEM_FROST]	    = get_pcvar_num( CVAR_wc3_cost_orb_of_frost );			// Orb of Frost
	ITEM_COST[ITEM_HEALTH]	    = get_pcvar_num( CVAR_wc3_cost_health );			// Periapt of Health
	ITEM_COST[ITEM_TOME]	    = get_pcvar_num( CVAR_wc3_cost_tome );			// Tome of Experience
	ITEM_COST[ITEM_SCROLL]	    = get_pcvar_num( CVAR_wc3_cost_scroll );			// Scroll of Respawning
	ITEM_COST[ITEM_PROTECTANT]	= get_pcvar_num( CVAR_wc3_cost_mole_protect );			// Mole Protectant
	ITEM_COST[ITEM_HELM]	    = get_pcvar_num( CVAR_wc3_cost_helm );			// Helm of Excellence
	ITEM_COST[ITEM_AMULET]	    = get_pcvar_num( CVAR_wc3_cost_amulet_cat );			// Amulet of the Cat
	ITEM_COST[ITEM_SOCK]	    = get_pcvar_num( CVAR_wc3_cost_sock );			// Sock of the Feather
	ITEM_COST[ITEM_GLOVES]	    = get_pcvar_num( CVAR_wc3_cost_flaming_gloves );			// Flaming Gloves of Warmth
	ITEM_COST[ITEM_RING]	    = get_pcvar_num( CVAR_wc3_cost_ring );			// Ring of Regeneration + 1
	ITEM_COST[ITEM_CHAMELEON]	= get_pcvar_num( CVAR_wc3_cost_chameleon );			// Chameleon
	ITEM_COST[ITEM_MOLE]	    = get_pcvar_num( CVAR_wc3_cost_mole );		// Mole
	//Для shopmenu3
	ITEM_COST[ITEM_INTELLECT]	    = get_pcvar_num( CVAR_wc3_cost_intellect );		// Intellect
	ITEM_COST[ITEM_GRENADE_PROTECTION]	= get_pcvar_num( CVAR_wc3_cost_he_protect );	// Grenade Protection
	ITEM_COST[ITEM_MIRROR_SHIELD]	    = get_pcvar_num( CVAR_wc3_cost_mirror_shield );	// Mirror Shield
	ITEM_COST[ITEM_ESP]					= get_pcvar_num( CVAR_wc3_cost_esp );	// ESP
	ITEM_COST[ITEM_FROST_ARMOR]			= get_pcvar_num( CVAR_wc3_cost_frost_armor );	// Frost Armor
	ITEM_COST[ITEM_DEATH_TOUCH]			= get_pcvar_num( CVAR_wc3_cost_death_touch );	//Death's Touch
	ITEM_COST[ITEM_HEALTH_POTION]	    = get_pcvar_num( CVAR_wc3_cost_health_potion );	// Health Potion
	ITEM_COST[ITEM_ULTIMATE_TIMER_DIVIDER]	= get_pcvar_num( CVAR_wc3_cost_ult_timer );	// Ultimate Timer Divider
	ITEM_COST[ITEM_STEEL_SKIN]   = get_pcvar_num( CVAR_wc3_cost_steel_skin );				// Steel Skin

	//Для shopmenu4
	sm4_ITEM_InitCost();

}

// Item Setup Functions
ITEM_Init()
{	
	// Items are chargeable
	//Предметы с зарядами
	g_iFlag[ITEM_NECKLACE]	|= ITEM_CHARGEABLE;
	g_iFlag[ITEM_HELM]		|= ITEM_CHARGEABLE;
	g_iFlag[ITEM_RING]		|= ITEM_CHARGEABLE;

	//Для shopmenu3
	g_iFlag[ITEM_GRENADE_PROTECTION] |= ITEM_CHARGEABLE;
	g_iFlag[ITEM_DEATH_TOUCH] |= ITEM_CHARGEABLE;

	//Для shopmenu4
	g_iFlag[ITEM_DEVOURER_LIGHT] |= ITEM_CHARGEABLE;
	g_iFlag[ITEM_MIRROR_ULTIMATE] |= ITEM_CHARGEABLE;
	g_iFlag[ITEM_DEATHCOIL] |= ITEM_CHARGEABLE;
	g_iFlag[ITEM_IMPALE] |= ITEM_CHARGEABLE;
	g_iFlag[ITEM_SLEEP] |= ITEM_CHARGEABLE;
	g_iFlag[ITEM_BOOK_DEAD] |= ITEM_USEONBUY;
	g_iFlag[ITEM_TRANQUILITY] |= ITEM_USEONBUY;

	// Items should be used when bought
	//Предмет используется сразу при покупке
	g_iFlag[ITEM_TOME]		|= ITEM_USEONBUY;
	g_iFlag[ITEM_HEALTH_POTION]		|= ITEM_USEONBUY;
	g_iFlag[ITEM_STEEL_SKIN]		|= ITEM_USEONBUY;

	// Items CAN be bought when dead
	//Предметы может покупать когда мертв
//	g_iFlag[ITEM_ANKH]		|= ITEM_CHARGEABLE;
	g_iFlag[ITEM_ANKH]		|= ITEM_BUYWHENDEAD;
	g_iFlag[ITEM_SCROLL]	|= ITEM_BUYWHENDEAD;
	g_iFlag[ITEM_MOLE]		|= ITEM_BUYWHENDEAD;
	g_iFlag[ITEM_TOME]		|= ITEM_BUYWHENDEAD;

	// Items are used when the next round starts...
	//Предметы используются когда стартует следующий раунд
	g_iFlag[ITEM_ANKH]		|= ITEM_NEXTROUNDUSE;
	g_iFlag[ITEM_MOLE]		|= ITEM_NEXTROUNDUSE;
}

// We created this to allow for different prices of items at different levels
ITEM_Cost( idUser, iItem )
{
	// How much should ratio increase per level?
	//Насколько отношение должно увеличиться за уровень?
	new Float:fInc = ( 1.0 - ITEM_COST_RATIO ) / float( MAX_LEVELS );

	// Cost * ratio (ITEM_COST_RATIO to 1.0 dependent on level)
	// Стоимость * отношение (ITEM_COST_RATIO к 1.0 зависящий на уровне)
	new Float:fRatio = (float( p_data[idUser][P_LEVEL] ) * fInc) + ITEM_COST_RATIO;
		
	//Сниженные цены для админов и VIP игроков
	new Float:fPercent = get_pcvar_float( CVAR_wc3_cost_percent_vip );

	if((get_user_flags(idUser) & fBuyItemCost()))
		return ( p_data[idUser][P_RACE] != RACE_NONE ) ? floatround( (float( ITEM_COST[iItem] ) * fRatio) * fPercent ) : ITEM_COST[iItem];

	return ( p_data[idUser][P_RACE] != RACE_NONE ) ? floatround( float( ITEM_COST[iItem] ) * fRatio ) : ITEM_COST[iItem];
}


//Для авто закупки предметов
ITEM_CanBuy(idUser,iItem, bool:bIsAutoBuy = false )
{
	// User doesn't have the money
	if ( SHARED_GetUserMoney( idUser ) < ITEM_Cost( idUser, iItem ) )
	{
		if(!bIsAutoBuy)
		{
			client_print( idUser, print_center, "%L", LANG_PLAYER, "INSUFFICIENT_FUNDS" );
			client_cmd(idUser, "speak %s", g_szSounds[SOUND_NO_GOLD] );
		}

		return false;
	}
	
	// User already owns the item and it's not a chargeable item!
	else if ( ITEM_Has( idUser, iItem ) > ITEM_NONE && !ITEM_CheckFlag( iItem, ITEM_CHARGEABLE ) )
	{
		if(!bIsAutoBuy)
			client_print( idUser, print_center, "%L", LANG_PLAYER, "ALREADY_OWN_THAT_ITEM" );

		return false;
	}
	
	// Make sure these items can be bought if the user is dead
	else if ( !is_user_alive( idUser ) && !ITEM_CheckFlag( iItem, ITEM_BUYWHENDEAD ) ) 
	{
		if(!bIsAutoBuy)
			client_print( idUser, print_center, "%L", LANG_PLAYER, "NOT_PURCHASE_WHEN_DEAD" );

		return false;
	}
	
	// User has necklace + blink, they don't need a necklace
	else if ( iItem == ITEM_NECKLACE && p_data_b[idUser][PB_WARDENBLINK] )
	{
		if(!bIsAutoBuy)
			client_print( idUser, print_center, "%L", LANG_PLAYER, "CLIENT_PRINT_ITEM_CAN_BUY_0" );

		return false;
	}

	// User doesn't need an ankh if they're going to reincarnate
	else if ( iItem == ITEM_ANKH && SM_GetSkillLevel( idUser, SKILL_REINCARNATION ) == 3 && !p_data[idUser][P_CHANGERACE] && p_data[idUser][P_RACE] != RACE_CHAMELEON )
	{
		if(!bIsAutoBuy)
			client_print( idUser, print_center, "%L", LANG_PLAYER, "CLIENT_PRINT_ITEM_CAN_BUY_1" );

		return false;
	}
	
	// User has purchased the maximum allowed rings
	else if ( g_iTotalRings[idUser] >= 5 && iItem == ITEM_RING )
	{
		if(!bIsAutoBuy)
			client_print( idUser, print_center, "%L", LANG_PLAYER, "NOT_PURCHASE_MORE_THAN_FIVE_RINGS" );

		return false;
	}

	// User has purchased gloves when they're disabled on this map
	else if ( iItem == ITEM_GLOVES && g_bGlovesDisabled )
	{
		if(!bIsAutoBuy)
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
					LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
					LANG_PLAYER,"CLIENT_PRINT_ITEM_CAN_BUY_2");	
			
			cssbColoredPrint(idUser, szMessage);
		}

		return false;
	}

	// User is already going to reincarnate weapons because they bought an ankh earlier (probably lost it when died)
	else if ( ( iItem == ITEM_ANKH && g_bPlayerBoughtAnkh[idUser] ) || ( iItem == ITEM_MOLE && g_bPlayerBoughtMole[idUser] ) )
	{
		if(!bIsAutoBuy)
			client_print( idUser, print_center, "%L", LANG_PLAYER, "ALREADY_OWN_THAT_ITEM" );

		return false;
	}



	//Ограничение на покупку предмета Прикосновение смерти
	else if ( ( iItem == ITEM_DEATH_TOUCH && arrIsByuItemOneRound[idUser][iItem] == BUY_ITEM_ONE_ROUND_DEATH_TOUCH ))
	{
		if(!bIsAutoBuy)
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_LIMIT_BUY_ONE_ROUND",BUY_ITEM_ONE_ROUND_DEATH_TOUCH);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}

		return false;
	}

	//Ограничение на покупку предмета Микстура здоровья
	else if ( ( iItem == ITEM_HEALTH_POTION && arrIsByuItemOneRound[idUser][iItem] == BUY_ITEM_ONE_ROUND_HEALTH_POTION ))
	{
		if(!bIsAutoBuy)
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_LIMIT_BUY_ONE_ROUND",BUY_ITEM_ONE_ROUND_HEALTH_POTION);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}

		return false;
	}

	//Ограничение на покупку предмета Стальная кожа
	else if ( ( iItem == ITEM_STEEL_SKIN && arrIsByuItemOneRound[idUser][iItem] == BUY_ITEM_ONE_ROUND_STEEL_SKIN ))
	{
		if(!bIsAutoBuy)
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_LIMIT_BUY_ONE_ROUND",BUY_ITEM_ONE_ROUND_STEEL_SKIN);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}

		return false;
	}
	//Для shopmenu4
	//Проверка заблокирован предмет для карты или нет
	new szItemName[32];
	if(sm4_DisabledItemOnMap(idUser,iItem,szItemName) == true)
	{
		if(!bIsAutoBuy)
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_ITEM_DISABLED_ON_MAP",szItemName);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}

		return false;
	}
	//Если покупатель предмета сам является скелетом
	if(sm4_IsPlayerSkeleton(idUser) == true)
	{
		if(!bIsAutoBuy)
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_NO_BUY_SKELETON");
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}

		return false;
	}
	//Ограничение на покупку предмета Книга мертвых
	else if ( ( iItem == ITEM_BOOK_DEAD && arrIsByuItemOneRound[idUser][iItem] == BUY_ITEM_ONE_ROUND_BOOK_DEAD ))
	{
		if(!bIsAutoBuy)
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_LIMIT_BUY_ONE_ROUND",BUY_ITEM_ONE_ROUND_BOOK_DEAD);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}

		return false;
	}
	//Ограничение на покупку предмета Спокойствие
	else if ( ( iItem == ITEM_TRANQUILITY && arrIsByuItemOneRound[idUser][iItem] == BUY_ITEM_ONE_ROUND_TRANQUILITY ))
	{
		if(!bIsAutoBuy)
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_LIMIT_BUY_ONE_ROUND",BUY_ITEM_ONE_ROUND_TRANQUILITY);
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}

		return false;
	}
	else if( iItem == ITEM_TRANQUILITY && arrPlayerInfo[idUser][fCurrentSpeed] > MAX_CASTING_VELOCITY)
	{
		if(!bIsAutoBuy)
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_NO_MOVING_TRANQUILITY");
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}

		return false;
	}
	else if( (iItem == ITEM_TRANQUILITY && arrPlayerInfo[idUser][bCastingSpell] == true) || ( iItem == ITEM_TRANQUILITY && task_exists( TASK_TRANQUILITY + idUser )) )
	{
		if(!bIsAutoBuy)
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_NO_BUY_TRANQUILITY");
			cssbChatColor(idUser,"%s%s",fTagWar3ft(),szMessage);
		}

		return false;
	}


	return true;
}
//Для авто закупки предметов
ITEM_Buy( idUser, iItem, bool:bIsAutoBuy = false )
{
	
	// If the user can buy this item...
	if ( ITEM_CanBuy( idUser,iItem,bIsAutoBuy ) )
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
	
		// We're clear!

		// Remove user's money
		new iNewMoney = SHARED_GetUserMoney( idUser ) - ITEM_Cost( idUser, iItem );
		SHARED_SetUserMoney( idUser, iNewMoney );

		ITEM_GiveItem( idUser, iItem );
	}

	return;
}

// Item Buy Functions
bool:ITEM_MenuCanBuyCheck( idUser )
{
	// Duh how can they buy if they're dead!
	if ( !p_data_b[idUser][PB_ISCONNECTED] )
	{
		return false;
	}

	new isPlayerAlive		= is_user_alive( idUser );

	if ( !get_pcvar_num( CVAR_wc3_buy_dead ) && !isPlayerAlive )
	{
		client_print( idUser, print_center, "%L", LANG_PLAYER, "NOT_BUY_ITEMS_WHEN_DEAD" );
		
		return false;
	}
	
	else if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		new isPlayerInBuyZone	= cs_get_user_buyzone( idUser )
		
		if ( get_pcvar_num( CVAR_wc3_buy_time ) && !g_buyTime )
		{
			client_print( idUser, print_center, "%0.0f %L",(get_cvar_float( "mp_buytime" ) * 60.0 ), LANG_PLAYER, "SECONDS_HAVE_PASSED_CANT_BUY");
													
			return false;
		}
		
		else if ( get_pcvar_num( CVAR_wc3_buy_zone ) && !isPlayerInBuyZone && isPlayerAlive )
		{
			client_print( idUser, print_center, "%L", LANG_PLAYER, "MUST_BE_IN_BUYZONE" );
			
			return false;
		}
	}
	
	return true;
}

// Item Preset Function
ITEM_GiveItem( idUser, iItem )
{

	// This item we should use instantly
	if ( ITEM_CheckFlag( iItem, ITEM_USEONBUY ) )
	{
		if ( iItem == ITEM_TOME )
		{
			ITEM_Tome( idUser );
			return;
		}

		if ( iItem == ITEM_HEALTH_POTION )
		{
			ITEM_HealthPotion( idUser );
			return;
		}

		if ( iItem == ITEM_STEEL_SKIN )
		{
			ITEM_SteelSkin( idUser );
			return;
		}

		//Для shopmenu4
		if ( iItem == ITEM_BOOK_DEAD )
		{
			sm4_ITEM_Book_Dead(idUser,iItem);
			return;
		}
		if ( iItem == ITEM_TRANQUILITY )
		{
			sm4_4_9_Tranquility( idUser,iItem );
			return;
		}
		
	}

	// They are just adding some charges
	else
	{
		// Actually set our item variable
		if ( is_user_alive( idUser ) )
		{
			ITEM_Equip( idUser, iItem );
		}

		// Display message to the user regarding the item they just purchased
		ITEM_DisplayMessage( idUser, iItem );

		// Give bonuses
		ITEM_GiveBonuses( idUser, iItem );

		// Play purchase sound
		emit_sound( idUser, CHAN_STATIC, g_szSounds[SOUND_PICKUPITEM], 1.0, ATTN_NORM, 0, PITCH_NORM );
	}

	WC3_ShowBar( idUser );

	return;
}

ITEM_DisplayMessage( idUser, iItem )
{
	// Display a message regarding the item they just purchased
	switch ( iItem )
	{
		case ITEM_ANKH:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
			LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
			LANG_PLAYER,"INFO_SHOPMENU_1");	
				
			(g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO) ? cssbColoredPrint(idUser, szMessage) : 0;
		}

		case ITEM_BOOTS:
		{
			if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
			{
				format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%0.1f%%",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU_2",
				( (get_pcvar_float(CVAR_wc3_boots)) * 100.00 ));	
			
				cssbColoredPrint(idUser, szMessage);
			}
		}

		case ITEM_CLAWS:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%d",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU_3",
				get_pcvar_num( CVAR_wc3_claw ));	
			
			cssbColoredPrint(idUser, szMessage);
		}

		case ITEM_CLOAK:
		{
			new Float:fInvis = 100.0 * ( float( get_pcvar_num( CVAR_wc3_cloak ) ) / 255.0 );
			
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%0.1f%%",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU_4",
				fInvis);	
				
			cssbColoredPrint(idUser, szMessage);				
		}

		case ITEM_MASK:
		{
			new Float:fMask = ( 100.0 * get_pcvar_float( CVAR_wc3_mask ) );
		
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%0.1f%%",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU_5",
				fMask);	
				
			cssbColoredPrint(idUser, szMessage);
		}

		case ITEM_NECKLACE:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%d",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU_6",
				NECKLACE_CHARGES);	
				
			cssbColoredPrint(idUser, szMessage);
		}

		case ITEM_FROST:
		{
			new Float:fFrost = 100.0 * ( get_pcvar_float( CVAR_wc3_frost ) / 260.0 );
			
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%0.1f%%",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU_7",
				fFrost);	
				
			cssbColoredPrint(idUser, szMessage);						
		}

		case ITEM_HEALTH:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%d",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU_8",
				get_pcvar_num( CVAR_wc3_health ));	
				
			cssbColoredPrint(idUser, szMessage);
		}

		case ITEM_SCROLL:
		{
			if ( is_user_alive( idUser ) )
			{
				format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
					LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
					LANG_PLAYER,"INFO_SHOPMENU2_1");	
			}

			else
			{
				format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
					LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
					LANG_PLAYER,"INFO_SHOPMENU2_1_DEAD");	
			}
			
				cssbColoredPrint(idUser, szMessage);			
		}

		case ITEM_PROTECTANT:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU2_2");
			
			cssbColoredPrint(idUser, szMessage);								
		}

		case ITEM_HELM:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%d",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU2_3",
				HELM_CHARGES);	
				
			cssbColoredPrint(idUser, szMessage);
		}

		case ITEM_AMULET:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU2_4");
			
			cssbColoredPrint(idUser, szMessage);											
		}

		case ITEM_SOCK:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU2_5");
			
			cssbColoredPrint(idUser, szMessage);											
		}

		case ITEM_GLOVES:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%d ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU2_6",
				get_pcvar_num( CVAR_wc3_glove_timer ),
				LANG_PLAYER,"WC3_SECONDS");	
				
			cssbColoredPrint(idUser, szMessage);						
		}

		case ITEM_RING:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU2_7");
			
			cssbColoredPrint(idUser, szMessage);
		}

		case ITEM_CHAMELEON:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU2_8");
			
			cssbColoredPrint(idUser, szMessage);											
		}

		case ITEM_MOLE:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU2_9");
			
			cssbColoredPrint(idUser, szMessage);											
		}

		//Для shopmenu3
		case ITEM_INTELLECT:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU3_1",
				BONUS_INTELLECT);
			
			cssbColoredPrint(idUser, szMessage);											
		}
		case ITEM_GRENADE_PROTECTION:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU3_2");
			
			cssbColoredPrint(idUser, szMessage);											
		}
		case ITEM_MIRROR_SHIELD:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU3_3",
				floatround(OFFENSIVE_MIRROR_SHIELD * 100),"%%",floatround(DEFENSIVE_MIRROR_SHIELD * 100),"%%");
			
			cssbColoredPrint(idUser, szMessage);											
		}
		case ITEM_ESP:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU3_4");
			
			cssbColoredPrint(idUser, szMessage);											
		}
		case ITEM_FROST_ARMOR:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU3_5",
				floatround(FROST_ARMOR_DAMAGE * ARMOR_RATIO),
				floatround(FROST_ARMOR_DAMAGE * ARMOR_BONUS));
			
			cssbColoredPrint(idUser, szMessage);											
		}
		case ITEM_DEATH_TOUCH:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU3_6");
			
			cssbColoredPrint(idUser, szMessage);											
		}
		case ITEM_HEALTH_POTION:
		{
			/*format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU3_7");
			
			cssbColoredPrint(idUser, szMessage);*/											
		}
		case ITEM_ULTIMATE_TIMER_DIVIDER:
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU3_8",
				RESET_ULTIMATE_TIMER);
			
			cssbColoredPrint(idUser, szMessage);											
		}
		case ITEM_STEEL_SKIN:
		{
		/*	format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU3_9");
			
			cssbColoredPrint(idUser, szMessage);	*/										
		}
		
	}

	//Для shopmenu4
	sm4_ITEM_DisplayMessage( idUser, iItem );
}

// Give the user bonuses for their items (except charges)
ITEM_GiveAllBonuses( idUser )
{
	//Для talisman +
	//Если изменяем на другой
	if(arrTalismanChange[idUser] != TALISMAN_ITEM_NONE)
	{
		arrTalismanActive[idUser] = arrTalismanChange[idUser];

	//	client_print(idUser,print_chat,"CHANGE GB: %d",arrTalismanChange[idUser]);

		fActivateTalismanPassive(idUser, arrTalismanChange[idUser]);

		arrTalismanChange[idUser] = TALISMAN_ITEM_NONE;
	}
	else
	{
		fActivateTalismanPassive(idUser, arrTalismanActive[idUser]);

		//client_print(idUser,print_chat,"CURRENT GB: %d",arrTalismanActive[idUser]);
	}

	//client_print(0,print_chat,"ITEM_GiveAllBonuses: %d",arrTalismanActive[idUser]);

	// Loop through all item slots
	for ( new i = ITEM_SLOT_ONE; i <= ITEM_SLOT_THREE; i++ )
	{
		
		// Do we have a valid item here?
		if ( g_iShopMenuItems[idUser][i] != ITEM_NONE )
		{

			// Don't want to give the user more charges for free do we?
			//  And we don't want to give the bonuses if this is a next round use item (i.e. if we do then mole for infinity - that doesn't seem nice)
			if ( !ITEM_CheckFlag( g_iShopMenuItems[idUser][i], ITEM_CHARGEABLE ) && !ITEM_CheckFlag( g_iShopMenuItems[idUser][i], ITEM_NEXTROUNDUSE ) )
			{
				ITEM_GiveBonuses( idUser, g_iShopMenuItems[idUser][i] );
			}
		}
	}
}

// Give our players their bonus!
ITEM_GiveBonuses( idUser, iItem )
{
		
	// Display a message regarding the item they just purchased
	switch ( iItem )
	{
		case ITEM_ANKH:
		{
			g_bPlayerBoughtAnkh[idUser] = true;
		}

		case ITEM_BOOTS:
		{
			SHARED_SetSpeed( idUser );
		}

		case ITEM_CLOAK:
		{
			SHARED_INVIS_Set( idUser );
		}

		case ITEM_NECKLACE:
		{
			g_iNecklaceCharges[idUser] += NECKLACE_CHARGES;
		}

		case ITEM_HEALTH:
		{
			new iHealth = get_pcvar_num( CVAR_wc3_health );
			iHealth += get_user_health( idUser );

			if (iHealth > 255)
				set_user_health(idUser, 255 );
			else
				set_user_health( idUser, iHealth );
		}

		case ITEM_SCROLL:
		{
			if ( !is_user_alive( idUser ) )
			{
				ITEM_Scroll( idUser );
			}
		}

		case ITEM_HELM:
		{
			g_iHelmCharges[idUser] += HELM_CHARGES;
		}

		case ITEM_SOCK:
		{
			SHARED_SetGravity( idUser );
		}

		case ITEM_GLOVES:
		{
			ITEM_Gloves( idUser );
		}

		case ITEM_RING:
		{
			g_iTotalRings[idUser] += RING_INCREMENT;

			if ( !task_exists( TASK_ITEM_RING + idUser ) )
			{
				_ITEM_Ring( idUser );
			}
		}

		case ITEM_CHAMELEON:
		{
			SHARED_ChangeSkin( idUser, SKIN_SWITCH );
		}

		case ITEM_MOLE:
		{
			g_bPlayerBoughtMole[idUser] = true;
		}

		//Для shopmenu3
		case ITEM_INTELLECT:
		{
		}
		case ITEM_GRENADE_PROTECTION:
		{
			arriTotalRingsGrenadeProtection[idUser] += GRENADE_PROTECTION_CHARGES;
		}
		case ITEM_MIRROR_SHIELD:
		{
		}
		case ITEM_ESP:
		{
			fEnableItemESP(idUser);
		}
		case ITEM_FROST_ARMOR:
		{
		}
		case ITEM_DEATH_TOUCH:
		{
			arrDeathTouch[idUser] = true;
			arrIsByuItemOneRound[idUser][ITEM_DEATH_TOUCH]++;
		}
		case ITEM_HEALTH_POTION:
		{
		}
		case ITEM_ULTIMATE_TIMER_DIVIDER:
		{
		}
		case ITEM_STEEL_SKIN:
		{
		}
	}

	//Для shopmenu4
	sm4_ITEM_GiveBonuses( idUser, iItem );
}

// Item Equip Function
ITEM_Equip( idUser, iItem )
{
	new iItemSlot = ITEM_GetSlot( idUser );

	// Items are not full
	if ( iItemSlot != ITEM_SLOT_FULL )
	{

		new iOldItem = g_iShopMenuItems[idUser][iItemSlot];

		if ( iItem == iOldItem || ITEM_Has( idUser, iItem ) > ITEM_NONE )
		{
			// Might hit this if we added charges - we want to update the user's HUD
			WC3_ShowBar( idUser );

			return;
		}

		// Remove the user's old item if necessary
		else if ( g_iShopMenuItems[idUser][iItemSlot] > ITEM_NONE )
		{
			ITEM_Remove( idUser, iItemSlot );
		}
			
		// Set their new item
		g_iShopMenuItems[idUser][iItemSlot] = iItem;
	}

	WC3_ShowBar( idUser );

	return;
}

// Item Remove Functions
ITEM_RemoveID( idUser, iItem )
{
	new iItemSlot = ITEM_Has( idUser, iItem );

	if ( iItemSlot > ITEM_NONE )
	{
		ITEM_Remove( idUser, iItemSlot );
	}

	return;
}

ITEM_Remove( idUser, iItemSlot, bResetAnkhMole = true )
{
	new iItem = g_iShopMenuItems[idUser][iItemSlot];

	g_iShopMenuItems[idUser][iItemSlot] = ITEM_NONE;

	switch( iItem )
	{
		case ITEM_ANKH:
		{
			if ( bResetAnkhMole )
			{
				g_bPlayerBoughtAnkh[idUser] = false;
			}
		}

		case ITEM_BOOTS:
		{
			SHARED_SetSpeed( idUser );
		}

		case ITEM_CLOAK:
		{
			SHARED_INVIS_Set( idUser );
		}

		case ITEM_NECKLACE:
		{
			g_iNecklaceCharges[idUser] = 0;
		}

		case ITEM_HEALTH:
		{
			new iNewHealth = get_user_health( idUser ) - get_pcvar_num( CVAR_wc3_health );
			
			// Lets not kill the user, give them 1 health
			iNewHealth = ( ( iNewHealth <= 0 ) ? 1 : iNewHealth );

			if (iNewHealth > 255)
				set_user_health(idUser, 255 );
			else
				set_user_health( idUser, iNewHealth );
		}

		case ITEM_HELM:
		{
			g_iHelmCharges[idUser] = 0;
		}
		
		case ITEM_SOCK:
		{
			SHARED_SetGravity( idUser );
		}

		case ITEM_GLOVES:
		{
			if ( task_exists( TASK_ITEM_GLOVES + idUser ) )
			{
				remove_task( TASK_ITEM_GLOVES + idUser );
			}
		}

		case ITEM_RING:
		{
			if ( task_exists( TASK_ITEM_RING + idUser ) )
			{
				remove_task( TASK_ITEM_RING + idUser );
			}
			
			// Set the number of rings to 0
			g_iTotalRings[idUser] = 0;
		}

		case ITEM_CHAMELEON:
		{
			SHARED_ChangeSkin( idUser, SKIN_RESET );
		}

		case ITEM_MOLE:
		{
			if ( bResetAnkhMole )
			{
				g_bPlayerBoughtMole[idUser] = false;
			}
		}

		//Для shopmenu3
		case ITEM_INTELLECT:
		{
		}
		case ITEM_GRENADE_PROTECTION:
		{
			arriTotalRingsGrenadeProtection[idUser] = 0;
		}
		case ITEM_MIRROR_SHIELD:
		{
		}
		case ITEM_ESP:
		{
			fDisableItemESP(idUser);
		}
		case ITEM_FROST_ARMOR:
		{
		}
		case ITEM_DEATH_TOUCH:
		{
			arrDeathTouch[idUser] = false;
		}
		case ITEM_HEALTH_POTION:
		{
		}
		case ITEM_ULTIMATE_TIMER_DIVIDER:
		{
		}
		case ITEM_STEEL_SKIN:
		{
		}
	}

	//Для shopmenu4
	sm4_ITEM_Remove( idUser,iItem);

	WC3_ShowBar( idUser );

	return;
}

ITEM_RemoveCharge( idUser, iItem )
{
	//Для talisman
	fTalismanRemoveCharge(idUser);

	if ( ITEM_Has( idUser, iItem ) > ITEM_NONE )//Есть ли предмет чтобы удалить заряд
	{
		switch ( iItem )
		{
			case ITEM_NECKLACE:
			{
				g_iNecklaceCharges[idUser] -= CHARGE_DISPOSE;
				
				if ( g_iNecklaceCharges[idUser] <= 0 )
				{
					ITEM_RemoveID( idUser, iItem );
				}
			}

			case ITEM_HELM:
			{
				g_iHelmCharges[idUser] -= CHARGE_DISPOSE;
				
				if ( g_iHelmCharges[idUser] <= 0 )
				{
					ITEM_RemoveID( idUser, iItem );
				}
			}

			case ITEM_RING:
			{
				g_iTotalRings[idUser] -= CHARGE_DISPOSE;
				
				if ( g_iTotalRings[idUser] <= 0 )
				{
					ITEM_RemoveID( idUser, iItem );
				}
			}
			//Для shopmenu3
			case ITEM_GRENADE_PROTECTION:
			{
				arriTotalRingsGrenadeProtection[idUser] -= CHARGE_DISPOSE;
				
				if (arriTotalRingsGrenadeProtection[idUser] <= 0 )
				{
					ITEM_RemoveID( idUser, iItem );
				}
			}
		}

		//Для shopmenu4
		sm4_ITEM_RemoveCharge(idUser,iItem);
	}

	WC3_ShowBar( idUser );
	return;
}


//Проверка свободен ли слот
// Item Get Functions
ITEM_GetSlot( idUser )
{
	if (g_iShopMenuItems[idUser][ITEM_SLOT_ONE] > ITEM_NONE && 
		g_iShopMenuItems[idUser][ITEM_SLOT_TWO] > ITEM_NONE &&
		g_iShopMenuItems[idUser][ITEM_SLOT_THREE] > ITEM_NONE)
		return ITEM_SLOT_FULL;

	if (g_iShopMenuItems[idUser][ITEM_SLOT_ONE] > ITEM_NONE && 
		g_iShopMenuItems[idUser][ITEM_SLOT_TWO] == ITEM_NONE &&
		g_iShopMenuItems[idUser][ITEM_SLOT_THREE] == ITEM_NONE)
		return ITEM_SLOT_TWO;

	if (g_iShopMenuItems[idUser][ITEM_SLOT_ONE] > ITEM_NONE && 
		g_iShopMenuItems[idUser][ITEM_SLOT_TWO] == ITEM_NONE &&
		g_iShopMenuItems[idUser][ITEM_SLOT_THREE] > ITEM_NONE)
		return ITEM_SLOT_TWO;

	if (g_iShopMenuItems[idUser][ITEM_SLOT_ONE] > ITEM_NONE && 
		g_iShopMenuItems[idUser][ITEM_SLOT_TWO] > ITEM_NONE &&
		g_iShopMenuItems[idUser][ITEM_SLOT_THREE] == ITEM_NONE)
		return ITEM_SLOT_THREE;

	return ITEM_SLOT_ONE;//Первый свободен если все не заняты
}

ITEM_Has( idUser, iItem )
{
	if ( g_iShopMenuItems[idUser][ITEM_SLOT_ONE] == iItem )
		return ITEM_SLOT_ONE;

	if ( g_iShopMenuItems[idUser][ITEM_SLOT_TWO] == iItem )
		return ITEM_SLOT_TWO;

	if ( g_iShopMenuItems[idUser][ITEM_SLOT_THREE] == iItem )
		return ITEM_SLOT_THREE;

	return ITEM_NONE;
}

// Item Death Function
//Удаление всех предметов когда игрок умирает
ITEM_UserDied( idUser )
{
	// The user just died, remove all items
	if ( g_iShopMenuItems[idUser][ITEM_SLOT_ONE] > ITEM_NONE )
	{
		ITEM_Remove( idUser, ITEM_SLOT_ONE, false );
	}

	if ( g_iShopMenuItems[idUser][ITEM_SLOT_TWO] > ITEM_NONE )
	{
		ITEM_Remove( idUser, ITEM_SLOT_TWO, false );
	}

	if ( g_iShopMenuItems[idUser][ITEM_SLOT_THREE] > ITEM_NONE )
	{
		ITEM_Remove( idUser, ITEM_SLOT_THREE, false );
	}
	
}

// Item Specific Functions
ITEM_Offensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace )
{

	//Для talisman +
	// Claws of Attack
	if ( ITEM_Has( iAttacker, ITEM_CLAWS ) > ITEM_NONE || fWorkTalismanPassive(iAttacker, arrTalismanActive[iAttacker]) == TALISMAN_ITEM_CLAWS)
	{	
		WC3_Damage( iVictim, iAttacker, get_pcvar_num( CVAR_wc3_claw ), iWeapon, iHitPlace );
		
		SHARED_Glow( iAttacker, (2 * get_pcvar_num( CVAR_wc3_claw ) ), 0, 0, 0 );

		Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 0, 0, g_GlowLevel[iVictim][0] );
	}

	//Для talisman +
	// Mask of Death
	if ( ITEM_Has( iAttacker, ITEM_MASK ) > ITEM_NONE && p_data_b[iAttacker][PB_ISCONNECTED] ||
		 fWorkTalismanPassive(iAttacker, arrTalismanActive[iAttacker]) == TALISMAN_ITEM_MASK && p_data_b[iAttacker][PB_ISCONNECTED])
	{
		new iHealth = get_user_health( iAttacker );
		new iBonusHealth = floatround( float( iDamage ) * get_pcvar_float( CVAR_wc3_mask ) );
		
		new iVampiricBonus = p_data_b[iAttacker][PB_HEXED] ? 0 : SM_GetSkillLevel( iAttacker, SKILL_VAMPIRICAURA );

		// Then the user already gets a bonus, lets lower the total amount the user is going to get
		if ( iVampiricBonus > 0 )
		{
			iBonusHealth /= iVampiricBonus;
		}
		
		// User needs to be set to max health
		if ( iHealth + iBonusHealth > get_user_maxhealth( iAttacker ) )
		{
				if (get_user_maxhealth( iAttacker ) > 255)
					set_user_health(iAttacker, 255 );
				else
					set_user_health( iAttacker, get_user_maxhealth( iAttacker ) );
		}
		
		// Give them bonus
		else
		{
				if (iHealth + iBonusHealth > 255)
					set_user_health(iAttacker, 255 );
				else
					set_user_health( iAttacker, iHealth + iBonusHealth );
		}

		SHARED_Glow( iAttacker, 0, iBonusHealth, 0, 0 );

		Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 0, 255, 0, g_GlowLevel[iAttacker][1] );
	}

	//Для talisman +
	// Orb of Frost
	if ( ITEM_Has( iAttacker, ITEM_FROST ) > ITEM_NONE ||
		 fWorkTalismanPassive(iAttacker, arrTalismanActive[iAttacker]) == TALISMAN_ITEM_FROST)
	{
		// Only slow them if they aren't slowed/stunned already
		if ( !SHARED_IsPlayerSlowed( iVictim ) )
		{

			p_data_b[iVictim][PB_SLOWED]	= true;

			SHARED_SetSpeed( iVictim );

			set_task( 1.0, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + iVictim );

			SHARED_Glow( iAttacker, 0, 0, 0, 100 );

			Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 255, 255, g_GlowLevel[iVictim][3] );
		}
	}
}

ITEM_Tome( idUser )
{
	new iXp = get_pcvar_num( CVAR_wc3_tome ) + XP_GivenByLevel( p_data[idUser][P_LEVEL] );
	
	new iBonusXP = XP_Give( idUser, iXp );

	if ( iBonusXP != 0 )
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%d ^x04XP",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU_9",
				iBonusXP);	
				
		cssbColoredPrint(idUser, szMessage);		
	}

	emit_sound( idUser, CHAN_STATIC, "warcraft3/Tomes.wav", 1.0, ATTN_NORM, 0, PITCH_NORM );

	return;
}

ITEM_Gloves( idUser )
{
	if ( !WC3_Check() )
	{
		return;
	}

	if ( !SHARED_HasGrenade( idUser ) )
	{
		g_iGloveTimer[idUser] = 0;

		_ITEM_Glove_Give( idUser );
	}

	return;
}

ITEM_Glove_Begin( idUser )
{
	// Then lets start a timer to give them a grenade!
	g_iGloveTimer[idUser] = get_pcvar_num( CVAR_wc3_glove_timer );

	format(szMessage, sizeof(szMessage) - 1, "%d %L",g_iGloveTimer[idUser],LANG_PLAYER,"UNTIL_YOUR_NEXT_GRENADE");
	WC3_StatusText( idUser, TXT_TIMER,szMessage);

	g_iGloveTimer[idUser]--;

	set_task( 1.0, "_ITEM_Glove_Give", TASK_ITEM_GLOVES + idUser );
}

public _ITEM_Glove_Give( idUser )
{
	if ( !WC3_Check() )
	{
		return;
	}

	if ( idUser >= TASK_ITEM_GLOVES )
	{
		idUser -= TASK_ITEM_GLOVES;
	}

	if ( !p_data_b[idUser][PB_ISCONNECTED] || !is_user_alive( idUser ) )
	{
		return;
	}

	// Only need to save once! - this is b/c I'm not sure when the loss of a grenade is updated - and I wanted SHARED_HasGrenade to work @ all times!
	if ( g_iGloveTimer[idUser] == get_pcvar_num( CVAR_wc3_glove_timer ) - 1 )
	{
		SHARED_SaveWeapons( idUser );
	}

	// If somehow they already got a grenade - stop this!
	/*new bool:bHasGrenade = false;
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( cs_get_user_bpammo( idUser, CSW_HEGRENADE ) > 0 )
		{
			bHasGrenade = true;
		}
	}
	*/

	// Lets do CS/CZ only
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Already have a grenade!!
		if ( SHARED_HasGrenade( idUser ) )
		{
			g_iGloveTimer[idUser] = 0;

			return;
		}
	}


	if ( g_iGloveTimer[idUser] > 0 )
	{
		format(szMessage, sizeof(szMessage) - 1, "%d %L",g_iGloveTimer[idUser],LANG_PLAYER,"UNTIL_YOUR_NEXT_GRENADE");
		WC3_StatusText( idUser, TXT_TIMER,szMessage);

		g_iGloveTimer[idUser]--;

		set_task( 1.0, "_ITEM_Glove_Give", TASK_ITEM_GLOVES + idUser );

		return;
	}

	// Counter-Strike or Condition Zero grenade
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		give_item( idUser, "weapon_hegrenade" );
	}
	// Display a message to the user
	WC3_StatusText( idUser, TXT_TIMER, "%L", LANG_PLAYER, "ENJOY_A_GRENADE" )

	return;
}		

ITEM_BuyRings( idUser )
{

	new iItemSlot = ITEM_GetSlot( idUser );

	// Items are full
	if ( iItemSlot == ITEM_SLOT_FULL && ITEM_Has( idUser, ITEM_RING ) == ITEM_NONE )
	{
		g_iFutureItem[idUser] = -3;

		MENU_ReplaceItem( idUser );

		return;
	}

	new iMoney;
	new iAdditionalRings = 0;
	
	while ( g_iTotalRings[idUser] + iAdditionalRings < 5 )
	{
		iMoney = SHARED_GetUserMoney( idUser );

		if ( iMoney < ITEM_Cost( idUser, ITEM_RING ) )
		{
			break;
		}

		iAdditionalRings++;
		
		new iNewMoney = iMoney - ITEM_Cost( idUser, ITEM_RING );
		SHARED_SetUserMoney( idUser, iNewMoney, 1 );
	}

	// Then we need to give them some rings!
	if ( iAdditionalRings > 0 )
	{

		// Subtract 1 b/c ITEM_GiveItem will add one
		g_iTotalRings[idUser] += ( ( iAdditionalRings * RING_INCREMENT ) - ( RING_INCREMENT ) );

		ITEM_GiveItem( idUser, ITEM_RING );
	}

	return;
}

public _ITEM_Ring( idUser )
{
	if ( !WC3_Check() )
	{
		return;
	}

	if ( idUser >= TASK_ITEM_RING )
	{
		idUser -= TASK_ITEM_RING;
	}

	//Для talisman + BEGIN {
	if ( !p_data_b[idUser][PB_ISCONNECTED] || ITEM_Has( idUser, ITEM_RING ) == ITEM_NONE && bIsActiveTalisman(idUser,TALISMAN_ITEM_RING) == false)
		return;

	new iBonusHealth;
	if(fWorkTalismanPassive(idUser, arrTalismanActive[idUser]) == TALISMAN_ITEM_RING)
		iBonusHealth = RING_MAX;
	else
		iBonusHealth = g_iTotalRings[idUser];
	//Для talisman END }

	while ( iBonusHealth > 0 )
	{
		new iHealth =  get_user_health( idUser ) + 1;

		if ( iHealth <= get_user_maxhealth( idUser ) )
		{

			if (iHealth > 255)
				set_user_health( idUser, 255 );
			else
				set_user_health( idUser, iHealth );
		}

		iBonusHealth--;
	}

	set_task( 2.0, "_ITEM_Ring", TASK_ITEM_RING + idUser );

	return;
}

ITEM_Scroll( idUser )
{
	// Make sure the user isn't about to respawn when we do these checks
	if ( !p_data[idUser][P_RESPAWNBY] )
	{
		p_data[idUser][P_RESPAWNBY] = RESPAWN_ITEM;

		set_task( SPAWN_DELAY, "_SHARED_Spawn", TASK_SPAWN + idUser );
	}
}

ITEM_CheckFlag( iItemID, iFlag )
{
	if ( g_iFlag[iItemID] & iFlag )
	{
		return true;
	}

	return false;	
}

// Format the item for WC3_ShowBar
ITEM_Format( idUser, iItem, szItemString[], iLen )
{
	new szItemName[32];
	LANG_GetItemName( iItem, idUser, szItemName, 31, true );

	// Special options
	if ( iItem == ITEM_NECKLACE )
	{
		formatex( szItemString, iLen, "%s[%d]", szItemName, g_iNecklaceCharges[idUser] );
	}

	else if ( iItem == ITEM_HELM )
	{
		formatex( szItemString, iLen, "%s[%d]", szItemName, g_iHelmCharges[idUser] );
	}

	else if ( iItem == ITEM_RING )
	{
		formatex( szItemString, iLen, "%s[%d]", szItemName, g_iTotalRings[idUser] );
	}
	//Для shopmenu3
	else if ( iItem == ITEM_GRENADE_PROTECTION )
	{
		formatex( szItemString, iLen, "%s[%d]", szItemName, arriTotalRingsGrenadeProtection[idUser] );
	}
	//Для shopmenu4
	else if ( iItem == ITEM_DEVOURER_LIGHT )
	{
		formatex( szItemString, iLen, "%s[%d]", szItemName, arriTotalRingsDevourerLight[idUser] );
	}
	else if ( iItem == ITEM_MIRROR_ULTIMATE )
	{
		formatex( szItemString, iLen, "%s[%d]", szItemName, arriTotalChargeMirrorUltimate[idUser] );
	}
	else if ( iItem == ITEM_DEATHCOIL )
	{
		formatex( szItemString, iLen, "%s[%d]", szItemName, arriTotalChargeDeathCoil[idUser] );
	}
	else if ( iItem == ITEM_IMPALE)
	{
		formatex( szItemString, iLen, "%s[%d]", szItemName, arriTotalChargeImpale[idUser] );
	}
	else if ( iItem == ITEM_SLEEP)
	{
		formatex( szItemString, iLen, "%s[%d]", szItemName, arriTotalChargeSleep[idUser] );
	}

	

	// All other cases
	else
	{
		copy( szItemString, iLen, szItemName );
	}
}

//Возвращает есть ли конкретный предмет
ITEM_HasItem( idUser, iItem )
{
	if ( g_iShopMenuItems[idUser][ITEM_SLOT_ONE] == iItem )
		return iItem;

	else if ( g_iShopMenuItems[idUser][ITEM_SLOT_TWO] == iItem )
		return iItem;

	else if ( g_iShopMenuItems[idUser][ITEM_SLOT_THREE] == iItem )
		return iItem;

	return ITEM_NONE;
}

//Сброс всех счетчиков на ограничение покупок
ITEM_LimitOneRound(idUser)
{
	arrIsByuItemOneRound[idUser][ITEM_ANKH]	    = 0;			// Ankh of Reincarnation
	arrIsByuItemOneRound[idUser][ITEM_BOOTS]       = 0;			// Boots of Speed
	arrIsByuItemOneRound[idUser][ITEM_CLAWS]	    = 0;		// Claws of Attack
	arrIsByuItemOneRound[idUser][ITEM_CLOAK]	    = 0;		// Cloak of Shadows
	arrIsByuItemOneRound[idUser][ITEM_MASK]	    = 0;			// Mask of Death
	arrIsByuItemOneRound[idUser][ITEM_NECKLACE]	= 0;			// Necklace of Immunity
	arrIsByuItemOneRound[idUser][ITEM_FROST]	    = 0;		// Orb of Frost
	arrIsByuItemOneRound[idUser][ITEM_HEALTH]	    = 0;		// Periapt of Health
	arrIsByuItemOneRound[idUser][ITEM_TOME]	    = 0;			// Tome of Experience
	arrIsByuItemOneRound[idUser][ITEM_SCROLL]	    = 0;		// Scroll of Respawning
	arrIsByuItemOneRound[idUser][ITEM_PROTECTANT]	= 0;		// Mole Protectant
	arrIsByuItemOneRound[idUser][ITEM_HELM]	    = 0;			// Helm of Excellence
	arrIsByuItemOneRound[idUser][ITEM_AMULET]	    = 0;		// Amulet of the Cat
	arrIsByuItemOneRound[idUser][ITEM_SOCK]	    = 0;			// Sock of the Feather
	arrIsByuItemOneRound[idUser][ITEM_GLOVES]	    = 0;		// Flaming Gloves of Warmth
	arrIsByuItemOneRound[idUser][ITEM_RING]	    = 0;			// Ring of Regeneration + 1
	arrIsByuItemOneRound[idUser][ITEM_CHAMELEON]	= 0;		// Chameleon
	arrIsByuItemOneRound[idUser][ITEM_MOLE]	    = 0;			// Mole
	//Для shopmenu3
	arrIsByuItemOneRound[idUser][ITEM_INTELLECT]	    = 0;		// Intellect
	arrIsByuItemOneRound[idUser][ITEM_GRENADE_PROTECTION]	= 0;	// Grenade Protection
	arrIsByuItemOneRound[idUser][ITEM_MIRROR_SHIELD]	    = 0;	// Mirror Shield
	arrIsByuItemOneRound[idUser][ITEM_ESP]					= 0;	// ESP
	arrIsByuItemOneRound[idUser][ITEM_FROST_ARMOR]			= 0;	// Frost Armor
	arrIsByuItemOneRound[idUser][ITEM_DEATH_TOUCH]			= 0;	//Death's Touch
	arrIsByuItemOneRound[idUser][ITEM_HEALTH_POTION]	    = 0;	// Health Potion
	arrIsByuItemOneRound[idUser][ITEM_ULTIMATE_TIMER_DIVIDER]	= 0;	// Ultimate Timer Divider
	arrIsByuItemOneRound[idUser][ITEM_STEEL_SKIN]   = 0;				// Steel Skin

	//Для shopmenu4
	sm4_ITEM_LimitOneRound(idUser);
}

