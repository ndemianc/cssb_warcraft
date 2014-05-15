/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Global Defines 
*** Date:          Aug 09, 2007
*** Last Update:   Jan 31, 2007
*
*   World of Warcraft Mod continues in the tradition of UWC3 for CS 1.6, providing
*  a level-based RPG experience within the realm of the CS FPS. Many aspects are
*  borrowed from UWC3 (also by K2mia) including some skills and underlying themes.
*
*  Module: Main
*  Module Description:
*  This module provides the plugin initialization, client connection/disconnection
*  handling and other misc shared routines that are used throughout the mod.
*
*  Licensing Info:
*  You are free to use and modify this plugin to your liking with the following
*  restrictions:
*  a) You cannot altar the header/license/credit info in the sourcecode
*  b) You cannot altar/remove/replace the credits screen defined in the
*     wowmod_info_credits subroutine
*  c) You cannot altar/remove/replace the register_plugin() call in wowmod.sma
*----------------------------------------------------------------------------
*
*  WowMOD is written exclusively for AMX Mod X by K2mia
*  
****************************************************************************/

// **************************************************************************
// BEGIN WowMOD Global Definess
// **************************************************************************

// Defines needed for plugin compat
#define VER "0.1.26"                                        // WowMOD Version
#define MOD "WowMOD"                                        // Mod name for Logging
#define MOD_FULLNAME "World of Warcraft Mod (WowMOD")	      // Mod fullname

#define STEAM_POWERED 1          // Running CS1.6 required for this plugin


// Defines needed for game system
#define MAX_PLAYERS 33           // Max number of players in the game
#define MAX_LEVEL 81             // Max level a player can achieve (+1)
#define MAX_ATTRIBS 200          // Max value for attribs and proficiencies 
#define MAX_TIER 10              // Max tier for equipment
#define MAX_GEAR 400             // Max number of gear items that can be created
#define MAX_GLIST 200            // Max number of gear that is purchasable
#define MAX_INV 100              // Max inventory space for each player
#define MAX_BADGES 200           // Max number of badges that can be created


// Defines for Player Races
#define NUM_RACES 8
#define RACE_NIGHTELF 0
#define RACE_GNOME 1
#define RACE_DWARF 2
#define RACE_HUMAN 3
#define RACE_ORC 4
#define RACE_TAUREN 5
#define RACE_UNDEAD 6
#define RACE_BLOODELF 7


// Defines for the number of classes available for each race
#define RACE_NIGHTELF_NUMCLASSES 4
#define RACE_GNOME_NUMCLASSES 3
#define RACE_DWARF_NUMCLASSES 3
#define RACE_HUMAN_NUMCLASSES 4
#define RACE_ORC_NUMCLASSES 3
#define RACE_TAUREN_NUMCLASSES 2
#define RACE_UNDEAD_NUMCLASSES 4
#define RACE_BLOODELF_NUMCLASSES 5


// Defines for Player Classes
#define NUM_CLASSES 6
#define CLASS_DRUID 0
#define CLASS_HUNTER 1
#define CLASS_MAGE 2
#define CLASS_ROGUE 3
#define CLASS_WARLOCK 4
#define CLASS_PRIEST 5
#define CLASS_NA 10 


// Defines for WowMod Proficiency Types
#define PF_ARCANE 1
#define PF_FIRE 2
#define PF_FROST 3
#define PF_MARKSMAN 4
#define PF_MELEE 5
#define PF_NATURE 6
#define PF_SHADOW 7
#define PF_HOLY 8


// Defines for Gear System - reduce magic numbers
#define TIER1 1
#define TIER2 2
#define TIER3 3
#define TIER4 4
#define TIER5 5
#define TIER6 6
#define TIER7 7
#define TIER8 8
#define TIER9 9
#define TIER10 10


// Equipment class dependency bitfields
#define CB_DRUID 1
#define CB_HUNTER 2
#define CB_MAGE 4
#define CB_ROGUE 8
#define CB_WARLOCK 16
#define CB_PRIEST 32 
#define CB_ALL (CB_DRUID|CB_HUNTER|CB_MAGE|CB_ROGUE|CB_WARLOCK|CB_PRIEST)


