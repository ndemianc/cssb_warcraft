
// Forwards from the CSX module
public grenade_throw( idUser, greindex, wId )
{
	
	if ( !WC3_Check() )
	{
		return;
	}
	
	// If user isn't alive do nothing!
	if ( !is_user_alive( idUser ) )
	{
		return;
	}

	//Для talisman +
	// User has gloves!  Do we need to give them another grenade?
	if ( ITEM_Has( idUser, ITEM_GLOVES ) > ITEM_NONE || fWorkTalismanPassive(idUser, arrTalismanActive[idUser] == TALISMAN_ITEM_GLOVES))
		ITEM_Glove_Begin( idUser );

	//Для shopmenu4
	sm4_ITEM_IceBlock_Begin( idUser );
	
	static iSkillLevel;
	iSkillLevel = SM_GetSkillLevel( idUser, SKILL_CRITICALGRENADE );

	// Make sure the user has the skill and we actually have a grenade index
	if ( greindex && iSkillLevel > 0 )
	{
		
		// Then Critical Grenades are allowed
		if ( OR_CriticalGrenadeAllowed( idUser ) )
		{

			// Then draw it!
			if ( SHARED_IsGrenade( wId ) )
			{
				new iWidth = 3 * iSkillLevel;

				Create_TE_BEAMFOLLOW( greindex, g_iSprites[SPR_TRAIL], 20, iWidth, 255, 32, 32, 196 );
			}
		}
	}
	return;
}

// HamSandwich implementation
public EVENT_TakeDamage( iVictim, iWeapon, iAttacker, Float:f_Damage, damagetype )
{

	static szWeaponClassName[32];
	pev ( iWeapon, pev_classname, szWeaponClassName, charsmax ( szWeaponClassName ) );


	new s_PlayerName[32], s_PlayerName2[32];

	get_user_name( iVictim, s_PlayerName, 31 );
	get_user_name( iAttacker, s_PlayerName2, 31 );
      

	if(SHARED_ValidPlayer(iAttacker) && is_user_alive( iVictim ))
	{
		//Для talisman +
		//Grenade Protection
		if (ITEM_Has(iVictim, ITEM_GRENADE_PROTECTION ) > ITEM_NONE || fWorkTalismanPassive(iVictim, arrTalismanActive[iVictim]) == TALISMAN_ITEM_GRENADE_PROTECTION)
		{
			if(equali(szWeaponClassName,"grenade"))
			{
				ITEM_RemoveCharge(iVictim, ITEM_GRENADE_PROTECTION );
				return HAM_SUPERCEDE;
			}
		}
	}
	
/*
	if(!is_user_bot(iAttacker) || !is_user_bot(iVictim))
	{
		client_print(iAttacker,print_chat,"A wpn: %s",szWpnName);
		client_print(iVictim,print_chat,"V wpn: %s",szWpnName);
	}
	client_print( iAttacker, print_chat, "Damage: %f0.0, Victim: %d, Inflictor: %d (%s)", iDamage, iVictim, inflictor, s_Classname );
	client_print( iVictim, print_chat, "Damage: %f0.0, iAttacker: %d, Inflictor: %d (%s)", iDamage, iAttacker, inflictor, s_Classname );

	server_print( "Attacker: %d, Damage: %f0.0, Victim: %d, Inflictor: %d (%s)", iAttacker, iDamage, iVictim, inflictor, s_Classname );
*/

	return HAM_IGNORED;
}

