/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Badges Module
*** Date:          Oct 10 , 2007
*** Last Update:   Oct 24 , 2007
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
// BEGIN WowMOD Badges Module
// **************************************************************************


// **************************************************************************
// BEGIN wowmod_badge_tier
// Chceck badge requirements for specificed tier
// **************************************************************************
public wowmod_badges_tier( id, tier ){
   //client_print( id, print_chat, ">> badge_tier() checking for tier %d", tier )

   if ( tier == 5 ){
      new bit1 = b_bit[BADGE_ALL_TIER5_1]
      new bit2 = b_bit[BADGE_ALL_TIER5_2]
      new bit3 = b_bit[BADGE_ALL_TIER5_3]
   
      //client_print( id, print_chat, ">> badge_tier()-tier5 p_badges=(%d) b1=(%d) b2=(%d) b3=(%d)",
      //   p_badges[id], bit1, bit2, bit3 )

      if ( ( ( p_badges[id]&bit1) && ( (p_badges[id]&bit2) || (p_badges[id]&bit3) ) )||
           ( ( p_badges[id]&bit2) && ( (p_badges[id]&bit1) || (p_badges[id]&bit3) ) )||
           ( ( p_badges[id]&bit3) && ( (p_badges[id]&bit1) || (p_badges[id]&bit2) ) ) )
      {
         //client_print( id, print_chat, ">> badge_tier() Tier5 Badges found!" )
         return true
      } 
   }else if ( tier == 6 ){
      new bit1 = b_bit[BADGE_ALL_TIER6_1]
      new bit2 = b_bit[BADGE_ALL_TIER6_2]
      new bit3 = b_bit[BADGE_ALL_TIER6_3]
      if ( ( ( p_badges[id]&bit1) && ( (p_badges[id]&bit2) || (p_badges[id]&bit3) ) )||
           ( ( p_badges[id]&bit2) && ( (p_badges[id]&bit1) || (p_badges[id]&bit3) ) )||
           ( ( p_badges[id]&bit3) && ( (p_badges[id]&bit1) || (p_badges[id]&bit2) ) ) )
      {
         //client_print( id, print_chat, ">> badge_tier() Tier6 Badges found!" )
         return true
      } 
   }else if ( tier == 7 ){
      new bit1 = b_bit[BADGE_ALL_TIER7_1]
      new bit2 = b_bit[BADGE_ALL_TIER7_2]
      new bit3 = b_bit[BADGE_ALL_TIER7_3]
      if ( ( ( p_badges[id]&bit1) && ( (p_badges[id]&bit2) || (p_badges[id]&bit3) ) )||
           ( ( p_badges[id]&bit2) && ( (p_badges[id]&bit1) || (p_badges[id]&bit3) ) )||
           ( ( p_badges[id]&bit3) && ( (p_badges[id]&bit1) || (p_badges[id]&bit2) ) ) )
      {
         //client_print( id, print_chat, ">> badge_tier() Tier7 Badges found!" )
         return true
      } 
   }else if ( tier == 8 ){
      new bit = -1
      new badges = ( p_class1[id] == CLASS_DRUID ) ? p_badges[id] : p_badges2[id] 
      
      switch( p_class1[id] ){
         case CLASS_DRUID: bit = b_bit[BADGE_DRUID_TIER8]
         case CLASS_HUNTER: bit = b_bit[BADGE_HUNTER_TIER8]
         case CLASS_MAGE: bit = b_bit[BADGE_MAGE_TIER8]
         case CLASS_ROGUE: bit = b_bit[BADGE_ROGUE_TIER8]
         case CLASS_WARLOCK: bit = b_bit[BADGE_WARLOCK_TIER8]
         case CLASS_PRIEST: bit = b_bit[BADGE_PRIEST_TIER8]
      }
      if ( badges & bit ){
         //client_print( id, print_chat, ">> badge_tier() Tier8 Badges found!" )
         return true
      }
   }else if ( tier == 9 ){
      new bit = -1
      new badges = ( p_class1[id] == CLASS_DRUID ) ? p_badges[id] : p_badges2[id] 
      
      switch( p_class1[id] ){
         case CLASS_DRUID: bit = b_bit[BADGE_DRUID_TIER9]
         case CLASS_HUNTER: bit = b_bit[BADGE_HUNTER_TIER9]
         case CLASS_MAGE: bit = b_bit[BADGE_MAGE_TIER9]
         case CLASS_ROGUE: bit = b_bit[BADGE_ROGUE_TIER9]
         case CLASS_WARLOCK: bit = b_bit[BADGE_WARLOCK_TIER9]
         case CLASS_PRIEST: bit = b_bit[BADGE_PRIEST_TIER9]
      }
      if ( badges & bit ){
         //client_print( id, print_chat, ">> badge_tier() Tier9 Badges found!" )
         return true
      }
   }else if ( tier == 10 ){
      new bit = -1
      new badges = ( p_class1[id] == CLASS_DRUID ) ? p_badges[id] : p_badges2[id] 
      
      switch( p_class1[id] ){
         case CLASS_DRUID: bit = b_bit[BADGE_DRUID_TIER10]
         case CLASS_HUNTER: bit = b_bit[BADGE_HUNTER_TIER10]
         case CLASS_MAGE: bit = b_bit[BADGE_MAGE_TIER10]
         case CLASS_ROGUE: bit = b_bit[BADGE_ROGUE_TIER10]
         case CLASS_WARLOCK: bit = b_bit[BADGE_WARLOCK_TIER10]
         case CLASS_PRIEST: bit = b_bit[BADGE_PRIEST_TIER10]
      }
      if ( badges & bit ){
         //client_print( id, print_chat, ">> badge_tier() Tier10 Badges found!" )
         return true
      }
   }
   return false
}