// Equipment slot locations (for wearing gear)
#define SLOT_HEAD 1
#define SLOT_CHEST 2
#define SLOT_BACK 3
#define SLOT_LEGS 4
#define SLOT_HANDS 5
#define SLOT_FEET 6
#define SLOT_WRISTS 7
#define SLOT_RING1 8
#define SLOT_RING2 9
#define SLOT_NECK 10


// Equipment Tier level requirements
#define TIER1_LEVEL 4
#define TIER2_LEVEL 8
#define TIER3_LEVEL 12 
#define TIER4_LEVEL 16 
#define TIER5_LEVEL 24 
#define TIER6_LEVEL 32 
#define TIER7_LEVEL 40 
#define TIER8_LEVEL 50 
#define TIER9_LEVEL 60 
#define TIER10_LEVEL 70 


// Defines for the XP earning system
#define XP_PER_LEVEL 500            // Amount to add for levels after 20 (Fast XP)
#define XP_PER_LEVEL_LT 2000        // Amount to add for levels after 20 (Long term XP)
#define XP_PER_LEVEL_BASE 4500      // Base amount, xp from lvl 20  (Fast XP)
#define XP_PER_LEVEL_BASE_LT 21000  // Base amount, xp from lvl 20 (Long term XP)
#define XP_BASE 10                  // Base XP for xp gaining (xpgiven)
#define XP_ADD_LEVEL 10             // Per level xp gain (xpgiven)
#define MANA_ADD_LEVEL 10           // Base mana gain per level


// Defines needed for SQL storage system
#define SQL_SAVEBY_STEAMID 1  // Save SQL data by steamid
#define SQL_SAVEBY_NAME 2  // Save SQL data by name


// Defines needed for HUD system
#define HUD_MODE_SPELLS 1
#define HUD_MODE_DETAILED 2
#define HUD_MODE_BASIC 3


// Miscellaneous Defines - New to WowMOD
#define HEARTBEAT 3.0            // Server heartbeat (in seconds )

#define FASTMODE_START_XP 15500  // If server in Fast-mode, how much starting XP
#define FASTMODE_START_GOLD 10000// If server in Fast-mode, how much starting Gold

#define BODYPART_BODY 2          // Body bodypart returned by get_user_attacker()
#define BODYPART_CHEST 4         // Chest bodypart returned by get_user_attacker()
#define WEAPON_KNIFE 29          // Knife weapon value 

#define OFFSET_SHIELD 510        // Used for checking for shield usage
#define HAS_SHIELD (1<<24)       // Used for checking for shield usage
#define USING_SHIELD (1<<16)     // Used for checking for shield usage

#define R_NORMAL 1               // To toggle verbose or quiet mode
#define R_QUIET 2                // To toggle verbose or quiet mode


// Miscellaneous Defines - Kept from UWC3 for internal routines
#define TE_BEAMPOINTS 0
#define TE_EXPLOSION 3
#define TE_EXPLFLAG_NONE 0
#define TE_SMOKE 5
#define TE_BEAMENTS 8
#define TE_IMPLOSION 14
#define TE_SPRITETRAIL 15
#define TE_SPRITE 17
#define TE_BEAMCYLINDER 21
#define TE_BEAMFOLLOW 22
#define TE_ELIGHT 28
#define TE_PLAYERATTACHMENT 124
#define TE_LARGEFUNNEL 100
#define MAX_NAME_LENGTH 31
#define MAX_VAR_LENGTH 64
#define SPELL_SEARCH_TIME 50
#define AUTOCAST_TIME 8.0        // Autocast task time delay in seconds
#define TELEPORT_RADIUS 50       // For Hearthstone

// Team Numbers for get_user_team(id)
#define SPEC 0
#define TEAM_T 1
#define TEAM_CT 2


// Bomb states
#define BOMB_NOTPLANTED 0
#define BOMB_PLANTED 1
#define BOMB_DEFUSED 2