public client_damage( iAttacker, iVictim, iDamage, iWeapon, iHitPlace, TA )
{

	if ( !WC3_Check() )
	{
		return;
	}
	
	// If they damage themself we don't care now do we ?
	if ( iVictim == iAttacker )
	{
		return;
	}
	
	//Death's Touch
	if (ITEM_Has(iAttacker, ITEM_DEATH_TOUCH ) > ITEM_NONE && arrDeathTouch[iAttacker])
	{
		new parm[5], vOrigin[3];
		get_user_origin(iVictim, vOrigin );

		parm[0] = iVictim;
		parm[1] = 6;
		parm[2] = vOrigin[0];
		parm[3] = vOrigin[1];
		parm[4] = vOrigin[2];

		emit_sound(iAttacker, CHAN_STATIC, g_szSounds[SOUND_DEATH_TOUCH], 1.0, ATTN_NORM, 0, PITCH_NORM );
		
		set_task(0.2, "fDrawDeathTouchExplode", TASK_DEATH_TOUCH + iVictim, parm, 5 );
				
		WC3_Kill( iVictim, iAttacker, iWeapon, 0 );	
		ITEM_RemoveID(iAttacker, ITEM_DEATH_TOUCH );

	}


	//Для talisman +
	// Frost Armor
	if (ITEM_Has(iVictim, ITEM_FROST_ARMOR ) > ITEM_NONE || fWorkTalismanPassive(iVictim, arrTalismanActive[iVictim]) == TALISMAN_ITEM_FROST_ARMOR)
	{
		WC3_Damage(iAttacker,iVictim,FROST_ARMOR_DAMAGE, iWeapon,0);
		SHARED_GlowShell(iVictim,0,139,139,0.2);
	}

	g_iDamageDealt[iAttacker][iVictim] += iDamage;


	// Bot should "auto" cast his/her ultimate on random
	if ( SHARED_ValidPlayer( iAttacker ) && is_user_bot( iAttacker ) && random_num( 0, 100 ) <= ( BOT_CAST_ULT_CHANCE * 100 ) )
	{
		// Check for an actual ultimate is done in this function, why do it twice?
		cmd_Ultimate( iAttacker );
	}
	

	// We need to make sure that we have a valid attacker and the user isn't hexed
	if ( SHARED_ValidPlayer( iAttacker ) && !p_data_b[iAttacker][PB_HEXED] )
	{
		//Для talisman +
		//Mirror Shield
		if (ITEM_Has(iVictim, ITEM_MIRROR_SHIELD ) > ITEM_NONE || fWorkTalismanPassive(iVictim, arrTalismanActive[iVictim]) == TALISMAN_ITEM_MIRROR_SHIELD)
		{
			iDamage = iDamage - floatround(iDamage * OFFENSIVE_MIRROR_SHIELD);
			SHARED_GlowShell(iVictim,255,105,180,0.2);
		}

		// Run the offensive spells
		//Наступательные скилы
		UD_SkillsOffensive( iAttacker,iVictim,iDamage );
		HU_SkillsOffensive( iAttacker, iVictim );
		OR_SkillsOffensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
		NE_SkillsOffensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
		BM_SkillsOffensive( iAttacker, iVictim, iDamage );
		SH_SkillsOffensive( iAttacker, iVictim );
		WA_SkillsOffensive( iAttacker, iVictim, iHitPlace );
		CL_SkillsOffensive( iAttacker, iVictim, iHitPlace );
		
		FM_SkillsOffensive( iAttacker, iVictim, iHitPlace );
		FM_SkillsOffensive_Ice_Arrow( iAttacker, iVictim);
		FM_SkillsOffensive_Cone_Cold( iAttacker, iVictim);

	}

	// Make sure we can run the defensive skills
	if ( SHARED_ValidPlayer( iAttacker ) && !p_data_b[iVictim][PB_HEXED] )
	{
		//Для talisman +
		//Mirror Shield
		if (ITEM_Has(iVictim, ITEM_MIRROR_SHIELD ) > ITEM_NONE || fWorkTalismanPassive(iVictim, arrTalismanActive[iVictim]) == TALISMAN_ITEM_MIRROR_SHIELD)
		{
			iDamage = iDamage - floatround(iDamage * DEFENSIVE_MIRROR_SHIELD);
			SHARED_GlowShell(iVictim,255,105,180,0.2);
		}

		//Защитные скилы
		//UD_SkillsDefensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
		//HU_SkillsDefensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
		//OR_SkillsDefensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
		NE_SkillsDefensive( iAttacker, iVictim, iDamage, iHitPlace );
		BM_SkillsDefensive( iAttacker, iVictim, iDamage );
		SH_SkillsDefensive( iAttacker, iVictim );
		//WA_SkillsDefensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
		CL_SkillsDefensive( iAttacker, iVictim, iDamage, iHitPlace );
	}

	// Item abilities
	if ( SHARED_ValidPlayer( iAttacker ) )
	{
		//Для talisman +
		//Mirror Shield
		if (ITEM_Has(iVictim, ITEM_MIRROR_SHIELD ) > ITEM_NONE || fWorkTalismanPassive(iVictim, arrTalismanActive[iVictim]) == TALISMAN_ITEM_MIRROR_SHIELD)
		{
			iDamage = iDamage - floatround(iDamage * ABILITIES_MIRROR_SHIELD);
			SHARED_GlowShell(iVictim,255,105,180,0.2);
		}

		ITEM_Offensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
	}

	return;
}