// **************************************************************************
// BEGIN wowmod_badge_init
// Create all known WowMOD badges 
// **************************************************************************
public wowmod_badge_init(){

   log_amx("WowMOD Badges :: Initializing Badge Creation System..." )

   badge[0] = "NA"

   new bid = 1

   // Arguments List:
   // bid, name, desc, classes, xp, gold

   bid = wowmod_create_badge( bid, "Basic Combat", "200 total kills" , CB_ALL, 
         100, 100, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0 ) 
   bid = wowmod_create_badge( bid, "Basic Reinforcement", 
         "50 total kill assists + 5 kill assists in a session", CB_ALL, 
         250, 250, 0, 0, 50, 5, 0, 0, 0, 0, 0, 0 )
   bid = wowmod_create_badge( bid, "Basic Healer", "200 total heals" , 
         CB_DRUID|CB_PRIEST, 250, 250, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0 ) 
   bid = wowmod_create_badge( bid, "Intermediate Combat", 
         "1000 total kills [Partial Unlock Tier5 Gear]" , CB_ALL,
         500, 500, 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0 ) 
   bid = wowmod_create_badge( bid, "Intermediate Reinforcement", 
         "100 total kill assists + 10 kill assists in a session [Partial Unlock Tier5 Gear]", CB_ALL, 
         550, 550, 0, 0, 100, 10, 0, 0, 0, 0, 0, 0 )
   bid = wowmod_create_badge( bid, "Intermediate Healer", 
         "1000 total heals [Partial Unlock Tier5 Gear]" , CB_DRUID|CB_PRIEST, 
         750, 750, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0 )
   bid = wowmod_create_badge( bid, "Advanced Combat", 
         "2500 total kills [Partial Unlock Tier6 Gear]" , CB_ALL, 
         1000, 1000, 2500, 0, 0, 0, 0, 0, 0, 0, 0, 0 ) 
   bid = wowmod_create_badge( bid, "Advanced Reinforcement", 
         "200 total kill assists + 20 kill assists in a session [Partial Unlock Tier6 Gear]", CB_ALL, 
         850, 850, 0, 0, 200, 20, 0, 0, 0, 0, 0, 0 )
   bid = wowmod_create_badge( bid, "Advanced Healer", 
         "2000 total heals [Partial Unlock Tier6 Gear]", CB_DRUID|CB_PRIEST, 
         1500, 1500, 0, 0, 0, 0, 2000, 0, 0, 0, 0, 0 )
   bid = wowmod_create_badge( bid, "Expert Combat", 
         "5000 total kills [Partial Unlock Tier7 Gear]" , CB_ALL, 
         5000, 5000, 5000, 0, 0, 0, 0, 0, 0, 0, 0, 0  ) 
   bid = wowmod_create_badge( bid, "Expert Reinforcement", 
         "400 total kill assists + 25 kill assists in a session [Partial Unlock Tier7 Gear]", CB_ALL, 
         1250, 1250, 0, 0, 400, 25, 0, 0, 0, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Expert Healer", 
         "5000+ total heals [Partial Unlock Tier7 Gear]", CB_DRUID|CB_PRIEST, 
         5000, 5000, 0, 0, 0, 0, 5000, 0, 0, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Combat Medic", "250+ heals in a session" , 
         CB_DRUID|CB_PRIEST, 5000, 5000, 0, 0, 0, 0, 0, 250, 0, 0, 0, 0  ) 
   bid = wowmod_create_badge( bid, "Combat Medic 1st Class", "500+ heals in a session" , 
         CB_DRUID|CB_PRIEST, 10000, 10000, 0, 0, 0, 0, 0, 500, 0, 0, 0, 0  ) 
   bid = wowmod_create_badge( bid, "Cenarion Novice", 
         "1000 total heals + 500 total team-aid spell casts [Unlocks Tier8 Gear]",
         CB_DRUID, 1500, 1500, 0, 0, 0, 0, 1000, 0, 500, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Cenarion Middling", 
         "2500 total heals + 1000 total team-aid spell casts [Unlocks Tier9 Gear]",
         CB_DRUID, 3500, 3500, 0, 0, 0, 0, 2500, 0, 1000, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Cenarion Adept", 
         "5000 total heals + 2000 total team-aid spell casts [Unlocks Tier10 Gear]",
         CB_DRUID, 6500, 6500, 0, 0, 0, 0, 5000, 0, 2000, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Amateur Hunter", 
         "1000 total kills + 500 total team-aid spell casts [Unlocks Tier8 Gear]", 
         CB_HUNTER, 1500, 1500, 1000, 0, 0, 0, 0, 0, 500, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Intermediate Hunter", 
         "2500 total kills + 1000 total team-aid spell casts [Unlocks Tier9 Gear]",
         CB_HUNTER, 3500, 3500, 2500, 0, 0, 0, 0, 0, 1000, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Master of the Hunt", 
         "5000 total kills + 2000 total team-aid spell casts [Unlocks Tier10 Gear]",
         CB_HUNTER, 6500, 6500, 5000, 0, 0, 0, 0, 0, 2000, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Novice Magician", 
         "1000 total kills + 500 total team-aid spell casts [Unlocks Tier8 Gear]",
         CB_MAGE, 1500, 1500, 1000, 0, 0, 0, 0, 0, 500, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Skilled Magician", 
         "2500 total kills + 1000 total team-aid spell casts [Unlocks Tier9 Gear]",
         CB_MAGE, 3500, 3500, 2500, 0, 0, 0, 0, 0, 1000, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Arch Magus", 
         "5000 total kills + 2000 total team-aid spell casts [Unlocks Tier10 Gear]",
         CB_MAGE, 6500, 6500, 5000, 0, 0, 0, 0, 0, 2000, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Cutpurse", 
         "1000 total rogue special attacks + 25 rogue special attacks in a session [Unlocks Tier8 Gear]",
         CB_ROGUE, 1500, 1500, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 25  )
   bid = wowmod_create_badge( bid, "Assassin", 
         "2500 total rogue special attacks + 50 rogue special attacks in a session [Unlocks Tier9 Gear]",
         CB_ROGUE, 3500, 3500, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 50  )
   bid = wowmod_create_badge( bid, "Master Assassin", 
         "5000 total rogue special attacks + 100 rogue special attacks in a session [Unlocks Tier10 Gear]",
         CB_ROGUE, 6500, 6500, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 100  )
   bid = wowmod_create_badge( bid, "Augurer", 
         "1000 total kills + 50 kills in a session [Unlocks Tier8 Gear]",
         CB_WARLOCK, 1500, 1500, 1000, 50, 0, 0, 0, 0, 0, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Necromancer", 
         "2500 total kills + 75 kills in a session [Unlocks Tier9 Gear]",
         CB_WARLOCK, 3500, 3500, 2500, 75, 0, 0, 0, 0, 0, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Keeper of Souls", 
         "5000 total kills + 100 kills in a session [Unlocks Tier10 Gear]",
         CB_WARLOCK, 6500, 6500, 5000, 100, 0, 0, 0, 0, 0, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Vicar", 
         "1000 total heals + 500 team-aid spell casts [Unlocks Tier8 Gear]",
         CB_PRIEST, 1500, 1500, 0, 0, 0, 0, 1000, 0, 500, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Bishop", 
         "2500 total heals + 1000 team-aid spell casts [Unlocks Tier9 Gear]",
         CB_PRIEST, 3500, 3500, 0, 0, 0, 0, 2500, 0, 1000, 0, 0, 0  )
   bid = wowmod_create_badge( bid, "Cardinal", 
         "5000 total heals + 2000 team-aid spell casts [Unlocks Tier10 Gear]",
         CB_PRIEST, 6500, 6500, 0, 0, 0, 0, 5000, 0, 2000, 0, 0, 0  )

   num_badges = bid
}


