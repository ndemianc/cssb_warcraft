/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Spoken Commands
*** Date:          Aug 12, 2007
*** Last Update:   Oct 25, 2007
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
// BEGIN WowMOD Spoken Commands 
// **************************************************************************


// **************************************************************************
// BEGIN check_say subroutine
// Say and Team_say handler, to check for issued spoken commands
// **************************************************************************
public check_say(id){

   new said[64]
   new tsaid[64]
   new arg1[64]
   new arg2[64]
   read_args( said, 63 )

   tsaid = said
   remove_quotes(tsaid)
   strbreak(tsaid, arg1, 64, arg2, 64)

   //client_print( id, print_chat, "arg1=(%s) arg2=(%s)", arg1, arg2 )

   if ( equali( said, "^"/save^"") || equali(said,"^"save^"")){
      wowmod_save( id )
   }else if ( equali( said, "^"/wowmod^"") || equali( said, "^"wowmod^"") ||
             equali( said, "^"/war3menu^"") || equali( said, "^"warcraft3^"") || 
             equali( said, "^"/changerace^"") || equali( said, "^"changerace^"") ||
             equali( said, "^"/mainmenu^"") || equali( said, "^"mainmenu^"") ||
             equali( said, "^"/wowmenu^"") || equali( said, "^"wowmenu^"") ||
             equali( said, "^"shopmenu^"") || equali( said, "^"shopmenu2^"") ||
             equali( said, "^"/wow^"" ) || equali( said, "^"wow^"" ) )
   {
      wowmod_display_mainmenu( id )
   }else if ( equali( said, "^"/commands^"") || equali( said, "^"commands^"") ){ 
      wowmod_info_commands( id )
   }else if ( equali( said, "^"/spellmenu^"") || equali( said, "^"spellmenu^"") ){ 
      wowmod_display_spellmerchant( id )
   }else if ( equali( said, "^"/merchantmenu^"") || equali( said, "^"merchantmenu^"") ){ 
      wowmod_display_merchantmenu( id )
   }else if ( equali( said, "^"/autocast^"") || equali( said, "^"autocast^"") ){ 
      wowmod_display_autocastmenu( id, 1 )
   }else if ( equali( said, "^"/sellspells^"") || equali( said, "^"sellspells^"") ){ 
      wowmod_display_sellspellmenu( id, 1 )
   }else if ( equali( said, "^"/sellequip^"") || equali( said, "^"sellequip^"") ){ 
      wowmod_display_sellequipmenu( id, 1 )
   }else if ( equali( said, "^"/help^"") || equali( said, "^"wowhelp^"") ) {
      wowmod_info_helpmain( id )
   }else if ( equali( arg1, "cast" ) || equali( arg1, "/cast" ) ){
      // Start spell casting checks here
      wowmod_cast( id, arg2 )
   }else if ( equali( arg1, "help" ) || equali( arg1, "/help" ) ){
      // Start help selection checks 
      wowmod_help( id, arg2 )
   }else if ( equali( said, "^"news^"" ) || equali( said, "^"/news^"" ) ){
      wowmod_info_news( id )
   }else if ( equali( said,"^"/who^"" ) || equali( said,"^"who^"" )){
      wowmod_info_who( id )
   }else if (equali(said,"^"/character^"") || equali(said,"^"character^"") ||
            equali(said,"^"/charactersheet^"") || equali(said,"^"charactersheet^"") ||
            equali(said,"^"/charsheet^"") || equali(said,"^"charsheet^""))
   {
      wowmod_info_charsheet( id )
   }else if (equali(said,"^"/spells^"") || equali(said,"^"spells^"") ||
            equali(said,"^"/myspells^"") || equali(said,"^"myspells^"") ||
            equali(said,"^"/charspells^"") || equali(said,"^"charspells^""))
   {
      wowmod_info_charsheet2( id )
   }else if ( equali(said,"^"/inventory^"") || equali(said,"^"inventory^"") ||
            equali(said,"^"/inv^"") || equali(said,"^"inv^"") )
   { 
      wowmod_info_inventory( id )
   }else if ( equali(said,"^"/worn^"") || equali(said,"^"worn^"") ||
            equali(said,"^"/equipment^"") )
   { 
      wowmod_info_worn( id )
   }else if ( equali( arg1, "/wear" ) || equali( arg1, "wear" ) ){ 
      wowmod_gear_wear( id, arg2 )
   }else if ( equali( arg1, "/wear_all" ) || equali( arg1, "wear_all" ) ){ 
      wowmod_gear_wear_all( id, false )
   }else if ( equali( arg1, "/remove" ) || equali( arg1, "remove" ) ||
              equali( arg1, "/rem" ) || equali( arg1, "rem" ) ){ 
      wowmod_gear_remove( id, arg2 )
   }else if ( equali( arg1, "/remove_all" ) || equali( arg1, "remove_all" ) ||
              equali( arg1, "/rem_all" ) || equali( arg1, "remall" ) ){ 
      wowmod_gear_remove_all( id, false )
   }else if ( equali( arg1, "/drop" ) || equali( arg1, "drop" ) ){
      wowmod_gear_drop( id, arg2 )
   }else if ( equali( arg1, "/examine" ) || equali( arg1, "examine" )  || equali( arg1, "/ex" ) ){ 
      wowmod_info_examine( id, arg2 )
   }else if ( equali( said, "^"/wowstats^"") ){ 
      wowmod_info_stats( id )
   }else if ( equali( said, "^"/allbadges^"") || equali( said, "^"/wowbadges^"") ){ 
      wowmod_display_allbadges( id )
   }else if ( equali( said, "^"/mybadges^"") ){ 
      wowmod_info_mybadges( id )
   }
   
   return PLUGIN_CONTINUE
}



// **************************************************************************
// END WowMOD Spoken Commands 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

