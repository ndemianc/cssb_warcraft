/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Global Variables 
*** Date:          Aug 09, 2007
*** Last Update:   Feb 02, 2008
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
// BEGIN WowMOD Global Variables 
// **************************************************************************

// I. Variables kept from UWC3

new bool:wowmod_enabled									// Is WowMOD enabled?

new bool:wowmod_shutdown = false                // Has WowMOD been shutdown by an admin

//new bool:buytime                              // Check for buying time in round

new html_head[500]                              // Holds the html head/css section

// Default setting for the weaponxpmultiplier, modified later in plugin
//new Float:weaponxpmultiplier[31] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0}


// Variables needed for XP earning system
new xplevel_base[21] = { 0, 50, 100, 150, 250, 350, 450, 600, 750, 900, 1050, 1250, 1450, 1650, 1950, 2250, 2550, 3000, 3500, 4000, 4500 }
new xplevel_base_LT[21] = { 0, 100, 300, 600, 1000, 1500, 2100, 2800, 3600, 4500, 5500, 6600, 7800, 9100, 10500, 12000, 13600, 15300, 17100, 19000, 21000 }

new xplevel_lev[MAX_LEVEL] = 0 // Table of XP needed to achieve each level
new xpgiven_lev[MAX_LEVEL] = 0 // Table of XP given at each level
new xpgiven                    // Used to store current level of XP given


// XP bonuses, can be overridden with CVARS
new VIP_ESCAPE_BONUS
new VIP_KILL_BONUS
new DEFUSER_KILL_BONUS
new KILL_BOMB_CARRIER
new BOMBPLANTXP
new DEFUSEXP
new HOSTAGEXP
new KILLRESCUEMANXP
new XPBONUS
new HEADSHOT_BONUS
new ROUNDXP
new MENDXP
new PRAYERXP
new REMCURSEXP
new GIFTXP
new TRANQXP
new ASSISTXP
new REZXP


// Sprites - Kept from UWC3
new g_sModelIndexSmoke
new m_iSpriteTexture
new gatespr
new blood1
new blood2
new lightning
new burning
new medicspr
new shieldspr
new medshieldspr
new blast
new blizzard
new frostbolt
new class_druid
new class_hunter
new class_mage
new class_rogue
new class_warlock
new class_priest


// SQL storage variables
new sqlhost[64]
new sqluser[32]
new sqlpass[32]
new sqldb[32]
new sqlerror[128]
new sqltable_players[32] = "players"


// Captured Messages - kept from UWC3
//new gMsgScreenFade
//new gmsgFade
new gmsgStatusText
new gmsgDeathMsg
new gmsgShake


// II. Variables for Race / Class / Spell System / Gear

new races[NUM_RACES][] = { "NightElf", "Gnome", "Dwarf", "Human", "Orc", "Tauren", "Undead", "BloodElf" }
new classes[NUM_CLASSES][] = { "Druid", "Hunter", "Mage", "Rogue", "Warlock", "Priest" }
new racials[NUM_RACES][] = { "Shadow Meld", "Escape Artist", "Stoneform", "Perception", "Berzerk", 
                             "Warstomp", "Cannibalize", "Mana Tap" }

// Classes available for each Race
new nelf_classes[RACE_NIGHTELF_NUMCLASSES][] = { "Druid", "Hunter", "Rogue", "Priest" }
new gnome_classes[RACE_GNOME_NUMCLASSES][] = { "Mage", "Rogue", "Warlock" }
new dwarf_classes[RACE_DWARF_NUMCLASSES][] = { "Hunter", "Rogue", "Priest" }
new human_classes[RACE_HUMAN_NUMCLASSES][] = { "Mage", "Rogue", "Warlock", "Priest" }
new orc_classes[RACE_ORC_NUMCLASSES][] = { "Hunter", "Rogue", "Warlock" }
new tauren_classes[RACE_TAUREN_NUMCLASSES][] = { "Druid", "Hunter" }
new undead_classes[RACE_UNDEAD_NUMCLASSES][] = { "Mage", "Rogue", "Warlock", "Priest" }
new belf_classes[RACE_BLOODELF_NUMCLASSES][] = { "Hunter", "Mage", "Rogue", "Warlock", "Priest" }

// <class>_spells is the array of spell names, right now 8 max per class
new druid_spells[8][] = { "Healing Touch", "Wrath", "Remove Curse", "Gift of the Wild", "Thorns", "Tranquility", "Barkskin", "Entangling Roots" }
// <class>_saycmds is the array of spell spoken commands, if applicable
new druid_saycmds[8][] = { "na", "na", "na", "giftwild", "na", "tranquility", "barkskin", "na" }
// <class>_splvls is the array of level needed to learn each spell
new druid_splvls[8] = { 1, 4, 8, 12, 16, 20, 24, 28 }
// <class>_spgold is the array of gold needed to buy each spell
new druid_spgold[8] = { 50, 100, 200, 300, 400, 500, 600, 700 }
// <class>_spmax is the array of max ranks for each spell
new druid_spmax[8] = { 10, 10, 10, 10, 5, 10, 10, 10 } 
// <class>_spmana is the array of mana base required to cast each spell
new druid_spmana[8] = { 50, 50, 100, 200, 50, 300, 350, 300 } 

new hunter_spells[8][] = { "Serpent Sting", "Aspect of the Cheetah", "Wing Clip", "Aspect of the Pack", "Aspect of the Wild", "Counterattack", "Aspect of the Monkey", "Aimed Shot" }
new hunter_saycmds[8][] = { "na", "na", "na", "pack", "wild", "na", "na", "na" }
new hunter_splvls[8] = { 1, 4, 8, 12, 16, 20, 24, 28 }
new hunter_spgold[8] = { 50, 100, 200, 300, 400, 500, 600, 700 }
new hunter_spmax[8] = { 10, 10, 10, 5, 10, 10, 10, 10 } 
new hunter_spmana[8] = { 10, 40, 100, 200, 250, 50, 350, 300 } 

