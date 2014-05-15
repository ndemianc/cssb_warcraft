/*
*	All WC3 Specific functions
*/


#define BOT_RESPAWN			0.10			// Chance that a bot will buy a respawn scroll when dying

/***********************************************************************
		Precaching FUNCTIONS (idea from war3x)
***********************************************************************/

public WC3_Precache()
{
	
	// Precache wc3.css (if it exists!)
	if ( !file_exists( "wc3.css" ) )
	{
		WC3_Log( true, "[ERROR] Missing file 'wc3.css'" );

		set_fail_state( "A required file is missing, unable to load plugin" );
	}
	else
	{
		precache_generic( "wc3.css" );
	}

	copy( g_szSounds[SOUND_VENGEANCE]		, 63, "warcraft3/MiniSpiritPissed1.wav"				);	// Warden's Ultimate
	copy( g_szSounds[SOUND_SUICIDE]			, 63, "ambience/particle_suck1.wav"					);	// Undead's Ultimate
	copy( g_szSounds[SOUND_BANISH]			, 63, "warcraft3/banishcaster.wav"					);	// Banish
	copy( g_szSounds[SOUND_LOCUSTSWARM]		, 63, "warcraft3/locustswarmloop.wav"				);	// Locust Swarm
	copy( g_szSounds[SOUND_SHADOWSTRIKE]	, 63, "warcraft3/shadowstrikebirth.wav"				);	// Shadow Strike
	copy( g_szSounds[SOUND_CARRION]			, 63, "warcraft3/carrionswarmlaunch1.wav"			);	// Carrion Beetles
	copy( g_szSounds[SOUND_IMPALE]			, 63, "warcraft3/impalehit.wav"						);	// Impale
	copy( g_szSounds[SOUND_VOODOO]			, 63, "warcraft3/divineshield.wav"					);	// Big Bad Voodoo
	copy( g_szSounds[SOUND_FLAMESTRIKE]		, 63, "warcraft3/flamestriketargetwavenonloop1.wav"	);	// Flame Strike
	copy( g_szSounds[SOUND_ENTANGLING]		, 63, "warcraft3/entanglingrootstarget1.wav"		);	// Entangling Roots
	copy( g_szSounds[SOUND_LIGHTNING]		, 63, "warcraft3/lightningbolt.wav"					);	// Chain Lightning
	copy( g_szSounds[SOUND_TELEPORT]		, 63, "warcraft3/massteleporttarget.wav"			);	// Teleport
	copy( g_szSounds[SOUND_BLINK]			, 63, "warcraft3/blinkarrival.wav"					);	// Blink Teleport
	copy( g_szSounds[SOUND_LEVELUP]			, 63, "warcraft3/levelupcaster.wav"					);	// Level up
	copy( g_szSounds[SOUND_PICKUPITEM]		, 63, "warcraft3/pickupitem.wav"					);	// Buy / Pick up item
	copy( g_szSounds[SOUND_TOME]			, 63, "warcraft3/Tomes.wav"							);	// Tome of Experience
	copy( g_szSounds[SOUND_ULTIMATESCAN]	, 63, "turret/tu_ping.wav"							);	// Ultimate Beep
	copy( g_szSounds[SOUND_ULTIMATEREADY]	, 63, "warcraft3/resurrecttarget.wav"				);	// Ultimate Beep
	copy( g_szSounds[SOUND_ANNIHILATION]	, 63, "warcraft3/PurgeTarget1.wav"					);	// Orb of Annihilation
	copy( g_szSounds[SOUND_CONCOCTION_CAST]	, 63, "warcraft3/PossessionMissileLaunch1.wav"		);	// Unstable Concoction Cast
	copy( g_szSounds[SOUND_CONCOCTION_HIT]	, 63, "warcraft3/PossessionMissileHit1.wav"			);	// Unstable Concoction Hit
	copy( g_szSounds[SOUND_HEX]				, 63, "warcraft3/PolymorphDone.wav"					);	// Hex
	copy( g_szSounds[SOUND_IMMOLATE]		, 63, "warcraft3/ImmolationDecay1.wav"				);	// Immolate
	copy( g_szSounds[SOUND_IMMOLATE_BURNING], 63, "ambience/flameburst1.wav"				    );	// Immolate Burning

	copy( g_szSounds[SOUND_REINCARNATION]	, 63, "warcraft3/soundpack/reincarnation.wav"		);	// Weapon Reincarnation/Ankh
	copy( g_szSounds[SOUND_ANTEND]			, 63, "warcraft3/antend.wav"						);	// Sound played when you try to shoot a chameleoned teammate 
	copy( g_szSounds[SOUND_ERROR]			, 63, "warcraft3/bonus/Error.wav"					);	// Sound played on errors 

	copy( g_szSounds[SOUND_DISPELL]			, 63, "warcraft3/DispelMagicTarget.wav"				);	// Dispell an ultimate - played on iAttacker
	copy( g_szSounds[SOUND_SPELLSHIELD]		, 63, "warcraft3/SpellShieldImpact1.wav"			);	// Dispell an ultimate - played on blocker

	copy( g_szSounds[SOUND_DEATH_TOUCH]		, 63, "warcraft3/item_death_touch.wav"			);	// Item Death Touch
	copy( g_szSounds[SOUND_HEALTH_POTION]	, 63, "warcraft3/item_health_potion.wav"		);	// Item Health Potion
	copy( g_szSounds[SOUND_STEEL_SKIN]		, 63, "warcraft3/item_steel_skin.wav"			);	// Item  Steel Skin

	copy( g_szSounds[SOUND_BLIZZARD]		, 63, "warcraft3/skill_blizzard.wav"			);	// Frost Mage Ultimate Blizzard
	copy( g_szSounds[SOUND_ICE_SPLINTERS]	, 63, "warcraft3/skill_ice_splinters.wav"	);	// Frost Mage Skill Ice splinters

	copy( g_szSounds[SOUND_ICE_ARROW_MISSILE]	, 63, "warcraft3/ice_arrow_missile.wav"	);	// Frost Mage Skill Ice Arrow
	copy( g_szSounds[SOUND_ICE_EXPLODE]	, 63, "warcraft3/ice_explode.wav"	);	// Frost Mage Skill Ice Arrow Explode

	copy( g_szSounds[SOUND_COLD_CONE_S]	, 63, "warcraft3/cold_cone_s.wav"	);	// Frost Mage Cold Cone
	copy( g_szSounds[SOUND_COLD_CONE_E]	, 63, "warcraft3/cold_cone_e.wav"	);	// Frost Mage Cold Cone (remove)
	
	//Для shopmenu4
	copy( g_szSounds[SOUND_ICE_BLOCK_EXPLODE]		, 63, "x/x_shoot1.wav"	);							//Взрыв Ледяной глыбы
	copy( g_szSounds[SOUND_ICE_BLOCK_FROZEN]		, 63, "debris/glass1.wav"	);						//Заморозка Ледяной глыбы
	copy( g_szSounds[SOUND_ICE_BLOCK_UNFROZEN]		, 63, "debris/glass3.wav"	);						//Разморозка Ледяной глыбы
	copy( g_szSounds[SOUND_ICE_BLOCK_CHILLED]		, 63, "player/pl_duct2.wav"	);						//Заморожен Ледяной глыбой
	copy( g_szSounds[SOUND_MIRROR_ULTIMATE_BLOCK]	, 63, "warcraft3/item_mirror_ult.wav"	);			//Блокировка отражателя супер навыка
	copy( g_szSounds[SOUND_DEATHCOIL]				, 63, "warcraft3/DeathCoilSpecialArt1.wav"	);		//Лик смерти
	copy( g_szSounds[SOUND_DEATHCOIL_MISSILE]		, 63, "warcraft3/DeathCoilMissileLaunch1.wav"	);	//Срабатывание Лик смерти
	copy( g_szSounds[SOUND_SLEEP]					, 63, "warcraft3/SleepBirth1.wav"	);				//Сонная аура
	copy( g_szSounds[SOUND_BOOK_DEAD_RAISE]			, 63, "warcraft3/book_dead_raise.wav"	);			//Вызов мертвецов хозяином
	copy( g_szSounds[SOUND_BOOK_DEAD_WARLOCK]		, 63, "warcraft3/book_dead_warlock.wav"	);			//Звук для ожившего мертвеца
	copy( g_szSounds[SOUND_TRANQUILITY]				, 63, "warcraft3/Tranquility.wav"	);				// Tranquility - Спокойствие
	copy( g_szSounds[SOUND_PRECAST_HOLY]			, 63, "warcraft3/PreCastHolyMagicLow.wav"	);	
	copy( g_szSounds[SOUND_CAST_HOLY]				, 63, "warcraft3/HolyCast.wav"	);			

	//Звук когда при покупке не хватает денег
	copy(g_szSounds[SOUND_NO_GOLD]				, 63, "warcraft3/no_gold.wav"	);

	// Lets make sure we have all of the sound files!!!
	new i, szTmp[128], bool:bError = false;

	for ( i = 0; i < MAX_SOUNDS; i++ )
	{

		// We only need to look for warcraft3 files (other files should be w/base HLDS installation)
		if ( containi( g_szSounds[i], "warcraft3" ) != -1 )
		{
			formatex( szTmp, 127, "sound/%s", g_szSounds[i] );

			if ( !file_exists( szTmp ) )
			{
				WC3_Log( true, "[ERROR] Missing sound file '%s'", szTmp );

				bError = true;
			}
		}
	}

	// All sound files exist!! Lets continue!
	if ( !bError )
	{
		
		// Precache all files
		for ( i = 0; i < MAX_SOUNDS; i++ )
		{
			precache_sound( g_szSounds[i] );
		}
	}

	// We need to throw a runtime error :/
	else
	{
		set_fail_state( "Sound files are missing, unable to load plugin" );
	}

	bError = false;

	// Copy the sprite files to the array
	copy( g_szSprites[SPR_BANISH]		, 63, "sprites/nhth1.spr" );
	copy( g_szSprites[SPR_SHADOWSTRIKE]	, 63, "sprites/animglow01.spr" );
	copy( g_szSprites[SPR_FIRE]			, 63, "sprites/explode1.spr" );
	copy( g_szSprites[SPR_BURNING]		, 63, "sprites/xfire.spr" );
	copy( g_szSprites[SPR_BEETLE]		, 63, "sprites/agrunt1.spr" );
	copy( g_szSprites[SPR_FIREBALL]		, 63, "sprites/zerogxplode.spr" );
	copy( g_szSprites[SPR_BEAM]			, 63, "sprites/zbeam4.spr" );
	copy( g_szSprites[SPR_TRAIL]		, 63, "sprites/smoke.spr" );
	copy( g_szSprites[SPR_SHOCKWAVE]	, 63, "sprites/shockwave.spr" );
	copy( g_szSprites[SPR_LIGHTNING]	, 63, "sprites/lgtning.spr" );
	copy( g_szSprites[SPR_FLARE]		, 63, "sprites/blueflare2.spr" );
	copy( g_szSprites[SPR_WAVE]			, 63, "sprites/gwave1.spr" );
	copy( g_szSprites[SPR_IMMOLATE]		, 63, "sprites/warcraft3/fireball.spr" );

	//Death's Touch
	copy( g_szSprites[SPR_DEATH_TOUCH]	,63, "sprites/warcraft3/eff_death_touch.spr" );

	// Health Potion
	copy( g_szSprites[SPR_HEALTH_POTION],63, "sprites/warcraft3/eff_health_potion.spr" );

	// Steel Skin
	copy( g_szSprites[SPR_STEEL_SKIN],63, "sprites/warcraft3/eff_steel_skin.spr" );

	// Frost Mage Ultimate Blizzard
	copy( g_szSprites[SPR_BLUE_LIGHTNING],63, "sprites/warcraft3/blue_lightning_blizzard.spr" );
	copy( g_szSprites[SPR_ICE_FROSTBOLT],63, "sprites/warcraft3/ice_frostbolt.spr" );

	// Frost Mage Ice splinters
	copy( g_szSprites[SPR_ICE_SPLINTERS],63, "sprites/warcraft3/ice_splinters.spr" );

	// Frost Mage Ледяная стрела (Ice Arrow)
	copy( g_szSprites[SPR_ICE_ARROW_TRAIL],63, "sprites/warcraft3/ice_arrow_trail.spr" );
	copy( g_szSprites[SPR_ICE_ARROW],63, "sprites/warcraft3/ice_arrow.spr" );
	copy( g_szSprites[SPR_ICE_EXPLODE],63, "sprites/warcraft3/ice_explode.spr" );

	copy( g_szSprites[SPR_SMOKE]		, 63, "sprites/steam1.spr");
	copy( g_szSprites[SPR_SNOW]			, 63, "sprites/snow.spr");

	//Для shopmenu4
	copy( g_szSprites[SPR_ICE_BLOCK_TRAIL]	, 63, "sprites/laserbeam.spr");
	copy( g_szSprites[SPR_MIRROR_ULTIMATE]	, 63, "sprites/xflare3.spr");
	copy( g_szSprites[SPR_DEATHCOIL]		, 63, "sprites/flare6.spr");			//Лик смерти
	copy( g_szSprites[SPR_BLOODSPRAY]		, 63, "sprites/bloodspray.spr");		//New Impale
	copy( g_szSprites[SPR_BLOODDROP]		, 63, "sprites/blood.spr");				//New Impale2
	copy( g_szSprites[SPR_SLEEP]			, 63, "sprites/warcraft3/sleep.spr");	//Сонная аура

	// Precache models
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( g_MOD == GAME_CZERO )
		{
			precache_model( "models/player/spetsnaz/spetsnaz.mdl"	);
			precache_model( "models/player/militia/militia.mdl"		);
		}

		precache_model( "models/player/gign/gign.mdl"			);
		precache_model( "models/player/sas/sas.mdl"				);
		precache_model( "models/player/gsg9/gsg9.mdl"			);
		precache_model( "models/player/urban/urban.mdl"			);
		precache_model( "models/player/arctic/arctic.mdl"		);
		precache_model( "models/player/leet/leet.mdl"			);
		precache_model( "models/player/guerilla/guerilla.mdl"	);
		precache_model( "models/player/terror/terror.mdl"		);
		mdlGlassSpr = precache_model( "models/glassgibs.mdl"	);

		//Для shopmenu4
		precache_model( "models/bskull_template1.mdl"			);	// Death Coil
		precache_model( "models/tentacle2.mdl"					);	// Impale
		//Модели скелетов
		precache_model( "models/player/skull_t/skull_t.mdl") 
		precache_model( "models/player/skull_ct/skull_ct.mdl")

		//Для talisman +
		precache_model(szModelTalisman);

	}
	
	// Now lets make sure the sprites exist
	for ( i = 0; i < MAX_SPRITES; i++ )
	{

		// We only need to look for warcraft3 files (other files should be w/base HLDS installation)
		if ( containi( g_szSprites[i], "warcraft3" ) != -1 )
		{
			if ( !file_exists( g_szSprites[i] ) )
			{
				WC3_Log( true, "[ERROR] Missing sprite file '%s'", g_szSprites[i] );

				bError = true;
			}
		}
	}

	// Check the level sprites
	// All sprite files exist!! Lets continue!
	if ( !bError )
	{
		
		// Precache all common sprites
		for ( i = 0; i < MAX_SPRITES; i++ )
		{
			if ( g_szSprites[i][0] )
			{
				g_iSprites[i] = precache_model( g_szSprites[i] );
			}
		}
	}

	// We need to throw a runtime error
	else
	{
		set_fail_state( "Sprite files are missing, unable to load plugin" );
	}

}

