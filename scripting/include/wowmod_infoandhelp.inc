/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Information and Help Module
*** Date:          Aug 12, 2007
*** Last Update:   Dec 28, 2007
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
// BEGIN WowMOD Info and Help Module 
// **************************************************************************


// **************************************************************************
// BEGIN wowmod_info_ subroutine
// Display the credits for the WowMOD plugin
// **************************************************************************
public wowmod_info_(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new temp[3072]
   new pos = 0

   pos += format(temp[pos], 3072-pos, "%s", html_head )
   pos += format(temp[pos], 3072-pos, "<body>")
   pos += format(temp[pos], 3072-pos, "<body bgcolor=#000000><font color=#FFB000>")
   pos += format(temp[pos], 3072-pos, "<center><table border=0 cellpadding=3 cellspacing=3><tr><td><font color=#FFB000>")

   pos += format(temp[pos], 3072-pos, "<center><b>World of Warcraft Mod (WowMOD) v%s</b></center><p>", VER )

   pos += format(temp[pos], 3072-pos, "</font></td></tr></table></center>")

   show_motd( id, temp, "Page Title")

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_commands subroutine
// Display the list of wowmod commands
// **************************************************************************
public wowmod_info_commands(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new temp[3072]
   new pos = 0

   pos += format(temp[pos], 3072-pos, "%s", html_head )
   pos += format(temp[pos], 3072-pos, "<body>")
   pos += format(temp[pos], 3072-pos, "<center><table border=0 cellpadding=6 cellspacing=6><tr><td><font color=#FFB000>")

   pos += format(temp[pos], 3072-pos, "<center><b>WowMOD: Spoken Commands List</b><br /><br />" )
   pos += format(temp[pos], 3072-pos, "All of the following commands are issued via 'say' or 'team_say'<br />" )
   pos += format(temp[pos], 3072-pos, "<table border=0 cellpadding=5 cellspacing=5>" )
   pos += format(temp[pos], 3072-pos, "<tr><td>/wowmod</td><td>/help</td><td>/commands</td><td>/news</td></tr>" )
   pos += format(temp[pos], 3072-pos, "<tr><td>/charsheet</td><td>/spells</td><td>/spellmenu</td><td>/merchantmenu</td></tr>" )
   pos += format(temp[pos], 3072-pos, "<tr><td>/cast 'spell'</td><td>/inv</td><td>/worn</td><td>/examine 'item'</td></tr>" )
   pos += format(temp[pos], 3072-pos, "<tr><td>/wear 'item'</td><td>/wear_all</td><td>/rem 'item'</td><td>/rem_all</td></tr>" )
   pos += format(temp[pos], 3072-pos, "<tr><td>/drop 'item'</td><td>/wowstats</td><td>/wowbadges</td><td>/mybadges</td></tr>" )
   pos += format(temp[pos], 3072-pos, "<tr><td>/sellspells</td><td>/sellequip</td><td>/autocast</td><td>/save</td></tr>" )
   pos += format(temp[pos], 3072-pos, "<tr><td>/who</td><td></td><td></td><td></td></tr>" )

   pos += format(temp[pos], 3072-pos, "</table></td></tr></table></center>")

   show_motd( id, temp, "WowMOD: Spoken Commands")

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_mybadges subroutine
// Display the list of player's earned wowmod badges
// **************************************************************************
public wowmod_info_mybadges( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !( get_cvar_num("mp_savexp") ) ){
      wowmod_info_mybadges_fast( id )
      return PLUGIN_CONTINUE
   }

   new temp[4600]
   new desc[128]
   new classes[128]
   new pos = 0

   pos += format(temp[pos], 4600-pos, "%s", html_head )
   pos += format(temp[pos], 4600-pos, "<body>")
   pos += format(temp[pos], 4600-pos, "<body bgcolor=#000000><font color=#FFB000>")
   pos += format(temp[pos], 4600-pos, "<center><table border=0 cellpadding=3 cellspacing=3><tr><td><font color=#FFB000>")

   pos += format(temp[pos], 4600-pos, "<center><b>WowMOD: Your Earned Badges</b><br />", page )
   pos += format(temp[pos], 4600-pos, "Note: Equipment tiers with multiple badges as partial requirements only require 2 of the partials to complete the tier<br />", page )
   pos += format(temp[pos], 4600-pos, "<table border=0 cellpadding=5 cellspacing=5>" )
   pos += format(temp[pos], 4600-pos, "<tr><td align=center>Badge</td><td align=center>Description</td></tr>" )


   new vis_index = 0
   new bool:found = false
 
   for ( new j = 1; j < MAX_BADGES; j++ ){
      if ( equali( badge[j],"" ) )
         break

      //log_amx( "MIH j=(%d) badge=(%s) pos=(%d)", j, badge[j], pos )

      copy( desc, 127, b_desc[j] )
      copy( classes, 127, "" )
   
      if ( ( j <= 17 ) && ( p_badges[id] & b_bit[j] ) ){
         vis_index += 1
         if ( !found )
            found = true
         pos += format(temp[pos], 4600-pos, "<tr><td>%d. %s</td><td>%s</td></tr>",
               vis_index , badge[j], desc )
      }else if ( ( j > 17 ) && ( p_badges2[id] & b_bit[j] ) ){
         vis_index += 1
         if ( !found )
            found = true
         pos += format(temp[pos], 4600-pos, "<tr><td>%d. %s</td><td>%s</td></tr>",
               vis_index , badge[j], desc )
      }
   }

   if ( !found )
      pos += format(temp[pos], 4600-pos, "<tr><td align=center>No Badges Earned</td></tr>")

   pos += format(temp[pos], 4600-pos, "</table></td></tr></table></center>")

   show_motd( id, temp, "WowMOD: Your Badges")

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_mybadges_fast subroutine
// Display the list of player's earned wowmod badges - for fast-xp mode
// **************************************************************************
public wowmod_info_mybadges_fast( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new temp[4600]
   new classes[128]
   new pos = 0

   pos += format(temp[pos], 4600-pos, "%s", html_head )
   pos += format(temp[pos], 4600-pos, "<body>")
   pos += format(temp[pos], 4600-pos, "<body bgcolor=#000000><font color=#FFB000>")
   pos += format(temp[pos], 4600-pos, "<center><table border=0 cellpadding=3 cellspacing=3><tr><td><font color=#FFB000>")

   pos += format(temp[pos], 4600-pos, "<center><b>WowMOD: Your Earned Badges</b><br />", page )
   pos += format(temp[pos], 4600-pos, "Note: Equipment tiers with multiple badges as partial requirements only require 2 of the partials to complete the tier<br />", page )
   pos += format(temp[pos], 4600-pos, "<table border=0 cellpadding=5 cellspacing=5>" )
   pos += format(temp[pos], 4600-pos, "<tr><td>" )

   new bool:found = false
   new vis_index = 0
 
   for ( new j = 1; j < MAX_BADGES; j++ ){
      if ( equali( badge[j],"" ) )
         break

      //log_amx( "MIH j=(%d) badge=(%s) pos=(%d)", j, badge[j], pos )

      copy( classes, 127, "" )
   
      if ( ( j <= 17 ) && ( p_badges[id] & b_bit[j] ) ){
         vis_index += 1
         if ( !found )
            found = true
         pos += format(temp[pos], 4600-pos, "%d. %s <br />",
               vis_index , badge[j] )
      }else if ( ( j > 17 ) && ( p_badges2[id] & b_bit[j] ) ){
         vis_index += 1
         if ( !found )
            found = true
         pos += format(temp[pos], 4600-pos, "%d. %s <br />",
               vis_index , badge[j] )
      }
   }

   if ( !found )
      pos += format(temp[pos], 4600-pos, "<tr><td align=center>No Badges Earned</td></tr>")

   pos += format(temp[pos], 4600-pos, "</td></tr></table></center>")

   show_motd( id, temp, "WowMOD: Your Badges")

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_allbadges subroutine
// Display the list of all wowmod badges
// **************************************************************************
public wowmod_info_allbadges( id, page ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new temp[4600]
   new desc[128]
   new classes[128]
   new pos = 0

   pos += format(temp[pos], 4600-pos, "%s", html_head )
   pos += format(temp[pos], 4600-pos, "<body>")
   pos += format(temp[pos], 4600-pos, "<body bgcolor=#000000><font color=#FFB000>")
   pos += format(temp[pos], 4600-pos, "<center><table border=0 cellpadding=3 cellspacing=3><tr><td><font color=#FFB000>")

   pos += format(temp[pos], 4600-pos, "<center><b>WowMOD: Badge Info (page %d)</b><br /><br />", page )
   pos += format(temp[pos], 4600-pos, "<table border=0 cellpadding=5 cellspacing=5>" )
   pos += format(temp[pos], 4600-pos, "<tr><td align=center>Badge</td><td align=center>Description</td><td>Classes</td></tr>" )

   new startidx = ( page - 1 ) * 5 + 1 
   new endidx = ( startidx + 5 ) < num_badges ? startidx + 5 : num_badges
 
   for ( new j = startidx; j < endidx; j++ ){
      if ( equali( badge[j],"" ) )
         break

      //log_amx( "MIH j=(%d) badge=(%s) pos=(%d)", j, badge[j], pos )

      copy( desc, 127, b_desc[j] )
      copy( classes, 127, "" )

      if ( b_classes[j] == CB_ALL ){
         format( classes, 127, "All Classes |" )
      }else{
         if ( CB_DRUID & b_classes[j] )
            format( classes, 127, " Druid |" )
         if ( CB_HUNTER & b_classes[j] )
            format( classes, 127, "%s Hunter |", classes )
         if ( CB_MAGE & b_classes[j] )
            format( classes, 127, "%s Mage |", classes )
         if ( CB_ROGUE & b_classes[j] )
            format( classes, 127, "%s Rogue |", classes )
         if ( CB_WARLOCK & b_classes[j] )
            format( classes, 127, "%s Warlock |", classes )
         if ( CB_PRIEST & b_classes[j] )
            format( classes, 127, "%s Priest |", classes )
      }

      pos += format(temp[pos], 4600-pos, "<tr><td>%d. %s</td><td>%s</td><td>| %s</td></tr>",
               j , badge[j], desc, classes )
   }

   pos += format(temp[pos], 4600-pos, "</table></td></tr></table></center>")

   show_motd( id, temp, "WowMOD: Badge Info")

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_worn subroutine
// Display a player's worn equipment
// **************************************************************************
public wowmod_info_worn(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new temp[5000]
   new pos = 0

   pos += format(temp[pos], 5000-pos, "%s", html_head )
   pos += format(temp[pos], 5000-pos, "<body>")
   pos += format(temp[pos], 5000-pos, "<img src='http://www.cswowmod.com/forums/images/wowmodlogo.gif'><br />")
   pos += format(temp[pos], 5000-pos, "<center><table border=0 cellpadding=3 cellspacing=3><tr><td colspan=4>")

   pos += format(temp[pos], 5000-pos, "<center><b>WowMOD: Your Worn Equipment</b></center>" )

   pos += format(temp[pos], 5000-pos, "<center><b>Equipment Worn</b></center></td></tr>" )
   pos += format(temp[pos], 5000-pos, "<tr><td width=100>Head</td><td width=200>%s</td><td width=100>Feet</td><td width=200>%s</td></tr>", gear[p_head[id]], gear[p_feet[id]] )
   pos += format(temp[pos], 5000-pos, "<tr><td>Chest</td><td>%s</td><td>Wrists</td><td>%s</td></tr>", gear[p_chest[id]], gear[p_wrists[id]] )
   pos += format(temp[pos], 5072-pos, "<tr><td>Back</td><td>%s</td><td>Ring1</td><td>%s</td></tr>", gear[p_back[id]], gear[p_ring1[id]] )
   pos += format(temp[pos], 5000-pos, "<tr><td>Legs</td><td>%s</td><td>Ring2</td><td>%s</td></tr>", gear[p_legs[id]], gear[p_ring2[id]] )
   pos += format(temp[pos], 5000-pos, "<tr><td>Hands</td><td>%s</td><td>Neck</td><td>%s</td></tr>", gear[p_hands[id]], gear[p_neck[id]] )

   pos += format(temp[pos], 5000-pos, "</font></td></tr></table><br /><br />")
   pos += format(temp[pos], 5000-pos, "[ say /examine item_name - See full details about a piece of WowMOD gear ]<br />" )
   pos += format(temp[pos], 5000-pos, "[ Say /inv - view your stored inventory ]</center>")

   show_motd( id, temp, "WowMOD: Worn Equipment")

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_inventory subroutine
// Display a player's inventory
// **************************************************************************
public wowmod_info_inventory(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new temp[5000]
   new pos = 0

   pos += format(temp[pos], 5000-pos, "%s", html_head )
   pos += format(temp[pos], 5000-pos, "<body>")
   pos += format(temp[pos], 5000-pos, "<img src='http://www.cswowmod.com/forums/images/wowmodlogo.gif'><br />")
   pos += format(temp[pos], 5000-pos, "<center><table border=0 cellpadding=3 cellspacing=3><tr><td colspan=4>")

   pos += format(temp[pos], 5000-pos, "<center><b>WowMOD: Player Inventory</b><br />" )
   pos += format(temp[pos], 5000-pos, "[ say /examine item_name - See full details about a piece of WowMOD gear ]<br />" )
   pos += format(temp[pos], 5000-pos, "[ say /worn - view your worn equipment ]</center><br />" )

   new bool:bFirst = true
   new count[MAX_GEAR] = 0
   new ifound[MAX_GEAR] = 0

   for( new j = 10; j < MAX_INV; j++ )
      if ( p_inventory[id][j] > 0 )
         count[p_inventory[id][j]] += 1


   for( new j = 10; j < MAX_INV; j++ ){
      if ( p_inventory[id][j] > 0 ){
         ifound[p_inventory[id][j]] += 1

         if ( ifound[p_inventory[id][j]] > 1 ) continue

         if ( bFirst ){
            bFirst = false
            pos += format( temp[pos], 5000-pos, "%s(x%d) ", 
               gear[p_inventory[id][j]], count[p_inventory[id][j]] )
         }else{
            pos += format( temp[pos], 5000-pos, " , %s(x%d) ", 
               gear[p_inventory[id][j]], count[p_inventory[id][j]] )
         }
      }
   }
   

   pos += format(temp[pos], 5000-pos, "</td></tr></table></center>" )

   pos += format(temp[pos], 5000-pos, "</font></td></tr></table></center>")

   show_motd( id, temp, "WowMOD: Your Inventory")

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_wowmod subroutine
// Display the credits for the WowMOD plugin
// **************************************************************************
public wowmod_info_helpmain(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new temp[3072]
   new pos = 0

   pos += format(temp[pos], 3072-pos, "%s", html_head )
   pos += format(temp[pos], 3072-pos, "<body>")
   pos += format(temp[pos], 3072-pos, "<center><table border=0 cellpadding=3 cellspacing=3><tr><td>")
   pos += format(temp[pos], 3072-pos, "<img src='http://www.cswowmod.com/forums/images/wowmodlogo.gif'><br />")

   pos += format(temp[pos], 3072-pos, "<p><center><b>WowMOD: Help System</b></center></p><br />" )
   pos += format(temp[pos], 3072-pos, "<p>To access the WowMOD Main Menu say 'wowmod' or just 'wow'</p><br />" )
   pos += format(temp[pos], 3072-pos, "<p>Help is available for the following topics</p></b></center></p>" )
   pos += format(temp[pos], 3072-pos, "<ul><li>Commands - say /commands to see a list of spoken commands<br />" )
   pos += format(temp[pos], 3072-pos, "<li>Races - say help races <br />" )
   pos += format(temp[pos], 3072-pos, "<li>Classes - say help classes <br />" )
   pos += format(temp[pos], 3072-pos, "<li>Spells - say help spells <br />" )
   pos += format(temp[pos], 3072-pos, "<li>Racial Abilities - say help racials <br />" )
   pos += format(temp[pos], 3072-pos, "<li>Attributes - say help attributes <br />" )
   pos += format(temp[pos], 3072-pos, "<li>Proficiencies - say help proficiencies <br />" )
   pos += format(temp[pos], 3072-pos, "<li>Critical Strike - say help crit <br />" )
   pos += format(temp[pos], 3072-pos, "<li>Merchants - say help merchants <br />" )
   pos += format(temp[pos], 3072-pos, "<li>Inventory - say help inventory (say /inv to see inventory, say /worn to see worn gear)<br />" )
   pos += format(temp[pos], 3072-pos, "<li>Stats - say /wowstats to view your statistics<br />" )
   pos += format(temp[pos], 3072-pos, "<li>Character Sheet - say /charsheet to view your Character<br />" )
   pos += format(temp[pos], 3072-pos, "<li>Your Spells - say /spells to view your known spells<br />" )
   pos += format(temp[pos], 3072-pos, "<li>News - say /news to see the latest WowMOD news<br />" )
   pos += format(temp[pos], 3072-pos, "</ul><br />" )

   pos += format(temp[pos], 3072-pos, "</td></tr></table></center>")

   show_motd( id, temp, "WowMOD Help")

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_stats subroutine
// Display the player stats for the WowMOD plugin
// **************************************************************************
public wowmod_info_stats(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new name[32]
   new temp[3072]
   new pos = 0

   get_user_name( id, name, 31 )

   pos += format(temp[pos], 3072-pos, "%s", html_head )
   pos += format(temp[pos], 3072-pos, "<body>")
   pos += format(temp[pos], 3072-pos, "<center><table border=0 cellpadding=3 cellspacing=3><tr><td>")
   pos += format(temp[pos], 3072-pos, "<img src='http://www.cswowmod.com/forums/images/wowmodlogo.gif'><br />")
   pos += format(temp[pos], 3072-pos, "<p><center><b>WowMOD: Stats for %s</b></center></p><br />", name )
   pos += format(temp[pos], 3072-pos, "<center><table border=0 cellpadding=4 cellspacing=4>")
   pos += format(temp[pos], 3072-pos, "<tr><td></td><td>Session Stats</td><td>Lifetime Stats</td></tr>")
   pos += format(temp[pos], 3072-pos, "<tr><td>Rounds Played</td><td>%d</td><td>%d</td></tr>", p_rounds_s[id], p_rounds[id] )
   pos += format(temp[pos], 3072-pos, "<tr><td>Kills</td><td>%d</td><td>%d</td></tr>", p_kills_s[id], p_kills[id] )
   pos += format(temp[pos], 3072-pos, "<tr><td>Deaths</td><td>%d</td><td>%d</td></tr>", p_deaths_s[id], p_deaths[id] )
   pos += format(temp[pos], 3072-pos, "<tr><td>Kill Assists</td><td>%d</td><td>%d</td></tr>", p_assists_s[id], p_assists[id] )
   pos += format(temp[pos], 3072-pos, "<tr><td>Spells Cast</td><td>%d</td><td>%d</td></tr>", p_casts_s[id], p_casts[id] )
   pos += format(temp[pos], 3072-pos, "<tr><td>Heals Cast</td><td>%d</td><td>%d</td></tr>", p_heals_s[id], p_heals[id] )
   pos += format(temp[pos], 3072-pos, "<tr><td>Bombs Planted</td><td>%d</td><td>%d</td></tr>", p_bombs_s[id], p_bombs[id] )
   pos += format(temp[pos], 3072-pos, "<tr><td>Bombs Defused</td><td>%d</td><td>%d</td></tr>", p_defuses_s[id], p_defuses[id] )
   pos += format(temp[pos], 3072-pos, "<tr><td>Supporting Roles</td><td>%d</td><td>%d</td></tr>", p_supports_s[id], p_supports[id] )
   pos += format(temp[pos], 3072-pos, "<tr><td>Rogue Special Attacks</td><td>%d</td><td>%d</td></tr>", p_roguespec_s[id], p_roguespec[id] )
   pos += format(temp[pos], 3072-pos, "<tr><td>Priest Resurrections</td><td>%d</td><td>%d</td></tr>", p_rezzes_s[id], p_rezzes[id] )
   pos += format(temp[pos], 3072-pos, "<tr><td>Team Assist Actions</td><td>%d</td><td>%d</td></tr>", p_teamacts_s[id], p_teamacts[id] )
   pos += format(temp[pos], 3072-pos, "</table></center>")
   pos += format(temp[pos], 3072-pos, "</td></tr></table></center>")

   show_motd( id, temp, "WowMOD Stats")

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_news subroutine
// Display the News for the WowMOD plugin
// **************************************************************************
public wowmod_info_news(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new temp[3072]
   new pos = 0

   pos += format(temp[pos], 3072-pos, "%s", html_head )
   pos += format(temp[pos], 3072-pos, "<body>")
   pos += format(temp[pos], 3072-pos, "<img src='http://www.cswowmod.com/forums/images/wowmodlogo.gif'><br />")

   pos += format(temp[pos], 3072-pos, "<center><b>WowMOD: Recent Changelog</b></center><p>" )
   pos += format(temp[pos], 3072-pos, "[02/21/2008] Bug Fix: Array index out of range in dmgevent(), added cooldown to healing spells to prevent heal spam.<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[02/17/2008] New Command: say /who to display a list of players and their levels<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[02/15/2008] Bug Fix: Index out of range error fixed in spell selection routines<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[02/14/2008] Bug Fix: Resurrection now makes sure the caster is alive before casting Resurrection<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[02/12/2008] Bug Fix: Fixed the enforcement of minimum level requirement for spell training<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[02/10/2008] Bug Fix: Fixed the dual-class selection not imposing the minimum required level<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[02/03/2008] Numerous Bug Fixes including Heal spell mana check, Power Word: Shield carrying over to next round, etc<br /><br />" )

   /*
   pos += format(temp[pos], 3072-pos, "[11/16/2007] The first magic item with a special ability is now available at the merchant, look for 'Hearthstone Ring', /examine it to see an explanation of its use. This item will be the test bed for the entire special items system so please report any bugs.<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[11/14/2007] Tier9 and Tier10 armor is now in. Autocast system is now in, say /autocast to bring up the Autocast menu for selecting spells to autocast. These are not yet saved, you must do this every time you enter the server for now. Added new help entries for Spell Crit, Spell Damage and Autocast. <br /><br />" )
   pos += format(temp[pos], 3072-pos, "[11/10/2007] New commands added: say /sellspells , say /sellequip<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[11/07/2007] Added Tier8 armor sets. Added the ability to sell back spell ranks (from /wowmenu select option 5 then option 4)<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[10/31/2007] Happy Halloween! Added Tier7 armor sets for all classes. All Tier5 and tier6 epic armor set prices have been increased (was too cheap).<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[10/30/2007] Added Bindable Console Commands: /wowmod, /spells, /spellmenu, /inv, /worn, /wowstats, /mybadges (spoken forms are not bindable for menus)<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[10/27/2007] Bug Fix: Fall/world damage no longer goes through crit/spell check processing. Modified the badge requirements for the reinforcement line. <br /><br />" )
   pos += format(temp[pos], 3072-pos, "[10/24/2007] Added achievement badges (say cmds: /allbadges and /mybadges). Epic Equipment tiers 5 and 6 now unlockable via badges, full armor sets added for each class. Added two new stats, spell crit and spell dmg modifier, you'll start to see these on new epic gear. Fixed Druid Thorns spell (caused backfire dmg). New say cmds: /wear_all and /remove_all<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[10/15/2007] Added dual-classing, you can now choose a secondary class at level 30, you may train up to 20 spell ranks in your secondary class.<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[10/14/2007] Fixed Ressurection spell, please test. Fixed Sprint 'passive spell' merchant text<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[10/11/2007] Added required confirmation before all race/class changes can occur. Fixed some broken menu options in race menus<br /><br />" )
   pos += format(temp[pos], 3072-pos, "[10/10/2007] The Rogue class is in, all classes are now playable<br />" )
   pos += format(temp[pos], 3072-pos, "[10/08/2007] The Warlock class is in, please test playing. Spell curse of agony changed to Lifetap. Chance for all DOTs to land decreased. New cmd: say /worn to see worn equipment<br />" )
   pos += format(temp[pos], 3072-pos, "[10/07/2007] The Priest class is in, please test away (specifically Resurrection which doesn't work on bots). Fixed the worn equipment modifier bug (from changing race/class). Fixed the Merchant not having inventory bug. Added Tier3 and Tier4 Armor sets for all classes.<br />" )
   pos += format(temp[pos], 3072-pos, "[10/03/2007] Necklace Stacking Bug Fixed - Players XP table reset. Moved spells from charsheet to /spells command. Critical Strike gear added ( Wrist slot - bracers/wristguards ).<br />" )
   pos += format(temp[pos], 3072-pos, "[10/02/2007] All Racials should now be bound to 'wow_racial'. Armor and Magic Items now cost real gold to purchase. Added first batch of Magical Necklaces, Added first batch of Cloaks.<br />" )
   pos += format(temp[pos], 3072-pos, "[09/30/2007] Added first batch of Magical Rings to Items Merchant.<br />" )
   pos += format(temp[pos], 3072-pos, "[09/29/2007] WowMOD Dev Server opened with pre-alpha version of WowMOD.<br />" )
   */

   pos += format(temp[pos], 3072-pos, "</font></td></tr></table></center>")

   show_motd( id, temp, "WowMOD News")

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_wowmod subroutine
// Display the credits for the WowMOD plugin
// **************************************************************************
public wowmod_info_wowmod(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new temp[3072]
   new pos = 0

   pos += format(temp[pos], 3072-pos, "%s", html_head )
   pos += format(temp[pos], 3072-pos, "<body>")
   pos += format(temp[pos], 3072-pos, "<img src='http://www.cswowmod.com/forums/images/wowmodlogo.gif'><br />")

   pos += format(temp[pos], 3072-pos, "<center><b>World of Warcraft Mod (WowMOD) v%s</b></center><p>", VER )
   pos += format(temp[pos], 3072-pos, "<p>WowMOD is a new Amx Modx CS plugin built by K2mia which continues the efforts he began with his Ultimate Warcraft (UWC3) plugin.</p>" )
   pos += format(temp[pos], 3072-pos, "<p>In WowMOD the race-based system of play made popular by Warcraft3XP and WarcraftFT returns with many of the popular races from the World of Warcraft MMORPG available.  WowMOD has the following features:</p><ul>" )
   pos += format(temp[pos], 3072-pos, "<li>80 levels of play<br />" )
   pos += format(temp[pos], 3072-pos, "<li>Races have unique traits and a special racial ability<br />" )
   pos += format(temp[pos], 3072-pos, "<li>Dual-classing, players may choose a primary and secondary class( each class offers 8 spells, each with 10-20 ranks to train )<br />" )
   pos += format(temp[pos], 3072-pos, "<li>Merchant and armourer systems which use a currency that is seperate from the normal CS cashflow.<br />" )
   pos += format(temp[pos], 3072-pos, "<li>Much more<br /></ul><br />" )
   pos += format(temp[pos], 3072-pos, "<p>Note: WowMOD is a work in progress, please check back often as this exciting new plugin is developed" )

   pos += format(temp[pos], 3072-pos, "</font></td></tr></table></center>")

   show_motd( id, temp, "WowMOD Info")

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_credits subroutine
// Display the credits for the WowMOD plugin
// **************************************************************************
public wowmod_info_credits(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new temp[3072]
   new pos = 0

   pos += format(temp[pos], 3072-pos, "%s", html_head )
   pos += format(temp[pos], 3072-pos, "<body>")
   pos += format(temp[pos], 3072-pos, "<img src='http://www.cswowmod.com/forums/images/wowmodlogo.gif'><br />")
   pos += format(temp[pos], 3072-pos, "<center><table border=0 cellpadding=3 cellspacing=3><tr><td><font color=#FFB000>")

   pos += format(temp[pos], 3072-pos, "<center><b>World of Warcraft Mod (WowMOD) v%s</b></center><p>", VER )
   pos += format(temp[pos], 3072-pos, "<p>WowMOD Programming: K2mia (Andrew Cowan) [ admin@cswowmod.com ]</p>" )
   pos += format(temp[pos], 3072-pos, "<p>Graphic Effects kept from UWC3: steve french [ garbageweed@hotmail.com ]</p>" )
   pos += format(temp[pos], 3072-pos, "<p>Original Game Concept / Some Code Derived from War3FT</p>" )

   pos += format(temp[pos], 3072-pos, "<p>Miscellaneous: str_break() routine provided by BAILOPAN from AMXX0.20 to replace parse()</p>" )
   pos += format(temp[pos], 3072-pos, "</font></td></tr></table></center>")

   show_motd( id, temp, "WowMOD Credits")

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_info_who subroutine
// Display the list of players
// **************************************************************************
public wowmod_info_who( id ){
   new message[4096] = ""
   new name[32]
   new team[32]
   new players[32]
   new numplayers
   get_players(players, numplayers)
   new i
   new playerid
   new pos = 0

   pos += format( message[pos], 4096-pos, "%s", html_head )
   pos += format( message[pos], 4096-pos, "<body>" )
   pos += format( message[pos], 4096-pos, "<img src='http://www.cswowmod.com/forums/images/wowmodlogo.gif'><br />" )
   pos += format( message[pos], 4096-pos, "<body bgcolor=#000000 text=#FFB000>^n" )
   pos += format( message[pos], 4096-pos, "<center><b>The following people are playing on this server now</b></center><p>^n" )
   pos += format( message[pos], 4096-pos, "<center><table width=800 border=0 cellpadding=3 cellspacing=3>^n" )
   pos += format( message[pos], 4096-pos, "<tr><td>Name</td><td>Team</td><td>Level</td></tr>^n" )

   for (i=0; i< numplayers; i++){
      playerid = players[i]

      if ( get_user_team(playerid) == TEAM_T ) team = "Terrorist"
      else if ( get_user_team(playerid) == TEAM_CT ) team = "CT"
      else team = "Spectator"

      get_user_name( playerid, name, 31 )

      pos += format( message[pos], 4096-pos, "<tr><td>%s</td><td>%s</td><td>%d</td></tr>^n",
                     name, team, p_level[playerid], name)
   }

   pos += format( message[pos], 4096-pos, "</table></center>^n" )

   /*
   new whofile[64]
   get_configsdir( whofile, 63 )
   format( whofile, 63, "%s/WowMOD/wowmod_who.txt", whofile )

   if ( file_exists( whofile ) )
      delete_file( whofile )

   write_file( whofile, message )
   */

   show_motd( id, message, "WowMOD: Playing Now" )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_charsheet subroutine
// Display a player's character sheet
// **************************************************************************
public wowmod_info_charsheet(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new name[32]
   new message[4096] = ""
   new stemp[4096] = ""
   new class2[32]

   if ( ( p_race[id] < 0 ) || ( p_race[id] > NUM_RACES ) ){
      client_print( id, print_chat, "[%s] You must first select a race/class", MOD )
      return PLUGIN_HANDLED
   }

   if ( p_class2[id] >= 0 ) 
      copy( class2, 31, classes[p_class2[id]] )
   else
      copy( class2, 31, "NA" )

   get_user_name(id, name, 31)

   add( message, 3072, html_head )
   add( message, 3072, "<body>" )
   add( message, 3072, "<img src='http://www.cswowmod.com/forums/images/wowmodlogo.gif'><br />" )
   format( stemp, 1024, "<center><b>WowMOD Character Sheet: %s</b></center><p>", name )
   add( message, 3072, stemp )
   add( message, 3072, "<center><table border=0 cellpadding=4 cellspacing=4>" )

   add( message, 3072, "<tr><td valign=top width=50%>" )

   format( stemp, 1024, "Player Name: %s <br>", name )
   add( message, 3072, stemp )
   format( stemp, 1024, "Player Race: %s <br>", races[p_race[id]] )
   add( message, 3072, stemp )
   format( stemp, 4096, "Player Class: %s / %s <br>", classes[p_class1[id]], class2 )
   add( message, 4096, stemp )

   if ( p_class1[id] != CLASS_ROGUE ){
      format( stemp, 4096, "Mana: %d <br>", p_mana[id] )
      add( message, 2047, stemp )
   }else{
      format( stemp, 4096, "Energy: %d <br>", p_energy[id] )
      add( message, 4096, stemp )
   }

   add( message, 4096, "</td><td valign=top width=50%>" )

   format( stemp, 4096, "Player Level: %d <br>", p_level[id] )
   add( message, 4096, stemp )
   if (p_level[id] == (MAX_LEVEL-1)){
      format( stemp, 4096, "Player XP: %d <br>", p_xp[id] )
   }else{
      format( stemp, 4096, "Player XP: %d / %d <br>",
         p_xp[id], xplevel_lev[p_level[id]+1] )
   }
   add( message, 4096, stemp )

   if (p_level[id] == (MAX_LEVEL-1)){
      add( message, 4096, "XP Needed: NA <br>" )
   }else{
      format( stemp, 4096, "XP Needed: %d <br>",
         (xplevel_lev[p_level[id]+1] - p_xp[id]) )
      add( message, 4096, stemp )
   }

   format( stemp, 4096, "Gold: %d <br>", p_gold[id] )
   add( message, 4096, stemp )

   add( message, 4096, "</td></tr><tr><td valign=top>" )

   add( message, 4096, "<p><b>Character Attributes</b></p>" )
   format( stemp, 4096, "Critical Strike: %d <br>", p_crit[id] )
   add( message, 4096, stemp )
   format( stemp, 4096, "Spell Crit: %d <br>", p_spcrit[id] )
   add( message, 4096, stemp )
   format( stemp, 4096, "Spell Damage: %d <br>", p_spdmg[id] )
   add( message, 4096, stemp )
   format( stemp, 4096, "Strength: %d / %d <br>", p_str[id], MAX_ATTRIBS )
   add( message, 4096, stemp )
   format( stemp, 4096, "Agility: %d / %d <br>", p_agi[id], MAX_ATTRIBS )
   add( message, 4096, stemp )
   format( stemp, 4096, "Stamina: %d / %d <br>", p_stam[id], MAX_ATTRIBS )
   add( message, 4096, stemp )
   format( stemp, 4096, "Intellect: %d / %d <br>", p_int[id], MAX_ATTRIBS )
   add( message, 4096, stemp )
   format( stemp, 4096, "Spirit: %d / %d <br>", p_spir[id], MAX_ATTRIBS )
   add( message, 4096, stemp )
   format( stemp, 4096, "Armor: %d / %d <br>", p_armor[id], MAX_ATTRIBS )
   add( message, 4096, stemp )

   add( message, 4096, "</td><td valign=top width=50%>" )

   add( message, 4096, "<p><b>Character Proficiencies</b></p>" )
   format( stemp, 4096, "Melee: %d / %d <br>", p_melee[id], MAX_ATTRIBS )
   add( message, 4096, stemp )
   format( stemp, 4096, "Marksman: %d / %d <br>", p_marksman[id], MAX_ATTRIBS )
   add( message, 4096, stemp )
   format( stemp, 4096, "Arcane: %d / %d <br>", p_arcane[id], MAX_ATTRIBS )
   add( message, 4096, stemp )
   format( stemp, 4096, "Fire: %d / %d <br>", p_fire[id], MAX_ATTRIBS )
   add( message, 4096, stemp )
   format( stemp, 4096, "Frost: %d / %d <br>", p_frost[id], MAX_ATTRIBS )
   add( message, 4096, stemp )
   format( stemp, 4096, "Shadow: %d / %d <br>", p_shadow[id], MAX_ATTRIBS )
   add( message, 4096, stemp )
   format( stemp, 4096, "Nature: %d / %d <br>", p_nature[id], MAX_ATTRIBS )
   add( message, 4096, stemp )
   format( stemp, 4096, "Holy: %d / %d <br>", p_holy[id], MAX_ATTRIBS )
   add( message, 4096, stemp )

   add( message, 4096, "</td></tr>" )
   add( message, 4096, "<tr><td colspan=2 align=center>" )
   add( message, 4096, "<br />Say /spells to view your known spells" )
   add( message, 4096, "</td></tr>" )

   add( message, 4096,"</table></center>" )

   show_motd( id, message, "WowMOD: Your Character" )
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_info_charsheet2 subroutine
// Display a player's known spells 
// **************************************************************************
public wowmod_info_charsheet2(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new name[32]
   new message[4096] = ""
   new temp[4096] = ""
   new stemp[4096] = ""
   new temp2[64] = ""
   new sname[64] = ""
   new i = 0, j = 0

   if ( ( p_race[id] < 0 ) || ( p_race[id] > NUM_RACES ) ){
      client_print( id, print_chat, "[%s] You must first select a race/class", MOD )
      return PLUGIN_HANDLED
   }

   get_user_name(id, name, 31)

   add( message, 3072, html_head )
   add( message, 3072, "<body>" )
   add( message, 3072, "<img src='http://www.cswowmod.com/forums/images/wowmodlogo.gif'><br />" )
   format( stemp, 1024, "<center><b>WowMOD: Spells Known By %s</b></center><p>", name )
   add( message, 3072, stemp )
   add( message, 3072, "<br /><center><table border=0 cellpadding=4 cellspacing=4>" )

   new sidx = 0

   new num_iter = ( ( p_class2[id] > -1 ) && ( p_class1[id] != p_class2[id] ) ) ? 2 : 1

   for ( i = 0; i < num_iter; i++ ){
    for ( j = 0; j < 8; j++ ){
      if ( ((i==0) && (p_class1[id] == CLASS_DRUID)) || ((i==1) && (p_class2[id] == CLASS_DRUID)) ){
         sidx = SPELL_DRUID_BASE_IDX + j 
         if ( p_spells[id][sidx] > 0 ){
            copy( sname, 31, druid_spells[j] )
            if ( p_spells[id][sidx] == druid_spmax[j] )
               format(temp2, 64, "<tr><td>%s</td><td>[ Master ]</td></tr> ",  sname )
            else
               format(temp2, 64, "<tr><td>%s</td><td>[ Spellrank %d ]</td></tr>",  sname, p_spells[id][sidx] )
            add(temp,4096,temp2)
         }
      }else if ( ((i==0) && (p_class1[id] == CLASS_HUNTER)) || ((i==1) && (p_class2[id] == CLASS_HUNTER)) ){
         sidx = SPELL_HUNTER_BASE_IDX + j 
         if ( p_spells[id][sidx] > 0 ){
            copy( sname, 31, hunter_spells[j] )
            if ( p_spells[id][sidx] == hunter_spmax[j] )
               format(temp2, 64, "<tr><td>%s</td<><td>[ Master ]</td></tr>",  sname )
            else
               format(temp2, 64, "<tr><td>%s</td><td>[ Spellrank %d ]</td></tr>",  sname, p_spells[id][sidx] )
            add(temp,4096,temp2)
         }
      }else if ( ((i==0) && (p_class1[id] == CLASS_MAGE)) || ((i==1) && (p_class2[id] == CLASS_MAGE)) ){
         sidx = SPELL_MAGE_BASE_IDX + j 
         if ( p_spells[id][sidx] > 0 ){
            copy( sname, 31, mage_spells[j] )
            if ( p_spells[id][sidx] == mage_spmax[j] )
               format(temp2, 64, "<tr><td>%s</td><td>[ Master ]</td></tr>",  sname )
            else
               format(temp2, 64, "<tr><td>%s</td><td>[ Spellrank %d ]</td></tr>",  sname, p_spells[id][sidx] )
            add(temp,4096,temp2)
         }
      }else if ( ((i==0) && (p_class1[id] == CLASS_ROGUE)) || ((i==1) && (p_class2[id] == CLASS_ROGUE)) ){
         sidx = SPELL_ROGUE_BASE_IDX + j 
         if ( p_spells[id][sidx] > 0 ){
            copy( sname, 31, rogue_spells[j] )
            if ( p_spells[id][sidx] == rogue_spmax[j] )
               format(temp2, 64, "<tr><td>%s</td><td>[ Master ]</td></tr>",  sname )
            else
               format(temp2, 64, "<tr><td>%s</td><td>[ Spellrank %d ]</td></tr>",  sname, p_spells[id][sidx] )
            add(temp,4096,temp2)
         }
      }else if ( ((i==0) && (p_class1[id] == CLASS_WARLOCK)) || ((i==1) && (p_class2[id] == CLASS_WARLOCK)) ){
         sidx = SPELL_WARLOCK_BASE_IDX + j 
         if ( p_spells[id][sidx] > 0 ){
            copy( sname, 31, warlock_spells[j] )
            if ( p_spells[id][sidx] == warlock_spmax[j] )
               format(temp2, 64, "<tr><td>%s</td><td>[ Master ]</td></tr>",  sname )
            else
               format(temp2, 64, "<tr><td>%s</td><td>[ Spellrank %d ]</td></tr>",  sname, p_spells[id][sidx] )
            add(temp,4096,temp2)
         }
      }else if ( ((i==0) && (p_class1[id] == CLASS_PRIEST)) || ((i==1) && (p_class2[id] == CLASS_PRIEST)) ){
         sidx = SPELL_PRIEST_BASE_IDX + j 
         if ( p_spells[id][sidx] > 0 ){
            copy( sname, 31, priest_spells[j] )
            if ( p_spells[id][sidx] == priest_spmax[j] )
               format(temp2, 64, "<tr><td>%s</td><td>[ Master ]</td></tr>",  sname )
            else
               format(temp2, 64, "<tr><td>%s</td><td>[ Spellrank %d ]</td></tr>",  sname, p_spells[id][sidx] )
            add(temp,4096,temp2)
         }
      }
    }
   }

   add( message, 4096, temp )
   add( message, 4096, "</td></tr>" )

   add( message, 4096,"</table></center>" )

   show_motd( id, message, "WowMOD: Your Spells" )
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_info_examine subroutine
// Examine an item or piece of gear
// **************************************************************************
public wowmod_info_examine( id, arg[] ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new temp[3072]
   new temp2[3072]
   new pos = 0, pos2 = 0
   new bool:found = false

   for ( new i = 0; i < MAX_GEAR; i++ ){
      if ( equali( gear[i], arg ) ){
         found = true
         pos2 += format( temp2[pos2], 3072-pos2, "<center>Examining: %s</center>", gear[i] )
         pos2 += format( temp2[pos2], 3072-pos2, "<table border=0 cellpadding=1 cellspacing=1>" )
         pos2 += format( temp2[pos2], 3072-pos2, "<tr><td valign=top>" )
         pos2 += format( temp2[pos2], 3072-pos2, "<center>Gear Info</center><br />" )
         pos2 += format( temp2[pos2], 3072-pos2, "<table border=0 cellpadding=2 cellspacing=2>" )
         pos2 += format( temp2[pos2], 3072-pos2, "<tr><td>Equipment Tier:</td><td>[%d]%s</td></tr>", g_tier[i], tiers[g_tier[i]] )
         pos2 += format( temp2[pos2], 3072-pos2, "<tr><td>Equipment Cost:</td><td>%d Gold</td></tr>", g_cost[i] )
         pos2 += format( temp2[pos2], 3072-pos2, "<tr><td>Slot Worn:</td><td>%s</td></tr>", slots[g_slot[i]] )
         pos2 += format( temp2[pos2], 3072-pos2, "<tr><td valign=top>Professions Worn By:</td><td valign=top>" )
         if ( CB_DRUID & g_classes[i] )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Druid" )
         if ( CB_HUNTER & g_classes[i] )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Hunter" )
         if ( CB_MAGE & g_classes[i] )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Mage" )
         if ( CB_ROGUE & g_classes[i] )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Rogue" )
         if ( CB_WARLOCK & g_classes[i] )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Warlock" )
         if ( CB_PRIEST & g_classes[i] )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Priest" )
         pos2 += format( temp2[pos2], 3072-pos2, "</td></tr></table>" )
         pos2 += format( temp2[pos2], 3072-pos2, "</td><td valign=top>" )
         pos2 += format( temp2[pos2], 3072-pos2, "<center>Gear Modifiers</center><ol>" )
         if ( g_str[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Strength: +%d", g_str[i] )
         if ( g_agi[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Agility: +%d", g_agi[i] )
         if ( g_stam[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Stamina: +%d", g_stam[i] )
         if ( g_int[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Intelligence: +%d", g_int[i] )
         if ( g_spir[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Spirit: +%d", g_spir[i] )
         if ( g_armor[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Armor: +%d", g_armor[i] )
         if ( g_mana[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Mana Pool: +%d", g_mana[i] )
         if ( g_melee[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Melee Skill: +%d", g_melee[i] )
         if ( g_marksman[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Marksmanship Skill: +%d", g_marksman[i] )
         if ( g_arcane[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Arcane Magic: +%d", g_arcane[i] )
         if ( g_fire[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Fire Magic: +%d", g_fire[i] )
         if ( g_frost[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Frost Magic: +%d", g_frost[i] )
         if ( g_shadow[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Shadow Magic: +%d", g_shadow[i] )
         if ( g_nature[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Nature Magic: +%d", g_nature[i] )
         if ( g_holy[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Holy Magic: +%d", g_holy[i] )
         if ( g_crit[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Critical Strike: +%d", g_crit[i] )
         if ( g_spcrit[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Spell Crit: +%d", g_spcrit[i] )
         if ( g_spdmg[i] > 0 )
            pos2 += format( temp2[pos2], 3072-pos2, "<li>Spell Damage: +%d", g_spdmg[i] )
         pos2 += format( temp2[pos2], 3072-pos2, "</ul>" )
 
         if ( g_spec_idx[i] > 0 ){
            pos2 += format( temp2[pos2], 3072-pos2, "</td></tr><tr><td colspan=2>" )
            pos2 += format( temp2[pos2], 3072-pos2, "<center>Special Ability</center><ol>" )
            pos2 += format( temp2[pos2], 3072-pos2, "%s", g_specabil[g_spec_idx[i]] )
         }

         pos2 += format( temp2[pos2], 3072-pos2, "</td></tr></table>" )
      }
   }

   pos += format(temp[pos], 3072-pos, "%s", html_head )
   pos += format(temp[pos], 3072-pos, "<body>")
   pos += format(temp[pos], 3072-pos, "<center><table border=0 cellpadding=3 cellspacing=3><tr><td>")
   pos += format(temp[pos], 3072-pos, "<img src='http://www.cswowmod.com/forums/images/wowmodlogo.gif'><br />")

   if ( found )
      pos += format(temp[pos], 3072-pos, "%s", temp2 )
   else
      pos += format(temp[pos], 3072-pos, "<p><center>Item not found: %s</center></p><br />", arg )

   pos += format(temp[pos], 3072-pos, "</font></td></tr></table></center>")

   show_motd( id, temp, "WowMOD Examine")

   return PLUGIN_CONTINUE

}


// **************************************************************************
// END WowMOD Info and Help Module 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