// All we want to check is if the user was killed by the bomb
public client_death( iAttacker, iVictim, iWeapon, hitplace, TK )
{
	// Counter-Strike and Condition Zero Checks
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Check out who the inflictor was
		new iInflictor = entity_get_edict( iVictim, EV_ENT_dmg_inflictor );

		// Check to see if the death was from the bomb
		if ( !SHARED_ValidPlayer( iInflictor ) && iWeapon != CSW_HEGRENADE && iInflictor )
		{
			
			if ( is_valid_ent ( iInflictor ) )
			{
				new szClassName[64];
				entity_get_string( iInflictor, EV_SZ_classname, szClassName, 63 );

				// Check the classname of our inflictor
				if ( equali( szClassName, "grenade" ) || equali( szClassName, "env_explosion" ) )
				{
					iWeapon = CSW_C4;
					iAttacker = 0;

					// Well if this isn't set, shouldn't it be?
					if ( !p_data_b[iVictim][PB_DIEDLASTROUND] )
					{
						on_Death( iVictim, iAttacker, iWeapon, 0 );
					}
					//client_print( iVictim, print_chat, "[DEBUG] You were just killed by the bomb (%s) Alive? %d", szClassName, is_user_alive( iVictim ) );
				}
			}
		}
	}

	return PLUGIN_HANDLED;
}

public on_Death( iVictim, iAttacker, iWeaponID, iHeadshot )
{

	if ( !WC3_Check() )
	{
		return;
	}
	
	// Counter-Strike and Condition Zero Checks
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// For some reason the damage passed by explosions is not actually correct (perhaps armor adjustments weren't done yet), so lets check
		if ( is_user_alive( iVictim ) && iWeaponID == CSW_C4 )
		{
			return;
		}

		// Check for NightVision
		p_data_b[iVictim][PB_NIGHTVISION]	= (cs_get_user_nvg( iVictim )) ? true : false;

		// Check if a user had a shield on death
		p_data_b[iVictim][PB_SHIELD]		= (cs_get_user_shield( iVictim )) ? true : false;

		// See if the user has a defuse kit
		p_data_b[iVictim][PB_DEFUSE]		= (cs_get_user_defuse( iVictim )) ? true : false;

		// Save the number of flash grenades the user has
		p_data[iVictim][P_FLASHCOUNT]		= cs_get_user_bpammo( iVictim, CSW_FLASHBANG );
		
		// Check to see if weapons were silenced on death
		new iWeaponUSPEnt = find_ent_by_owner( -1, "weapon_usp", iVictim );
		new iWeaponM4A1Ent = find_ent_by_owner( -1, "weapon_m4a1", iVictim );
		
		if ( is_valid_ent( iWeaponUSPEnt ) )
		{
			p_data_b[iVictim][PB_USP_SILENCED]	= cs_get_weapon_silen( iWeaponUSPEnt ) ? true : false;
		}

		if ( is_valid_ent( iWeaponM4A1Ent ) )
		{
			p_data_b[iVictim][PB_M4A1_SILENCED]	= cs_get_weapon_silen( iWeaponM4A1Ent ) ? true : false;
		}


		// Check to see if weapons were in burst mode on death
		new iWeaponGlock18Ent = find_ent_by_owner( -1, "weapon_glock18", iVictim );
		new iWeaponFamasEnt = find_ent_by_owner( -1, "weapon_famas", iVictim );

		if ( is_valid_ent( iWeaponGlock18Ent ) )
		{
			p_data_b[iVictim][PB_GLOCK_BURST]	= cs_get_weapon_burst( iWeaponGlock18Ent ) ? true : false;
		}

		if ( is_valid_ent( iWeaponFamasEnt ) )
		{
			p_data_b[iVictim][PB_FAMAS_BURST]	= cs_get_weapon_burst( iWeaponFamasEnt ) ? true : false;
		}
	}
	
	WC3_Death( iVictim, iAttacker, iWeaponID, iHeadshot );
	
	return;
}

