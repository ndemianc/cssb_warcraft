/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: XP Module
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
// BEGIN WowMOD XP Handler 
// **************************************************************************


// **************************************************************************
// BEGIN set_xpfrontend subroutine - Kept from UWC3
// Front end to choose the appropriate routine for setting up XP variables
// **************************************************************************
public set_xpfrontend(){
   if ( get_cvar_num("mp_savexp") ){
      set_longtermxp()
      log_amx( "WowMOD XP Table Configured for Saved (Long-term) XP" )
   }else{
      set_shorttermxp()
      log_amx( "WowMOD XP Table Configured for Fast XP" )
   }

   new diff = 0

   for ( new j=0; j<MAX_LEVEL; j++ ){
      if ( j > 0 )
         diff = xplevel_lev[j] - xplevel_lev[j-1]
      log_amx( "Level %d   %d xp needed  %d diff   %d xpgiven", j, xplevel_lev[j], diff, xpgiven_lev[j] )
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN set_shorttermxp subroutine
// Modify some variables based on short term XP status
// **************************************************************************
public set_shorttermxp(){

   if (get_cvar_num("mp_savexp") == 0){
      BOMBPLANTXP /= 3
      DEFUSEXP /= 3
      HOSTAGEXP /= 3
      KILLRESCUEMANXP /= 3
      XPBONUS /= 3
      KILL_BOMB_CARRIER /= 3
      HEADSHOT_BONUS /= 3
      DEFUSER_KILL_BONUS /= 3
      VIP_ESCAPE_BONUS /= 3
      ROUNDXP /= 3
      MENDXP /= 3
      PRAYERXP /= 3
      REMCURSEXP /= 3
      GIFTXP /= 3
      TRANQXP /= 3
      ASSISTXP /= 3
      REZXP /= 3

      GOLD_KILL *= GOLD_FASTXP_MULT 
      GOLD_DMG *= GOLD_FASTXP_MULT 
      GOLD_BOMB *= GOLD_FASTXP_MULT 
      GOLD_DEFUSE *= GOLD_FASTXP_MULT 
      GOLD_SUPPORT *= GOLD_FASTXP_MULT 
      GOLD_WIN *= GOLD_FASTXP_MULT 
      GOLD_ESCAPE *= GOLD_FASTXP_MULT 
      GOLD_KILLVIP *= GOLD_FASTXP_MULT 
      GOLD_ASSIST *= GOLD_FASTXP_MULT 
      GOLD_REZ *= GOLD_FASTXP_MULT 

      for ( new j=0; j<MAX_LEVEL; j++ ){
         // Sets the amount of xp needed for next level
         if ( j <= 20 )
            xplevel_lev[j] = xplevel_base[j]
         else
            xplevel_lev[j] = XP_PER_LEVEL_BASE + ( (j-20) * XP_PER_LEVEL )

         // Sets the amount of xp given for each level
         xpgiven_lev[j] = XP_BASE + ( j * XP_ADD_LEVEL )

         if ( get_cvar_float("mp_xpmultiplier") ){
            new Float:xpmultiplier = get_cvar_float("mp_xpmultiplier")
            xplevel_lev[j] = floatround( xplevel_lev[j] * xpmultiplier )
         }

         // Use additional multipliers for higher levels
         if (j >= 70)
            xplevel_lev[j] = floatround( xplevel_lev[j] * 1.010 )
         else if (j >= 60)
            xplevel_lev[j] = floatround( xplevel_lev[j] * 1.009 )
         else if (j >= 50)
            xplevel_lev[j] = floatround( xplevel_lev[j] * 1.008 )
         else if (j >= 40)
            xplevel_lev[j] = floatround( xplevel_lev[j] * 1.007 )
         else if (j >= 30)
            xplevel_lev[j] = floatround( xplevel_lev[j] * 1.006 )
         else if (j >= 20)
            xplevel_lev[j] = floatround( xplevel_lev[j] * 1.005 )
      }

   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN set_longterm subroutine - Kept from UWC3
// Modify some variables based on long term XP status
// **************************************************************************
public set_longtermxp(){

   if (get_cvar_num("mp_savexp")){
      BOMBPLANTXP /= 10
      DEFUSEXP /= 10
      HOSTAGEXP /= 10
      KILLRESCUEMANXP /= 10
      XPBONUS /= 10
      KILL_BOMB_CARRIER /= 10
      HEADSHOT_BONUS /= 10
      DEFUSER_KILL_BONUS /= 10
      VIP_ESCAPE_BONUS /= 10
      ROUNDXP /= 10
      MENDXP /= 10
      PRAYERXP /= 10
      REMCURSEXP /= 10
      GIFTXP /= 10
      TRANQXP /= 10
      ASSISTXP /= 10
      REZXP /= 10

      for (new j=0; j<MAX_LEVEL; j++){
         // Sets the amount of xp needed for next level
         if (j <= 20)
            xplevel_lev[j] = xplevel_base_LT[j]
         else
            //xplevel_lev[j] = XP_PER_LEVEL_BASE_LT + ( (j-20) * XP_PER_LEVEL_LT ) +
            //   floatround( float( j * XP_PER_LEVEL_LT ) / 2.0 )
            xplevel_lev[j] = xplevel_lev[j-1] + XP_PER_LEVEL_LT 

         // Sets the amount of xp given for each level
         xpgiven_lev[j] = ( XP_BASE + (j * XP_ADD_LEVEL) ) / 4

         if (get_cvar_float("mp_xpmultiplier")){
            new Float:xpmultiplier = get_cvar_float("mp_xpmultiplier")
            xplevel_lev[j] = floatround(xplevel_lev[j] * xpmultiplier)

         }

         // Use additional multipliers for higher levels
         if (j >= 70)
            xplevel_lev[j] = floatround( xplevel_lev[j] * 1.010 )
         else if (j >= 60)
            xplevel_lev[j] = floatround( xplevel_lev[j] * 1.009 )
         else if (j >= 50)
            xplevel_lev[j] = floatround( xplevel_lev[j] * 1.008 )
         else if (j >= 40)
            xplevel_lev[j] = floatround( xplevel_lev[j] * 1.007 )
         else if (j >= 30)
            xplevel_lev[j] = floatround( xplevel_lev[j] * 1.006 )
         else if (j >= 20)
            xplevel_lev[j] = floatround( xplevel_lev[j] * 1.005 )

      }

   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN amx_givexp subroutine - Kept from UWC3
// Server command for issuing xp to a team or person
// **************************************************************************
public amx_givexp( id, level ){
   if ( !(get_user_flags(id) & get_cvar_num("WOW_admin_flag")) ) {
      if( id != 0 ){
         client_print( id,print_console,"[%s] You have no access to that command", MOD )
         return PLUGIN_HANDLED
      }
   }

   if ( get_cvar_num("WOW_disable_givexp") ){
      client_print( id, print_console, "[%s] This command has been disabled.", MOD )
      return PLUGIN_HANDLED
   }


   new arg[32], xp[10]
   read_argv(1,arg,31)
   read_argv(2,xp,9)


   // [9-30-04] Added logging of amx_givexp() command - K2mia
   new name[32]
   get_user_name(id, name, 31)
   log_amx( "[%s] %s issued amx_givexp() %s %s", MOD, name, arg, xp )


   if(equali(arg,"@ALL")){
      new players[32], inum
      get_players(players,inum)
      for(new a=0;a<inum;++a){
         client_print(players[a], print_chat,
            "[%s] The admin gave you %d experience",MOD, str_to_num(xp))
         p_xp[players[a]]+=str_to_num(xp)
         wowmod_hud_playerdata( players[a], 1 )
      }
      return PLUGIN_HANDLED
   }

   if (arg[0]=='@'){
      new players[32], inum
      get_players( players,inum,"e",arg[1] )
      if (inum==0){
         console_print(id,"No clients in such team")
         return PLUGIN_HANDLED
      }

      for(new a=0;a<inum;++a){
         client_print(players[a], print_chat,
            "[%s] The admin gave you %d experience",MOD, str_to_num(xp))
         p_xp[players[a]] += str_to_num(xp)
         wowmod_hud_playerdata( players[a], 1 )
      }
   }else{
      new player = cmd_target2(id,arg)
      if (!player) return PLUGIN_HANDLED
      client_print(player,print_chat,"[%s] The admin gave you %d experience",MOD, str_to_num(xp))
      p_xp[player] += str_to_num(xp)
      wowmod_hud_playerdata( player, 1 )
   }

   return PLUGIN_HANDLED
}

// **************************************************************************
// END WowMOD XP Handler 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