new mage_spells[8][] = { "Scorch", "Arcane Intellect", "Frostbolt", "Mage Armor", "Flamestrike", "Arcane Brilliance", "Blizzard", "Blink" }
new mage_saycmds[8][] = { "na", "arcint", "na", "magearmor", "na", "brilliance", "na", "na" }
new mage_splvls[8] = { 1, 4, 8, 12, 16, 20, 24, 28 }
new mage_spgold[8] = { 50, 100, 200, 300, 400, 500, 600, 700 }
new mage_spmax[8] = { 10, 10, 10, 5, 10, 10, 10, 10 } 
new mage_spmana[8] = { 10, 40, 100, 200, 250, 300, 350, 400 } 

new rogue_spells[8][] = { "Cheap Shot", "Sprint", "Vanish", "Backstab", "Rupture", "Evasion", "Garrotte", "Eviscerate" }
new rogue_saycmds[8][] = { "na", "sprint", "vanish", "na", "na", "evasion", "na", "na" }
new rogue_splvls[8] = { 1, 4, 8, 12, 16, 20, 24, 28 }
new rogue_spgold[8] = { 50, 100, 200, 300, 400, 500, 600, 700 }
new rogue_spmax[8] = { 10, 20, 5, 3, 10, 10, 10, 5 } 
new rogue_spenergy[8] = { 20, 30, 30, 60, 60, 60, 60, 80 } 

new warlock_spells[8][] = { "Drain Life", "Curse of Weakness", "Drain Mana", "Corruption", "Demon Skin", "Lifetap", "Immolate", "Shadow Bolt" }
new warlock_saycmds[8][] = { "na", "na", "na", "na", "demonskin", "lifetap", "na", "na" }
new warlock_splvls[8] = { 1, 4, 8, 12, 16, 20, 24, 28 }
new warlock_spgold[8] = { 50, 100, 200, 300, 400, 500, 600, 700 }
new warlock_spmax[8] = { 10, 10, 10, 10, 10, 5, 10, 10 } 
new warlock_spmana[8] = { 10, 100, 0, 100, 200, 300, 350, 400 } 

new priest_spells[8][] = { "Heal", "Smite", "Lesser Heal", "Power Word: Shield", "Shadow Word: Pain", "Mana Burn", "Prayer of Healing", "Resurrect" }
new priest_saycmds[8][] = { "na", "na", "na", "shield", "na", "na", "prayer", "na" }
new priest_splvls[8] = { 1, 4, 8, 12, 16, 20, 24, 28 }
new priest_spgold[8] = { 50, 100, 200, 300, 400, 500, 600, 700 }
new priest_spmax[8] = { 10, 10, 5, 10, 10, 10, 10, 10 } 
new priest_spmana[8] = { 30, 50, 0, 100, 300, 100, 600, 800 } 

// Gear data subsection

new tiers[MAX_TIER + 1][] = { "", "Common Level 1", "Common Level 2", "Common Level 3", "Common Level 4",
                              "Epic Level 1", "Epic Level 2", "Epic Level 3", "Epic Level 4", "Epic Level 5",
                              "Epic Level 6" }

new slots[SLOT_NECK + 1][] = { "", "Head", "Chest", "Back", "Legs", "Hands", "Feet", "Wrists",
                               "Ring 1", "Ring 2", "Neck" }

new gear[MAX_GEAR][32] = {             // Gear names
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }

new g_specabil[MAX_GEAR][128] = {             // Special abilities 
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }

new g_slot[MAX_GEAR]                   // Gear slot ( ex hands, feet, head, etc )
new g_classes[MAX_GEAR]                // Gear clases allowed (bitfield)
new g_tier[MAX_GEAR]                   // Gear tier level
new g_cost[MAX_GEAR]                   // Gear price
new g_str[MAX_GEAR]                    // Gear strength attribute mod
new g_agi[MAX_GEAR]                    // Gear agility attribute
new g_stam[MAX_GEAR]                   // Gear stamina attribute
new g_int[MAX_GEAR]                    // Gear int attribute mod 
new g_spir[MAX_GEAR]                   // Gear spirit attribute
new g_armor[MAX_GEAR]                  // Gear armor mod
new g_mana[MAX_GEAR]                   // Gear mana mod
new g_melee[MAX_GEAR]                  // Gear melee proficiency mod
new g_marksman[MAX_GEAR]               // Gear marksmanship mod
new g_arcane[MAX_GEAR]                 // Gear arcane mod
new g_fire[MAX_GEAR]                   // Gear fire mod
new g_frost[MAX_GEAR]                  // Gear frost mod
new g_shadow[MAX_GEAR]                 // Gear shadow mod
new g_nature[MAX_GEAR]                 // Gear nature mod
new g_holy[MAX_GEAR]                   // Gear holy mod
new g_crit[MAX_GEAR]                   // Gear critical strike mod
new g_spcrit[MAX_GEAR]                 // Gear spell-critical strike mod
new g_spdmg[MAX_GEAR]                  // Gear spell damage mod
new g_spec_idx[MAX_GEAR]               // Gear special index

// Badge data Subsection

new badge[MAX_BADGES][32] = {          // Badge names
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }
new b_desc[MAX_BADGES][127] = {          // Badge descriptions
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }
new b_classes[MAX_BADGES]              // Badge clases allowed (bitfield)
new b_xp[MAX_BADGES]                   // Badge xp awarded for earning badge
new b_gold[MAX_BADGES]                 // Badge gold awarded for earning badge 
new b_bit[MAX_BADGES]                  // Badge bitfield bit 
new b_kills[MAX_BADGES]                // Badge kills total requirement
new b_kills_s[MAX_BADGES]              // Badge kills session requirement
new b_assists[MAX_BADGES]              // Badge kill assists total requirement
new b_assists_s[MAX_BADGES]            // Badge kill assists session requirement
new b_heals[MAX_BADGES]                // Badge heals total requirement
new b_heals_s[MAX_BADGES]              // Badge heals session requirement
new b_teamacts[MAX_BADGES]             // Badge teamacts total requirement
new b_teamacts_s[MAX_BADGES]           // Badge teamacts session requirement
new b_roguespec[MAX_BADGES]            // Badge rogue special attack total requirement
new b_roguespec_s[MAX_BADGES]          // Badge rogue special attack session requirement
new num_badges = 0                     // Holds actual number of badges created