WC3_Check( idUser = 0, print_location = print_chat )
{
	if ( warcraft3 )
	{
		return true;
	}

	if ( idUser != 0 && print_location )
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_WC3_CHECK");	
			
		cssbColoredPrint(idUser, szMessage);
	}

	return false;
}

// Function called when the plugin loads
public WC3_Init()
{
	// Store this now so we don't need to make a native call everytime we use it
	MAXPLAYERS = get_global_int( GL_maxClients );

	// Register the CVARs
	CVAR_Init();
	
	// Initialize our items
	ITEM_Init();
	
	// Initialize our skills
	SM_Init();

	// Register the player menus
	LANG_SetMenus();

	//Для авто закупки предметов
	abi_InitLangAllMenus();

	// Register any forwards
	FWD_Register();


	//Для talisman + BEGIN {
	//Инициализация переменных
	fInitTalisman();

	//Считывание файла респаунов талисмана для конкретной карты
	fReadMapSpawnsTalisman();

	//Отсчет времени для талисмана
	set_task(TIME_TASK_SPAWN_TALISMAN, "taskCounterLoopTalisman", TASK_SPAWN_TALISMAN, "", 0, "b" );
	
	//Для talisman END }

	// We need to execute config file in a bit don't we?
	set_task( 0.3, "_CVAR_ExecuteConfig", TASK_EXECUTECONFIG );

	// Configure the database connection
	set_task( 1.0, "DB_Init", TASK_SETSQL );

	// Set up things when our config file has loaded the values
	set_task( 1.5, "_WC3_RunAfterConfig", TASK_AFTERCONFIG );

	// cl_minmodels check
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		set_task( 4.5, "_CS_MinModelsLoop", TASK_MOLEFIX, "", 0, "b" );
	}

	// Set up the spawn entities
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		copy( szSpawnEnt[0], 31, "info_player_start" );
		copy( szSpawnEnt[1], 31, "info_player_deathmatch" );
	}
	
	// Run the ultimate delay task
	set_task( 1.0, "_ULT_Delay", TASK_UDELAY, "", 0, "b" );
	
	// We need to determine the objective entities (if they exist)
	new i, bool:bFound;
	for ( i = 0; i <= get_global_int( GL_maxEntities ); i++ )
	{
		if ( !is_valid_ent( i ) )
		{
			continue;
		}
		
		// More than were necessary was found
		if ( g_iTotalObjectiveEnts >= MAX_OBJECTIVES )
		{
			WC3_Log( true, "Woa we found more than 11" );
			break;
		}

		bFound = false;

		new szClassName[64];
		entity_get_string( i, EV_SZ_classname, szClassName, 63 );
		
		// VIP Escape zone!!
		if ( equal( szClassName, "func_vip_safetyzone") )	
		{
			g_iObjectiveEntType[g_iTotalObjectiveEnts] = OBJENT_VIP_ESCAPE;
			bFound = true;
		}

		// Hostage zone
		else if ( equal( szClassName, "hostage_entity") )
		{
			g_iObjectiveEntType[g_iTotalObjectiveEnts] = OBJENT_HOSTAGE;
			bFound = true;
		}

		// Bomb zone
		else if ( equal( szClassName, "func_bomb_target") )
		{
			g_iObjectiveEntType[g_iTotalObjectiveEnts] = OBJENT_BOMBSITE;
			bFound = true;
		}

		// Hostage escape zone
		else if ( equal( szClassName, "func_escapezone") )
		{
			g_iObjectiveEntType[g_iTotalObjectiveEnts] = OBJENT_HOSTAGE_ESCAPE;
			bFound = true;
		}

		// We found an objective entity!!!
		if ( bFound )
		{
			g_iObjectiveEnt[g_iTotalObjectiveEnts] = i;

			get_brush_entity_origin( i, g_fObjectiveOrigin[g_iTotalObjectiveEnts] );

			// Increment the total number
			g_iTotalObjectiveEnts++;
		}
	}

	// Lets find out if we should disable orc nades or gloves of warmth
	g_bOrcNadesDisabled		= WC3_MapDisableCheck( "skill_orc_nade.cfg" );
	g_bGlovesDisabled		= WC3_MapDisableCheck( "item_gloves.cfg" );
	g_bShadowSerpentWardDisabled = WC3_MapDisableCheck("skill_shadow_serpent_ward.cfg");

	//Для shopmenu4
	bDisabledDevourerLight		= WC3_MapDisableCheck( "item_devourer_light.cfg" );		//Пожиратель змей
	bDisabledIceBlock			= WC3_MapDisableCheck( "item_ice_block.cfg" );			//Ледяная глыба
	bDisabledThief				= WC3_MapDisableCheck( "item_thief.cfg" );				//похититель денег
	bDisabledMirrorUltimate		= WC3_MapDisableCheck( "item_mirror_ultimate.cfg" );	//Отражатель супер навыка
	bDisabledDeathcoil			= WC3_MapDisableCheck( "item_deathcoil.cfg" );			//Лик смерти
	bDisabledImpale				= WC3_MapDisableCheck( "item_impale.cfg" );				//Пронзающий шип
	bDisabledSleep				= WC3_MapDisableCheck( "item_sleep.cfg" );				//Сонная аура
	bDisabledBookDead			= WC3_MapDisableCheck( "item_book_dead.cfg" );			//Книга мертвых
	bDisabledTranquility		= WC3_MapDisableCheck( "item_tranquility.cfg" );		//Спокойствие

//	new szCurMap[32];
//	get_mapname( szCurMap, 31 );
//	WC3_Log( false, "** Map %s started, version: '%s'", szCurMap, WC3VERSION );
}

