#if defined _jumpstats_const_included
	#endinput
#endif

#define _jumpstats_const_included

#include <hlsdk_const>

stock const PLUGIN_NAME[] =		"JumpStats";
stock const PLUGIN_VERSION[] =		"1.7.2";
stock const PLUGIN_AUTHOR[] =		"Exolent";

#define GRAVITY				800.0

#define SCRIPT_PRESTRAFE		(1 << 0)
#define SCRIPT_AIRSTRAFE		(1 << 1)

#define MAX_STRAFES			20

#define GROUND_FRAMES			20 // minimum frames required for touching the ground before jumping for the jump to be allowed
#define PREDUCK_GROUND_FRAMES		20 // maximum frames allowed to touch the ground before a preduck for countjump is disallowed
#define LADDER_FRAMES			50 // frames required for touching ladder before doing ladder[jump|bhop]

#define MAX_FALL_HEIGHT			200.0 // maximum units allowed for a player to fall for drop [bhop|count]jumps, weirdjumps, and ladderbhops
#define MIN_WJ_FALL_HEIGHT		16.0 // minimum units required for a player to fall for the weirdjump to work

#define EXTRA_DISTANCE			32.0

#define MAX_TOP				10
#define MOTD_INTERVAL			10

#define FL_ONGROUND2			(FL_ONGROUND|FL_PARTIALGROUND|FL_INWATER|FL_CONVEYOR|FL_FLOAT)

#define MAX_BEAM_POINTS			120
#define BEAM_TIME			0.8 // time is in seconds
#define DECAY_BEAM_TIME			0.8 // time is in seconds
#define BEAM_DISTANCE			2.5 // minimum distance between beam points

#define HUD_POS_STATS_X			0.35
#define HUD_POS_STATS_Y			0.4
#define HUD_TIME_STATS			BEAM_TIME + DECAY_BEAM_TIME

#define HUD_POS_STRAFE_X		0.57
#define HUD_POS_STRAFE_Y		0.4
#define HUD_TIME_STRAFE			HUD_TIME_STATS

#define HUD_POS_FALL_X			-1.0
#define HUD_POS_FALL_Y			0.4
#define HUD_TIME_FALL			1.0

#define HUD_POS_PRE_X			-1.0
#define HUD_POS_PRE_Y			0.4
#define HUD_TIME_PRE			0.7

#define HUD_POS_EDGE_X			-1.0
#define HUD_POS_EDGE_Y			-0.875 // wtf SchlumPF ?
#define HUD_TIME_EDGE			HUD_TIME_STATS

#define HUD_POS_SPEEDOMETER_X		-1.0
#define HUD_POS_SPEEDOMETER_Y		0.65
#define SPEEDOMETER_INTERVAL		0.1

#define HUD_FADE_IN			0.1
#define HUD_FADE_OUT			0.1

#define CONFIG_EXEC_DELAY		2.0

#define CACHE_UPDATE_INTERVAL		120.0 // seconds between manual cvar cache (if servertype = 2)

#define HLBSP_EXTRA			0.03125

enum
{
	JUMP_LONG,
	JUMP_HIGH,
	JUMP_WEIRD,
	
	JUMP_BHOP,
	JUMP_STAND_BHOP,
	JUMP_DROPBHOP,
	JUMP_STAND_DROPBHOP,
	
	JUMP_COUNT,
	JUMP_DOUBLECOUNT,
	JUMP_MULTICOUNT,
	JUMP_DROP_COUNT,
	
	JUMP_LADDERJUMP,
	JUMP_LADDERBHOP,
	
	JUMP_TYPES
};

stock const g_jump_names[JUMP_TYPES][] =
{
	"JS_CSSB_LONGJUMP",
	"JS_CSSB_HIGHJUMP",
	"JS_CSSB_WEIRDJUMP",
	
	"JS_CSSB_BHOPJUMP",
	"JS_CSSB_STANDUP_BHOPJUMP",
	"JS_CSSB_DROP_BHOPJUMP",
	"JS_CSSB_STANDUP_DROP_BHOPJUMP",
	
	"JS_CSSB_COUNTJUMP",
	"JS_CSSB_DOUBLE_COUNTJUMP",
	"JS_CSSB_MULTI_COUNTJUMP",
	"JS_CSSB_DROP_COUNTJUMP",
	
	"JS_CSSB_LADDERJUMP",
	"JS_CSSB_LADDERBHO"
};

stock const g_jump_prefixes[JUMP_TYPES][] =
{
	"lj",
	"hj",
	"wj",
	
	"bj",
	"sbj",
	"dbj",
	"sdbj",
	
	"cj",
	"dcj",
	"mcj",
	"drcj",
	
	"laj",
	"lab"
};

stock g_jump_filenames[JUMP_TYPES][64] =
{
	"longjumps",
	"highjumps",
	"weirdjumps",
	
	"bhopjumps",
	"standupbhopjumps",
	"dropbhopjumps",
	"standupdropbhopjumps",
	
	"countjumps",
	"doublecountjumps",
	"multicountjumps",
	"dropcountjumps",
	
	"ladderjumps",
	"ladderbhops"
};