// Defines for WowMOD Spell system and talents system
#define NUM_SPELLS 60
#define NUM_SPELLS_CLASS 8
#define SPELL_DRUID_BASE_IDX 0
#define SPELL_HUNTER_BASE_IDX 8
#define SPELL_MAGE_BASE_IDX 16 
#define SPELL_ROGUE_BASE_IDX 24 
#define SPELL_WARLOCK_BASE_IDX 32 
#define SPELL_PRIEST_BASE_IDX 40 
#define NUM_TALENTS 10
// Defines for spells indices
#define SP_NA 0                     // Bypass a specific spell needed (for apply_damage() )
#define SP_MANATAP (NUM_SPELLS+1)   // Bypass for search_target since manatap is not a spell
// Druid Spells
#define SP_HEALTOUCH 0
#define SP_WRATH 1
#define SP_REMCURSE 2
#define SP_GIFTWILD 3
#define SP_THORNS 4
#define SP_TRANQ 5
#define SP_BARKSKIN 6
#define SP_ROOTS 7
// Hunter Spells
#define SP_SSTING 0
#define SP_ACHEETAH 1 
#define SP_WINGCLIP 2 
#define SP_APACK 3 
#define SP_AWILD 4 
#define SP_COUNTER 5 
#define SP_AMONKEY 6 
#define SP_AIMED 7 
// Mage Spells
#define SP_SCORCH 0
#define SP_ARCINT 1
#define SP_FROSTBOLT 2
#define SP_MAGEARMOR 3
#define SP_FLAMESTRIKE 4
#define SP_ARCBRIL 5
#define SP_BLIZZARD 6
#define SP_BLINK 7
// Rogue Spells
#define SP_CHEAPSHOT 0
#define SP_SPRINT 1
#define SP_VANISH 2
#define SP_BACKSTAB 3
#define SP_RUPTURE 4
#define SP_EVASION 5
#define SP_GARROTTE 6
#define SP_EVISCERATE 7
// Warlock Spells
#define SP_DRAINLIFE 0
#define SP_CURSEWEAK 1
#define SP_DRAINMANA 2
#define SP_CORRUPTION 3
#define SP_DEMONSKIN 4
#define SP_LIFETAP 5
#define SP_IMMOLATE 6
#define SP_SHADOWBOLT 7
// Priest Spells
#define SP_HEAL 0
#define SP_SMITE 1
#define SP_LESSERHEAL 2
#define SP_PWSHIELD 3
#define SP_SWPAIN 4
#define SP_MANABURN 5
#define SP_PRAYERHEAL 6
#define SP_RESURRECT 7


// Player data indices
#define IDX_GEAR_HEAD 0
#define IDX_GEAR_CHEST 1
#define IDX_GEAR_BACK 2
#define IDX_GEAR_LEGS 3
#define IDX_GEAR_HANDS 4
#define IDX_GEAR_FEET 5
#define IDX_GEAR_WRISTS 6
#define IDX_GEAR_RING1 7
#define IDX_GEAR_RING2 8
#define IDX_GEAR_NECK 9

#define IDX_RACE 0
#define IDX_CLASS1 1
#define IDX_CLASS2 2
#define IDX_XP 3
#define IDX_STR 4
#define IDX_AGI 5
#define IDX_STAM 6
#define IDX_INT 7
#define IDX_SPIR 8
#define IDX_ARMOR 9
#define IDX_MANA 10
#define IDX_GOLD 11
#define IDX_TIER 12
#define IDX_ASSISTS 13
#define IDX_ROGUESPEC 14
#define IDX_REZZES 15
#define IDX_TEAMACTS 16
#define IDX_BADGES 17
#define IDX_SPELL1 18
#define IDX_SPELL60 77
#define IDX_KILLS 78
#define IDX_DEATHS 79
#define IDX_ROUNDS 80
#define IDX_CASTS 81
#define IDX_HEALS 82
#define IDX_BOMBS 83
#define IDX_DEFUSES 84
#define IDX_SUPPORTS 85
#define IDX_MELEE 86
#define IDX_MARKSMAN 87
#define IDX_ARCANE 88
#define IDX_FIRE 89
#define IDX_FROST 90
#define IDX_SHADOW 91
#define IDX_NATURE 92
#define IDX_HOLY 93
#define IDX_BADGES2 94 