// III. Variables for holding player data

// Core player data
new p_xp[MAX_PLAYERS]                     // Player xp 
new p_level[MAX_PLAYERS]                  // Player level
new p_race[MAX_PLAYERS]                   // Player race
new p_class1[MAX_PLAYERS]                 // Player primary class
new p_class2[MAX_PLAYERS]                 // Player secondary class
new p_gold[MAX_PLAYERS]                   // Player gold amount
new p_spellpage[MAX_PLAYERS]              // Last spell merch page viewed
new p_energy[MAX_PLAYERS]                 // Energy for Rogues
new p_maxhealth[MAX_PLAYERS]              // Max. health for player
new p_maxarmor[MAX_PLAYERS]               // Max. armor for player
new Float:p_maxspeed[MAX_PLAYERS]         // Max. speed for player

// Player attributes
new p_str[MAX_PLAYERS]                    // Player strength attribute
new p_agi[MAX_PLAYERS]
new p_stam[MAX_PLAYERS]
new p_int[MAX_PLAYERS]                    // Player active int value
new p_spir[MAX_PLAYERS]
new p_armor[MAX_PLAYERS]                  // Player armor modifier
new p_mana[MAX_PLAYERS]                   // Player mana
new p_mana_base[MAX_PLAYERS]              // Player mana base
new p_mana_max[MAX_PLAYERS]               // Player mana max
new p_tier[MAX_PLAYERS]                   // Player gear tier

new p_int_unb[MAX_PLAYERS]                // Player unbuffed int value

// Player secondary attributes
new p_melee[MAX_PLAYERS]                  // Player melee ability
new p_marksman[MAX_PLAYERS]               // Player marksman ability
new p_arcane[MAX_PLAYERS]                 // Player arcane ability
new p_fire[MAX_PLAYERS]                   // Player fire ability
new p_frost[MAX_PLAYERS]                  // Player frost ability
new p_shadow[MAX_PLAYERS]                 // Player shadow ability
new p_nature[MAX_PLAYERS]                 // Player nature ability
new p_holy[MAX_PLAYERS]                   // Player holy ability

new p_crit[MAX_PLAYERS]                   // Player critical strike chance
new p_spcrit[MAX_PLAYERS]                 // Player spell-critical strike chance
new p_spdmg[MAX_PLAYERS]                  // Player spell dmg chance


// Player stats data, used for gear/armour unlocking
new p_kills[MAX_PLAYERS]                  // Player total lifetime kills
new p_deaths[MAX_PLAYERS]                 // Player total lifetime deaths
new p_rounds[MAX_PLAYERS]                 // Player total lifetime rounds played
new p_casts[MAX_PLAYERS]                  // Player total lifetime # spells casts 
new p_heals[MAX_PLAYERS]                  // Player total lifetime heal spells cast
new p_bombs[MAX_PLAYERS]                  // Player total lifetime bomb plants
new p_defuses[MAX_PLAYERS]                // Player total lifetime bomb defuses 
new p_supports[MAX_PLAYERS]               // Player total lifetime bomb/defuse supports 
new p_assists[MAX_PLAYERS]                // Player total lifetime kill assists
new p_roguespec[MAX_PLAYERS]              // Player total lifetime rogue special attacks
new p_rezzes[MAX_PLAYERS]                 // Player total lifetime resurrections
new p_teamacts[MAX_PLAYERS]               // Player total lifetime team actions
new p_badges[MAX_PLAYERS]                 // Player badges bitfield1
new p_badges2[MAX_PLAYERS]                // Player badges bitfield2

new p_kills_s[MAX_PLAYERS]                // Player session kills
new p_deaths_s[MAX_PLAYERS]               // Player session deaths
new p_rounds_s[MAX_PLAYERS]               // Player session rounds played
new p_casts_s[MAX_PLAYERS]                // Player session # spells casts 
new p_heals_s[MAX_PLAYERS]                // Player session heal spells cast
new p_bombs_s[MAX_PLAYERS]                // Player session bomb plants
new p_defuses_s[MAX_PLAYERS]              // Player session bomb defuses 
new p_supports_s[MAX_PLAYERS]             // Player session bomb/defuse supports 
new p_assists_s[MAX_PLAYERS]              // Player session kill assists
new p_roguespec_s[MAX_PLAYERS]            // Player session rogue special attacks
new p_rezzes_s[MAX_PLAYERS]               // Player session resurrections
new p_teamacts_s[MAX_PLAYERS]             // Player session team actions


// Player gear and items
new p_gearpage[MAX_PLAYERS]               // Armor/gear page player is on
new p_itempage[MAX_PLAYERS]               // Items merch page player is on
new p_gearlist[MAX_PLAYERS][MAX_GLIST]    // Available gearlist
new p_itemlist[MAX_PLAYERS][MAX_GLIST]    // Available itemlist
new p_inventory[MAX_PLAYERS][MAX_INV]     // Player inventory
new p_head[MAX_PLAYERS]                   // Armor/gear for head
new p_chest[MAX_PLAYERS]                  // Armor/gear for torso 
new p_back[MAX_PLAYERS]                   // Armor/gear for back 
new p_legs[MAX_PLAYERS]                   // Armor/gear for legs 
new p_hands[MAX_PLAYERS]                  // Armor/gear for hands 
new p_feet[MAX_PLAYERS]                   // Armor/gear for feet 
new p_wrists[MAX_PLAYERS]                 // Armor/gear for wrists 
new p_ring1[MAX_PLAYERS]                  // Armor/gear for ring1 
new p_ring2[MAX_PLAYERS]                  // Armor/gear for ring2 
new p_neck[MAX_PLAYERS]                   // Armor/gear for neck 

// Player entity-related data 
new p_rootent[MAX_PLAYERS]                // Stores roots entity for rooted players

