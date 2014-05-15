/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Heads Up Displays
*** Date:          Aug 11, 2007
*** Last Update:   Aug 21, 2007
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
// BEGIN WowMOD HUD Systems 
// **************************************************************************


// **************************************************************************
// BEGIN wowmod_hud_playerdata subroutine
// Display important player data in HUD
// **************************************************************************
public wowmod_hud_playerdata( id, mode ){
   //log_amx( "id=(%d) ready_main=(%d) readyload=(%d)", id, client_ready_mainmenu[id], readytoload[id] )

   if ( !wowmod_enabled || !id || !is_user_connected(id) || client_ready_mainmenu[id] || readytoload[id] )
      return PLUGIN_CONTINUE

   // Get ready to call wowmod_hud_mana
   new parm2[2]
   parm2[0] = id
   if ( !manahud_running[id] && !endround )
      set_task( 0.5, "wowmod_hud_mana", TASK_MANAHUD+id, parm2, 2, "b" );

   new pdata[1023] 
   new name[32]

   get_user_name(id, name, 31)

   //log_amx( "id=(%d) name=(%s) is_bot=(%d)", id, name, is_user_bot(id) )

   if ( is_user_bot(id) )
      return PLUGIN_CONTINUE
   

   // Should never have negative xp
   if ( p_xp[id] < 0 )
      p_xp[id] = 0

   if ( ( p_race[id] < 0 ) || ( p_class1[id] < 0 ) )
      return PLUGIN_CONTINUE

   // Handle levelup and gear tier checks and initial levels loaded from saved xp data
   wowmod_check_levelup( id )
   //wowmod_check_tier( id )

   //log_amx( "r=(%d) race=(%s) c=(%d) class1=(%s)", p_race[id], races[p_race[id]], p_class1[id], classes[p_class1[id]] )

   //new sec_class[32] = "No Secondary Prof"
   //if ( p_class2[id] >= 0 )
   //   copy( sec_class, 31, classes[p_class2[id]] )

   format( pdata, 1023, "WowMOD> Lvl: %d XP: %d ( %s : %s ) ", 
      p_level[id], p_xp[id], races[p_race[id]], classes[p_class1[id]] )

   message_begin( MSG_ONE, gmsgStatusText, {0,0,0}, id)
   write_byte( 0 )
   write_string( pdata )
   message_end()


   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_hud_mana subroutine
// Display player mana/energy info
// **************************************************************************
public wowmod_hud_mana( parm2[2] ){

   new id = parm2[0]
   new temp[64] = ""
   new temp2[64] = ""
   new affects[128] = ""
   new rready[32] = ""
   new color[3] 

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   if ( id > MAX_PLAYERS )
      return PLUGIN_CONTINUE

   if ( !is_user_connected( id ) )
      return PLUGIN_CONTINUE

   color = color_white
   manahud_running[id] = true

   // Top HUD channel is for mod name, version and website url
   format( temp2, 63, "%s  v%s  [ cswowmod.com ]", MOD, VER )
 
   if ( p_hud1[id] == 0 )
      set_hudmessage( color[0], color[1], color[2], 0.01, 0.69, 0, 6.0, 12.0, 0.01, 0.01, 2) 
   show_hudmessage( id, temp2 ) 

   // Next HUD channel is for racial ability info

   if ( ( p_race[id] >= RACE_NIGHTELF ) && ( p_race[id] < NUM_RACES ) ){
      if ( racial_used[id] ){
         color = color_red
         copy( rready, 31, "Not Ready" )
      }else{
         color = color_green
         copy( rready, 31, "Ready" )
      }
      //format( temp2, 63, "Racial Ability: %s [ %s ]", racials[p_race[id]], rready )
      format( temp2, 63, "[%s %s]", racials[p_race[id]], rready )
   }else{
      color = color_white
      //format( temp2, 63, "[No race]" )
   }

   //if ( p_hud1[id] == 0 )
   //   set_hudmessage( color[0], color[1], color[2], 0.01, 0.71, 0, 6.0, 12.0, 0.01, 0.01, 3) 
   //show_hudmessage( id, temp2 ) 

   // Next HUD channel is for Effects display
   new bool:has_affect = false
   format( affects, 127, "%s Affects: ", temp2 )
   if ( isweakened[id] ){
      format( affects, 127, "%s Weakened", affects )
      has_affect = true
   }
   if ( isrooted[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Rooted", affects )
      else
         format( affects, 127, "%s Rooted", affects )
      has_affect = true
   }
   if ( stunned[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Stunned", affects )
      else
         format( affects, 127, "%s Stunned", affects )
      has_affect = true
   }
   if ( isstomped[id] || blizzardon[id] || isfrosted[id] || wingclipon[id] || cheapshoton[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Slowed", affects )
      else
         format( affects, 127, "%s Slowed", affects )
      has_affect = true
   }
   if ( arcinton[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Intellect", affects )
      else
         format( affects, 127, "%s Intellect", affects )
      has_affect = true
   }
   if ( magearmoron[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Mage Armor", affects )
      else
         format( affects, 127, "%s Mage Armor", affects )
      has_affect = true
   }
   if ( demonskinon[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Demon Skin", affects )
      else
         format( affects, 127, "%s Demon Skin", affects )
      has_affect = true
   }
   if ( gifton[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Gift of the Wild", affects )
      else
         format( affects, 127, "%s Gift of the Wild", affects )
      has_affect = true
   }
   if ( barkon[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Barkskin", affects )
      else
         format( affects, 127, "%s Barkskin", affects )
      has_affect = true
   }
   if ( pwshieldon[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Shield", affects )
      else
         format( affects, 127, "%s Shield", affects )
      has_affect = true
   }
   if ( sprinton[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Sprint", affects )
      else
         format( affects, 127, "%s Sprint", affects )
      has_affect = true
   }
   if ( vanishon[id] || has_shadow[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Invisible", affects )
      else
         format( affects, 127, "%s Invisible", affects )
      has_affect = true
   }
   if ( evasionon[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Evasion", affects )
      else
         format( affects, 127, "%s Evasion", affects )
      has_affect = true
   }
   if ( has_perception[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Perception", affects )
      else
         format( affects, 127, "%s Perception", affects )
      has_affect = true
   }
   if ( has_berzerk[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Berzerk", affects )
      else
         format( affects, 127, "%s Berzerk", affects )
      has_affect = true
   }
   if ( has_cannibal[id] ){
      if ( has_affect )
         format( affects, 127, "%s | Cannibalize", affects )
      else
         format( affects, 127, "%s Cannibalize", affects )
      has_affect = true
   }

   if ( !has_affect )
      format( affects, 127, "%s None", affects )
 
   color = color_white
   if ( p_hud1[id] == 0 )
      set_hudmessage( color[0], color[1], color[2], 0.01, 0.71, 0, 6.0, 12.0, 0.01, 0.01, 4 ) 
   show_hudmessage( id, affects ) 


   // Last HUD channel is for Energy/Mana Info
   if ( p_class2[id] > -1 ){
      // Secondary class selected
      if ( ( p_class1[id] == CLASS_ROGUE ) && ( p_class2[id] != CLASS_ROGUE ))
         format(temp, 63, "Energy: %d  Mana: %d  Gold: %d", p_energy[id], p_mana[id], p_gold[id] )
      else if ( ( p_class1[id] != CLASS_ROGUE ) && ( p_class2[id] == CLASS_ROGUE ))
         format(temp, 63, "Mana: %d  Energy: %d  Gold: %d", p_mana[id], p_energy[id], p_gold[id] )
      else
         format(temp, 63, "Mana: %d  Gold: %d", p_mana[id], p_gold[id] )
   }else{
      if ( p_class1[id] == CLASS_ROGUE )
         format(temp, 63, "Energy: %d  Gold: %d", p_energy[id], p_gold[id] )
      else
         format(temp, 63, "Mana: %d  Gold: %d", p_mana[id], p_gold[id] )
   }

   // Hud message set based on lowres setting
   color = color_white
   if ( p_hud1[id] == 0 )
      set_hudmessage( color[0], color[1], color[2], 0.01, 0.73, 2, 0.02, 12.0, 0.01, 0.01, 1 )
   show_hudmessage( id, temp )


   //set_task( 1.0, "wowmod_hud_mana", TASK_MANAHUD+id, parm2, 2 );

   return PLUGIN_CONTINUE

}



// **************************************************************************
// END WowMOD HUD Systems 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

