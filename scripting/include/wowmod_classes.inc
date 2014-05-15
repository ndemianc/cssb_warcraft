/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Classes
*** Date:          Aug 11, 2007
*** Last Update:   Oct 24, 2007
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
// BEGIN WowMOD Class Module
// **************************************************************************


// **************************************************************************
// BEGIN wowmod_classinit_
// Initialize routine for player class selection
// **************************************************************************
public wowmod_classinit_( id ){
}


// **************************************************************************
// BEGIN wowmod_classinit_druid
// Initialize routine for player class selection
// **************************************************************************
public wowmod_classinit_druid( id, mode ){
   if ( mode == R_NORMAL ){
      client_print( id, print_center, 
         "[%s] You have selected the Druid class - say 'help Druid' for info about this profession", MOD )
      client_print( id, print_chat, 
         "[%s] You have selected the Druid class - say 'help Druid' for info about this profession", MOD )
   }

   p_class1[id] = CLASS_DRUID

   if ( p_race[id] == RACE_NIGHTELF ){
      p_str[id] = 18
      p_agi[id] = 25 
      p_stam[id] = 19 
      p_int[id] = 22 
      p_spir[id] = 22 
      p_armor[id] = 55 
      p_mana[id] = 100 

      p_melee[id] = 0
      p_marksman[id] = 0
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 25
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_TAUREN ){
      p_str[id] = 26
      p_agi[id] = 15 
      p_stam[id] = 22 
      p_int[id] =  17 
      p_spir[id] = 24 
      p_armor[id] = 35 
      p_mana[id] = 67 

      p_melee[id] = 0
      p_marksman[id] = 0
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 30 
      p_shadow[id] = 0
      p_holy[id] = 0
   }
   p_mana_base[id] = p_mana[id]
   p_int_unb[id] = p_int[id]
}


// **************************************************************************
// BEGIN wowmod_classinit_hunter
// Initialize routine for player class selection
// **************************************************************************
public wowmod_classinit_hunter( id, mode ){
   if ( mode == R_NORMAL ){
      client_print( id, print_center, 
         "[%s] You have selected the Hunter class - say 'help Hunter' for info about this profession", MOD )
      client_print( id, print_chat, 
         "[%s] You have selected the Hunter class - say 'help Hunter' for info about this profession", MOD )
   }

   p_class1[id] = CLASS_HUNTER

   if ( p_race[id] == RACE_NIGHTELF ){
      p_str[id] = 18
      p_agi[id] = 25 
      p_stam[id] = 19 
      p_int[id] = 22 
      p_spir[id] = 22 
      p_armor[id] = 55 
      p_mana[id] = 100 

      p_melee[id] = 0
      p_marksman[id] = 20
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 10
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_DWARF ){
      p_str[id] = 22 
      p_agi[id] = 19 
      p_stam[id] = 24 
      p_int[id] = 19
      p_spir[id] = 20
      p_armor[id] = 40 
      p_mana[id] = 84 

      p_melee[id] = 5
      p_marksman[id] = 30
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_ORC ){
      p_str[id] = 23 
      p_agi[id] = 20 
      p_stam[id] = 23 
      p_int[id] = 17
      p_spir[id] = 24
      p_armor[id] = 42 
      p_mana[id] = 82 

      p_melee[id] = 10
      p_marksman[id] = 20
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_TAUREN ){
      p_str[id] = 25 
      p_agi[id] = 18 
      p_stam[id] = 23 
      p_int[id] = 15
      p_spir[id] = 23
      p_armor[id] = 38 
      p_mana[id] = 80 

      p_melee[id] = 0
      p_marksman[id] = 20
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 10
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_BLOODELF ){
      p_str[id] = 17 
      p_agi[id] = 25 
      p_stam[id] = 19 
      p_int[id] = 24
      p_spir[id] = 20
      p_armor[id] = 54 
      p_mana[id] = 145 

      p_melee[id] = 0
      p_marksman[id] = 20
      p_arcane[id] = 10
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }
   p_mana_base[id] = p_mana[id]
   p_int_unb[id] = p_int[id]
}