// **************************************************************************
// BEGIN wowmod_create_badge
// Create a wowmod badge 
// **************************************************************************
public wowmod_create_badge( bid, name[31], desc[127], classes, xp, gold, kills, kills_s, assists, assists_s,
                            heals, heals_s, teamacts, teamacts_s, roguespec, roguespec_s ){

   if ( ( bid < 1 ) || ( bid > MAX_BADGES ) ){
      log_amx( "create_badge() Error: bid outside of range [ 0 .. %d ]", MAX_BADGES )
      return PLUGIN_CONTINUE
   }

   badge[bid] = name
   b_desc[bid] = desc
   b_classes[bid] = classes
   b_xp[bid] = xp
   b_gold[bid] = gold
   b_kills[bid] = kills
   b_kills_s[bid] = kills_s
   b_assists[bid] = assists
   b_assists_s[bid] = assists_s
   b_heals[bid] = heals
   b_heals_s[bid] = heals_s 
   b_teamacts[bid] = teamacts 
   b_teamacts_s[bid] = teamacts_s
   b_roguespec[bid] = roguespec
   b_roguespec_s[bid] = roguespec_s

   if ( !( get_cvar_num("mp_savexp") ) ){
      b_kills[bid] = floatround( float(b_kills[bid]) / BADGE_DIV )
      b_kills_s[bid] = floatround( float(b_kills_s[bid]) / BADGE_DIV )
      b_assists[bid] = floatround( float(b_assists[bid]) / BADGE_DIV )
      b_assists_s[bid] = floatround( float(b_assists_s[bid]) / BADGE_DIV )
      b_heals[bid] = floatround( float(b_heals[bid]) / BADGE_DIV )
      b_heals_s[bid] = floatround( float(b_heals_s[bid]) / BADGE_DIV )
      b_teamacts[bid] = floatround( float(b_teamacts[bid]) / BADGE_DIV )
      b_teamacts_s[bid] = floatround( float(b_teamacts_s[bid]) / BADGE_DIV )
      b_roguespec[bid] = floatround( float(b_roguespec[bid]) / BADGE_DIV )
      b_roguespec_s[bid] = floatround( float(b_roguespec_s[bid]) / BADGE_DIV )
   }

   if ( bid <= 17 )
      b_bit[bid] = (1<<(bid-1)) 
   else
      b_bit[bid] = (1<<(bid-18)) 

   log_amx( "CREATING BADGE [%d] ( %s ) classes=[%d] x/g=(%d/%d)", 
      bid, badge[bid], classes, xp, gold )
   log_amx( "Description: %s bit=(%d)", b_desc[bid], b_bit[bid] )
   log_amx( "tk=(%d) sk=(%d) ta=(%d) sa=(%d) th=(%d) sh=(%d) tt=(%d) st=(%d) tr=(%d) sr=(%d)", 
      b_kills[bid], b_kills_s[bid], b_assists[bid], b_assists_s[bid], b_heals[bid], b_heals_s[bid], 
      b_teamacts[bid], b_teamacts_s[bid], b_roguespec[bid], b_roguespec_s[bid] )

   return ( bid + 1 )
}


// **************************************************************************
// END WowMOD Badges Module 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

