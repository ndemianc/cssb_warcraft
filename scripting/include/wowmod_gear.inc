/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Gear Module
*** Date:          Aug 28, 2007
*** Last Update:   Jan 31, 2007
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
// BEGIN WowMOD Gear Module 
// **************************************************************************


// **************************************************************************
// BEGIN wowmod_specabil_init subroutine
// Initialize the special abilities system for worn equipment
// **************************************************************************
public wowmod_specabil_init(){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   log_amx( "[%s] CREATING Equipment Special Abilities", MOD )

   copy( g_specabil[SPA_HSTONE], 127, 
      "Say /cast hearthstone to teleport yourself back to spawn ( reusable )" ) 
   copy( g_specabil[SPA_SPEED1], 127, 
      "Say /cast speed to increase your runspeed ( reuseable )" ) 
   copy( g_specabil[SPA_SPEED2], 127, 
      "Say /cast speed to increase your runspeed ( reuseable )" ) 
   copy( g_specabil[SPA_SPEED3], 127, 
      "Say /cast speed to increase your runspeed ( reuseable )" ) 
   copy( g_specabil[SPA_LIGHTNING1], 127, 
      "Bind key to /wow_lightning to call forth lightning upon your enemy ( once per turn )" ) 
   copy( g_specabil[SPA_LIGHTNING2], 127, 
      "Bind key to /wow_lightning to call forth lightning upon your enemies ( once per turn )" ) 

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_gear_worninit
// Add all mods from worn equipment, called on player data load 
// **************************************************************************
public wowmod_gear_worninit( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new sidx = -1
   new gidx = -1
   new class = -1
   new name[32]

   switch ( p_class1[id] ){
      case CLASS_DRUID: class = CB_DRUID
      case CLASS_HUNTER: class = CB_HUNTER
      case CLASS_MAGE: class = CB_MAGE
      case CLASS_ROGUE: class = CB_ROGUE
      case CLASS_WARLOCK: class = CB_WARLOCK
      case CLASS_PRIEST: class = CB_PRIEST
   }

   p_crit[id] = 0    // This one is treated differently since not stored
   p_spcrit[id] = 0  // This one is treated differently since not stored
   p_spdmg[id] = 0   // This one is treated differently since not stored

   get_user_name( id, name, 31 )

   for ( new i = IDX_GEAR_HEAD; i <= IDX_GEAR_NECK; i++ ){
      if ( p_inventory[id][i] <= 0 ){
         //log_amx( "Nothing worn slot [%d]", i )
         continue
      }

      gidx = p_inventory[id][i]

      if ( !( get_user_flags(id) & get_cvar_num("WOW_admin_flag") ) ){
         // Check that non-admin/dev players can wear this item
         if ( !( class & g_classes[gidx] ) ){
            // Class prevents this player from wearing item
            sidx =  wowmod_gear_nextinvidx( id )
            switch( g_slot[gidx] ){
               case SLOT_HEAD: p_head[id] = 0 
               case SLOT_CHEST: p_chest[id] = 0 
               case SLOT_BACK: p_back[id] = 0 
               case SLOT_LEGS: p_legs[id] = 0
               case SLOT_HANDS: p_hands[id] = 0 
               case SLOT_FEET: p_feet[id] = 0 
               case SLOT_WRISTS: p_wrists[id] = 0 
               case SLOT_NECK: p_neck[id] = 0 
               case SLOT_RING1: {
                  if ( p_ring1[id] == gidx )
                     p_ring1[id] = 0
                  else
                     p_ring2[id] = 0
               }
            }
            
            p_inventory[id][i] = 0
            p_inventory[id][sidx] = gidx
            
            client_print( id, print_chat, 
               "[%s] You are unable to wear [%d] '%s', item returned to your inventory", MOD, gidx, gear[gidx] )
            log_amx( "[%s] gear_worninit() %s is unable to wear item [%d] (%s)", 
               MOD, name, gidx, gear[gidx] )
           
            continue 
         }
      }
 
      log_amx( "[%s] gear_worninit() %s wearing item [%d] (%s) at slot [%d] found", 
         MOD, name, gidx, gear[gidx],  i )
 
      // Modify attributes and proficiencies
      p_str[id] += g_str[gidx]
      p_agi[id] += g_agi[gidx]
      p_stam[id] += g_stam[gidx]
      p_int[id] += g_int[gidx]
      p_int_unb[id] += g_int[gidx]
      p_spir[id] += g_spir[gidx]
      p_armor[id] += g_armor[gidx]
      p_mana[id] += g_mana[gidx]
      p_mana_base[id] += g_mana[gidx]

      p_melee[id] += g_melee[gidx]
      p_marksman[id] += g_marksman[gidx]
      p_arcane[id] += g_arcane[gidx]
      p_fire[id] += g_fire[gidx]
      p_frost[id] += g_frost[gidx]
      p_shadow[id] += g_shadow[gidx]
      p_nature[id] += g_nature[gidx]
      p_holy[id] += g_holy[gidx]

      p_crit[id] += g_crit[gidx]
      p_spcrit[id] += g_spcrit[gidx]
      p_spdmg[id] += g_spdmg[gidx]
      
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_gear_wear
// Player command to wear a piece of gear
// **************************************************************************
public wowmod_gear_wear( id, arg[] ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   //log_amx( "Wear: %s", arg )

   new gidx = -1  // inventory index for item found

   if ( p_class1[id] == -1 ){
      client_print( id, print_chat, "[%s] You must choose a primary class before being able to wear equipment",
         MOD )
      return PLUGIN_HANDLED
   }

   new class = -1
   switch ( p_class1[id] ){
      case CLASS_DRUID: class = CB_DRUID
      case CLASS_HUNTER: class = CB_HUNTER
      case CLASS_MAGE: class = CB_MAGE
      case CLASS_ROGUE: class = CB_ROGUE
      case CLASS_WARLOCK: class = CB_WARLOCK
      case CLASS_PRIEST: class = CB_PRIEST
   }

   for ( new j = 10; j < MAX_INV; j++ ){
      gidx = p_inventory[id][j] 

      if ( equali( gear[gidx], arg ) ){
         // Make sure this class can wear this item
         if ( !( class & g_classes[gidx] ) ){
            client_print( id, print_chat, "[%s] Your class is restricted from wearing '%s'", MOD, gear[gidx] )
            return PLUGIN_HANDLED
         }

         if ( ( g_slot[gidx] == SLOT_RING1 ) || ( g_slot[gidx] == SLOT_RING2 ) ){
            new ring_slot = -1 
            if ( (p_inventory[id][IDX_GEAR_RING1] > 0) && (p_inventory[id][IDX_GEAR_RING2] > 0) ){
               client_print( id, print_chat, "[%s] You are wearing 2 rings already, you must remove one", MOD )
               return PLUGIN_HANDLED
            }else if ( p_inventory[id][IDX_GEAR_RING1] > 0 ){
               p_ring2[id] = gidx
               ring_slot = IDX_GEAR_RING2
            }else{
               p_ring1[id] = gidx
               ring_slot = IDX_GEAR_RING1
            }

            p_inventory[id][ring_slot] = gidx
            p_inventory[id][j] = 0
         }else{
            // Scan all slots for the right one
            for ( new i = IDX_GEAR_HEAD; i <= IDX_GEAR_NECK; i++ ){
               if ( g_slot[gidx] == (i+1) ){
                  // Found the right equip slot
                  if ( p_inventory[id][i] > 0 ){
                     // Player wearing something, must be remoed first
                     client_print( id, print_chat, 
                        "[%s] You must first remove %s (ex. /rem itemname)", 
                        MOD, gear[p_inventory[id][i]] )
                     return PLUGIN_HANDLED
                  }else{
                     p_inventory[id][i] = gidx
                     p_inventory[id][j] = 0
                     break
                  }
               }
            }
         }

         // Handle worn gear positions, except rings which are handled above
         switch( g_slot[gidx] ){
            case SLOT_HEAD: p_head[id] = gidx
            case SLOT_CHEST: p_chest[id] = gidx
            case SLOT_BACK: p_back[id] = gidx
            case SLOT_LEGS: p_legs[id] = gidx
            case SLOT_HANDS: p_hands[id] = gidx
            case SLOT_FEET: p_feet[id] = gidx
            case SLOT_WRISTS: p_wrists[id] = gidx
            case SLOT_NECK: p_neck[id] = gidx
         }

         // Modify attributes and proficiencies
         p_str[id] += g_str[gidx]
         p_agi[id] += g_agi[gidx]
         p_stam[id] += g_stam[gidx]
         p_int[id] += g_int[gidx]
         p_int_unb[id] += g_int[gidx]
         p_spir[id] += g_spir[gidx]
         p_armor[id] += g_armor[gidx]
         p_mana[id] += g_mana[gidx]
         p_mana_base[id] += g_mana[gidx]

         p_melee[id] += g_melee[gidx]
         p_marksman[id] += g_marksman[gidx]
         p_arcane[id] += g_arcane[gidx]
         p_fire[id] += g_fire[gidx]
         p_frost[id] += g_frost[gidx]
         p_shadow[id] += g_shadow[gidx]
         p_nature[id] += g_nature[gidx]
         p_holy[id] += g_holy[gidx]

         p_crit[id] += g_crit[gidx]
         p_spcrit[id] += g_spcrit[gidx]
         p_spdmg[id] += g_spdmg[gidx]

         client_print( id, print_chat, "[%s] You start wearing '%s'", MOD, gear[gidx] )
         return PLUGIN_HANDLED
      }
   }

   client_print( id, print_chat, "[%s] Nothing found in your inventory matching '%s'", MOD, arg )
   return PLUGIN_HANDLED

}


// **************************************************************************
// BEGIN wowmod_gear_wear_all
// Player command to wear all gear
// **************************************************************************
public wowmod_gear_wear_all( id, quiet ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new bool:slot_ok = false 
   new class = -1
   new gidx = -1  // inventory index for item found
   new temp = -1
   new inv1 = -1
   new inv2 = -1

   if ( p_class1[id] == -1 ){
      if ( !quiet )
         client_print( id, print_chat, 
            "[%s] You must choose a primary class before being able to wear equipment", MOD )
      return PLUGIN_HANDLED
   }

   switch ( p_class1[id] ){
      case CLASS_DRUID: class = CB_DRUID
      case CLASS_HUNTER: class = CB_HUNTER
      case CLASS_MAGE: class = CB_MAGE
      case CLASS_ROGUE: class = CB_ROGUE
      case CLASS_WARLOCK: class = CB_WARLOCK
      case CLASS_PRIEST: class = CB_PRIEST
   }

   // Bubble sort, not the fastest but for n=100 items why get fancy
   for ( new i = 10; i < MAX_INV; i++ ){
      for ( new j = i+1; j < MAX_INV; j++ ){
         inv1 = p_inventory[id][i]
         inv2 = p_inventory[id][j]
         if ( ( inv1 <= 0 ) || ( g_tier[inv1] < g_tier[inv2] ) ){
            temp = inv1
            p_inventory[id][i] = inv2
            p_inventory[id][j] = temp
         }
      }
   }

   /*
   // Debug code for testing sorted inventory
   client_print( id, print_console, "After Sorting" )
   for ( new i = 10; i < MAX_INV; i++ ){
      inv1 = p_inventory[id][i]
      if ( inv1 != -1 ){
         client_print( id, print_console, "[%d] inv=(%d) t=(%d)", i, inv1, g_tier[inv1] )
      }
   }
   return PLUGIN_HANDLED
   */

   new num_worn = 0

   for ( new j = 10; j < MAX_INV; j++ ){
      slot_ok = false 

      if ( p_inventory[id][j] <= 0 )
         continue

      gidx = p_inventory[id][j] 

      // Make sure this class can wear this item
      if ( !( class & g_classes[gidx] ) )
         continue

      if ( ( g_slot[gidx] == SLOT_RING1 ) || ( g_slot[gidx] == SLOT_RING2 ) ){
         new ring_slot = -1 
         if ( (p_inventory[id][IDX_GEAR_RING1] > 0) && (p_inventory[id][IDX_GEAR_RING2] > 0) ){
            continue
         }else if ( p_inventory[id][IDX_GEAR_RING1] > 0 ){
            slot_ok = true
            p_ring2[id] = gidx
            ring_slot = IDX_GEAR_RING2
         }else{
            slot_ok = true
            p_ring1[id] = gidx
            ring_slot = IDX_GEAR_RING1
         }

         p_inventory[id][ring_slot] = gidx
         p_inventory[id][j] = 0
      }else{
         // Scan all slots for the right one
         for ( new i = IDX_GEAR_HEAD; i <= IDX_GEAR_NECK; i++ ){
            if ( g_slot[gidx] == (i+1) ){
               // Found the right equip slot
               if ( p_inventory[id][i] <= 0 ){
                  slot_ok = true
                  p_inventory[id][i] = gidx
                  p_inventory[id][j] = 0
                  break
               }
            }
         }
      }

      if ( !slot_ok )
         continue

      num_worn += 1

      client_print( id, print_chat, "[%s] You start wearing '%s'", MOD, gear[gidx] )

      // Handle worn gear positions, except rings which are handled above
      switch( g_slot[gidx] ){
         case SLOT_HEAD: p_head[id] = gidx
         case SLOT_CHEST: p_chest[id] = gidx
         case SLOT_BACK: p_back[id] = gidx
         case SLOT_LEGS: p_legs[id] = gidx
         case SLOT_HANDS: p_hands[id] = gidx
         case SLOT_FEET: p_feet[id] = gidx
         case SLOT_WRISTS: p_wrists[id] = gidx
         case SLOT_NECK: p_neck[id] = gidx
      }

      // Modify attributes and proficiencies
      p_str[id] += g_str[gidx]
      p_agi[id] += g_agi[gidx]
      p_stam[id] += g_stam[gidx]
      p_int[id] += g_int[gidx]
      p_int_unb[id] += g_int[gidx]
      p_spir[id] += g_spir[gidx]
      p_armor[id] += g_armor[gidx]
      p_mana[id] += g_mana[gidx]
      p_mana_base[id] += g_mana[gidx]

      p_melee[id] += g_melee[gidx]
      p_marksman[id] += g_marksman[gidx]
      p_arcane[id] += g_arcane[gidx]
      p_fire[id] += g_fire[gidx]
      p_frost[id] += g_frost[gidx]
      p_shadow[id] += g_shadow[gidx]
      p_nature[id] += g_nature[gidx]
      p_holy[id] += g_holy[gidx]

      p_crit[id] += g_crit[gidx]
      p_spcrit[id] += g_spcrit[gidx]
      p_spdmg[id] += g_spdmg[gidx]
   }

   if ( num_worn > 0 )
      client_print( id, print_chat, "[%s] %d items worn from your inventory", MOD, num_worn )
   else
      client_print( id, print_chat, "[%s] No wearable items were found in your inventory", MOD )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_gear_remove
// Player command to remove a piece of gear
// **************************************************************************
public wowmod_gear_remove( id, arg[] ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   //log_amx( "Remove: %s", arg )

   new gidx = -1  // inventory index for item found

   new sidx =  wowmod_gear_nextinvidx( id )

   if ( sidx == -1 ){
      client_print( id, print_chat, 
         "[%s] You have no inventory space left, free a space by saying '/drop item_name' to drop an item", MOD )
      return PLUGIN_HANDLED
   }

   for ( new j = IDX_GEAR_HEAD; j <= IDX_GEAR_NECK; j++ ){
      gidx = p_inventory[id][j] 

      if ( equali( gear[gidx], arg ) ){
         if ( ( g_slot[gidx] == SLOT_RING1 ) || ( g_slot[gidx] == SLOT_RING2 ) ){
            if ( j == IDX_GEAR_RING1 )
               p_ring1[id] = 0 
            else if ( j == IDX_GEAR_RING2 )
               p_ring2[id] = 0 
         }

         switch( g_slot[gidx] ){
            case SLOT_HEAD: p_head[id] = 0
            case SLOT_CHEST: p_chest[id] = 0 
            case SLOT_BACK: p_back[id] = 0 
            case SLOT_LEGS: p_legs[id] = 0 
            case SLOT_HANDS: p_hands[id] = 0 
            case SLOT_FEET: p_feet[id] = 0 
            case SLOT_WRISTS: p_wrists[id] = 0 
            case SLOT_NECK: p_neck[id] = 0 
         }

         p_inventory[id][j] = 0
         p_inventory[id][sidx] = gidx

         // Modify attributes and proficiencies
         p_str[id] -= g_str[gidx]
         p_agi[id] -= g_agi[gidx]
         p_stam[id] -= g_stam[gidx]
         p_int[id] -= g_int[gidx]
         p_int_unb[id] -= g_int[gidx]
         p_spir[id] -= g_spir[gidx]
         p_armor[id] -= g_armor[gidx]
         p_mana[id] -= g_mana[gidx]
         p_mana_base[id] -= g_mana[gidx]

         p_melee[id] -= g_melee[gidx]
         p_marksman[id] -= g_marksman[gidx]
         p_arcane[id] -= g_arcane[gidx]
         p_fire[id] -= g_fire[gidx]
         p_frost[id] -= g_frost[gidx]
         p_shadow[id] -= g_shadow[gidx]
         p_nature[id] -= g_nature[gidx]
         p_holy[id] -= g_holy[gidx]

         p_crit[id] -= g_crit[gidx]
         p_spcrit[id] -= g_spcrit[gidx]
         p_spdmg[id] -= g_spdmg[gidx]

         client_print( id, print_chat, "[%s] You are no longer wearing '%s'", MOD, gear[gidx] )
         return PLUGIN_HANDLED
      }
   }

   client_print( id, print_chat, "[%s] Nothing found in your worn equipment matching '%s'", MOD, arg )
   return PLUGIN_HANDLED

}


// **************************************************************************
// BEGIN wowmod_gear_remove_all
// Player command to remove all worn gear
// **************************************************************************
public wowmod_gear_remove_all( id, quiet ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new sidx = -1
   new gidx = -1  // inventory index for item found
   new bool:found = false

   for ( new j = IDX_GEAR_HEAD; j <= IDX_GEAR_NECK; j++ ){
      sidx =  wowmod_gear_nextinvidx( id )
      if ( sidx == -1 ){
         if ( !quiet )
            client_print( id, print_chat, 
               "[%s] You have no inventory space left, free a space by saying '/drop item_name' to drop an item",
               MOD )
         return PLUGIN_HANDLED
      }

      if ( !( p_inventory[id][j] ) )
         continue

      if ( !found )
         found = true

      gidx = p_inventory[id][j] 

      if ( ( g_slot[gidx] == SLOT_RING1 ) || ( g_slot[gidx] == SLOT_RING2 ) ){
         if ( j == IDX_GEAR_RING1 )
            p_ring1[id] = 0 
         else if ( j == IDX_GEAR_RING2 )
            p_ring2[id] = 0 
      }

      switch( g_slot[gidx] ){
         case SLOT_HEAD: p_head[id] = 0
         case SLOT_CHEST: p_chest[id] = 0 
         case SLOT_BACK: p_back[id] = 0 
         case SLOT_LEGS: p_legs[id] = 0 
         case SLOT_HANDS: p_hands[id] = 0 
         case SLOT_FEET: p_feet[id] = 0 
         case SLOT_WRISTS: p_wrists[id] = 0 
         case SLOT_NECK: p_neck[id] = 0 
      }

      p_inventory[id][j] = 0
      p_inventory[id][sidx] = gidx

      // Modify attributes and proficiencies
      p_str[id] -= g_str[gidx]
      p_agi[id] -= g_agi[gidx]
      p_stam[id] -= g_stam[gidx]
      p_int[id] -= g_int[gidx]
      p_int_unb[id] -= g_int[gidx]
      p_spir[id] -= g_spir[gidx]
      p_armor[id] -= g_armor[gidx]
      p_mana[id] -= g_mana[gidx]
      p_mana_base[id] -= g_mana[gidx]

      p_melee[id] -= g_melee[gidx]
      p_marksman[id] -= g_marksman[gidx]
      p_arcane[id] -= g_arcane[gidx]
      p_fire[id] -= g_fire[gidx]
      p_frost[id] -= g_frost[gidx]
      p_shadow[id] -= g_shadow[gidx]
      p_nature[id] -= g_nature[gidx]
      p_holy[id] -= g_holy[gidx]

      p_crit[id] -= g_crit[gidx]
      p_spcrit[id] -= g_spcrit[gidx]
      p_spdmg[id] -= g_spdmg[gidx]

      if ( !quiet )
         client_print( id, print_chat, "[%s] You have removed to your inventory: %s", MOD, gear[gidx] )
   }

   if ( !found && !quiet )
      client_print( id, print_chat, "[%s] No worn equipment was found - say /worn to see your worn gear", MOD )
   
   return PLUGIN_HANDLED

}


// **************************************************************************
// BEGIN wowmod_gear_drop
// Player command to drop a piece of gear
// **************************************************************************
public wowmod_gear_drop( id, arg[] ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   //log_amx( "Drop: %s", arg )

   new gidx = -1  // inventory index for item found

   for ( new j = 0; j < MAX_INV; j++ ){
      gidx = p_inventory[id][j] 

      if ( equali( gear[gidx], arg ) ){
         // Handle rings differently
         if ( ( g_slot[gidx] == SLOT_RING1 ) || ( g_slot[gidx] == SLOT_RING2 ) ){
            if ( j == IDX_GEAR_RING1 )
               p_ring1[id] = 0 
            else if ( j == IDX_GEAR_RING2 )
               p_ring2[id] = 0 
         }

         if ( j <= IDX_GEAR_NECK ){
            switch( g_slot[gidx] ){
               case SLOT_HEAD: p_head[id] = 0
               case SLOT_CHEST: p_chest[id] = 0 
               case SLOT_BACK: p_back[id] = 0 
               case SLOT_LEGS: p_legs[id] = 0 
               case SLOT_HANDS: p_hands[id] = 0 
               case SLOT_FEET: p_feet[id] = 0 
               case SLOT_WRISTS: p_wrists[id] = 0 
               case SLOT_NECK: p_neck[id] = 0 
            }


            // Item worn so modify attributes and proficiencies
            p_str[id] -= g_str[gidx]
            p_agi[id] -= g_agi[gidx]
            p_stam[id] -= g_stam[gidx]
            p_int[id] -= g_int[gidx]
            p_int_unb[id] -= g_int[gidx]
            p_spir[id] -= g_spir[gidx]
            p_armor[id] -= g_armor[gidx]
            p_mana[id] -= g_mana[gidx]
            p_mana_base[id] -= g_mana[gidx]

            p_melee[id] -= g_melee[gidx]
            p_marksman[id] -= g_marksman[gidx]
            p_arcane[id] -= g_arcane[gidx]
            p_fire[id] -= g_fire[gidx]
            p_frost[id] -= g_frost[gidx]
            p_shadow[id] -= g_shadow[gidx]
            p_nature[id] -= g_nature[gidx]
            p_holy[id] -= g_holy[gidx]

            p_crit[id] -= g_crit[gidx]
            p_spcrit[id] -= g_spcrit[gidx]
            p_spdmg[id] -= g_spdmg[gidx]
         }

         p_inventory[id][j] = 0

         client_print( id, print_chat, "[%s] You have dropped '%s'", MOD, gear[gidx] )
         return PLUGIN_HANDLED
      }
   }

   client_print( id, print_chat, "[%s] Nothing found in your inventory matching '%s'", MOD, arg )
   return PLUGIN_HANDLED

}


// **************************************************************************
// BEGIN wowmod_gear_getcraftable
// Get a list of all craftable pieces of WowMOD gear for a player
// **************************************************************************
public wowmod_gear_getcraftable( id, list[MAX_GLIST] ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new name[32]
   get_user_name( id, name, 31 )

   //log_amx( "wowmod_gear_craftable() [%d](%s) tier=(%d)", id, name, p_tier[id] )

   new class = -1
   switch ( p_class1[id] ){
      case CLASS_DRUID: class = CB_DRUID
      case CLASS_HUNTER: class = CB_HUNTER
      case CLASS_MAGE: class = CB_MAGE
      case CLASS_ROGUE: class = CB_ROGUE
      case CLASS_WARLOCK: class = CB_WARLOCK
      case CLASS_PRIEST: class = CB_PRIEST
   }

   //log_amx( "MIH2 class=(%d)", class )

   new count = 0
   new idx = 0

   for ( new j = 1; j < MAX_GEAR; j++ ){
      // Skip magical items, we only want armor
      if ( (g_slot[j] == SLOT_RING1) || (g_slot[j] == SLOT_RING2) || (g_slot[j] == SLOT_NECK) )
         continue
      copy( name, 31, gear[j] ) 
      //log_amx( "MIH3 [%d] (%s) g_tier=(%d) g_classes=(%d)", j, name, g_tier[j], g_classes[j] )
      if ( ( g_tier[j] <= p_tier[id] ) && ( g_classes[j] & class ) ){
         count += 1
         list[idx++] = j         
         //log_amx( "getcraftable() found elligible gear (%d) %s", j, name )
      }
   }

   //log_amx( "wowmod_gear_craftable() [%d](%s) tier=(%d) count=(%d)", id, name, p_tier[id], count )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_gear_getitems
// Get a list of all available WowMOD magical items for a player
// **************************************************************************
public wowmod_gear_getitems( id, list[MAX_GLIST] ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   //log_amx( "MIH1 id=(%d) tier=(%d)", id, p_tier[id] )

   new class = -1
   switch ( p_class1[id] ){
      case CLASS_DRUID: class = CB_DRUID
      case CLASS_HUNTER: class = CB_HUNTER
      case CLASS_MAGE: class = CB_MAGE
      case CLASS_ROGUE: class = CB_ROGUE
      case CLASS_WARLOCK: class = CB_WARLOCK
      case CLASS_PRIEST: class = CB_PRIEST
   }

   //log_amx( "MIH2 class=(%d)", class )
   //client_print( 0, print_console, "DEBUG: In wowmod_gear_getitems() id=(%d) class=(%d) tier=(%d)", 
   //   id, class, p_tier[id] )

   new name[32]
   new idx = 0

   for ( new j = 1; j < MAX_GEAR; j++ ){
      // Skip armor pieces, we only want magic items ( rings, necklaces, etc )
      if ( (g_slot[j] != SLOT_RING1) && (g_slot[j] != SLOT_RING2) && (g_slot[j] != SLOT_NECK) )
         continue

      copy( name, 31, gear[j] ) 
      //log_amx( "MIH3 [%d] (%s) g_tier=(%d) g_classes=(%d)", j, name, g_tier[j], g_classes[j] )
      if ( ( g_tier[j] <= p_tier[id] ) && ( g_classes[j] & class ) ){
         list[idx++] = j         
         //log_amx( "getitems() found elligible item (%d) %s", j, name )
         //client_print( 0, print_console, "DEBUG: In wowmod_gear_getitems() found elligible item (%d) %s", 
         //   j, name )
      }
   }

   return PLUGIN_CONTINUE
}



// **************************************************************************
// BEGIN wowmod_gear_init
// Create all known pieces of WowMOD gear 
// **************************************************************************
public wowmod_gear_init(){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   log_amx("WowMOD Gear :: Initializing Gear System..." )

   gear[0] = "NA"

   new gid = 1

   // wowmod_create_gear() arguments List:
   // gid, name, slot, classes, tier, cost, 
   // str, agi, stam, int, spir, armor, mana, melee, mark, arc, fire, frst, shad, nat, hol, crit, spcrit, spdmg, spec_idx

   // Leather Gear Tier1 
   gid = wowmod_create_gear( gid, "Ragged Leather Cap", SLOT_HEAD, CB_DRUID|CB_ROGUE, TIER1, 25,
      0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Ragged Leather Jerkin", SLOT_CHEST, CB_DRUID|CB_ROGUE, TIER1, 50,
      0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Ragged Leather Leggings", SLOT_LEGS, CB_DRUID|CB_ROGUE, TIER1, 45,
      0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Ragged Leather Gloves", SLOT_HANDS, CB_DRUID|CB_ROGUE, TIER1, 35,
      0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Ragged Leather Boots", SLOT_FEET, CB_DRUID|CB_ROGUE, TIER1, 25,
      0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )

   // Cloth Gear Tier1 
   gid = wowmod_create_gear( gid, "Torn Cloth Hat", SLOT_HEAD, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER1, 25,
      0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Torn Cloth Shirt", SLOT_CHEST, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER1, 50,
      0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Torn Cloth Pants", SLOT_LEGS, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER1, 45,
      0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Torn Cloth Gloves", SLOT_HANDS, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER1, 35,
      0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Torn Cloth Slippers", SLOT_FEET, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER1, 25,
      0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 )

   // Chain Gear Tier1 
   gid = wowmod_create_gear( gid, "Worn Chain Helm", SLOT_HEAD, CB_HUNTER, TIER1, 25,
      1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Worn Chain Breastplate", SLOT_CHEST, CB_HUNTER, TIER1, 50,
      1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Worn Chain Leggings", SLOT_LEGS, CB_HUNTER, TIER1, 45,
      1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Worn Chain Gloves", SLOT_HANDS, CB_HUNTER, TIER1, 35,
      1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Worn Chain Boots", SLOT_FEET, CB_HUNTER, TIER1, 25,
      1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )


   // Leather Gear Tier2
   gid = wowmod_create_gear( gid, "Studded Leather Helmet", SLOT_HEAD, CB_DRUID|CB_ROGUE, TIER2, 150,
      0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Studded Leather Vest", SLOT_CHEST, CB_DRUID|CB_ROGUE, TIER2, 200,
      0, 2, 2, 2, 2, 2, 1, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Studded Leather Leggings", SLOT_LEGS, CB_DRUID|CB_ROGUE, TIER2, 175,
      0, 2, 2, 1, 2, 2, 1, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Studded Leather Gloves", SLOT_HANDS, CB_DRUID|CB_ROGUE, TIER2, 135,
      0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Studded Leather Boots", SLOT_FEET, CB_DRUID|CB_ROGUE, TIER2, 125,
      0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 )

   // Cloth Gear Tier2 
   gid = wowmod_create_gear( gid, "Fine Cloth Hat", SLOT_HEAD, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER2, 150,
      0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Fine Cloth Shirt", SLOT_CHEST, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER2, 200,
      0, 0, 1, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 2, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Fine Cloth Pants", SLOT_LEGS, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER2, 175,
      0, 0, 1, 2, 2, 2, 2, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Fine Cloth Gloves", SLOT_HANDS, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER2, 135,
      0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Fine Cloth Slippers", SLOT_FEET, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER2, 125,
      0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 )

   // Chain Gear Tier2 
   gid = wowmod_create_gear( gid, "Sturdy Chain Helm", SLOT_HEAD, CB_HUNTER, TIER2, 150,
      1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Sturdy Chain Breastplate", SLOT_CHEST, CB_HUNTER, TIER2, 200,
      2, 1, 2, 0, 0, 2, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Sturdy Chain Leggings", SLOT_LEGS, CB_HUNTER, TIER2, 175,
      1, 1, 1, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Sturdy Chain Gloves", SLOT_HANDS, CB_HUNTER, TIER2, 135,
      0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Sturdy Chain Boots", SLOT_FEET, CB_HUNTER, TIER2, 125,
      1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )

   // Cloak Gear Tier2 
   gid = wowmod_create_gear( gid, "Cloak of Minor Melee", SLOT_BACK, CB_ALL, TIER2, 75,
      0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Minor Marksmanship", SLOT_BACK, CB_ALL, TIER2, 75,
      0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Minor Arcane", SLOT_BACK, CB_ALL, TIER2, 75,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Minor Fire", SLOT_BACK, CB_ALL, TIER2, 75,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Minor Frost", SLOT_BACK, CB_ALL, TIER2, 75,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Minor Shadow", SLOT_BACK, CB_ALL, TIER2, 75,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Minor Nature", SLOT_BACK, CB_ALL, TIER2, 75,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Minor Holy", SLOT_BACK, CB_ALL, TIER2, 75,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0 )

   // Leather Gear Tier3
   gid = wowmod_create_gear( gid, "Reinforced Leather Helmet", SLOT_HEAD, CB_DRUID|CB_ROGUE, TIER3, 250,
      0, 2, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Reinforced Leather Jerkin", SLOT_CHEST, CB_DRUID|CB_ROGUE, TIER3, 300,
      0, 3, 3, 3, 3, 3, 2, 3, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Reinforced Leather Leggings", SLOT_LEGS, CB_DRUID|CB_ROGUE, TIER3, 275,
      0, 3, 3, 2, 3, 3, 2, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Reinforced Leather Gloves", SLOT_HANDS, CB_DRUID|CB_ROGUE, TIER3, 235,
      0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Reinforced Leather Boots", SLOT_FEET, CB_DRUID|CB_ROGUE, TIER3, 225,
      0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0 )

   // Cloth Gear Tier3 
   gid = wowmod_create_gear( gid, "Silk Cap", SLOT_HEAD, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER3, 250,
      0, 0, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2, 0, 2, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Silk Robe", SLOT_CHEST, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER3, 200,
      0, 0, 2, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 0, 3, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Silk Pants", SLOT_LEGS, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER3, 275,
      0, 0, 2, 3, 3, 3, 3, 0, 0, 2, 2, 2, 2, 0, 2, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Silk Handwraps", SLOT_HANDS, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER3, 235,
      0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Silk Shoes", SLOT_FEET, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER3, 225,
      0, 0, 0, 0, 2, 0, 2, 0, 0, 2, 2, 2, 2, 0, 2, 0, 0, 0, 0 )

   // Chain Gear Tier3 
   gid = wowmod_create_gear( gid, "Mail Helm", SLOT_HEAD, CB_HUNTER, TIER3, 250,
      2, 2, 2, 0, 0, 2, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Mail Armor", SLOT_CHEST, CB_HUNTER, TIER3, 300,
      3, 2, 3, 0, 0, 3, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Mail Legguards", SLOT_LEGS, CB_HUNTER, TIER3, 275,
      2, 2, 2, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Mail Gauntlets", SLOT_HANDS, CB_HUNTER, TIER3, 235,
      0, 2, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Mail Boots", SLOT_FEET, CB_HUNTER, TIER3, 225,
      2, 2, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )

   // Cloak Gear Tier3 
   gid = wowmod_create_gear( gid, "Cloak of Melee", SLOT_BACK, CB_ALL, TIER3, 175,
      0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Marksmanship", SLOT_BACK, CB_ALL, TIER3, 175,
      0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Arcane", SLOT_BACK, CB_ALL, TIER3, 175,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Fire", SLOT_BACK, CB_ALL, TIER3, 175,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Frost", SLOT_BACK, CB_ALL, TIER3, 175,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Shadow", SLOT_BACK, CB_ALL, TIER3, 175,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Nature", SLOT_BACK, CB_ALL, TIER3, 175,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cloak of Holy", SLOT_BACK, CB_ALL, TIER3, 175,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0 )

   // Leather Gear Tier4
   gid = wowmod_create_gear( gid, "Toughened Leather Helm", SLOT_HEAD, CB_DRUID|CB_ROGUE, TIER4, 350,
      0, 3, 0, 3, 3, 0, 3, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Toughened Leather Tunic", SLOT_CHEST, CB_DRUID|CB_ROGUE, TIER4, 400,
      0, 4, 4, 4, 4, 4, 3, 4, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Toughened Leather Pants", SLOT_LEGS, CB_DRUID|CB_ROGUE, TIER4, 375,
      0, 4, 4, 3, 4, 4, 3, 4, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Toughened Leather Gloves", SLOT_HANDS, CB_DRUID|CB_ROGUE, TIER4, 335,
      0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Toughened Leather Boots", SLOT_FEET, CB_DRUID|CB_ROGUE, TIER4, 325,
      0, 3, 0, 3, 0, 3, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0 )

   // Cloth Gear Tier4 
   gid = wowmod_create_gear( gid, "Fine Silk Cap", SLOT_HEAD, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER4, 350,
      0, 0, 3, 3, 0, 0, 3, 0, 0, 3, 3, 3, 3, 0, 3, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Fine Silk Robe", SLOT_CHEST, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER4, 300,
      0, 0, 3, 4, 4, 4, 4, 0, 0, 4, 4, 4, 4, 0, 4, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Fine Silk Pants", SLOT_LEGS, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER4, 375,
      0, 0, 3, 4, 4, 4, 4, 0, 0, 3, 3, 3, 3, 0, 3, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Fine Silk Handwraps", SLOT_HANDS, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER4, 335,
      0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Fine Silk Shoes", SLOT_FEET, CB_MAGE|CB_WARLOCK|CB_PRIEST, TIER4, 325,
      0, 0, 0, 0, 3, 0, 3, 0, 0, 3, 3, 3, 3, 0, 3, 0, 0, 0, 0 )

   // Chain Gear Tier4 
   gid = wowmod_create_gear( gid, "Banded Mail Visor", SLOT_HEAD, CB_HUNTER, TIER4, 350,
      3, 3, 3, 0, 0, 3, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Banded Mail Chestpiece", SLOT_CHEST, CB_HUNTER, TIER4, 400,
      4, 3, 4, 0, 0, 4, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Banded Mail Greaves", SLOT_LEGS, CB_HUNTER, TIER4, 375,
      3, 3, 3, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Banded Mail Handguards", SLOT_HANDS, CB_HUNTER, TIER4, 335,
      0, 3, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Banded Mail Sabatons", SLOT_FEET, CB_HUNTER, TIER4, 325,
      3, 3, 3, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )

   // BEGIN Magical Items for Items Merchant
   // gid, name, slot, classes, tier, cost, 
   // str, agi, stam, int, spir, armor, mana, melee, mark, arc, fire, frst, shad, nat, hol, crit, spcrit, spdmg, spec_idx

   // Magical Items Tier1 - Rings
   gid = wowmod_create_gear( gid, "Dull Silver Ring", SLOT_RING1, CB_ALL, TIER1, 50,
      1, 1, 1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dull Red Ring", SLOT_RING1, CB_ALL, TIER1, 50,
      0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dull Blue Ring", SLOT_RING1, CB_ALL, TIER1, 50,
      0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dull Black Ring", SLOT_RING1, CB_ALL, TIER1, 50,
      0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dull Green Ring", SLOT_RING1, CB_ALL, TIER1, 50,
      0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dull Glowing Ring", SLOT_RING1, CB_ALL, TIER1, 50,
      0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0 )

   // Magical Items Tier2 - Rings
   gid = wowmod_create_gear( gid, "Silver Ring", SLOT_RING1, CB_ALL, TIER2, 200,
      2, 2, 2, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Red Ring", SLOT_RING1, CB_ALL, TIER2, 200,
      0, 0, 0, 2, 2, 0, 2, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Blue Ring", SLOT_RING1, CB_ALL, TIER2, 200,
      0, 0, 0, 2, 2, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Black Ring", SLOT_RING1, CB_ALL, TIER2, 200,
      0, 0, 0, 2, 2, 0, 2, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Green Ring", SLOT_RING1, CB_ALL, TIER2, 200,
      0, 0, 0, 2, 2, 0, 2, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Glowing Ring", SLOT_RING1, CB_ALL, TIER2, 200,
      0, 0, 0, 2, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0 )

   // Magical Items Tier3 - Rings
   gid = wowmod_create_gear( gid, "Shiny Silver Ring", SLOT_RING1, CB_ALL, TIER3, 350,
      3, 3, 3, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Shiny Red Ring", SLOT_RING1, CB_ALL, TIER3, 350,
      0, 0, 0, 3, 3, 0, 5, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Shiny Blue Ring", SLOT_RING1, CB_ALL, TIER3, 350,
      0, 0, 0, 3, 3, 0, 5, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Shiny Black Ring", SLOT_RING1, CB_ALL, TIER3, 350,
      0, 0, 0, 3, 3, 0, 5, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Shiny Green Ring", SLOT_RING1, CB_ALL, TIER3, 350,
      0, 0, 0, 3, 3, 0, 5, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Shiny Glowing Ring", SLOT_RING1, CB_ALL, TIER3, 350,
      0, 0, 0, 3, 3, 0, 5, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0 )

   // Magical Items Tier2 - Necklaces
   gid = wowmod_create_gear( gid, "Necklace of Minor Strength", SLOT_NECK, CB_ALL, TIER2, 200,
      10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Necklace of Minor Intellect", SLOT_NECK, CB_ALL, TIER2, 200,
      0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Necklace of Minor Stamina", SLOT_NECK, CB_ALL, TIER2, 200,
      0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Necklace of Minor Agility", SLOT_NECK, CB_ALL, TIER2, 200,
      0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Necklace of Minor Mana", SLOT_NECK, CB_ALL, TIER2, 200,
      0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )

   // Magical Items Tier3 - Necklaces
   gid = wowmod_create_gear( gid, "Necklace of Strength", SLOT_NECK, CB_ALL, TIER3, 350,
      20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Necklace of Intellect", SLOT_NECK, CB_ALL, TIER3, 350,
      0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Necklace of Stamina", SLOT_NECK, CB_ALL, TIER3, 350,
      0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Necklace of Agility", SLOT_NECK, CB_ALL, TIER3, 350,
      0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Necklace of Mana", SLOT_NECK, CB_ALL, TIER3, 350,
      0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )

   // Magical Items Tier4 - Necklaces
   gid = wowmod_create_gear( gid, "Necklace of Improved Strength", SLOT_NECK, CB_ALL, TIER4, 350,
      30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Necklace of Improved Intellect", SLOT_NECK, CB_ALL, TIER4, 350,
      0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Necklace of Improved Stamina", SLOT_NECK, CB_ALL, TIER4, 350,
      0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Necklace of Improved Agility", SLOT_NECK, CB_ALL, TIER4, 350,
      0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Necklace of Improved Mana", SLOT_NECK, CB_ALL, TIER4, 350,
      0, 0, 0, 0, 0, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )

   // gid, name, slot, classes, tier, cost, 
   // str, agi, stam, int, spir, armor, mana, melee, mark, arc, fire, frst, shad, nat, hol, crit, spcrit, spdmg, spec_idx

   // Armor Tier3 - Bracers/Wristguards 
   gid = wowmod_create_gear( gid, "Bracers of Critical Strike", SLOT_WRISTS, CB_ALL, TIER1, 375,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 )

   // Armor Tier4 - Bracers/Wristguards 
   gid = wowmod_create_gear( gid, "Wristguards of Critical Strike", SLOT_WRISTS, CB_ALL, TIER2, 675,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 )

   // Armor Tier5 - Bracers/Wristguards 
   gid = wowmod_create_gear( gid, "Bracers of Power", SLOT_WRISTS, CB_ALL, TIER3, 1000,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0 )

   // Armor Tier6 - Bracers/Wristguards 
   gid = wowmod_create_gear( gid, "Wristguards of Power", SLOT_WRISTS, CB_ALL, TIER4, 2500,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0 )

   // gid, name, slot, classes, tier, cost, 
   // str, agi, stam, int, spir, armor, mana, melee, mark, arc, fire, frst, shad, nat, hol, crit, spcrit, spdmg, spec_idx

   // Druid Tier5 (Epic1) Set
   gid = wowmod_create_gear( gid, "Cenarion Helm", SLOT_HEAD, CB_DRUID, TIER5, 1000,
      2, 2, 4, 5, 5, 2, 10, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cenarion Vestments", SLOT_CHEST, CB_DRUID, TIER5, 1500,
      2, 2, 4, 5, 5, 5, 20, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cenarion Leggings", SLOT_LEGS, CB_DRUID, TIER5, 1350,
      2, 2, 4, 5, 5, 5, 20, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cenarion Gloves", SLOT_HANDS, CB_DRUID, TIER5, 1050,
      2, 2, 4, 5, 3, 2, 10, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cenarion Boots", SLOT_FEET, CB_DRUID, TIER5, 1125,
      2, 2, 2, 3, 3, 2, 10, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cenarion Bracers", SLOT_WRISTS, CB_DRUID, TIER5, 1250,
      2, 2, 2, 3, 3, 2, 15, 0, 0, 0, 0, 0, 0, 3, 0, 1, 1, 1, 0 )

   // Hunter Tier5 (Epic1) Set
   gid = wowmod_create_gear( gid, "Gianstalker's Helmet", SLOT_HEAD, CB_HUNTER, TIER5, 1000,
      2, 5, 4, 5, 5, 5, 10, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gianstalker's Breastplate", SLOT_CHEST, CB_HUNTER, TIER5, 1500,
      2, 5, 4, 5, 5, 7, 15, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Giantstalker's Leggings", SLOT_LEGS, CB_HUNTER, TIER5, 1350,
      2, 5, 4, 5, 5, 7, 15, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Giantstalker's Gloves", SLOT_HANDS, CB_HUNTER, TIER5, 1050,
      2, 3, 4, 5, 3, 4, 10, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Giantstalker's Boots", SLOT_FEET, CB_HUNTER, TIER5, 1125,
      2, 3, 2, 3, 3, 4, 10, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Giantstalker's Bracers", SLOT_WRISTS, CB_HUNTER, TIER5, 1250,
      2, 3, 2, 3, 3, 3, 10, 0, 2, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 )

   // Mage Tier5 (Epic1) Set
   gid = wowmod_create_gear( gid, "Arcanist Crown", SLOT_HEAD, CB_MAGE, TIER5, 1000,
      2, 2, 4, 5, 5, 2, 10, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Arcanist Robes", SLOT_CHEST, CB_MAGE, TIER5, 1500,
      2, 2, 4, 5, 5, 3, 20, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Arcanist Leggings", SLOT_LEGS, CB_MAGE, TIER5, 1350,
      2, 2, 4, 5, 5, 3, 20, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Arcanist Gloves", SLOT_HANDS, CB_MAGE, TIER5, 1050,
      2, 2, 4, 5, 3, 2, 20, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Arcanist Boots", SLOT_FEET, CB_MAGE, TIER5, 1125,
      2, 2, 2, 3, 3, 2, 10, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Arcanist Bindings", SLOT_WRISTS, CB_MAGE, TIER5, 1250,
      2, 2, 2, 3, 3, 2, 10, 0, 0, 2, 2, 2, 0, 0, 0, 0, 2, 1, 0 )

   // Rogue Tier5 (Epic1) Set
   gid = wowmod_create_gear( gid, "Nightslayer Cover", SLOT_HEAD, CB_ROGUE, TIER5, 1000,
      2, 3, 4, 0, 5, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nightslayer Chestpiece", SLOT_CHEST, CB_ROGUE, TIER5, 1500,
      2, 5, 4, 0, 5, 4, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nightslayer Pants", SLOT_LEGS, CB_ROGUE, TIER5, 1350,
      2, 5, 4, 0, 5, 4, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nightslayer Gloves", SLOT_HANDS, CB_ROGUE, TIER5, 1050,
      2, 5, 4, 0, 3, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nightslayer Boots", SLOT_FEET, CB_ROGUE, TIER5, 1125,
      2, 4, 2, 0, 3, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nightslayer Bracelets", SLOT_WRISTS, CB_ROGUE, TIER5, 1250,
      2, 3, 2, 0, 3, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0 )

   // Warlock Tier5 (Epic1) Set
   gid = wowmod_create_gear( gid, "Felheart Horns", SLOT_HEAD, CB_WARLOCK, TIER5, 1000,
      2, 2, 4, 4, 2, 3, 15, 0, 0, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Felheart Robes", SLOT_CHEST, CB_WARLOCK, TIER5, 1500,
      2, 2, 4, 5, 4, 4, 20, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Felheart Pants", SLOT_LEGS, CB_WARLOCK, TIER5, 1350,
      2, 2, 4, 5, 4, 4, 20, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Felheart Gloves", SLOT_HANDS, CB_WARLOCK, TIER5, 1050,
      2, 2, 4, 3, 3, 3, 15, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Felheart Slippers", SLOT_FEET, CB_WARLOCK, TIER5, 1125,
      2, 2, 2, 3, 3, 3, 10, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Felheart Bracers", SLOT_WRISTS, CB_WARLOCK, TIER5, 1250,
      2, 2, 2, 2, 2, 2, 10, 0, 0, 0, 3, 0, 4, 0, 0, 1, 1, 1, 0 )

   // Priest Tier5 (Epic1) Set
   gid = wowmod_create_gear( gid, "Circlet of Prophecy", SLOT_HEAD, CB_PRIEST, TIER5, 1000,
      2, 2, 4, 4, 3, 2, 10, 0, 0, 0, 0, 0, 3, 0, 4, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Robes of Prophecy", SLOT_CHEST, CB_PRIEST, TIER5, 1500,
      2, 2, 4, 5, 5, 3, 25, 0, 0, 0, 0, 0, 4, 0, 5, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Pants of Prophecy", SLOT_LEGS, CB_PRIEST, TIER5, 1350,
      2, 2, 4, 5, 5, 3, 20, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gloves of Prophecy", SLOT_HANDS, CB_PRIEST, TIER5, 1050,
      2, 2, 4, 3, 3, 3, 10, 0, 0, 0, 0, 0, 4, 0, 3, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Boots of Prophecy", SLOT_FEET, CB_PRIEST, TIER5, 1125,
      2, 2, 2, 3, 3, 3, 10, 0, 0, 0, 0, 0, 4, 0, 3, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Vambraces of Prophecy", SLOT_WRISTS, CB_PRIEST, TIER5, 1250,
      2, 2, 2, 2, 2, 2, 10, 0, 0, 0, 0, 0, 4, 0, 3, 0, 2, 1, 0 )

   // gid, name, slot, classes, tier, cost, 
   // str, agi, stam, int, spir, armor, mana, melee, mark, arc, fire, frst, shad, nat, hol, crit, spcrit, spdmg, spec_idx

   // Tier5 Magical Items
   gid = wowmod_create_gear( gid, "Mana Infused Necklace", SLOT_NECK, CB_ALL, TIER5, 1500,
      0, 0, 0, 0, 0, 0, 750, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Spirit Infused Necklace", SLOT_NECK, CB_ALL, TIER5, 1500,
      0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Boots of Speed I", SLOT_FEET, CB_ALL, TIER5, 2000,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SPA_SPEED1 )

   // Druid Tier6 (Epic2) Set
   gid = wowmod_create_gear( gid, "Stormrage Cover", SLOT_HEAD, CB_DRUID, TIER6, 2000,
      2, 2, 4, 6, 7, 3, 20, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Stormrage Chestguard", SLOT_CHEST, CB_DRUID, TIER6, 3000,
      3, 3, 5, 8, 8, 7, 40, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Stormrage Legguards", SLOT_LEGS, CB_DRUID, TIER6, 2550,
      3, 3, 5, 8, 8, 7, 40, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Stormrage Handguards", SLOT_HANDS, CB_DRUID, TIER6, 2050,
      2, 2, 4, 6, 6, 3, 20, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Stormrage Boots", SLOT_FEET, CB_DRUID, TIER6, 2025,
      2, 2, 2, 5, 5, 3, 25, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Stormrage Bracers", SLOT_WRISTS, CB_DRUID, TIER6, 2500,
      3, 3, 3, 5, 5, 4, 30, 0, 0, 0, 0, 0, 0, 4, 0, 2, 2, 2, 0 )

   // Hunter Tier6 (Epic2) Set
   gid = wowmod_create_gear( gid, "Dragonstalker's Helm", SLOT_HEAD, CB_HUNTER, TIER6, 2000,
      3, 6, 5, 6, 7, 7, 20, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dragonstalker's Breastplate", SLOT_CHEST, CB_HUNTER, TIER6, 3000,
      4, 8, 5, 7, 8, 8, 30, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dragonstalker's Legguards", SLOT_LEGS, CB_HUNTER, TIER6, 2550,
      4, 8, 5, 6, 8, 8, 30, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dragonstalker's Gauntlets", SLOT_HANDS, CB_HUNTER, TIER6, 2050,
      3, 4, 4, 5, 4, 5, 20, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dragonstalker's Greaves", SLOT_FEET, CB_HUNTER, TIER6, 2025,
      2, 5, 3, 4, 4, 5, 20, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dragonstalker's Bracers", SLOT_WRISTS, CB_HUNTER, TIER6, 2500,
      2, 4, 3, 4, 4, 4, 20, 0, 3, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0 )

   // Mage Tier6 (Epic2) Set
   gid = wowmod_create_gear( gid, "Netherwind Crown", SLOT_HEAD, CB_MAGE, TIER6, 2000,
      2, 2, 4, 8, 8, 3, 25, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Netherwind Robes", SLOT_CHEST, CB_MAGE, TIER6, 3000,
      3, 3, 5, 8, 8, 4, 45, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Netherwind Pants", SLOT_LEGS, CB_MAGE, TIER6, 2550,
      3, 3, 5, 8, 8, 4, 40, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Netherwind Gloves", SLOT_HANDS, CB_MAGE, TIER6, 2050,
      2, 2, 4, 7, 5, 3, 30, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Netherwind Boots", SLOT_FEET, CB_MAGE, TIER6, 2025,
      2, 2, 2, 6, 6, 3, 20, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Netherwind Bindings", SLOT_WRISTS, CB_MAGE, TIER6, 2500,
      3, 2, 2, 5, 5, 3, 20, 0, 0, 4, 4, 4, 0, 0, 0, 1, 3, 2, 0 )

   // Rogue Tier6 (Epic2) Set
   gid = wowmod_create_gear( gid, "Bloodfang Hood", SLOT_HEAD, CB_ROGUE, TIER6, 2000,
      4, 8, 5, 0, 10, 4, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bloodfang Chestpiece", SLOT_CHEST, CB_ROGUE, TIER6, 3000,
      4, 9, 5, 0, 10, 5, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bloodfang Pants", SLOT_LEGS, CB_ROGUE, TIER6, 2550,
      4, 9, 5, 0, 10, 5, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bloodfang Gloves", SLOT_HANDS, CB_ROGUE, TIER6, 2050,
      3, 6, 4, 0, 5, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bloodfang Boots", SLOT_FEET, CB_ROGUE, TIER6, 2025,
      3, 5, 3, 0, 5, 4, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bloodfang Bracers", SLOT_WRISTS, CB_ROGUE, TIER6, 2500,
      3, 4, 3, 0, 5, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 0 )

   // Warlock Tier6 (Epic2) Set
   gid = wowmod_create_gear( gid, "Nemesis Skullcap", SLOT_HEAD, CB_WARLOCK, TIER6, 2000,
      3, 2, 4, 6, 5, 4, 25, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nemesis Robes", SLOT_CHEST, CB_WARLOCK, TIER6, 3000,
      3, 3, 4, 7, 6, 5, 30, 0, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nemesis Leggings", SLOT_LEGS, CB_WARLOCK, TIER6, 2500,
      3, 3, 4, 7, 6, 5, 35, 0, 0, 0, 7, 0, 6, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nemesis Gloves", SLOT_HANDS, CB_WARLOCK, TIER6, 2050,
      2, 2, 4, 5, 5, 4, 25, 0, 0, 0, 6, 0, 6, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nemesis Boots", SLOT_FEET, CB_WARLOCK, TIER6, 2025,
      2, 2, 2, 5, 5, 4, 20, 0, 0, 0, 6, 0, 6, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nemesis Bracers", SLOT_WRISTS, CB_WARLOCK, TIER6, 2500,
      3, 2, 2, 4, 4, 3, 20, 0, 0, 0, 5, 0, 6, 0, 0, 2, 2, 1, 0 )

   // Priest Tier6 (Epic2) Set
   gid = wowmod_create_gear( gid, "Halo of Transcendence", SLOT_HEAD, CB_PRIEST, TIER6, 2000,
      3, 2, 5, 8, 6, 3, 30, 0, 0, 0, 0, 0, 6, 0, 6, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Robes of Transcendence", SLOT_CHEST, CB_PRIEST, TIER6, 3000,
      3, 3, 5, 8, 8, 4, 45, 0, 0, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Leggings of Transcendence", SLOT_LEGS, CB_PRIEST, TIER6, 2550,
      3, 3, 5, 8, 8, 4, 35, 0, 0, 0, 0, 0, 7, 0, 7, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Handguards of Transcendence", SLOT_HANDS, CB_PRIEST, TIER6, 2050,
      2, 2, 5, 5, 6, 3, 25, 0, 0, 0, 0, 0, 6, 0, 6, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Boots of Transcendence", SLOT_FEET, CB_PRIEST, TIER6, 2025,
      2, 2, 3, 5, 6, 3, 20, 0, 0, 0, 0, 0, 6, 0, 6, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bindings of Transcendence", SLOT_WRISTS, CB_PRIEST, TIER6, 2500,
      2, 2, 3, 4, 5, 2, 20, 0, 0, 0, 0, 0, 6, 0, 6, 1, 2, 2, 0 )

   // Tier6 Magical Items
   gid = wowmod_create_gear( gid, "Mana Charged Necklace", SLOT_NECK, CB_ALL, TIER6, 3000,
      0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Spirit Charged Necklace", SLOT_NECK, CB_ALL, TIER6, 3000,
      0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Hearthstone Ring", SLOT_RING1, CB_ALL, TIER6, 25000,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SPA_HSTONE )

   // Druid Tier7 (Epic3) Set
   gid = wowmod_create_gear( gid, "Dreamwalker Headpiece", SLOT_HEAD, CB_DRUID, TIER7, 4000,
      4, 3, 5, 8, 9, 5, 30, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dreamwalker Tunic", SLOT_CHEST, CB_DRUID, TIER7, 6000,
      5, 4, 7, 10, 10, 9, 50, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dreamwalker Legguards", SLOT_LEGS, CB_DRUID, TIER7, 5000,
      5, 4, 7, 10, 10, 9, 50, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dreamwalker Handguards", SLOT_HANDS, CB_DRUID, TIER7, 4000,
      4, 3, 5, 8, 8, 5, 25, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dreamwalker Boots", SLOT_FEET, CB_DRUID, TIER7, 4000,
      4, 3, 4, 7, 7, 5, 30, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Dreamwalker Spaulders", SLOT_WRISTS, CB_DRUID, TIER7, 5000,
      5, 4, 4, 7, 7, 6, 40, 0, 0, 0, 0, 0, 0, 6, 0, 2, 3, 3, 0 )

   // Hunter Tier7 (Epic3) Set
   gid = wowmod_create_gear( gid, "Cryptstalker Headpiece", SLOT_HEAD, CB_HUNTER, TIER7, 4000,
      5, 8, 7, 7, 8, 9, 30, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cryptstalker Tunic", SLOT_CHEST, CB_HUNTER, TIER7, 6000,
      6, 10, 8, 8, 9, 10, 40, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cryptstalker Legguards", SLOT_LEGS, CB_HUNTER, TIER7, 5000,
      6, 10, 8, 8, 9, 10, 40, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cryptstalker Handguards", SLOT_HANDS, CB_HUNTER, TIER7, 4000,
      5, 7, 6, 6, 5, 7, 30, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cryptstalker Boots", SLOT_FEET, CB_HUNTER, TIER7, 4000,
      4, 7, 5, 5, 5, 7, 25, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Cryptstalker Spaulders", SLOT_WRISTS, CB_HUNTER, TIER7, 5000,
      4, 6, 5, 5, 5, 6, 25, 0, 5, 0, 0, 0, 0, 0, 0, 3, 1, 3, 0 )

   // Mage Tier7 (Epic3) Set
   gid = wowmod_create_gear( gid, "Frostfire Circlet", SLOT_HEAD, CB_MAGE, TIER7, 4000,
      3, 3, 5, 10, 10, 4, 35, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Frostfire Robe", SLOT_CHEST, CB_MAGE, TIER7, 6000,
      4, 4, 6, 11, 10, 5, 55, 0, 0, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Frostfire Leggings", SLOT_LEGS, CB_MAGE, TIER7, 5000,
      4, 4, 6, 11, 10, 5, 50, 0, 0, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Frostfire Gloves", SLOT_HANDS, CB_MAGE, TIER7, 4000,
      3, 3, 5, 9, 8, 4, 40, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Frostfire Sandals", SLOT_FEET, CB_MAGE, TIER7, 4000,
      3, 3, 4, 8, 8, 4, 30, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Frostfire Bindings", SLOT_WRISTS, CB_MAGE, TIER7, 5000,
      3, 3, 4, 7, 7, 4, 30, 0, 0, 6, 6, 6, 0, 0, 0, 1, 4, 3, 0 )

   // Rogue Tier7 (Epic3) Set
   gid = wowmod_create_gear( gid, "Bonescythe Helmet", SLOT_HEAD, CB_ROGUE, TIER7, 4000,
      6, 11, 7, 0, 12, 5, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bonescythe Breastplate", SLOT_CHEST, CB_ROGUE, TIER7, 6000,
      6, 12, 7, 0, 13, 7, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bonescythe Legplates", SLOT_LEGS, CB_ROGUE, TIER7, 5000,
      6, 12, 7, 0, 13, 7, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bonescythe Gauntlets", SLOT_HANDS, CB_ROGUE, TIER7, 4000,
      5, 8, 6, 0, 7, 5, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bonescythe Sabatons", SLOT_FEET, CB_ROGUE, TIER7, 4000,
      5, 7, 5, 0, 7, 5, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bonescythe Pauldrons", SLOT_WRISTS, CB_ROGUE, TIER7, 5000,
      4, 6, 5, 0, 7, 4, 0, 6, 0, 0, 0, 0, 0, 0, 0, 4, 2, 2, 0 )

   // Warlock Tier7 (Epic3) Set
   gid = wowmod_create_gear( gid, "Plagueheart Circlet", SLOT_HEAD, CB_WARLOCK, TIER7, 4000,
      4, 3, 5, 8, 8, 5, 30, 0, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Plagueheart Robe", SLOT_CHEST, CB_WARLOCK, TIER7, 6000,
      4, 4, 5, 10, 9, 6, 45, 0, 0, 0, 9, 0, 9, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Plagueheart Leggings", SLOT_LEGS, CB_WARLOCK, TIER7, 5000,
      4, 4, 5, 9, 9, 6, 45, 0, 0, 0, 9, 0, 9, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Plagueheart Gloves", SLOT_HANDS, CB_WARLOCK, TIER7, 4000,
      3, 3, 5, 7, 7, 5, 35, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Plagueheart Sandals", SLOT_FEET, CB_WARLOCK, TIER7, 4000,
      3, 3, 3, 7, 7, 5, 30, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Plagueheart Bindings", SLOT_WRISTS, CB_WARLOCK, TIER7, 5000,
      3, 3, 3, 7, 6, 4, 30, 0, 0, 0, 7, 0, 7, 0, 0, 2, 4, 2, 0 )

   // Priest Tier7 (Epic3) Set
   gid = wowmod_create_gear( gid, "Circlet of Faith", SLOT_HEAD, CB_PRIEST, TIER7, 4000,
      4, 3, 6, 10, 8, 4, 35, 0, 0, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Robe of Faith", SLOT_CHEST, CB_PRIEST, TIER7, 6000,
      4, 4, 6, 10, 10, 5, 55, 0, 0, 0, 0, 0, 10, 0, 10, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Leggings of Faith", SLOT_LEGS, CB_PRIEST, TIER7, 5000,
      4, 4, 6, 10, 10, 5, 50, 0, 0, 0, 0, 0, 9, 0, 9, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gloves of Faith", SLOT_HANDS, CB_PRIEST, TIER7, 4000,
      3, 3, 6, 7, 8, 4, 35, 0, 0, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Sandals of Faith", SLOT_FEET, CB_PRIEST, TIER7, 4000,
      3, 3, 4, 7, 8, 4, 30, 0, 0, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bindings of Faith", SLOT_WRISTS, CB_PRIEST, TIER7, 5000,
      3, 3, 4, 6, 7, 3, 30, 0, 0, 0, 0, 0, 8, 0, 8, 1, 4, 3, 0 )

   // Tier7 Magical Items
   gid = wowmod_create_gear( gid, "Boots of Speed II", SLOT_FEET, CB_ALL, TIER7, 10000,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SPA_SPEED1 )
   gid = wowmod_create_gear( gid, "Necklace of Electricity", SLOT_NECK, CB_ALL, TIER7, 25000,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SPA_LIGHTNING1 )

   // Druid Tier8 (Epic4) Set
   gid = wowmod_create_gear( gid, "Antlers of Malorne", SLOT_HEAD, CB_DRUID, TIER8, 8000,
      5, 4, 6, 10, 12, 6, 40, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Chestpiece of Malorne", SLOT_CHEST, CB_DRUID, TIER8, 12000,
      6, 5, 7, 13, 13, 10, 60, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Britches of Malorne", SLOT_LEGS, CB_DRUID, TIER8, 10000,
      6, 5, 7, 13, 12, 10, 60, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gloves of Malorne", SLOT_HANDS, CB_DRUID, TIER8, 8000,
      5, 4, 6, 10, 10, 6, 35, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Boots of Malorne", SLOT_FEET, CB_DRUID, TIER8, 8000,
      5, 4, 6, 10, 10, 6, 35, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Pauldrons of Malorne", SLOT_WRISTS, CB_DRUID, TIER8, 10000,
      5, 4, 5, 10, 9, 6, 50, 0, 0, 0, 0, 0, 0, 8, 0, 3, 4, 3, 0 )

   // Hunter Tier8 (Epic4) Set
   gid = wowmod_create_gear( gid, "Demon Stalker Greathelm", SLOT_HEAD, CB_HUNTER, TIER8, 8000,
      7, 12, 9, 8, 9, 11, 50, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Demon Stalker Harness", SLOT_CHEST, CB_HUNTER, TIER8, 12000,
      8, 13, 10, 10, 10, 12, 60, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Demon Stalker Greaves", SLOT_LEGS, CB_HUNTER, TIER8, 10000,
      8, 13, 10, 10, 10, 12, 60, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Demon Stalker Gauntlets", SLOT_HANDS, CB_HUNTER, TIER8, 8000,
      7, 9, 7, 8, 7, 8, 40, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Demon Stalker Boots", SLOT_FEET, CB_HUNTER, TIER8, 8000,
      6, 9, 7, 7, 7, 8, 30, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Demon Stalker Bindings", SLOT_WRISTS, CB_HUNTER, TIER8, 10000,
      6, 9, 7, 7, 7, 8, 30, 0, 8, 0, 0, 0, 0, 0, 0, 4, 2, 3, 0 )

   // Mage Tier8 (Epic4) Set
   gid = wowmod_create_gear( gid, "Collar of the Aldor", SLOT_HEAD, CB_MAGE, TIER8, 8000,
      4, 4, 6, 13, 12, 5, 55, 0, 0, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Vestments of the Aldor", SLOT_CHEST, CB_MAGE, TIER8, 12000,
      5, 5, 7, 14, 13, 6, 65, 0, 0, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Legwraps of the Aldor", SLOT_LEGS, CB_MAGE, TIER8, 10000,
      5, 5, 7, 12, 12, 6, 60, 0, 0, 11, 11, 11, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gloves of the Aldor", SLOT_HANDS, CB_MAGE, TIER8, 8000,
      4, 3, 6, 11, 10, 5, 50, 0, 0, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Sandals of the Aldor", SLOT_FEET, CB_MAGE, TIER8, 8000,
      4, 3, 6, 10, 10, 5, 35, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bindings of the Aldor", SLOT_WRISTS, CB_MAGE, TIER8, 10000,
      4, 4, 5, 9, 10, 5, 35, 0, 0, 8, 8, 8, 0, 0, 0, 2, 4, 4, 0 )

   // Rogue Tier8 (Epic4) Set
   gid = wowmod_create_gear( gid, "Netherblade Facemask", SLOT_HEAD, CB_ROGUE, TIER8, 8000,
      8, 13, 8, 0, 14, 6, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Netherblade Chestpiece", SLOT_CHEST, CB_ROGUE, TIER8, 12000,
      8, 14, 8, 0, 15, 8, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Netherblade Breeches", SLOT_LEGS, CB_ROGUE, TIER8, 10000,
      8, 14, 8, 0, 15, 8, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Netherblade Gloves", SLOT_HANDS, CB_ROGUE, TIER8, 8000,
      6, 10, 7, 0, 10, 6, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Netherblade Boots", SLOT_FEET, CB_ROGUE, TIER8, 8000,
      6, 9, 6, 0, 9, 6, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Netherblade Bracers", SLOT_WRISTS, CB_ROGUE, TIER8, 10000,
      5, 8, 6, 0, 9, 6, 0, 8, 0, 0, 0, 0, 0, 0, 0, 4, 2, 4, 0 )

   // Warlock Tier8 (Epic4) Set
   gid = wowmod_create_gear( gid, "Voidheart Crown", SLOT_HEAD, CB_WARLOCK, TIER8, 8000,
      5, 5, 7, 11, 11, 6, 45, 0, 0, 0, 9, 0, 9, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Voidheart Robe", SLOT_CHEST, CB_WARLOCK, TIER8, 12000,
      6, 6, 7, 14, 12, 7, 60, 0, 0, 0, 12, 0, 12, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Voidheart Leggings", SLOT_LEGS, CB_WARLOCK, TIER8, 10000,
      6, 6, 6, 11, 11, 7, 50, 0, 0, 0, 11, 0, 11, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Voidheart Gloves", SLOT_HANDS, CB_WARLOCK, TIER8, 8000,
      4, 4, 6, 9, 9, 6, 45, 0, 0, 0, 10, 0, 10, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Voidheart Shoes", SLOT_FEET, CB_WARLOCK, TIER8, 8000,
      4, 4, 4, 9, 9, 6, 40, 0, 0, 0, 10, 0, 10, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Voidheart Bracers", SLOT_WRISTS, CB_WARLOCK, TIER8, 10000,
      4, 4, 5, 10, 8, 5, 45, 0, 0, 0, 9, 0, 9, 0, 0, 3, 4, 3, 0 )

   // Priest Tier8 (Epic4) Set
   gid = wowmod_create_gear( gid, "Collar of the Incarnate", SLOT_HEAD, CB_PRIEST, TIER8, 8000,
      5, 4, 7, 13, 11, 5, 45, 0, 0, 0, 0, 0, 10, 0, 10, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Shroud of the Incarnate", SLOT_CHEST, CB_PRIEST, TIER8, 12000,
      6, 6, 7, 14, 13, 6, 65, 0, 0, 0, 0, 0, 12, 0, 12, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Leggings of the Incarnate", SLOT_LEGS, CB_PRIEST, TIER8, 10000,
      5, 5, 7, 13, 12, 6, 60, 0, 0, 0, 0, 0, 11, 0, 11, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Handwraps of the Incarnate", SLOT_HANDS, CB_PRIEST, TIER8, 8000,
      4, 4, 7, 10, 10, 5, 40, 0, 0, 0, 0, 0, 10, 0, 10, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Sandals of the Incarnate", SLOT_FEET, CB_PRIEST, TIER8, 8000,
      3, 4, 5, 10, 10, 5, 40, 0, 0, 0, 0, 0, 10, 0, 10, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bindings of the Incarnate", SLOT_WRISTS, CB_PRIEST, TIER8, 10000,
      4, 3, 5, 9, 9, 4, 35, 0, 0, 0, 0, 0, 10, 0, 10, 2, 4, 4, 0 )

   // Tier8 Magical Items
   gid = wowmod_create_gear( gid, "Silver Mana Necklace", SLOT_NECK, CB_ALL, TIER8, 12500,
      0, 0, 0, 0, 0, 0, 2000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Silver Spirit Necklace", SLOT_NECK, CB_ALL, TIER8, 12500,
      0, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )

   // Druid Tier9 (Epic5) Set
   gid = wowmod_create_gear( gid, "Nordrassil Headpiece", SLOT_HEAD, CB_DRUID, TIER9, 16000,
      6, 5, 7, 12, 13, 7, 50, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nordrassil Chestpiece", SLOT_CHEST, CB_DRUID, TIER9, 24000,
      7, 6, 8, 15, 15, 11, 75, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nordrassil Wrath-Kilt", SLOT_LEGS, CB_DRUID, TIER9, 20000,
      7, 6, 7, 15, 14, 11, 70, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nordrassil Gauntlets", SLOT_HANDS, CB_DRUID, TIER9, 16000,
      5, 5, 6, 11, 12, 7, 50, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nordrassil Boots", SLOT_FEET, CB_DRUID, TIER9, 16000,
      6, 4, 7, 11, 12, 7, 50, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Nordrassil Bindings", SLOT_WRISTS, CB_DRUID, TIER9, 20000,
      5, 5, 6, 11, 10, 7, 60, 0, 0, 0, 0, 0, 0, 10, 0, 4, 4, 4, 0 )

   // Hunter Tier9 (Epic5) Set
   gid = wowmod_create_gear( gid, "Rift Stalker Helm", SLOT_HEAD, CB_HUNTER, TIER9, 16000,
      8, 14, 10, 9, 11, 12, 65, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Rift Stalker Hauberk", SLOT_CHEST, CB_HUNTER, TIER9, 24000,
      10, 15, 11, 11, 12, 13, 70, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Rift Stalker Leggings", SLOT_LEGS, CB_HUNTER, TIER9, 20000,
      9, 14, 11, 11, 12, 13, 70, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Rift Stalker Gauntlets", SLOT_HANDS, CB_HUNTER, TIER9, 16000,
      8, 11, 8, 10, 9, 9, 55, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Rift Stalker Boots", SLOT_FEET, CB_HUNTER, TIER9, 16000,
      6, 11, 8, 9, 9, 9, 45, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Rift Stalker Bracers", SLOT_WRISTS, CB_HUNTER, TIER9, 20000,
      7, 11, 7, 8, 9, 9, 40, 0, 10, 0, 0, 0, 0, 0, 0, 5, 3, 3, 0 )

   // Mage Tier9 (Epic5) Set
   gid = wowmod_create_gear( gid, "Cowl of Tirisfal", SLOT_HEAD, CB_MAGE, TIER9, 16000,
      5, 4, 7, 16, 15, 6, 70, 0, 0, 11, 11, 11, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Robes of Tirisfal", SLOT_CHEST, CB_MAGE, TIER9, 24000,
      6, 6, 8, 17, 16, 7, 80, 0, 0, 14, 14, 14, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Leggings of Tirisfal", SLOT_LEGS, CB_MAGE, TIER9, 20000,
      6, 6, 7, 14, 13, 7, 70, 0, 0, 13, 13, 13, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gloves of Tirisfal", SLOT_HANDS, CB_MAGE, TIER9, 16000,
      5, 4, 7, 13, 12, 6, 65, 0, 0, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Shoes of Tirisfal", SLOT_FEET, CB_MAGE, TIER9, 16000,
      5, 4, 6, 11, 11, 6, 45, 0, 0, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Wristbands of Tirisfal", SLOT_WRISTS, CB_MAGE, TIER9, 20000,
      5, 5, 6, 11, 12, 6, 50, 0, 0, 10, 10, 10, 0, 0, 0, 3, 4, 5, 0 )

   // Rogue Tier9 (Epic5) Set
   gid = wowmod_create_gear( gid, "Deathmantle Helm", SLOT_HEAD, CB_ROGUE, TIER9, 16000,
      9, 15, 9, 0, 15, 7, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Deathmantle Chestguard", SLOT_CHEST, CB_ROGUE, TIER9, 24000,
      10, 16, 9, 0, 17, 9, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Deathmantle Legguards", SLOT_LEGS, CB_ROGUE, TIER9, 20000,
      9, 16, 9, 0, 16, 9, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Deathmantle Handguards", SLOT_HANDS, CB_ROGUE, TIER9, 16000,
      7, 12, 8, 0, 11, 7, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Deathmantle Boots", SLOT_FEET, CB_ROGUE, TIER9, 16000,
      6, 11, 7, 0, 12, 7, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Deathmantle Bracers", SLOT_WRISTS, CB_ROGUE, TIER9, 20000,
      6, 10, 7, 0, 12, 7, 0, 10, 0, 0, 0, 0, 0, 0, 0, 5, 3, 4, 0 )

   // Warlock Tier9 (Epic5) Set
   gid = wowmod_create_gear( gid, "Hood of the Corruptor", SLOT_HEAD, CB_WARLOCK, TIER9, 16000,
      6, 6, 8, 13, 13, 7, 55, 0, 0, 0, 11, 0, 11, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Robe of the Corruptor", SLOT_CHEST, CB_WARLOCK, TIER9, 24000,
      7, 7, 8, 16, 15, 8, 75, 0, 0, 0, 14, 0, 14, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Leggings of the Corruptor", SLOT_LEGS, CB_WARLOCK, TIER9, 20000,
      6, 7, 7, 13, 13, 8, 65, 0, 0, 0, 13, 0, 13, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gloves of the Corruptor", SLOT_HANDS, CB_WARLOCK, TIER9, 16000,
      5, 4, 7, 11, 11, 7, 60, 0, 0, 0, 12, 0, 12, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Sandals of the Corruptor", SLOT_FEET, CB_WARLOCK, TIER9, 16000,
      4, 5, 5, 11, 11, 7, 50, 0, 0, 0, 12, 0, 12, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bracelets of the Corruptor", SLOT_WRISTS, CB_WARLOCK, TIER9, 20000,
      5, 5, 6, 12, 10, 6, 55, 0, 0, 0, 11, 0, 11, 0, 0, 4, 4, 4, 0 )

   // Priest Tier9 (Epic5) Set
   gid = wowmod_create_gear( gid, "Hood of the Avatar", SLOT_HEAD, CB_PRIEST, TIER9, 16000,
      6, 5, 8, 15, 13, 6, 65, 0, 0, 0, 0, 0, 12, 0, 12, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Shroud of the Avatar", SLOT_CHEST, CB_PRIEST, TIER9, 24000,
      7, 7, 9, 17, 16, 7, 80, 0, 0, 0, 0, 0, 14, 0, 14, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Leggings of the Avatar", SLOT_LEGS, CB_PRIEST, TIER9, 20000,
      5, 6, 8, 15, 14, 7, 70, 0, 0, 0, 0, 0, 13, 0, 13, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Handguards of the Avatar", SLOT_HANDS, CB_PRIEST, TIER9, 16000,
      5, 5, 7, 12, 12, 6, 50, 0, 0, 0, 0, 0, 12, 0, 12, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Slippers of the Avatar", SLOT_FEET, CB_PRIEST, TIER9, 16000,
      4, 5, 5, 11, 12, 6, 55, 0, 0, 0, 0, 0, 12, 0, 12, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bracelets of the Avatar", SLOT_WRISTS, CB_PRIEST, TIER9, 20000,
      5, 4, 6, 11, 11, 5, 50, 0, 0, 0, 0, 0, 12, 0, 12, 3, 5, 4, 0 )

   // Tier9 Magical Items
   gid = wowmod_create_gear( gid, "Gold Mana Necklace", SLOT_NECK, CB_ALL, TIER9, 25000,
      0, 0, 0, 0, 0, 0, 3000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gold Spirit Necklace", SLOT_NECK, CB_ALL, TIER9, 25000,
      0, 0, 0, 0, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Boots of Speed III", SLOT_FEET, CB_ALL, TIER9, 50000,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SPA_SPEED3 )

   // Druid Tier10 (Epic6) Set
   gid = wowmod_create_gear( gid, "Thunderheart Headguard", SLOT_HEAD, CB_DRUID, TIER10, 32000,
      7, 6, 8, 15, 15, 8, 70, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Thunderheart Vest", SLOT_CHEST, CB_DRUID, TIER10, 50000,
      9, 8, 9, 18, 18, 12, 90, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Thunderheart Pants", SLOT_LEGS, CB_DRUID, TIER10, 40000,
      8, 7, 8, 17, 17, 12, 80, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Thunderheart Handguards", SLOT_HANDS, CB_DRUID, TIER10, 36000,
      6, 6, 7, 13, 13, 8, 70, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Thunderheart Boots", SLOT_FEET, CB_DRUID, TIER10, 36000,
      7, 5, 8, 13, 14, 8, 65, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Thunderheart Bracers", SLOT_WRISTS, CB_DRUID, TIER10, 40000,
      6, 6, 7, 13, 12, 8, 70, 0, 0, 0, 0, 0, 0, 12, 0, 5, 5, 4, 0 )

   // Hunter Tier10 (Epic6) Set
   gid = wowmod_create_gear( gid, "Gronnstalker's Helmet", SLOT_HEAD, CB_HUNTER, TIER10, 32000,
      10, 16, 11, 11, 13, 13, 80, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gronnstalker Chestguard", SLOT_CHEST, CB_HUNTER, TIER10, 50000,
      12, 18, 12, 13, 14, 14, 90, 0, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gronnstalker Leggings", SLOT_LEGS, CB_HUNTER, TIER10, 40000,
      10, 16, 12, 13, 14, 14, 85, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gronnstalker Gloves", SLOT_HANDS, CB_HUNTER, TIER10, 36000,
      9, 13, 10, 12, 11, 10, 65, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gronnstalker Sabatons", SLOT_FEET, CB_HUNTER, TIER10, 36000,
      7, 13, 9, 11, 11, 10, 60, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gronnstalker Bindings", SLOT_WRISTS, CB_HUNTER, TIER10, 40000,
      8, 13, 8, 10, 11, 10, 55, 0, 12, 0, 0, 0, 0, 0, 0, 5, 4, 4, 0 )

   // Mage Tier10 (Epic6) Set
   gid = wowmod_create_gear( gid, "Cowl of the Tempest", SLOT_HEAD, CB_MAGE, TIER10, 36000,
      6, 5, 8, 18, 17, 7, 85, 0, 0, 13, 13, 13, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Robes of the Tempest", SLOT_CHEST, CB_MAGE, TIER10, 50000,
      8, 7, 9, 19, 18, 8, 100, 0, 0, 16, 16, 16, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Leggings of the Tempest", SLOT_LEGS, CB_MAGE, TIER10, 40000,
      7, 7, 8, 16, 15, 8, 85, 0, 0, 15, 15, 15, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gloves of the Tempest", SLOT_HANDS, CB_MAGE, TIER10, 36000,
      6, 5, 8, 15, 14, 7, 80, 0, 0, 14, 14, 14, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Boots of the Tempest", SLOT_FEET, CB_MAGE, TIER10, 36000,
      6, 5, 7, 13, 14, 7, 60, 0, 0, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bracers of the Tempest", SLOT_WRISTS, CB_MAGE, TIER10, 40000,
      6, 6, 7, 13, 13, 7, 60, 0, 0, 12, 12, 12, 0, 0, 0, 4, 5, 5, 0 )

   // Rogue Tier10 (Epic6) Set
   gid = wowmod_create_gear( gid, "Slayer's Helm", SLOT_HEAD, CB_ROGUE, TIER10, 16000,
      10, 17, 10, 0, 17, 8, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Slayer's Chestguard", SLOT_CHEST, CB_ROGUE, TIER10, 50000,
      12, 19, 10, 0, 20, 10, 0, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Slayer's Legguards", SLOT_LEGS, CB_ROGUE, TIER10, 40000,
      11, 18, 10, 0, 18, 10, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Slayer's Handguards", SLOT_HANDS, CB_ROGUE, TIER10, 36000,
      8, 14, 9, 0, 13, 8, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Slayer's Boots", SLOT_FEET, CB_ROGUE, TIER10, 36000,
      7, 13, 8, 0, 14, 8, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Slayer's Bracers", SLOT_WRISTS, CB_ROGUE, TIER10, 40000,
      6, 12, 8, 0, 14, 8, 0, 12, 0, 0, 0, 0, 0, 0, 0, 6, 3, 5, 0 )

   // Warlock Tier10 (Epic6) Set
   gid = wowmod_create_gear( gid, "Hood of the Malefic", SLOT_HEAD, CB_WARLOCK, TIER10, 36000,
      7, 7, 9, 15, 15, 8, 70, 0, 0, 0, 13, 0, 13, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Robe of the Malefic", SLOT_CHEST, CB_WARLOCK, TIER10, 50000,
      8, 8, 10, 19, 17, 9, 90, 0, 0, 0, 15, 0, 15, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Leggings of the Malefic", SLOT_LEGS, CB_WARLOCK, TIER10, 40000,
      7, 8, 9, 15, 15, 9, 80, 0, 0, 0, 15, 0, 15, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Gloves of the Malefic", SLOT_HANDS, CB_WARLOCK, TIER10, 36000,
      6, 5, 8, 13, 13, 8, 75, 0, 0, 0, 14, 0, 14, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Boots of the Malefic", SLOT_FEET, CB_WARLOCK, TIER10, 36000,
      5, 6, 6, 13, 12, 8, 60, 0, 0, 0, 14, 0, 14, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bracers of the Malefic", SLOT_WRISTS, CB_WARLOCK, TIER10, 40000,
      6, 6, 7, 14, 12, 7, 60, 0, 0, 0, 13, 0, 13, 0, 0, 4, 5, 5, 0 )

   // Priest Tier10 (Epic6) Set
   gid = wowmod_create_gear( gid, "Hood of Absolution", SLOT_HEAD, CB_PRIEST, TIER10, 36000,
      7, 6, 9, 17, 15, 7, 80, 0, 0, 0, 0, 0, 14, 0, 14, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Shroud of Absolution", SLOT_CHEST, CB_PRIEST, TIER10, 50000,
      9, 8, 11, 20, 18, 8, 1000, 0, 0, 0, 0, 0, 16, 0, 16, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Leggings of Absolution", SLOT_LEGS, CB_PRIEST, TIER10, 40000,
      6, 7, 9, 17, 16, 8, 85, 0, 0, 0, 0, 0, 15, 0, 15, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Handguards of Absolution", SLOT_HANDS, CB_PRIEST, TIER10, 36000,
      6, 6, 8, 14, 14, 7, 70, 0, 0, 0, 0, 0, 14, 0, 14, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Sandals of Absolution", SLOT_FEET, CB_PRIEST, TIER10, 36000,
      5, 6, 6, 13, 14, 7, 65, 0, 0, 0, 0, 0, 14, 0, 14, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Bracelets of Absolution", SLOT_WRISTS, CB_PRIEST, TIER10, 40000,
      6, 6, 7, 13, 13, 6, 65, 0, 0, 0, 0, 0, 14, 0, 14, 4, 5, 5, 0 )

   // Tier10 Magical Items
   gid = wowmod_create_gear( gid, "Platinum Mana Necklace", SLOT_NECK, CB_ALL, TIER10, 50000,
      0, 0, 0, 0, 0, 0, 5000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Platinum Spirit Necklace", SLOT_NECK, CB_ALL, TIER10, 50000,
      0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
   gid = wowmod_create_gear( gid, "Lightning Necklace", SLOT_NECK, CB_ALL, TIER10, 125000,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SPA_LIGHTNING2 )

   // gid, name, slot, classes, tier, cost, 
   // str, agi, stam, int, spir, armor, mana, melee, mark, arc, fire, frst, shad, nat, hol, crit, spcrit, spdmg, spec_idx



   //log_amx( "[DEBUG] Full Gear List: " )
   //new name[32]
   //for ( new j = 1; j < MAX_GEAR; j++ ){
   //   copy( name, 31, gear[j] ) 
   //   log_amx( "[%d] (%s) g_tier=(%d) g_classes=(%d)", j, name, g_tier[j], g_classes[j] )
   //}

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_create_gear
// Create a piece of wowmod gear
// **************************************************************************
public wowmod_create_gear( gid, name[31], slot, classes, tier, cost, str, agi, sta, int, spi,
                  arm, mana, mel, mark, arc, fir, fro, sha, nat, hol, crit, spcrit, spdmg, spec_idx ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   if ( ( gid < 1 ) || ( gid > MAX_GEAR ) ){
      log_amx( "Error: gid outside of range [ 0 .. %d ]", MAX_GEAR )
      return PLUGIN_CONTINUE
   }

   if ( ( slot < SLOT_HEAD ) || ( slot > SLOT_NECK ) ){
      log_amx( "Error: slot outside of range [ %d .. %d ]", SLOT_HEAD, SLOT_NECK )
      return PLUGIN_CONTINUE
   }

   gear[gid] = name
   g_slot[gid] = slot
   g_classes[gid] = classes
   g_tier[gid] = tier
   g_cost[gid] = cost

   g_str[gid] = str 
   g_agi[gid] = agi
   g_stam[gid] = sta
   g_int[gid] = int
   g_spir[gid] = spi
   g_armor[gid] = arm
   g_mana[gid] = mana
   g_melee[gid] = mel
   g_marksman[gid] = mark
   g_arcane[gid] = arc
   g_fire[gid] = fir
   g_frost[gid] = fro
   g_shadow[gid] = sha
   g_nature[gid] = nat
   g_holy[gid] = hol

   g_crit[gid] = crit 
   g_spcrit[gid] = spcrit 
   g_spdmg[gid] = spdmg 

   g_spec_idx[gid] = spec_idx
 
   log_amx( "[%s] CREATING GEAR [%d] ( %s ) classes=[%d]", MOD, gid, gear[gid], classes ) 
   log_amx( "[%s] str=%d agi=%d stam=%d int=%d shad=%d nat=%d holy=%d crit=%d spcrit=(%d) spdmg=(%d) sidx=(%d)", 
            MOD, g_str[gid], g_agi[gid], g_stam[gid], g_int[gid], g_shadow[gid],
            g_nature[gid], g_holy[gid], g_crit[gid], g_spcrit[gid], g_spdmg[gid], g_spec_idx[gid] ) 

   return ( gid + 1 ) 

}


// **************************************************************************
// BEGIN wowmod_gear_nextinvidx subroutine
// Get the next available index in a player's inventory or -1 if no space left 
// **************************************************************************
public wowmod_gear_nextinvidx( id ){
   new j = 0

   for ( j = 10; j < MAX_INV; j++ )
      if ( p_inventory[id][j] <= 0 )
         return j 
   
   return -1
}


// **************************************************************************
// BEGIN wowmod_admin_loaditem subroutine
// Admin command to load an item by its gid
// **************************************************************************
public wowmod_admin_loaditem( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new itemnum[32]
   new sidx =  wowmod_gear_nextinvidx( id )
   new gid = 0

   read_args( itemnum, 63)
   remove_quotes( itemnum )

   if ( !( get_user_flags(id) & get_cvar_num("WOW_admin_flag") ) ){
      client_print( id, print_console, "[%s] Access Denied" )
      return PLUGIN_HANDLED
   }

   gid = str_to_num( itemnum )

   if ( ( gid < 0 ) || ( gid >= MAX_GEAR ) || ( equali( gear[gid], "" ) ) ){
      client_print( id, print_console, "<WowMOD Admin> You have specified an invalid GID" )
      return PLUGIN_HANDLED
   }

   if ( sidx == -1 ){
      client_print( id, print_console, "<WowMOD Admin> You have no free inventory space" )
      return PLUGIN_HANDLED
   }

   p_inventory[id][sidx] = gid
   client_print( id, print_console, "<WowMOD Admin> Loaded to your inventory: [%d] %s", gid, gear[gid] )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_admin_lookupitem subroutine
// Admin command to lookup item gids by an item name 
// **************************************************************************
public wowmod_admin_lookupitem( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new itemname[64]
   new name[32]
   new pos = 0
   new num = 0

   read_args( itemname, 63)
   remove_quotes( itemname )

   if ( !( get_user_flags(id) & get_cvar_num("WOW_admin_flag") ) ){
      client_print( id, print_console, "[%s] Access Denied" )
      return PLUGIN_HANDLED
   }

   for ( new j = 1; j < MAX_GEAR; j++ ){
      copy( name, 31, gear[j] ) 
      pos = containi( name, itemname )
      //log_amx( "DEBUG-Lookup: %d in=(%s) n=(%s) pos=(%d)", j, itemname, name, pos ) 
      if ( pos >= 0 ){
         num += 1
         if ( num == 1 )
            client_print( id, print_console, "<WowMOD Admin> GID   Item Name" ) 
         client_print( id, print_console, "<WowMOD Admin> [%d]  %s", j, name ) 
      }
      if ( num >= 10 ){
         client_print( id, print_console, "<WowMOD Admin> Max. results reached, please refine your search" ) 
         return PLUGIN_HANDLED
      }
   }

   client_print( id, print_console, "<WowMOD Admin> %d matches found", num )

   return PLUGIN_HANDLED
}


// **************************************************************************
// END WowMOD Gear Module 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