// **************************************************************************
// BEGIN wowmod_classinit_mage
// Initialize routine for player class selection
// **************************************************************************
public wowmod_classinit_mage( id, mode ){
   if ( mode == R_NORMAL ){
      client_print( id, print_center, 
         "[%s] You have selected the Mage class - say 'help Mage' for info about this profession", MOD )
      client_print( id, print_chat, 
         "[%s] You have selected the Mage class - say 'help Mage' for info about this profession", MOD )
   }

   p_class1[id] = CLASS_MAGE

   if ( p_race[id] == RACE_HUMAN ){
      p_str[id] = 20
      p_agi[id] = 20 
      p_stam[id] = 20 
      p_int[id] = 23 
      p_spir[id] = 23 
      p_armor[id] = 45 
      p_mana[id] = 165 

      p_melee[id] = 5
      p_marksman[id] = 5
      p_arcane[id] = 15
      p_fire[id] = 15 
      p_frost[id] = 15 
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_GNOME ){
      p_str[id] = 15
      p_agi[id] = 23 
      p_stam[id] = 19 
      p_int[id] = 27 
      p_spir[id] = 22 
      p_armor[id] = 51 
      p_mana[id] = 225 

      p_melee[id] = 0
      p_marksman[id] = 0
      p_arcane[id] = 20
      p_fire[id] = 15
      p_frost[id] = 15
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_UNDEAD ){
      p_str[id] = 19
      p_agi[id] = 18 
      p_stam[id] = 21 
      p_int[id] = 21 
      p_spir[id] = 27 
      p_armor[id] = 41 
      p_mana[id] = 135 

      p_melee[id] = 0
      p_marksman[id] = 0
      p_arcane[id] = 15 
      p_fire[id] = 15 
      p_frost[id] = 15 
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_BLOODELF ){
      p_str[id] = 17
      p_agi[id] = 22 
      p_stam[id] = 18 
      p_int[id] = 27 
      p_spir[id] = 21 
      p_armor[id] = 49 
      p_mana[id] = 225 

      p_melee[id] = 0
      p_marksman[id] = 0
      p_arcane[id] = 25
      p_fire[id] = 10
      p_frost[id] = 10
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }
   p_mana_base[id] = p_mana[id]
   p_int_unb[id] = p_int[id]
}


// **************************************************************************
// BEGIN wowmod_classinit_rogue
// Initialize routine for player class selection
// **************************************************************************
public wowmod_classinit_rogue( id, mode ){
   if ( mode == R_NORMAL ){
      client_print( id, print_center, 
         "[%s] You have selected the Rogue class - say 'help Rogue' for info about this profession", MOD )
      client_print( id, print_chat, 
         "[%s] You have selected the Rogue class - say 'help Rogue' for info about this profession", MOD )
   }

   p_class1[id] = CLASS_ROGUE

   if ( p_race[id] == RACE_NIGHTELF ){
      p_str[id] = 18
      p_agi[id] = 28 
      p_stam[id] = 20 
      p_int[id] = 20 
      p_spir[id] = 20 
      p_armor[id] = 58 
      p_mana[id] = 0 

      p_melee[id] = 20
      p_marksman[id] = 0
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 10
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_GNOME ){
      p_str[id] = 16
      p_agi[id] = 26 
      p_stam[id] = 20 
      p_int[id] = 24 
      p_spir[id] = 20 
      p_armor[id] = 54 
      p_mana[id] = 0

      p_melee[id] = 20
      p_marksman[id] = 0
      p_arcane[id] = 5
      p_fire[id] = 5
      p_frost[id] = 5
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_DWARF ){
      p_str[id] = 23
      p_agi[id] = 19 
      p_stam[id] = 24 
      p_int[id] = 19 
      p_spir[id] = 19 
      p_armor[id] = 40 
      p_mana[id] = 0

      p_melee[id] = 15
      p_marksman[id] = 10
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_HUMAN ){
      p_str[id] = 21
      p_agi[id] = 23 
      p_stam[id] = 21 
      p_int[id] = 20 
      p_spir[id] = 21 
      p_armor[id] = 48 
      p_mana[id] = 0

      p_melee[id] = 15
      p_marksman[id] = 5
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_ORC ){
      p_str[id] = 24
      p_agi[id] = 20 
      p_stam[id] = 23 
      p_int[id] = 17 
      p_spir[id] = 23 
      p_armor[id] = 42 
      p_mana[id] = 0

      p_melee[id] = 30
      p_marksman[id] = 5
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_UNDEAD ){
      p_str[id] = 20
      p_agi[id] = 21 
      p_stam[id] = 22 
      p_int[id] = 18 
      p_spir[id] = 25 
      p_armor[id] = 65 
      p_mana[id] = 0

      p_melee[id] = 25
      p_marksman[id] = 0
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_BLOODELF ){
      p_str[id] = 18
      p_agi[id] = 25 
      p_stam[id] = 19 
      p_int[id] = 24 
      p_spir[id] = 19 
      p_armor[id] = 54 
      p_mana[id] = 0

      p_melee[id] = 15
      p_marksman[id] = 0
      p_arcane[id] = 10
      p_fire[id] = 5
      p_frost[id] = 5
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 0
   }
   p_mana_base[id] = p_mana[id]
   p_int_unb[id] = p_int[id]
}


