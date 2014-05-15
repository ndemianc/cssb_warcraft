/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Menus and Menu Handlers
*** Date:          Aug 10, 2007
*** Last Update:   Sep 5, 2007
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
// BEGIN WowMOD Menu System 
// **************************************************************************


// **************************************************************************
// BEGIN wowmod_do_ subroutine (TEMPLATE)
// Handler for the * menu 
// **************************************************************************
public wowmod_do_(id,key){
}


// **************************************************************************
// BEGIN wowmod_do_autocastmenu subroutine
// The WowMOD autocast menu handler
// **************************************************************************
public wowmod_do_autocastmenu( id, key ){

   new page = autocastpage[id]
   new bool:tog_ok = false
   new class = p_class1[id]

   if ( key == 9 )
      return PLUGIN_HANDLED

   if ( key == 8 ){
      if ( ( page == 1 ) && ( p_class2[id] >= CLASS_DRUID ) )
         wowmod_display_autocastmenu( id, 2 )
      else
         wowmod_display_autocastmenu( id, 1 )
      return PLUGIN_CONTINUE
   }

   if ( page == 2 )
      class = p_class2[id]

   switch( class ){
      case CLASS_DRUID:{
         if ( !( equali( druid_saycmds[key], "na" ) ) ){
            tog_ok = true
            client_print( id, print_chat, "[%s] Autocast setting changed for spell '%s'", 
                           MOD, druid_spells[key] )
         }else{
            tog_ok = false
            client_print( id, print_chat, "[%s] Autocast is not applicable for spell '%s'", 
                           MOD, druid_spells[key] )
         }
      }
      case CLASS_HUNTER:{
         if ( !( equali( hunter_saycmds[key], "na" ) ) ){
            tog_ok = true
            client_print( id, print_chat, "[%s] Autocast setting changed for spell '%s'", 
                           MOD, hunter_spells[key] )
         }else{
            tog_ok = false
            client_print( id, print_chat, "[%s] Autocast is not applicable for spell '%s'", 
                           MOD, hunter_spells[key] )
         }
      }
      case CLASS_MAGE:{
         if ( !( equali( mage_saycmds[key], "na" ) ) ){
            tog_ok = true
            client_print( id, print_chat, "[%s] Autocast setting changed for spell '%s'", 
                           MOD, mage_spells[key] )
         }else{
            tog_ok = false
            client_print( id, print_chat, "[%s] Autocast is not applicable for spell '%s'", 
                           MOD, mage_spells[key] )
         }
      }
      case CLASS_ROGUE:{
         if ( !( equali( rogue_saycmds[key], "na" ) ) ){
            tog_ok = true
            client_print( id, print_chat, "[%s] Autocast setting changed for spell '%s'", 
                           MOD, rogue_spells[key] )
         }else{
            tog_ok = false
            client_print( id, print_chat, "[%s] Autocast is not applicable for spell '%s'", 
                           MOD, rogue_spells[key] )
         }
      }
      case CLASS_WARLOCK:{
         if ( !( equali( warlock_saycmds[key], "na" ) ) ){
            tog_ok = true
            client_print( id, print_chat, "[%s] Autocast setting changed for spell '%s'", 
                           MOD, warlock_spells[key] )
         }else{
            tog_ok = false
            client_print( id, print_chat, "[%s] Autocast is not applicable for spell '%s'", 
                           MOD, warlock_spells[key] )
         }
      }
      case CLASS_PRIEST:{
         if ( !( equali( priest_saycmds[key], "na" ) ) ){
            tog_ok = true
            client_print( id, print_chat, "[%s] Autocast setting changed for spell '%s'", 
                           MOD, priest_spells[key] )
         }else{
            tog_ok = false
            client_print( id, print_chat, "[%s] Autocast is not applicable for spell '%s'", 
                           MOD, priest_spells[key] )
         }
      }
   }

   if ( tog_ok ){
      if ( page == 1 ){
         if ( p_class1autocast[id][key] )
            p_class1autocast[id][key] = false
         else
            p_class1autocast[id][key] = true 
      }else{
         if ( p_class2autocast[id][key] )
            p_class2autocast[id][key] = false
         else
            p_class2autocast[id][key] = true 
      }
   }

   wowmod_display_autocastmenu( id, autocastpage[id] )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_do_sellequipmenu subroutine
// The WowMOD sell equip menu handler
// **************************************************************************
public wowmod_do_sellequipmenu( id, key ){

   new page = sellequipmentpage[id]
   new gidx = 0, eq_idx = 0, gold = 0 

   if ( key == 8 ){
      new npage = ( page + 1 )
      wowmod_display_sellequipmenu( id, npage )
      return PLUGIN_CONTINUE
   }else if ( key == 9 ){
      return PLUGIN_HANDLED
   }

   eq_idx = 10 + ( (page-1) * 8 ) + key
   gidx = p_inventory[id][eq_idx]

   if ( gidx <= 0 ){
      client_print( id, print_chat, "[%s] That inventory slot is empty", MOD )
   }else{
      gold = floatround( g_cost[gidx] / SELL_EQUIP_DIV )
      p_gold[id] += gold
      p_inventory[id][eq_idx] = 0
      client_print( id, print_chat, "[%s] You have sold '%s' for %d gold", MOD, gear[gidx], gold )
   }

   wowmod_display_sellequipmenu( id, page )
   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_do_sellspellmenu subroutine
// The WowMOD sell spell menu handler
// **************************************************************************
public wowmod_do_sellspellmenu( id, key ){

   new page = sellspellpage[id]
   new sidx = 0, spell_index = 0, class = -1, gold = 0 
   new spell[64]

   if ( key == 8 ){
      new npage = ( page == 1 ) ? 2 : 1
      wowmod_display_sellspellmenu( id, npage )
      return PLUGIN_CONTINUE
   }else if ( key == 9 ){
      return PLUGIN_HANDLED
   }

   // Only options left are for spell sellbacks 

   if ( page == 2 ){
      if ( p_class2[id] < 0 ){
         client_print( id, print_chat, "[%s] You do not have a secondary class", MOD )
         return PLUGIN_HANDLED
      }

      class = p_class2[id]

      switch( p_class2[id] ){
         case CLASS_DRUID: sidx = SPELL_DRUID_BASE_IDX 
         case CLASS_HUNTER: sidx = SPELL_HUNTER_BASE_IDX
         case CLASS_MAGE: sidx = SPELL_MAGE_BASE_IDX
         case CLASS_ROGUE: sidx = SPELL_ROGUE_BASE_IDX
         case CLASS_WARLOCK: sidx = SPELL_WARLOCK_BASE_IDX
         case CLASS_PRIEST: sidx = SPELL_PRIEST_BASE_IDX
      }
   }else{
      class = p_class1[id]

      switch( p_class1[id] ){
         case CLASS_DRUID: sidx = SPELL_DRUID_BASE_IDX 
         case CLASS_HUNTER: sidx = SPELL_HUNTER_BASE_IDX
         case CLASS_MAGE: sidx = SPELL_MAGE_BASE_IDX
         case CLASS_ROGUE: sidx = SPELL_ROGUE_BASE_IDX
         case CLASS_WARLOCK: sidx = SPELL_WARLOCK_BASE_IDX
         case CLASS_PRIEST: sidx = SPELL_PRIEST_BASE_IDX
      }
   }

   if ( ( key >= 0 ) && ( key < 8 ) ){
      spell_index = sidx + key 

      switch( class ){
         case CLASS_DRUID: {
            copy( spell, 32, druid_spells[key] ) 
            gold = floatround( druid_spgold[key] / SELL_SPELL_DIV )
         }
         case CLASS_HUNTER: {
            copy( spell, 32, hunter_spells[key] ) 
            gold = floatround( hunter_spgold[key] / SELL_SPELL_DIV )
         }
         case CLASS_MAGE: {
            copy( spell, 32, mage_spells[key] )
            gold = floatround( mage_spgold[key] / SELL_SPELL_DIV )
         }
         case CLASS_ROGUE: {
            copy( spell, 32, rogue_spells[key] )
            gold = floatround( rogue_spgold[key] / SELL_SPELL_DIV )
         }
         case CLASS_WARLOCK: {
            copy( spell, 32, warlock_spells[key] )
            gold = floatround( warlock_spgold[key] / SELL_SPELL_DIV )
         }
         case CLASS_PRIEST: {
            copy( spell, 32, priest_spells[key] )
            gold = floatround( priest_spgold[key] / SELL_SPELL_DIV )
         }
      }

      if ( p_spells[id][spell_index] > 1 ){
         client_print( id, print_chat, "[%s] You have sold back one rank of the spell '%s' for %d gold", 
            MOD, spell, gold )
         p_spells[id][spell_index] -= 1
         p_gold[id] += gold
      }else if ( p_spells[id][spell_index] == 1 ){
         client_print( id, print_chat, 
            "[%s] You have sold back your final rank and unlearned the spell '%s' for %d gold", 
            MOD, spell, gold )
         p_spells[id][spell_index] = 0
         p_gold[id] += gold
      }else{
         client_print( id, print_chat, "[%s] You do not know the spell %s", MOD, spell )
      }
   }

   wowmod_display_sellspellmenu( id, page )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_do_changeconf subroutine
// Handle the race/class change confirmation screen
// **************************************************************************
public wowmod_do_changeconf( id, key ){

   if ( change_conf[id] ){
      return PLUGIN_CONTINUE
   }

   if ( key == 0 ){
      change_conf[id] = true
      client_print( id, print_chat, "[%s] You have confirmed that you wish to change your race/class", MOD )
      wowmod_display_racemenu( id )
   }else if ( key == 1 ){
      client_print( id, print_chat, "[%s] You have chosen to keep your race/class", MOD )
   }else if ( key == 8 ){
      wowmod_display_mainmenu( id )
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_do_changeconf2 subroutine
// Handle the secondary class change confirmation screen
// **************************************************************************
public wowmod_do_changeconf2( id, key ){

   if ( change_conf[id] ){
      return PLUGIN_CONTINUE
   }

   if ( key == 0 ){
      change_conf[id] = true
      client_print( id, print_chat, "[%s] You have confirmed that you wish to change your secondary class", 
         MOD )
      wowmod_display_class2menu( id )
   }else if ( key == 1 ){
      client_print( id, print_chat, "[%s] You have chosen to keep your chosen secondary class", MOD )
   }else if ( key == 8 ){
      wowmod_display_mainmenu( id )
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_do_allbadges subroutine 
// Handler for the show all badges menu
// **************************************************************************
public wowmod_do_allbadges( id, key ){
   switch( key ){
      case 0: wowmod_info_allbadges( id, 1 )
      case 1: wowmod_info_allbadges( id, 2 )
      case 2: wowmod_info_allbadges( id, 3 )
      case 3: wowmod_info_allbadges( id, 4 )
      case 4: wowmod_info_allbadges( id, 5 )
      case 5: wowmod_info_allbadges( id, 6 )
      case 6: wowmod_info_allbadges( id, 7 )
      case 7: wowmod_info_allbadges( id, 1 )
      case 8: wowmod_display_mainmenu( id )
   }
   if ( key < 8 )
      wowmod_display_allbadges( id )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_do_resurrect subroutine 
// Handler for the Priest Resurrection menu
// **************************************************************************
public wowmod_do_resurrect( id, key ){
   new page = respage[id]

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cannot cast resurrect!", MOD )
      return PLUGIN_HANDLED
   }

   if ( key == 8 ){
      if ( page == 1 ){
         respage[id] = 2
         wowmod_display_resurrect( id, 2 ) 
      }else{
         respage[id] = 1
         wowmod_display_resurrect( id, 1 ) 
      }
      return PLUGIN_HANDLED
   }else if ( key == 9 ){
      return PLUGIN_HANDLED
   }

   new spell_index = SPELL_PRIEST_BASE_IDX + SP_RESURRECT
   new spell_level = p_spells[id][spell_index]
   new cast_mana = priest_spmana[SP_RESURRECT] + ( SPELL_MANA_RANK2 * spell_level )
   if ( p_mana[id] < cast_mana ){
      client_print(id, print_chat,
         "[%s] You do not have enough mana to cast Resurrection", MOD )
      return PLUGIN_HANDLED
   }

   new name[32]
   get_user_name( id, name, 31 )

   new friend = reslist[id][key]
   new fname[32]
   get_user_name( friend, fname, 31 )

   if ( is_user_alive(friend) ){
      client_print( id, print_chat, "[%s] %s seems to be very much alive!", MOD, fname )
      return PLUGIN_HANDLED
   }

   p_mana[id] -= cast_mana

   new xpbonus = REZXP + ( ( REZXP * spell_level ) / 4 )
   p_xp[id] += xpbonus
   p_gold[id] += GOLD_REZ

   p_rezzes[id] += 1
   p_rezzes_s[id] += 1

   user_spawn( friend )

   new smod = priest_spmax[SP_RESURRECT] - spell_level + 1
   new mana = p_mana[friend] / smod
   new health = REZ_HEALTH * spell_level
   if ( spell_level == priest_spmax[SP_RESURRECT] )
      health = p_maxhealth[friend]

   //client_print( id, print_chat, "resurrect> health=(%d) mana=(%d) smod=(%d)", 
   //   health, mana, smod )

   new parm[4]
   parm[0] = friend
   parm[1] = health
   parm[2] = mana
   set_task( 0.3, "spawn_player", 2, parm, 2 )
   set_task( 0.5, "spawn_player_fix", 2, parm, 4 )

   client_print( id, print_chat, "[%s] You have Resurrected %s, XP and Gold bonuses earned", MOD, fname )
   client_print( friend, print_chat, "[%s] You have been Resurrected by %s", MOD, name )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_adminmenu subroutine - Kept from UWC3
// Handler for the * menu 
// **************************************************************************
public wowmod_do_adminmenu(id,key){
   switch (key){
      case 0:{
         g_menuOption[id] = 1
         g_menuSettings[id] = 100
         wowmod_display_playerxpmenu( id, g_menuPosition[id] = 0 )
      }
      case 1:{
         g_menuOption[id] = 1
         g_menuSettings[id] = 100
         wowmod_display_teamxpmenu(id)
      }
      case 2:{
         g_menuOption[id] = 1
         g_menuSettings[id] = 100
         wowmod_display_playergoldmenu( id, g_menuPosition[id] = 0 )
      }
      case 8: wowmod_display_mainmenu(id)
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_playerxpmenu subroutine - Kept from UWC3
// Handler for the playerxp_menu 
// **************************************************************************
public wowmod_do_playerxpmenu( id,key ){

   switch(key){
      case 7:{
         ++g_menuOption[id]
         if (g_menuOption[id]>6){
            g_menuOption[id]=1
         }
         switch(g_menuOption[id]){
            case 1: g_menuSettings[id] = 100
            case 2: g_menuSettings[id] = 500
            case 3: g_menuSettings[id] = 1000
            case 4: g_menuSettings[id] = 5000
            case 5: g_menuSettings[id] = 10000
            case 6: g_menuSettings[id] = 25000
         }
         wowmod_display_playerxpmenu( id,g_menuPosition[id] )
      }
      case 8: wowmod_display_playerxpmenu( id,++g_menuPosition[id] )
      case 9: wowmod_display_playerxpmenu( id,--g_menuPosition[id] )
      default:{
         new player = g_menuPlayers[id][g_menuPosition[id] * 7 + key]

         new name[32]
         new pname[32]
         get_user_name(id, name, 31)
         get_user_name(player, pname, 31)

         //log_amx( "[%s] %s used Admin-Menu XP [%s] for %s",
         //         MOD, name, g_menuSettings[id], pname )

         client_print( player, print_chat,
            "[%s] The admin just awarded you %d experience.", MOD, g_menuSettings[id] )

         p_xp[player] += g_menuSettings[id]
         wowmod_hud_playerdata( player, 1 )
         wowmod_display_playerxpmenu( id,g_menuPosition[id] )
      }
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_playergoldmenu subroutine
// Handler for the playergold_menu 
// **************************************************************************
public wowmod_do_playergoldmenu( id,key ){

   switch(key){
      case 7:{
         ++g_menuOption[id]
         if (g_menuOption[id]>6){
            g_menuOption[id]=1
         }
         switch(g_menuOption[id]){
            case 1: g_menuSettings[id] = 100
            case 2: g_menuSettings[id] = 500
            case 3: g_menuSettings[id] = 1000
            case 4: g_menuSettings[id] = 5000
            case 5: g_menuSettings[id] = 10000
            case 6: g_menuSettings[id] = 25000
         }
         wowmod_display_playergoldmenu( id,g_menuPosition[id] )
      }
      case 8: wowmod_display_playergoldmenu( id,++g_menuPosition[id] )
      case 9: wowmod_display_playergoldmenu( id,--g_menuPosition[id] )
      default:{
         new player = g_menuPlayers[id][g_menuPosition[id] * 7 + key]

         new name[32]
         new pname[32]
         get_user_name(id, name, 31)
         get_user_name(player, pname, 31)

         //log_amx( "[%s] %s used Admin-Menu Gold [%s] for %s",
         //         MOD, name, g_menuSettings[id], pname )

         client_print( player, print_chat,
            "[%s] The admin just awarded you %d Gold.", MOD, g_menuSettings[id] )

         p_gold[player] += g_menuSettings[id]
         wowmod_hud_playerdata( player, 1 )
         wowmod_display_playergoldmenu( id,g_menuPosition[id] )
      }
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN do_teamxpmenu subroutine
// Handler for the teamxp_menu for the WC3FT-Unlimited plugin
// **************************************************************************
public wowmod_do_teamxpmenu(id,key){

   // [10-08-04] Added logging of amx_givexp() command - K2mia
   new name[32]
   get_user_name(id, name, 31)

   switch(key){
      case 0:{
         //log_amx( "[%s] %s used Admin-Menu XP [%d] for team Terrorist",
         //         MOD, name, g_menuSettings[id] )
         server_cmd( "amx_givexp @TERRORIST %d", g_menuSettings[id] )
         wowmod_display_teamxpmenu( id )
      }
      case 1:{
         //log_amx( "[%s] %s used Admin-Menu XP [%d] for team CT",
         //         MOD, name, g_menuSettings[id] )
         server_cmd( "amx_givexp @CT %d",g_menuSettings[id] )
         wowmod_display_teamxpmenu( id )
      }
      case 2:{
         //log_amx( "[%s] %s used Admin-Menu XP [%d] for EVERYONE",
         //         MOD, name, g_menuSettings[id] )
         server_cmd( "amx_givexp @ALL %d",g_menuSettings[id] )
         wowmod_display_teamxpmenu( id )
      }
      case 7:{
         ++g_menuOption[id]
         if (g_menuOption[id]>6){
            g_menuOption[id]=1
         }
         switch(g_menuOption[id]){
            case 1: g_menuSettings[id] = 100
            case 2: g_menuSettings[id] = 500
            case 3: g_menuSettings[id] = 1000
            case 4: g_menuSettings[id] = 5000
            case 5: g_menuSettings[id] = 10000
            case 6: g_menuSettings[id] = 25000
         }
         wowmod_display_teamxpmenu( id )
      }
      case 8: wowmod_display_adminmenu(id)
      case 9: return PLUGIN_HANDLED
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_mainmenu subroutine
// Handler for the * menu 
// **************************************************************************
public wowmod_do_mainmenu(id,key){
   //menu_items[0]= "Welcome to WowMOD"
   //menu_items[1]= "Race Selection"
   //menu_items[2]= "Secondary Class Selection"
   //menu_items[3]= "Spell Merchant"
   //menu_items[4]= "Merchants Menu"
   //menu_items[5]= "Character Sheet"
   //menu_items[6]= "Your Known Spells"
   //menu_items[7]= "Help"

   if ( client_ready_mainmenu[id] )
      client_ready_mainmenu[id] = false

   switch (key){
      case 0: wowmod_info_wowmod( id )
      case 1: wowmod_display_racemenu( id )
      case 2: if ( p_level[id] >= DUAL_CLASS_LEVEL ) 
                 wowmod_display_class2menu( id )
              else
                 client_print( id, print_chat, 
                    "[%s] You have not reached the required level for dual-class selection!", MOD )
      case 3: wowmod_display_spellmerchant( id )
      case 4: wowmod_display_merchantmenu( id )
      case 5: wowmod_info_charsheet( id )
      case 6: wowmod_info_charsheet2( id )
      case 7: wowmod_info_helpmain( id )
      case 8:{
         if ( !get_cvar_num( "WOW_disable_adminmenu") &&
              ( get_user_flags(id) & get_cvar_num("WOW_admin_flag") ) )
         {
            wowmod_display_adminmenu( id )
         }else{
            wowmod_info_credits( id )
         }
      }
      //case 8: wowmod_display_mainmenu2( id )
      default: return PLUGIN_HANDLED
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_mainmenu2 subroutine
// Handler for the * menu page 2
// **************************************************************************
public wowmod_do_mainmenu2(id,key){
   //menu_items[0]= "Character Sheet"
   //menu_items[1]= "Reset"
   //menu_items[2]= "HUD Resolution"

   if ( client_ready_mainmenu[id] )
      client_ready_mainmenu[id] = false

   switch (key){
      case 0: client_print( id, print_center, "Not yet implemented" )
      case 1: client_print( id, print_center, "Not yet implemented" ) 
      case 2: wowmod_info_credits( id )
      case 3: client_print( id, print_center, "Not yet implemented" ) 
      case 8: wowmod_display_mainmenu( id )
      default: return PLUGIN_HANDLED
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_racemenu subroutine
// Handler for the race selection menu 
// **************************************************************************
public wowmod_do_racemenu(id,key){
   switch (key){
      case 0: wowmod_race_selected( id, RACE_NIGHTELF ) 
      case 1: wowmod_race_selected( id, RACE_GNOME )  
      case 2: wowmod_race_selected( id, RACE_DWARF )
      case 3: wowmod_race_selected( id, RACE_HUMAN ) 
      case 4: wowmod_race_selected( id, RACE_ORC ) 
      case 5: wowmod_race_selected( id, RACE_TAUREN ) 
      case 6: wowmod_race_selected( id, RACE_UNDEAD ) 
      case 7: wowmod_race_selected( id, RACE_BLOODELF ) 
      case 8: wowmod_display_mainmenu( id )
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_class2menu subroutine
// Handler for the secondary class selection menu 
// **************************************************************************
public wowmod_do_class2menu( id,key ){
   new class2 = -1

   if ( key == 9 ){
      return PLUGIN_HANDLED
   }

   if ( p_race[id] == RACE_NIGHTELF ){
      switch (key){
         case 0: class2 = CLASS_DRUID
         case 1: class2 = CLASS_HUNTER
         case 2: class2 = CLASS_ROGUE
         case 3: class2 = CLASS_PRIEST
         case 8: {
            wowmod_display_mainmenu( id )
            return PLUGIN_CONTINUE
         }
         default: class2 = -1
      }
   }else if ( p_race[id] == RACE_GNOME ){
      switch (key){
         case 0: class2 = CLASS_MAGE
         case 1: class2 = CLASS_ROGUE
         case 2: class2 = CLASS_WARLOCK
         case 8: {
            wowmod_display_mainmenu( id )
            return PLUGIN_CONTINUE
         }
         default: class2 = -1
      }
   }else if ( p_race[id] == RACE_DWARF ){
      switch (key){
         case 0: class2 = CLASS_HUNTER
         case 1: class2 = CLASS_ROGUE
         case 2: class2 = CLASS_PRIEST
         case 8: {
            wowmod_display_mainmenu( id )
            return PLUGIN_CONTINUE
         }
         default: class2 = -1
      }
   }else if ( p_race[id] == RACE_HUMAN ){
      switch (key){
         case 0: class2 = CLASS_MAGE
         case 1: class2 = CLASS_ROGUE
         case 2: class2 = CLASS_WARLOCK
         case 3: class2 = CLASS_PRIEST
         case 8: {
            wowmod_display_mainmenu( id )
            return PLUGIN_CONTINUE
         }
         default: class2 = -1
      }
   }else if ( p_race[id] == RACE_ORC ){
      switch (key){
         case 0: class2 = CLASS_HUNTER
         case 1: class2 = CLASS_ROGUE
         case 2: class2 = CLASS_WARLOCK
         case 8: {
            wowmod_display_mainmenu( id )
            return PLUGIN_CONTINUE
         }
         default: class2 = -1
      }
   }else if ( p_race[id] == RACE_TAUREN ){
      switch (key){
         case 0: class2 = CLASS_DRUID
         case 1: class2 = CLASS_HUNTER
         case 8: {
            wowmod_display_mainmenu( id )
            return PLUGIN_CONTINUE
         }
         default: class2 = -1
      }
   }else if ( p_race[id] == RACE_UNDEAD ){
      switch (key){
         case 0: class2 = CLASS_MAGE
         case 1: class2 = CLASS_ROGUE
         case 2: class2 = CLASS_WARLOCK
         case 3: class2 = CLASS_PRIEST
         case 8: {
            wowmod_display_mainmenu( id )
            return PLUGIN_CONTINUE
         }
         default: class2 = -1
      }
   }else if ( p_race[id] == RACE_BLOODELF ){
      switch (key){
         case 0: class2 = CLASS_HUNTER
         case 1: class2 = CLASS_MAGE
         case 2: class2 = CLASS_ROGUE
         case 3: class2 = CLASS_WARLOCK
         case 4: class2 = CLASS_PRIEST
         case 8: {
            wowmod_display_mainmenu( id )
            return PLUGIN_CONTINUE
         }
         default: class2 = -1
      }
   }

   if ( class2 == -1 ){
      client_print( id, print_chat, "[%s] Invalid secondary class selection!", MOD )
      return PLUGIN_HANDLED
   }

   // Reset spells from secondary class
   new startidx = -1
   new endidx = -1

   if ( p_class2[id] > -1 ){
      switch ( p_class2[id] ){
         case CLASS_DRUID: startidx = SPELL_DRUID_BASE_IDX
         case CLASS_HUNTER: startidx = SPELL_HUNTER_BASE_IDX
         case CLASS_MAGE: startidx = SPELL_HUNTER_BASE_IDX
         case CLASS_ROGUE: startidx = SPELL_ROGUE_BASE_IDX
         case CLASS_WARLOCK: startidx = SPELL_WARLOCK_BASE_IDX
         case CLASS_PRIEST: startidx = SPELL_PRIEST_BASE_IDX
      }

      endidx = startidx + 8

      for( new j = startidx; j < endidx; j++ ){
         // Only reset spells from secondary class selection
         p_spells[id][j] = 0
      }
   }

   p_class2[id] = class2

   client_print( id, print_chat, "[%s] You have selected your secondary class to be '%s'",
      MOD, classes[p_class2[id]] )

   wowmod_display_mainmenu( id )

   return PLUGIN_CONTINUE
}



// **************************************************************************
// BEGIN wowmod_do_nelf_classmenu subroutine
// Handler for the Nightelf class selection menu 
// **************************************************************************
public wowmod_do_nelf_classmenu(id,key){
   switch (key){
      case 0: wowmod_class_selected( id, CLASS_DRUID ) 
      case 1: wowmod_class_selected( id, CLASS_HUNTER )  
      case 2: wowmod_class_selected( id, CLASS_ROGUE )
      case 3: wowmod_class_selected( id, CLASS_PRIEST )
      case 8: wowmod_display_mainmenu( id )
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_gnome_classmenu subroutine
// Handler for the Gnome class selection menu 
// **************************************************************************
public wowmod_do_gnome_classmenu(id,key){
   switch (key){
      case 0: wowmod_class_selected( id, CLASS_MAGE ) 
      case 1: wowmod_class_selected( id, CLASS_ROGUE )  
      case 2: wowmod_class_selected( id, CLASS_WARLOCK )  
      case 8: wowmod_display_mainmenu( id )
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_dwarf_classmenu subroutine
// Handler for the Dwarf class selection menu 
// **************************************************************************
public wowmod_do_dwarf_classmenu(id,key){
   switch (key){
      case 0: wowmod_class_selected( id, CLASS_HUNTER ) 
      case 1: wowmod_class_selected( id, CLASS_ROGUE )  
      case 2: wowmod_class_selected( id, CLASS_PRIEST )  
      case 8: wowmod_display_mainmenu( id )
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_human_classmenu subroutine
// Handler for the Human class selection menu 
// **************************************************************************
public wowmod_do_human_classmenu(id,key){
   switch (key){
      case 0: wowmod_class_selected( id, CLASS_MAGE ) 
      case 1: wowmod_class_selected( id, CLASS_ROGUE )  
      case 2: wowmod_class_selected( id, CLASS_WARLOCK )  
      case 3: wowmod_class_selected( id, CLASS_PRIEST )  
      case 8: wowmod_display_mainmenu( id )
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_orc_classmenu subroutine
// Handler for the Orc class selection menu 
// **************************************************************************
public wowmod_do_orc_classmenu(id,key){
   switch (key){
      case 0: wowmod_class_selected( id, CLASS_HUNTER ) 
      case 1: wowmod_class_selected( id, CLASS_ROGUE )  
      case 2: wowmod_class_selected( id, CLASS_WARLOCK )  
      case 8: wowmod_display_mainmenu( id )
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_tauren_classmenu subroutine
// Handler for the Tauren class selection menu 
// **************************************************************************
public wowmod_do_tauren_classmenu(id,key){
   switch (key){
      case 0: wowmod_class_selected( id, CLASS_DRUID ) 
      case 1: wowmod_class_selected( id, CLASS_HUNTER )  
      case 8: wowmod_display_mainmenu( id )
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_undead_classmenu subroutine
// Handler for the Undead class selection menu 
// **************************************************************************
public wowmod_do_undead_classmenu(id,key){
   switch (key){
      case 0: wowmod_class_selected( id, CLASS_MAGE ) 
      case 1: wowmod_class_selected( id, CLASS_ROGUE )  
      case 2: wowmod_class_selected( id, CLASS_WARLOCK )  
      case 3: wowmod_class_selected( id, CLASS_PRIEST )  
      case 8: wowmod_display_mainmenu( id )
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_belf_classmenu subroutine
// Handler for the Bloodelf class selection menu 
// **************************************************************************
public wowmod_do_belf_classmenu(id,key){
   switch (key){
      case 0: wowmod_class_selected( id, CLASS_HUNTER ) 
      case 1: wowmod_class_selected( id, CLASS_MAGE )  
      case 2: wowmod_class_selected( id, CLASS_ROGUE )  
      case 3: wowmod_class_selected( id, CLASS_WARLOCK )  
      case 4: wowmod_class_selected( id, CLASS_PRIEST )  
      case 8: wowmod_display_mainmenu( id )
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_adminmenu subroutine
// The WowMOD admin menu
// **************************************************************************
public wowmod_display_adminmenu(id){

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   if ( !( get_user_flags(id) & get_cvar_num("WOW_admin_flag") ) ){ 
      if( id != 0 ){
         client_print( id, print_center," You have no access to that command" )
         return PLUGIN_HANDLED
      }
   }

   if ( get_cvar_num("WOW_disable_adminmenu") ){
      client_print( id, print_center, "The Admin Menu has been disabled on this server" )
      return PLUGIN_HANDLED
   }

   new pos = 0, i, menu_body[512], menu_items[3][32]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<8)|(1<<9)
   //new keys = (1<<0)|(1<<1)|(1<<8)|(1<<9)

   menu_items[0]= "Give Individual Players XP"
   menu_items[1]= "Give Multiple Players XP"
   menu_items[2]= "Give Individual Players Gold"
   /*menu_items[2]= "Save everyone's XP"*/

   pos += format(menu_body[pos], 511-pos, "\yAdmin Menu^n^n")
   for (i = 0; i<3; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_HANDLED

}


// **************************************************************************
// BEGIN wowmod_display_playerxpmenu subroutine - Kept from UWC3
// Display the playerxp_menu 
// **************************************************************************
public wowmod_display_playerxpmenu( id,pos ){
   if ( pos < 0 ){
      wowmod_display_adminmenu(id)
      return PLUGIN_CONTINUE
   }

   get_players( g_menuPlayers[id], g_menuPlayersNum[id] )
   new menuBody[512]
   new b = 0
   new i
   new name[32], team[4]
   new start = pos * 7

   if ( start >= g_menuPlayersNum[id] )
      start = pos = g_menuPosition[id] = 0

   new len = format(menuBody,511, "\yGive Players XP\R%d/%d^n\w^n",pos+1,(g_menuPlayersNum[id] / 7 + ((g_menuPlayersNum[id] % 7) ? 1 : 0 )))
   new end = start + 7
   new keys = (1<<9)|(1<<7)

   if (end > g_menuPlayersNum[id])
      end = g_menuPlayersNum[id]

   for(new a = start; a < end; ++a){
      i = g_menuPlayers[id][a]
      get_user_name(i,name,31)
      get_user_team(i,team,3)
      keys |= (1<<b)
      len += format( menuBody[len],511-len,"\w%d. %s^n\w",++b,name )
   }

   len += format( menuBody[len],511-len,"^n8. Give  %d XP^n",g_menuSettings[id] )

   if (end != g_menuPlayersNum[id]){
      format( menuBody[len],511-len,"^n9. More...^n0. %s", pos ? "Back" : "Back" )
      keys |= (1<<8)
   }else
      format( menuBody[len],511-len,"^n0. %s", pos ? "Back" : "Exit" )

   show_menu( id,keys,menuBody )

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_display_playergoldmenu subroutine
// Display the player gold_menu 
// **************************************************************************
public wowmod_display_playergoldmenu( id,pos ){
   if ( pos < 0 ){
      wowmod_display_adminmenu(id)
      return PLUGIN_CONTINUE
   }

   get_players( g_menuPlayers[id], g_menuPlayersNum[id] )
   new menuBody[512]
   new b = 0
   new i
   new name[32], team[4]
   new start = pos * 7

   if ( start >= g_menuPlayersNum[id] )
      start = pos = g_menuPosition[id] = 0

   new len = format(menuBody,511, "\yAward Players Gold\R%d/%d^n\w^n",pos+1,(g_menuPlayersNum[id] / 7 + ((g_menuPlayersNum[id] % 7) ? 1 : 0 )))
   new end = start + 7
   new keys = (1<<9)|(1<<7)

   if (end > g_menuPlayersNum[id])
      end = g_menuPlayersNum[id]

   for(new a = start; a < end; ++a){
      i = g_menuPlayers[id][a]
      get_user_name(i,name,31)
      get_user_team(i,team,3)
      keys |= (1<<b)
      len += format( menuBody[len],511-len,"\w%d. %s^n\w",++b,name )
   }

   len += format( menuBody[len],511-len,"^n8. Give  %d Gold^n",g_menuSettings[id] )

   if (end != g_menuPlayersNum[id]){
      format( menuBody[len],511-len,"^n9. More...^n0. %s", pos ? "Back" : "Back" )
      keys |= (1<<8)
   }else
      format( menuBody[len],511-len,"^n0. %s", pos ? "Back" : "Exit" )

   show_menu( id,keys,menuBody )

   return PLUGIN_CONTINUE

}



// **************************************************************************
// BEGIN wowmod_display_teamxpmenu subroutine - Kept from UWC3
// Handler for the teamxp_menu 
// **************************************************************************
public wowmod_display_teamxpmenu(id){
   new pos = 0, i, menu_body[512], menu_items[3][32]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<7)|(1<<8)|(1<<9)

   menu_items[0]= "Terrorist"
   menu_items[1]= "Counter-Terrorists"
   menu_items[2]= "Everyone"

   pos += format(menu_body[pos], 511-pos, "\yTeam XP Menu^n^n")
   for (i = 0; i<3; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos,"^n8. Give  %d XP^n",g_menuSettings[id])
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu( id,keys,menu_body,-1 )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_display_autocastmenu subroutine
// The WowMOD autocast menu
// **************************************************************************
public wowmod_display_autocastmenu( id, page ){

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   autocastpage[id] = page

   new autocast[8]
   new class = p_class1[id]
   new pos = 0, i, menu_body[512]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)
   new etext[32]

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Autocast Menu^n") 

   autocast = p_class1autocast[id]

   if ( page == 1 ){
      pos += format(menu_body[pos], 511-pos, "\yPage 1: Primary spells^n^n") 
   }else{
      if ( p_class2[id] >= CLASS_DRUID ){
         autocast = p_class2autocast[id]
         class = p_class2[id]
         pos += format(menu_body[pos], 511-pos, "\yPage 2: Secondary spells^n^n") 
      }else{
         pos += format(menu_body[pos], 511-pos, "\yPage 1: Primary spells^n^n") 
         page = 1
      } 
   }

   for ( i = 0; i< 8; i++ ){ 
      if ( autocast[i] )
         copy( etext, 31, "enabled" )
      else
         copy( etext, 31, "disabled" )

      if ( class >= CLASS_DRUID )
         switch( class ){
         case CLASS_DRUID: {
            if ( !( equali( druid_saycmds[i], "na" ) ) ){
               pos += format( menu_body[pos], 511-pos, "\w%d. %s [%s]^n", i+1, druid_spells[i], etext )
            }else{
               pos += format( menu_body[pos], 511-pos, "\d%d. %s [not available]^n", i+1, druid_spells[i] )
            }
         }
         case CLASS_HUNTER: {
            if ( !( equali( hunter_saycmds[i], "na" ) ) ){
               pos += format( menu_body[pos], 511-pos, "\w%d. %s [%s]^n", i+1, hunter_spells[i], etext )
            }else{
               pos += format( menu_body[pos], 511-pos, "\d%d. %s [not available]^n", i+1, hunter_spells[i] )
            }
         }
         case CLASS_MAGE: {
            if ( !( equali( mage_saycmds[i], "na" ) ) ){
               pos += format( menu_body[pos], 511-pos, "\w%d. %s [%s]^n", i+1, mage_spells[i], etext )
            }else{
               pos += format( menu_body[pos], 511-pos, "\d%d. %s [not available]^n", i+1, mage_spells[i] )
            }
         }
         case CLASS_ROGUE: {
            if ( !( equali( rogue_saycmds[i], "na" ) ) ){
               pos += format( menu_body[pos], 511-pos, "\w%d. %s [%s]^n", i+1, rogue_spells[i], etext )
            }else{
               pos += format( menu_body[pos], 511-pos, "\d%d. %s [not available]^n", i+1, rogue_spells[i] )
            }
         }
         case CLASS_WARLOCK: {
            if ( !( equali( warlock_saycmds[i], "na" ) ) ){
               pos += format( menu_body[pos], 511-pos, "\w%d. %s [%s]^n", i+1, warlock_spells[i], etext )
            }else{
               pos += format( menu_body[pos], 511-pos, "\d%d. %s [not available]^n", i+1, warlock_spells[i] )
            }
         }
         case CLASS_PRIEST: {
            if ( !( equali( priest_saycmds[i], "na" ) ) ){
               pos += format( menu_body[pos], 511-pos, "\w%d. %s [%s]^n", i+1, priest_spells[i], etext )
            }else{
               pos += format( menu_body[pos], 511-pos, "\d%d. %s [not available]^n", i+1, priest_spells[i] )
            }
         }
      }
   }

   pos += format( menu_body[pos], 511-pos, "^n\w0. Exit" )
   show_menu( id, keys, menu_body, -1 )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_merchantmenu subroutine
// The WowMOD merchants menu
// **************************************************************************
public wowmod_display_merchantmenu( id ){

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new nLoop = 4
   new pos = 0, i, menu_body[512], menu_items[9][32]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<8)|(1<<9)

   menu_items[0]= "Spell Merchant"
   menu_items[1]= "Items Merchant"
   menu_items[2]= "Armourer"
   menu_items[3]= "Sell Spells"
   menu_items[4]= "Sell Equipment"
 
   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Merchants Menu^n^n") 

   for ( i = 0; i<= nLoop; i++ ){ 
      pos += format( menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i] )
   }

   pos += format( menu_body[pos], 511-pos, "^n\w9. Main Menu" )
   pos += format( menu_body[pos], 511-pos, "^n\w0. Exit" )
   show_menu( id, keys, menu_body, -1 )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_sellequipmentmenu subroutine
// The WowMOD sell equipment menu
// **************************************************************************
public wowmod_display_sellequipmenu( id, page ){

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED
   
   new pos = 0, eq_idx = 0, gidx = 0, gold = 0, max = 0
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)
   new menu_body[512]

   for ( new i = 10; i < MAX_INV; i++ )
      if ( p_inventory[id][i] > 0 )
         max += 1 

   if ( ( page > 1 ) && ( ( (page-1) * 8 ) > max ) )
      page = 1
   sellequipmentpage[id] = page

   new startidx = 10 + (page-1) * 8

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Sell Equipment Menu (page %d)^n", page ) 

   for ( new i = 0; i < 8; i++ ){
      eq_idx = startidx + i
      if ( p_inventory[id][eq_idx] <= 0 ){
         pos += format( menu_body[pos], 511-pos, "\d%d. (empty slot)^n", i+1 )
      }else{
         gidx = p_inventory[id][eq_idx]
         gold = floatround( g_cost[gidx] / SELL_EQUIP_DIV )

         pos += format( menu_body[pos], 511-pos, "\w%d. %s (Value: %d gold)^n", 
                i+1, gear[gidx], gold )
      }
   }

   pos += format( menu_body[pos], 511-pos, "^n\w9. Next" )
   pos += format( menu_body[pos], 511-pos, "^n\w0. Exit" )
   show_menu( id, keys, menu_body, -1 )

   return PLUGIN_HANDLED

}


// **************************************************************************
// BEGIN wowmod_display_sellspellmenu subroutine
// The WowMOD sell spell menu
// **************************************************************************
public wowmod_display_sellspellmenu( id, page ){

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new sidx = 0, gold = 0
   new pos = 0, i, menu_body[512]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)
   new spell[64]

   page = ( (page > 0) && (page <= 2) ) ? page : 1
   sellspellpage[id] = page

   switch( p_class1[id] ){
      case CLASS_DRUID: sidx = SPELL_DRUID_BASE_IDX 
      case CLASS_HUNTER: sidx = SPELL_HUNTER_BASE_IDX
      case CLASS_MAGE: sidx = SPELL_MAGE_BASE_IDX
      case CLASS_ROGUE: sidx = SPELL_ROGUE_BASE_IDX
      case CLASS_WARLOCK: sidx = SPELL_WARLOCK_BASE_IDX
      case CLASS_PRIEST: sidx = SPELL_PRIEST_BASE_IDX
   }

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Sell Back Spells Menu (page %d)^n", page ) 
   pos += format(menu_body[pos], 511-pos, "\ySelling a spell will cause your spell rank to decrease^n^n" ) 

   if ( page == 1 ){
      for ( i = 0; i < 8; i++ ){
         switch( p_class1[id] ){
            case CLASS_DRUID: {
               copy( spell, 32, druid_spells[i] ) 
               gold = floatround( druid_spgold[i] / SELL_SPELL_DIV )
            }
            case CLASS_HUNTER: {
               copy( spell, 32, hunter_spells[i] ) 
               gold = floatround( hunter_spgold[i] / SELL_SPELL_DIV )
            }
            case CLASS_MAGE: {
               copy( spell, 32, mage_spells[i] )
               gold = floatround( mage_spgold[i] / SELL_SPELL_DIV )
            }
            case CLASS_ROGUE: {
               copy( spell, 32, rogue_spells[i] )
               gold = floatround( rogue_spgold[i] / SELL_SPELL_DIV )
            }
            case CLASS_WARLOCK: {
               copy( spell, 32, warlock_spells[i] )
               gold = floatround( warlock_spgold[i] / SELL_SPELL_DIV )
            }
            case CLASS_PRIEST: {
               copy( spell, 32, priest_spells[i] )
               gold = floatround( priest_spgold[i] / SELL_SPELL_DIV )
            }
         }

         if ( p_spells[id][sidx + i] > 0 ){
            pos += format( menu_body[pos], 511-pos, "\w%d. %s [Spellrank: %d] (Value: %d gold)^n", 
                   i+1, spell, p_spells[id][sidx+i], gold )
         }else{
            pos += format( menu_body[pos], 511-pos, "\d%d. %s [Unlearned]^n", 
                   i+1, spell, p_spells[id][sidx+i] )
         }
      } 
   }

   if ( ( page == 2 ) && ( p_class2[id] >= 0 ) ){
      switch( p_class2[id] ){
         case CLASS_DRUID: sidx = SPELL_DRUID_BASE_IDX 
         case CLASS_HUNTER: sidx = SPELL_HUNTER_BASE_IDX
         case CLASS_MAGE: sidx = SPELL_MAGE_BASE_IDX
         case CLASS_ROGUE: sidx = SPELL_ROGUE_BASE_IDX
         case CLASS_WARLOCK: sidx = SPELL_WARLOCK_BASE_IDX
         case CLASS_PRIEST: sidx = SPELL_PRIEST_BASE_IDX
      }

      for ( i = 0; i < 8; i++ ){
         switch( p_class2[id] ){
            case CLASS_DRUID: {
               copy( spell, 32, druid_spells[i] ) 
               gold = floatround( druid_spgold[i] / SELL_SPELL_DIV )
            }
            case CLASS_HUNTER: {
               copy( spell, 32, hunter_spells[i] ) 
               gold = floatround( hunter_spgold[i] / SELL_SPELL_DIV )
            }
            case CLASS_MAGE: {
               copy( spell, 32, mage_spells[i] )
               gold = floatround( mage_spgold[i] / SELL_SPELL_DIV )
            }
            case CLASS_ROGUE: {
               copy( spell, 32, rogue_spells[i] )
               gold = floatround( rogue_spgold[i] / SELL_SPELL_DIV )
            }
            case CLASS_WARLOCK: {
               copy( spell, 32, warlock_spells[i] )
               gold = floatround( warlock_spgold[i] / SELL_SPELL_DIV )
            }
            case CLASS_PRIEST: {
               copy( spell, 32, priest_spells[i] )
               gold = floatround( priest_spgold[i] / SELL_SPELL_DIV )
            }
         }

         if ( p_spells[id][sidx + i] > 0 ){
            pos += format( menu_body[pos], 511-pos, "\w%d. %s [Spellrank: %d] (Value: %d gold)^n", 
                   i+1, spell, p_spells[id][sidx+i], gold )
         }else{
            pos += format( menu_body[pos], 511-pos, "\d%d. %s [Unlearned]^n", 
                   i+1, spell, p_spells[id][sidx+i] )
         }
      } 

   }

   pos += format( menu_body[pos], 511-pos, "^n\w9. Next" )
   pos += format( menu_body[pos], 511-pos, "^n\w0. Exit" )
   show_menu( id, keys, menu_body, -1 )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_mainmenu subroutine
// The main WowMOD menu
// **************************************************************************
public wowmod_display_mainmenu(id){

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new team = get_user_team( id )
   //client_print( id, print_console, ">>DEBUG in menu() team=(%d)", team )

   if ( team == 0 )
      return PLUGIN_HANDLED

   new nLoop = 8
   new pos = 0, i, menu_body[512], menu_items[9][32]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   menu_items[0]= "Welcome to WowMOD"
   menu_items[1]= "Race Selection"
   menu_items[2]= "Secondary Class Selection"
   menu_items[3]= "Spell Merchant"
   menu_items[4]= "Merchants Menu"
   menu_items[5]= "Character Sheet"
   menu_items[6]= "Your Known Spells"
   menu_items[7]= "Help"


   if ( ( get_user_flags(id) & get_cvar_num("WOW_admin_flag") ) &&
        ( !get_cvar_num( "WOW_disable_adminmenu") ) )
   {
      menu_items[8]= "Admin Menu"
   }else{
      menu_items[8]= "WowMOD Credits"
   }
 
   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Main Menu^n^n") 

   for ( i = 0; i<= nLoop; i++ ){ 
      if ( ( p_level[id] < DUAL_CLASS_LEVEL ) && ( i == 2 ) )
         pos += format( menu_body[pos], 511-pos, "\d%d. %s^n",i+1,menu_items[i] )
      else
         pos += format( menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i] )
   }

   pos += format( menu_body[pos], 511-pos, "^n\w0. Exit" )
   show_menu( id, keys, menu_body, -1 )


   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_mainmenu2 subroutine
// The main WowMOD menu page 2
// **************************************************************************
public wowmod_display_mainmenu2(id){

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new nLoop = 2
   new pos = 0, i, menu_body[512], menu_items[9][32]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   menu_items[0]= "Character Sheet"
   menu_items[1]= "Reset"
   menu_items[2]= "WowMOD Credits"
   menu_items[3]= "HUD Resolution"
 
 
   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Menu Pg2^n^n") 

   for ( i = 0; i<= nLoop; i++ ){ 
      if ( i == 0 || i == 1 || i == 2 )
         pos += format( menu_body[pos], 511-pos, "\d%d. %s^n",i+1,menu_items[i] )
      else
         pos += format( menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i] )
   }

   pos += format( menu_body[pos], 511-pos, "^n\w9. Back" )
   pos += format( menu_body[pos], 511-pos, "^n\w0. Exit" )
   show_menu( id, keys, menu_body, -1 )


   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_ subroutine
// Display the * menu  
// **************************************************************************
public wowmod_display_(id){

}


// **************************************************************************
// BEGIN wowmod_display_class2menu subroutine
// Display the secondary class selection menu  
// **************************************************************************
public wowmod_display_class2menu(id){

   new pos = 0, i, num, menu_body[512], menu_items[8][64]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   if ( ( p_class2[id] >= CLASS_DRUID ) && !change_conf[id] ){
      wowmod_display_changeconf2( id )
      return PLUGIN_CONTINUE
   }

   change_conf[id] = false

   if ( p_race[id] == RACE_NIGHTELF ){
      num = 3
      menu_items[0]= "Druid"
      menu_items[1]= "Hunter"
      menu_items[2]= "Rogue"
      menu_items[3]= "Priest"
   }else if ( p_race[id] == RACE_GNOME ){
      num = 2
      menu_items[0]= "Mage"
      menu_items[1]= "Rogue"
      menu_items[2]= "Warlock"
   }else if ( p_race[id] == RACE_DWARF ){
      num = 2
      menu_items[0]= "Hunter"
      menu_items[1]= "Rogue"
      menu_items[2]= "Priest"
   }else if ( p_race[id] == RACE_HUMAN ){
      num = 3
      menu_items[0]= "Mage"
      menu_items[1]= "Rogue"
      menu_items[2]= "Warlock"
      menu_items[3]= "Priest"
   }else if ( p_race[id] == RACE_ORC ){
      num = 2
      menu_items[0]= "Hunter"
      menu_items[1]= "Rogue"
      menu_items[2]= "Warlock"
   }else if ( p_race[id] == RACE_TAUREN ){
      num = 1
      menu_items[0]= "Druid"
      menu_items[1]= "Hunter"
   }else if ( p_race[id] == RACE_UNDEAD ){
      num = 3
      menu_items[0]= "Mage"
      menu_items[1]= "Rogue"
      menu_items[2]= "Warlock"
      menu_items[3]= "Priest"
   }else if ( p_race[id] == RACE_BLOODELF ){
      num = 4
      menu_items[0]= "Hunter"
      menu_items[1]= "Mage"
      menu_items[2]= "Rogue"
      menu_items[3]= "Warlock"
      menu_items[4]= "Priest"
   }

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Secondary Class Selection^n^n")
   for (i = 0; i<=num; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_display_nelf_classmenu subroutine
// Display the Nightelf class selection menu 
// **************************************************************************
public wowmod_display_nelf_classmenu(id){

   new pos = 0, i, menu_body[512], menu_items[8][64]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<8)|(1<<9)

   new num = 3
   menu_items[0]= "Druid"
   menu_items[1]= "Hunter"
   menu_items[2]= "Rogue"
   menu_items[3]= "Priest"

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Nightelf Class Selection^n^n")
   for (i = 0; i<=num; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_display_gnome_classmenu subroutine
// Display the Gnome class selection menu 
// **************************************************************************
public wowmod_display_gnome_classmenu(id){

   new pos = 0, i, menu_body[512], menu_items[8][64]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<8)|(1<<9)

   new num = 2
   menu_items[0]= "Mage"
   menu_items[1]= "Rogue"
   menu_items[2]= "Warlock"

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Gnome Class Selection^n^n")
   for (i = 0; i<=num; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_display_dwarf_classmenu subroutine
// Display the Dwarf class selection menu 
// **************************************************************************
public wowmod_display_dwarf_classmenu(id){

   new pos = 0, i, menu_body[512], menu_items[8][64]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<8)|(1<<9)

   new num = 2
   menu_items[0]= "Hunter"
   menu_items[1]= "Rogue"
   menu_items[2]= "Priest"

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Dwarf Class Selection^n^n")
   for (i = 0; i<=num; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE
}



// **************************************************************************
// BEGIN wowmod_display_human_classmenu subroutine
// Display the Human class selection menu 
// **************************************************************************
public wowmod_display_human_classmenu(id){

   new pos = 0, i, menu_body[512], menu_items[8][64]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<8)|(1<<9)

   new num = 3
   menu_items[0]= "Mage"
   menu_items[1]= "Rogue"
   menu_items[2]= "Warlock"
   menu_items[3]= "Priest"

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Human Class Selection^n^n")
   for (i = 0; i<=num; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_display_orc_classmenu subroutine
// Display the Orc class selection menu 
// **************************************************************************
public wowmod_display_orc_classmenu(id){

   new pos = 0, i, menu_body[512], menu_items[8][64]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<8)|(1<<9)

   new num = 2
   menu_items[0]= "Hunter"
   menu_items[1]= "Rogue"
   menu_items[2]= "Warlock"

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Orc Class Selection^n^n")
   for (i = 0; i<=num; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_display_tauren_classmenu subroutine
// Display the Tauren class selection menu 
// **************************************************************************
public wowmod_display_tauren_classmenu(id){

   new pos = 0, i, menu_body[512], menu_items[8][64]
   new keys = (1<<0)|(1<<1)|(1<<8)|(1<<9)

   new num = 1
   menu_items[0]= "Druid"
   menu_items[1]= "Hunter"

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Tauren Class Selection^n^n")
   for (i = 0; i<=num; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_display_undead_classmenu subroutine
// Display the Undead class selection menu 
// **************************************************************************
public wowmod_display_undead_classmenu(id){

   new pos = 0, i, menu_body[512], menu_items[8][64]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<8)|(1<<9)

   new num = 3
   menu_items[0]= "Mage"
   menu_items[1]= "Rogue"
   menu_items[2]= "Warlock"
   menu_items[3]= "Priest"

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Undead Class Selection^n^n")
   for (i = 0; i<=num; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_display_belf_classmenu subroutine
// Display the Bloodelf class selection menu 
// **************************************************************************
public wowmod_display_belf_classmenu(id){

   new pos = 0, i, menu_body[512], menu_items[8][64]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<8)|(1<<9)

   new num = 4
   menu_items[0]= "Hunter"
   menu_items[1]= "Mage"
   menu_items[2]= "Rogue"
   menu_items[3]= "Warlock"
   menu_items[4]= "Priest"

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Bloodelf Class Selection^n^n")
   for (i = 0; i<=num; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_display_racemenu subroutine
// Display the race selection menu menu
// **************************************************************************
public wowmod_display_racemenu(id){

   if ( ( p_race[id] >= RACE_NIGHTELF ) && ( p_class1[id] >= CLASS_DRUID ) && !change_conf[id] ){
      wowmod_display_changeconf( id )
      return PLUGIN_CONTINUE
   }

   change_conf[id] = false

   new pos = 0, i, menu_body[512], menu_items[8][32]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   menu_items[0]= "Nightelf"
   menu_items[1]= "Gnome"
   menu_items[2]= "Dwarf"
   menu_items[3]= "Human"
   menu_items[4]= "Orc"
   menu_items[5]= "Tauren"
   menu_items[6]= "Undead"
   menu_items[7]= "Bloodelf"

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Race Selection^n^n")
   for (i = 0; i<8; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_display_changeconf subroutine
// Display the race/class change confirmation screen
// **************************************************************************
public wowmod_display_changeconf(id){

   if ( change_conf[id] ){
      return PLUGIN_CONTINUE
   }

   new pos = 0, i, menu_body[512], menu_items[8][64]
   new keys = (1<<0)|(1<<1)|(1<<8)|(1<<9)

   menu_items[0]= "Yes, I want to change my race/primary class"
   menu_items[1]= "No, please keep my race/primary class"

   pos += format(menu_body[pos], 511-pos, "\yConfirm Race and Class Change^n")
   pos += format(menu_body[pos], 511-pos, "\wWarning: You will lose all of your spells by changing your race/primary class! ^n^n")
   for (i = 0; i<2; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. WowMOD Main Menu")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_display_changeconf2 subroutine
// Display the secondary class change confirmation screen
// **************************************************************************
public wowmod_display_changeconf2(id){

   if ( change_conf[id] ){
      return PLUGIN_CONTINUE
   }

   new pos = 0, i, menu_body[512], menu_items[8][64]
   new keys = (1<<0)|(1<<1)|(1<<8)|(1<<9)

   menu_items[0]= "Yes, I want to change my secondary class"
   menu_items[1]= "No, please keep my chosen secondary class"

   pos += format(menu_body[pos], 511-pos, "\yConfirm Secondary Class Change^n")
   pos += format(menu_body[pos], 511-pos, "\wWarning: You will lose your secondary class spells by changing your class! ^n^n")
   for (i = 0; i<2; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. WowMOD Main Menu")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_display_resurrect subroutine
// The WowMOD Priest resurrect menu
// **************************************************************************
public wowmod_display_resurrect( id, page ){

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cannot cast resurrect!", MOD )
      return PLUGIN_HANDLED
   }

   new spell_index = SPELL_PRIEST_BASE_IDX + SP_RESURRECT
   new spell_level = p_spells[id][spell_index]
   new cast_mana = priest_spmana[SP_RESURRECT] + ( SPELL_MANA_RANK2 * spell_level )
   if ( p_mana[id] < cast_mana ){
      client_print(id, print_chat,
         "[%s] You do not have enough mana to cast Resurrection", MOD )
      return PLUGIN_HANDLED
   }

   page = ( (page > 0) && (page <= 2) ) ? page : 1
   respage[id] = page

   new pos = 0, menu_body[512]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   pos += format(menu_body[pos], 511-pos, "\yResurrection Menu^n")
   pos += format(menu_body[pos], 511-pos, "\ySelect a dead player from your team:^n^n")

   new friend
   new fname[32]
   new player[32]
   new tdead[32]
   new numplayers

   get_players( player, numplayers )

   // Preload dead teamate list
   new numdead = 0
   for ( new i = 0; i < numplayers; ++i ){
      friend = player[i]
      //client_print( id, print_chat, "Evaluating %d for the tdead list", friend )

      //if ( id == friend )
      //   continue

      if ( get_user_team( id ) != get_user_team( friend ) )
         continue

      if ( is_user_alive( friend ) )
         continue

      //client_print( id, print_chat, "Adding %d to the tdead list", friend )

      tdead[numdead++] = friend 
   }
   
   new startidx = (page-1) * 8
   new endidx = ( (startidx + 7)>numdead ) ? numdead : ( startidx + 7 )

   //client_print( id, print_chat, "page=%d startidx %d endidx %d", page, startidx, endidx )
    
   for ( new i = startidx; i < endidx; ++i ){
      friend = tdead[i]
      //client_print( id, print_chat, "i=%d friend=%d", i, friend )

      reslist[id][i] = friend

      get_user_name( friend, fname, 31 )

      pos += format(menu_body[pos], 511-pos, "\w%d. %s [%d]^n", i+1, fname, friend )
   }

   pos += format(menu_body[pos], 511-pos, "^n\w9. Next")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_allbadges subroutine
// The WowMOD all badges display menu
// **************************************************************************
public wowmod_display_allbadges(id){

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new pos = 0, i, menu_body[512], menu_items[7][32]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   menu_items[0]= "WowMOD Badges Page 1"
   menu_items[1]= "WowMOD Badges Page 2"
   menu_items[2]= "WowMOD Badges Page 3"
   menu_items[3]= "WowMOD Badges Page 4"
   menu_items[4]= "WowMOD Badges Page 5"
   menu_items[5]= "WowMOD Badges Page 6"
   menu_items[6]= "WowMOD Badges Page 7"

   pos += format(menu_body[pos], 511-pos, "\yDisplay All Badges Menu^n^n")
   for (i = 0; i<7; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Main Menu")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_HANDLED

}



// **************************************************************************
// END WowMOD Menu System 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

