/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: SQL Storage
*** Date:          Aug 09, 2007
*** Last Update:   Aug 10, 2007
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
// BEGIN WowMOD SQL Storage
// **************************************************************************


// **************************************************************************
// BEGIN check_sql and grab_sql_vars subroutines - kept from UWC3
// Check if mp_sqp is set and load sql config file if so
// **************************************************************************
public check_sql(){

   new basedir[64]
   new sql_cfgfile[64]
   get_customdir(basedir, 63)

   if ( !get_cvar_num("mp_savexp") )
      return PLUGIN_CONTINUE


   // [09-08-04] Added SQL support, config file loading
   if (get_cvar_num("mp_sql") == 1){
      format(sql_cfgfile, 63, "%s/WowMOD/WowMOD_sql.cfg", basedir)
      if (file_exists(sql_cfgfile)){
         log_amx("WowMOD SQL :: Loading Configuration File...")
         server_cmd("exec %s", sql_cfgfile)
         log_amx("WowMOD SQL :: Configuration File Loaded [OK]")

         set_task(0.6, "grab_sql_vars", 824)

      }else{
         log_amx("WowMOD SQL :: Configuration File NOT FOUND, Switching to Fast XP Mode")
         set_cvar_num("mp_savexp", 0)
         set_cvar_num("mp_sql", 0)
      }
   }else{
      log_amx("WowMOD VAULT :: Vault Storage Configured...")
   }

   return PLUGIN_CONTINUE
}