// **************************************************************************
// BEGIN wowmod_classinit_warlock
// Initialize routine for player class selection
// **************************************************************************
public wowmod_classinit_warlock( id, mode ){
   if ( mode == R_NORMAL ){
      client_print( id, print_center, 
         "[%s] You have selected the Warlock class - say 'help Warlock' for info about this profession", MOD )
      client_print( id, print_chat, 
         "[%s] You have selected the Warlock class - say 'help Warlock' for info about this profession", MOD )
   }

   p_class1[id] = CLASS_WARLOCK

   if ( p_race[id] == RACE_HUMAN ){
      p_str[id] = 20
      p_agi[id] = 20 
      p_stam[id] = 21 
      p_int[id] = 22 
      p_spir[id] = 23 
      p_armor[id] = 45 
      p_mana[id] = 140 

      p_melee[id] = 5
      p_marksman[id] = 5
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 20
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_GNOME ){
      p_str[id] = 15
      p_agi[id] = 23 
      p_stam[id] = 20 
      p_int[id] = 26 
      p_spir[id] = 22 
      p_armor[id] = 51 
      p_mana[id] = 200 

      p_melee[id] = 0
      p_marksman[id] = 0
      p_arcane[id] = 5
      p_fire[id] = 5
      p_frost[id] = 5
      p_nature[id] = 0
      p_shadow[id] = 20
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_ORC ){
      p_str[id] = 23
      p_agi[id] = 17 
      p_stam[id] = 23 
      p_int[id] = 19 
      p_spir[id] = 25 
      p_armor[id] = 39 
      p_mana[id] = 109 

      p_melee[id] = 10
      p_marksman[id] = 5
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 15
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_UNDEAD ){
      p_str[id] = 19
      p_agi[id] = 18 
      p_stam[id] = 22 
      p_int[id] = 20 
      p_spir[id] = 27 
      p_armor[id] = 41 
      p_mana[id] = 110 

      p_melee[id] = 0
      p_marksman[id] = 0
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 25
      p_holy[id] = 0
   }else if ( p_race[id] == RACE_BLOODELF ){
      p_str[id] = 17
      p_agi[id] = 22 
      p_stam[id] = 19 
      p_int[id] = 26 
      p_spir[id] = 21 
      p_armor[id] = 49 
      p_mana[id] = 197 

      p_melee[id] = 0
      p_marksman[id] = 0
      p_arcane[id] = 10
      p_fire[id] = 5
      p_frost[id] = 5
      p_nature[id] = 0
      p_shadow[id] = 20
      p_holy[id] = 0
   }
   p_mana_base[id] = p_mana[id]
   p_int_unb[id] = p_int[id]
}