// Player adjustable screen data
new p_hud1[MAX_PLAYERS]                   // Player HUD1 resolution setting
new p_spells[MAX_PLAYERS][NUM_SPELLS]     // Player spells and skills data
new p_talents[MAX_PLAYERS][NUM_TALENTS]   // Player class talents data

new p_weapon[MAX_PLAYERS] = 0             // Weapon currently wielded
new respage[MAX_PLAYERS]                  // Resurrection page to display
new sellspellpage[MAX_PLAYERS]            // Sell Spell Menu page to display
new sellequipmentpage[MAX_PLAYERS]        // Sell Equipment Menu page to display
new autocastpage[MAX_PLAYERS]             // Autocast menu page
new page[MAX_PLAYERS]                     // Current displayed menu page
new deathmessageshown[MAX_PLAYERS]        // Death messages shown for players
new loadedlevel[MAX_PLAYERS] = 0          // Has saved xp-levels been loaded yet
new damageto[MAX_PLAYERS][MAX_PLAYERS]    // Damage dealt table for added XP
new reslist[MAX_PLAYERS][MAX_PLAYERS+1]   // For Priests, res list

new bool:p_class1autocast[MAX_PLAYERS][8]       // Primary class autocast vals (0 or 1)
new bool:p_class2autocast[MAX_PLAYERS][8]       // Primary class autocast vals (0 or 1)
new bool:readytoload[MAX_PLAYERS]               // Ready to load from storage 
new bool:client_ready_mainmenu[MAX_PLAYERS]     // Ready to display main wowmod menu
new bool:g_specMode[MAX_PLAYERS]                // Is player spectating
new bool:change_conf[MAX_PLAYERS] = false       // Player has confirmed race/class change
new bool:manahud_running[MAX_PLAYERS] = false   // Is player seeing mana HUD display
new bool:racial_used[MAX_PLAYERS] = false       // Has racial ability been used this round?
new bool:has_shadow[MAX_PLAYERS] = false        // Has Shadow Meld racial ability on
new bool:has_perception[MAX_PLAYERS] = false    // Has Perception racial ability on
new bool:has_stoneform[MAX_PLAYERS] = false     // Has Stoneform racial ability on
new bool:has_berzerk[MAX_PLAYERS] = false       // Has Berzerk racial ability on
new bool:has_cannibal[MAX_PLAYERS] = false      // Has Berzerk racial ability on
new bool:medicalert[MAX_PLAYERS] = false        // Has medic alert been sent this round 
new bool:arcinton[MAX_PLAYERS] = false          // Has arcane intellect buff?
new bool:brillianceon[MAX_PLAYERS] = false      // Has arcane brilliance buff?
new bool:magearmoron[MAX_PLAYERS] = false       // Has mage armor buff?
new bool:sprinton[MAX_PLAYERS] = false          // Has sprint buff
new bool:vanishon[MAX_PLAYERS] = false          // Has vanish buff
new bool:evasionon[MAX_PLAYERS] = false         // Is Rogue evasion on?
new bool:gifton[MAX_PLAYERS] = false            // Has gift of the wild buff?
new bool:tranqon[MAX_PLAYERS] = false           // Is Tranquility on 
new bool:barkon[MAX_PLAYERS] = false            // Is Barkskin on 
new bool:apackon[MAX_PLAYERS] = false           // Is aspect of the Pack on 
new bool:awildon[MAX_PLAYERS] = false           // Is aspect of the Wild on 
new bool:demonskinon[MAX_PLAYERS] = false       // Is demon skin on warlock
new bool:eviscerateon[MAX_PLAYERS] = false      // Is eviscerate on
new bool:wingclipon[MAX_PLAYERS] = false        // Is WingClip effect on 
new bool:cheapshoton[MAX_PLAYERS] = false       // Is Cheapshot on 
new bool:blizzardon[MAX_PLAYERS] = false        // Is Blizzard effect on 
new bool:pwshieldon[MAX_PLAYERS] = false        // Is Power Word: Shield on 
new bool:stunned[MAX_PLAYERS] = false           // Has player been stunned
new bool:isstomped[MAX_PLAYERS] = false         // Is Warstomp stun on
new bool:isrooted[MAX_PLAYERS] = false          // Is Rooted 
new bool:isfrosted[MAX_PLAYERS] = false         // Is affected by frostbolt slowdown
new bool:isweakened[MAX_PLAYERS] = false        // Has curse of weakness 
new bool:issearching[MAX_PLAYERS] = false       // Is player searching for a target
new bool:isinvis[MAX_PLAYERS] = false           // Is invsible due to racial or spell 
new bool:serpstingon[MAX_PLAYERS] = false       // Has been afflicted with serpent sting
new bool:corruptionon[MAX_PLAYERS] = false      // Has been afflicted with corruption
new bool:ruptureon[MAX_PLAYERS] = false         // Has been afflicted with rupture
new bool:garrotteon[MAX_PLAYERS] = false        // Has been afflicted with garrotte 
new bool:swpainon[MAX_PLAYERS] = false          // Has been afflicted with shadow word: pain
new bool:wordpainon[MAX_PLAYERS] = false        // Has been afflicted with shadow word: pain 
new bool:blinkdown[MAX_PLAYERS] = false         // Is player recovering from blink
new bool:game_commence[MAX_PLAYERS] = false     // Has game begun for player?
new bool:diedlastround[MAX_PLAYERS]             // Did the player die last round
new bool:helmet[MAX_PLAYERS]                    // Is player wearing a helmet
new bool:hassuit[MAX_PLAYERS]                   // Is player wearing armor
new bool:ctshieldon[MAX_PLAYERS] = false        // Is player a CT using tactical shield 
new bool:healdown[MAX_PLAYERS] = false          // Is player in heal cooldown mode
new Float:spa_speed[MAX_PLAYERS] = 0.0          // Spec abil speed on/off


// Miscellaneous new to WowMOD