public _WC3_RunAfterConfig()
{
	// Set which string should be displayed with messages (war3ft or war3)
//	if ( get_pcvar_num( CVAR_wc3_races ) < 5 ){}

	// Configure the XP based on level
	XP_Configure();

	// Set up our CVARs - some of them
	CVAR_Configure();

	// Randomize Chameleon if necessary
	CHAM_Randomize();

	// Инициализация цен на предметы
	ITEM_InitCost();
}

public WC3_DetermineGame()
{
	if ( is_running( "cstrike" ) )
	{
		g_MOD = GAME_CSTRIKE;
	}
	else if ( is_running( "czero" ) )
	{
		g_MOD = GAME_CZERO;
	}
}

// Function will prompt a user for a race or skill selection
public WC3_GetUserInput( idUser )
{
	
	if ( !WC3_Check() )
	{
		return;
	}

	if ( idUser >= TASK_GETINPUT )
	{
		idUser -= TASK_GETINPUT;
	}

	if ( !p_data_b[idUser][PB_ISCONNECTED] )
	{
		return;
	}


	// User has no race
	if ( p_data[idUser][P_RACE] == 0 )
	{
		new menu_id, keys;

		new menuUp = player_menu_info( idUser, menu_id, keys );

		// Only display menu if another isn't shown
		if ( menuUp <= 0 || menu_id < 0 )
		{
			WC3_ChangeRaceStart( idUser );
		}

		// Try again in 1 second
		else
		{
			set_task( 1.0, "WC3_GetUserInput", TASK_GETINPUT + idUser );
		}
	}

	// User has skills points available
	else if ( SM_TotalSkillPointsUsed( idUser ) < p_data[idUser][P_LEVEL] )
	{
		MENU_SelectSkill( idUser );
	}

	else
	{
		WC3_ShowBar( idUser );
	}

	return;
}

// Function will "reset" the game - i.e. "sv_restart 1"
//Функция срабатывает при рестарте
public WC3_ResetGame()
{
	new players[32], numplayers, idUser, i
	get_players(players, numplayers);

	for ( i=0; i < numplayers; i++ )
	{
		idUser = players[i];
		
		// Remove Ultimate Delay
		task_exists( TASK_UDELAY + idUser ) ? remove_task( TASK_UDELAY + idUser ) : 0;
		
		// User didn't die last round... 
		p_data_b[idUser][PB_DIEDLASTROUND]		= false;

		//Обнуления заморозки скилов
		p_data_b[idUser][PB_ISFROZENSKILL]		= false;

		//Обнуления заморозки игрока от Ледяной стрелы
		p_data_b[idUser][PB_ISFROZENPLAYER]		= false;
		
		// Reset item info
		g_bPlayerBoughtAnkh[idUser]		= false;
		g_bPlayerBoughtMole[idUser]		= false;

		// Remove player's items
		ITEM_Remove( idUser, ITEM_SLOT_ONE );
		ITEM_Remove( idUser, ITEM_SLOT_TWO );
		ITEM_Remove( idUser, ITEM_SLOT_THREE );
		
		// Reset user's XP if we're not saving XP
		if ( !get_pcvar_num( CVAR_wc3_save_xp ) )
		{
			p_data[idUser][P_LEVEL]					= 0;
			p_data[idUser][P_RACE]					= 0;
			p_data[idUser][P_XP]					= 0;
			SM_ResetSkillLevels( idUser );
			SM_ResetSkills( idUser );
		}

		// Check for Counter-Strike or Condition Zero
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			p_data[idUser][P_HECOUNT]			= 0;
			p_data[idUser][P_FLASHCOUNT]		= 0;
		}
	}

	WC3_ShowBar( idUser );

	g_GameRestarting = false;
}

// Function will grab XP for the user
WC3_ChangeRaceStart( idUser )
{
	
	// Make sure the user is on a team!
	if ( SHARED_IsOnTeam( idUser ) )
	{
		// Get the XP if we're saving XP
		if ( get_pcvar_num( CVAR_wc3_save_xp ) )
		{
			// This function will also display the changerace menu
			DB_GetAllXP(idUser);

			//Для talisman
			if(arrPlayerInitTalisman[idUser] == false)
			{
				fTalismanInit(idUser);

				arrPlayerInitTalisman[idUser] = true;
			}
		}
		
		// We're not saving XP, so lets just change the user's race
		else
		{
			WC3_ChangeRaceShowMenu( idUser );

		}
	}

	else
	{
		format(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_WC3_CHANGE_RACE_START");
		client_print( idUser, print_center,szMessage);
	}
}

// Function will show the "select a race" menu to the user
WC3_ChangeRaceShowMenu( idUser, iRaceXP[MAX_RACES] = {0})
{

	// We don't want to replace the player's current XP with whats in the database now do we ?
	if ( p_data[idUser][P_RACE] )
	{
		iRaceXP[p_data[idUser][P_RACE]-1] = p_data[idUser][P_XP];
	}

	// Need to call this here
	MENU_ChangeRace( idUser, iRaceXP );
}

// This will actually give the user a given race
WC3_SetRace( idUser, race )
{
	// Clear any ultimate icons that might exist
	ULT_ClearIcons( idUser );
	
	// Play level up sound
	emit_sound( idUser, CHAN_STATIC, g_szSounds[SOUND_LEVELUP], 1.0, ATTN_NORM, 0, PITCH_NORM );

	// Set the user's race
	p_data[idUser][P_RACE] = race

	// Reset all race data
	SM_ResetSkillLevels( idUser );
	SM_ResetSkills( idUser );
	p_data[idUser][P_CHANGERACE] = 0

	// This function will assign the race's skills to this player
	SM_SetPlayerRace( idUser, p_data[idUser][P_RACE] );

	if ( get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		p_data[idUser][P_LEVEL] = 0
		DB_SetDataForRace( idUser );
	}
	else
	{
		WC3_SetRaceUp( idUser );
	}

	return;
}

// Function called right after the user's race information is set
WC3_SetRaceUp( idUser )
{
	// Do we need to give this user XP?
	new iStartLevel = get_pcvar_num( CVAR_wc3_start_level );
	if ( p_data[idUser][P_XP] == 0 && iStartLevel > 0 && iStartLevel <= MAX_LEVELS )
	{
		p_data[idUser][P_XP] = XP_GetByLevel( iStartLevel );

		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L ^x03%d",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_WC3_SET_RACE_UP",
				iStartLevel);	
			
		cssbColoredPrint(idUser, szMessage);
		
		//WC3_Log( true, "Your race has started off at level %d!", iStartLevel );
	}

	// Need to check the exception here for randomized chameleon!
	if ( p_data[idUser][P_RACE] == RACE_CHAMELEON && get_pcvar_num( CVAR_wc3_cham_random ) )
	{
		// Skill levels were just loaded from the DB - we don't care about these
		SM_ResetSkillLevels( idUser );

		// Fill all skills randomly!
		CHAM_ConfigureSkills( idUser );
	}

	WC3_InitPlayerSkills( idUser );

	// Copy the global ULT timeout over to just this user...
	fSetUltimateDelay(idUser);
	
	// See if there are any skills available
	new iSkillsUsed = SM_TotalSkillPointsUsed( idUser );
	if ( iSkillsUsed < p_data[idUser][P_LEVEL] )
	{
		MENU_SelectSkill( idUser );
	}
	
	XP_Check( idUser, false );
	WC3_ShowBar( idUser );
	WC3_ShowRaceInfo( idUser );

	// Show ultimate info if available
	ULT_IconHandler( idUser );

	
}