// **************************************************************************
// BEGIN wowmod_classinit_priest
// Initialize routine for player class selection
// **************************************************************************
public wowmod_classinit_priest( id, mode ){
   if ( mode == R_NORMAL ){
      client_print( id, print_center, 
         "[%s] You have selected the Priest class - say 'help Priest' for info about this profession", MOD )
      client_print( id, print_chat, 
         "[%s] You have selected the Priest class - say 'help Priest' for info about this profession", MOD )
   }

   p_class1[id] = CLASS_PRIEST

   if ( p_race[id] == RACE_NIGHTELF ){
      p_str[id] = 17
      p_agi[id] = 25 
      p_stam[id] = 19 
      p_int[id] = 22 
      p_spir[id] = 23 
      p_armor[id] = 55 
      p_mana[id] = 160 

      p_melee[id] = 0
      p_marksman[id] = 0
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 10
      p_shadow[id] = 0
      p_holy[id] = 20
   }else if ( p_race[id] == RACE_DWARF ){
      p_str[id] = 22
      p_agi[id] = 16 
      p_stam[id] = 23 
      p_int[id] = 21 
      p_spir[id] = 22 
      p_armor[id] = 37 
      p_mana[id] = 145 

      p_melee[id] = 5
      p_marksman[id] = 5
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 20
   }else if ( p_race[id] == RACE_HUMAN ){
      p_str[id] = 20
      p_agi[id] = 20 
      p_stam[id] = 20 
      p_int[id] = 22 
      p_spir[id] = 24 
      p_armor[id] = 45 
      p_mana[id] = 160 

      p_melee[id] = 5
      p_marksman[id] = 5
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 20
   }else if ( p_race[id] == RACE_UNDEAD ){
      p_str[id] = 19
      p_agi[id] = 18 
      p_stam[id] = 21 
      p_int[id] = 20 
      p_spir[id] = 28 
      p_armor[id] = 41 
      p_mana[id] = 130 

      p_melee[id] = 0
      p_marksman[id] = 0
      p_arcane[id] = 0
      p_fire[id] = 0
      p_frost[id] = 0
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 25
   }else if ( p_race[id] == RACE_BLOODELF ){
      p_str[id] = 17
      p_agi[id] = 22 
      p_stam[id] = 18 
      p_int[id] = 26 
      p_spir[id] = 22 
      p_armor[id] = 49 
      p_mana[id] = 220 

      p_melee[id] = 0
      p_marksman[id] = 0
      p_arcane[id] = 10
      p_fire[id] = 5
      p_frost[id] = 5
      p_nature[id] = 0
      p_shadow[id] = 0
      p_holy[id] = 20
   }
   p_mana_base[id] = p_mana[id]
   p_int_unb[id] = p_int[id]
}


// **************************************************************************
// BEGIN wowmod_class_selected
// Player has selected a class/profession
// **************************************************************************
public wowmod_class_selected( id, class ){

   switch( class ){
      case CLASS_DRUID: wowmod_classinit_druid( id, R_NORMAL )
      case CLASS_HUNTER: wowmod_classinit_hunter( id, R_NORMAL )
      case CLASS_MAGE: wowmod_classinit_mage( id, R_NORMAL )
      case CLASS_ROGUE: wowmod_classinit_rogue( id, R_NORMAL )
      case CLASS_WARLOCK: wowmod_classinit_warlock( id, R_NORMAL )
      case CLASS_PRIEST: wowmod_classinit_priest( id, R_NORMAL )
      default: return PLUGIN_CONTINUE
   }

   readytoload[id] = false
   sqlwrite_id( id )

   wowmod_gear_worninit( id )

   check_effects( id, 1 )

   wowmod_hud_playerdata( id, HUD_MODE_BASIC )

   wowmod_display_mainmenu( id )

   return PLUGIN_CONTINUE

}


// **************************************************************************
// END WowMOD Class Module 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

