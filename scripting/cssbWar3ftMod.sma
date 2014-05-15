//Warcraft 3 Frozen Throne by Geesu http://www.war3ft.com
//Авторы мода Geesu, Avanderik & YamiKaitou (версия 3.0 RC13)
//Внес изменения SMaster
//Официальный сайт perfect-soft.su

//Добавлено выпадение предметов
//Добавлена статистика которая показывает победителя в конце раунда
//Изменено отображение которое показывает свой или чужой
//Добавлено вывод в чат информации о киллере
//Добавлена привязка по IP
//Добавлено удаление через определенное время выпавших предметов с земли
//---2.3---
//Добавлена новая раса Ледяной маг
//Добавлен супер навык для расы Хамелеон
//Добавлена подсветка скилов
//---2.4---
//Для расы Хамелеон добавлена возможноть выставлять скилы для VIP на выбор
//---2.5---
//При подборе предмета сообщения заменены на dHud
//---2.6---
//Добавлены квары цен для всех предметов
//---2.7---
//Добавлен запрет установки "Ядовитых змей" на респе врага
//---2.8---
//Изменено отображение цен в магазинах (если не хватает денег отображает не активным)
//---2.9---
//Добавлено shopmenu4
//---3.0---
//Добавлена система талисманов,которые работают как пассивный навык

#pragma tabsize			0
//#pragma semicolon		1

new const WC3NAME[]		=	"Warcraft 3 Frozen Throne";
new const WC3AUTHOR[]	=	"CSSB";
new const WC3VERSION[]	=	"CSSB 3.0.1 Private";
new const WC3DATE[]		=	__DATE__;

//Подключение файлов BEGIN }
// Let AMX X know that we NEED these modules (as of amx x 1.75)
#pragma reqclass	xstats
#pragma reqlib		engine
#pragma reqlib		fun
#pragma reqlib		fakemeta
#pragma reqlib		cstrike
#pragma loadlib		sqlite
#pragma loadlib		mysql

#include <amxmodx>
#include <amxmisc>
#include <engine>
#include <fun>
#include <fakemeta>
#include <cstrike>
#include <dbi>
#include <sqlx>
#include <hamsandwich>
#include <dhudmessage>
#include <csstats>
#include <xs>

#include "war3ft/skills.inl"
#include "war3ft/base.h"

#include <cssb/messages.inc>

// Header files that contain function declarations and variables
#include "war3ft/constants.inl"
#include "war3ft/db/db_common.h"
#include "war3ft/db/db_mysqlx.h"
#include "war3ft/db/db_sqlite.h"
#include "war3ft/items.h"
#include "war3ft/XP.h"
#include "war3ft/war3ft.h"
#include "war3ft/cstrike.h"
#include "war3ft/menus.h"
#include "war3ft/shared.h"
#include "war3ft/ultimates.h"
#include "war3ft/loot.h"
#include "war3ft/showwin.h"
#include "war3ft/infokiller.h"
#include "war3ft/item_esp.h"
#include "war3ft/item_death_touch.h"
#include "war3ft/item_health_potion.h"
#include "war3ft/ultimate_timer_divider.h"
#include "war3ft/item_steel_skin.h"
#include "war3ft/race_frost_mag.h"
#include "war3ft/vip_skills.h"
#include "war3ft/shopmenu4.h"
#include "war3ft/item_ice_block.h"
#include "war3ft/item_death_coil.h"
#include "war3ft/item_impale.h"
#include "war3ft/item_sleep.h"
#include "war3ft/item_tranquility.h"
#include "war3ft/system_talisman.h"
#include "war3ft/autobuy_items.h"

// Source Code
#include "war3ft/cvar.inl"

#include "war3ft/race_undead.inl"           // Undead Scourge   - 1
#include "war3ft/race_human.inl"			// Human Alliance	- 2
#include "war3ft/race_orc.inl"				// Orcish Horde		- 3
#include "war3ft/race_elf.inl"				// Night Elf		- 4
#include "war3ft/race_blood.inl"			// Blood Mage		- 5
#include "war3ft/race_shadow.inl"			// Shadow Hunter	- 6
#include "war3ft/race_warden.inl"           // Warden           - 7
#include "war3ft/race_crypt.inl"			// Crypt Lord       - 8
#include "war3ft/race_chameleon.inl"		// Chameleon		- 9
#include "war3ft/race_frost_mag.inl"		// Frost Mage		- 10