// Hook for CS/CZ
public on_DeathMsg()
{

	if ( !WC3_Check() )
	{
		return;
	}

	new iAttacker	= read_data( 1 );
	new iVictim		= read_data( 2 );
	new iHeadshot	= read_data( 3 );
	new iWeaponID;
	get_user_attacker( iVictim, iWeaponID );

	//Отключение ESP
	if (ITEM_Has(iVictim, ITEM_ESP ) > ITEM_NONE )
		fDisableItemESP(iVictim);


	on_Death( iVictim, iAttacker, iWeaponID, iHeadshot );

	
	if(get_pcvar_num(CVAR_wc3_respawn_onoff) == 1)
	{
		//if(p_data[iVictim][P_RACE] == RACE_ORC)
			SHARED_CopySavedWeapons( iVictim );
	}
	
	//Вывод в чат информации о киллере
	if(get_pcvar_num(CVAR_wc3_info_killer_onoff) == 1)
		fInfoKiller(iAttacker,iVictim);

	return;
}

new g_iLastCurWeapon[33];
public on_CurWeapon( idUser )
{
	// read_data(1) = isActive?
	// read_data(2) = weapon index
	// read_data(3) = ammo

	if ( !WC3_Check() )
	{
		return;
	}

	new iCurWeapon = read_data( 2 );

	// Then a weapon strip was triggered - we don't want to save now do we?
	if ( iCurWeapon == 0 && read_data( 1 ) == 0 && read_data( 3 ) == 0 )
	{
		return;
	}

	// Record the last time a shot was fired
	fLastShotFired[idUser] = halflife_time();
	
	// Save the user's weapons
	SHARED_SaveWeapons( idUser );
		
	// We only need to run these functions if the user's weapon has changed since our last function call!
	if ( g_iLastCurWeapon[idUser] != iCurWeapon && SHARED_ValidPlayer( idUser ))
	{
		// Check to see if we should set the player's invisibility
		SHARED_INVIS_Set( idUser );
		
		// Set the user's speed
		SHARED_SetSpeed( idUser );
	}

	g_iLastCurWeapon[idUser] = iCurWeapon;

	return;
}

public on_Drop( idUser )
{
	if ( !WC3_Check() )
	{
		return;
	}

	SHARED_SaveWeapons( idUser );

	return;
}

// HamSandwich implementation
public EVENT_Spawn( idUser )
{
	if ( !WC3_Check() )
	{
		return HAM_HANDLED;
	}


	// ResetHUD can be called when the user is not alive, lets ignore those calls
	if ( !is_user_alive( idUser ) )
		return HAM_HANDLED;

	// Store the player's team!
	g_iPlayerTeam[idUser] = get_user_team( idUser );

	// Prespawn call
	//   - Purpose is to have a spawn call that happens before everything else!
	WC3_PreSpawn( idUser );
	
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		
		// This is the first time the user has spawned this round
		if ( !p_data_b[idUser][PB_HAS_SPAWNED] )
		{	
			EVENT_PlayerInitialSpawn( idUser );

			p_data_b[idUser][PB_HAS_SPAWNED] = true;
		}
	}

	// Should be called at the end of each spawn
	WC3_PostSpawn( idUser );

	//Для shopmenu4
	sm4_4_2_hamPlayerSpawn(idUser);

	return HAM_HANDLED;
}

