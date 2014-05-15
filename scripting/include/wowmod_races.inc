/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Race System
*** Date:          Aug 11, 2007
*** Last Update:   Aug 12, 2007
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
// BEGIN WowMOD Race System 
// **************************************************************************




// **************************************************************************
// BEGIN wowmod_raceinit_nightelf
// Initialize a new race selection for race nightelf 
// **************************************************************************
public wowmod_raceinit_nightelf( id ){
   client_print( id, print_center, 
      "[%s] You have selected a Nightelf - say 'help Nightelf' for info about this race", MOD )
   client_print( id, print_chat, 
      "[%s] You have selected a Nightelf - say 'help Nightelf' for info about this race", MOD )
   wowmod_display_nelf_classmenu( id )
}


// **************************************************************************
// BEGIN wowmod_raceinit_gnome
// Initialize a new race selection for race gnome
// **************************************************************************
public wowmod_raceinit_gnome( id ){
   client_print( id, print_center, 
      "[%s] You have selected a Gnome - say 'help Gnome' for info about this race", MOD )
   client_print( id, print_chat, 
      "[%s] You have selected a Gnome - say 'help Gnome' for info about this race", MOD )
   wowmod_display_gnome_classmenu( id )
}


// **************************************************************************
// BEGIN wowmod_raceinit_dwarf
// Initialize a new race selection for race dwarf 
// **************************************************************************
public wowmod_raceinit_dwarf( id ){
   client_print( id, print_center, 
      "[%s] You have selected a Dwarf - say 'help Dwarf' for info about this race", MOD )
   client_print( id, print_chat, 
      "[%s] You have selected a Dwarf - say 'help Dwarf' for info about this race", MOD )
   wowmod_display_dwarf_classmenu( id )
}


// **************************************************************************
// BEGIN wowmod_raceinit_human
// Initialize a new race selection for race human 
// **************************************************************************
public wowmod_raceinit_human( id ){
   client_print( id, print_center, 
      "[%s] You have selected a Human - say 'help Human' for info about this race", MOD )
   client_print( id, print_chat, 
      "[%s] You have selected a Human - say 'help Human' for info about this race", MOD )
   wowmod_display_human_classmenu( id )
}


// **************************************************************************
// BEGIN wowmod_raceinit_orc
// Initialize a new race selection for race orc 
// **************************************************************************
public wowmod_raceinit_orc( id ){
   client_print( id, print_center, 
      "[%s] You have selected an Orc - say 'help Orc' for info about this race", MOD )
   client_print( id, print_chat, 
      "[%s] You have selected an Orc - say 'help Orc' for info about this race", MOD )
   wowmod_display_orc_classmenu( id )
}


// **************************************************************************
// BEGIN wowmod_raceinit_tauren
// Initialize a new race selection for race tauren 
// **************************************************************************
public wowmod_raceinit_tauren( id ){
   client_print( id, print_center, 
      "[%s] You have selected a Tauren - say 'help Tauren' for info about this race", MOD )
   client_print( id, print_chat, 
      "[%s] You have selected a Tauren - say 'help Tauren' for info about this race", MOD )
   wowmod_display_tauren_classmenu( id )
}


// **************************************************************************
// BEGIN wowmod_raceinit_undead
// Initialize a new race selection for race undead 
// **************************************************************************
public wowmod_raceinit_undead( id ){
   client_print( id, print_center, 
      "[%s] You have selected an Undead - say 'help Undead' for info about this race", MOD )
   client_print( id, print_chat, 
      "[%s] You have selected an Undead - say 'help Undead' for info about this race", MOD )
   wowmod_display_undead_classmenu( id )
}


// **************************************************************************
// BEGIN wowmod_raceinit_bloodelf
// Initialize a new race selection for race bloodelf 
// **************************************************************************
public wowmod_raceinit_bloodelf( id ){
   client_print( id, print_center, 
      "[%s] You have selected a Bloodelf - say 'help Bloodelf' for info about this race", MOD )
   client_print( id, print_chat, 
      "[%s] You have selected a Bloodelf - say 'help Bloodelf' for info about this race", MOD )
   wowmod_display_belf_classmenu( id )
}