#include "war3ft/forwards.inl"
#include "war3ft/effects.inl"
#include "war3ft/XP.inl"
#include "war3ft/db/db_mysqlx.inl"
#include "war3ft/db/db_sqlite.inl"
#include "war3ft/db/db_common.inl"
#include "war3ft/war3ft.inl"
#include "war3ft/events.inl"
#include "war3ft/clientCommands.inl"
#include "war3ft/items.inl"
#include "war3ft/stocks.inl"
#include "war3ft/shared.inl"
#include "war3ft/menus.inl"
#include "war3ft/motd.inl"
#include "war3ft/language.inl"
#include "war3ft/admin.inl"
#include "war3ft/ultimates.inl"
#include "war3ft/util.inl"
#include "war3ft/skill_manager.inl"
#include "war3ft/cstrike.inl"
#include "war3ft/loot.inl"
#include "war3ft/showwin.inl"
#include "war3ft/infokiller.inl"
#include "war3ft/item_esp.inl"
#include "war3ft/item_death_touch.inl"
#include "war3ft/item_health_potion.inl"
#include "war3ft/ultimate_timer_divider.inl"
#include "war3ft/item_steel_skin.inl"
#include "war3ft/ult_race_blood.inl"
#include "war3ft/vip_skills.inl"
#include "war3ft/shopmenu4.inl"
#include "war3ft/item_ice_block.inl"
#include "war3ft/item_death_coil.inl"
#include "war3ft/item_impale.inl"
#include "war3ft/item_sleep.inl"
#include "war3ft/race_vip_models.inl"
#include "war3ft/item_tranquility.inl"
#include "war3ft/system_talisman.inl"
#include "war3ft/autobuy_items.inl"

//Подключение файлов END }