// Help System Indices
#define HELP_RACES 0
#define HELP_CLASSES 1
#define HELP_SPELLS 2
#define HELP_RACE_NIGHTELF 3
#define HELP_RACE_GNOME 4
#define HELP_RACE_DWARF 5
#define HELP_RACE_HUMAN 6
#define HELP_RACE_ORC 7
#define HELP_RACE_TAUREN 8
#define HELP_RACE_UNDEAD 9
#define HELP_RACE_BLOODELF 10 
#define HELP_CLASS_DRUID 11 
#define HELP_CLASS_HUNTER 12
#define HELP_CLASS_MAGE 13 
#define HELP_CLASS_ROGUE 14 
#define HELP_CLASS_WARLOCK 15 
#define HELP_CLASS_PRIEST 16 
#define HELP_SPELLS_BASE 17
#define HELP_INVENTORY 65
#define HELP_MERCHANTS 66
#define HELP_ATTRIBUTES 67
#define HELP_PROFICIENCIES 68
#define HELP_RACIALS 69
#define HELP_RACIAL_SMELD 70
#define HELP_RACIAL_ESCAPE 71
#define HELP_RACIAL_STONE 72
#define HELP_RACIAL_PERC 73
#define HELP_RACIAL_BERZ 74
#define HELP_RACIAL_STOMP 75
#define HELP_RACIAL_CANN 76
#define HELP_RACIAL_MANA 77
#define HELP_CRIT 78
#define HELP_SPCRIT 79
#define HELP_SPDMG 80
#define HELP_AUTOCAST 81


// Color defs
#define COLOR_BLUE 1
#define COLOR_RED 2
#define COLOR_GREEN 3
#define COLOR_YELLOW 4
#define COLOR_PURPLE 5
#define COLOR_ORANGE 6
#define COLOR_WHITE 7
#define COLOR_GRAY 8


// Sound defs
#define SOUND_ 0
#define SOUND_SPELLCAST 1
#define SOUND_HEAL 2
#define SOUND_TUPING 3
#define SOUND_LEVELUP 4
#define SOUND_LEVBELL 5
#define SOUND_WRATH 10 
#define SOUND_ROOTS 11 
#define SOUND_BIGGUN 20 
#define SOUND_FLAMESTRIKE 30 
#define SOUND_BLIZZARD 31 
#define SOUND_BLINK 32 
#define SOUND_HEARTHSTONE 33 
#define SOUND_BERZERK 40 
#define SOUND_NA 50


// Glow defines
#define GLOW_OFF 0
#define GLOW_WHITE 1 
#define GLOW_RED 2
#define GLOW_GREEN 3
#define GLOW_BLUE 4
#define GLOW_ORANGE 5
#define GLOW_PURPLE 6
#define GLOW_YELLOW 7
#define GLOW_REDPULSE 20 
#define GLOW_GREENPULSE 21 
#define GLOW_BLUEPULSE 22 
#define GLOW_ORANGEPULSE 23 
#define GLOW_PURPLEPULSE  24 
#define GLOW_YELLOWPULSE 25 
#define GLOW_INVIS 40 
#define GLOW_NORMAL 41 


// Badge identifiers 
#define BADGE_ALL_TIER5_1 4
#define BADGE_ALL_TIER5_2 5
#define BADGE_ALL_TIER5_3 6
#define BADGE_ALL_TIER6_1 7
#define BADGE_ALL_TIER6_2 8
#define BADGE_ALL_TIER6_3 9
#define BADGE_ALL_TIER7_1 10 
#define BADGE_ALL_TIER7_2 11 
#define BADGE_ALL_TIER7_3 12 
#define BADGE_DRUID_TIER8 15 
#define BADGE_DRUID_TIER9 16 
#define BADGE_DRUID_TIER10 17 
#define BADGE_HUNTER_TIER8 18 
#define BADGE_HUNTER_TIER9 19 
#define BADGE_HUNTER_TIER10 20 
#define BADGE_MAGE_TIER8 21 
#define BADGE_MAGE_TIER9 22 
#define BADGE_MAGE_TIER10 23 
#define BADGE_ROGUE_TIER8 24 
#define BADGE_ROGUE_TIER9 25 
#define BADGE_ROGUE_TIER10 26 
#define BADGE_WARLOCK_TIER8 27 
#define BADGE_WARLOCK_TIER9 28 
#define BADGE_WARLOCK_TIER10 29 
#define BADGE_PRIEST_TIER8 30 
#define BADGE_PRIEST_TIER9 31 
#define BADGE_PRIEST_TIER10 32 