public WC3_ShowBar( idUser )
{
	if (idUser >= TASKID_HUDINFO )
		idUser -= TASKID_HUDINFO;
	
	// User is not connected
	if ( !p_data_b[idUser][PB_ISCONNECTED] )
		return;

	new szString[256], pos = 0, szXPInfo[32],szCurrentItems[32];
	new szItemInfo[256], szRaceInfo[256];
	new szRaceName[64], szShortRaceName[32];//, szItemName[32];

	// Get the race names
	lang_GetRaceName( p_data[idUser][P_RACE], idUser, szRaceName, 63 );
	lang_GetRaceName( p_data[idUser][P_RACE], idUser, szShortRaceName, 31, true );
	
	// This information is displayed differently for CS/CZ
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// No race selected
		if ( !p_data[idUser][P_RACE] )
		{
			pos += formatex( szRaceInfo[pos], 255, "%s ", szRaceName );
		}

		// User has a race
		else
		{

			// User is level 0
			if ( p_data[idUser][P_LEVEL] == 0 )
			{
				pos += formatex( szRaceInfo[pos], 255, "%s  XP: %d/%d ", szRaceName, p_data[idUser][P_XP], XP_GetByLevel( p_data[idUser][P_LEVEL]+1 ) );
				formatex( szXPInfo, 31, "XP: %d/%d", p_data[idUser][P_XP], XP_GetByLevel( p_data[idUser][P_LEVEL]+1 ) );
			}

			//CSSB
			// User is under level MAX_LEVELS
			else if ( p_data[idUser][P_LEVEL] < MAX_LEVELS )
			{
				pos += formatex( szRaceInfo[pos], 255, "%s %L: %d   XP: %d/%d ", szShortRaceName, LANG_PLAYER, "WORD_LEVEL", p_data[idUser][P_LEVEL], p_data[idUser][P_XP], XP_GetByLevel( p_data[idUser][P_LEVEL]+1) );
				formatex( szXPInfo, 31, "%L: %d   XP: %d/%d", LANG_PLAYER, "WORD_LEVEL", p_data[idUser][P_LEVEL], p_data[idUser][P_XP], XP_GetByLevel( p_data[idUser][P_LEVEL]+1 ) );
			}			
				
			// User is level MAX_LEVELS
			else
			{
				pos += formatex( szRaceInfo[pos], 255, "%s %L: %d   XP: %d ", szShortRaceName, LANG_PLAYER, "WORD_LEVEL", p_data[idUser][P_LEVEL], p_data[idUser][P_XP] );
				formatex( szXPInfo, 31, "%L: %d   XP: %d", LANG_PLAYER, "WORD_LEVEL", p_data[idUser][P_LEVEL], p_data[idUser][P_XP] );
			}
		}
	}

	
	
	// Reset our position since we're using a new array
	pos = 0;

	new szItemName1[64], szItemName2[64],szItemName3[64];

	// Игрок имеет одну вещь
	if ( g_iShopMenuItems[idUser][ITEM_SLOT_ONE] > ITEM_NONE )
	{
		ITEM_Format( idUser, g_iShopMenuItems[idUser][ITEM_SLOT_ONE], szItemName1,charsmax(szItemName1))

		//if(!is_user_bot(idUser))
		//	client_print(idUser, print_chat, "1: %s", szItemName1 );

		formatex(szCurrentItems, 31, "%L:",LANG_PLAYER,"WC3_CURRENT_ITEM");
		formatex(szItemInfo, charsmax(szItemInfo), "%s",szItemName1);

		//if(!equali(szItemName1,""))
		//	bIsItem1 = true;
	}

	// Игрок имеет две вещи
	if ( g_iShopMenuItems[idUser][ITEM_SLOT_TWO] > ITEM_NONE )
	{
		ITEM_Format( idUser, g_iShopMenuItems[idUser][ITEM_SLOT_TWO], szItemName2,charsmax(szItemName2) )

		// Then the string isn't empty and we have information in it (so we have a first item)
		if ( g_iShopMenuItems[idUser][ITEM_SLOT_ONE] > ITEM_NONE )
		{
			formatex(szCurrentItems, 31, "%L:",LANG_PLAYER,"WC3_CURRENT_ITEMS" );
			formatex(szItemInfo, charsmax(szItemInfo), "%s %L %s",
					szItemName1,
					LANG_PLAYER, "WORD_AND",
					szItemName2);
		}

		// We don't need the word "and"
		else
		{
			formatex(szItemInfo, charsmax(szItemInfo), "%s",szItemName2);
		}

	}

	// Игрок имеет три вещи
	if ( g_iShopMenuItems[idUser][ITEM_SLOT_THREE] > ITEM_NONE )
	{
		ITEM_Format( idUser, g_iShopMenuItems[idUser][ITEM_SLOT_THREE], szItemName3,charsmax(szItemName3) )
	
		formatex(szCurrentItems, 31, "%L:",LANG_PLAYER,"WC3_CURRENT_ITEMS" );

		new bool:bIsItemSlots = false;
		if ( g_iShopMenuItems[idUser][ITEM_SLOT_ONE] > ITEM_NONE && g_iShopMenuItems[idUser][ITEM_SLOT_TWO] == ITEM_NONE )
		{
			formatex(szItemInfo, charsmax(szItemInfo), "%s %L %s",
					szItemName1,
					LANG_PLAYER, "WORD_AND",
					szItemName3);

			bIsItemSlots = true;
		}

		if ( g_iShopMenuItems[idUser][ITEM_SLOT_TWO] > ITEM_NONE && g_iShopMenuItems[idUser][ITEM_SLOT_ONE] == ITEM_NONE )
		{
			formatex(szItemInfo, charsmax(szItemInfo), "%s %L %s",
					szItemName2,
					LANG_PLAYER, "WORD_AND",
					szItemName3);

			bIsItemSlots = true;
		}

		if ( g_iShopMenuItems[idUser][ITEM_SLOT_ONE] > ITEM_NONE && g_iShopMenuItems[idUser][ITEM_SLOT_TWO] > ITEM_NONE )
		{
			formatex(szItemInfo, charsmax(szItemInfo), "%s,%s,%s",szItemName1,szItemName2,szItemName3);

			bIsItemSlots = true;
		}

		// We don't need the word "and"
		if(bIsItemSlots == false)
		{
			formatex(szItemInfo, charsmax(szItemInfo), "%s",szItemName3);
		}

	}

	new szRaceItemInfo[256];
	//CSSB
	// Put the final string together
	formatex( szString, 255, "%s%s", szRaceInfo, szItemInfo );
	formatex( szRaceItemInfo, 255, "%s^n%s %s", szRaceInfo,szCurrentItems, szItemInfo);
	
	if ( SHARED_IsOnTeam( idUser ) )
	{
		// Display the item + race info with a hudmessage
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{

			if ( is_user_alive( idUser ) )
			{
				//Для talisman +
				new cRed = get_pcvar_num(CVAR_wc3_status_color_R);
				new cGreen = get_pcvar_num(CVAR_wc3_status_color_G);
				new cBlue = get_pcvar_num(CVAR_wc3_status_color_B);

				//Отображение в статусной строке сколько талисманов и какой активен
				new szItemNameShort[32];
				if(arrTalismanActive[idUser] != TALISMAN_ITEM_NONE)
					LANG_GetItemName(arrTalismanActive[idUser], idUser, szItemNameShort, 31, true );
				else
					formatex(szItemNameShort, charsmax(szItemNameShort), "%L",LANG_PLAYER,"WC3_SHOW_STATUS_TALISMAN_ACTIVE_NO");

				new szStringStatusTalisman[256];
				new szShowStatus[256];

				if(MAX_MODE_TALISMAN == 1 || MAX_MODE_TALISMAN == 2)
				{
					if(arrTalismanActive[idUser] == TALISMAN_ITEM_NECKLACE)
						formatex(szStringStatusTalisman, charsmax(szStringStatusTalisman), "%L[%d]",LANG_PLAYER,"WC3_SHOW_STATUS_TALISMAN_ACTIVE",szItemNameShort,arrTalismanNecklaceNum[idUser]);
					else if(arrTalismanActive[idUser] == TALISMAN_ITEM_HELM)
						formatex(szStringStatusTalisman, charsmax(szStringStatusTalisman), "%L[%d]",LANG_PLAYER,"WC3_SHOW_STATUS_TALISMAN_ACTIVE",szItemNameShort,arrTalismanHelmNum[idUser]);
					else
						formatex(szStringStatusTalisman, charsmax(szStringStatusTalisman), "%L",LANG_PLAYER,"WC3_SHOW_STATUS_TALISMAN_ACTIVE",szItemNameShort);

					formatex(szShowStatus, charsmax(szShowStatus), "%s^n%s^n%s %s",szRaceInfo,szStringStatusTalisman,szCurrentItems, szItemInfo);

					set_hudmessage(cRed,cGreen,cBlue, 0.012, 0.90, HUDMESSAGE_FX_FADEIN, 10.0, 5.0, 2.0, 3.0, HUD_XP);
					show_hudmessage(idUser, szShowStatus);
				}
				else if(MAX_MODE_TALISMAN == 3)
				{
					if((get_user_flags(idUser) & fVipTalisman()))
					{
						if(arrTalismanActive[idUser] == TALISMAN_ITEM_NECKLACE)
							formatex(szStringStatusTalisman, charsmax(szStringStatusTalisman), "%L[%d]",LANG_PLAYER,"WC3_SHOW_STATUS_TALISMAN_ACTIVE",szItemNameShort,arrTalismanNecklaceNum[idUser]);
						else if(arrTalismanActive[idUser] == TALISMAN_ITEM_HELM)
							formatex(szStringStatusTalisman, charsmax(szStringStatusTalisman), "%L[%d]",LANG_PLAYER,"WC3_SHOW_STATUS_TALISMAN_ACTIVE",szItemNameShort,arrTalismanHelmNum[idUser]);
						else
							formatex(szStringStatusTalisman, charsmax(szStringStatusTalisman), "%L",LANG_PLAYER,"WC3_SHOW_STATUS_TALISMAN_ACTIVE",szItemNameShort);

						formatex(szShowStatus, charsmax(szShowStatus), "%s^n%s^n%s %s",szRaceInfo,szStringStatusTalisman,szCurrentItems, szItemInfo);
						
						set_hudmessage(cRed,cGreen,cBlue, 0.012, 0.90, HUDMESSAGE_FX_FADEIN, 10.0, 5.0, 2.0, 3.0, HUD_XP);
						show_hudmessage(idUser, szShowStatus);
					}
					else
					{
						set_hudmessage(cRed,cGreen,cBlue, 0.012, 0.92, HUDMESSAGE_FX_FADEIN, 10.0, 5.0, 2.0, 3.0, HUD_XP);
						show_hudmessage(idUser, szRaceItemInfo);
					}
				}

//				Create_StatusText( idUser, 0, szStringStatusTalisman );

//			set_dhudmessage( 0, 255, 0, 0.012, 0.82, HUDMESSAGE_FX_FADEIN, 10.0, 540.0, 2.0, 3.0, HUD_XP );
//			show_dhudmessage(idUser,"%s", szRaceItemInfo);
/*
				new szNameLang[28];
				if (vaultdata_exists("server_language"))
				{
					get_vaultdata("server_language", szNameLang, 27);
				} 
				else 
				{
					copy(szNameLang, 27, "en")
					//set_vaultdata("server_language", szNameLang);
				}
				
				if( equali(szNameLang,"ru") )
				{
					set_hudmessage( 0, 255, 0, 0.012, 0.92, HUDMESSAGE_FX_FADEIN, 10.0, 540.0, 2.0, 3.0, HUD_XP );
					show_hudmessage(idUser, szRaceItemInfo);
				}
				else
				{
					Create_StatusText( idUser, 0, szString );
				}
*/
			}

			else
			{
				set_hudmessage( 160, 160, 160, 0.012, 0.90, HUDMESSAGE_FX_FADEIN, 10.0, 0.0, 2.0, 3.0, HUD_XP );
				show_hudmessage( idUser, "%s", szXPInfo );
			}
		}

		set_task(10.0, "WC3_ShowBar", idUser + TASKID_HUDINFO);
	}

	return;
}

// Function will display the level information in the center of the user's screen
WC3_ShowRaceInfo( idUser )
{

	if ( p_data[idUser][P_RACE] != 0 )
	{
		new szSkillName[64], iSkillLevel;

		new szTrainable[256],	szUltimate[256],	szPassive[256];
		new posT = 0,			posU = 0,			posP = 0;

		// Loop through all available skills to find the trainable options...
		for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
		{
			// Get the skill's level
			iSkillLevel = SM_GetSkillLevel( idUser, iSkillID, 8 );

			// If the skill level is > 0 then the user has this skill!
			if ( iSkillLevel > 0 )
			{
				// Get the skill's name
				LANG_GetSkillName( iSkillID , idUser, szSkillName, 63, 19 );

				// Trainable skill
				if ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_TRAINABLE )
				{
					posT += formatex( szTrainable[posT], 255-posT, "^n%s %L %d", szSkillName, LANG_PLAYER, "WORD_LEVEL", iSkillLevel );
				}

				// Skill is an ultimate
				else if ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_ULTIMATE && iSkillLevel > 0 )
				{
					posU += formatex( szUltimate[posU], 255-posU, "^n%L: %s", LANG_PLAYER, "WORD_ULTIMATE", szSkillName );
				}

				// Skill is passive
				else if ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_PASSIVE )
				{
					posP += formatex( szPassive[posP], 255-posP, "^n%s", szSkillName );
				}
			}
		}

		// Get the race's name
		new szRaceName[64];
		lang_GetRaceName( p_data[idUser][P_RACE], idUser, szRaceName, 63 );
		
		// Add the header
		new szMsg[1024];
		formatex( szMsg, 1023, "%s^n%L %d", szRaceName, LANG_PLAYER, "WORD_LEVEL", p_data[idUser][P_LEVEL] );

		// Add all the skills to the message!
		strcat( szMsg, szTrainable, 1023 );
		strcat( szMsg, szUltimate, 1023 );
		strcat( szMsg, szPassive, 1023 );


		WC3_StatusText( idUser, TXT_RACE_INFO, szMsg );
	}
	
	// User has no race
	else
	{
		format(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_WC3_RACE_FIRST");
		WC3_StatusText( idUser, TXT_RACE_INFO,szMessage);
	}
}