public plugin_init()
{
	register_plugin( WC3NAME, WC3VERSION, WC3AUTHOR );

	WC3_DetermineGame();

	gmsgDeathMsg		= get_user_msgid( "DeathMsg"	);
	gmsgScreenFade		= get_user_msgid( "ScreenFade"	);
	gmsgScreenShake		= get_user_msgid( "ScreenShake"	);
	gmsgScoreInfo		= get_user_msgid( "ScoreInfo"	);
	
	register_clcmd( "autobuyitems"		, "CMD_Handler"		, -1 );//Для авто закупки предметов
	register_clcmd( "talisman"			, "CMD_Handler"		, -1 );
	register_concmd( "playerskills"		, "CMD_Handler"		, -1 );

	register_clcmd( "debug"				, "CMD_Handler"		, -1 );
	register_clcmd( "levitation"		, "CMD_Handler"		, -1 );
	register_clcmd( "war3menu"			, "CMD_Handler"		, -1 );
	register_clcmd( "changerace"		, "CMD_Handler"		, -1 );
	register_clcmd( "selectskill"		, "CMD_Handler"		, -1 );
	register_clcmd( "skillsinfo"		, "CMD_Handler"		, -1 );
	register_clcmd( "resetskills"		, "CMD_Handler"		, -1 );
	register_clcmd( "resetxp"			, "CMD_Handler"		, -1 );
	register_clcmd( "itemsinfo"			, "CMD_Handler"		, -1 );
	register_clcmd( "itemsinfo2"		, "CMD_Handler"		, -1 );
	register_clcmd( "itemsinfo3"		, "CMD_Handler"		, -1 );
	register_clcmd( "itemsinfo4"		, "CMD_Handler"		, -1 );
	register_clcmd( "shopmenu"			, "CMD_Handler"		, -1 );
	register_clcmd( "shopmenu2"			, "CMD_Handler"		, -1 );
	register_clcmd( "shopmenu3"			, "CMD_Handler"		, -1 );
	register_clcmd( "shopmenu4"			, "CMD_Handler"		, -1 );
	register_clcmd( "ability"			, "CMD_Handler"		, -1 );
	register_clcmd( "ankh"				, "CMD_Handler"		, -1 );
	register_clcmd( "amulet"			, "CMD_Handler"		, -1 );
	register_clcmd( "boots"				, "CMD_Handler"		, -1 );
	register_clcmd( "claws"				, "CMD_Handler"		, -1 );
	register_clcmd( "cloak"				, "CMD_Handler"		, -1 );
	register_clcmd( "chameleon"			, "CMD_Handler"		, -1 );
	register_clcmd( "frost"				, "CMD_Handler"		, -1 );
	register_clcmd( "gloves"			, "CMD_Handler"		, -1 );
	register_clcmd( "health"			, "CMD_Handler"		, -1 );
	register_clcmd( "helm"				, "CMD_Handler"		, -1 );
	register_clcmd( "mask"				, "CMD_Handler"		, -1 );
	register_clcmd( "mole"				, "CMD_Handler"		, -1 );
	register_clcmd( "necklace"			, "CMD_Handler"		, -1 );
	register_clcmd( "rings"				, "CMD_Handler"		, -1 );
	register_clcmd( "socks"				, "CMD_Handler"		, -1 );
	register_clcmd( "scroll"			, "CMD_Handler"		, -1 );
	register_clcmd( "tome"				, "CMD_Handler"		, -1 );
	register_clcmd( "level"				, "CMD_Handler"		, -1 );
	register_clcmd( "say"				, "cmd_Say"			, -1 );
	register_clcmd( "say_team"			, "cmd_Say"			, -1 );
	register_clcmd( "war3help"			, "CMD_Handler"		, -1 );
	register_clcmd( "ultimate"			, "cmd_Ultimate"	, -1 );
	register_clcmd( "fullupdate"		, "cmd_fullupdate"	, -1 );
	register_clcmd( "drop"				, "on_Drop"			, -1 );
	register_clcmd( "jointeam"			, "cmd_Jointeam"	, -1 );

	//Для shopmenu3
	register_clcmd( "brain"				, "CMD_Handler"		, -1 );
	register_clcmd( "nogren"			, "CMD_Handler"		, -1 );
	register_clcmd( "mirror"			, "CMD_Handler"		, -1 );
	register_clcmd( "esp"				, "CMD_Handler"		, -1 );
	register_clcmd( "farmor"			, "CMD_Handler"		, -1 );
	register_clcmd( "deathtouch"		, "CMD_Handler"		, -1 );
	register_clcmd( "hpp"				, "CMD_Handler"		, -1 );
	register_clcmd( "ulttimediv"		, "CMD_Handler"		, -1 );
	register_clcmd( "steelskin"			, "CMD_Handler"		, -1 );

	//Для shopmenu4
	register_clcmd( "devlight"		, "CMD_Handler"		, -1 );
	register_clcmd( "iceblock"		, "CMD_Handler"		, -1 );
	register_clcmd( "thief"			, "CMD_Handler"		, -1 );
	register_clcmd( "mirrorult"		, "CMD_Handler"		, -1 );
	register_clcmd( "deathcoil"		, "CMD_Handler"		, -1 );
	register_clcmd( "impale"		, "CMD_Handler"		, -1 );
	register_clcmd( "sleep"			, "CMD_Handler"		, -1 );
	register_clcmd( "skeletons"		, "CMD_Handler"		, -1 );
	register_clcmd( "tranquility"	, "CMD_Handler"		, -1 );



	// Admin Commands
	register_concmd( "wc3_givexp"		, "ADMIN_Handler"		, -1 , " -- Gives XP to players"				);
	register_concmd( "wc3_setlevel"	    , "ADMIN_Handler"		, -1 , " -- Sets a player's level"				);
	register_concmd( "wc3_giveitem"		, "ADMIN_Handler"		, -1 , " -- Gives an item to a player"			);
	register_concmd( "wc3_enable"		, "ADMIN_Handler"		, -1 , " -- Enables war3ft"						);
	register_concmd( "wc3_disable"		, "ADMIN_Handler"		, -1 , " -- Disables war3ft"					);
	//Для talisman
	register_concmd( "wc3_givetalisman"	, "ADMIN_Handler"		, -1 , " -- Gives an talisman to a player"			);


	// Depreciated Admin Commands
	register_concmd( "amx_givexp"		, "ADMIN_Handler"		, -1 , " -- Gives XP to players"				);

	// Server Admin Commands (used by external plugins)
	register_srvcmd( "amx_takexp"		, "ADMIN_ServerHandler"	);
	register_srvcmd( "changexp"			, "ADMIN_ServerHandler"	);

	//FM Форварды
	register_forward( FM_TraceLine, "TRIGGER_TraceLine");
	register_forward(FM_Touch,"player_Touch");
	register_forward(FM_SetModel,"fwSetModel",1); //sm4
	register_forward(FM_PlayerPostThink, "fwPlayerPostThink"); //sm4
	register_forward(FM_ClientUserInfoChanged, "fwClientUserInfoChanged"); //sm4

	//HAM Форварды
	RegisterHam( Ham_TakeDamage, "player", "EVENT_TakeDamage" );
	RegisterHam( Ham_Spawn, "player", "EVENT_Spawn", 1);

	RegisterHam(Ham_Killed, "player", "fwHamKilled");
	RegisterHam(Ham_CS_RoundRespawn, "player", "fwRoundRespawn", 1);
	RegisterHam(Ham_TraceAttack, "player", "fwTraceAttack", 1);
	RegisterHam(Ham_Player_ResetMaxSpeed,"player","fwHamPlayerResetMaxSpeed",1); //sm4
	RegisterHam(Ham_Think,"grenade","fwHamThinkGrenade",0); //sm4
	RegisterHam(Ham_Use, "player_weaponstrip", "fwHamUsePlayerWeaponstrip", 1); //sm4
	RegisterHam(Ham_Item_Deploy, "weapon_flashbang", "fwHamItemDeployFlashBang", 1); //sm4
	RegisterHam(Ham_Item_Holster, "weapon_flashbang", "fwHamItemHolsterFlashBang", 1); //sm4
	RegisterHam(Ham_Item_AddToPlayer, "weapon_flashbang", "fwHamItemAddToPlayerFlashBang", 1); //sm4
	RegisterHam(Ham_Item_AddDuplicate, "weapon_flashbang", "fwHamItemAddDuplicateFlashBang", 1); //sm4
	RegisterHam( Ham_Player_PreThink,	"player",	"fwHamPlayerPreThink" ); //sm4

	//Touch (Скил 2 Ледяная стрела)
	register_touch("IceArrow","*","touchIceArrow");

	//Touch (Лик смерти)
	register_touch( "DEATH_COIL",   "*", "touchDeathCoil" );//sm4

	//Для talisman
	register_think("war3ft_talisman", "Talisman_Think");
	stSyncHudCreate = CreateHudSyncObj();

	//Сообщения
	register_message(get_user_msgid("DeathMsg"),"msgDeathMessage"); //sm4

	//События
	register_event( "DeathMsg"			, "on_DeathMsg"		, "a"								);
	register_event( "CurWeapon"			, "on_CurWeapon"	, "be"	, "1=1"						);
	register_event( "HideWeapon"		, "on_CurWeapon"	, "b"								);
//	register_event( "ResetHUD"			, "on_ResetHud"		, "be"								);
	register_event( "TextMsg"			, "on_GameRestart"	, "a"	, "2&#Game_will_restart_in" );
	register_event( "HLTV"				, "EVENT_NewRound"	, "a"	, "1=0"	,			"2=0"	);
	register_event( "ResetHUD"			, "evResetHud"		, "b"								); //sm4

	//Create HudSyncObj
	msgSyncObj = CreateHudSyncObj();

	register_dictionary( "cssbWar3ftMod.txt");

	// Game Specific Initialization
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		gmsgBarTime			= get_user_msgid( "BarTime"		);
		gmsgStatusText		= get_user_msgid( "StatusText"	);
		gmsgStatusIcon		= get_user_msgid( "StatusIcon"	);

		register_logevent( "on_PlayerAction"		, 3		, "1=triggered"								);
		register_logevent( "on_FreezeTimeComplete"	, 2		, "0=World triggered"	, "1=Round_Start"	);
		register_logevent( "on_EndRound"			, 2		, "0=World triggered"	, "1=Round_End"		);

		register_event( "SendAudio"		, "on_TerroristWin"		, "a"	, "2=%!MRAD_terwin"					);
		register_event( "SendAudio"		, "on_CTWin"			, "a"	, "2=%!MRAD_ctwin"					);
		register_event( "ArmorType"		, "on_ArmorType"		, "be"										);
		register_event( "Battery"		, "on_Battery"			, "be"										);
		register_event( "WeapPickup"	, "on_WeapPickup"		, "b"										);
		register_event( "StatusValue"	, "on_ShowStatus"		, "be"	, "1=2"		,"2!0"					);
		register_event( "StatusValue"	, "on_HideStatus"		, "be"	, "1=1"		,"2=0"					);
		register_event( "SetFOV"		, "on_Zoom"				, "be"										);
		register_event( "BarTime"		, "on_BombPlanting"		, "be"	, "1=3"								);
		register_event( "BarTime"		, "on_BombStopPlanting"	, "b"	, "1=0"								);

		// Old Style
		register_menucmd( register_menuid( "BuyItem" )	, (1<<3)	, "cmd_hegren"	);

		// VGUI
		register_menucmd( -34	, (1<<3)	, "cmd_hegren"	);

		// Steam
		register_clcmd( "hegren"	, "cmd_hegren"	);

		// Old style menu (now its jointeam client command)
		register_menucmd( register_menuid( "Team_Select" , 1 )	, (1<<0)|(1<<1)|(1<<4)	, "cmd_Teamselect" );

		// Condition Zero
		if ( g_MOD == GAME_CZERO )
		{
			register_event("TextMsg", "on_GameRestart", "a", "2&#Game_Commencing")
		}
		// Counter-Strike
		else
		{
			register_event("TextMsg", "on_GameRestart", "a", "2&#Game_C")
		}
	}

	// Plugin initialization procedures
	WC3_Init();

	//Инициализация массивов
	//Для выпадающих вещей
	arrIntTotalLootEntity = ArrayCreate(1, 1);
	arrIntOriginsLootX = ArrayCreate(1, 1);
	arrIntOriginsLootY = ArrayCreate(1, 1);
	arrIntOriginsLootZ = ArrayCreate(1, 1);
	arrIntItemVictimShop1 = ArrayCreate(1, 1);	//Массив вещей, которые были у жертвы в первом слоте
	arrIntItemVictimShop2 = ArrayCreate(1, 1);	//Массив вещей, которые были у жертвы во втором слоте
	arrIntItemVictimShop3 = ArrayCreate(1, 1);	//Массив вещей, которые были у жертвы в третьем слоте
	arrIntDrawSpriteItems = ArrayCreate(1, 1);	//Массив спрайтов отрисованных вещей
	arrIntScaleSpriteItems = ArrayCreate(1, 1);	//Массив масштабов спрайтов отрисованных вещей
	arrIntLootRemove = ArrayCreate(1, 1);		//Массив таймеров отсчета удаления предметов

	//Для расы Хамелеон (выставление каждому игроку своих скилов)
	arrStrAuthData = ArrayCreate(64, 1);		//Массив Steam ID и ников
	arrIntWc3ChamSkill1 = ArrayCreate(1, 1);	//Массив скилов 1
	arrIntWc3ChamSkill2 = ArrayCreate(1, 1);	//Массив скилов 2
	arrIntWc3ChamSkill3 = ArrayCreate(1, 1);	//Массив скилов 3
	arrIntWc3ChamUltimate = ArrayCreate(1, 1);	//Массив супер навыков
	arrIntWc3ChamPassive = ArrayCreate(1, 1);	//Массив пассивных навыков
	arrStrFlags = ArrayCreate(64, 1);			//Массив флагов

	//ID игроков которых вызвали скелетами
	arrIntSkeletons = ArrayCreate(1, 1);	//Массив пассивных навыков sm4

}

