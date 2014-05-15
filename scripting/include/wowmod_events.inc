/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Event Handlers
*** Date:          Aug 10, 2007
*** Last Update:   Sep 20, 2007
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
// BEGIN WowMOD Event Handlers 
// **************************************************************************

// **************************************************************************
// BEGIN new_round subroutine
// Called when a new round begins
// **************************************************************************
public new_round(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   // Check for an invalid id or non-connected user
   if ((id <= 0) || (id > MAX_PLAYERS))
      return PLUGIN_CONTINUE

   if ( !is_user_connected( id ) )
      return PLUGIN_CONTINUE

   new parm2[2]
   parm2[0] = id

   wowmod_kill_tasks( id )

   if ( is_user_connected( id ) )
      set_user_maxspeed( id, 1.0 ) 

   if( game_commence[id] ){
      // If game is commencing, do additional init
      diedlastround[id] = false
      game_commence[id] = false
      // Reset spawn sites internally if needed
   }


   // Check for players ready to load from storage
   if ( get_cvar_num("mp_savexp") && readytoload[id] ){
      if ( get_cvar_num("mp_sql") )
         sqlload_id(id)
      else
         load_id(id)
   }else if ( !get_cvar_num("mp_savexp") && readytoload[id] ){
      // Reset readytoload[] for Fast XP mode
      readytoload[id] = false
   }

   if ( get_cvar_num("WOW_dmgxpbonus") ){
      for ( new i=0; i < MAX_PLAYERS; i++ ){
         damageto[id][i] = 0
      }
   }

   // If we are in Fast mode, display message
   if (!get_cvar_num("mp_savexp")){
      set_hudmessage(200, 100, 0, -1.0, 0.35, 0, 1.0, 15.0, 0.1, 0.2, 2)
      show_hudmessage(id, "Server Mode: Fast XP -- Long Term XP is Disabled")
   }

   u_delay = false
   if ( get_cvar_num("FT_ultimatedelay") ){
      u_delay = true
      if ( !task_exists( TASK_UDELAYCOOL ) )
         set_task( get_cvar_float("FT_ultimatedelay"), "udelay_cooldown", TASK_UDELAYCOOL )
   }


   // Remove running tasks
   if (task_exists( TASK_MEDICSPR ))
      // Stops the medic sprite display
      remove_task_log( TASK_MEDICSPR )


   if ( freezecalled == 0 ){
      freezetime = 1
      freezecalled = 1
   }

   p_rounds[id] += 1       // Increment the number of rounds played stat
   p_rounds_s[id] += 1       // Increment the number of rounds played stat
   init_pdata( id, 1 )     // Perform a Limited init of player data
   endround = false        // New round, reset endround state

   if ( is_user_connected( id ) )
      set_user_maxspeed( id, 1.0 ) 

   check_effects( id, 1 )  // Old check_skills routine from UWC3


   // [09-03-04] - Display the medic alert sprites - K2mia
   if ( get_cvar_num("UW_medicalerts") == 2 )
      set_task( 1.0, "draw_medicspr", TASK_MEDICSPR )


   // heartbeat() is the main pulse timer for the round, all effects that occur
   // periodically like health/mana regen occur in the heartbeat routine
   set_task( HEARTBEAT, "heartbeat", TASK_HEARTBEAT+id, parm2, 2, "b" );
   set_task( AUTOCAST_TIME, "wowmod_autocast", TASK_AUTOCAST+id, parm2, 2 );

   // Reset the deathmessage so it can be shown this round
   deathmessageshown[id] = false

   // Display the main playerdata hud to the player
   wowmod_hud_playerdata( id, HUD_MODE_BASIC )

   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts

   if ( spell_pts_avail > 0 ){
      client_print( id, print_chat, "[%s] You have spell points available, say /spellmenu", MOD )
      client_print( id, print_center, "[%s] You have spell points available, say /spellmenu", MOD )
      //wowmod_display_spellmerchant( id )
   }

   // Determine which menu to show to the player, if any applicable
   if ( client_ready_mainmenu[id] ){
      // Display the WowMOD main menu
      wowmod_display_mainmenu( id )
   }else if ( readytoload[id] ){
      // Player data failed to load, display start menu
      wowmod_display_racemenu( id )
   }else if ( p_race[id] == -1 ){
      // Player has not yet selected a race
      wowmod_display_racemenu( id )
   }

   client_print( id, print_chat, 
      "[%s] Say: /help - for help, /wowmod - for the main menu, /wowstats - for your stats", MOD )

   // Check for new badges earned
   wowmod_check_badges( id )
   // Check for unlocked equipment tiers
   wowmod_check_tier( id )
 
   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN end_round subroutine
// Handler for the end of a round event
// **************************************************************************
public end_round(){
   if ( !wowmod_enabled )
     return PLUGIN_CONTINUE

   endround = true         // Toggle round has ended bool var
   freezecalled = 0        // Turn off freezecalled

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN event_player_action subroutine
// Handler for player events (bomb, hostages, etc)
// **************************************************************************
public event_player_action(){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new sArg[MAX_VAR_LENGTH], sAction[MAX_VAR_LENGTH]
   new sName[MAX_NAME_LENGTH]
   new id, iUserId

   read_logargv(0,sArg,MAX_VAR_LENGTH)
   read_logargv(2,sAction,MAX_VAR_LENGTH)
   parse_loguser(sArg,sName,MAX_NAME_LENGTH,iUserId)
   id = find_player("k",iUserId)

   if( id == 0 ){
      // Prevents the server from gaining XP
      return PLUGIN_CONTINUE
   }

   new CTnum = count_CT()
   new Tnum = count_T()

   if( ( Tnum + CTnum ) < get_cvar_num("FT_min_b4_XP") )
      return PLUGIN_HANDLED
   if ( !( ( Tnum > 0 ) && ( CTnum > 0 ) ) )
      return PLUGIN_HANDLED

   new temp = 0

   if (equal(sAction,"Planted_The_Bomb")) {
      // Bomb planted
      new origin[3]
      new teamname[32]
      new player[32]
      new numplayers
      new targetorigin[3]
      new targetid

      get_user_origin(id, origin)
      get_user_team(id, teamname, 31)
      get_players(player, numplayers)

      if (get_cvar_num("FT_objectives")) {
         temp = 3 * xpgiven_lev[p_level[id]]

         p_bombs[id] += 1
         p_bombs_s[id] += 1
         p_xp[id] += temp
         p_gold[id] += GOLD_BOMB

         client_print( id, print_chat,
            "[%s] %s recieved %d extra XP for personally planting the bomb.",
            MOD, sName, temp )
         client_print( id, print_chat,
            "[%s] %s recieved %d GOLD for personally planting the bomb.",
            MOD, sName, GOLD_BOMB )
      }

      for ( new i=0; i<numplayers; ++i ){
         targetid=player[i]

         if ( get_user_team(targetid) != TEAM_T ) continue
         if ( targetid == id ) continue

         get_user_origin( targetid, targetorigin )

         // [09-27-04] Modified playerlist so now have to check for living status - K2mia
         if ( is_user_alive(targetid) &&
              (get_distance(origin, targetorigin) <= get_cvar_num("FT_xp_radius")) )
         {
            if (get_cvar_num("FT_objectives")){
               temp = BOMBPLANTXP + xpgiven_lev[p_level[targetid]]

               p_supports[targetid] += 1
               p_supports_s[targetid] += 1
               p_xp[targetid] += temp
               p_gold[targetid] += GOLD_SUPPORT

               get_user_name( targetid, sName, 31 )
               client_print( targetid, print_chat,
                  "[%s] %s recieved %d extra XP for supporting the bomb planting effort.",
                  MOD, sName, temp)
               client_print( targetid, print_chat,
                  "[%s] %s recieved %d GOLD for supporting the bomb planting effort.",
                  MOD, sName, GOLD_SUPPORT)
            }
         }
      }
      wowmod_hud_playerdata( targetid, 1 )
   }else if ( equal(sAction,"Defused_The_Bomb")) {
      // Bomb defused
      new origin[3]
      new teamname[32]
      new player[32]
      new numplayers
      new targetorigin[3]
      new targetid

      get_user_origin( id, origin )
      get_user_team( id, teamname, 31 )
      get_players( player, numplayers )

      //bombstate = BOMB_DEFUSED  // Set BOMB_DEFUSED state, bomb was defused

      if (get_cvar_num("FT_objectives")){
         temp = 2 *  xpgiven_lev[p_level[id]]
         p_defuses[id] += 1
         p_defuses_s[id] += 1
         p_xp[id] += temp
         p_gold[id] += GOLD_DEFUSE
         client_print( id, print_chat,
            "[%s] %s recieved %d extra XP for defusing the bomb.", MOD, sName, temp )
         client_print( id, print_chat,
            "[%s] %s recieved %d GOLD for defusing the bomb.", MOD, sName, GOLD_DEFUSE )
      }

      for ( new i=0; i<numplayers; ++i ){
         targetid=player[i]

         if (get_user_team(targetid) != TEAM_CT) continue
         if ( targetid == id ) continue

         get_user_origin(targetid, targetorigin)

         // [09-27-04] Modified playerlist so now have to check for living status - K2mia
         if ( is_user_alive(targetid) &&
              (get_distance(origin, targetorigin)<=get_cvar_num("FT_xp_radius")) )
         {

            if (get_cvar_num("FT_objectives")){
               temp = DEFUSEXP +  xpgiven_lev[p_level[targetid]]

               p_supports[targetid] += 1
               p_supports_s[targetid] += 1
               p_xp[targetid]+= temp
               p_gold[targetid]+= GOLD_SUPPORT 

               get_user_name(targetid,sName,31)
               client_print(targetid, print_chat,
                  "[%s] %s recieved %d for supporting the bomb defusing effort.",
                  MOD, sName, temp)
               client_print( targetid, print_chat,
                  "[%s] %s recieved %d GOLD for supporting the bomb defusing effort.",
                  MOD, sName, GOLD_SUPPORT)
            }
         }
         wowmod_hud_playerdata( targetid, 1 )
      }
   }else if (equal(sAction,"Rescued_A_Hostage")) {
      // Hostage rescued

      if (get_cvar_num("FT_objectives")){
         temp = xpgiven_lev[p_level[id]]
         p_xp[id] += temp
         p_gold[id] += GOLD_RESCUE
         client_print( id, print_chat,
            "[%s] You have earned XP/Gold bonuses from a hostage rescue", MOD )
 
         wowmod_hud_playerdata( id, 1 )
      }
   }else if (equal(sAction,"Assassinated_The_VIP")) {
      // VIP assassinated
      new sNameVIP[MAX_NAME_LENGTH]
      get_user_name( vipID,sNameVIP,MAX_NAME_LENGTH)

      if(get_cvar_num("FT_VIP_kill_bonus")){
         if (get_cvar_num("FT_objectives")){
            temp = VIP_KILL_BONUS + xpgiven_lev[p_level[id]]

            p_xp[id] +=  temp
            p_gold[id] += GOLD_KILLVIP

            client_print( id, print_chat, "[%s] %s gained %d XP for assinating %s the VIP.",
               MOD, sName, temp, sNameVIP)
            client_print( id, print_chat, "[%s] %s received %d GOLD for assinating %s the VIP.",
               MOD, sName, GOLD_KILLVIP, sNameVIP)
         }

         wowmod_hud_playerdata( id, 1 )
      }
   }else if ( equal(sAction,"Escaped_As_VIP")) {
      // VIP escaped
      new origin[3]
      new teamname[32]
      new player[32]
      new numplayers
      new targetorigin[3]
      new targetid

      get_user_origin(id, origin)
      get_user_team(id, teamname, 31)
      get_players(player, numplayers)

      new nName[31]
      if (get_cvar_num("FT_objectives")){
         temp = xpgiven_lev[p_level[id]]
         p_xp[id] +=  temp
         p_gold[id] += GOLD_ESCAPE
         client_print( id,print_chat, "[%s] %s gained %d XP for evading assasination.",
            MOD,sName,temp)
      }

      for (new i=0; i<numplayers; ++i){
         targetid=player[i]

         if (get_user_team(targetid) != TEAM_CT) continue

         get_user_origin( targetid, targetorigin )

         // [09-27-04] Added check for alive status of player - K2mia
         if ( is_user_alive(targetid) &&
              (get_distance(origin, targetorigin)<=get_cvar_num("FT_xp_radius")) &&
              (targetid!=id) )
         {
            if (get_cvar_num("FT_objectives")){
               temp = VIP_ESCAPE_BONUS + xpgiven_lev[p_level[targetid]]

               p_xp[targetid] += temp
               p_gold[targetid] += GOLD_ESCAPE 

               get_user_name( targetid,nName,31)
               client_print( targetid, print_chat,
                  "[%s] %s recieved %d extra XP for helping the VIP escape.", 
                  MOD, nName, temp )
               client_print(targetid, print_chat,
                  "[%s] %s recieved %d GOLD for helping the VIP escape.", 
                  MOD, nName, GOLD_ESCAPE )
            }
         }
         wowmod_hud_playerdata( targetid, 1 )
      }
   }

   wowmod_hud_playerdata( id, HUD_MODE_BASIC )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN freezetimedone and unfreezespeed subroutine
// From Warcraft3FT - Used w/ unholyspeed routine
// **************************************************************************
public freezetimedone() {
   if (!wowmod_enabled)
      return PLUGIN_CONTINUE

   freezetime = 0

   new players[32],numberofplayers, id, i, parm[1]
   get_players(players, numberofplayers)

   for ( i = 0; i < numberofplayers; ++i ){
      id = players[i]

      if ( !is_user_connected(id) )
         return PLUGIN_CONTINUE

      parm[0]=id
      set_user_maxspeed( id, MAXSPEED_DEF )
      set_task( 0.1, "wowmod_cheetah", TASK_ACHEETAH, parm, 1 )

      wowmod_hud_playerdata( id, HUD_MODE_BASIC )
   }
   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN showStatus and hideStatus - Kept from UWC3 and War3FT
// **************************************************************************
public showStatus(id){
   new pid = read_data(2)

   new name[32]
   get_user_name(pid,name,31)
   new color1 = 0,color2 = 0

   if ( get_user_team(pid)==1 )
      color1 = 255
   else
      color2 = 255

   set_hudmessage( color1,50,color2,-1.0,0.60,1, 0.01, 3.0, 0.01, 0.01, 4 )
   show_hudmessage( id, name )

   if ( ( get_user_team(id) == get_user_team(pid) ) && ( get_cvar_num("WOW_classicons") == 1 ) ){
      new parm3[2]
      parm3[0] = id
      parm3[1] = pid 
      set_task( 1.0, "wowmod_sprite_class", TASK_SPRCLASS+id, parm3, 2 )
   }

   //client_print( id, print_center, "SHOWING Status" )
}

public hideStatus(id){
   set_hudmessage( 0,0,0,0.0,0.0,0, 0.0, 0.01, 0.0, 0.0, 4 )
   show_hudmessage( id,"" )
   //client_print( id, print_center, "HIDING Status" )

   wowmod_hud_playerdata( id, HUD_MODE_BASIC )
}


// **************************************************************************
// BEGIN T_win subroutine - Kept from UWC3
// Handler for the T team winning - Rewritten by K2mia 9-22-04
// **************************************************************************
public T_win(){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new CTnum = count_CT()
   new Tnum = count_T()


   if( (Tnum + CTnum ) < get_cvar_num("FT_min_b4_XP") )
      return PLUGIN_HANDLED
   if (!( ( Tnum > 0 ) && ( CTnum > 0 ) ) )
      return PLUGIN_HANDLED

   new a, players[32], numberofplayers
   get_players(players, numberofplayers)

   for (a = 0; a < numberofplayers; ++a){
      new id = players[a]

      new addxp = 0
      new addgold = 0

      if ( get_user_team(id) == TEAM_T ){
         if ( is_user_alive( id ) || ( !get_cvar_num("mp_savexp") )  ){
            addxp = ( ROUNDXP + xpgiven_lev[p_level[players[a]]] )
            addgold = GOLD_WIN
         }else{
            // Only limit bonus xp and gold if in long-term mode
            addxp = ROUNDXP
            addgold = ( GOLD_WIN / 2 )
         }

         if (get_cvar_num("FT_objectives")){
            new sName[32]
            get_user_name(players[a],sName,31)
            client_print( players[a],print_chat,
               "[%s] %s recieved %d XP for winning the round", MOD, sName, addxp )
            client_print( players[a],print_chat,
               "[%s] %s recieved %d GOLD for winning the round", MOD, sName, addgold )
         }

         p_xp[players[a]] += addxp 
         p_gold[players[a]] += addgold 

         wowmod_hud_playerdata( players[a], HUD_MODE_BASIC )
      }
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN CT_win subroutine - Kept from UWC3
// Handler for the T team winning - Rewritten by K2mia 9-22-04
// **************************************************************************
public CT_win(){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE
   
   new CTnum = count_CT()
   new Tnum = count_T()


   if( (Tnum + CTnum ) < get_cvar_num("FT_min_b4_XP") )
      return PLUGIN_HANDLED
   if (!( ( Tnum > 0 ) && ( CTnum > 0 ) ) )
      return PLUGIN_HANDLED

   new a, players[32], numberofplayers
   get_players(players, numberofplayers)

   for (a = 0; a < numberofplayers; ++a){
      new id = players[a]

      new addxp = 0
      new addgold = 0

      if ( get_user_team(id) == TEAM_CT ){
         if ( is_user_alive( id ) ){
            addxp = ( ROUNDXP + xpgiven_lev[p_level[players[a]]] )
            addgold = GOLD_WIN
         }else{
            addxp = ROUNDXP
            addgold = ( GOLD_WIN / 2 )
         }

         if (get_cvar_num("FT_objectives")){
            new sName[32]
            get_user_name(players[a],sName,31)
            client_print( players[a],print_chat,
               "[%s] %s recieved %d XP for winning the round", MOD, sName, addxp )
            client_print( players[a],print_chat,
               "[%s] %s recieved %d GOLD for winning the round", MOD, sName, addgold )
         }

         p_xp[players[a]] += addxp 
         p_gold[players[a]] += addgold 

         wowmod_hud_playerdata( players[a], HUD_MODE_BASIC )
      }
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN setTeam - Player has chosen a team
// **************************************************************************
public setTeam(id){
   //g_friend[id] = read_data(2)
   //log_amx( "Set Team called" )
}


// **************************************************************************
// BEGIN game_commencing subroutine - Kept from UWC3
// The game is beginning
// **************************************************************************
public game_commencing(){
   new players[32], numplayers, id
   get_players(players, numplayers)

   for (new i=0; i<numplayers; i++){
      id = players[i]
      game_commence[id] = true
   }
}


// **************************************************************************
// END WowMOD Event Handlers 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