// Function simply checks if an enemy of idUser is near vOrigin and has a necklace/warden's blink
WC3_IsImmunePlayerNear( idUser, vOrigin[3] )
{
	new players[32], numplayers, vTargetOrigin[3], i;
	new iTeam = get_user_team( idUser );

	// Get all players
	get_players( players, numplayers, "a" );
	
	// Loop through all players and check for immunity
	for ( i = 0; i < numplayers; i++ )
	{
		
		// Make sure that the user we're looking at is on the opposite team of "idUser"
		if ( get_user_team( players[i] ) != iTeam )
		{	
			get_user_origin( players[i], vTargetOrigin );
			
			// Check the distance
			if ( get_distance( vOrigin, vTargetOrigin ) <= IMMUNITY_RADIUS )
			{

				// Does this player have a necklace or warden's blink?
				if ( ULT_CanUserBlockUlt( players[i] ) )
				{
					return players[i];
				}
			}
		}
	}

	return 0;
}

WC3_ResetSkills( idUser )
{
	// Reset the user's skills
	if ( p_data_b[idUser][PB_RESETSKILLS] ) 
	{
		p_data[idUser][P_LEVEL]		= 0;
		SM_ResetSkillLevels( idUser );

		WC3_ShowBar( idUser );
		XP_Check( idUser, false );

		p_data_b[idUser][PB_RESETSKILLS] = false;

		return 1;
	}

	return 0;
}

WC3_ShowSpecInfo( idUser, iTargetID )
{
	// Get the target's name
	new szTargetName[32];
	get_user_name( iTargetID, szTargetName, 31 ); 
	
	// Get the target's race name
	new szRaceName[64];
	lang_GetRaceName( p_data[iTargetID][P_RACE], idUser, szRaceName, 63, true );
	
	new szMsg[512], iMsgPos = 0;

	// User is level 0
	if ( p_data[iTargetID][P_LEVEL] == 0 )
	{
		iMsgPos += formatex( szMsg, 511, "%s  XP: %d/%d", szRaceName, p_data[iTargetID][P_XP], XP_GetByLevel( p_data[iTargetID][P_LEVEL]+1 ) );
	}

	//CSSB
	// User is under level MAX_LEVELS
	else if ( p_data[iTargetID][P_LEVEL] < MAX_LEVELS )
	{
		iMsgPos += formatex( szMsg, 511, "%s %L: %d   XP: %d/%d", szRaceName, LANG_PLAYER, "WORD_LEVEL", p_data[iTargetID][P_LEVEL], p_data[iTargetID][P_XP], XP_GetByLevel( p_data[iTargetID][P_LEVEL]+1 ) );
	}			
		
	// User is level MAX_LEVELS
	else
	{
		iMsgPos += formatex( szMsg, 511, "%s %L: %d   XP: %d", szRaceName, LANG_PLAYER, "WORD_LEVEL", p_data[iTargetID][P_LEVEL], p_data[iTargetID][P_XP] );
	}
	
	//CSSB
	// Reset our position counter
	//new pos = 0;

	new szItemName[32], szItemName2[32],szItemName3[32],szCurrentItems[32];

	// Get the item and race names
	LANG_GetItemName( g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE], idUser, szItemName, 31, true );
	LANG_GetItemName( g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO], idUser, szItemName2, 31, true );
	LANG_GetItemName( g_iShopMenuItems[iTargetID][ITEM_SLOT_THREE], idUser, szItemName3, 31, true );
	
	new bool:ItemSlotOne = false;
	new bool:ItemSlotTwo = false;
	new bool:HasMoreThanOneRing = false;
	
	if ( g_iTotalRings[iTargetID] > 1 )
		HasMoreThanOneRing = true;

	//Игрок имеет однин предмет
	if ( g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE] > ITEM_NONE && g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE] != ITEM_MOLE )
	{
		formatex(szCurrentItems, 31, "%L:",LANG_PLAYER,"WC3_CURRENT_ITEMS" );

		// Then they have rings, lets print how many there are
		if ( HasMoreThanOneRing && g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE] == ITEM_RING )
			iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, "^n%s %s x%d",szCurrentItems, szItemName, g_iTotalRings[iTargetID] );
		else
			iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, "^n%s %s",szCurrentItems, szItemName );

		ItemSlotOne = true;
	}

	//Игрок имеет два предмета
	if ( g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO] > ITEM_NONE && g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO] != ITEM_MOLE )
	{
		// Then the string isn't empty and we have information in it (so we have a first item)
		if ( ItemSlotOne )
		{

			// Then they have rings, lets print how many there are
            if (HasMoreThanOneRing && g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO] == ITEM_RING)
                iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, " %L %s x%d", LANG_PLAYER, "WORD_AND", szItemName2, g_iTotalRings[iTargetID] );
            else
                iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, " %L %s", LANG_PLAYER, "WORD_AND", szItemName2 );

				ItemSlotTwo = true;
		}
		// We don't need the word "and"
		else
		{
			formatex(szCurrentItems, 31, "%L:",LANG_PLAYER,"WC3_CURRENT_ITEMS" );
			
			// Then they have rings, lets print how many there are
            if (HasMoreThanOneRing && g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO] == ITEM_RING)
                iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, "^n%s %s x%d",szCurrentItems, szItemName2, g_iTotalRings[iTargetID] );
            else
                iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, "^n%s %s",szCurrentItems, szItemName2 );
			
		}
		
	}

	//Игрок имеет три предмета
	if ( g_iShopMenuItems[iTargetID][ITEM_SLOT_THREE] > ITEM_NONE && g_iShopMenuItems[iTargetID][ITEM_SLOT_THREE] != ITEM_MOLE )
	{
		// Then the string isn't empty and we have information in it (so we have a first item)
		if ( ItemSlotOne == true && ItemSlotTwo == true )
		{
			// Then they have rings, lets print how many there are
            if (HasMoreThanOneRing && g_iShopMenuItems[iTargetID][ITEM_SLOT_THREE] == ITEM_RING)
                iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, " %L %s x%d", LANG_PLAYER, "WORD_AND", szItemName3, g_iTotalRings[iTargetID] );
            else
                iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, " %L %s", LANG_PLAYER, "WORD_AND", szItemName3 );
			
		}
		// We don't need the word "and"
		else
		{
			formatex(szCurrentItems, 31, "%L:",LANG_PLAYER,"WC3_CURRENT_ITEMS" );
			
			// Then they have rings, lets print how many there are
            if (HasMoreThanOneRing && g_iShopMenuItems[iTargetID][ITEM_SLOT_THREE] == ITEM_RING)
                iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, "^n%s %s x%d",szCurrentItems, szItemName3, g_iTotalRings[iTargetID] );
            else
                iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, "^n%s %s",szCurrentItems, szItemName3 );	
			
		}
		
	}

	//Для talisman +
	// Add the Health + Armor to the message
	iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, "^n%L: %d %L: %d", LANG_PLAYER, "CURRENT_HEALTH", get_user_health( iTargetID ), LANG_PLAYER, "WORD_ARMOR", get_user_armor( iTargetID ) );
		
	// Format the message
	new Float:fSpecTime = SPEC_HOLDTIME;

	//Для talisman
	if (get_pcvar_num( CVAR_wc3_spec_position ) == 0 )
		set_hudmessage( 255, 255, 255, 0.018, 0.89, 2, 1.5, fSpecTime, 0.02, 5.0, HUD_SPEC_INFO );
	else
		set_hudmessage( 255, 255, 255, 0.65, 0.89, 2, 1.5, fSpecTime, 0.02, 5.0, HUD_SPEC_INFO );

	//Отображение в статусной строке сколько талисманов и какой активен
	new szItemNameShort[32];
	if(arrTalismanActive[iTargetID] != TALISMAN_ITEM_NONE)
		LANG_GetItemName(arrTalismanActive[iTargetID], iTargetID, szItemNameShort, 31, true );
	else
		formatex(szItemNameShort, charsmax(szItemNameShort), "%L",LANG_PLAYER,"WC3_SHOW_STATUS_TALISMAN_ACTIVE_NO");

	if(MAX_MODE_TALISMAN == 1)
	{
		iMsgPos += formatex(szMsg[iMsgPos], 512-iMsgPos, "^n%L^n%L",LANG_PLAYER,"WC3_SHOW_STATUS_TALISMAN_ACTIVE",szItemNameShort,LANG_PLAYER,"WC3_SHOW_STATUS_TALISMAN_NUM",arrTalismanNum[iTargetID]);
		show_hudmessage(idUser, szMsg);
	}
	else if(MAX_MODE_TALISMAN == 2)
	{
		iMsgPos += formatex(szMsg[iMsgPos], 512-iMsgPos, "^n%L",LANG_PLAYER,"WC3_SHOW_STATUS_TALISMAN_ACTIVE",szItemNameShort);
		show_hudmessage(idUser, szMsg);
	}
	else if(MAX_MODE_TALISMAN == 3)
	{
		if((get_user_flags(iTargetID) & fVipTalisman()))
		{
			iMsgPos += formatex(szMsg[iMsgPos], 512-iMsgPos, "^n%L",LANG_PLAYER,"WC3_SHOW_STATUS_TALISMAN_ACTIVE",szItemNameShort);
			show_hudmessage(idUser, szMsg);
		}
		else
		{
			show_hudmessage(idUser, szMsg );
		}
	}

}