public plugin_end()
{
	new szCurMap[32];
	get_mapname( szCurMap, 31 );
	WC3_Log( false, "** Map '%s' ended", szCurMap );

	if ( !WC3_Check() )
	{
		return;
	}

	if ( !get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return;
	}

	FWD_Destroy();
	DB_SaveAll( false );
	DB_Prune();
	DB_Close();


	return;
}

public plugin_precache()
{
	WC3_Precache();

	//Новые модели
	format(szModelItems, charsmax(szModelItems), "%s","models/CSSB/Items/box_items.mdl");
	format(szModelArXpHpItems, charsmax(szModelArXpHpItems), "%s","models/CSSB/Items/box_arxphp.mdl");

//	format(szModelItems, charsmax(szModelItems), "%s","models/w_kevlar.mdl");
//	format(szSoundLootItems, charsmax(szSoundLootItems), "%s", "CSSB/loot/loot.wav");
	format(szSpriteItems, charsmax(szSpriteItems), "%s","sprites/loot_items.spr");
	format(szSpriteNoItems, charsmax(szSpriteNoItems), "%s","sprites/icon_supplybox2.spr");
	format(szSpriteItemWhite, charsmax(szSpriteItemWhite), "%s","sprites/white.spr");

	// Precache
	engfunc(EngFunc_PrecacheModel, szModelItems);
	engfunc(EngFunc_PrecacheModel, szModelArXpHpItems);
//	engfunc(EngFunc_PrecacheSound, szSoundLootItems);
	idSpriteItems = engfunc(EngFunc_PrecacheModel,szSpriteItems);
	idSpriteNoItems = engfunc(EngFunc_PrecacheModel,szSpriteNoItems);
	idSpriteItemWhite = engfunc(EngFunc_PrecacheModel,szSpriteItemWhite);

	sm4_PluginPrecache();
}