// Task identifiers
#define TASK_ACHEETAH 19
#define TASK_SSTING 84 
#define TASK_ARCINT 120 
#define TASK_WEAKNESS 160 
#define TASK_MEDICSPR 200
#define TASK_PDATA_LOOP 240 
#define TASK_HEARTBEAT 280 
#define TASK_GIFTWILD 320 
#define TASK_TRANQ 360 
#define TASK_BARK 400 
#define TASK_ROOTS 440 
#define TASK_SETVARS 480
#define TASK_SQLINIT 520
#define TASK_GEARINIT 560
#define TASK_WINGCLIP 600 
#define TASK_APACK 640 
#define TASK_AWILD 680 
#define TASK_AIMED 720 
#define TASK_FROSTBOLT 760 
#define TASK_MAGEARMOR 800 
#define TASK_BRILLIANCE 840 
#define TASK_BLIZZARD 880 
#define TASK_BLINK 920 
#define TASK_PWSHIELD 960 
#define TASK_SWPAIN 1000 
#define TASK_CORRUPT 1040 
#define TASK_DEMONSKIN 1080 
#define TASK_CHEAPSHOT 1120 
#define TASK_SPRINT 1160 
#define TASK_VANISH 1200 
#define TASK_RUPTURE 1240 
#define TASK_EVASION 1280 
#define TASK_GARROTTE 1320 
#define TASK_VANISHWARN1 1240 
#define TASK_VANISHWARN2 1280 
#define TASK_SPA_SPEED1 5000
#define TASK_HEALCOOL 11940
#define TASK_AUTOCAST 11980
#define TASK_SHADOWWARN1 12020
#define TASK_SHADOWWARN2 12060
#define TASK_RACIAL_WARSTOMP 12100
#define TASK_RACIAL_PERCEPTION 12140
#define TASK_RACIAL_ESCAPE 12180
#define TASK_GLOWOFF 12220
#define TASK_RACIAL 12260
#define TASK_BLINKCHECK 12300
#define TASK_BLINKTEST 12305
#define TASK_SPRCLASS 12345
#define TASK_UDELAYCOOL 12385
#define TASK_MANAHUD 12390 
#define TASK_CHKENABLED_LOOP 12395 
#define TASK_CHKENABLED 12400 
#define TASK_SETXP 12405 
#define TASK_BADGEINIT 12410 
#define TASK_SPECABILINIT 12415 


// Miscellaneous Spcial Ability (from Equipment) Defines
#define SPECABIL_DURATION1 90.0     // Duration (short) for spec abil
#define SPECABIL_SPEED1_SPEED 260.0 // Spell for speed1 spec abil
#define SPECABIL_SPEED2_SPEED 275.0 // Spell for speed2 spec abil
#define SPECABIL_SPEED3_SPEED 290.0 // Spell for speed3 spec abil