// Function will print a message in the center of the screen
WC3_StatusText( idUser, iType, const fmt[], ... )
{
	static szFormattedText[512];

	vformat( szFormattedText, 511, fmt, 4 );

	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{

		// I.E. "You have gained a level"
		if ( iType == TXT_TOP_CENTER )
		{
			set_hudmessage( 200, 100, 0, -1.0, 0.25, HUDMESSAGE_FX_FADEIN, 1.0, 2.0, 0.1, 0.2, HUD_AUTO );
		}

		// I.E. /level
		else if ( iType == TXT_RACE_INFO )
		{
			set_hudmessage( 255, 255, 255, -1.0, 0.3, HUDMESSAGE_FX_FADEIN, 3.0, 5.0, 0.1, 0.2, HUD_RACE_INFO );
		}

		// Ultimate messages
		else if ( iType == TXT_ULTIMATE )
		{
			set_hudmessage( 255, 208, 0, -1.0, 0.85, HUDMESSAGE_FX_FADEIN, 6.0, 0.5, 0.1, 0.5, HUD_ULTIMATE );
		}

		// I.E. Suicide Bomb Armed
		else if ( iType == TXT_BLINK_CENTER )
		{
			
			set_hudmessage( 79, 209, 204, -1.0, -0.4, HUDMESSAGE_FX_FLICKER, 0.5, 1.7, 0.2, 0.2, HUD_AUTO );
		}

		// I.E. "You have evaded a shot"
		else if ( iType == TXT_SKILL )
		{
			set_hudmessage( 0, 0, 255, 0.75, 0.85, HUDMESSAGE_FX_FADEIN, 6.0, 3.0, 0.2, 0.7, HUD_SKILL );
		}

		// I.E.
		else if ( iType == TXT_TIMER )
		{
			set_hudmessage( 0, 100, 0, 0.05, 0.65, HUDMESSAGE_FX_WRITEOUT, 0.02, 10.0, 0.01, 0.1, HUD_TIMER );	
		}
		else if ( iType == ICE_BLOCK_TIMER )
		{
			set_hudmessage( 0, 206, 209, 0.05, 0.55, HUDMESSAGE_FX_FLICKER, 0.5, 2.0, 0.2, 0.2, HUD_AUTO );
		}

		// All other cases
		else
		{
			set_hudmessage( 255, 255, 10, -1.0, -0.4, HUDMESSAGE_FX_FLICKER, 0.5, 2.0, 0.2, 0.2, HUD_AUTO );
		}

		show_hudmessage( idUser, szFormattedText );
	}

}
//#define CSW_CONCOCTION		61
//#define SH_CONCOCTION_DAMAGE	15		// Damage done by Unstable Concoction
//WC3_Damage( players[i], iVictim, SH_CONCOCTION_DAMAGE, CSW_CONCOCTION, 0 );
public WC3_Damage( iVictim, iAttacker, iDamage, iWeapon, iBodyPart )
{

	// We have an invalid attacker/victim, that sucks...
	if ( iAttacker == 0 || iVictim == 0 )
		return;
	
	// User is not alive, why damage them more?
	if ( !is_user_alive( iVictim ) )
		return;

	// Don't damage if we shouldn't!
	if ( p_data_b[iVictim][PB_NO_DAMAGE] )
		return;

	// Warden's Hardened Skin
	iDamage = WA_HardenedSkin( iVictim, iDamage );

	if ( iBodyPart == -1 )
	{
		iBodyPart = random_num( 1, 7 );
	}

	// Modify the amount of damage done based on the user's armor
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		new Float:fNewDamage	= float( iDamage ) * ARMOR_RATIO;
		new Float:fArmorLost	= ( float( iDamage ) - fNewDamage ) * ARMOR_BONUS;
		new Float:fCurrentArmor	= float( get_user_armor( iVictim ) );

		// Does this use more armor than we have?
		if ( fArmorLost > fCurrentArmor )
		{
			fArmorLost = fCurrentArmor;
			fArmorLost *= ( 1 / ARMOR_BONUS );

			fNewDamage = float( iDamage ) - fArmorLost;

			set_user_armor( iVictim, 0 );
		}
		else
		{
			new iNewArmor = floatround( fCurrentArmor - ( 3.0 * fArmorLost ) );

			set_user_armor( iVictim, iNewArmor );
		}

		iDamage = floatround( fNewDamage );
	}

	new iHeadshot = 0;

	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( iBodyPart == 1 )
		{
			iHeadshot = 1;
		}
	}

	// Psychostats Statistics is turned on!
	if ( get_pcvar_num( CVAR_wc3_psychostats ) )
	{
		if ( CSW_WAR3_MIN <= iWeapon <= CSW_WAR3_MAX )
		{
			new iSkillWeapon = iWeapon - CSW_WAR3_MIN;
			
			// Make the "generic" the stomach
			if ( iBodyPart == -1 )
			{
				iBodyPart = HIT_STOMACH;
			}
		
			if ( iBodyPart == HIT_HEAD )
			{
				iStatsHead[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_CHEST )
			{
				iStatsChest[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_STOMACH )
			{
				iStatsStomach[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_LEFTARM )
			{
				iStatsLeftArm[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_RIGHTARM )
			{
				iStatsRightArm[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_LEFTLEG )
			{
				iStatsLeftLeg[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_RIGHTLEG )
			{
				iStatsRightLeg[iAttacker][iSkillWeapon]++;
			}

			iStatsHits[iAttacker][iSkillWeapon]++;
			iStatsShots[iAttacker][iSkillWeapon]++;
			iStatsDamage[iAttacker][iSkillWeapon] += iDamage;
		}
	}

	//Стандартный код
	// Magical damage should go toward kill assist
	g_iDamageDealt[iAttacker][iVictim] += iDamage;

	new iHealth = get_user_health( iVictim );
	
	// User has been killed
	if ( iHealth - iDamage <= 0 )
	{
		WC3_Kill( iVictim, iAttacker, iWeapon, iHeadshot );
	}

	// Just do the damage
	else
	{
		set_user_health( iVictim, iHealth - iDamage );
	}

	/*
	//Не стандартный код
	// Magical damage should go toward kill assist
	g_iDamageDealt[iAttacker][iVictim] += iDamage;

	new iHealth = get_user_health( iVictim );
		
	// User has been killed
	if ( iHealth - iDamage <= 0 )
	{
		if(p_data[iVictim][P_RACE] == RACE_ELF || p_data[iAttacker][P_RACE] == RACE_ELF)
		{
			if(iHealth > 0)
				set_user_health(iAttacker, iHealth );
		}
		else
		{
			WC3_Kill( iVictim, iAttacker, iWeapon, iHeadshot );	
		}

	}

	// Just do the damage
	else
	{
		set_user_health( iVictim, iHealth - iDamage );
	}*/

	return;
}

public WC3_Death( idVictim, iKiller, iWeaponID, iHeadshot )
{
	// Award XP	
	XP_onDeath( idVictim, iKiller, iWeaponID, iHeadshot );

	// Reset the "about to spawn" variable
	p_data[idVictim][P_RESPAWNBY] = 0;
	
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Remove the bar time if the user dies during god mode (in theory this shouldn't be possible)
		if ( p_data_b[idVictim][PB_GODMODE] )
		{
			Create_BarTime( idVictim, 0, 0 );
		}

		// Lets give a little extra money if CSDM is on...
		if ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 )
		{
			SHARED_SetUserMoney( iKiller, SHARED_GetUserMoney( iKiller ) + 300, 1 );
		}
	}

	// Give the bot a chance to respawn
	if ( is_user_bot( idVictim ) )
	{
		if ( random_float(0.01, 1.0 ) <= BOT_RESPAWN )
		{
			_MENU_Shopmenu2( idVictim, ITEM_SCROLL - 1 );
		}
	}

	// Remove icon because you don't have an ultimate when you're dead
	ULT_Icon( idVictim, ICON_HIDE );	

	// Player is no longer a mole after they die, right?
	p_data_b[idVictim][PB_MOLE] = false;

	// Yes the user died...
	p_data_b[idVictim][PB_DIEDLASTROUND] = true;

	// Suicide Bomber should go off when the user dies
	if ( SM_GetSkillLevel( idVictim, ULTIMATE_SUICIDE ) > 0 )
	{	
		// User can't use their ultimate when they're hexed
		if ( p_data_b[idVictim][PB_HEXED] )
		{
			WC3_StatusText( idVictim, TXT_ULTIMATE, "%L", LANG_PLAYER, "HEX_NO_ABILITY" );

			client_cmd( idVictim, "speak %s", g_szSounds[SOUND_ERROR] );
		}
		else
		{
			UD_Suicide( idVictim );
		}
	}
	

	// Check to see if player should respawn
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{

		// Check to see if a player should be revived
		BM_PhoenixSpawn( idVictim );

		// Does the user have a scroll?
		if ( ITEM_Has( idVictim, ITEM_SCROLL ) > ITEM_NONE )
		{
			ITEM_Scroll( idVictim );
		}

		// Should we respawn for Vengeance?
		if ( SM_GetSkillLevel( idVictim, ULTIMATE_VENGEANCE ) > 0 && iKiller != idVictim && !g_EndRound )
		{
			// Killer has immunity, user shouldn't respawn :/
			if ( ULT_CanUserBlockUlt( iKiller ) )
			{
				ULT_RemoveCharge( iKiller );
			
				format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
					LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
					LANG_PLAYER,"CLIENT_PRINT_WC3_DEATH");	
			
				cssbColoredPrint(idVictim, szMessage);
					
			}
			
			// User can't use their ultimate when they're hexed
			else if ( p_data_b[idVictim][PB_HEXED] )
			{
				WC3_StatusText( idVictim, TXT_ULTIMATE, "%L", LANG_PLAYER, "HEX_NO_ABILITY" );
			
				client_cmd( idVictim, "speak %s", g_szSounds[SOUND_ERROR] );
			}
			
			// Otherwise lets spawn them!!!
			else
			{
				WA_ULT_Vengeance( idVictim );
			}
		}
	}

	// Remove hex if the user is hexed
	if ( p_data_b[idVictim][PB_HEXED] )
	{
		if ( task_exists( TASK_HEX + idVictim) )
		{
			remove_task( TASK_HEX + idVictim );
		}

		_SH_RemoveHex( idVictim + TASK_HEX );
	}


	// Удаление заморозки скилов
	if ( p_data_b[idVictim][PB_ISFROZENSKILL] == true )
	{
		if (task_exists( TASK_FROZEN_SKILL + idVictim) )
			remove_task( TASK_FROZEN_SKILL + idVictim );

		fRemoveFrozenSkill( idVictim + TASK_FROZEN_SKILL );
	}

	// Удаление заморозки от Скила Ледяная стрела
	if ( p_data_b[idVictim][PB_ISFROZENPLAYER] == true )
	{
		if (task_exists( TASK_REMOVE_FROZEN + idVictim) )
			remove_task( TASK_REMOVE_FROZEN + idVictim );

		fRemoveFreeze(idVictim + TASK_REMOVE_FROZEN );
	}


////Выпадение предметов BEGIN {

	if(get_pcvar_num( CVAR_wc3_loot_items ) > 0)
	{
		//Определение какие предметы были у жертвы	
		//Для shopmenu
		new iVictimItemShop1 = bIsItemUserSlot(idVictim,ITEM_SLOT_ONE);
		//Для shopmenu2
		new iVictimItemShop2 = bIsItemUserSlot(idVictim,ITEM_SLOT_TWO);
		//Для shopmenu3
		new iVictimItemShop3 = bIsItemUserSlot(idVictim,ITEM_SLOT_THREE);

		//Проверка на наличие вещей
		new bIsDropItems = fIsDropItems(iVictimItemShop1,iVictimItemShop2,iVictimItemShop3);
		if(bIsDropItems != 0)//Если есть предметы
		{
			if (random_num(0, 100 ) <= get_pcvar_num( CVAR_wc3_loot_items_percent ) ) 
			{
				ArrayPushCell(arrIntItemVictimShop1, iVictimItemShop1);
				ArrayPushCell(arrIntItemVictimShop2, iVictimItemShop2);
				ArrayPushCell(arrIntItemVictimShop3,iVictimItemShop3);
				ArrayPushCell(arrIntDrawSpriteItems, idSpriteItems);
				ArrayPushCell(arrIntScaleSpriteItems, 8);

				if (get_pcvar_num( CVAR_wc3_loot_remove ) == 1)
					ArrayPushCell(arrIntLootRemove, get_pcvar_num( CVAR_wc3_loot_stay ));
				

				//Определение цвета вещей
				fColorItems(iVictimItemShop1,iVictimItemShop2,iVictimItemShop3);
				//Создание модели брони
				fCreateItemsClass(idVictim);
			
			}

		}
		else//Если нет предметов
		{
			if(get_pcvar_num( CVAR_wc3_loot_items ) == 2)
			{
				if (random_num(0, 100 ) <= get_pcvar_num( CVAR_wc3_loot_ar_hp_xp_percent ) ) 
				{
					ArrayPushCell(arrIntItemVictimShop1, iVictimItemShop1);
					ArrayPushCell(arrIntItemVictimShop2, iVictimItemShop2);
					ArrayPushCell(arrIntItemVictimShop3,iVictimItemShop3);
					ArrayPushCell(arrIntDrawSpriteItems, idSpriteNoItems);
					ArrayPushCell(arrIntScaleSpriteItems, 3);

					if (get_pcvar_num( CVAR_wc3_loot_remove ) == 1)
						ArrayPushCell(arrIntLootRemove, get_pcvar_num( CVAR_wc3_loot_stay ));

					//Определение цвета вещей
					fColorItems(iVictimItemShop1,iVictimItemShop2,iVictimItemShop3);
					//Создание модели брони
					fCreateArXpHpClass(idVictim);

				}
			}
		}
		
		//Звук при выпадении предмета
		//PlayEmitSound(idVictim, szSoundLootItems);
	}

//Выпадение предметов END }

	//Для shopmenu4
	//Удаление из массива скелета если жертва им является
	sm4_RemoveSkeleton(idVictim);

	//Остановка работы предмета Спокойствие
	sm4_4_9_fRemoveSpellCasting( idVictim );
	remove_task( TASK_TRANQUILITY + idVictim );

	ITEM_UserDied( idVictim );

	//Для talisman
	fTalismanUserDied(idVictim);

	set_task( 1.0, "WC3_GetUserInput", TASK_GETINPUT + idVictim );

	return;
}

public WC3_Kill( iVictim, iKiller, iWeapon, iHeadshot )
{
	new szWpnName[64], iRet = 0;
	UTIL_GetWeaponName( iWeapon, szWpnName, 63 );
	ExecuteForward( fwd_ReportKill, iRet, iKiller, iVictim, iWeapon, szWpnName );

	// Save stats information?
	if ( get_pcvar_num( CVAR_wc3_psychostats ) )
	{
		if ( CSW_WAR3_MIN <= iWeapon <= CSW_WAR3_MAX )
		{
			new iSkillWeapon = iWeapon - CSW_WAR3_MIN;
			
			if ( SHARED_ValidPlayer( iKiller ) )
			{

				// Team kill
				if ( get_user_team( iVictim ) == get_user_team( iKiller ) )
				{
					iStatsTKS[iKiller][iSkillWeapon]++;
				}

				// Random chance so some skills that aren't headshots have a chance
				if ( ( iHeadshot || random_num( 0, 100 ) < 30 ) )
				{
					iStatsHS[iKiller][iSkillWeapon]++;
				}

				iStatsKills[iKiller][iSkillWeapon]++;
			}

			iStatsDeaths[iKiller][iSkillWeapon]++;
		}
	}

	// Remove all ultimate icons since the user is going to be killed...
	ULT_ClearIcons( iVictim );

	new iVictimTeam = get_user_team( iVictim );
	new iKillerTeam = get_user_team( iKiller );

	// Create Death Message
	if ( is_user_alive( iVictim ) )
	{
		// Kill Victim
		WC3_KillUser( iVictim, iKiller, iWeapon );

		// Update frags ( realtime )
		new iVictimFrags = get_user_frags( iVictim );

		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			new iVictimDeaths = get_user_deaths( iVictim );

			Create_ScoreInfo( iVictim, iVictimFrags, iVictimDeaths, 0, iVictimTeam );

			// Get the weapon name
			new szWeaponName[256];
			UTIL_GetWeaponName( iWeapon, szWeaponName, charsmax(szWeaponName) );

			Create_DeathMsg_CS( iKiller, iVictim, iHeadshot, szWeaponName );
		}
	}

	// Call all war3 functions when the user dies
 	WC3_Death( iVictim, iKiller, iWeapon, iHeadshot );

	// Award $300 for a Kill
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Make sure they're not on the same team
		if ( iVictimTeam != iKillerTeam && iKiller != iVictim )
		{
			SHARED_SetUserMoney( iKiller, SHARED_GetUserMoney( iKiller ) + 300, 1 );
		}
	}

	// Get the killer's frags
	new iKillerFrags = get_user_frags( iKiller ) + 1;

	// Team Kill
	if ( iVictimTeam == iKillerTeam && iKiller != iVictim )
	{
		// Remove one from the killer's frags

		iKillerFrags = get_user_frags( iKiller ) - 2; // (-2 since when we init'd it was +1)

		set_user_frags( iKiller, iKillerFrags );
	}

	// Update frags ( realtime )
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( is_user_connected( iKiller ) )
		{
			new iKillerDeaths = get_user_deaths( iKiller );
			
			// If we don't do this then next round the number of kills will go back to what it was
			set_user_frags( iKiller, iKillerFrags );

			// Updates realtime
			Create_ScoreInfo( iKiller, iKillerFrags, iKillerDeaths, 0, iKillerTeam );
		}
	}

	// Log it so psychostats can pick the death up...
	UTIL_LogDeath( iVictim, iKiller, iWeapon );

	return;
}

