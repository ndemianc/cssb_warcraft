/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Utility Routines
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
// BEGIN WowMOD Utility Routines
// **************************************************************************

// **************************************************************************
// BEGIN fullupdate routine - Kept from UWC3
// Prevent fullupdate client command
// **************************************************************************
public fullupdate(id) {
   new name[32]
   get_user_name(id, name, 31)
   log_amx("FULLUPDATE: %s attempted to use the fullupdate command!", name)

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_kill_tasks subroutine
// Kill all player-based running tasks
// **************************************************************************
public wowmod_kill_tasks( id ){

   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   if ( task_exists( TASK_HEARTBEAT+id ) )
      // Stops the heartbeat task
      remove_task_log( TASK_HEARTBEAT+id )
   if ( task_exists( TASK_SSTING+id ) )
      // Stops the serpent sting task
      remove_task_log( TASK_SSTING+id )
   if ( task_exists( TASK_WEAKNESS+id ) )
      // Stop the Curse of Weakness task
      remove_task_log( TASK_WEAKNESS+id )
   if ( task_exists( TASK_ARCINT+id ) )
      // Stop the Arcane Intellect task
      remove_task_log( TASK_ARCINT+id )
   if ( task_exists( TASK_TRANQ+id ) )
      // Stop the Tranquility task
      remove_task_log( TASK_TRANQ+id )
   if ( task_exists( TASK_BARK+id ) )
      // Stop the Barkskin task
      remove_task_log( TASK_BARK+id )
   if ( task_exists( TASK_GIFTWILD+id ) )
      // Stop the Gift of the Wild task
      remove_task_log( TASK_GIFTWILD+id )
   if ( task_exists( TASK_WINGCLIP+id ) )
      // Stop the WingClip task
      remove_task_log( TASK_WINGCLIP+id )
   if ( task_exists( TASK_APACK+id ) )
      // Stop the Aspect of the Pack task
      remove_task_log( TASK_APACK+id )
   if ( task_exists( TASK_AWILD+id ) )
      // Stop the Aspect of the Wild task
      remove_task_log( TASK_AWILD+id )
   if ( task_exists( TASK_BRILLIANCE+id ) )
      // Stop the Brilliance task
      remove_task_log( TASK_BRILLIANCE+id )
   if ( task_exists( TASK_MAGEARMOR+id ) )
      // Stop the Mage Armor task
      remove_task_log( TASK_MAGEARMOR+id )
   if ( task_exists( TASK_BLIZZARD+id ) )
      // Stop the Blizzard task
      remove_task_log( TASK_BLIZZARD+id )
   if ( task_exists( TASK_RACIAL+id ) )
      // Stop the Racial task
      remove_task_log( TASK_RACIAL+id )
   if ( task_exists( TASK_RACIAL_PERCEPTION+id ) )
      // Stop the Perception Racial task
      remove_task_log( TASK_RACIAL_PERCEPTION+id )
   if ( task_exists( TASK_RACIAL_ESCAPE+id ) )
      // Stop the Escape Racial task
      remove_task_log( TASK_RACIAL_ESCAPE+id )
   if ( task_exists( TASK_SWPAIN+id ) )
      // Stops the shadow word: pain
      remove_task_log( TASK_SWPAIN+id )
   if ( task_exists( TASK_CORRUPT+id ) )
      // Stops the corruption
      remove_task_log( TASK_CORRUPT+id )
   if ( task_exists( TASK_DEMONSKIN+id ) )
      // Stops Demonskin 
      remove_task_log( TASK_DEMONSKIN+id )
   if ( task_exists( TASK_SPRINT+id ) )
      // Stops Sprint 
      remove_task_log( TASK_SPRINT+id )
   if ( task_exists( TASK_VANISH+id ) )
      // Stops Vanish 
      remove_task_log( TASK_VANISH+id )
   if ( task_exists( TASK_VANISHWARN1+id ) )
      // Stops the first Vanish warning 
      remove_task_log( TASK_VANISHWARN1+id )
   if ( task_exists( TASK_VANISHWARN2+id ) )
      // Stops the second Vanish warning 
      remove_task_log( TASK_VANISHWARN2+id )
   if ( task_exists( TASK_RUPTURE+id ) )
      // Stops Rupture 
      remove_task_log( TASK_RUPTURE+id )
   if ( task_exists( TASK_EVASION+id ) )
      // Stops Evasion 
      remove_task_log( TASK_EVASION+id )
   if ( task_exists( TASK_GARROTTE+id ) )
      // Stops Garrotte bleed 
      remove_task_log( TASK_GARROTTE+id )
   if ( task_exists( TASK_PWSHIELD+id ) )
      // Stops Power Word: Shield 
      remove_task_log( TASK_PWSHIELD+id )
   if ( task_exists( TASK_HEALCOOL+id ) )
      // Stops Heal Cooldown 
      remove_task_log( TASK_HEALCOOL+id )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN init_pdata subroutine
// Initialize player data
// **************************************************************************
public init_pdata(id, limited){
   // new_round event init sectio
   // core player data section
   p_maxspeed[id] = MAXSPEED_DEF             // Set p_maxspeed for normal run speed
   p_energy[id] = START_ENERGY               // Reset energy levels for Rogues
   p_int[id] = p_int_unb[id]                 // Reset normal intellect
   p_maxarmor[id] = 0                        // Reset normal maxarmor
   // racial data section 
   racial_used[id] = false                   // Reset racial not yet used
   has_shadow[id] = false                    // Reset shadow meld racial
   has_perception[id] = false                // Reset perception racial
   has_stoneform[id] = false                 // Reset perception racial
   has_berzerk[id] = false                   // Reset berzerk racial
   has_cannibal[id] = false                  // Reset cannibal racial
   isstomped[id] = false                     // Reset warstomp racial
   isinvis[id] = false                       // Reset isinvis racial
   // spell data section 
   blinkdown[id] = false                     // Reset player blink recovery
   issearching[id] = false                   // Reset player searching for target status
   serpstingon[id] = false                   // Reset serpent sting
   ruptureon[id] = false                     // Reset rupture 
   garrotteon[id] = false                    // Reset garrotte 
   corruptionon[id] = false                  // Reset corruption 
   swpainon[id] = false                      // Reset shadow word pain
   arcinton[id] = false                      // Reset arc intellect 
   blizzardon[id] = false                    // Reset blizzard 
   brillianceon[id] = false                  // Reset arc brilliance 
   magearmoron[id] = false                   // Reset mage armor
   sprinton[id] = false                      // Reset sprint 
   vanishon[id] = false                      // Reset vanish
   evasionon[id] = false                     // Reset evasion 
   tranqon[id] = false                       // Reset tranquility 
   barkon[id] = false                        // Reset barkskin 
   wingclipon[id] = false                    // Reset wingclip 
   cheapshoton[id] = false                   // Reset cheapshot 
   apackon[id] = false                       // Reset aspect of the pack 
   awildon[id] = false                       // Reset aspect of the wild
   demonskinon[id] = false                   // Reset demon skin
   pwshieldon[id] = false                    // Reset power word: shield
   eviscerateon[id] = false                  // Reset eviscerate
   isrooted[id] = false                      // Reset roots
   isfrosted[id] = false                     // Reset frostbolt effect 
   isweakened[id] = false                    // Reset curse of weakness
   stunned[id] = false                       // Reset player stunned status
   medicalert[id] = false                    // Reset medic alerts
   ctshieldon[id] = false                    // Reset tactical shield
   healdown[id] = false                      // Reset heal cooldown
   spa_speed[id] = 0.0                       // Reset spec abil speed buff

   if ( is_user_connected(id) ){
      wowmod_effect_glow( id, GLOW_NORMAL )  // Reset invis rendering
   }

   // Reset reslist for Priests
   for (new j = 1; j <= MAX_PLAYERS; j++)
      reslist[id][j] = 0

   // If limited set make sure to exit routine
   if ( limited ){
      if ( gifton[id] ){
         // Unbuff Gift of the Wild
         new parm[2]
         parm[0] = id
         wowmod_unbuff_giftwild( parm )
      }

      //set_user_maxspeed( id, MAXSPEED_DEF )     // Reset normal run speed
      return PLUGIN_CONTINUE
   }

   // If limited init NOT set, we make it here
   // Init core player data
   p_level[id] = 0      // Init player level to 0
   p_xp[id] = 0         // Init player xp to 0
   p_race[id] = -1      // Init player race
   p_class1[id] = -1    // Init player primary class 
   p_class2[id] = -1    // Init player secondary class 
   p_gold[id] = 0       // Init player gold amount
   p_spellpage[id] = 0  // Init spell page viewed last
   p_maxhealth[id] = 100// Init player max health

   // Init player attributes
   p_str[id] = -1       
   p_agi[id] = -1
   p_stam[id] = -1
   p_int[id] = -1
   p_spir[id] = -1
   p_armor[id] = -1
   p_mana[id] = -1 
   p_tier[id] = -1

   // Init player stats
   p_kills[id] = 0 
   p_deaths[id] = 0
   p_rounds[id] = 0 
   p_casts[id] = 0 
   p_heals[id] = 0 
   p_bombs[id] = 0 
   p_defuses[id] = 0 
   p_supports[id] = 0 
   p_assists[id] = 0 
   p_roguespec[id] = 0 
   p_rezzes[id] = 0 
   p_teamacts[id] = 0 
   p_badges[id] = 0 
   p_badges2[id] = 0 

   p_kills_s[id] = 0 
   p_deaths_s[id] = 0
   p_rounds_s[id] = 0 
   p_casts_s[id] = 0 
   p_heals_s[id] = 0 
   p_bombs_s[id] = 0 
   p_defuses_s[id] = 0 
   p_supports_s[id] = 0 
   p_assists_s[id] = 0 
   p_roguespec_s[id] = 0 
   p_rezzes_s[id] = 0 
   p_teamacts_s[id] = 0 

   // Init player secondary attributes
   p_melee[id] = -1
   p_marksman[id] = -1
   p_arcane[id] = -1
   p_fire[id] = -1
   p_frost[id] = -1
   p_shadow[id] = -1
   p_nature[id] = -1
   p_holy[id] = -1
   p_crit[id] = 0 
   p_spcrit[id] = 0 
   p_spdmg[id] = 0 

   // Init gear/armor
   p_head[id] = 0
   p_chest[id] = 0
   p_back[id] = 0
   p_legs[id] = 0
   p_hands[id] = 0
   p_feet[id] = 0
   p_wrists[id] = 0
   p_ring1[id] = 0
   p_ring2[id] = 0

   loadedlevel[id] = 0     // Init player has loaded xp/level data state
   change_conf[id] = false  // Reset race/class change confirmation 
   p_hud1[id] = 0

   for ( new j = 0; j < NUM_SPELLS; j++ )
      p_spells[id][j] = 0

   for ( new j = 0; j < NUM_TALENTS; j++ )
      p_talents[id][j] = 0

   if ( !get_cvar_num("mp_savexp") ){
      log_amx( "[%s] Fast-XP mode, Setting starting XP/Gold for Incoming Player", MOD )
      p_xp[id] = FASTMODE_START_XP
      p_gold[id] = FASTMODE_START_GOLD
   }

   for ( new j = 0; j < 8; j++ ){
      p_class1autocast[id][j] = false 
      p_class2autocast[id][j] = false 
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN replaceall routine - Kept from UWC3
// Routine to replace all of a specified character in a string
// **************************************************************************
stock replaceall(text[], const LEN, const WHAT[], const WITH[]) {
    while (contain(text, WHAT) != -1){
        server_print("replacing a ' ...")
        replace(text, LEN, WHAT, WITH)
    }
}


// **************************************************************************
// BEGIN check_wowmod_enabled subroutine
// Make sure WowMOD is enabled
// **************************************************************************
public check_wowmod_enabled(){
   if ( wowmod_shutdown )
      return PLUGIN_HANDLED

   if (get_cvar_num("sv_wowmod_enabled")==0){
      wowmod_enabled = false
      set_msg_block( gmsgDeathMsg, BLOCK_NOT )
   }else{
      wowmod_enabled = true
      set_msg_block( gmsgDeathMsg, BLOCK_SET )
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN str_break routine
// Routine provided by BAILOPAN to replace parse()
// **************************************************************************
stock str_break(const String[], Left[], Right[], lMax, rMax)
{
    new bool:quote_flag = false
    new bool:done_flag = false
    new left_pos = 0
    new right_pos = 0
    new i
    new hold[] = "'^"x"

    copy(Left, lMax, "")
    copy(Right, rMax, "")

    for (i=0; i<=strlen(String)-1; i++) {
        if (equali(String[i], "^"", 1)) {
            quote_flag = true
        } else if ((equali(String[i], "^"", 1)) && (quote_flag)) {
            quote_flag = false
        }
        if ((equali(String[i], " ", 1)) && (!quote_flag) && (!done_flag)) {
            done_flag = true
            i++
        }
        if (!done_flag && !equali(String[i], "^"", 1)) {
            if (left_pos < lMax) {
                setc(Left[left_pos], 1, String[i])
                if (equali(Left[left_pos], "'", 1)) {
                    setc(Left[left_pos], 1, hold[1])
                }
                left_pos++
            }
        } else {
            if (right_pos < rMax && !equali(String[i], "^"", 1)) {
                setc(Right[right_pos], 1, String[i])
                if (equali(Right[right_pos], "'", 1)) {
                    setc(Right[right_pos], 1, hold[1])
                }
                right_pos++
            }
        }
    }

    Left[left_pos] = 0
    Right[right_pos] = 0

    return true
}



// **************************************************************************
// BEGIN strsplit
// Uses Bailopan's str_break() routine to split a string into segments
// **************************************************************************
public strsplit( strlist[][12], strsource[512] ){
   new index = 0
   new item[12] = ""
   new temp[512] = ""
   while( strfind( strsource, " ", 0, 0 ) != -1 ){
      str_break( strsource, item, temp, 32, 511 )
      strsource = temp
      strlist[index++] = item
      //log_amx( "item=(%s) temp=(%s)", item, temp )
   }
   //strlist[index] = temp
   copy( strlist[index], 11, temp )
   //log_amx( "item=(%s)", strlist[index] )
}



// **************************************************************************
// BEGIN cmd_target2 subroutine - STOCK , Kept from UWC3
// A varient of cmd_target, copied verbatim from Warcraft3 FT
// **************************************************************************
stock cmd_target2(id,const arg[]) {
  new player = find_player("bl",arg)
  if (player){
    if ( player != find_player("blj",arg) ){
      console_print(id,"There are more clients matching to your argument")
      return 0
    }
  }
  else if ( ( player = find_player("c",arg) )==0 && arg[0]=='#' && arg[1] )
    player = find_player("k",str_to_num(arg[1]))
  if (!player){
    console_print(id,"Client with that name or userid not found")
    return 0
  }
  return player
}


// **************************************************************************
// BEGIN count_T subroutine - Kept from UWC3
// Count the members of the T team - rewrite for buggy amxx function
// **************************************************************************
public count_T(){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new players[32]
   new numberofplayers
   get_players(players, numberofplayers)
   new count = 0

   for (new i = 0; i < numberofplayers; ++i){
      new id = players[i]
      if ( get_user_team(id)==TEAM_T )
         count += 1
   }

   return count
}


// **************************************************************************
// BEGIN count_CT subroutine - Kept from UWC3
// Count the members of the CT team - rewrite for buggy amxx function
// **************************************************************************
public count_CT(){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new players[32]
   new numberofplayers
   get_players(players, numberofplayers)
   new count = 0

   for (new i = 0; i < numberofplayers; ++i){
      new id = players[i]
      if ( get_user_team(id)==TEAM_CT )
         count += 1
   }

   return count
}


// **************************************************************************
// BEGIN wowmod_count_spellpoints 
// Count the used spell points for the players
// **************************************************************************
public wowmod_count_spellpoints( id ){
   new count = 0
   for( new j = 0; j < NUM_SPELLS; j++ ){
      count += p_spells[id][j]
   }
   return count
}


// **************************************************************************
// BEGIN wowmod_count_spellpoints2 
// Count the used spell points for the players' secondary class
// **************************************************************************
public wowmod_count_spellpoints2( id ){
   new count = 0

   if ( p_class2[id] == -1 )
      return 0

   new startidx = -1
   new endidx = -1

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
      count += p_spells[id][j]
   }
   return count
}


// **************************************************************************
// BEGIN set_maxhealth subroutine - kept from UWC3
// Sets a player's maxhealth based on skills and attrib settings
// **************************************************************************
public set_maxhealth( id, sethealth ){

   // Set player's max. health for use with mend
   p_maxhealth[id] = 100

   new bonus = 0

   //client_print(id , print_chat, "MIH1 sethealth=(%d) str=(%d) fr=(%d)",
   //   sethealth, p_str[id], floatround( float(p_str[id]) / 10.0 ) )

   if ( p_str[id] > 10 ){
      bonus = floatround( float(p_str[id]) / 10.0 ) * STR_HEALTH_BONUS
   }

   if ( bonus ){
      p_maxhealth[id] += bonus
      if ( sethealth )
         set_user_health_log( id, p_maxhealth[id], true, "set_maxhealth()" )
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN check_effects subroutine
// Gives skill abilities at new round start and when skills are selected
// **************************************************************************
public check_effects( id, sethealth ){

   new parm[2]
   parm[0]=id

   set_maxhealth( id, sethealth )   // Set user id's maxhealth[] for the round

   //client_print( id, print_console,
   //   "mana_base=(%d) p_lev=(%d) * (%d)", p_mana_base[id], p_level[id], ( p_int[id] + MANA_ADD_LEVEL ) )

   p_mana_max[id] = p_mana_base[id] + ( p_level[id] * ( p_int[id] + MANA_ADD_LEVEL ) )
   p_mana[id] = p_mana_max[id]

   
   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_modify_armor
// Add modifications to armor based on p_armor and other affects
// **************************************************************************
public wowmod_modify_armor( id ){

   new bonus = floatround( p_armor[id] / 10.0 ) * ARMOR_BONUS

   set_user_armor( id, p_maxarmor[id] + bonus )
   
}


// **************************************************************************
// BEGIN heartbeat subroutine
// Called every second heartbeatGives skill abilities at new round start and when skills are selected
// **************************************************************************
public heartbeat( parm2[2] ){

   new id = parm2[0]

   if ( !is_user_alive( id ) )
      return PLUGIN_HANDLED

   //client_print( id, print_chat,
   //   "In heartbeat() mana=(%d) mana_max=(%d) adjust=(%d)", p_mana[id], p_mana_max[id],
   //      floatround( p_spir[id] / 10.0 ) )

   // Spirit attribute affects mana regeneration
   if ( p_mana[id] < p_mana_max[id] )
      p_mana[id] += floatround( p_spir[id] / 10.0 ) 

   // Health regeneration occurs here
   if ( !corruptionon[id] && ( get_user_health( id ) < p_maxhealth[id] ) ){
      // Stamina attribute affects health regeneration 
      new addhealth = floatround( p_stam[id] / 10.0 )
      // Cannibalize ability for Undead affects health regeneration 
      if ( ( p_race[id] == RACE_UNDEAD ) && has_cannibal[id] ){
         client_print( id, print_chat, "[%s] *CHOMP* *CHOMP* *CHOMP* you feel better ...", MOD )
         new can_health = RACIAL_CANNIBAL_HEALTH + floatround( p_level[id] / 10.0 )
         can_health = ( can_health > RACIAL_CANNIBAL_MAX ) ? RACIAL_CANNIBAL_MAX : can_health 
         addhealth += can_health 
      }
      // Warlock Demon Skin health regeneration
      if ( demonskinon[id] ){
         new name[32]
         get_user_name( id, name, 31 )
         client_print( id, print_chat, "[%s] The demons are chanting your name '%s!', you feel better...",
            MOD, name )
         new dskin_level = p_spells[id][SPELL_WARLOCK_BASE_IDX + SP_DEMONSKIN]
         new dskin_health = DEMONSKIN_HEALTH + floatround( dskin_level / 3.0 )
         addhealth += dskin_health 
      }
      set_user_health_log( id, get_user_health( id ) + addhealth, true, "heartbeat() health regen" )
   }

   // If player has tranquility, call tranq regen routine
   if ( tranqon[id] )
      wowmod_tranq_health( id )

   // Rogue energy regen handler
   if ( ( ( p_class1[id] == CLASS_ROGUE ) || ( p_class2[id] == CLASS_ROGUE ) ) &&
        ( p_energy[id] < ENERGY_MAX ) )
   {
      // Spirit affects energy buildup for rogues
      new addenergy = floatround( p_spir[id] / 10.0 )
      // Vanish spell adds extra energy regen
      if ( vanishon[id] )
         addenergy += ENERGY_FROM_VANISH 

      p_energy[id] += addenergy

      if ( p_energy[id] > ENERGY_MAX )
         p_energy[id] = ENERGY_MAX
   }

   //client_print( id, print_chat,
   //   "End of heartbeat() mana=(%d) mana_max=(%d)", p_mana[id], p_mana_max[id] )

   //set_task( HEARTBEAT, "heartbeat", TASK_HEARTBEAT+id, parm2, 2 );

   return PLUGIN_CONTINUE

}



// **************************************************************************
// BEGIN udelay_cooldown subroutine - Kept from UWC3
// Ultimate abilities cooldown routines
// **************************************************************************
public udelay_cooldown(){
   u_delay = false

   //new numplayers, players[32]
   //get_players(players, numplayers)
   //for (new i=0; i<numplayers; i++)
   //   icon_controller(players[i],ICON_SHOW)
   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN set_user_health_log subroutine - Kept from UWC3
// A frontend for set_user_health() which adds logging for debugging
// All modifications to a player's health should go through this routine
// **************************************************************************
public set_user_health_log( id, health, blog, sdesc[] ){

   // [10-05-04] Added the next check to prevent entity errors
   if ( !is_user_connected( id ) )
      return PLUGIN_CONTINUE

   new name[32]
   get_user_name(id, name, 31)

   if ( health > p_maxhealth[id] ){
      // Prevent setting too much health, log event and return
      log_amx( "[%s] set_user_health_log() :: [%s] Set Health [%d] EXCEEDS Maxhealth [%d] Reason=(%s)", 
         MOD, name, health, p_maxhealth[id], sdesc )
      set_user_health( id, p_maxhealth[id] )
      return PLUGIN_CONTINUE
   }

   //if (blog)
   //   client_print(0, print_console,
   //      "[%s DEBUG] [In set_user_health_log()] (%s) id=(%d)<%s> BEFORE-health=(%d)",
   //      MOD, sdesc, id, name, get_user_health(id))

   set_user_health( id, health )

   //if (blog)
   //   client_print(0, print_console,
   //       "[%s DEBUG] [In set_user_health_log()] (%s) id=(%d)<%s> AFTER-health=(%d)",
   //       MOD, sdesc, id, name, get_user_health(id))

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_info_spelllist subroutine
// Grab the spelllist for a specific wowmod class
// **************************************************************************
public wowmod_info_spelllist( spells[], class ){
   new temp[1024]

   if ( class == CLASS_DRUID ){
      format( temp, 1023, "<p>Druid Spells: <br />" )
      for ( new j = 0; j < 8; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, druid_spells[j], druid_spells[j] )
      }
      format( temp, 1023, "%s </p>", temp )
   }else if ( class == CLASS_HUNTER ){
      format( temp, 1023, "<p>Hunter Spells: <br />" )
      for ( new j = 0; j < 8; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, hunter_spells[j], hunter_spells[j] )
      }
      format( temp, 1023, "%s </p>", temp )
   }else if ( class == CLASS_MAGE ){
      format( temp, 1023, "<p>Mage Spells: <br />" )
      for ( new j = 0; j < 8; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, mage_spells[j], mage_spells[j] )
      }
      format( temp, 1023, "%s </p>", temp )
   }else if ( class == CLASS_ROGUE ){
      format( temp, 1023, "<p>Rogue Spells: <br />" )
      for ( new j = 0; j < 8; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, rogue_spells[j], rogue_spells[j] )
      }
      format( temp, 1023, "%s </p>", temp )
   }else if ( class == CLASS_WARLOCK ){
      format( temp, 1023, "<p>Warlock Spells: <br />" )
      for ( new j = 0; j < 8; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, warlock_spells[j], warlock_spells[j] )
      }
      format( temp, 1023, "%s </p>", temp )
   }else if ( class == CLASS_PRIEST ){
      format( temp, 1023, "<p>Prist Spells: <br />" )
      for ( new j = 0; j < 8; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, priest_spells[j], priest_spells[j] )
      }
      format( temp, 1023, "%s </p>", temp )
   }

   copy( spells, 1023, temp )
}


// **************************************************************************
// BEGIN wowmod_info_classlist subroutine
// Grab the classlist for a specific wowmod race 
// **************************************************************************
public wowmod_info_classlist( classes[], race ){
   new temp[1024]

   if ( race == RACE_NIGHTELF ){
      format( temp, 1023, "<p>Nightelf Classes: <br />" )
      for ( new j = 0; j < RACE_NIGHTELF_NUMCLASSES; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, nelf_classes[j], nelf_classes[j] )
      }
   }else if ( race == RACE_GNOME ){
      format( temp, 1023, "<p>Gnome Classes: <br />" )
      for ( new j = 0; j < RACE_GNOME_NUMCLASSES; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, gnome_classes[j], gnome_classes[j] )
      }
   }else if ( race == RACE_DWARF ){
      format( temp, 1023, "<p>Dwarf Classes: <br />" )
      for ( new j = 0; j < RACE_DWARF_NUMCLASSES; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, dwarf_classes[j], dwarf_classes[j] )
      }
   }else if ( race == RACE_HUMAN ){
      format( temp, 1023, "<p>Human Classes: <br />" )
      for ( new j = 0; j < RACE_HUMAN_NUMCLASSES; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, human_classes[j], human_classes[j] )
      }
   }else if ( race == RACE_ORC ){
      format( temp, 1023, "<p>Orc Classes: <br />" )
      for ( new j = 0; j < RACE_ORC_NUMCLASSES; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, orc_classes[j], orc_classes[j] )
      }
   }else if ( race == RACE_TAUREN ){
      format( temp, 1023, "<p>Tauren Classes: <br />" )
      for ( new j = 0; j < RACE_TAUREN_NUMCLASSES; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, tauren_classes[j], tauren_classes[j] )
      }
   }else if ( race == RACE_UNDEAD ){
      format( temp, 1023, "<p>Undead Classes: <br />" )
      for ( new j = 0; j < RACE_UNDEAD_NUMCLASSES; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, undead_classes[j], undead_classes[j] )
      }
   }else if ( race == RACE_BLOODELF ){
      format( temp, 1023, "<p>Bloodelf Classes: <br />" )
      for ( new j = 0; j < RACE_BLOODELF_NUMCLASSES; j++ ){
         format( temp, 1023, "%s %s - say help %s <br />", temp, belf_classes[j], belf_classes[j] )
      }
   }

   copy( classes, 1023, temp )
}


// **************************************************************************
// BEGIN wowmod_check_badges
// Check player for new badges earned 
// **************************************************************************
public wowmod_check_badges( id ){

   //new last_bid
   new num_reqs = 0
   new num_reqs_met = 0
   new badges = 0
   new class = -1

   switch ( p_class1[id] ){
      case CLASS_DRUID: class = CB_DRUID
      case CLASS_HUNTER: class = CB_HUNTER
      case CLASS_MAGE: class = CB_MAGE
      case CLASS_ROGUE: class = CB_ROGUE
      case CLASS_WARLOCK: class = CB_WARLOCK
      case CLASS_PRIEST: class = CB_PRIEST
   }

   for ( new j = 1; j < num_badges; j++ ){
      if ( !( b_classes[j] & class ) )
         continue

      if ( j <= 17 )
         badges = p_badges[id]
      else
         badges = p_badges2[id]

      // Check if we already have this badge
      if ( badges & b_bit[j] )
         continue

      num_reqs = 0
      num_reqs_met = 0

      //last_bid = j

      if ( b_kills[j] ){
         num_reqs += 1
         if ( p_kills[id] >= b_kills[j] )
            num_reqs_met += 1
      }
      if ( b_kills_s[j] ){
         num_reqs += 1
         if ( p_kills_s[id] >= b_kills_s[j] )
            num_reqs_met += 1
      }
      if ( b_assists[j] ){
         num_reqs += 1
         if ( p_assists[id] >= b_assists[j] )
            num_reqs_met += 1
      }
      if ( b_assists_s[j] ){
         num_reqs += 1
         if ( p_assists_s[id] >= b_assists_s[j] )
            num_reqs_met += 1
      }
      if ( b_heals[j] ){
         num_reqs += 1
         if ( p_heals[id] >= b_heals[j] )
            num_reqs_met += 1
      }
      if ( b_heals_s[j] ){
         num_reqs += 1
         if ( p_heals_s[id] >= b_heals_s[j] )
            num_reqs_met += 1
      }
      if ( b_teamacts[j] ){
         num_reqs += 1
         if ( p_teamacts[id] >= b_teamacts[j] )
            num_reqs_met += 1
      }
      if ( b_teamacts_s[j] ){
         num_reqs += 1
         if ( p_teamacts_s[id] >= b_teamacts_s[j] )
            num_reqs_met += 1
      }
      if ( b_roguespec[j] ){
         num_reqs += 1
         if ( p_roguespec[id] >= b_roguespec[j] )
            num_reqs_met += 1
      }
      if ( b_roguespec_s[j] ){
         num_reqs += 1
         if ( p_roguespec_s[id] >= b_roguespec_s[j] )
            num_reqs_met += 1
      }

      if ( num_reqs && ( num_reqs == num_reqs_met ) ){
         //client_print( id, print_chat, ">> badge Earned [%d] (%s) nr=(%d) nrm=(%d)", 
         //   j, badge[j], num_reqs, num_reqs_met )

         if ( j <= 17 )
            p_badges[id] |= b_bit[j]
         else
            p_badges2[id] |= b_bit[j]

         new name[32]
         get_user_name( id, name, 31 )

         client_print( 0, print_chat, "[%s] Announcement: Congratulations %s for earning Badge: %s",
            MOD, name, badge[j] )
         client_print( id, print_center, "[%s] Congratulations! You have earned a Badge: %s",
            MOD, badge[j] )
      }else{
         //client_print( id, print_chat, ">> badge Not earned [%d] (%s) nr=(%d) nrm=(%d)", 
         //   j, badge[j], num_reqs, num_reqs_met )
      }

   }

   //Debug Only, test adding badges to badges bitfields
   //client_print( id, print_chat, ">> debug - giving badge [%d] (%s)",
   //   last_bid, badge[last_bid] )
   //if ( last_bid <= 17 )
   //   p_badges[id] |= b_bit[last_bid]
   //else
   //   p_badges2[id] |= b_bit[last_bid]
   
}


// **************************************************************************
// BEGIN wowmod_check_tier
// Handle gear tier levelups
// **************************************************************************
public wowmod_check_tier( id ){
   new name[32]
   get_user_name(id, name, 31)

   new tier = -1 
   new old_tier = p_tier[id]

   if ( ( p_level[id] >= TIER10_LEVEL ) && ( p_tier[id] == 9 ) && wowmod_badges_tier( id, 10 ) ){
      tier = 10   
   }else if ( ( p_level[id] >= TIER9_LEVEL ) && ( p_tier[id] == 8 ) && wowmod_badges_tier( id, 9 ) ){
      tier = 9   
   }else if ( ( p_level[id] >= TIER8_LEVEL ) && ( p_tier[id] == 7 ) && wowmod_badges_tier( id, 8 ) ){
      tier = 8   
   }else if ( ( p_level[id] >= TIER7_LEVEL ) && ( p_tier[id] == 6 ) && wowmod_badges_tier( id, 7 ) ){
      tier = 7   
   }else if ( ( p_level[id] >= TIER6_LEVEL ) && ( p_tier[id] == 5 ) && wowmod_badges_tier( id, 6 ) ){
      tier = 6   
   }else if ( ( p_level[id] >= TIER5_LEVEL ) && ( p_tier[id] == 4 ) && wowmod_badges_tier( id, 5 ) ){
      tier = 5   
   }else if ( ( p_level[id] >= TIER4_LEVEL ) ){
      tier = 4   
   }else if ( ( p_level[id] >= TIER3_LEVEL ) ){
      tier = 3   
   }else if ( ( p_level[id] >= TIER2_LEVEL ) ){
      tier = 2   
   }else if ( ( p_level[id] >= TIER1_LEVEL ) ){
      tier = 1   
   }

   p_tier[id] = tier

   new gearlist[MAX_GLIST]
   wowmod_gear_getcraftable( id, gearlist )
   p_gearlist[id] = gearlist

   new itemlist[MAX_GLIST]
   wowmod_gear_getitems( id, itemlist )
   p_itemlist[id] = itemlist

   if ( tier <= old_tier )
      return PLUGIN_CONTINUE

   client_print(id, print_center, 
      "Congratulations - Equipment upgrades available at the merchants")
   client_print( 0, print_chat,
      "[%s] Announcement ::  Congratulations to %s for unlocking tier %d equipment", 
      MOD, name, tier )


   //client_print( 0, print_console, "DEBUG: In wowmod_check_tier() p_tier=(%d)", p_tier[id] )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_check_levelup 
// Handle level gains and initial level load from data
// **************************************************************************
public wowmod_check_levelup( id ){

   new name[32]
   get_user_name(id, name, 31)

   new bool:bLvled = false

   if ( ( p_level[id] < ( MAX_LEVEL-1 ) ) && 
      ( p_xp[id] >= xplevel_lev[p_level[id]+1] ) )
   {
      for ( new i = ( p_level[id]+1 ); i < MAX_LEVEL; ++i ){
         if ( p_xp[id] >= xplevel_lev[i] ){
            // Level Gain
            bLvled = true
            p_level[id] = i
            //log_amx( "New level for %s  %d pxp=(%d) xplev=(%d)",
            //        name, p_level[id], p_xp[id], xplevel_lev[i] )
         }
      }
   }

   //new bool:bretLvl = false

   if (bLvled){
      // Level Gained - Player has gained a level
      //log_amx( "[%s DEBUG] Level Gain p_level=(%d) ", MOD, p_level[id] )

      //set_hudmessage(200, 100, 0, -1.0, 0.35, 0, 1.0, 7.0, 0.1, 0.2, 2)

      if ( loadedlevel[id] ){
         //bretLvl = true
         //show_hudmessage(id, "Congratulations - You gained a level")

         client_print( id, print_center,
            "[%s] Congratulations! You gained a level", MOD )
         client_print( 0, print_chat,
            "[%s] Announcement ::  Congratulations %s for gaining a level", 
            MOD, name )

         if ( ( p_level[id] >= DUAL_CLASS_LEVEL ) && ( p_class2[id] == -1 ) ){
            client_print( id, print_chat,
               "[%s] You are now elligible to make a secondary class selection!", MOD )
            client_print( id, print_center,
               "[%s] You are now elligible to make a secondary class selection!", MOD )
         }
      }else{
         loadedlevel[id] = 1    // Now player has initial levels loaded from data 
         //show_hudmessage(id, "WowMOD> Saved XP and Levels Loaded")
         client_print( id, print_chat,
            "[%s] Your Character Data has been loaded...", MOD )

         // Initialize p_mana_max and adjust mana to max value
         p_mana_max[id] = p_mana_base[id] + ( p_level[id] * ( p_int[id] + MANA_ADD_LEVEL ) )
         p_mana[id] = p_mana_max[id]
      }

      if ( is_user_alive( id ) )
         wowmod_sound_playsound( id, SOUND_LEVELUP )

   }

   // Reset loadedlevel
   loadedlevel[id] = 1

   return PLUGIN_CONTINUE 
}


// **************************************************************************
// BEGIN armor_type subroutine
// Kept from Warcraft3FT / UWC3
// **************************************************************************
public armor_type(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if (read_data(1)){
      helmet[id] = true
      hassuit[id] = true
   }else{
      helmet[id] = false
      hassuit[id] = true
   }

   p_maxarmor[id] = get_user_armor( id )

   wowmod_modify_armor( id )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN change_weapon subroutine
// Kept from Warcraft3FT / UWC3
// **************************************************************************
public change_weapon(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new clipamount = 0
   new ammoamount = 0
   new weaponnum = 0
   weaponnum = ( get_user_weapon( id, clipamount, ammoamount ) )

   p_weapon[id] = weaponnum
   //client_print( id, print_chat, ">> change_weapon() weap=(%d)", p_weapon[id] )

   new parm[1]
   parm[0] = id
   set_task( 0.1, "wowmod_cheetah", TASK_ACHEETAH, parm, 1 )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN remove_task_log_log subroutine
// Remove task and log it 
// **************************************************************************
public remove_task_log( tid ){
   log_amx( "[%s] CLEANUP: Removing task id [%d]", MOD, tid )
   remove_task( tid )
}


// **************************************************************************
// BEGIN wowmod_admin_godmode subroutine
// Enable Godmode for admin 
// **************************************************************************
public wowmod_admin_godmode( id ){

   if ( !( get_user_flags(id) & get_cvar_num("WOW_admin_flag") ) ){
      client_print( id, print_console, "[%s] Access Denied" )
      return PLUGIN_HANDLED
   }

   set_user_health( id, 5000 )

   p_maxhealth[id] = 5000

   client_print( id, print_center, "[%s] God-mode Enabled", MOD )
   client_print( id, print_chat, "[%s] God-mode Enabled", MOD )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_admin_shutdown subroutine
// Admin command to shutdown the WowMOD plugin
// **************************************************************************
public wowmod_admin_shutdown( id ){

   if ( !( get_user_flags(id) & get_cvar_num("WOW_admin_flag") ) ){
      client_print( id, print_console, "[%s] Access Denied" )
      return PLUGIN_HANDLED
   }

   wowmod_enabled = false
   wowmod_shutdown = true
 
   client_print( id, print_console, "[%s] WowMOD has been disabled by admin (wowmod-enabled=%b)", 
      MOD, wowmod_enabled )
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN spawn_player subroutine - kept from UWC3
// Respawn a dead player 
// **************************************************************************
public spawn_player( parm[2] ){
   new id = parm[0]

   if ( !is_user_connected( id ) )
      return PLUGIN_CONTINUE

   user_spawn( id )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN spawn_player subroutine 
// Setup a respawned character 
// **************************************************************************
public spawn_player_fix( parm[4] ){
   new id = parm[0]
   new health = parm[1]
   new mana = parm[2]

   if ( !is_user_connected( id ) )
      return PLUGIN_CONTINUE

   set_user_godmode(id, 1)
   set_user_godmode(id, 0)

   give_item( id, "item_suit" )
   give_item( id, "weapon_knife" )
   set_user_maxspeed( id, p_maxspeed[id] )

   set_user_health_log( id, health, true, "spawn_player()" )
   p_mana[id] = mana

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN stock user_spawn - Kept from UWC3
// Spawn a user
// **************************************************************************
stock user_spawn(index)
   return spawn(index)



// **************************************************************************
// END WowMOD Utility Routines 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