// Function called EVERYTIME a user spawns!
public on_ResetHud( idUser )
{

	if ( !WC3_Check() )
	{
		return PLUGIN_CONTINUE;
	}

	// ResetHUD can be called when the user is not alive, lets ignore those calls
	if ( !is_user_alive( idUser ) )
	{
		return PLUGIN_CONTINUE;
	}
		

	// Store the player's team!
	g_iPlayerTeam[idUser] = get_user_team( idUser );

	// We're forcibly respawning the player - so lets just return
	if ( bIgnorePlayerSpawning[idUser] )
	{
		return PLUGIN_CONTINUE;
	}

	// Prespawn call
	//   - Purpose is to have a spawn call that happens before everything else!
	WC3_PreSpawn( idUser );

	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		
		// This is the first time the user has spawned this round
		if ( !p_data_b[idUser][PB_HAS_SPAWNED] )
		{	
			EVENT_PlayerInitialSpawn( idUser );

			p_data_b[idUser][PB_HAS_SPAWNED] = true;
		}
	}

	// Should be called at the end of each spawn
	WC3_PostSpawn( idUser );

	p_data_b[idUser][PB_ISBURNING] = false;
	p_data[idUser][P_FLAMECOUNT] = 0;

	return PLUGIN_CONTINUE;
}

// Function is called when the user is spawned at the START of each round - only WC3_PreSpawn is called first
//		TRIGGERED BY: ResetHUD
public EVENT_PlayerInitialSpawn( idUser )
{
	// Display the cheat message ONLY if they just joined!
	if ( p_data_b[idUser][PB_JUSTJOINED] && get_pcvar_num( CVAR_wc3_cheats ) )
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03/level10",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_EVENT_PLAYER_INITIAL_SPAWN");	
			
		cssbColoredPrint(idUser, szMessage);
		
		p_data_b[idUser][PB_JUSTJOINED]	= false;
	}

	// Display the new Chameleon skills for the round
	if ( p_data[idUser][P_RACE] == 9 && get_pcvar_num( CVAR_wc3_cham_random ) )
	{
		WC3_ShowRaceInfo( idUser );
	}

	//if ( CVAR_csdm_active == 0)
		WC3_NewSession( idUser );

	return;
}

// Function is called ONCE at the start of a new round BEFORE user's spawn
public EVENT_NewRound()
{
	if ( !WC3_Check() )
	{
		return;
	}

	//Для shopmenu4
	sm4_fRemoveAllArrayBookDead();

	//Удаление всех брошеных вещей
	fRemoveAllItems();

	// Reset the global ultimate delay
	g_iUltimateDelay = get_pcvar_num( CVAR_wc3_ult_delay );

	// User's have not spawned yet, so lets do some pre-spawn things
	new players[32], numplayers, i;
	get_players( players, numplayers );
	for ( i = 0; i < numplayers; i++ )
	{
		ITEM_LimitOneRound(players[i]);

		WC3_BeforeSpawn( players[i] );

		//Для shopmenu4
		//Остановка работы предмета Спокойствие
		sm4_4_9_fRemoveSpellCasting( players[i] );
		remove_task( TASK_TRANQUILITY + players[i] );

		//Разморозка игрока
		sm4_4_2_UnfrozenPlayer(players[i]);

	}
	
	// Reset which spawn points are reserved....
	_SHARED_SpawnReset();

	// If someone restarted the game, then lets reset war3
	if ( g_GameRestarting )
	{
		WC3_ResetGame();
	}
	
	// Randomize Chameleon if we need to
	CHAM_Randomize();

	// We need to determine when the buytime is up
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( get_pcvar_num( CVAR_wc3_buy_time ) )
		{
			// Remove our buytime task if it exists (from a previous round)
			( task_exists( TASK_BUYTIME ) ) ? remove_task( TASK_BUYTIME ) : 0;

			g_buyTime = true;

			set_task( get_cvar_float("mp_buytime") * 60.0, "_CS_BuyTimeOver", TASK_BUYTIME );
		}

		g_freezeTime	= true;
	}

	g_EndRound = false;

	sm4_4_2_evNewRound();

}