public client_putinserver( idUser )
{
	if ( !WC3_Check() )
		return;

	// Check for steam ID pending
	static szPlayerID[32];
	get_user_authid( idUser, szPlayerID, 31 );

	// Then the player doesn't have a steam id, lets make them reconnect
	if ( equal(szPlayerID, "STEAM_ID_PENDING") )
	{
		client_cmd( idUser, "reconnect" );
	}

	// Get the user's ID!
	DB_FetchUniqueID( idUser );

	p_data_b[idUser][PB_ISCONNECTED] = true;

	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Check user's cvar if the user isn't a bot and if we're not running a 64-bit server
		if ( !is_user_bot( idUser ) && !is_amd64_server() )
		{
			query_client_cvar( idUser, "cl_minmodels", "_CS_CheckMinModelsValue" );
		}

		if ( g_MOD == GAME_CZERO )
		{
			// Only want to run this if: mod is CZ (zbot not supported), client is a bot,
			// these are CZ bots (bot_quota), and the ham has not been registed yet.
			if ( (pev(idUser, pev_flags) & FL_FAKECLIENT) && get_pcvar_num(CVAR_bot_quota) > 0 && !g_bCZBotRegisterHam )
			{
				// Delay for private data to initialize
				set_task( 0.1, "CZ_BotHookHam", idUser )
			}
		}
	}

	sm4_ClientPutinserver(idUser);

	return;
}