public WC3_KillUser( iVictim, iKiller, iWeapon )
{
	set_msg_block(gmsgDeathMsg, BLOCK_ONCE );

	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		user_kill( iVictim, 1 );
	}

//	set_msg_block(get_user_msgid("DeathMsg"), BLOCK_SET);
//	user_kill(iVictim, 1);
//	set_msg_block(get_user_msgid("DeathMsg"), BLOCK_NOT);
}

// Funtion will check a file to see if the mapname exists
bool:WC3_MapDisableCheck( szFileName[] )
{
	// Format the Orc Nade Disable File
	new szFile[128];
	get_configsdir( szFile, 127 );
	formatex( szFile, 127, "%s/war3ft/disable/%s", szFile, szFileName );

	if ( !file_exists( szFile ) )
		return false;

	new iLineNum, szData[64], iTextLen, iLen;
	new szMapName[64], szRestrictName[64];
	get_mapname( szMapName, 63 );

	while ( read_file( szFile, iLineNum, szData, 63, iTextLen ) )
	{
		iLen = copyc( szRestrictName, 63, szData, '*' );

		if ( equali( szMapName, szRestrictName, iLen ) )
		{
			return true;
		}

		iLineNum++;
	}

	return false;
}


// This function can be called at ANY time to configure and/or remove skills for a given race
WC3_InitPlayerSkills( idUser )
{
	
	// Undead's Unholy Aura
	SHARED_SetSpeed( idUser );

	// Undead's Levitation
	SHARED_SetGravity( idUser );
	
	// Human's Invisibility
	SHARED_INVIS_Set( idUser );
	
	// Human's Health
    HU_DevotionAura( idUser );

	// Blood Mage's Phoenix
	BM_PhoenixCheck( idUser );

	// Shadow Hunter's Healing Wave
	SH_HealingWave( idUser );

	// Shadow Hunter's Serpent Ward
	SH_SerpentWard( idUser );

	// Warden's Blink
	WA_Blink( idUser );

	// Stop all ultimates if the user shouldn't have an ultimate!
	new iUltimateID = SM_GetSkillOfType( idUser, SKILL_TYPE_ULTIMATE );
	if ( iUltimateID > -1 && SM_GetSkillLevel( idUser, iUltimateID, 9 ) <= 0 )
	{
		ULT_Reset( idUser );
	}

	//Считывание файла users_vips.ini при ициализации
	fInitVipsUsersSkills();
}

// Function called right before the user spawns - ONLY ON NEW ROUND!
WC3_BeforeSpawn( idUser )
{
	
	// Remove the spectating info if necessary
	if ( g_fLastSpecDisplay[idUser] >= halflife_time() )
	{
		UTIL_ClearHudChannel( idUser, HUD_SPEC_INFO );
	}

	//Обнуления заморозки скилов
	p_data_b[idUser][PB_ISFROZENSKILL]		= false;

	//Обнуления заморозки игрока от Ледяной стрелы
	p_data_b[idUser][PB_ISFROZENPLAYER]		= false;

	// Reset who the user was spectating
	g_iSpectatingID[idUser] = -1;

	// Reset the player's role
	g_iPlayerRole[idUser] = 0;
	
	// User shouldn't be a mole anymore...
	p_data_b[idUser][PB_MOLE] = false;

	// Reset the bomb/defusing check
	bHasBegunPlantingOrDefusing[idUser] = false;

	// Reset all ultimates
	ULT_Reset( idUser );

	// Reset certain player variables
	p_data_b[idUser][PB_HAS_SPAWNED]		= false;
	p_data[idUser][P_RESPAWNBY]				= 0;

	// Remove any reset_maxspeeds occuring (could cause a person to move during freezetime)
	task_exists( TASK_RESETSPEED + idUser ) ? remove_task( TASK_RESETSPEED + idUser ) : 0;
	
	// Save a copy of what weapons the user had the previous round (for weapon reincarnation)
	SHARED_CopySavedWeapons( idUser );

	// Remove any serpant wards
	( task_exists( TASK_LIGHT + idUser ) ) ? remove_task( TASK_LIGHT + idUser ) : 0;
}

// This is called at the START of a user spawning
WC3_PreSpawn( idUser )
{
	// Reset human's devotion aura - this needs to be done here
	//  - If we don't do it here, then a user can get > 145 health... i.e. 190
	g_HU_DevotionAuraGiven[idUser]	= 0;


}