new Float:BADGE_DIV = 100.0                     // Divisor for badge requirements (to calc fast-xp mode)
new DUAL_CLASS_LEVEL = 30                       // Level needed to be able to select second class
new DUAL_CLASS_NUMSPELLS = 20                   // Number of spells trainable from secondary class
new GOLD_FASTXP_MULT = 100                      // Gold multiplier for fast XP mode

new Float:SELL_SPELL_DIV = 10.0                 // Sellback value divisor for spells
new Float:SELL_EQUIP_DIV = 10.0                 // Sellback value divisor for equipment

new GOLD_KILL = 20 
new GOLD_DMG = 5
new GOLD_BOMB = 30
new GOLD_DEFUSE = 30
new GOLD_RESCUE = 20 
new GOLD_SUPPORT = 10 
new GOLD_WIN = 50 
new GOLD_ESCAPE = 50
new GOLD_KILLVIP = 50
new GOLD_ASSIST = 20 
new GOLD_ASSIST2 = 100 
new GOLD_REZ = 20 

new color_white[3] = { 255, 255, 255 }
new color_red[3] = { 255, 0, 0 }
new color_green[3] = { 0, 255, 0 }
//new color_blue[3] = { 0, 0, 255 }

// Miscellaneous Kept from UWC3
new HEALTHBONUS = HEALTHBONUS_DEF
new g_menuPosition[33]
new g_menuPlayers[33][32]
new g_menuPlayersNum[33]
new g_menuOption[33]
new g_menuSettings[33]

new bool:endround                               // Has round ended or still going
new bool:u_delay = false                        // Kept from UWC3, ultimate delay toggle
 
new freezetime = 0                              // Are we in freezetime mode
new freezecalled = 0                            // Is freezetime over
new vipID = 0                                   // Player ID for VIP
//new hostagesaver                                // Player ID that saved hostage(s)
//new bombDefuserIndex[33]                        // Player IDs defusing the bomb
//new bombCarrier                                 // Player ID carrying the bomb
//new bombdefuser                                 // Player ID thhat completed defuse
//new bombstate                                   // Bomb state (0, 1 or 2)