// **************************************************************************
// BEGIN wowmod_race_selected
// Front end for new race selection handler
// **************************************************************************
public wowmod_race_selected( id, race ){

   if ( p_race[id] != -1 ){
      // Reset race-dependant data
      p_class1[id] = -1
      p_class2[id] = -1
      p_mana[id] = 0
      p_mana_base[id] = 0
      for ( new j = 0; j < NUM_SPELLS; j++ )
         p_spells[id][j] = 0
   }

   p_race[id] = race

   switch ( race ){
      case RACE_NIGHTELF: wowmod_raceinit_nightelf( id )
      case RACE_GNOME: wowmod_raceinit_gnome( id )
      case RACE_DWARF: wowmod_raceinit_dwarf( id )
      case RACE_HUMAN: wowmod_raceinit_human( id )
      case RACE_ORC: wowmod_raceinit_orc( id )
      case RACE_TAUREN: wowmod_raceinit_tauren( id )
      case RACE_UNDEAD: wowmod_raceinit_undead( id )
      case RACE_BLOODELF: wowmod_raceinit_bloodelf( id )
      
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_racial_chooser
// WowMOD racial ability
// **************************************************************************
public wowmod_racial_chooser( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   if ( p_race[id] < 0 || p_race[id] > NUM_RACES ){
      client_print( id, print_chat, "[%s] You must first choose a race to play", MOD )
      return PLUGIN_HANDLED
   }

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cannot use their racial abilities", MOD )
      return PLUGIN_HANDLED
   }

   switch ( p_race[id] ){
      case RACE_NIGHTELF: wowmod_racial_shadow( id )
      case RACE_GNOME: wowmod_racial_escape( id )
      case RACE_DWARF: wowmod_racial_stone( id )
      case RACE_HUMAN: wowmod_racial_perception( id )
      case RACE_ORC: wowmod_racial_berzerk( id )
      case RACE_TAUREN: wowmod_racial_warstomp( id )
      case RACE_UNDEAD: wowmod_racial_cannibal( id )
      case RACE_BLOODELF: wowmod_racial_manatap( id )
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_racial_shadow
// WowMOD racial ability
// **************************************************************************
public wowmod_racial_shadow( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new ability[32]
   copy( ability, 31, "Shadow Meld" )

   if ( p_race[id] != RACE_NIGHTELF ){
      client_print( id, print_chat, "[%s] Only Nightelves have the %s ability", MOD, ability )
      return PLUGIN_HANDLED
   } 

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cannot use racial abilities", MOD )
      return PLUGIN_HANDLED
   }

   if ( racial_used[id] ){
      client_print( id, print_chat, "[%s] You have already used your %s ability this round", MOD, ability )
      return PLUGIN_HANDLED
   } 
  
   racial_used[id] = true 

   has_shadow[id] = true
   wowmod_effect_glow( id, GLOW_INVIS )

   client_print( id, print_chat, "[%s] You blend into the shadows", MOD )

   new args[2]
   args[0] = id

   new Float:ftimer = RACIAL_BASE_TIMER + p_level[id] - 10.0
   args[1] = 10 
   set_task( ftimer, "wowmod_warning_shadow", TASK_SHADOWWARN1+id, args, 2 );

   ftimer = RACIAL_BASE_TIMER + p_level[id] - 5.0
   args[1] = 5 
   set_task( ftimer, "wowmod_warning_shadow", TASK_SHADOWWARN2+id, args, 2 );

   ftimer = RACIAL_BASE_TIMER + p_level[id]
   set_task( ftimer, "wowmod_racial_end", TASK_RACIAL+id, args, 2 );

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_racial_escape
// WowMOD racial ability
// **************************************************************************
public wowmod_racial_escape( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new ability[32]
   copy( ability, 31, "Escape Artist" )

   if ( p_race[id] != RACE_GNOME ){
      client_print( id, print_chat, "[%s] Only Gnomes have the %s ability", MOD, ability )
      return PLUGIN_HANDLED
   } 

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cannot use racial abilities", MOD )
      return PLUGIN_HANDLED
   }

   if ( racial_used[id] ){
      client_print( id, print_chat, "[%s] Your %s ability is not ready", MOD, ability )
      return PLUGIN_HANDLED
   } 
  
   racial_used[id] = true 

   if ( isrooted[id] ){
      // free from roots
      remove_entity( p_rootent[id] )
      p_rootent[id] = 0
      isrooted[id] = false
      stunned[id] = false
      set_user_maxspeed( id, p_maxspeed[id] )

      if ( is_user_alive( id ) )
         client_print( id, print_chat,
            "[%s] The roots fall away and dissolve", MOD )
   }else if ( wingclipon[id] ){
      // free from wingclip 
      wingclipon[id] = false
      set_user_maxspeed( id, p_maxspeed[id] )

      if ( is_user_alive( id ) )
         client_print( id, print_chat,
            "[%s] You feel your normal runspeed return", MOD )
   }else if ( blizzardon[id] ){
      // free from blizzard 
      blizzardon[id] = false
      set_user_maxspeed( id, p_maxspeed[id] )

      if ( is_user_alive( id ) )
         client_print( id, print_chat,
            "[%s] The chill subsides, you feel your normal runspeed return", MOD )
   }else if ( isfrosted[id] ){
      // free from frostbolt 
      isfrosted[id] = false
      set_user_maxspeed( id, p_maxspeed[id] )

      if ( is_user_alive( id ) )
         client_print( id, print_chat,
            "[%s] The frost melts away, you feel your normal runspeed return", MOD )

   }

   new args[2]
   args[0] = id
   new Float:ftimer = RACIAL_BASE_TIMER2 - ( 3.0 * p_level[id] )
   set_task( ftimer, "wowmod_racial_end", TASK_RACIAL+id, args, 2 );

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_racial_stone
// WowMOD racial ability
// **************************************************************************
public wowmod_racial_stone( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new ability[32]
   copy( ability, 31, "Stoneform" )

   if ( p_race[id] != RACE_DWARF ){
      client_print( id, print_chat, "[%s] Only Dwarves have the %s ability", MOD, ability )
      return PLUGIN_HANDLED
   } 

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cannot use racial abilities", MOD )
      return PLUGIN_HANDLED
   }

   if ( racial_used[id] ){
      client_print( id, print_chat, "[%s] You have already used your %s ability this round", MOD, ability )
      return PLUGIN_HANDLED
   } 
  
   racial_used[id] = true 
   has_stoneform[id] = true

   if ( serpstingon[id] ){
      // free from serpent sting 
      serpstingon[id] = false
      client_print( id, print_chat, "[%s] The Serpent's Sting fades away, you feel better", MOD )
   }else if ( corruptionon[id] ){
      // free from corruption 
      corruptionon[id] = false
      client_print( id, print_chat, "[%s] Your disease has subsided, you feel better ", MOD )
   }else if ( ruptureon[id] ){
      // free from rupture
      ruptureon[id] = false
      client_print( id, print_chat, "[%s] Your wound has ceased to bleed, you feel better", MOD )
   }else if ( garrotteon[id] ){
      // free from garrotte
      garrotteon[id] = false
      client_print( id, print_chat, "[%s] Your wound has ceased to bleed, you feel better", MOD )
   }else if ( wordpainon[id] ){
      // free from shadow word: pain 
      wordpainon[id] = false
      client_print( id, print_chat, "[%s] Shadow Word: Pain has subsided, you feel better", MOD )
   }

   new args[2]
   args[0] = id
   new Float:ftimer = RACIAL_BASE_TIMER + p_level[id]
   set_task( ftimer, "wowmod_racial_end", TASK_RACIAL+id, args, 2 );

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_racial_perception
// WowMOD racial ability
// **************************************************************************
public wowmod_racial_perception( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new ability[32]
   copy( ability, 31, "Perception" )

   if ( p_race[id] != RACE_HUMAN ){
      client_print( id, print_chat, "[%s] Only Humans have the %s ability", MOD, ability )
      return PLUGIN_HANDLED
   } 

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cannot use racial abilities", MOD )
      return PLUGIN_HANDLED
   }

   if ( racial_used[id] ){
      client_print( id, print_chat, "[%s] Your %s ability is not ready", MOD, ability )
      return PLUGIN_HANDLED
   } 
  
   racial_used[id] = true 
   has_perception[id] = true

   new Float:ftimer = RACIAL_BASE_TIMER + p_level[id]

   client_print( id, print_chat, "[%s] For the next %d seconds you will see your enemies more clearly", 
      MOD, floatround( ftimer ) )

   new tid
   new player[32]
   new numplayers
   get_players( player, numplayers )

   for ( new j = 0; j < numplayers; j++ ){
      tid = player[j]
      if ( get_user_team( id ) == get_user_team( tid ) )
         continue

      new parm[3]
      parm[0] = id
      parm[1] = tid
      parm[2] = 5 
      wowmod_effect_perception( parm )
   }
 
   new args[2]
   args[0] = id
   set_task( ftimer, "wowmod_racial_end", TASK_RACIAL+id, args, 2 );

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_racial_berzerk
// WowMOD racial ability
// **************************************************************************
public wowmod_racial_berzerk( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new ability[32]
   copy( ability, 31, "Berzerk" )

   if ( p_race[id] != RACE_ORC ){
      client_print( id, print_chat, "[%s] Only Orcs have the %s ability", MOD, ability )
      return PLUGIN_HANDLED
   } 

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cannot use racial abilities", MOD )
      return PLUGIN_HANDLED
   }

   if ( racial_used[id] ){
      client_print( id, print_chat, "[%s] You have already used your %s this round", MOD, ability )
      return PLUGIN_HANDLED
   } 
  
   racial_used[id] = true 
   has_berzerk[id] = true

   client_print( id, print_chat, "[%s] You feel a Rage coming on, you are now BERZERK", MOD )

   wowmod_sound_playsound( id, SOUND_BERZERK )
   wowmod_sound_playsound_allnear( id, SOUND_BERZERK )

   new args[2]
   args[0] = id
   new Float:ftimer = RACIAL_BASE_TIMER + p_level[id]
   set_task( ftimer, "wowmod_racial_end", TASK_RACIAL+id, args, 2 );

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_racial_warstomp
// WowMOD racial ability
// **************************************************************************
public wowmod_racial_warstomp( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new ability[32]
   copy( ability, 31, "War Stomp" )

   if ( p_race[id] != RACE_TAUREN ){
      client_print( id, print_chat, "[%s] Only Tauren have the %s ability", MOD, ability )
      return PLUGIN_HANDLED
   } 

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cannot use racial abilities", MOD )
      return PLUGIN_HANDLED
   }

   if ( racial_used[id] ){
      client_print( id, print_chat, "[%s] You have already used your %s this round", MOD, ability )
      return PLUGIN_HANDLED
   } 
  
   racial_used[id] = true 

   client_print( id, print_chat, "[%s] Your Warstomp shakes the ground all around you", MOD )

   new tid
   new tname[32]
   new origin[3]
   new torigin[3]
   new player[32]
   new numplayers

   get_user_origin( id, origin )
   get_players( player, numplayers )

   new Float:ftimer = RACIAL_BASE_TIMER + ( float(p_level[id]) / 5.0 )

   message_begin( MSG_ONE, gmsgShake, {0,0,0}, id)
   write_short( 135<< 14 ) //ammount
   write_short( 5 << 14) //lasts this long
   write_short( 255<< 14) //frequency
   message_end()

   for ( new i = 0; i < numplayers; ++i ){
      tid = player[i]
      get_user_name( tid, tname, 31 )
      get_user_origin( tid, torigin )

      if ( !is_user_alive( tid ) )
         continue
      if ( get_user_team( tid ) == get_user_team( id ) )
         continue

      if ( get_distance( origin, torigin ) <= get_cvar_num("FT_xp_radius") ){
         if ( p_race[tid] == RACE_ORC ){  
            client_print( tid, print_chat, 
               "[%s] You are immune to the stun effects of a nearby Warstomp", MOD )
            client_print( id, print_chat, 
               "[%s] %s is immune to the stun effects of your Warstomp", MOD, tname )
            continue
         }

         message_begin( MSG_ONE, gmsgShake, {0,0,0}, tid)
         write_short( 255<< 14 ) //ammount
         write_short( 10 << 14) //lasts this long
         write_short( 255<< 14) //frequency
         message_end()

         isstomped[tid] = true
         set_user_maxspeed( tid, RACIAL_WARSTOMP_SPEED )
         apply_damage( tid, id, RACIAL_WARSTOMP_DMG, CLASS_NA, SP_NA, "Warstomp" )
         client_print( id, print_chat, "[%s] Your Warstomp has stunned %s", MOD, tname )
         client_print( tid, print_chat, 
            "[%s] You are stunned as a nearby Warstomp shudders the ground beneath you", MOD )

         new args[2]
         args[0] = tid
         set_task( ftimer, "wowmod_racialunbuff_warstomp", TASK_RACIAL_WARSTOMP+tid, args, 2 );
      }
   }

   new args[2]
   args[0] = id
   set_task( ftimer, "wowmod_racial_end", TASK_RACIAL+id, args, 2 );

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_racial_cannibal
// WowMOD racial ability
// **************************************************************************
public wowmod_racial_cannibal( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new ability[32]
   copy( ability, 31, "Cannibalize" )

   if ( p_race[id] != RACE_UNDEAD ){
      client_print( id, print_chat, "[%s] Only Undead have the %s ability", MOD, ability )
      return PLUGIN_HANDLED
   } 

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cannot use racial abilities", MOD )
      return PLUGIN_HANDLED
   }

   if ( racial_used[id] ){
      client_print( id, print_chat, "[%s] You have already used your %s this round", MOD, ability )
      return PLUGIN_HANDLED
   } 
  
   racial_used[id] = true 
   has_cannibal[id] = true

   client_print( id, print_chat, 
      "[%s] You feed on your damaged flesh to improve your health regeneration", MOD )

   new args[2]
   args[0] = id
   new Float:ftimer = RACIAL_BASE_TIMER + p_level[id]
   set_task( ftimer, "wowmod_racial_end", TASK_RACIAL+id, args, 2 );

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_racial_manatap
// WowMOD racial ability
// **************************************************************************
public wowmod_racial_manatap( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new ability[32]
   copy( ability, 31, "Mana Tap" )

   if ( p_race[id] != RACE_BLOODELF ){
      client_print( id, print_chat, "[%s] Only Bloodelves have the %s ability", MOD, ability )
      return PLUGIN_HANDLED
   } 

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cannot use racial abilities", MOD )
      return PLUGIN_HANDLED
   }

   if ( racial_used[id] ){
      client_print( id, print_chat, "[%s] You have already used your %s this round", MOD, ability )
      return PLUGIN_HANDLED
   } 
 
   if ( issearching[id] ){
      client_print( id, print_chat, "[%s] You cannot use %s when already in search-target mode", 
         MOD, ability )
      return PLUGIN_HANDLED
   }
  
   racial_used[id] = true 

   new parm[3]
   parm[0] = id
   parm[1] = SPELL_SEARCH_TIME
   parm[2] = SP_MANATAP
   wowmod_search_target(parm)

   new args[2]
   args[0] = id
   new Float:ftimer = RACIAL_BASE_TIMER3 + p_level[id]
   set_task( ftimer, "wowmod_racial_end", TASK_RACIAL+id, args, 2 );

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_racial_end
// WowMOD racial ability - end affects
// **************************************************************************
public wowmod_racial_end( parm[2] ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new id = parm[0]
   new ability[32]

   if ( p_race[id] == RACE_NIGHTELF ){
      copy( ability, 31, "Shadow Meld" )
      has_shadow[id] = false
      wowmod_effect_glow( id, GLOW_NORMAL )
      if ( is_user_alive( id ) )
         client_print( id, print_chat, 
            "[%s] Your %s has ended, you fade back to visibility", MOD, ability )
   }else if ( p_race[id] == RACE_GNOME ){
      copy( ability, 31, "Escape Artist" )
      racial_used[id] = false
      if ( is_user_alive( id ) )
         client_print( id, print_chat, "[%s] Your %s ability is ready", MOD, ability )
   }else if ( p_race[id] == RACE_DWARF ){
      copy( ability, 31, "Stoneform" )
      has_stoneform[id] = false 
      if ( is_user_alive( id ) )
         client_print( id, print_chat, 
            "[%s] Your %s has ended, your skin returns to normal", MOD, ability )
   }else if ( p_race[id] == RACE_HUMAN ){
      copy( ability, 31, "Perception" )
      has_perception[id] = false 
      if ( is_user_alive( id ) )
         client_print( id, print_chat, 
            "[%s] Your %s has ended, you now see things normally", MOD, ability )
   }else if ( p_race[id] == RACE_ORC ){
      copy( ability, 31, "Berzerk" )
      has_berzerk[id] = false 
      if ( is_user_alive( id ) )
         client_print( id, print_chat, 
            "[%s] Your %s has ended, you feel your rage subside", MOD, ability )
   }else if ( p_race[id] == RACE_TAUREN ){
      copy( ability, 31, "War Stomp" )
      racial_used[id] = false
      if ( is_user_alive( id ) )
         client_print( id, print_chat, "[%s] Your %s ability is ready", MOD, ability )
   }else if ( p_race[id] == RACE_UNDEAD ){
      copy( ability, 31, "Cannibalize" )
      has_cannibal[id] = false 
      if ( is_user_alive( id ) )
         client_print( id, print_chat, 
            "[%s] Your %s ability has ended, you no longer feed on your damaged flesh", MOD, ability )
   }else if ( p_race[id] == RACE_BLOODELF ){
      copy( ability, 31, "Mana Tap" )
      racial_used[id] = false
      if ( is_user_alive( id ) )
         client_print( id, print_chat, "[%s] Your %s ability is ready", MOD, ability )
   }


   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_racialunbuff_warstomp subroutine
// Unbuff Warstomp effects 
// **************************************************************************
public wowmod_racialunbuff_warstomp( parm[] ){

   new id = parm[0]

   isstomped[id] = false
   set_user_maxspeed( id, p_maxspeed[id] )

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] You are no longer stunned from Warstomp", MOD )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_warning_shadow subroutine
// Warning that shadow meld will fade soon
// **************************************************************************
public wowmod_warning_shadow( parm[] ){

   new id = parm[0]
   new time_left = parm[1]

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Warning: Your Shadow Meld will fade in %d seconds", MOD, time_left )

   return PLUGIN_HANDLED
}


// **************************************************************************
// END WowMOD Race System 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