// This is called at the end of someone spawning - i.e. NewSession called before this
WC3_PostSpawn( idUser )
{
	// These things need to be reset when the user spawns
	WC3_ResetOnSpawn( idUser );

	// Do we need to give the user a gravity boost?
	SHARED_SetGravity( idUser );

	// Set the user's speed!
	SHARED_SetSpeed( idUser );

	// Give the user their item bonuses!
	ITEM_GiveAllBonuses( idUser );

	//Для talisman
	fPostSpawnTalisman(idUser);

	//Для авто закупки предметов
	if(bAutoBuyOnOff[idUser] == true)
		fBuyAllItems(idUser);

	//Остановка работы предмета Спокойствие
	sm4_4_9_fRemoveSpellCasting( idUser );
	remove_task( TASK_TRANQUILITY + idUser );

	//Добавлен SH_HealingWave если CSDM активен
	if (( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 ))
		SH_HealingWave( idUser );

	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		p_data[idUser][P_HECOUNT]		= 0;
		
		// If we need to give the user their weapons back, then lets
		SHARED_CS_Reincarnation( idUser );

		// If the user's ultimate is ready, lets show their icon!
		ULT_IconHandler( idUser );
	}
	
	// Check for bomb on the back of chameleon
	if ( p_data_b[idUser][PB_SKINSWITCHED] )
		entity_set_int(idUser, EV_INT_body, 0);


	// If the user is a bot they should have a chance to buy an item
	if ( is_user_bot( idUser ) )
	{
		new Float:fBotChance = get_pcvar_float( CVAR_wc3_bot_buy_item );

		if ( fBotChance > 0 && random_float(0.01, 1.0 ) <= fBotChance )
		{
			( random_num( 1, 2 ) == 1 ) ? _MENU_Shopmenu1( idUser, random_num( 0, 8 ) ) : _MENU_Shopmenu2( idUser, random_num( 0, 8 ) );
		}
	}

	// Find out if they need to choose a race or select a skill
	set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + idUser );

	// Set this last - otherwise some skills might be screwed up
	p_data_b[idUser][PB_DIEDLASTROUND]	= false;

	//if (( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 ))
	//	WC3_NewSession( idUser );
}

// Configure this player
//   - this should be called whenever a "new round" begins, for example:
//		CSDM - whenever anyone spawns
//		Normal CS/CZ - @ start of new round
WC3_NewSession( idUser )
{
	// If it's a bot, should we change the race?
	if ( is_user_bot( idUser ) )
	{
		// Give the bot some random XP if we're saving XP
		if ( get_pcvar_num( CVAR_wc3_save_xp ) && !p_data[idUser][P_XP] )
		{
			p_data[idUser][P_XP] = XP_GetByLevel( floatround(random_float(0.01,3.16)*random_float(0.01,3.16)) );
		}

		// Set the bot's race?
		if ( random_float(0.01, 1.0 ) <= BOT_CHANGERACE || !p_data[idUser][P_RACE] )
		{
//			p_data[idUser][P_RACE] = random_num( 1, get_pcvar_num( CVAR_wc3_races ) );
			p_data[idUser][P_RACE] = 3;

			// Now lets set the bot's race!
			WC3_SetRace( idUser, p_data[idUser][P_RACE] );
		}
	}

	// Reset Assist XP handler!
	for ( new i = 0; i < MAXPLAYERS; i++ )
	{
		g_iDamageDealt[idUser][i] = 0;
	}

	// Ultimate cooldown on session start!
	//  - CSDM - Do nothing, let it be their default ultimate delay
	//  - CS/CZ - Set delay to be equal to global, a global countdown will occur @ round start
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// CSDM isn't running!
		if ( ! ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 ) )
		{
			// Copy the global ULT timeout over to just this user...
			fSetUltimateDelay(idUser);
			
		}
	}
	
	// Should we mole b/c of an item?
	SHARED_MoleCheck( idUser, true );			// Only check item!

	// Reset some shizit!
	WC3_ResetOnNewSession( idUser );

	new bool:bChangedRace = false;

	// User has a race selection pending, set it
	//	- Changing race should get priority over reseting skills!
	if ( p_data[idUser][P_CHANGERACE] )
	{
		WC3_SetRace( idUser, p_data[idUser][P_CHANGERACE] );
		
		bChangedRace = true;

		if ( p_data_b[idUser][PB_RESETSKILLS] )
		{
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_PB_RESETSKILLS");	
			
			cssbColoredPrint(idUser, szMessage);	
			
			p_data_b[idUser][PB_RESETSKILLS] = false;
		}
	}

	// Reset user skills if we need to
	if ( WC3_ResetSkills( idUser ) )
	{
		// We need to return here b/c we don't want to set everyones' abilities!
		return;
	}

	// Should we mole b/c of an ability?
	SHARED_MoleCheck( idUser );			// Check skill
	
	// If we made it this far we need to configure the user's skills!
	// Set the user's skills!
	if ( !bChangedRace )
	{
		// This is called when we set a player's race - so we don't need to call it if their race was changed!
		WC3_InitPlayerSkills( idUser );
	}
}

WC3_ResetOnNewSession( idUser )
{
	// Shadow Hunter's Serpent Wards
	p_data[idUser][P_SERPENTCOUNT]	= 0;
	g_SH_SerpentGiven[idUser]		= 0;

	// Warden's shouldn't default being immune
	p_data_b[idUser][PB_WARDENBLINK] = false;
}

WC3_ResetOnSpawn( idUser )
{
	// Human should gain health when he spawns right?
	HU_DevotionAura( idUser );

	// User isn't changing a team if they just spawned
	p_data_b[idUser][PB_CHANGINGTEAM]	= false;
	
	// Reset suicide attempt
	p_data_b[idUser][PB_SUICIDEATTEMPT] = false;
	
	// User should not be burning
	p_data_b[idUser][PB_ISBURNING]		= false;
	p_data[idUser][P_FLAMECOUNT] = 0;

	// The user should not be frozen when they spawn
	SHARED_ResetMaxSpeed( idUser );

	// Reset the user's skin
	SHARED_ChangeSkin( idUser, SKIN_RESET );

	// User won't be zoomed when they spawn!
	g_bPlayerZoomed[idUser]				= false;
}

// Called when a player first joins the server! - we need to reset everything!
WC3_PlayerInit( idUser )
{
//	log_amx("WC3_PlayerInit( %d )",idUser);

	// Reset all skill information for this player!
	SM_ResetSkillLevels( idUser );
	SM_ResetSkills( idUser );

	// This player shouldn't have a race/level or xp yet! - I mean they did just join duh!
	p_data[idUser][P_RACE]					= 0;
	p_data[idUser][P_LEVEL]					= 0;
	p_data[idUser][P_XP]					= 0;

	g_SH_SerpentGiven[idUser]			= 0;			// Shadow Hunter's Serpent Ward - Amount given
	p_data[idUser][P_SERPENTCOUNT]		= 0;			// Shadow Hunter's Serpent Ward - Total the user currently has

	p_data_b[idUser][PB_WARDENBLINK]	= false;		// Warden's Blink - Shouldn't start off immune right?  duh!

	g_bLevitation[idUser]				= true;			// By default a user's levitation should be enabled!


	g_iSpectatingID[idUser] = -1;						// Reset who the user was spectating

	g_iPlayerRole[idUser] = 0;							// Reset the player's role
	
	p_data_b[idUser][PB_MOLE] = false;					// User shouldn't be a mole anymore...

	bHasBegunPlantingOrDefusing[idUser] = false;		// Reset the bomb/defusing check

	p_data_b[idUser][PB_HAS_SPAWNED]	= false;		// Has the user spawned?  nope
	
	p_data[idUser][P_RESPAWNBY]			= 0;			// What did they respawn by?  Nothing I hope the user just joined!

	bIgnorePlayerSpawning[idUser]		= false;		// We don't want to ignore when the player spawns do we?

	g_ULT_iLastIconShown[idUser]		= 0;			// User hasn't shown an icon yet!


	// Misc Item shizit
	g_bPlayerBoughtAnkh[idUser]			= false;		// User didn't buy an ankh!
	g_bPlayerBoughtMole[idUser]			= false;		// User didn't buy mole!

	p_data_b[idUser][PB_LIGHTNINGHIT]	= false;		// User wasn't hit by lightning!  They just joined!

	p_data_b[idUser][PB_NO_DAMAGE]		= false;		// User should be damaged

	g_iDBPlayerUniqueID[idUser]			= 0;			// Reset the user's UniqueID

	for ( new i = 0; i < MAXPLAYERS; i++ )
	{
		g_iDamageDealt[idUser][i]		= 0;			// Reset damage dealt
	}

	p_data[idUser][P_CHANGERACE]		= 0;			// User shouldn't have a changerace pending

	bDBXPRetrieved[idUser]				= false;		// User hasn't gotten his/her XP since he's being reset right?

	p_data_b[idUser][PB_BIGBAD_ATTACKER]	= false;	// User isn't allowed to attack someone running it duh!

	// Reset charges!
	g_iTotalRings[idUser]				= 0;
	g_iNecklaceCharges[idUser]			= 0;
	g_iHelmCharges[idUser]				= 0;

	//Для shopmenu3
	arriTotalRingsGrenadeProtection[idUser] = 0;		// Количество колец защиты от гранат
	arrDeathTouch[idUser] = false;						// Прикосновение смерти

	//Для shopmenu4
	arriTotalRingsDevourerLight[idUser] = 0;			// Массив зарядов пожирателя змей
	arriTotalChargeMirrorUltimate[idUser] = 0;			// Массив зарядов отражателя супер навыка
	arriTotalChargeDeathCoil[idUser] = 0;				// Массив зарядов Лик смерти
	arriTotalChargeImpale[idUser] = 0;					// Массив зарядов Пронзающий Шип
	arriTotalChargeSleep[idUser] = 0;					// Массив зарядов Сонная аура
	g_iPlayerTeam[idUser]				= -1;			// Player doesn't have a team yet! So reset it!

	//Для talisman
	fPlayerInitTalisman(idUser);

	//Для авто закупки предметов
	abi_InitDataAutoBuy(idUser);
}

// Function will simply log to a file as well as amxx log
WC3_Log( bool:bAmxx, const fmt[], ... )
{
	static szFormattedText[512];
	vformat( szFormattedText, 511, fmt, 3 );

	// Write to amxx log file
	if ( bAmxx )
	{
		log_amx( szFormattedText );
	}

	new szLogFile[128];
	get_configsdir( szLogFile, 127 );
	formatex( szLogFile, 127, "%s/war3ft/wc3_error.log", szLogFile );

	// Write to the war3ft log file as well
	log_to_file( szLogFile, szFormattedText );
}