enum BlockSize
{
	BLOCK_MIN,
	BLOCK_MAX
};

stock const g_iBlockSizes[ JUMP_TYPES ][ BlockSize ] =
{
	{	210,	280	},	// LongJump
	{	210,	280	},	// HighJump
	{	210,	280	},	// WeirdJump
	
	{	210,	280	},	// BhopJump
	{	210,	280	},	// Standup BhopJump
	{	210,	280	},	// Drop BhopJump
	{	210,	280	},	// Standup Drop BhopJump
	
	{	210,	280	},	// CountJump
	{	210,	280	},	// Double CountJump
	{	210,	280	},	// Multi CountJump
	{	210,	280	},	// Drop CountJump
	
	{	120,	220	},	// LadderJump
	{	210,	280	}	// LadderBhop
};

enum
{
	SOUND_IMPRESSIVE,
	SOUND_PERFECT,
	SOUND_GODLIKE,
	SOUND_DOMINATING,
	SOUND_DOMINATING_GODLIKE,
	SOUND_HOLYSHIT,
	SOUND_RAMPAGE,
	
	SOUND_TOTAL
};

stock const g_default_sound_files[SOUND_TOTAL][] =
{
	"jumpstats/impressive.wav",
	"jumpstats/perfect.wav",
	"jumpstats/godlike.wav",
	"jumpstats/dominating.wav",
	"jumpstats/dominatingf.wav",
	"jumpstats/holyshit.wav",
	"jumpstats/rampage.wav"
};

stock const g_sound_file_keys[SOUND_TOTAL][] =
{
	"impressive",
	"perfect",
	"godlike",
	"dominating",
	"dominating_godlike",
	"holyshit",
	"rampage"
};

stock const g_sound_file_desc[SOUND_TOTAL][] =
{
	"good jump",
	"pro jump",
	"leet jump",
	"new position in top with a pro jump",
	"new position in top with a leet jump",
	"leet jump with distance that is X units more than the minimum, where X is from js_holyshit_extra",
	"X or more leet jumps in a row, where X is from js_rampage_count"
};

enum
{
	BEAM_PREDUCK,
	BEAM_STAND,
	BEAM_DUCK
};

enum
{
	DIR_FORWARDS,
	DIR_SIDEWAYS,
	DIR_BACKWARDS,
	
	DIR_TOTAL
};



stock const g_direction_names[DIR_TOTAL][] =
{
	"JS_CSSB_FORWARDS",
	"JS_CSSB_SIDEWAYS",
	"JS_CSSB_BACKWARDS"
};

stock const g_direction_motd[DIR_TOTAL][] =
{
	"JS_CSSB_DIRECTION_MOTD_NULL",
	"JS_CSSB_DIRECTION_MOTD_SW",
	"JS_CSSB_DIRECTION_MOTD_BW"
};

enum _:RGB
{
	R,
	G,
	B
};

stock const g_default_beam_colors[][RGB] =
{
	{255, 255, 255}, // white
	{100, 100, 100}, // gray
	{255, 0, 0}, // red
	{255, 127, 0}, // orange
	{255, 255, 0}, // yellow
	{0, 255, 0}, // green
	{0, 127, 255}, // light blue
	{0, 0, 255}, // blue
	{127, 0, 255}, // purple
	{255, 0, 255}, // magenta
	{255, 127, 255} // pink
};

stock const g_weapon_names[][] =
{
	"JS_CSSB_NULL",
	"JS_CSSB_P228",
	"JS_CSSB_SHIELD",
	"JS_CSSB_SCOUT",
	"JS_CSSB_HE_GRENADE",
	"JS_CSSB_AUTO_SHOTGUN_XM1014",
	"JS_CSSB_BOMB_C4",
	"JS_CSSB_MAC_10",
	"JS_CSSB_AUG",
	"JS_CSSB_SMOKE_GRENADE",
	"JS_CSSB_DUALIES_ELITE",
	"JS_CSSB_FIVESEVEN",
	"JS_CSSB_UMP45",
	"JS_CSSB_CT_AUTO_SNIPER_SG550",
	"JS_CSSB_GALIL",
	"JS_CSSB_FAMAS",
	"JS_CSSB_USP",
	"JS_CSSB_GLOCK",
	"JS_CSSB_AWP",
	"JS_CSSB_MP5",
	"JS_CSSB_PARA_M249",
	"JS_CSSB_SHOTGUN_M3",
	"JS_CSSB_M4A1",
	"JS_CSSB_TMP",
	"JS_CSSB_T_AUTO_SNIPER_G3SG1", // G3SG1
	"JS_CSSB_FLASHBANG",
	"JS_CSSB_DEAGLE",
	"JS_CSSB_SG552",
	"JS_CSSB_AK47",
	"JS_CSSB_KNIFE",
	"JS_CSSB_P90"
};