public client_connect( id )
{
	if ( !WC3_Check() )
	{
		return;
	}

	// Initialize this player
	WC3_PlayerInit( id );

	client_cmd( id, "hud_centerid 0" );


	p_data[id][P_RESPAWNBY]				= 0;
	p_data[id][P_SPECMODE]				= 0;

	p_data_b[id][PB_ISBURNING]			= false;
	p_data_b[id][PB_JUSTJOINED]			= true;
	p_data_b[id][PB_CAN_RENDER]			= true;
	p_data_b[id][PB_HAS_SPAWNED]		= false;
	p_data_b[id][PB_SKINSWITCHED]		= false;

	// User should have no items on connect...
	g_iShopMenuItems[id][ITEM_SLOT_ONE]	= ITEM_NONE;
	g_iShopMenuItems[id][ITEM_SLOT_TWO] = ITEM_NONE;
	g_iShopMenuItems[id][ITEM_SLOT_THREE] = ITEM_NONE;

	//Для авто-закупки предметов
	abi_ClientConnect(id);
	
	// Automatically set their XP if it's enabled
	if ( get_pcvar_num( CVAR_wc3_xp_auto_average ) && !get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		new iTotalXP;
		new iNum, i;
		for( i = 1; i <= MAXPLAYERS; i++ )
		{
			if ( p_data[i][P_XP] > 0 )
			{
				iNum++;
				iTotalXP += p_data[i][P_XP];
			}
		}
		if ( iNum > 0 && iTotalXP > 0 )
		{
			p_data[id][P_XP] = iTotalXP/iNum;
		}
	}

	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		p_data[id][P_HECOUNT]		= 0;
		p_data[id][P_FLASHCOUNT]	= 0;
	}

	// Reset the "cheat" variables
	new j;
	for ( j = 0; j < MAX_RACES; j++ )
		g_bGivenLevel10[id][j]	= false;

	return;
}

