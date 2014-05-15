/*
*	XP Header File
*/

// Objective Modifiers
#define DEFUSING_BOMB		100			// XP awarded when the user starts to defuse the bomb
#define DEFUSED_BOMB		200			// XP awarded when the user defuses the bomb
#define PLANTING_BOMB		100			// XP awarded when the user starts planting the bomb
#define PLANT_BOMB			200			// XP awarded when the user plants the bomb
#define SPAWN_BOMB			100			// XP awarded when the user spawns with the bomb
#define BOMB_PICKUP			100			// XP awarded when the user picks up the bomb
#define TOUCH_HOSTAGE		100			// XP awarded when the user touches a hostage
#define RESCUE_HOSTAGE		200			// XP awarded when the user rescues the hostage
#define KILL_HOSTAGE		100			// XP lost when killing a hostage
#define SPAWN_VIP			100			// XP awarded for spawning as the VIP
#define ESCAPE_VIP			200			// XP awarded for escaping as the VIP
#define OBJ_RADIUS			5000			// Nearby radius to award XP for helping complete objectives

// Kill modifiers
#define KILL_HEADSHOT		100			// XP awarded for getting a headshot
#define KILL_HOSTAGE_SAVER	100			// XP awarded for killing the hostage saver
#define	KILL_DEFUSER		100			// XP awarded for killing the defuser
#define KILL_PLANTER		100			// XP awarded for killing the planter
#define KILL_BOMB_CARRIER	100			// XP awarded for killing the bomb carrier
#define KILL_VIP			200			// XP awarded for killing the VIP
#define KILL_RADIUS			2500			// Nearby radius to award XP

#define WIN_ROUND			200			// XP awarded for winning the round


// Holds information about the player
enum
{
	PLR_BOMB_DEFUSER = 1,
	PLR_BOMB_PLANTER,
	PLR_HOSTAGE_RESCUER,
	PLR_VIP,
	PLR_BOMB_CARRIER,
};

new g_iPlayerRole[33];
new bool:bHasBegunPlantingOrDefusing[33];

// Holds the XP Multipliers per weapon
new Float:fWpnXPMultiplier[CSW_WAR3_MAX+1] =		{1.0};
