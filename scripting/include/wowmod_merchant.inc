/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Merchant System
*** Date:          Aug 13, 2007
*** Last Update:   Sep 14, 2007
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
// BEGIN WowMOD Merchant System 
// **************************************************************************


// **************************************************************************
// BEGIN wowmod_do_merchantmenu subroutine
// Handler for the merchants selection menu
// **************************************************************************
public wowmod_do_merchantmenu(id,key){
   switch (key){
      case 0: wowmod_display_spellmerchant( id )
      case 1: wowmod_display_itemmerch( id, 1 )
      case 2: wowmod_display_armourer( id, 1 )
      case 3: wowmod_display_sellspellmenu( id, 1 )
      case 4: wowmod_display_sellequipmenu( id, 1 )
      case 8: wowmod_display_mainmenu( id )
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_itemmerch subroutine
// Handler for the items merchant menu
// **************************************************************************
public wowmod_do_itemmerch( id, key ){
   new page = p_itempage[id]

   if ( ( key >= 0 ) && ( key < 8 ) ){

      new inv_idx = wowmod_gear_nextinvidx( id )

      if ( inv_idx == -1 ){
         client_print( id, print_chat, "Your inventory is full, you must drop an item first" )
         return PLUGIN_HANDLED
      }

      if ( p_itemlist[id][(page-1)*8+key] > 0 ){
         new idx = p_itemlist[id][(page-1)*8+key]

         if ( p_gold[id] < g_cost[idx] ){
            client_print( id, print_center, "You do not have enough gold to purchase %s", gear[idx] )
            return PLUGIN_HANDLED
         }

         client_print( id, print_chat, "The merchant sells you a '%s'", gear[idx] )

         p_gold[id] -= g_cost[idx]
         p_inventory[id][inv_idx] = idx
      }else{
         client_print( id, print_chat, "Invalid item selection!" )
      }

      wowmod_display_itemmerch( id, page )
      return PLUGIN_CONTINUE

   }

   if ( key == 8 ){
      if ( p_itemlist[id][page*8] > 0 )
         p_itempage[id] += 1
      else
         p_itempage[id] = 1
         
      wowmod_display_itemmerch( id, p_itempage[id] )
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_armourer subroutine
// Handler for the armourer menu
// **************************************************************************
public wowmod_do_armourer( id, key ){
   new page = p_gearpage[id]

   if ( ( key >= 0 ) && ( key < 8 ) ){

      new inv_idx = wowmod_gear_nextinvidx( id )

      if ( inv_idx == -1 ){
         client_print( id, print_chat, "Your inventory is full, you must drop an item first" )
         return PLUGIN_HANDLED
      }

      if ( p_gearlist[id][(page-1)*8+key] > 0 ){
         new idx = p_gearlist[id][(page-1)*8+key]

         if ( p_gold[id] < g_cost[idx] ){
            client_print( id, print_center, "You do not have enough gold to purchase %s", gear[idx] )
            return PLUGIN_HANDLED
         }

         client_print( id, print_chat, "The armorsmith crafts for you '%s'", gear[idx] )

         p_gold[id] -= g_cost[idx]
         p_inventory[id][inv_idx] = idx
      }else{
         client_print( id, print_chat, "Invalid armor selection!" )
      }

      wowmod_display_armourer( id, page )
      return PLUGIN_CONTINUE

   }

   if ( key == 8 ){
      if ( p_gearlist[id][page*8] > 0 )
         p_gearpage[id] += 1
      else
         p_gearpage[id] = 1
         
      wowmod_display_armourer( id, p_gearpage[id] )
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_buy_spell subroutine
// Handler for the purchasing of a new spell rank
// **************************************************************************
public wowmod_buy_spell( id, class, index ){

   new spell[32]
   new spell_idx = 0

   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   new bool:has_pts = false
   if ( spell_pts_avail > 0 )
      has_pts = true

   if ( !has_pts ){
      client_print( id , print_center, "You have no spell points available" )
      return PLUGIN_HANDLED
   }

   new name[32]
   get_user_name( id, name, 31 )

   new spell_cost = 0
   new spell_max = 0

   if ( class == CLASS_DRUID ){
      copy( spell, 31, druid_spells[index] )
      spell_cost = druid_spgold[index]
      spell_max = druid_spmax[index]
      spell_idx = SPELL_DRUID_BASE_IDX + index 
   }else if ( class == CLASS_HUNTER ){
      copy( spell, 31, hunter_spells[index] )
      spell_cost = hunter_spgold[index]
      spell_max = hunter_spmax[index]
      spell_idx = SPELL_HUNTER_BASE_IDX + index 
   }else if ( class == CLASS_MAGE ){
      copy( spell, 31, mage_spells[index] )
      spell_cost = mage_spgold[index]
      spell_max = mage_spmax[index]
      spell_idx = SPELL_MAGE_BASE_IDX + index 
   }else if ( class == CLASS_ROGUE ){
      copy( spell, 31, rogue_spells[index] )
      spell_cost = rogue_spgold[index]
      spell_max = rogue_spmax[index]
      spell_idx = SPELL_ROGUE_BASE_IDX + index 
   }else if ( class == CLASS_WARLOCK ){
      copy( spell, 31, warlock_spells[index] )
      spell_cost = warlock_spgold[index]
      spell_max = warlock_spmax[index]
      spell_idx = SPELL_WARLOCK_BASE_IDX + index 
   }else if ( class == CLASS_PRIEST ){
      copy( spell, 31, priest_spells[index] )
      spell_cost = priest_spgold[index]
      spell_max = priest_spmax[index]
      spell_idx = SPELL_PRIEST_BASE_IDX + index 
   }

   if ( p_gold[id] < spell_cost ){
      client_print( id, print_center, "[%s] You do not have enough gold to purchase %s", MOD, spell )
      return PLUGIN_HANDLED
   }

   if ( p_spells[id][spell_idx] >= spell_max ){
      client_print( id, print_center, "[%s] You already have a mastery of the spell %s", MOD, spell )
      return PLUGIN_HANDLED
   }

   log_amx( "[%s] %s is Buying %s spell (%s)", MOD, name, classes[class], spell )

   p_spells[id][spell_idx] += 1
   p_gold[id] -= spell_cost 
 
   client_print( id, print_center, 
      "[%s] You have learned a new rank of the spell '%s'", MOD, spell )

   if ( p_spells[id][spell_idx] == 1 ){
      if ( class == CLASS_DRUID ){
         if ( index == SP_HEALTOUCH )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_healtouch' to use this spell to heal a teamate", MOD )
         else if ( index == SP_REMCURSE )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_remcurse' to use this spell to cure a teamate", MOD )
         else if ( index == SP_GIFTWILD )
            client_print( id, print_chat, 
               "[%s] Say 'cast giftwild' to buff yourself or bind a key to 'wow_giftwild' to buff a targetted teamate", MOD )
         else if ( index == SP_TRANQ )
            client_print( id, print_chat, 
               "[%s] Say 'cast tranquility' to begin regenerating nearby teamates", MOD )
         else if ( index == SP_BARKSKIN )
            client_print( id, print_chat, 
               "[%s] Say 'cast barkskin' to protect yourself with a Bark", MOD )
         else if ( index == SP_ROOTS )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_roots' to attempt to root your targetted enemy", MOD )
         else
            client_print( id, print_chat, 
               "[%s] The Druid spell '%s' is passive and has no cmd to bind", MOD, spell )
      }else if ( class == CLASS_HUNTER ){
         if ( index == SP_APACK )
            client_print( id, print_chat, 
               "[%s] Say 'cast pack' to increase your team's runspeed", MOD )
         else if ( index == SP_AWILD )
            client_print( id, print_chat, 
               "[%s] Say 'cast wild' to enhance your team's nature resistance", MOD )
         else if ( index == SP_AIMED )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_aimed' to attempt to hit your target with an Aimed Shot", MOD )
         else
            client_print( id, print_chat, 
               "[%s] The Hunter spell '%s' is passive and has no cmd to bind", MOD, spell )
      }else if ( class == CLASS_MAGE ){
         if ( index == SP_ARCINT )
            client_print( id, print_chat, 
               "[%s] Say 'cast arcint' to cast this spell and buff your intelligence", MOD )
         else if ( index == SP_FROSTBOLT )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_frostbolt' to attempt to hit your target with a Frostbolt", MOD )
         else if ( index == SP_MAGEARMOR )
            client_print( id, print_chat, 
               "[%s] Say 'cast magearmor' to cast this spell and fortify your resistance to all forms of magic", MOD )
         else if ( index == SP_FLAMESTRIKE )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_flamestrike' to call down a flamestrike on your enemies", MOD )
         else if ( index == SP_ARCBRIL )
            client_print( id, print_chat, 
               "[%s] Say 'cast brilliance' to cast this spell and buff your team's intelligence",
               MOD )
         else if ( index == SP_BLIZZARD )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_blizzard' to call down a blizzard on a group of your enemies", MOD )
         else if ( index == SP_BLINK )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_blink' to teleport to the location you are targetting", MOD )
         else
            client_print( id, print_chat, 
               "[%s] The Mage spell '%s' is passive and has no cmd to bind", MOD, spell )
      }else if ( class == CLASS_ROGUE ){
         if ( index == SP_SPRINT )
            client_print( id, print_chat, 
               "[%s] Say 'cast sprint' to cast this spell and start to run VERY fast", MOD )
         else if ( index == SP_VANISH )
            client_print( id, print_chat, 
               "[%s] Say 'cast vanish' to cast this spell and vanish into thin air", MOD )
         else if ( index == SP_EVASION )
            client_print( id, print_chat, 
               "[%s] Say 'cast evasion' to cast this spell and improve your dodge rating", MOD )
         else if ( index == SP_EVISCERATE )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_eviscerate' to eviscerate your enemy for a kill shot (Knife Required)", MOD )
         else
            client_print( id, print_chat, 
               "[%s] The Rogue spell '%s' is passive and has no cmd to bind", MOD, spell )
      }else if ( class == CLASS_WARLOCK ){
         if ( index == SP_IMMOLATE )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_immolate' to seek out a target burn", MOD )
         else if ( index == SP_SHADOWBOLT )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_bolt' to seek out a target for your Shadow Bolt", MOD )
         else
            client_print( id, print_chat, 
               "[%s] The Warlock spell '%s' is passive and has no cmd to bind", MOD, spell )
      }else if ( class == CLASS_PRIEST ){
         if ( index == SP_HEAL )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_heal' to target and heal your teamates", MOD )
         else if ( index == SP_LESSERHEAL )
            client_print( id, print_chat, 
               "[%s] Bind a key to 'wow_lesserheal' to target and heal your teamates (manaless)", MOD )
         else if ( index == SP_PWSHIELD )
            client_print( id, print_chat, 
               "[%s] Say 'cast shield' to protect yourself", MOD )
         else if ( index == SP_PRAYERHEAL )
            client_print( id, print_chat, 
               "[%s] Say 'cast prayer' to heal your entire team and earn XP/Gold bonuses", MOD )
         else if ( index == SP_RESURRECT )
            client_print( id, print_chat, 
               "[%s] Say 'cast rez' to bring up the Resurrection menu (Rezzing earns you XP/Gold bonuses)", MOD )
         else
            client_print( id, print_chat, 
               "[%s] The Priest spell '%s' is passive and has no cmd to bind", MOD, spell )
      }
   }else{
      if ( class == CLASS_DRUID ){
         new spindex = SPELL_DRUID_BASE_IDX + index
         client_print( id, print_chat, 
            "[%s] You have purchased Spellrank %d of the Druid spell %s", 
            MOD, p_spells[id][spindex], spell, druid_spells[index] )
      }else if ( class == CLASS_HUNTER ){
         new spindex = SPELL_HUNTER_BASE_IDX + index
         client_print( id, print_chat, 
            "[%s] You have purchased Spellrank %d of the Hunter spell %s", 
            MOD, p_spells[id][spindex], spell, hunter_spells[index] )
      }else if ( class == CLASS_MAGE ){
         new spindex = SPELL_MAGE_BASE_IDX + index
         client_print( id, print_chat, 
            "[%s] You have purchased Spellrank %d of the Mage spell %s", 
            MOD, p_spells[id][spindex], spell, mage_spells[index] )
      }else if ( class == CLASS_ROGUE ){
         new spindex = SPELL_ROGUE_BASE_IDX + index
         client_print( id, print_chat, 
            "[%s] You have purchased Spellrank %d of the Rogue spell %s", 
            MOD, p_spells[id][spindex], spell, rogue_spells[index] )
      }else if ( class == CLASS_WARLOCK ){
         new spindex = SPELL_WARLOCK_BASE_IDX + index
         client_print( id, print_chat, 
            "[%s] You have purchased Spellrank %d of the Warlock spell %s", 
            MOD, p_spells[id][spindex], spell, warlock_spells[index] )
      }else if ( class == CLASS_PRIEST ){
         new spindex = SPELL_PRIEST_BASE_IDX + index
         client_print( id, print_chat, 
            "[%s] You have purchased Spellrank %d of the Priest spell %s", 
            MOD, p_spells[id][spindex], spell, priest_spells[index] )
      }
   }

   switch (class ){
      case CLASS_DRUID: wowmod_display_druidspells( id )
      case CLASS_HUNTER: wowmod_display_hunterspells( id )
      case CLASS_MAGE: wowmod_display_magespells( id )
      case CLASS_ROGUE: wowmod_display_roguespells( id )
      case CLASS_WARLOCK: wowmod_display_warlockspells( id )
      case CLASS_PRIEST: wowmod_display_priestspells( id )
   }
 
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_spellmerchant subroutine
// Handler for spell merchant class menu
// **************************************************************************
public wowmod_do_spellmerchant( id, key ){

   switch( key ){
      case CLASS_DRUID: wowmod_display_druidspells( id ) 
      case CLASS_HUNTER: wowmod_display_hunterspells( id )
      case CLASS_MAGE: wowmod_display_magespells( id )
      case CLASS_ROGUE: wowmod_display_roguespells( id ) 
      case CLASS_WARLOCK: wowmod_display_warlockspells( id ) 
      case CLASS_PRIEST: wowmod_display_priestspells( id )
      case 8: wowmod_display_mainmenu( id )
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_druidspells subroutine
// Handler for druid spells merchant 
// **************************************************************************
public wowmod_do_druidspells( id, key ){
   if ( ( p_class1[id] != CLASS_DRUID ) && ( p_class2[id] != CLASS_DRUID ) ){
      client_print( id, print_center, "You cannot learn Druid spells" )
      return PLUGIN_HANDLED
   }

   // Calculate primary class spell pts avail
   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   // Calculate secondary class spell pts avail
   new spell_pts2 = wowmod_count_spellpoints2( id )
   new spell_pts_avail2 = p_level[id] - spell_pts2
   new bool:has_pts = false
   new bool:has_pts2 = false 

   if ( ( key < NUM_SPELLS_CLASS ) && ( p_level[id] < druid_splvls[key] ) ){
      client_print( id , print_center, "[%s] You are not a high enough level to learn that spell!", MOD )
      return PLUGIN_HANDLED
   }

   if ( spell_pts_avail2 && ( spell_pts2 < DUAL_CLASS_NUMSPELLS ) )
      has_pts2 = true 

   if ( spell_pts_avail > 0 )
      has_pts = true

   if ( !has_pts ){
      client_print( id , print_center, "[%s] You have no spell points available", MOD )
      return PLUGIN_HANDLED
   }

   if ( ( p_class1[id] != p_class2[id] ) && ( p_class2[id] == CLASS_DRUID ) && !has_pts2 ){
      client_print( id , print_center, "[%s] You have no spell points available for secondary class spells", 
         MOD )
      return PLUGIN_HANDLED
   }

   switch( key ){
      case 0: wowmod_buy_spell( id, CLASS_DRUID, 0 )
      case 1: wowmod_buy_spell( id, CLASS_DRUID, 1 )
      case 2: wowmod_buy_spell( id, CLASS_DRUID, 2 )
      case 3: wowmod_buy_spell( id, CLASS_DRUID, 3 )
      case 4: wowmod_buy_spell( id, CLASS_DRUID, 4 )
      case 5: wowmod_buy_spell( id, CLASS_DRUID, 5 )
      case 6: wowmod_buy_spell( id, CLASS_DRUID, 6 )
      case 7: wowmod_buy_spell( id, CLASS_DRUID, 7 )
      case 8: wowmod_display_spellmerchant( id )
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_hunterspells subroutine
// Handler for hunter spells merchant 
// **************************************************************************
public wowmod_do_hunterspells( id, key ){
   if ( ( p_class1[id] != CLASS_HUNTER ) && ( p_class2[id] != CLASS_HUNTER ) ){
      client_print( id, print_center, "You cannot learn Hunter spells" )
      return PLUGIN_HANDLED
   }

   // Calculate primary class spell pts avail
   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   // Calculate secondary class spell pts avail
   new spell_pts2 = wowmod_count_spellpoints2( id )
   new spell_pts_avail2 = p_level[id] - spell_pts2
   new bool:has_pts = false
   new bool:has_pts2 = false

   if ( ( key < NUM_SPELLS_CLASS ) && ( p_level[id] < hunter_splvls[key] ) ){
      client_print( id , print_center, "[%s] You are not a high enough level to learn that spell!", MOD )
      return PLUGIN_HANDLED
   }

   if ( spell_pts_avail2 && ( spell_pts2 < DUAL_CLASS_NUMSPELLS ) )
      has_pts2 = true

   if ( spell_pts_avail > 0 )
      has_pts = true

   if ( !has_pts ){
      client_print( id , print_center, "[%s] You have no spell points available", MOD )
      return PLUGIN_HANDLED
   }

   if ( ( p_class1[id] != p_class2[id] ) && ( p_class2[id] == CLASS_HUNTER ) && !has_pts2 ){
      client_print( id , print_center, "[%s] You have no spell points available for secondary class spells",
         MOD )
      return PLUGIN_HANDLED
   }


   switch( key ){
      case 0: wowmod_buy_spell( id, CLASS_HUNTER, 0 )
      case 1: wowmod_buy_spell( id, CLASS_HUNTER, 1 )
      case 2: wowmod_buy_spell( id, CLASS_HUNTER, 2 )
      case 3: wowmod_buy_spell( id, CLASS_HUNTER, 3 )
      case 4: wowmod_buy_spell( id, CLASS_HUNTER, 4 )
      case 5: wowmod_buy_spell( id, CLASS_HUNTER, 5 )
      case 6: wowmod_buy_spell( id, CLASS_HUNTER, 6 )
      case 7: wowmod_buy_spell( id, CLASS_HUNTER, 7 )
      case 8: wowmod_display_spellmerchant( id )
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_magespells subroutine
// Handler for mage spells merchant 
// **************************************************************************
public wowmod_do_magespells( id, key ){
   if ( ( p_class1[id] != CLASS_MAGE ) && ( p_class2[id] != CLASS_MAGE ) ){
      client_print( id, print_center, "You cannot learn Mage spells" )
      return PLUGIN_HANDLED
   }

   // Calculate primary class spell pts avail
   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   // Calculate secondary class spell pts avail
   new spell_pts2 = wowmod_count_spellpoints2( id )
   new spell_pts_avail2 = p_level[id] - spell_pts2
   new bool:has_pts = false
   new bool:has_pts2 = false

   if ( ( key < NUM_SPELLS_CLASS ) && ( p_level[id] < mage_splvls[key] ) ){
      client_print( id , print_center, "[%s] You are not a high enough level to learn that spell!", MOD )
      return PLUGIN_HANDLED
   }

   if ( spell_pts_avail2 && ( spell_pts2 < DUAL_CLASS_NUMSPELLS ) )
      has_pts2 = true

   if ( spell_pts_avail > 0 )
      has_pts = true

   if ( !has_pts ){
      client_print( id , print_center, "[%s] You have no spell points available", MOD )
      return PLUGIN_HANDLED
   }

   if ( ( p_class1[id] != p_class2[id] ) && ( p_class2[id] == CLASS_MAGE ) && !has_pts2 ){
      client_print( id , print_center, "[%s] You have no spell points available for secondary class spells",
         MOD )
      return PLUGIN_HANDLED
   }

   switch( key ){
      case 0: wowmod_buy_spell( id, CLASS_MAGE, 0 )
      case 1: wowmod_buy_spell( id, CLASS_MAGE, 1 )
      case 2: wowmod_buy_spell( id, CLASS_MAGE, 2 )
      case 3: wowmod_buy_spell( id, CLASS_MAGE, 3 )
      case 4: wowmod_buy_spell( id, CLASS_MAGE, 4 )
      case 5: wowmod_buy_spell( id, CLASS_MAGE, 5 )
      case 6: wowmod_buy_spell( id, CLASS_MAGE, 6 )
      case 7: wowmod_buy_spell( id, CLASS_MAGE, 7 )
      case 8: wowmod_display_spellmerchant( id )
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_roguespells subroutine
// Handler for rogue spells merchant 
// **************************************************************************
public wowmod_do_roguespells( id, key ){
   if ( ( p_class1[id] != CLASS_ROGUE ) && ( p_class2[id] != CLASS_ROGUE ) ){
      client_print( id, print_center, "You cannot learn Rogue spells" )
      return PLUGIN_HANDLED
   }

   // Calculate primary class spell pts avail
   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   // Calculate secondary class spell pts avail
   new spell_pts2 = wowmod_count_spellpoints2( id )
   new spell_pts_avail2 = p_level[id] - spell_pts2
   new bool:has_pts = false
   new bool:has_pts2 = false

   if ( ( key < NUM_SPELLS_CLASS ) && ( p_level[id] < rogue_splvls[key] ) ){
      client_print( id , print_center, "[%s] You are not a high enough level to learn that spell!", MOD )
      return PLUGIN_HANDLED
   }

   if ( spell_pts_avail2 && ( spell_pts2 < DUAL_CLASS_NUMSPELLS ) )
      has_pts2 = true

   if ( spell_pts_avail > 0 )
      has_pts = true

   if ( !has_pts ){
      client_print( id , print_center, "[%s] You have no spell points available", MOD )
      return PLUGIN_HANDLED
   }

   if ( ( p_class1[id] != p_class2[id] ) && ( p_class2[id] == CLASS_ROGUE ) && !has_pts2 ){
      client_print( id , print_center, "[%s] You have no spell points available for secondary class spells",
         MOD )
      return PLUGIN_HANDLED
   }

   switch( key ){
      case 0: wowmod_buy_spell( id, CLASS_ROGUE, 0 )
      case 1: wowmod_buy_spell( id, CLASS_ROGUE, 1 )
      case 2: wowmod_buy_spell( id, CLASS_ROGUE, 2 )
      case 3: wowmod_buy_spell( id, CLASS_ROGUE, 3 )
      case 4: wowmod_buy_spell( id, CLASS_ROGUE, 4 )
      case 5: wowmod_buy_spell( id, CLASS_ROGUE, 5 )
      case 6: wowmod_buy_spell( id, CLASS_ROGUE, 6 )
      case 7: wowmod_buy_spell( id, CLASS_ROGUE, 7 )
      case 8: wowmod_display_spellmerchant( id )
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_warlockspells subroutine
// Handler for warlock spells merchant 
// **************************************************************************
public wowmod_do_warlockspells( id, key ){
   if ( ( p_class1[id] != CLASS_WARLOCK ) && ( p_class2[id] != CLASS_WARLOCK ) ){
      client_print( id, print_center, "You cannot learn Warlock spells" )
      return PLUGIN_HANDLED
   }

   // Calculate primary class spell pts avail
   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   // Calculate secondary class spell pts avail
   new spell_pts2 = wowmod_count_spellpoints2( id )
   new spell_pts_avail2 = p_level[id] - spell_pts2
   new bool:has_pts = false
   new bool:has_pts2 = false

   if ( ( key < NUM_SPELLS_CLASS ) && ( p_level[id] < warlock_splvls[key] ) ){
      client_print( id , print_center, "[%s] You are not a high enough level to learn that spell!", MOD )
      return PLUGIN_HANDLED
   }

   if ( spell_pts_avail2 && ( spell_pts2 < DUAL_CLASS_NUMSPELLS ) )
      has_pts2 = true

   if ( spell_pts_avail > 0 )
      has_pts = true

   if ( !has_pts ){
      client_print( id , print_center, "[%s] You have no spell points available", MOD )
      return PLUGIN_HANDLED
   }

   if ( ( p_class1[id] != p_class2[id] ) && ( p_class2[id] == CLASS_WARLOCK ) && !has_pts2 ){
      client_print( id , print_center, "[%s] You have no spell points available for secondary class spells",
         MOD )
      return PLUGIN_HANDLED
   }

   switch( key ){
      case 0: wowmod_buy_spell( id, CLASS_WARLOCK, 0 )
      case 1: wowmod_buy_spell( id, CLASS_WARLOCK, 1 )
      case 2: wowmod_buy_spell( id, CLASS_WARLOCK, 2 )
      case 3: wowmod_buy_spell( id, CLASS_WARLOCK, 3 )
      case 4: wowmod_buy_spell( id, CLASS_WARLOCK, 4 )
      case 5: wowmod_buy_spell( id, CLASS_WARLOCK, 5 )
      case 6: wowmod_buy_spell( id, CLASS_WARLOCK, 6 )
      case 7: wowmod_buy_spell( id, CLASS_WARLOCK, 7 )
      case 8: wowmod_display_spellmerchant( id )
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_do_priestspells subroutine
// Handler for priest spells merchant 
// **************************************************************************
public wowmod_do_priestspells( id, key ){
   if ( ( p_class1[id] != CLASS_PRIEST ) && ( p_class2[id] != CLASS_PRIEST ) ){
      client_print( id, print_center, "You cannot learn Priest spells" )
      return PLUGIN_HANDLED
   }

   // Calculate primary class spell pts avail
   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   // Calculate secondary class spell pts avail
   new spell_pts2 = wowmod_count_spellpoints2( id )
   new spell_pts_avail2 = p_level[id] - spell_pts2
   new bool:has_pts = false
   new bool:has_pts2 = false

   if ( ( key < NUM_SPELLS_CLASS ) && ( p_level[id] < priest_splvls[key] ) ){
      client_print( id , print_center, "[%s] You are not a high enough level to learn that spell!", MOD )
      return PLUGIN_HANDLED
   }

   if ( spell_pts_avail2 && ( spell_pts2 < DUAL_CLASS_NUMSPELLS ) )
      has_pts2 = true

   if ( spell_pts_avail > 0 )
      has_pts = true

   if ( !has_pts ){
      client_print( id , print_center, "[%s] You have no spell points available", MOD )
      return PLUGIN_HANDLED
   }

   if ( ( p_class1[id] != p_class2[id] ) && ( p_class2[id] == CLASS_PRIEST ) && !has_pts2 ){
      client_print( id , print_center, "[%s] You have no spell points available for secondary class spells",
         MOD )
      return PLUGIN_HANDLED
   }

   switch( key ){
      case 0: wowmod_buy_spell( id, CLASS_PRIEST, 0 )
      case 1: wowmod_buy_spell( id, CLASS_PRIEST, 1 )
      case 2: wowmod_buy_spell( id, CLASS_PRIEST, 2 )
      case 3: wowmod_buy_spell( id, CLASS_PRIEST, 3 )
      case 4: wowmod_buy_spell( id, CLASS_PRIEST, 4 )
      case 5: wowmod_buy_spell( id, CLASS_PRIEST, 5 )
      case 6: wowmod_buy_spell( id, CLASS_PRIEST, 6 )
      case 7: wowmod_buy_spell( id, CLASS_PRIEST, 7 )
      case 8: wowmod_display_spellmerchant( id )
   }
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_druidspells subroutine
// Menu for druid spell merchant 
// **************************************************************************
public wowmod_display_druidspells( id ){

   if ( ( p_class1[id] != CLASS_DRUID ) && ( p_class2[id] != CLASS_DRUID ) ){
      client_print( id, print_center, "You cannot learn Druid spells" )
      return PLUGIN_HANDLED
   }

   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   new bool:has_pts = false
   new pos = 0, i, menu_body[512]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   if ( spell_pts_avail > 0 )
      has_pts = true

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Druid Spells Merchant^n")
   pos += format(menu_body[pos], 511-pos, "\y(%d spell points available, %d Gold)^n^n", spell_pts_avail, p_gold[id] )
   for ( i = 0; i < 8; i++ ){
      if ( p_level[id] >= druid_splvls[i] ){
         new next_rank = ( p_spells[id][SPELL_DRUID_BASE_IDX + i] + 1 )
         if ( next_rank > druid_spmax[i] )
            pos += format( menu_body[pos], 511-pos, "\d%d. %s (max rank)^n",i+1, druid_spells[i] )
         else
            if ( has_pts )
               pos += format( menu_body[pos], 511-pos, "\w%d. %s (next rank: %d) [%d gold]^n",
                     i+1, druid_spells[i], next_rank, druid_spgold[i])
            else
               pos += format( menu_body[pos], 511-pos, "\d%d. %s (next rank: %d) [%d gold]^n",
                     i+1, druid_spells[i], next_rank, druid_spgold[i])
      }else{
         pos += format( menu_body[pos], 511-pos, "\d%d. %s^n",i+1, druid_spells[i])
      }
   }
   pos += format(menu_body[pos], 511-pos, "\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")

   show_menu( id, keys, menu_body, -1 )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_hunterspells subroutine
// Menu for hunter spell merchant 
// **************************************************************************
public wowmod_display_hunterspells( id ){

   if ( ( p_class1[id] != CLASS_HUNTER ) && ( p_class2[id] != CLASS_HUNTER ) ){
      client_print( id, print_center, "You cannot learn Hunter spells" )
      return PLUGIN_HANDLED
   }

   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   new bool:has_pts = false
   new pos = 0, i, menu_body[512]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   if ( spell_pts_avail > 0 )
      has_pts = true

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Hunter Spells Merchant^n")
   pos += format(menu_body[pos], 511-pos, "\y(%d spell points available, %d Gold)^n^n", spell_pts_avail, p_gold[id] )
   for ( i = 0; i < 8; i++ ){
      if ( p_level[id] >= hunter_splvls[i] ){
         new next_rank = ( p_spells[id][SPELL_HUNTER_BASE_IDX + i] + 1 )
         if ( next_rank > hunter_spmax[i] )
            pos += format( menu_body[pos], 511-pos, "\d%d. %s (max rank)^n",i+1, hunter_spells[i] )
         else
            if ( has_pts )
               pos += format( menu_body[pos], 511-pos, "\w%d. %s (next rank: %d) [%d gold]^n", 
                     i+1, hunter_spells[i], next_rank, hunter_spgold[i])
            else
               pos += format( menu_body[pos], 511-pos, "\d%d. %s (next rank: %d) [%d gold]^n", 
                     i+1, hunter_spells[i], next_rank, hunter_spgold[i])
      }else{
         pos += format( menu_body[pos], 511-pos, "\d%d. %s^n",i+1, hunter_spells[i])
      }
   }
   pos += format(menu_body[pos], 511-pos, "\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")

   show_menu( id, keys, menu_body, -1 )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_magespells subroutine
// Menu for mage spell merchant 
// **************************************************************************
public wowmod_display_magespells( id ){

   if ( ( p_class1[id] != CLASS_MAGE ) && ( p_class2[id] != CLASS_MAGE ) ){
      client_print( id, print_center, "You cannot learn Mage spells" )
      return PLUGIN_HANDLED
   }

   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   new bool:has_pts = false
   new pos = 0, i, menu_body[512]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   if ( spell_pts_avail > 0 )
      has_pts = true

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Mage Spells Merchant^n")
   pos += format(menu_body[pos], 511-pos, "\y(%d spell points available, %d Gold)^n^n", spell_pts_avail, p_gold[id] )
   for ( i = 0; i < 8; i++ ){
      if ( p_level[id] >= mage_splvls[i] ){
         new next_rank = ( p_spells[id][SPELL_MAGE_BASE_IDX + i] + 1 )
         if ( next_rank > mage_spmax[i] )
            pos += format( menu_body[pos], 511-pos, "\d%d. %s (max rank)^n",i+1, mage_spells[i] )
         else
            if ( has_pts )
               pos += format( menu_body[pos], 511-pos, "\w%d. %s (next rank: %d) [%d gold]^n", 
                        i+1, mage_spells[i], next_rank, mage_spgold[i])
            else
               pos += format( menu_body[pos], 511-pos, "\d%d. %s (next rank: %d) [%d gold]^n", 
                        i+1, mage_spells[i], next_rank, mage_spgold[i])
      }else{
         pos += format( menu_body[pos], 511-pos, "\d%d. %s^n",i+1, mage_spells[i])
      }
   }
   pos += format(menu_body[pos], 511-pos, "\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")

   show_menu( id, keys, menu_body, -1 )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_roguespells subroutine
// Menu for rogue spell merchant 
// **************************************************************************
public wowmod_display_roguespells( id ){

   if ( ( p_class1[id] != CLASS_ROGUE ) && ( p_class2[id] != CLASS_ROGUE ) ){
      client_print( id, print_center, "You cannot learn Rogue spells" )
      return PLUGIN_HANDLED
   }

   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   new bool:has_pts = false
   new pos = 0, i, menu_body[512]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   if ( spell_pts_avail > 0 )
      has_pts = true

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Rogue Spells Merchant^n")
   pos += format(menu_body[pos], 511-pos, "\y(%d spell points available, %d Gold)^n^n", spell_pts_avail, p_gold[id] )
   for ( i = 0; i < 8; i++ ){
      if ( p_level[id] >= rogue_splvls[i] ){
         new next_rank = ( p_spells[id][SPELL_ROGUE_BASE_IDX + i] + 1 )
         if ( next_rank > rogue_spmax[i] )
            pos += format( menu_body[pos], 511-pos, "\d%d. %s (max rank)^n",i+1, rogue_spells[i] )
         else
            if ( has_pts )
               pos += format( menu_body[pos], 511-pos, "\w%d. %s (next rank: %d) [%d gold]^n", 
                        i+1, rogue_spells[i], next_rank, rogue_spgold[i])
            else
               pos += format( menu_body[pos], 511-pos, "\d%d. %s (next rank: %d) [%d gold]^n", 
                        i+1, rogue_spells[i], next_rank, rogue_spgold[i])
      }else{
         pos += format( menu_body[pos], 511-pos, "\d%d. %s^n",i+1, rogue_spells[i])
      }
   }
   pos += format(menu_body[pos], 511-pos, "\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")

   show_menu( id, keys, menu_body, -1 )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_warlockspells subroutine
// Menu for warlock spell merchant 
// **************************************************************************
public wowmod_display_warlockspells( id ){

   if ( ( p_class1[id] != CLASS_WARLOCK ) && ( p_class2[id] != CLASS_WARLOCK ) ){
      client_print( id, print_center, "You cannot learn Warlock spells" )
      return PLUGIN_HANDLED
   }

   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   new pos = 0, i, menu_body[512]
   new bool:has_pts = false
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   if ( spell_pts_avail > 0 )
      has_pts = true

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Warlock Spells Merchant^n")
   pos += format(menu_body[pos], 511-pos, "\y(%d spell points available, %d Gold)^n^n", spell_pts_avail, p_gold[id] )
   for ( i = 0; i < 8; i++ ){
      if ( p_level[id] >= warlock_splvls[i] ){
         new next_rank = ( p_spells[id][SPELL_WARLOCK_BASE_IDX + i] + 1 )
         if ( next_rank > warlock_spmax[i] )
            pos += format( menu_body[pos], 511-pos, "\d%d. %s (max rank)^n",
                     i+1, warlock_spells[i] )
         else
            if ( has_pts )
               pos += format( menu_body[pos], 511-pos, "\w%d. %s (next rank: %d) [%d gold]^n", 
                     i+1, warlock_spells[i], next_rank, warlock_spgold[i])
            else
               pos += format( menu_body[pos], 511-pos, "\d%d. %s (next rank: %d) [%d gold]^n", 
                     i+1, warlock_spells[i], next_rank, warlock_spgold[i])
      }else{
         pos += format( menu_body[pos], 511-pos, "\d %d. %s^n",i+1, warlock_spells[i])
      }
   }
   pos += format(menu_body[pos], 511-pos, "\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")

   show_menu( id, keys, menu_body, -1 )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_priestspells subroutine
// Menu for priest spell merchant 
// **************************************************************************
public wowmod_display_priestspells( id ){

   if ( ( p_class1[id] != CLASS_PRIEST ) && ( p_class2[id] != CLASS_PRIEST ) ){
      client_print( id, print_center, "You cannot learn Priest spells" )
      return PLUGIN_HANDLED
   }

   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   new pos = 0, i, menu_body[512]
   new bool:has_pts = false
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   if ( spell_pts_avail > 0 )
      has_pts = true
   

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Priest Spells Merchant^n")
   pos += format(menu_body[pos], 511-pos, "\y(%d spell points available, %d Gold)^n^n", spell_pts_avail, p_gold[id] )
   for ( i = 0; i < 8; i++ ){
      if ( p_level[id] >= priest_splvls[i] ){
         new next_rank = ( p_spells[id][SPELL_PRIEST_BASE_IDX + i] + 1 )
         if ( next_rank > priest_spmax[i] )
            pos += format( menu_body[pos], 511-pos, "\d%d. %s (max rank)^n", 
                     i+1, priest_spells[i] )
         else
            if ( has_pts )
               pos += format( menu_body[pos], 511-pos, "\w%d. %s (next rank: %d) [%d gold]^n", 
                     i+1, priest_spells[i], next_rank, priest_spgold[i])
            else
               pos += format( menu_body[pos], 511-pos, "\d%d. %s (next rank: %d) [%d gold]^n", 
                     i+1, priest_spells[i], next_rank, priest_spgold[i])
      }else{
         pos += format( menu_body[pos], 511-pos, "\d%d. %s^n",i+1, priest_spells[i])
      }
   }
   pos += format(menu_body[pos], 511-pos, "\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")

   show_menu( id, keys, menu_body, -1 )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_spellmerchant subroutine
// Menu for spell merchant class menu
// **************************************************************************
public wowmod_display_spellmerchant( id ){

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   // Calculate primary class spell pts avail
   new spell_pts = wowmod_count_spellpoints( id )
   new spell_pts_avail = p_level[id] - spell_pts
   // Calculate secondary class spell pts avail
   //new spell_pts2 = wowmod_count_spellpoints2( id )
   //new spell_pts_avail2 = p_level[id] - spell_pts2
   //if ( spell_pts2 >= DUAL_CLASS_NUMSPELLS )
   //   spell_pts_avail2 = 0

   new nLoop = 5
   new pos = 0, i, menu_body[512], menu_items[7][32]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<8)|(1<<9)

   menu_items[0]= "Druid Spells"
   menu_items[1]= "Hunter Spells"
   menu_items[2]= "Mage Spells"
   menu_items[3]= "Rogue Spells"
   menu_items[4]= "Warlock Spells"
   menu_items[5]= "Priest Spells"

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Spells and Skills Merchant^n")
   pos += format(menu_body[pos], 511-pos, "\y(%d spell points available, %d Gold)^n^n", spell_pts_avail, p_gold[id] )

   for ( i = 0; i<= nLoop; i++ ){
      if ( ( p_class1[id] == i ) || ( p_class2[id] == i ) )
         pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
      else
         pos += format(menu_body[pos], 511-pos, "\d%d. %s^n",i+1,menu_items[i])
   }

   pos += format(menu_body[pos], 511-pos, "^n\w9. Main Menu")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")

   show_menu( id, keys, menu_body, -1)

   return PLUGIN_HANDLED

}


// **************************************************************************
// BEGIN wowmod_display_itemmerch subroutine
// The WowMOD items merchant menu
// **************************************************************************
public wowmod_display_itemmerch( id, page ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   page = ( page > 0 ) ? page : 1

   p_itempage[id] = page

   new pos = 0, i, menu_body[512]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   new idx
   new name[32]

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Items Merchant^n")
   if ( p_tier[id] > 0 )
      pos += format(menu_body[pos], 511-pos, "\yYour Equipment Tier: %d (%s)^n^n", 
         p_tier[id], tiers[p_tier[id]] )

   new startidx = ( (page-1) * 8 )
   new endidx = ( startidx + 7 ) 
   new bool:item_found = false

   for ( i = startidx; ((i <= endidx)&&(p_itemlist[id][i]>0)); i++ ){
      if ( !item_found )
         item_found = true
      new vis_i = ( i - startidx ) + 1
      idx = p_itemlist[id][i]
      copy( name, 31, gear[idx] ) 
      //pos += format( menu_body[pos], 511-pos, 
      //         "\w%d. %s^n", vis_i, name )
      pos += format( menu_body[pos], 511-pos, "\w%d. %s (Tier: %d)  [%d gold]^n", 
                vis_i, name, g_tier[idx], g_cost[idx])
   }

   if ( !item_found )
      pos += format(menu_body[pos], 511-pos, "\yNo items available, check back after you have gained an equipment tier^n^n")

   if ( ( page > 1 ) || ( ( endidx < 31 ) && ( p_itemlist[id][endidx+1] > 0 ) ) )
      pos += format(menu_body[pos], 511-pos, "^n\w9. Next Page")

   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_display_armourer subroutine
// The WowMOD armourer menu
// **************************************************************************
public wowmod_display_armourer( id, page ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   page = ( page > 0 ) ? page : 1

   p_gearpage[id] = page

   new pos = 0, i, menu_body[512]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)

   new idx
   new name[32]
   //new gearlist[32]
   //wowmod_gear_getcraftable( id, gearlist )

   //for ( new j = 0; ((j < 32)&&(gearlist[j]>0)); j++ )
   //   log_amx( "j=(%d) gearlist=(%d)", j, gearlist[j] )

   pos += format(menu_body[pos], 511-pos, "\yWowMOD: Armourer^n")
   if ( p_tier[id] > 0 )
      pos += format(menu_body[pos], 511-pos, "\yYour Equipment Tier: %d (%s)^n^n", 
         p_tier[id], tiers[p_tier[id]] )

   new startidx = ( (page-1) * 8 )
   new endidx = ( startidx + 7 ) 
   new bool:item_found = false
   
   for ( i = startidx; ((i <= endidx)&&(p_gearlist[id][i]>0)); i++ ){
      if ( !item_found )
         item_found = true
      new vis_i = ( i - startidx ) + 1
      idx = p_gearlist[id][i]
      copy( name, 31, gear[idx] ) 
      //pos += format( menu_body[pos], 511-pos, 
      //         "\w%d. %s^n", vis_i, name )
      pos += format( menu_body[pos], 511-pos, "\w%d. %s (Tier: %d)  [%d gold]^n", 
                vis_i, name, g_tier[idx], g_cost[idx])
   }

   if ( !item_found )
      pos += format(menu_body[pos], 511-pos, "\yNo armor available, check back after you have gained an equipment tier^n^n")

   if ( ( page > 1 ) || ( ( endidx < 31 ) && ( p_gearlist[id][endidx+1] > 0 ) ) )
      pos += format(menu_body[pos], 511-pos, "^n\w9. Next Page")

   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_HANDLED

}


// **************************************************************************
// END WowMOD Merchant System 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