public client_disconnect( idUser )
{
	if ( !WC3_Check() )
		return;

	// Update the user's timestamps for each race if we're saving XP
	DB_UpdateTimestamp( idUser );

	p_data[idUser][P_SPECMODE]			= 0;
	p_data_b[idUser][PB_ISBURNING]		= false;
	p_data_b[idUser][PB_DIEDLASTROUND]	= false;
	p_data_b[idUser][PB_JUSTJOINED]		= false;
	p_data_b[idUser][PB_ISCONNECTED]	= false;
	bIgnoreArmorSet[idUser]				= false;

	// Reset xp assist
	for ( new i = 0; i < MAXPLAYERS; i++ )
	{
		g_iDamageDealt[idUser][i] = 0;
	}

	// Save the user's XP if we have XP to save
	if ( get_pcvar_num( CVAR_wc3_save_xp ) && !is_user_bot(idUser) && p_data[idUser][P_RACE] && p_data[idUser][P_XP] )
	{
		DB_SaveXP( idUser, true );
	}

	if ( get_pcvar_num( CVAR_wc3_psychostats ) )
	{
		static szWeapon[64];

		static szTeam[16], szName[32], szAuthid[32];
		new iWeap, iUserid = get_user_userid( idUser );

		if ( is_user_connected(idUser) )
		{
			get_user_team( idUser, szTeam, 15 );
		}
		get_user_name(idUser, szName, 31 );
		get_user_authid(idUser, szAuthid, 31 );

		for ( iWeap = CSW_WAR3_MIN; iWeap <=CSW_WAR3_MAX; iWeap++ )
		{
			format( szWeapon, 63, "" );

			switch( iWeap )
			{
				case CSW_LIGHTNING:     LANG_GetSkillName( ULTIMATE_CHAINLIGHTNING	, LANG_SERVER,	szWeapon	, 63, 20 );
				case CSW_SUICIDE:		LANG_GetSkillName( ULTIMATE_SUICIDE			, LANG_SERVER,	szWeapon	, 63, 21 );
				case CSW_IMMOLATE:	    LANG_GetSkillName( ULTIMATE_IMMOLATE		, LANG_SERVER,	szWeapon	, 63, 22 );
				case CSW_LOCUSTS:		LANG_GetSkillName( ULTIMATE_LOCUSTSWARM		, LANG_SERVER,	szWeapon	, 63, 23 );
				case CSW_SERPENTWARD:   LANG_GetSkillName( SKILL_SERPENTWARD		, LANG_SERVER,	szWeapon	, 63, 24 );
				case CSW_SHADOW:		LANG_GetSkillName( SKILL_SHADOWSTRIKE		, LANG_SERVER,	szWeapon	, 63, 25 );
				case CSW_THORNS:		LANG_GetSkillName( SKILL_THORNS				, LANG_SERVER,	szWeapon	, 63, 26 );
				case CSW_CARAPACE:		LANG_GetSkillName( SKILL_SPIKEDCARAPACE		, LANG_SERVER,	szWeapon	, 63, 27 );
				case CSW_CARRION:		LANG_GetSkillName( SKILL_CARRIONBEETLES		, LANG_SERVER,	szWeapon	, 63, 28 );
				case CSW_ORB:			LANG_GetSkillName( PASS_ORB					, LANG_SERVER,	szWeapon	, 63, 29 );
				case CSW_CONCOCTION:	LANG_GetSkillName( PASS_UNSTABLECONCOCTION	, LANG_SERVER,	szWeapon	, 63, 30 );
				case CSW_BLIZZARD:	LANG_GetSkillName( ULTIMATE_FROST_MAGE_BLIZZARD,LANG_SERVER,szWeapon	, 63, 31 );
				case CSW_ICE_SPLINTERS:	LANG_GetSkillName( SKILL_FROST_MAGE_ICE_SPLINTERS,LANG_SERVER,szWeapon	, 63, 31 );
				case CSW_CHAM_FLAME_STRIKE:	LANG_GetSkillName( ULTIMATE_CHAM_FLAME_STRIKE,LANG_SERVER,szWeapon	, 63, 31 );

			}

			replace( szWeapon, 63, " ", "_" );

			new WEAPON = iWeap - CSW_WAR3_MIN;

			if ( iStatsShots[idUser][WEAPON] || iStatsHits[idUser][WEAPON] || iStatsKills[idUser][WEAPON] ||
			iStatsHS[idUser][WEAPON] || iStatsTKS[idUser][WEAPON] || iStatsDamage[idUser][WEAPON] ||
			iStatsDeaths[idUser][WEAPON] || iStatsHead[idUser][WEAPON] ||
			iStatsChest[idUser][WEAPON] || iStatsStomach[idUser][WEAPON] ||
			iStatsLeftArm[idUser][WEAPON] || iStatsRightArm[idUser][WEAPON] ||
			iStatsLeftLeg[idUser][WEAPON] || iStatsRightLeg[idUser][WEAPON] )
			{

				// Counter-Strike/Condition Zero log format
				if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
				{
					log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats^" (weapon ^"%s^") (shots ^"%d^") (hits ^"%d^") \
								(kills ^"%d^") (headshots ^"%d^") (tks ^"%d^") (damage ^"%d^")(deaths ^"%d^")",
								szName, iUserid, szAuthid, szTeam, szWeapon,
								iStatsShots[idUser][WEAPON], iStatsHits[idUser][WEAPON], iStatsKills[idUser][WEAPON], iStatsHS[idUser][WEAPON],
								iStatsTKS[idUser][WEAPON], iStatsDamage[idUser][WEAPON], iStatsDeaths[idUser][WEAPON] );
				}

				log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats2^" (weapon ^"%s^") (head ^"%d^") \
				(chest ^"%d^") (stomach ^"%d^") (leftarm ^"%d^") (rightarm ^"%d^") (leftleg ^"%d^") (rightleg ^"%d^")",
				szName, iUserid, szAuthid, szTeam, szWeapon,
				iStatsHead[idUser][WEAPON], iStatsChest[idUser][WEAPON], iStatsStomach[idUser][WEAPON],
				iStatsLeftArm[idUser][WEAPON], iStatsRightArm[idUser][WEAPON], iStatsLeftLeg[idUser][WEAPON],
				iStatsRightLeg[idUser][WEAPON] );

				iStatsShots[idUser][WEAPON]		= 0;
				iStatsHits[idUser][WEAPON]		= 0;
				iStatsKills[idUser][WEAPON]		= 0;
				iStatsHS[idUser][WEAPON]		= 0;
				iStatsTKS[idUser][WEAPON]		= 0;
				iStatsDamage[idUser][WEAPON]	= 0;
				iStatsDeaths[idUser][WEAPON]	= 0;
				iStatsHead[idUser][WEAPON]		= 0;
				iStatsChest[idUser][WEAPON]		= 0;
				iStatsStomach[idUser][WEAPON]	= 0;
				iStatsLeftArm[idUser][WEAPON]	= 0;
				iStatsRightArm[idUser][WEAPON]	= 0;
				iStatsLeftLeg[idUser][WEAPON]	= 0;
				iStatsRightLeg[idUser][WEAPON]	= 0;
			}
		}
	}

	if ( is_user_connected(idUser) )
		sm4_ClientDisconnect(idUser);
}