// IV. Help System Variables
new help_text[82][] = {
   "WowMOD uses race-based gameplay and implements many of the races from the WoW MMORPG. The mod currently has the following races implemented: <b>Nightelf</b>, <b>Gnome</b>, <b>Human</b>, <b>Dwarf</b>, <b>Orc</b>, <b>Tauren</b>, <b>Undead</b>, <b>Bloodelf</b>. You can request the helpfile for a specific race implemented in WowMOD by saying the command 'help <race>'. ex. say help dwarf will display the Dwarf race helpfile." ,
   "When you select a race to play in WowMOD you must then choose a primary class or profession to play. Each race has a list of classes that are available to be chosen, the mod currently has the following classes implemented: <b>Druid</b>, <b>Hunter</b>, <b>Mage</b>, <b>Rogue</b>, <b>Warlock</b>, <b>Priest</b>. You can request the helpfile for a specific class implememnted in WowMOD by saying the command 'help <class>'. ex. say help warlock to see the Warlock class helpfile.",
   "Each class (profession) has 8 spells available for players to train. Many of these spells are passive and require no action on the player's part to use them in combat, they are used automatically based on chance. Other spells require that they be used specifically by the player, some of these spells can be used through a key-binding and others through spoken commands. Please see the specific spell helpfiles for how they may be used." ,
   "The Nightelves are an honorbound race and are distrusting of the other races of the world. They have an innate ability to evade danger and have a chance to dodge the attacks of their enemies. Nightelves also have an inherent resistance to nature damage and have the ability to shadow meld once per round.",
   "The gnomish race of today is all that remains of the survivors of the fall of Gnomeregan, they have an obsession for developing new technologies for which to serve the alliance. The gnomes benefit from an increased intelligence, a natural resistance to arcane damage and have the ability to break a root or snare once per round.",
   "The Dwarves of Ironforge are a robust humanoid race who are quick to assist their allies when the need arises. The dwarves have an innate ability to handle guns and benefit from a natural resistance to frost damage. Once per round they may use stoneform to give them immunity to bleed damage, poison and disease.",
   "The Humans of Stormwind are a resilient race and are seen as some of the fiercest warriors, they are allied with the Gnomes, Dwarves and Nightelves. The humans benefit from an increased spirit and have a natural resistance to knife-based attacks. Once per round they can use perception to boost their stealth detection.", 
   "The Orcs of Draenor are mistakenly thought to be mindless brutes with no sense of humanity. Although not born to Azeroth, they have sworn to uphold their right to survive in their new world. Orcs have an innate resistance to stun attacks and a Berzerk special ability, which increases their overall damage output while sacrificing some vulnerability to enemy attacks.",
   "The Tauren are large creatures from the lands of Kalimdor, they live their lives to serve the forces of nature. The Tauren benefit from an increased maximum health and an innate resistance to nature damage. Once per round they can use war stomp to stun all surrounding enemies.",
   "The Undead of today were born from a renegade group which had escaped the bounds of the Lich King, today they are known as the Forsaken. The undead benefit from a natural resistance to shadow damage and once per round they can use cannibalize to quickly regenerate a portion of their maximum health.",
   "The Bloodelves are the survivors of the undead scourge invasion of Quel'Thalas, their lives are devoted to regaining their lost superiority of the arcane. The bloodelves benefit from a natural resistance to all forms of magical damage and once per turn they may use mana tap to drain the mana from an enemy.",
   "The Druids are the masters of nature and are considered keepers of the world. Druid spells range from the healing arts to the ability to call nature's wrath down on their enemies, with many diverse abilities in between such as self-sustaining buffs to counter-damage shields. Druid healing spells are less efficient than priest healing spells but they have more offensive capabilities.",
   "Hunters are skilled marksmen and natural survivalists. Hunter abilities include damage over time spells, spells to slow down their enemies and/or speed up themselves and their party and others to aid in their combat power.",
   "Mages are the masters of the elements and of the arcane. Mages specialize in fire and frost damage as well as damage from the arcane arts, their spells can range from single-target damage to wide-ranging spells that may inflict damage on the entire enemy team. Mages possess little to help them defensively, but their damage capabilities more than make up any weakness.",
   "The Rogue is a skilled combatant with the knife and many of their spells are toned specifically for knife damage. Rogues are masters of stealth and can hide in plain sight, they also possess the ability to dodge out of harm's way. Rogue spells require energy to be cast instead of mana, rogues gain energy through normal combat and from being in stealth mode.",
   "Warlocks are the masters of fire and shadow, they have the power to drain the life and mana of their enemies to add to their powers. Warlock spells include those that deal fire or shadow damage, personal shields, damage over time spells and curses to weaken their enemies.",
   "The priest is the master of the restorative arts, they can heal themselves, their allies individually as well as their entire team with just one spell. The priest is also the only class which can resurrect a fallen friend. Additionally priests can destroy the mana pool of their enemies and invoke a personal shield to help them defensively. Priest healing spells are more efficient than other classes, however, their only damage spell, Smite, is less effective and more costly than other classes.",
   "Healing Touch is the standard heal spell available to Druids, not as efficient as a priest heal spell but it gets the job done.<br /><br />Healing Touch is a single-target spell, to use it bind a key to 'wow_healtouch', then target a friend and press the bound key to heal your friend.",
   "Wrath is the Druid's only direct offensive spell, it is passive and based on chance and the learned spell rank it strikes the druid's enemies in combat and inflicts nature damage.<br /><br />Wrath is a passive spell and requires explicit spell casting",
   "Remove Curse allows the Druid to help friends that have been afflicted by a warlock curse spell. Besides the benefit from helping your team to win, removing curses from your friends will also earn you bonus XP and Gold.<br /><br />Remove Curse is a single-target spell, to use it bind a key to 'wow_remcurse' then target a friend and press the bound key.",
   "Gift of the Wild is an incredibly valuable spell that enhances (buffs) all of the target's attributes and proficiencies which will help their offensive spells succeed and improve their resistances to enemy spell damage, it also grants you with bonus XP and Gold.<br /><br />Gift of the Wild can be cast on yourself and on your friends. To self-cast say 'cast giftwild', to buff a friend bind a key to 'wow_giftwild' and then target your friend and press the bound key.",
   "Thorns is a damage shield that helps to protect the druid by inflicting counter-damage to his/her enemies.<br /><br />Thorns is a passive spell and requires no explicit spell casting",
   "Tranquility allows the Druid to assist all nearby friends with enhanced health regeneration, it also grants bonus XP and Gold to the caster.<br /><br />Tranquility is a spoken spell, cast it by saying 'cast tranq'",
   "Barkskin is another spell in the Druid's defensive lineup providing the caster with spell damage absorption ( the higher spell rank obtained the more damage is absorbed ).<br /><br />Barkskin is a spoken spell cast by saying 'cast barkskin'",
   "Entangling Roots serves the Druid as both an offensive and defensive spell by causing an enemy's feet to be stuck to the ground. Note: The Gnome racial ability allows them a manaless opportunity to escape from your roots.<br /><br />Entangling Roots is a single-target search spell, to use it bind a key to 'wow_roots' then press the bound key to begin searching for an enemy. When an enemy appears in your crosshairs the spell will activate and root them to the ground." ,
   "Serpent Sting is the mainstay of the Hunter offensive lineup with high spellranks virtually guaranteeing enemy deaths or kill asists.<br /><br />Serpent Sting is a passive spell and requires no explicit spell casting.",
   "Aspect of the Cheetah allows the Hunter to run potentially very fast (at higher spell ranks).<br /><br />Aspect of the Cheetah is passive and requires no explicit spell casting, it kicks in at the start of every round.",
   "Wingclip allows the Hunter's speed (from Aspect of the Cheetah) to really have an impact in battle as it slows down the runspeed of your enemies.<br /><br />Wingclip is a passive spell requiring no explicit spell casting.",
   "Aspect of the Pack is the first of the Hunter's team assist commands which when cast will improve the runspeed of all the Hunter's friends.<br /><br />Aspect of the Pack is a spoken spell, cast by saying 'cast pack'",
   "Aspect of the Wild is a team assist spell that allows the Hunter to boost the nature magic of all friends, on a team full of Hunters or Druids this can easily turn the tide of battle.<br /><br />Aspect of the Wild is a spoken spell, cast by saying 'cast wild'",
   "Counter Attack is the Hunter's is the first in the Hunter's defensive lineup, providing counter-damage to all enemies inflicting damage on the Hunter.<br /><br />Counter Atack is passive and requires no explicit spell casting.",
   "Aspect of the Monkey is the mainstay of the Hunter defensive lineup providing evasion abilities to help the Hunter avoid taking damage. Aspect of the Monkey combined with the Nightelf inherent Dodge ability is a great way to maximize the damage you can avoid.<br /><br />Aspect of the Monkey is a passive spell and requires no explicit spell casting.",
   "Aimed Shot is the Hunter's most powerful attack, but it is the costliest in terms of mana usage so it requires careful usage ot be very effecive.<br /><br />To use Aimed Shot bind a key to 'wow_aimed', press the bound key to start searching for a target enemy in your crosshairs.",
   "Scorch is the Mage's base offensive spell, requiring relatively low amounts of mana to cast and at high spell ranks inflicting serious amounts of damage.<br /><br />Scorch is a passive spell and requires no explicit spell casting.",
   "Arcane Intellect allows the Mage to expand their base mana pool, which gives them the ability to cast more spells in combat.<br /><br />Arcane Intellect is a spoken spell, cast by saying 'cast arcint'",
   "Frostbolt is the first spell in the Mage lineup that afflicts their victims with runspeed slowdown, and since it also delivers damage it is serves as both an offensive and defensive spell.<br /><br />To use Frostbolt bind a key to 'wow_frostbolt', press the key in battle to start searching for a target in your crosshair.",
   "Mage Armor provides a resistance buff to the Mage in all forms of magic, if you have mana available this spell should never be left down.<br /><br />Mage Armor is a spoken spell, cast by saying 'cast magearmor'",
   "Flamestrike is the Mage's first area of effect spell and is a very effective way to damage a large group of the enemy team. It is mana intensive so cast it very carefully.<br /><br />To use Flamestrike bind a key to 'wow_flamestrike', then target an area in the map near your enemies and press the bound key",
   "Arcane Brilliance is the Mage's contribution to team assistance, it provides a team-wide intelligence buff similar to the Mage's self only Arcane Intellect. Team assist spells grant the caster bonus XP and Gold but they do require more mana to cast.<br /><br />Arcane Brilliance is a spoken spell cast by saying 'cast brilliance'",
"Blizzard is a very powerful spell allowing the Mage to deal frost damage to a large group of the enemy and potentially stunning/slowing them all down as well. A fun strategy is to cast Blizzard at a group of enemies and then cast Flamestrike immediately following before the slowed enemies can escape.<br /><br />To use bind a key to 'wow_blizzard', then target an area in the map and press the bound key.",
   "Blink is an extremely powerful ability that only the Mage possesses which allows him/her to teleport to an area of the map within sight. It is extremely useful as both an offensive and defensive spell.<br /><br />To use bind a key to 'wow_blink' then target an area in your sights and press the bound key.",
   "Cheap Shot is one of only two ranged offensive spells usable by the Rogue class. The spell does some damage to your targets as well as cause their runspeed to slow to a crawl for a short duration.<br /><br />Cheap Shot is a passive spell and requires no explicit spell casting.",
   "Sprint is an important utility sell for the Rogue, allowing some help if escape is needed or providing a nice speed advantage to help the Rogue get close enough for knife-based attacks.<br /><br />Sprint is a spoken spell, cast by saying 'cast sprint'",
   "Vanish is perhaps the most important Rogue spell in the game, allowing the Rogue to be all but invisible to enemies. Stealth is important for the many knife-based Rogue attacks and Vanish will provide it, plus, Vanish stacks with the Nightelf Shadow Meld racial ability allowing an even greater degree of invisibility.<br /><br />Vanish is a spoken spell and is cast by saying 'cast vanish'",
   "Backstab has the capability to be the Rogue's most lethal attack. If a rogue is able to get behind his enemy and do knife damage to the enemy's back, Backstab damage bonus is awarded. A maxxed out Backstab means an insta-kill hit for the Rogue.<br /><br />Backstab is a passive spell, try to get behind your enemy and do knife damage to their back to score a Backstab.",
   "Rupture is the only other ranged offensive Rogue spell beside Cheap Shot, when successful it will cause bleed damage to your enemy.<br /><br />Rupture is a passive spell and requires no explicit spell casting.",
   "Evasion is a Rogue utility spell that improves their dodge rating considerably for a short period of time, provided the rogue is wielding a knife during this time. When ranged weapons are being held while this spell is invoked only a small benefit may be seen in your dodge rating.<br /><br />Evasion is a spoken spell, cast by saying 'cast evasion'",
   "Garrotte is a bleed damage spell that is triggered automatically when you engage in knife combat. The chance for this spell to land on your enemy is based on your agility and your melee proficiency, once it hits your enemy will begin to bleed slowly until death.<br /><br />Garrotte is a passive spell that requires no explicit casting to use, it does require a knife attack.",
   "Eviscerate is the Rogue's attempt to gut his opponent, literally. This spell is unlike other Rogue spells, casting it will prepare yourself for the Eviscerate attempt which will not be triggered until you engage in knife combat with the enemy. A Master of Eviscerate will kill his enemy in one successful attack.<br /><br />Eviscerate is a bindable spell, to use it bind a key to 'wow_eviscerate', then trigger the bound key before engaging in knife combat.",
   "Drain Life is a vampiric spell used by the Warlock to drain health from your enemies and transfer it back to yourself.<br /><br />Drain Life is passive and requires no explicit spell casting",
   "Curse of Weakness afflicts your enemies with a weakness curse that caused all of their offeisve damage to be halved.<br /><br />Curse of Weakness is passive and requires no explicit spell casting.",
   "Drain Mana is a vampiric warlock spell which can be used to steal mana away from your enemies.<br /><br />Drain Mana is a passive spell and requires no explicit spell casting.",
   "Corruption is an affliction spell that will cause the Warlock's enemy to become diseased, causing slow damage over time and halting the normal health regeneration process. If left unchecked corruption will slowly kill the Warlock's afflicted enemies.<br /><br />Corruption is a passive spell and requires no explicit spell casting.",
   "Demon Skin is a Warlock self-only buff that will help protect the Warlock from weapons damage by absorbing some of the damage taken. Demon Skin also provides a buff to the Warlock's natural health regeneration process.<br /><br />Demon Skin is a spoken spell, it can be cast by saying 'cast demonskin'",
   "Lifetap is a spell which the Warlock can use as a last ditch effort to stay alive. Lifetap taps into a Warlock's remaining mana pool and converts some or all of it to health. The spell uses your mana more efficiently as you increase your Lifetap spell rank.<br /><br />Lifetap is a spoken spell, it can be cast by saying 'cast lifetap'",
   "Immolate is a single target fire-based damage spell which when successful will cause a small explosion to erupt about an enemy and cause fire damage.<br /><br />To use this spell bind a key to 'wow_immolate', then try to target an enemy in your crosshairs",
   "Shadow Bolt is the most efficient offensive spell in the Warlock's arsenal. When cast it attempts to seek out a targetted enemy and when successful it consumes the target in shadow, causing great damage.<br /><br />Shadow Bolt is a single target search spell, to use it bind a key to 'wow_bolt', then in combat press the key and target an enemy.",
   "Heal is the most efficient Healing spell in the game, providing the most healing power for the most reasonable mana cost, it is the basis for the Priest's team assistance contributions.<br /><br />To self heal say 'cast heal', to heal friends bind a key to 'wow_heal', then target a friend and press the bound key",
   "Smite is one of only two ways a Priest can deal spell damage, it is less efficient than other classes' offensive spells but it is one of the only spells in the mod that deal Holy damage (which is less likeluy to be resisted than other more common dmage types).<br /><br />Smite is passive and requires no explicit spell casting.",
   "Lesser Heal is a lesser form of the Priest Heal spell with significant differences: 1) it is completely manaless to cast, 2) it does not provide any bonus XP or Gold for casting, 3) it will never heal as effectively as the full-fledged Heal spell.<br /><br />To use this spell bind a key to 'wow_lesserheal', target a friend and press the bound key.",
   "Power Word: Shield is the Priest's defensive shield, providing damage absorption protection against weapons damage.<br /><br />Power Word: Shield is a spoken spell, to cast say 'cast shield'",
   "Shadow Word: Pain provides sustained holy damage over time to your enemies that are unfortunate enough to be afflicted. Dwarves wth Stoneform activated will resist this spell automatically.<br /><br />This spell is passive and requires no explicit spell casting to use.",
   "Manaburn attempts to burn some or all of the mana supply for your targetted enemy. An enemy without mana is a much less of a threat.<br /><br />Manaburn is a passive spell and requires no explicit casting to use.",
   "Prayer of Healing provides healing benefits for your whole team, and rewards you with the benefits of XP and Gold bonuses.<br /><br />Prayer of Healing is a spoken spell, cast it by saying 'cast prayer'",
   "Ressurect is the spell which singles the Priest class out as the most efficient team-assist based class. Resurrect will provide the Priest with a menu listing of all dead teamates, mana permitting they can have them back on their feet in no time.<br /><br />Resurrect is a spoken spell, say 'cast rez' or 'cast res' to display the Resurrection menu.",
   "Your WowMOD player account comes with 100 inventory slots for your armor and items purchased. The following commands are available: <b>say /inventory</b> - display your inventory , <b>say /worn</b> - view your worn equipment, <b>say /wear item_name</b> - wear an item in your inventory, <b>say /remove item_name</b> - to remove a worn item, <b>say /drop item_name</b> - to drop an item (you will no longer own it). <b>say /examine item_name</b> to view item info.",
   "There are 3 merchant types in WowMOD: 1) Spells merchant - sells player spells, 2) Magical Items Merchant - sells magic rings and necklaces and 3) The Armourer - Crafts armor sets for players. <b>say /wowmenu</b> and then select option 4 to access the main merchants menu.",
   "Players have 5 main character attributes that affect their performance and can be improved via spells or through armor upgrades.</p>Strength - Determines your maximum health<br />Agility - For rogues, determines attack success<br />Stamina - Determines your health regeneration<br />Intellect - For all casters, helps determine spell cast success<br />Spirit - Determines Mana and Energy generation",
   "Each class has one or more spell proficiencies that affect player spell casting, these can be improved through armor and item upgrades.</p>Melee - Rogue spells<br />Marksman - Hunter spells<br />Arcane, Fire and Frost - Mage spells, Fire also for Warlocks<br />Shadow - Warlock spells<br />Nature - Druid Spells<br />Holy - Priest spells",
   "Each race has a unique racial ability that distinguishes it from the others, players may use their racial abilities starting from level 1. As you progress in level some racial ability's duration extends and some also improve in their effectiveness. For additional info see 'help shadowmeld', 'help escape', 'help stoneform', 'help perception', 'help berzerk', 'help warstomp', 'help cannibal', and 'help manatap' -- Bind a key to 'wow_racial'",
   "Shadow Meld allows the Nightelf to blend into the shadows, becoming less visible to other players. As players progress in level the duration of Shadow Meld and invisibility level increase.<br /><br />To use, bind a key to wow_racial",
   "Escape Artist allows the Gnome to escape from a root or snare (runspeed slowdown) spell, as players progress in level the number of escapes increases.<br /><br />To use, bind a key to wow_racial",
   "Stoneform allows the Dwarf to tuen their skin into stone, making them immune to bleed damage (damage over time), poison and disease based attacks. As players progress in level their Stoneform duration increases.<br /><br />To use, bind a key to wow_racial",
   "Perception allows the Human to easily identify members of the opposing team, including those players that are invisible. As players progress in level the duration of their Perception increases.<br /><br />To use, bind a key to wow_racial",
   "Berzerk provides the Orc with improved base damage for all weapon types, as players progress in level the duration of their Berzerk increases. It should be noted that Orcs in a Berzerking state are also more vulnerable to enemy attacks, so be cautious and plan accordingly.<br /><br />To use, bind a key to wow_racial",
   "The Tauren War Stomp is a feared special attack ability only available to tauren players. War Stomp stuns all nearby enemies and inflicts a small amount of damage on them ( except those which are immune to stuns, such as Orcs ) . As players progress in level the duration of their stun increases.<br /><br />To use, bind a key to wow_racial and press the bound key when near one or more enemies",
   "The Undead may cannibalize their flesh once per turn to improve their base health regeration, as players progress in level the duration and magnitude of their regeneration rate increase.<br /><br />To use, bind a key to wow_racial",
   "The Bloodelves may use their Mana Tap special ability once per turn to steal the mana from a targetted enemy. As players progress in level they will be able to steal more mana.<br /><br />To use, bind a key to wow_racial",
   "Critical Strike is a special WowMOD ability that is not based on any one race or class, it becomes available through magical gear which granst the wearer a percentage chance to hit with Critical damage. Tip: Look for bracers or wristguards to /examine for +crit. A successful critical strike will double the base damage of your attack that has landed on an enemy.",
   "Spell Critical Strike is similar to a weapons Critical Strike (see /help critical strike), except the damage bonus applies only to spell-based damage.",
   "Spell Damage is a special WowMOD ability that adds bonus damage to all damage inflicted from spells. Spell damage modifiers can be acquired through worn epic-level equipment.",
   "The autocast system allows you to specify spells that you have learned to be automatically cast at the start of ever new round, this works great for buff spells like Rogue Sprint, Warlock Demonskin, Mage Magearmor, Priest shield, etc. You may only autocast spoken-cmd spells, say /autocast to bring up the autocast menu."

}


// **************************************************************************
// END WowMOD Global Variables 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