// Miscellaneous Ability and Spell-related defs
#define RACIAL_BASE_TIMER 10.0      // Base timer for duration based abilities
#define RACIAL_BASE_TIMER2 360.0    // Base timer for reusable abilities
#define RACIAL_BASE_TIMER3 15.0     // Base timer for manatap racial
#define RACIAL_ORC_RESIST 10        // Orc stun resistance bonus
#define RACIAL_WARSTOMP_SPEED 120.0 // Tauren warstomp runspeed debuff
#define RACIAL_WARSTOMP_DMG 5       // Tauren warstomp dmg inflicted
#define RACIAL_CANNIBAL_HEALTH 5    // Undead cannibalize health regen
#define RACIAL_CANNIBAL_MAX 10      // Undead cannibalize health regen MAX
#define RACIAL_MANATAP_BASE 100     // Bloodelf base mana stolen from manatap
#define ENERGY_FROM_VANISH 10       // Energy regen from Vanish spell
#define START_ENERGY 100            // How much Energy a rogue starts each round with
#define ENERGY_MAX 300              // Max. energy allowed
#define INVIS_MAX 120 
#define BASE_VISIBILITY 240
#define SHADOWMELD_INVIS 160
#define VANISH_INVIS 100
#define ALL_INVIS 80
#define SPELL_RESISTED 500
#define SPELL_MANA_RANK 25
#define SPELL_MANA_RANK2 50
#define SPELL_MANA_RANK3 75
#define SPELL_MANA_RANK4 100 
#define NO_DMG -1
#define NELF_DODGE_CHANCE 10
#define ROGUE_DODGE_CHANCE 1        // Rogue's default dodge chance ( per level )
#define ROGUE_DODGE2_CHANCE 15      // Rogue's dodge chance when holding a knife
#define MONKEY_DODGE_CHANCE 5
#define EVASION_DODGE_CHANCE1 20    // Rogue's evasion-dodge chance when knife wielded
#define EVASION_DODGE_CHANCE2 1     // Rogue's evasion-dodge chance when ranked weapon wielded
#define NATURE_BASE_RESIST 15
#define BELF_BASE_RESIST 10
#define UNDEAD_BASE_RESIST 15
#define HUMAN_BASE_RESIST 15
#define GNOME_BASE_RESIST 15
#define REZ_HEALTH 10 
#define STR_HEALTH_BONUS 5
#define ARMOR_BONUS 5
#define HEALTHBONUS_DEF 15
#define HEALTHBONUS_DEF2 8
#define MAXSPEED_DEF 240.0
#define REMCURSE_CHANCE 5
#define GIFTWILD_ARMOR_BUFF 50
#define GIFTWILD_ATT_BUFF 10
#define DEMONSKIN_HEALTH 2          // Warlock Demon Skin health regen
#define SPEED_SPRINT 340.0
#define SPEED_ACHEETAH_BASE 250.0
#define SPEED_ACHEETAH 5.0
#define SPEED_APACK 5.0
#define SPEED_FROSTBOLT 120.0
#define SSTING_BASE_CHANCE 5
#define SSTING_BASE_TIMER 5.0
#define CHEAPSHOT_BASE_TIMER 5.0
#define CHEAPSHOT_SPEED 120.0
#define WINGCLIP_BASE_TIMER 5.0
#define WINGCLIP_SPEED 120.0
#define BLIZZARD_BASE_TIMER 7.0
#define BLIZZARD_SPEED 90.0
#define SPELL_BASE_TIMER 30.0
#define SPELL_BASE_TIMER2 10.0
#define ROOTS_BASE_TIMER 5.0
#define FROSTBOLT_BASE_TIMER 5.0
#define TRANQ_HEALTH_BONUS 5
#define SWPAIN_BASE_TIMER 7.0
#define BUFF_ARCINT 5
#define BUFF_BRILLIANCE 10
#define BUFF_MAGEARMOR 10
#define CORRUPT_BASE_CHANCE 5
#define CORRUPT_BASE_TIMER 5.0
#define RUPTURE_BASE_TIMER 7.0
#define GARROTTE_BASE_TIMER 6.0
#define DMG_SSTING 1 
#define DMG_RUPTURE 1 
#define DMG_GARROTTE 1 
#define DMG_SWPAIN 2 
#define DMG_SCORCH_BASE 1
#define DMG_FLAMESTRIKE_BASE 1
#define DMG_BLIZZARD_BASE 1
#define DMG_CHEAPSHOT_BASE 1
#define DMG_KIDNEYSHOT_BASE 1
#define DMG_DRAINLIFE_BASE 1
#define DMG_DRAINMANA_BASE 100
#define DMG_MANABURN_BASE 100
#define DMG_SMITE_BASE 1
#define DMG_WRATH_BASE 5
#define DMG_THORNS_BASE 1
#define DMG_AIMED_BASE 25
#define DMG_COUNTER_BASE 1
#define DMG_FROSTBOLT_BASE 1
#define DMG_CORRUPT 3 
#define DMG_IMMOLATE_BASE 15
#define DMG_SHADOWBOLT_BASE 25
#define DMG_BACKSTAB_BASE 45
#define DMG_EVISCERATE_BASE 35
#define AWILD_NATBUFF_BASE 10
#define AWILD_NATBUFF 5
#define FB_LINEWIDTH 20 
#define FLAMESTRIKE_RADIUS 250
#define BLIZZARD_RADIUS 250
#define BLINK_RADIUS 50
#define BLINKDOWN_BASE_TIMER 20.0
#define PWSHIELD_PROT 0.1
#define PWSHIELD_PROT_MAX 0.6
#define DEMONSKIN_PROT 0.1
#define DEMONSKIN_PROT_MAX 0.5
#define HEAL_COOLDOWN_TIMER 1.0


// Defines for special abilities attached to equipment
#define SPA_HSTONE 1
#define SPA_SPEED1 2
#define SPA_SPEED2 3
#define SPA_SPEED3 4
#define SPA_LIGHTNING1 5
#define SPA_LIGHTNING2 6


// **************************************************************************
// END WowMOD Global Defs
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