public client_PreThink( id )
{
	if ( !WC3_Check() )
	{
		return;
	}

	if ( p_data_b[id][PB_ISCONNECTED] )
	{
		if ( is_user_alive( id ) )
		{

			// Counter-Strike or Condition Zero
			if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
			{

				// This is used so we can't hear the undead's footsteps at level 3
				if ( SM_GetSkillLevel( id, SKILL_UNHOLYAURA ) > 0 && !p_data_b[id][PB_STUNNED] && !p_data_b[id][PB_SLOWED] )
				{
					new Float:vel[3];
					entity_get_vector( id, EV_VEC_velocity, vel );

					// When this is true, the user is walking...  lets hope :P
					if ( vector_length( vel ) < 180.0 )
					{
						entity_set_int( id, EV_INT_flTimeStepSound, 999 );
					}

					// Otherwise if we just set it on the previous tick we need to set it back
					else if ( entity_get_int(id, EV_INT_flTimeStepSound) > 500 )
					{
						entity_set_int( id, EV_INT_flTimeStepSound, 200 );
					}
				}
			}

			// Amulet of the Cat
			if ( ITEM_Has( id, ITEM_AMULET ) > ITEM_NONE )
			{
				entity_set_int( id, EV_INT_flTimeStepSound, 999 );
			}

			//Для talisman
			if(fWorkTalismanPassive(id, arrTalismanActive[id]) == TALISMAN_ITEM_AMULET)
				entity_set_int( id, EV_INT_flTimeStepSound, 999 );
		}

		// User is dead
		else
		{

			// Check to see if spectated player has changed
			new iTarget = entity_get_int( id, EV_INT_iuser2 );

			new Float:fTime = halflife_time();

			// Then we are looking at a new player or the last hud message has disappeared
			if ( g_iSpectatingID[id] != iTarget || g_fLastSpecDisplay[id] <= fTime )
			{

				// We have a valid target!!
				if ( SHARED_ValidPlayer( iTarget ) && iTarget != id )
				{
					WC3_ShowSpecInfo( id, iTarget );

					g_fLastSpecDisplay[id] = fTime + SPEC_HOLDTIME;
				}

				g_iSpectatingID[id] = iTarget;
			}
		}
	}

	return;
}

public plugin_natives()
{
	WC3_DetermineGame();

	set_module_filter( "module_filter" );
	set_native_filter( "native_filter" );
	set_error_filter( "error_filter" );

}

public error_filter( error_code, bool:debugging, message[] )
{
	new szBuffer[256];
	dbg_fmt_error( szBuffer, 255 );

	log_amx( "[ERROR] '%s' '%d' '%d' '%s'", message, error_code, debugging, szBuffer );

	//L 12/27/2006 - 16:48:08: [war3ft.amxx] [ERROR] '[CSTRIKE] Invalid player 5' '10' '1' 'Run time error 10: native error (native "cs_set_user_money")'

	// We want to ignore invalid player messages that are generated by cstrike! - they're annoying + don't actually matter all that much...
	/*if ( error_code == AMX_ERR_NATIVE )
	{
		if ( containi( message, "" ) )
		{
		}
	}*/
}

public module_filter( const module[] )
{
	// We don't need to display a message, we'll display it later if we need to (when the DB is set up if XP Saving is on)
	if ( equal( module, "dbi" ) || equal( module, "sqlx" ) )
	{
		return PLUGIN_HANDLED;
	}
	// Dammit plugin can't load now :/ - technically we should never get here unless the module doesn't exist in the modules folder
	else
	{
		WC3_Log( true, "Please enable the '%s' module in your modules.ini file", module );

		return PLUGIN_CONTINUE;
	}

	return PLUGIN_HANDLED;
}

public native_filter( const name[], index, trap )
{
	if ( !trap )
	{
		return PLUGIN_HANDLED;
	}



	return PLUGIN_CONTINUE;
}