// Called when a user looks somewhere
public TRIGGER_TraceLine( Float:v1[3], Float:v2[3], noMonsters, pentToSkip )
{
	if ( !WC3_Check() )
	{
		return FMRES_IGNORED;
	}

	new iAttacker = pentToSkip;
	new iVictim = get_tr(TR_pHit);
	new iHitZone = (1 << get_tr(TR_iHitgroup));
	
	// Make sure we have a valid victim
	if ( SHARED_ValidPlayer( iVictim ) && p_data_b[iVictim][PB_ISCONNECTED] )
	{
		// We need to have a valid player!
		if ( SHARED_ValidPlayer( iAttacker ) )
		{
			// This is a check for ultimates that need to "search" for a target
			if ( p_data_b[iAttacker][PB_ISSEARCHING] )
			{

				// Now lets make sure the person he's looking at is in view and isn't on the same team
				if ( get_user_team( iAttacker ) != get_user_team( iVictim ) ) //&& UTIL_EntInView( iAttacker, iVictim ) )
				{
					
					// Check to see if the user should block this ultimate!
					if ( !g_EndRound && ULT_CanUserBlockUlt( iVictim ) )
					{
						ULT_RemoveCharge( iVictim );
						ULT_Blocked( iAttacker );
					}

					// Then the user's ult should work!
					else
					{

						//Для talisman +
						//Для shopmenu4
						//ITEM_MIRROR_ULTIMATE - Отражатель супер навыка - Mirror Ultimate
						if ( (ITEM_Has(iVictim, ITEM_MIRROR_ULTIMATE ) > ITEM_NONE && ITEM_HasItem( iVictim, ITEM_NECKLACE ) != ITEM_NECKLACE) ||
							 (fWorkTalismanPassive(iVictim, arrTalismanActive[iVictim]) == TALISMAN_ITEM_MIRROR_ULTIMATE && ITEM_HasItem( iVictim, ITEM_NECKLACE ) != ITEM_NECKLACE)
						   )
						{

							if(ITEM_Has(iAttacker, ITEM_NECKLACE ) > ITEM_NONE)
							{
								formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_MIRROR_ULT_BLOCKED");
								cssbChatColor(iAttacker,"%s%s",fTagWar3ft(),szMessage);

								emit_sound(iVictim, CHAN_STATIC, g_szSounds[SOUND_MIRROR_ULTIMATE_BLOCK], 1.0, ATTN_NORM, 0, PITCH_NORM );
								emit_sound(iAttacker, CHAN_STATIC, g_szSounds[SOUND_MIRROR_ULTIMATE_BLOCK], 1.0, ATTN_NORM, 0, PITCH_NORM );

								ITEM_RemoveCharge(iAttacker, ITEM_NECKLACE );
								ITEM_RemoveCharge(iVictim, ITEM_MIRROR_ULTIMATE );

								// No longer searching since we found a target
								p_data_b[iAttacker][PB_ISSEARCHING]	= false;

								// Set up the user's ultimate delay
								fResetUltimateTimerDivider(iAttacker);

								return FMRES_SUPERCEDE;
							}

							formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_MIRROR_ULT");
							cssbChatColor(iAttacker,"%s%s",fTagWar3ft(),szMessage);

							sm4_EffectMirrorUltimate(iVictim);

							// Well we do have a target so lets execute the user's ultimate!!
							if ( SM_GetSkillLevel( iAttacker, ULTIMATE_CHAINLIGHTNING ) > 0 )
							{
								OR_ULT_ChainLightning(iVictim,iAttacker, iHitZone );
							}
							else if ( SM_GetSkillLevel(iAttacker, ULTIMATE_ENTANGLE ) > 0 )
							{
								NE_ULT_Entangle(iVictim,iAttacker);
							}
							else if ( SM_GetSkillLevel( iAttacker, ULTIMATE_IMMOLATE ) > 0 )
							{
								BM_ULT_Immolate(iVictim,iAttacker );
							}
							else if ( SM_GetSkillLevel( iAttacker, ULTIMATE_FROST_MAGE_BLIZZARD ) > 0 )
							{
								FM_ULT_Blizzard(iVictim,iAttacker, iHitZone );
								
							}

							ITEM_RemoveCharge(iVictim, ITEM_MIRROR_ULTIMATE );
						}
						else
						{
							// Well we do have a target so lets execute the user's ultimate!!
							if ( SM_GetSkillLevel( iAttacker, ULTIMATE_CHAINLIGHTNING ) > 0 )
							{
								OR_ULT_ChainLightning( iAttacker, iVictim, iHitZone );
							}
							else if ( SM_GetSkillLevel( iAttacker, ULTIMATE_ENTANGLE ) > 0 )
							{
								NE_ULT_Entangle( iAttacker, iVictim );
							}
							else if ( SM_GetSkillLevel( iAttacker, ULTIMATE_IMMOLATE ) > 0 )
							{
								BM_ULT_Immolate( iAttacker, iVictim );
							}
							else if ( SM_GetSkillLevel( iAttacker, ULTIMATE_FROST_MAGE_BLIZZARD ) > 0 )
							{
								FM_ULT_Blizzard( iAttacker, iVictim, iHitZone );
								
							}
						}
					}
					
					// No longer searching since we found a target
					p_data_b[iAttacker][PB_ISSEARCHING]	= false;

					// Set up the user's ultimate delay
					fResetUltimateTimerDivider(iAttacker);
				}
			
			}

			//Для shopmenu4
			//ITEM_DEATHCOIL - Death Coil - Лик смерти
			if ( ITEM_Has( iAttacker, ITEM_DEATHCOIL ) > ITEM_NONE )
			{
				if (random_num(0, 99 ) <= get_pcvar_num( CVAR_wc3_deathcoil_percent ) ) 
					sm4_4_5_DeathCoil( iAttacker, iVictim);
			}

			//ITEM_IMPALE - Impale - Пронзающий шип
			if ( ITEM_Has( iAttacker, ITEM_IMPALE ) > ITEM_NONE )
			{
				if (random_num(0, 99 ) <= get_pcvar_num( CVAR_wc3_impale_percent ) ) 
					sm4_4_6_Impale( iAttacker, iVictim);
			}

			//ITEM_SLEEP - Sleep - Сонная аура
			if ( ITEM_Has( iAttacker, ITEM_SLEEP ) > ITEM_NONE )
			{
				if (random_num(0, 99 ) <= get_pcvar_num( CVAR_wc3_sleep_percent ) ) 
					sm4_4_7_Sleep( iAttacker, iVictim);
			}

			//Для talisman
			// This is a nice check for Helm of Excellence
			if ( ITEM_Has( iVictim, ITEM_HELM ) > ITEM_NONE || 
				(fWorkTalismanPassive(iVictim, arrTalismanActive[iVictim] == TALISMAN_ITEM_HELM) && arrTalismanHelmNum[iVictim] > 0)
				)
			{
				// Friendly fire is off! - This means same team shouldn't remove a charge!
				if ( !get_pcvar_num( CVAR_mp_friendlyfire ) )
				{
					if ( g_iPlayerTeam[iAttacker] == g_iPlayerTeam[iVictim] )
					{
						return FMRES_IGNORED;
					}
				}

				// If its a headshot then we want to block it
				if ( iHitZone & (1 << 1) )
				{
					set_tr( TR_flFraction, 1.0 );
					
					// Make sure we have a valid attacker!
					if ( SHARED_ValidPlayer( iAttacker ) )
					{
						// Do the check to see if we should flash the screen orange
						new Float:fTime = halflife_time();
						new Float:fDifference = fTime - fLastShotFired[iAttacker];

						if ( fDifference < 0.1 && fDifference > 0.0 )
						{
							Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 0, 0, 255, 150 );

							// Lets remove a charge from the helm!
							ITEM_RemoveCharge( iVictim, ITEM_HELM );
						}
					}
					
					return FMRES_SUPERCEDE;
				}
			}

			// Check to see if this user has night elf's evasion
			if ( SM_GetSkillLevel( iVictim, SKILL_EVASION ) > 0 )
			{
				// Do the check to see if we should "evade" this shot
				new Float:fTime = halflife_time();
				new Float:fDifference = fTime - fLastShotFired[iAttacker];

				if ( SHARED_ValidPlayer( iAttacker ) && fDifference < 0.1 && fDifference > 0.0 )
				{

					// Friendly fire is off! - This means we shouldn't evade since no damage will be done!
					if ( !get_pcvar_num( CVAR_mp_friendlyfire ) )
					{
						if ( g_iPlayerTeam[iAttacker] == g_iPlayerTeam[iVictim] )
						{
							return FMRES_IGNORED;
						}
					}
					
					// Then we should evade this shot!
					if ( NE_Evasion( iVictim, iHitZone ) )
					{
						set_tr( TR_flFraction, 1.0 );
						
						format(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_EVENT_EVADED_SHOT");
						WC3_StatusText( iVictim, TXT_SKILL, szMessage);

						return FMRES_SUPERCEDE;
					}
				}
			}
			
			//Для talisman +
			// Mole protectant
			if ( p_data_b[iAttacker][PB_MOLE] && ITEM_Has( iVictim, ITEM_PROTECTANT ) > ITEM_NONE ||
				 p_data_b[iAttacker][PB_MOLE] && fWorkTalismanPassive(iVictim, arrTalismanActive[iVictim]) == TALISMAN_ITEM_PROTECTANT)
			{	
				new Float:fTime = halflife_time();

				if ( fTime - fLastShotFired[iAttacker] < 0.1  )
				{
					format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
						LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
						LANG_PLAYER,"SHOT_DEFLECTED");	
			
					cssbColoredPrint(iVictim, szMessage);
		
					set_tr( TR_flFraction, 1.0 );

					return FMRES_SUPERCEDE;
				}
			}

			// Check for Big Bad Voodoo's ultimate!
			if ( p_data_b[iVictim][PB_GODMODE] )
			{
				
				new bool:bBlockDamage = true;

				// Do we allow this person to be attacked by this player?
				if ( p_data_b[iAttacker][PB_BIGBAD_ATTACKER] )
				{
					bBlockDamage = false;
				}

				// Check to see if immunity is available for the attacker
				else if ( ULT_CanUserBlockUlt( iAttacker ) )
				{
					// Remove charge and display message to attacker
					ULT_RemoveCharge( iAttacker );

					// Display message about user's ultimate being blocked!
					ULT_Blocked( iVictim );

					// This user can attack someone with big bad voodoo!
					p_data_b[iAttacker][PB_BIGBAD_ATTACKER] = true;

					// Reset the attacker dmg
					set_task( get_pcvar_float( CVAR_wc3_ult_cooldown ), "_SH_ResetBigBadAttacker", TASK_BIGBADATTACKER + iAttacker );
				}

				// Block the damage!
				if ( bBlockDamage )
				{
					set_tr( TR_flFraction, 1.0 );

					return FMRES_SUPERCEDE;
				}
			}
		}
	}
	
	return FMRES_IGNORED;
}