public grab_sql_vars(){

   get_cvar_string("WOW_sql_host", sqlhost, 63)
   get_cvar_string("WOW_sql_user", sqluser, 31)
   get_cvar_string("WOW_sql_pass", sqlpass, 31)
   get_cvar_string("WOW_sql_db", sqldb, 31)

   log_amx("WowMOD SQL :: Config Report: host=(%s) user=(%s) pass=(*) db=(%s)",
           sqlhost, sqluser, sqldb)

   new Sql:sql = dbi_connect(sqlhost, sqluser, sqlpass, sqldb, sqlerror, 127)
   if (sql <= SQL_FAILED) {
     log_amx("WowMOD SQL :: Connect Error [ %s ]", sqlerror)
     return PLUGIN_HANDLED
   }else{
     log_amx("WowMOD SQL :: Connected [OK]" )
   }

   // Create the players table if necessary
   dbi_query(sql, "CREATE TABLE IF NOT EXISTS %s ( name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, data TEXT, inv TEXT, talent TEXT, ip VARCHAR(32), PRIMARY KEY ( steamid ) )", sqltable_players)

   if (get_cvar_num("mp_sql_saveby") == SQL_SAVEBY_NAME){
      dbi_query(sql, "ALTER TABLE %s DROP PRIMARY KEY, ADD PRIMARY KEY ( name )", sqltable_players)
   }else{
      dbi_query(sql, "ALTER TABLE %s DROP PRIMARY KEY, ADD PRIMARY KEY ( steamid )", sqltable_players)
   }


   dbi_close(sql)

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN sqlload_id subroutine
// Retrieve a player's data from the sql server
// **************************************************************************
public sqlload_id(id){
   // If mod is disabled, abort
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   // If we are not saving data or not in sql mode, abort
   if ( !get_cvar_num("mp_savexp") || !get_cvar_num("mp_sql") )
      return PLUGIN_CONTINUE

   new playerid[32], playername[32], error[128]

   get_user_authid(id, playerid, 31)
   get_user_name(id, playername, 31)

   replaceall(playername, 32, "'", "")

   // Connect to the SQL server 
   new Sql:sql = dbi_connect(sqlhost, sqluser, sqlpass, sqldb, sqlerror, 127)
   if (sql <= SQL_FAILED) {
     log_amx("WowMOD SQL :: sqlload_id() :: Connect Error: %s", sqlerror)
     return PLUGIN_HANDLED
   }

   new Result:Res

   if ( get_cvar_num("mp_sql_saveby") == SQL_SAVEBY_NAME ){
      Res = dbi_query(sql,"SELECT name, steamid, data, inv, talent, ip FROM %s WHERE name = '%s'", sqltable_players, playername )
   }else{
      Res = dbi_query(sql,"SELECT name, steamid, data, inv, talent, ip FROM %s WHERE steamid = '%s'", sqltable_players, playerid )
   }

   if (Res == RESULT_FAILED) {
      dbi_error(sql,error,127)
      client_print(id, print_chat,
         "[%s] Error Retrieving Your SQL Data: [ %s ]", MOD, error)
      dbi_free_result(Res)
      dbi_close(sql)
      return PLUGIN_CONTINUE
   }else if (Res == RESULT_NONE) {
      client_print(id, print_chat, "[%s] Player data not found for player %s",
         MOD, playername )
      dbi_free_result(Res)
      dbi_close(sql)
      return PLUGIN_CONTINUE
   }

   // Do player data load stuff here
   new name[32], pid[32], ip[32], data[512], inv[512], talents[512] 

   if ( dbi_nextrow(Res) > 0 ){
      dbi_result(Res, "name", name, 31)
      dbi_result(Res, "steamid", pid, 31)
      dbi_result(Res, "data", data, 511 )
      dbi_result(Res, "inv", inv, 511 )
      dbi_result(Res, "talent", talents, 511 )
      dbi_result(Res, "ip", ip, 31)

      log_amx("WowMOD SQL :: Player Table Loaded [OK] : name=(%s) steamid=(%s) last_ip=(%s)", 
               name, pid, ip )


      new tlist[10][12] = { "", "", "", "", "", "", "", "", "", "" }
      strsplit( tlist, talents )

      new glist[100][12] = { 
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
         "", "", "", "" }
      strsplit( glist, inv )

      p_head[id] = str_to_num( glist[IDX_GEAR_HEAD] )
      p_chest[id] = str_to_num( glist[IDX_GEAR_CHEST] )
      p_back[id] = str_to_num( glist[IDX_GEAR_BACK] )
      p_legs[id] = str_to_num( glist[IDX_GEAR_LEGS] )
      p_hands[id] = str_to_num( glist[IDX_GEAR_HANDS] )
      p_feet[id] = str_to_num( glist[IDX_GEAR_FEET] )
      p_wrists[id] = str_to_num( glist[IDX_GEAR_WRISTS] )
      p_ring1[id] = str_to_num( glist[IDX_GEAR_RING1] )
      p_ring2[id] = str_to_num( glist[IDX_GEAR_RING2] )
      p_neck[id] = str_to_num( glist[IDX_GEAR_NECK] )


      new strlist[100][12] = { 
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
         "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
         "", "", "", "" }
      strsplit( strlist, data )

      p_race[id] = str_to_num( strlist[IDX_RACE] )
      p_class1[id] = str_to_num( strlist[IDX_CLASS1] )
      p_class2[id] = str_to_num( strlist[IDX_CLASS2] )
      p_xp[id] = str_to_num( strlist[IDX_XP] )
      p_str[id] = str_to_num( strlist[IDX_STR] )
      p_agi[id] = str_to_num( strlist[IDX_AGI] )
      p_stam[id] = str_to_num( strlist[IDX_STAM] )
      p_int[id] = str_to_num( strlist[IDX_INT] )
      p_int_unb[id] = p_int[id]
      p_spir[id] = str_to_num( strlist[IDX_SPIR] )
      p_armor[id] = str_to_num( strlist[IDX_ARMOR] )
      p_mana[id] = str_to_num( strlist[IDX_MANA] )
      p_mana_base[id] = p_mana[id]

      log_amx("WowMOD SQL :: %s core data : race=(%d) class1=(%d) class2=(%d) xp=(%d)", 
               name, p_race[id], p_class1[id], p_class2[id], p_xp[id] )
      log_amx("WowMOD SQL :: %s gear data : head=(%d) chest=(%d) back=(%d) legs=(%d) hands=(%d)", 
               name, p_head[id], p_chest[id], p_back[id], p_legs[id], p_hands[id] )
      log_amx("WowMOD SQL :: %s gear data : feet=(%d) wrists=(%d) ring1=(%d) ring2=(%d) neck=(%d)", 
               name, p_feet[id], p_wrists[id], p_ring1[id], p_ring2[id], p_neck[id] )
      log_amx("WowMOD SQL :: %s attrib data : st=(%d) ag=(%d) sta=(%d) in=(%d) sp=(%d) ar=(%d) mana=(%d)",
               name, p_str[id], p_agi[id], p_stam[id], p_int[id], p_spir[id], p_armor[id], p_mana[id] )


      p_gold[id] = str_to_num( strlist[IDX_GOLD] )
      p_tier[id] = str_to_num( strlist[IDX_TIER] )

      log_amx( "WowMOD SQL :: Loaded gold=(%d) tier=(%d)", p_gold[id], p_tier[id] )

      log_amx( "WowMOD :: Loaded player spells data" )
      for( new j=0; j < NUM_SPELLS; j++ ){
         p_spells[id][j] = str_to_num( strlist[IDX_SPELL1 + j] )
         log_amx( "WowMOD SQL :: Loaded spell %d = %d", j, p_spells[id][j] )
      }

      log_amx( "WowMOD :: Loaded player talents data" )
      for( new j=0; j < NUM_TALENTS; j++ ){
         p_talents[id][j] = str_to_num( tlist[j] )
         log_amx( "WowMOD SQL :: Loaded talent %d = %d", j, p_talents[id][j] )
      }

      log_amx( "WowMOD :: Loaded player inventory data" )
      for ( new j = 0; j < MAX_INV; j++ ){
         p_inventory[id][j] = str_to_num( glist[j] )
         log_amx( "WowMOD SQL :: inventory slot %d : %d", j, p_inventory[id][j] )
      }

      p_kills[id] = str_to_num( strlist[IDX_KILLS] )
      p_deaths[id] = str_to_num( strlist[IDX_DEATHS] )
      p_rounds[id] = str_to_num( strlist[IDX_ROUNDS] )
      p_casts[id] = str_to_num( strlist[IDX_CASTS] )
      p_heals[id] = str_to_num( strlist[IDX_HEALS] )
      p_assists[id] = str_to_num( strlist[IDX_ASSISTS] )
      p_roguespec[id] = str_to_num( strlist[IDX_ROGUESPEC] )
      p_rezzes[id] = str_to_num( strlist[IDX_REZZES] )
      p_teamacts[id] = str_to_num( strlist[IDX_TEAMACTS] )
      p_badges[id] = str_to_num( strlist[IDX_BADGES] )
      p_badges2[id] = str_to_num( strlist[IDX_BADGES2] )

      log_amx( "WowMOD SQL :: Loaded stats k=(%d) d=(%d) r=(%d) c=(%d) h=(%d) a=(%d) rog=(%d) rez=(%d) team=(%d) badges=(%d)", 
         p_kills[id], p_deaths[id], p_rounds[id], p_casts[id], p_heals[id], p_assists[id], p_roguespec[id],
         p_rezzes[id], p_teamacts[id], p_badges[id] )

      p_bombs[id] = str_to_num( strlist[IDX_BOMBS] )
      p_defuses[id] = str_to_num( strlist[IDX_DEFUSES] )
      p_supports[id] = str_to_num( strlist[IDX_SUPPORTS] )

      log_amx( "WowMOD SQL :: Loaded stats2 b=(%d) d=(%d) s=(%d)", 
               p_bombs[id], p_defuses[id], p_supports[id] )

      p_melee[id] = str_to_num( strlist[IDX_MELEE] )
      p_marksman[id] = str_to_num( strlist[IDX_MARKSMAN] )
      p_arcane[id] = str_to_num( strlist[IDX_ARCANE] )
      p_fire[id] = str_to_num( strlist[IDX_FIRE] )
      p_frost[id] = str_to_num( strlist[IDX_FROST] )
      p_shadow[id] = str_to_num( strlist[IDX_SHADOW] )
      p_nature[id] = str_to_num( strlist[IDX_NATURE] )
      p_holy[id] = str_to_num( strlist[IDX_HOLY] )

      log_amx( "WowMOD SQL :: Loaded atts me=(%d) ma=(%d) ar=(%d) fi=(%d) fr=(%d) sh=(%d) n=(%d) ho=(%d)",
            p_melee[id], p_marksman[id], p_arcane[id], p_fire[id], p_frost[id], p_shadow[id], 
            p_nature[id], p_holy[id] )
 
      wowmod_hud_playerdata( id, 0 )

      //client_print( id, print_chat, "Player Table Loaded [OK] : name=(%s) steamid=(%s) data=(%s) last_ip=(%s)", name, pid, data, ip )

      wowmod_check_levelup( id )
   }

   dbi_close(sql)

   // Reset readytoload to indicate succesful player data load
   readytoload[id] = false

   // Call _classinit and _worninit routines to get player balanced
   switch( p_class1[id] ){
      case CLASS_DRUID: wowmod_classinit_druid( id, R_QUIET )
      case CLASS_HUNTER: wowmod_classinit_hunter( id, R_QUIET )
      case CLASS_MAGE: wowmod_classinit_mage( id, R_QUIET )
      case CLASS_ROGUE: wowmod_classinit_rogue( id, R_QUIET )
      case CLASS_WARLOCK: wowmod_classinit_warlock( id, R_QUIET )
      case CLASS_PRIEST: wowmod_classinit_priest( id, R_QUIET )
   }

   log_amx( "WowMOD SQL :: Called RACE/CLASS classinit for %s", name )

   wowmod_gear_worninit( id )

   log_amx( "WowMOD SQL :: Called GEAR worninit for %s", name )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN sqlwrite_id subroutine
// Write a player's data to the SQL table
// **************************************************************************
public sqlwrite_id(id){

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   if ( !get_cvar_num("mp_savexp") || !get_cvar_num("mp_sql") ){
      return PLUGIN_CONTINUE
   }

   if (readytoload[id]){
      return PLUGIN_CONTINUE
   }

   if ( p_xp[id] <= 150 ){
      client_print( id, print_chat, "Not able to save data until 150 XP has been earned." )
      return PLUGIN_CONTINUE
   }

   new playerid[32], playername[32], ip[32], data[512], data2[512], inv[512], inv2[512], talents[512]

   get_user_authid(id, playerid, 31)
   get_user_name(id, playername, 31)
   get_user_ip(id, ip, 31)

   replaceall(playername, 32, "'", "")

   // Create talents string for talent data
   format( talents, 512, "%d %d %d %d %d %d %d %d %d %d", 
     p_talents[id][0], p_talents[id][1], p_talents[id][2], p_talents[id][3], p_talents[id][4], 
     p_talents[id][5], p_talents[id][6], p_talents[id][7], p_talents[id][8], p_talents[id][9] )

   // Create inv string for inventory data
   format( inv, 512, "%d %d %d %d %d %d %d %d %d %d", 
     p_head[id], p_chest[id], p_back[id], p_legs[id], p_hands[id], p_feet[id], p_wrists[id],
     p_ring1[id], p_ring2[id], p_neck[id] )
   format( inv2, 512, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
     inv, p_inventory[id][10], p_inventory[id][11], p_inventory[id][12], p_inventory[id][13], 
     p_inventory[id][14], p_inventory[id][15], p_inventory[id][16], p_inventory[id][17],
     p_inventory[id][18], p_inventory[id][19], p_inventory[id][20], p_inventory[id][21],
     p_inventory[id][22], p_inventory[id][23], p_inventory[id][24], p_inventory[id][25] )
   format( inv, 512, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
     inv2, p_inventory[id][26], p_inventory[id][27], p_inventory[id][28], p_inventory[id][29], 
     p_inventory[id][30], p_inventory[id][31], p_inventory[id][32], p_inventory[id][33],
     p_inventory[id][34], p_inventory[id][35], p_inventory[id][36], p_inventory[id][37],
     p_inventory[id][38], p_inventory[id][39], p_inventory[id][40], p_inventory[id][41] )
   format( inv2, 512, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
     inv, p_inventory[id][42], p_inventory[id][43], p_inventory[id][44], p_inventory[id][45], 
     p_inventory[id][46], p_inventory[id][47], p_inventory[id][48], p_inventory[id][49],
     p_inventory[id][50], p_inventory[id][51], p_inventory[id][52], p_inventory[id][53],
     p_inventory[id][54], p_inventory[id][55], p_inventory[id][56], p_inventory[id][57] )
   format( inv, 512, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
     inv2, p_inventory[id][58], p_inventory[id][59], p_inventory[id][60], p_inventory[id][61], 
     p_inventory[id][62], p_inventory[id][63], p_inventory[id][64], p_inventory[id][65],
     p_inventory[id][66], p_inventory[id][67], p_inventory[id][68], p_inventory[id][69],
     p_inventory[id][70], p_inventory[id][71], p_inventory[id][72], p_inventory[id][73] )
   format( inv2, 512, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
     inv, p_inventory[id][74], p_inventory[id][75], p_inventory[id][76], p_inventory[id][77], 
     p_inventory[id][78], p_inventory[id][79], p_inventory[id][80], p_inventory[id][81],
     p_inventory[id][82], p_inventory[id][83], p_inventory[id][84], p_inventory[id][85],
     p_inventory[id][86], p_inventory[id][87], p_inventory[id][88], p_inventory[id][89] )
   format( inv, 512, "%s %d %d %d %d %d %d %d %d %d %d", 
     inv2, p_inventory[id][90], p_inventory[id][91], p_inventory[id][92], p_inventory[id][93], 
     p_inventory[id][94], p_inventory[id][95], p_inventory[id][96], p_inventory[id][97],
     p_inventory[id][98], p_inventory[id][99] )

   // Create data string for all stat and atribute data
   format( data, 512, "%d %d %d %d %d %d %d %d %d %d %d %d", 
     p_race[id], p_class1[id], p_class2[id], p_xp[id], p_str[id], p_agi[id], 
     p_stam[id], p_int_unb[id], p_spir[id], p_armor[id], p_mana_base[id], p_gold[id] )
   format( data2, 512, "%s %d", data, p_tier[id] )
   format( data, 512, "%s %d %d %d %d %d", 
      data2, p_assists[id], p_roguespec[id], p_rezzes[id], p_teamacts[id], p_badges[id] )
   format( data2, 512, "%s %d %d %d %d %d %d %d %d %d %d",
     data, p_spells[id][0], p_spells[id][1], p_spells[id][2], p_spells[id][3], 
     p_spells[id][4], p_spells[id][5], p_spells[id][6], p_spells[id][7],
     p_spells[id][8], p_spells[id][9] )
   format( data, 512, "%s %d %d %d %d %d %d %d %d %d %d",
     data2, p_spells[id][10], p_spells[id][11], p_spells[id][12], p_spells[id][13],
     p_spells[id][14], p_spells[id][15], p_spells[id][16], p_spells[id][17],
     p_spells[id][18], p_spells[id][19] )
   format( data2, 512, "%s %d %d %d %d %d %d %d %d %d %d",
     data, p_spells[id][20], p_spells[id][21], p_spells[id][22], p_spells[id][23],
     p_spells[id][24], p_spells[id][25], p_spells[id][26], p_spells[id][27],
     p_spells[id][28], p_spells[id][29] )
   format( data, 512, "%s %d %d %d %d %d %d %d %d %d %d",
     data2, p_spells[id][30], p_spells[id][31], p_spells[id][32], p_spells[id][33],
     p_spells[id][34], p_spells[id][35], p_spells[id][36], p_spells[id][37],
     p_spells[id][38], p_spells[id][39] )
   format( data2, 512, "%s %d %d %d %d %d %d %d %d %d %d",
     data, p_spells[id][40], p_spells[id][41], p_spells[id][42], p_spells[id][43],
     p_spells[id][44], p_spells[id][45], p_spells[id][46], p_spells[id][47],
     p_spells[id][48], p_spells[id][49] )
   format( data, 512, "%s %d %d %d %d %d %d %d %d %d %d",
     data2, p_spells[id][50], p_spells[id][51], p_spells[id][52], p_spells[id][53],
     p_spells[id][54], p_spells[id][55], p_spells[id][56], p_spells[id][57],
     p_spells[id][58], p_spells[id][59] )
   format( data2, 512, "%s %d %d %d %d %d %d %d %d",
     data, p_kills[id], p_deaths[id], p_rounds[id], p_casts[id], p_heals[id], p_bombs[id],
     p_defuses[id], p_supports[id] )

   format( data, 512, "%s %d %d %d %d %d %d %d %d %d",
     data2, p_melee[id], p_marksman[id], p_arcane[id], p_fire[id], p_frost[id], 
     p_shadow[id], p_nature[id], p_holy[id], p_badges2[id] )

   log_amx( "In sqlwrite() name=(%s) data=(%s) r=(%d) c1=(%d) c2=(%d) x=(%d)", 
            playername, data, p_race[id], p_class1[id], p_class2[id], p_xp[id] )
   log_amx( "In sqlwrite() name=(%s) inv=(%s)" , playername, inv )
   log_amx( "In sqlwrite() name=(%s) talents=(%s)" , playername, talents )

   // Connect to the SQL server and create the tables if they do not exist
   new Sql:sql = dbi_connect(sqlhost, sqluser, sqlpass, sqldb, sqlerror, 127)
   if (sql <= SQL_FAILED) {
     log_amx("WowMOD SQL :: sqlwrite_id() :: Connect Error: %s", sqlerror)
     return PLUGIN_HANDLED
   }

   dbi_query( sql, 
   "REPLACE INTO %s (name, steamid, data, inv, talent, ip ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s')",
   sqltable_players, playername, playerid, data, inv, talents, ip )

   dbi_close(sql)

   client_print( id, print_chat, "[%s] Your WowMOD player data has been saved.", MOD )

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN load_id subroutine
// Retrieve a player's data from the vault 
// **************************************************************************
public load_id(id){
   // If mod is disabled, abort
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   // If we are not saving data or not in sql mode, abort
   if ( !get_cvar_num("mp_savexp") || get_cvar_num("mp_sql") )
      return PLUGIN_CONTINUE

   new pkey[128], playerid[32], playername[32]
   new pdata[1600], temp[800], temp1[800], temp2[32]

   get_user_authid(id, playerid, 31)
   get_user_name(id, playername, 31)

   format( pkey, 127, "%s", playerid )   

   new attempt = get_vaultdata( pkey, pdata, 1600 )
 
   // Do player data load stuff here
   new name[32], ip[32] 

   name = playername

   if ( attempt ){

      log_amx("WowMOD VAULT :: Player Table Loaded [OK] for steamid=(%s)", pkey )

      str_break( pdata, playerid, temp, 32, 1600 )
      str_break( temp, ip, temp1, 32, 1600 )
      str_break( temp1, temp2, temp, 32, 1600 )
      p_race[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_class1[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_class2[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_xp[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_str[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_agi[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_stam[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_int[id] = str_to_num( temp2 ) 
      p_int_unb[id] = p_int[id]
      str_break( temp1, temp2, temp, 32, 1600 )
      p_spir[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_armor[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_mana[id] = str_to_num( temp2 ) 
      p_mana_base[id] = p_mana[id]
      str_break( temp, temp2, temp1, 32, 1600 )
      p_gold[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_tier[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_kills[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_deaths[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_rounds[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_casts[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_heals[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_bombs[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_defuses[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_supports[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_melee[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_marksman[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_arcane[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_fire[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_frost[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_shadow[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_nature[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_holy[id] = str_to_num( temp2 ) 

      log_amx( "WowMOD VAULT :: Loaded player spells data" )
      for( new j=0; j < NUM_SPELLS; j++ ){
         if ( j % 2 == 0 )
            str_break( temp, temp2, temp1, 32, 1600 )
         else
            str_break( temp1, temp2, temp, 32, 1600 )
         p_spells[id][j] = str_to_num( temp2 )
         log_amx( "WowMOD VAULT :: Loaded spell %d = %d", j, p_spells[id][j] )
      }

      log_amx( "WowMOD VAULT :: Loaded player inventory data" )
      for ( new j = 0; j < MAX_INV; j++ ){
         if ( j % 2 == 0 )
            str_break( temp, temp2, temp1, 32, 1600 )
         else
            str_break( temp1, temp2, temp, 32, 1600 )
         p_inventory[id][j] = str_to_num( temp2 )
         log_amx( "WowMOD VAULT :: inventory slot %d : %d", j, p_inventory[id][j] )
      }

      p_head[id] = p_inventory[id][IDX_GEAR_HEAD]
      p_chest[id] = p_inventory[id][IDX_GEAR_CHEST]
      p_back[id] = p_inventory[id][IDX_GEAR_BACK]
      p_legs[id] = p_inventory[id][IDX_GEAR_LEGS]
      p_hands[id] = p_inventory[id][IDX_GEAR_HANDS]
      p_feet[id] = p_inventory[id][IDX_GEAR_FEET]
      p_wrists[id] = p_inventory[id][IDX_GEAR_WRISTS]
      p_ring1[id] = p_inventory[id][IDX_GEAR_RING1]
      p_ring2[id] = p_inventory[id][IDX_GEAR_RING2]
      p_neck[id] = p_inventory[id][IDX_GEAR_NECK]

      log_amx( "WowMOD VAULT :: Loaded player talents data" )
      log_amx( "WowMOD VAULT :: before talent parsing temp=(%s) temp1=(%s)", temp, temp1 )
      for( new j=0; j < NUM_TALENTS; j++ ){
         if ( j % 2 == 0 )
            str_break( temp, temp2, temp1, 32, 1600 )
         else
            str_break( temp1, temp2, temp, 32, 1600 )
         p_talents[id][j] = str_to_num( temp2 )
         log_amx( "WowMOD VAULT :: Loaded talent %d = %d", j, p_talents[id][j] )
      }

      //log_amx( "WowMOD VAULT :: after talent parsing temp=(%s) temp1=(%s)", temp, temp1 )
      str_break( temp, temp2, temp1, 32, 1600 )
      p_assists[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_roguespec[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_rezzes[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_teamacts[id] = str_to_num( temp2 ) 
      str_break( temp, temp2, temp1, 32, 1600 )
      p_badges[id] = str_to_num( temp2 ) 
      str_break( temp1, temp2, temp, 32, 1600 )
      p_badges2[id] = str_to_num( temp2 ) 

      log_amx( "WowMOD VAULT :: Loaded assists=(%d) rogue_spec=(%d) rez=(%d) team=(%d) badges=(%d , %d)",  
         p_assists[id], p_roguespec[id], p_rezzes[id], p_teamacts[id], p_badges[id], p_badges2[id] )

      // 4 unused data slots left in temp at this point


      log_amx("WowMOD VAULT :: %s core data : race=(%d) class1=(%d) class2=(%d) xp=(%d)", 
               name, p_race[id], p_class1[id], p_class2[id], p_xp[id] )
      log_amx("WowMOD VAULT :: %s gear data : head=(%d) chest=(%d) back=(%d) legs=(%d) hands=(%d)", 
               name, p_head[id], p_chest[id], p_back[id], p_legs[id], p_hands[id] )
      log_amx("WowMOD VAULT :: %s gear data : feet=(%d) wrists=(%d) ring1=(%d) ring2=(%d) neck=(%d)", 
               name, p_feet[id], p_wrists[id], p_ring1[id], p_ring2[id], p_neck[id] )
      log_amx("WowMOD VAULT :: %s attrib data : st=(%d) ag=(%d) sta=(%d) in=(%d) sp=(%d) ar=(%d) mana=(%d)",
               name, p_str[id], p_agi[id], p_stam[id], p_int[id], p_spir[id], p_armor[id], p_mana[id] )
      log_amx( "WowMOD VAULT :: Loaded gold=(%d) tier=(%d)", p_gold[id], p_tier[id] )
      log_amx( "WowMOD VAULT :: Loaded stats k=(%d) d=(%d) r=(%d) c=(%d) h=(%d) a=(%d) b=(%d) d=(%d) s=(%d)",
         p_kills[id], p_deaths[id], p_rounds[id], p_casts[id], p_heals[id], p_assists[id], p_bombs[id],
         p_defuses[id], p_supports[id] )

      // Call _classinit and _worninit routines to get player balanced
      switch( p_class1[id] ){
         case CLASS_DRUID: wowmod_classinit_druid( id, R_QUIET )
         case CLASS_HUNTER: wowmod_classinit_hunter( id, R_QUIET )
         case CLASS_MAGE: wowmod_classinit_mage( id, R_QUIET )
         case CLASS_ROGUE: wowmod_classinit_rogue( id, R_QUIET )
         case CLASS_WARLOCK: wowmod_classinit_warlock( id, R_QUIET )
         case CLASS_PRIEST: wowmod_classinit_priest( id, R_QUIET )
      }

      log_amx( "WowMOD VAULT :: Called RACE/CLASS classinit for %s", name )

      wowmod_gear_worninit( id )

      log_amx( "WowMOD VAULT :: Called GEAR worninit for %s", name )

      wowmod_hud_playerdata( id, 0 )
      wowmod_check_levelup( id )
   }

   // Reset readytoload to indicate succesful player data load
   readytoload[id] = false

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN write_id subroutine
// Write a player's data to the vault 
// **************************************************************************
public write_id(id){
   // If mod is disabled, abort
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   // If we are not saving data or not in sql mode, abort
   if ( !get_cvar_num("mp_savexp") || get_cvar_num("mp_sql") )
      return PLUGIN_CONTINUE

   // Try to avoid saving a player who has not yet loaded their data
   if ( p_xp[id] < 150 ){
      client_print( id, print_chat, "Not able to save data until 150 XP has been earned." )
      return PLUGIN_CONTINUE
   }

   new pkey[128], playerid[32], playername[32], ip[32]
   new pdata[1600], inv[512], data[512], talents[512]

   get_user_authid(id, playerid, 31)
   get_user_name(id, playername, 31)
   get_user_ip(id, ip, 31)

   format( pkey, 127, "%s", playerid )   

   // Create talents string for talent data
   format( talents, 512, "%d %d %d %d %d %d %d %d %d %d",
     p_talents[id][0], p_talents[id][1], p_talents[id][2], p_talents[id][3], p_talents[id][4],
     p_talents[id][5], p_talents[id][6], p_talents[id][7], p_talents[id][8], p_talents[id][9] )

   log_amx( "talents=(%s)", talents )

   // Create inv string for inventory data
   format( inv, 512, "%d %d %d %d %d %d %d %d %d %d",
     p_head[id], p_chest[id], p_back[id], p_legs[id], p_hands[id], p_feet[id], p_wrists[id],
     p_ring1[id], p_ring2[id], p_neck[id] )
   format( inv, 512, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
     inv, p_inventory[id][10], p_inventory[id][11], p_inventory[id][12], p_inventory[id][13],
     p_inventory[id][14], p_inventory[id][15], p_inventory[id][16], p_inventory[id][17],
     p_inventory[id][18], p_inventory[id][19], p_inventory[id][20], p_inventory[id][21],
     p_inventory[id][22], p_inventory[id][23], p_inventory[id][24], p_inventory[id][25] )
   format( inv, 512, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
     inv, p_inventory[id][26], p_inventory[id][27], p_inventory[id][28], p_inventory[id][29],
     p_inventory[id][30], p_inventory[id][31], p_inventory[id][32], p_inventory[id][33],
     p_inventory[id][34], p_inventory[id][35], p_inventory[id][36], p_inventory[id][37],
     p_inventory[id][38], p_inventory[id][39], p_inventory[id][40], p_inventory[id][41] )
   format( inv, 512, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
     inv, p_inventory[id][42], p_inventory[id][43], p_inventory[id][44], p_inventory[id][45],
     p_inventory[id][46], p_inventory[id][47], p_inventory[id][48], p_inventory[id][49],
     p_inventory[id][50], p_inventory[id][51], p_inventory[id][52], p_inventory[id][53],
     p_inventory[id][54], p_inventory[id][55], p_inventory[id][56], p_inventory[id][57] )
   format( inv, 512, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
     inv, p_inventory[id][58], p_inventory[id][59], p_inventory[id][60], p_inventory[id][61],
     p_inventory[id][62], p_inventory[id][63], p_inventory[id][64], p_inventory[id][65],
     p_inventory[id][66], p_inventory[id][67], p_inventory[id][68], p_inventory[id][69],
     p_inventory[id][70], p_inventory[id][71], p_inventory[id][72], p_inventory[id][73] )
   format( inv, 512, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
     inv, p_inventory[id][74], p_inventory[id][75], p_inventory[id][76], p_inventory[id][77],
     p_inventory[id][78], p_inventory[id][79], p_inventory[id][80], p_inventory[id][81],
     p_inventory[id][82], p_inventory[id][83], p_inventory[id][84], p_inventory[id][85],
     p_inventory[id][86], p_inventory[id][87], p_inventory[id][88], p_inventory[id][89] )
   format( inv, 512, "%s %d %d %d %d %d %d %d %d %d %d",
     inv, p_inventory[id][90], p_inventory[id][91], p_inventory[id][92], p_inventory[id][93],
     p_inventory[id][94], p_inventory[id][95], p_inventory[id][96], p_inventory[id][97],
     p_inventory[id][98], p_inventory[id][99] )

   // Create data string for spells data
   format( data, 512, "%d %d %d %d %d %d %d %d %d %d",
     p_spells[id][0], p_spells[id][1], p_spells[id][2], p_spells[id][3],
     p_spells[id][4], p_spells[id][5], p_spells[id][6], p_spells[id][7],
     p_spells[id][8], p_spells[id][9] )
   format( data, 512, "%s %d %d %d %d %d %d %d %d %d %d",
     data, p_spells[id][10], p_spells[id][11], p_spells[id][12], p_spells[id][13],
     p_spells[id][14], p_spells[id][15], p_spells[id][16], p_spells[id][17],
     p_spells[id][18], p_spells[id][19] )
   format( data, 512, "%s %d %d %d %d %d %d %d %d %d %d",
     data, p_spells[id][20], p_spells[id][21], p_spells[id][22], p_spells[id][23],
     p_spells[id][24], p_spells[id][25], p_spells[id][26], p_spells[id][27],
     p_spells[id][28], p_spells[id][29] )
   format( data, 512, "%s %d %d %d %d %d %d %d %d %d %d",
     data, p_spells[id][30], p_spells[id][31], p_spells[id][32], p_spells[id][33],
     p_spells[id][34], p_spells[id][35], p_spells[id][36], p_spells[id][37],
     p_spells[id][38], p_spells[id][39] )
   format( data, 512, "%s %d %d %d %d %d %d %d %d %d %d",
     data, p_spells[id][40], p_spells[id][41], p_spells[id][42], p_spells[id][43],
     p_spells[id][44], p_spells[id][45], p_spells[id][46], p_spells[id][47],
     p_spells[id][48], p_spells[id][49] )
   format( data, 512, "%s %d %d %d %d %d %d %d %d %d %d",
     data, p_spells[id][50], p_spells[id][51], p_spells[id][52], p_spells[id][53],
     p_spells[id][54], p_spells[id][55], p_spells[id][56], p_spells[id][57],
     p_spells[id][58], p_spells[id][59] )

   // Create pdata string from all data for storage
   format( pdata, 1600, "%s %s %d %d %d %d", playerid, ip, p_race[id],
      p_class1[id], p_class2[id], p_xp[id] )
   format( pdata, 1600, "%s %d %d %d %d %d %d %d %d %d", pdata,p_str[id],p_agi[id],
      p_stam[id], p_int_unb[id], p_spir[id], p_armor[id], p_mana_base[id],
      p_gold[id], p_tier[id] )
   format( pdata, 1600, "%s %d %d %d %d %d %d %d %d", pdata, p_kills[id],
      p_deaths[id], p_rounds[id], p_casts[id], p_heals[id], p_bombs[id],
      p_defuses[id], p_supports[id] )
   format( pdata, 1600, "%s %d %d %d %d %d %d %d %d", pdata, p_melee[id],
      p_marksman[id], p_arcane[id], p_fire[id], p_frost[id], p_shadow[id],
      p_nature[id], p_holy[id] )
   format( pdata, 1600, "%s %s %s %s %d %d %d %d %d %d", pdata, data, inv, talents,
      p_assists[id], p_roguespec[id], p_rezzes[id], p_teamacts[id], p_badges[id], p_badges2[id] )

   set_vaultdata( pkey, pdata )

   log_amx( "In write_id() name=(%s) race=(%d) class1=(%d) class2=(%d) xp=(%d)", 
        playername, p_race[id], p_class1[id], p_class2[id], p_xp[id] )
   log_amx( "In write_id() name=(%s) str=(%d) agi=(%d) stam=(%d) int=(%d) spir=(%d) mana=(%d)", 
        playername, p_str[id], p_agi[id], p_stam[id], p_int_unb[id], p_spir[id], p_mana_base[id] )
   log_amx( "In write_id() name=(%s) gold=(%d) tier=(%d) inv=(%s)" , playername,
        p_gold[id], p_tier[id], playername, inv )
   log_amx( "In write_id() name=(%s) talents=(%s)" , playername, talents )

   client_print( id, print_chat, "[%s] Your WowMOD player data has been saved.", MOD )

   return PLUGIN_CONTINUE
 
}


// **************************************************************************
// BEGIN wowmod_save routine - Kept mostly intact from UWC3
// Save all player data
// **************************************************************************
public wowmod_save(id){
   if ( get_cvar_num("mp_savexp") ){
      if ( get_cvar_num("mp_sql") )
         sqlwrite_id( id )
      else
         write_id( id )

   }else{
      client_print(id,print_chat,"[%s] You cannot save in Fast XP mode", MOD)
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// END WowMOD SQL Storage
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

