/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Damage Events
*** Date:          Aug 11, 2007
*** Last Update:   Sep 24, 2007
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
// BEGIN WowMOD Damage Events 
// **************************************************************************


// **************************************************************************
// BEGIN apply_damage subroutine
// Handles extra damage dealt by spells
// **************************************************************************
public apply_damage( vict_id, atk_id, dmg, class, spell, spname[] ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new vname[32], aname[32], logtext[64], logtext2[64]
   get_user_name( vict_id, vname, 31)
   get_user_name( atk_id, aname, 31)

   format( logtext, 63, "apply_damage::Spell %s", spname )

   //client_print( 0, print_chat, "In apply_damage() spell=(%s) dmg=(%d)", spname, dmg )

   // Check for extra spell damage
   if ( p_spdmg[atk_id] > 0 ){
      dmg += p_spdmg[atk_id]
      client_print( atk_id, print_chat, "[%s] Your %s spell does extra damage to %s", 
            MOD, spname, vname )
   }

   // Check for spell crit
   if ( p_spcrit[atk_id] > 0 ){
      new rnum = random_num( 0, 100 )
      //client_print( atk_id, print_chat, "spell_crit> p_spcrit=%d rnum=%d", p_spcrit[atk_id], rnum )
      if ( rnum < p_spcrit[atk_id] ){
         dmg *= 2
         client_print( atk_id, print_chat, "[%s] Your %s spell scores a Critical Strike on %s", 
            MOD, spname, vname )
         client_print( vict_id, print_chat, "[%s] %s scores a Critical Strike on you with %s", 
            MOD, aname, spname )
      }
   }

   wowmod_effect_glow( vict_id, GLOW_WHITE )

   new parm[2]
   parm[0] = vict_id
   set_task( 1.5, "wowmod_effect_glowoff", TASK_GLOWOFF+vict_id, parm, 2 )
 
   set_user_health_log( vict_id, get_user_health(vict_id) - dmg, false, logtext )

   if ( get_cvar_num("WOW_dmgxpbonus") )
      damageto[atk_id][vict_id] += dmg

   if ( equali( spname, "Drainlife" ) ){
      format( logtext2, 63, "apply_damage::Spell %s (replenish)", spname )
      set_user_health_log( atk_id, get_user_health(atk_id) + dmg, false, logtext2 )
   }

   if ( get_user_health(vict_id) <= 0 && !deathmessageshown[vict_id] ){
      // Spell damage caused player death
      deathmessageshown[vict_id] = true
      new string[48]
      copy( string, 47, spname )
      message_begin( MSG_ALL, gmsgDeathMsg, {0,0,0}, 0)
      write_byte( atk_id )
      write_byte( vict_id )
      write_byte( 0 )
      write_string( string )
      message_end()
      client_print( atk_id, print_chat, "[%s] You killed %s with %s", MOD, vname, spname )
      client_print( vict_id, print_chat, "[%s] You were killed by %s with %s", MOD, aname, spname )

      // Award XP, Gold and stat bonuses
      xpgiven = xpgiven_lev[p_level[atk_id]]
      p_kills[atk_id] += 1
      p_kills_s[atk_id] += 1
      p_deaths[vict_id] += 1
      p_deaths_s[vict_id] += 1

      p_xp[atk_id] += xpgiven
      p_gold[vict_id] += GOLD_KILL

   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN damage_event subroutine
// Handler for all damage events
// **************************************************************************
public damage_event(id){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new weapon = 0
   new bodypart = 0
   new enemy = get_user_attacker( id, weapon, bodypart )
   new damage = read_data(2)
   new orig_damage = damage
   
   //log_amx( "MIH1 in damage_event, enemy=(%d) damage=(%d)", enemy, damage )
   //if ( weapon == WEAPON_KNIFE )
   //   client_print( enemy, print_chat, ">> damage_event() vict=(%d) dmg=(%d) bodypart=(%d) weap=(%d)", 
   //      id, damage, bodypart, weapon )


   if ( ( id <= 0 ) || ( id > MAX_PLAYERS ) || ( enemy <= 0 ) || ( enemy > MAX_PLAYERS ) )
      return PLUGIN_CONTINUE

   if ( id == enemy )
      return PLUGIN_CONTINUE

   new name1[32], name2[32]
   get_user_name(id, name1, 31)
   get_user_name(enemy, name2, 31)

   new bool:dodged = false             // Dodge for Nightelves
   new rnum = random_num( 0, 100 )     // Random number for general use

   new monk_spell = p_spells[id][SPELL_HUNTER_BASE_IDX+SP_AMONKEY]
   new evade_spell = p_spells[id][SPELL_ROGUE_BASE_IDX+SP_EVASION]
   new dodge_chance = 0
   // Check for various ways a player can dodge damage
   if ( p_race[id] == RACE_NIGHTELF ){
      // Nightelf dodge chance
      dodge_chance += NELF_DODGE_CHANCE
      //client_print( id, print_chat, "dodge1> chance=(%d) from nelf race", dodge_chance )
   }
   if ( ( p_class1[id] == CLASS_ROGUE ) || ( p_class2[id] == CLASS_ROGUE ) ){
      if ( p_weapon[id] == WEAPON_KNIFE ){
         dodge_chance += ROGUE_DODGE2_CHANCE

         if ( evasionon[id] )
            dodge_chance += EVASION_DODGE_CHANCE1 + evade_spell
      }else{
         dodge_chance += floatround( ( ROGUE_DODGE_CHANCE * p_level[id] ) / 10.0 )

         if ( evasionon[id] )
            dodge_chance += EVASION_DODGE_CHANCE2 + evade_spell
      }
   }
   if ( monk_spell > 0 ){
      dodge_chance += ( MONKEY_DODGE_CHANCE + monk_spell )
      //client_print( id, print_chat, "dodge2> chance=(%d) from monkey spell", dodge_chance )
   }

   //client_print( id, print_chat, "dodge3> rnum=(%d) chance=(%d)", rnum, dodge_chance )
   if ( ( id != enemy ) && ( rnum < dodge_chance ) ){
      // Check if player has dodged the bullet
      dodged = true 
      set_user_health_log( id, get_user_health(id) + damage, false, "damage_event()::Dodge Success" )

      if ( evasionon[id] ){
         client_print( id, print_chat, "[%s] You skillfully evade an attack from %s", MOD, name2 )
         client_print( enemy, print_chat, "[%s] %s skillfully evades your attack", MOD, name1 )
      }else{
         client_print( id, print_chat, "[%s] You skillfully dodged an attack from %s", MOD, name2 )
         client_print( enemy, print_chat, "[%s] %s skillfully dodged your attack", MOD, name1 )
      }
   }
 

   if ( barkon[id] ){
      // Druid Barkskin damage absorption
      new bark_spell = p_spells[id][SPELL_DRUID_BASE_IDX + SP_BARKSKIN]
      set_user_health_log( id, get_user_health(id) + bark_spell, false, "damage_event()::Barkskin" )
      client_print( id, print_chat, "[%s] Your Barkskin absorbs some of the damage from %s", MOD, name2 )
   }

   if ( !dodged && pwshieldon[id] ){
      // Victim has Power Word: Shield
      new pws_spell = p_spells[id][SPELL_PRIEST_BASE_IDX+SP_PWSHIELD]
      new Float:shield = ( PWSHIELD_PROT * float(pws_spell) > PWSHIELD_PROT_MAX ) ? PWSHIELD_PROT_MAX :
                         ( PWSHIELD_PROT * float(pws_spell)  )
      new dmg_prot = floatround( float( damage ) * shield ) 
      set_user_health_log( id, get_user_health(id) + dmg_prot, false, 
                          "damage_event()::Power Word: Shield Protection ")
      //client_print( id, print_chat, "shield> damage=(%d) dmg_prot=(%d)", damage, dmg_prot )
   }

   if ( !dodged && demonskinon[id] ){
      // Victim has Demon Skin
      new dskin_spell = p_spells[id][SPELL_WARLOCK_BASE_IDX+SP_DEMONSKIN]
      new Float:shield = ( DEMONSKIN_PROT*float(dskin_spell) > DEMONSKIN_PROT_MAX ) ? DEMONSKIN_PROT_MAX :
                         ( DEMONSKIN_PROT*float(dskin_spell)  )
      new dmg_prot = floatround( float( damage ) * shield ) 
      set_user_health_log( id, get_user_health(id) + dmg_prot, false, 
                          "damage_event()::Demon Skin Protection ")
      //client_print( id, print_chat, "demonskin> damage=(%d) dmg_prot=(%d)", damage, dmg_prot )
   }

   if ( !dodged && isweakened[enemy] ){
      // Attacker has curse of weakness
      damage = floatround( damage / 2.0 )      
      set_user_health_log( id, get_user_health(id) + ( orig_damage - damage ), false, 
                          "damage_event()::Curse of Weakness Damage Lessened ")
      //client_print( 0, print_chat, "%s has curse of weakness, damage halved orig=(%d) dmg=(%d)", 
      //               name2, orig_damage, damage )
   }

   if ( !dodged && has_berzerk[enemy] ){
      // Attacker is Berzerk
      new Float:div = 15.0
      if ( p_level[enemy] > 70 )
         div = 8.0
      else if ( p_level[enemy] > 60 )
         div = 9.0
      else if ( p_level[enemy] > 50 )
         div = 10.0
      else if ( p_level[enemy] > 40 )
         div = 11.0
      else if ( p_level[enemy] > 30 )
         div = 12.0
      else if ( p_level[enemy] > 20 )
         div = 13.0
      new berzerk_dmg = floatround( damage / div )      
      apply_damage( id, enemy, berzerk_dmg, CLASS_NA, SP_NA, "Berzerk" )
      //client_print( enemy, print_chat, "berzerk> doing extra berzerk damage orig=(%d) dmg=(%d)", 
      //   orig_damage, berzerk_dmg )
   }

   if ( !dodged && has_berzerk[id] ){
      // Victim is Berzerk
      new Float:div = 7.0
      if ( p_level[id] > 70 )
         div = 10.0
      else if ( p_level[id] > 60 )
         div = 9.5
      else if ( p_level[id] > 50 )
         div = 9.0
      else if ( p_level[id] > 40 )
         div = 8.5
      else if ( p_level[id] > 30 )
         div = 8.0
      else if ( p_level[id] > 20 )
         div = 7.5
      new berzerk_dmg = floatround( damage / div )      
      apply_damage( enemy, id, berzerk_dmg, CLASS_NA, SP_NA, "Berzerk" )
      //client_print( id, print_chat, "berzerk> taking vulnerability berzerk damage orig=(%d) dmg=(%d)", 
      //   orig_damage, berzerk_dmg )
   }

   // Check for weapons critical strike
   if ( !dodged && ( p_crit[enemy] > 0 ) ){
      new rnum = random_num( 0, 100 )
      //client_print( enemy, print_chat, "crit> p_crit=%d rnum=%d", p_crit[enemy], rnum )
      if ( rnum < p_crit[enemy] ){
         new crit_dmg = damage
         client_print( enemy, print_chat, "[%s] You score a Critical Strike on %s!", MOD, name1 )
         client_print( id, print_chat, "[%s] %s scores a Critical Strike on you!", MOD, name2 )
         apply_damage( id, enemy, crit_dmg, CLASS_NA, SP_NA, "Critical Strike" )
      }
   }

   // Check for a Rogue Backstab
   if ( ( p_class1[enemy] == CLASS_ROGUE ) || ( p_class2[enemy] == CLASS_ROGUE ) ){
      new backstab_rank = p_spells[enemy][SPELL_ROGUE_BASE_IDX + SP_BACKSTAB]
      if ( ( backstab_rank > 0 ) && ( weapon == WEAPON_KNIFE ) && 
         ( ( bodypart & BODYPART_BODY ) || ( bodypart & BODYPART_CHEST ) ) )
      {
         new Float:angles[3]
         new Float:tangles[3]

         pev( enemy, pev_angles, angles )
         pev( id, pev_angles, tangles )

         //client_print( enemy, print_chat, ">> My Angles: x=(%f) y=(%f) z=(%f) ", 
         //   angles[0], angles[1], angles[2] )
         //client_print( enemy, print_chat, ">> Victim Angles: x=(%f) y=(%f) z=(%f) ", 
         //   tangles[0], tangles[1], tangles[2] )

         new Float:X_diff = floatabs( angles[0] - tangles[0] )
         new Float:Y_diff = floatabs( angles[1] - tangles[1] )

         //client_print( enemy, print_chat, ">> Angle Diffs: X_diff=(%f) Y_diff=(%f)", X_diff, Y_diff )

         if ( ( X_diff < 45.0 ) && ( Y_diff < 45.0 ) ){
            p_roguespec[enemy] += 1
            p_roguespec_s[enemy] += 1

            new bs_dmg = DMG_BACKSTAB_BASE * backstab_rank
            client_print( enemy, print_chat, "[%s] You land a Backstab on %s!", MOD, name1 )
            client_print( id, print_chat, "[%s] %s has landed a Backstab on you!", MOD, name2 )
            apply_damage( id, enemy, bs_dmg, CLASS_NA, SP_NA, "Backstab" )
         }
      } 
   }

   // Check for a Rogue Garrotte or Eviscerate 
   if ( ( p_class1[enemy] == CLASS_ROGUE ) || ( p_class2[enemy] == CLASS_ROGUE ) ){
      new gar_rank = p_spells[enemy][SPELL_ROGUE_BASE_IDX + SP_GARROTTE]
      new evis_rank = p_spells[enemy][SPELL_ROGUE_BASE_IDX + SP_EVISCERATE]
      if ( eviscerateon[enemy] && ( weapon == WEAPON_KNIFE ) ){
         // Eviscerate
         new rnum = random_num( 0, 100 )

         new victim_resist = wowmod_calc_resist( id, PF_MELEE )

         //client_print( enemy, print_chat, "eviscerate>> rnum=(%d) chance=(%d)",
         //   rnum, floatround ( ( (2*p_agi[enemy]) + p_melee[enemy] - victim_resist ) / 2.0 ) )

         eviscerateon[enemy] = false

         if ( rnum < floatround ( ( (2*p_agi[enemy]) + p_melee[enemy] - victim_resist ) / 2.0 ) ){
            p_roguespec[enemy] += 1
            p_roguespec_s[enemy] += 1

            client_print( enemy, print_chat, "[%s] You Eviscerate %s", MOD, name1 )
            client_print( id, print_chat, "[%s] You have been Eviscerated by %s", MOD, name2 )

            new dmg = DMG_EVISCERATE_BASE * evis_rank
            apply_damage( id, enemy, dmg, CLASS_ROGUE, SP_EVISCERATE, "Eviscerate" )
         }else{
            client_print( enemy, print_chat, "[%s] Your Eviscerate attempt on %s is resisted", MOD, name1 )
         }

      }
      if ( ( gar_rank > 0 ) && ( weapon == WEAPON_KNIFE ) ){
         // Garrotte
         new rnum = random_num( 0, 100 )

         new victim_resist = wowmod_calc_resist( id, PF_MELEE )

         //client_print( enemy, print_chat, "garrotte>> rnum=(%d) chance=(%d)",
         //   rnum, floatround ( ( p_agi[enemy] + p_melee[enemy] - victim_resist ) / 4.0 ) )

         if ( rnum < floatround ( ( p_agi[enemy] + p_melee[enemy] - victim_resist ) / 4.0 ) ){

            garrotteon[id] = true

            client_print( enemy, print_chat, "[%s] Your Garrotte causes %s to bleed out", MOD, name1 )
            client_print( id, print_chat, "[%s] A Garrotte causes you to bleed out", MOD, name2 )

            new Float:ftimer = GARROTTE_BASE_TIMER - ( 0.20 * gar_rank )
            new dmg = DMG_GARROTTE * gar_rank
            new args[5]
            args[0] = id
            args[1] = enemy
            args[2] = 0
            args[3] = dmg
            args[4] = SP_GARROTTE
            set_task( ftimer, "wowmod_damage_rupture", TASK_GARROTTE+id, args, 5, "b" );
         }
      }
   }


   if ( get_cvar_num("WOW_dmgxpbonus") ){
      //log_amx( "MIH2 in damage_event, enemy=(%d) damage=(%d)", enemy, damage )
      damageto[enemy][id] += damage
   }

   // Award Rogues some energy
   p_energy[id] += damage 
   if ( p_energy[id] > ENERGY_MAX )
      p_energy[id] = ENERGY_MAX

   // Now check against passive damage dealing spells

   // spell_dmg is return value from wowmod_dmgspell_<spell> subroutine
   new spell_dmg = 0

   // Druid spell Wrath
   if ( ( spell_dmg = wowmod_dmgspell_wrath( id, enemy ) ) > 0 )
      apply_damage( id, enemy, spell_dmg, CLASS_DRUID, SP_WRATH, "Wrath" )

   // Druid spell Thorns 
   if ( ( spell_dmg = wowmod_dmgspell_thorns( id, enemy ) ) > 0 )
      apply_damage( enemy, id, spell_dmg, CLASS_DRUID, SP_THORNS, "Thorns" )

   // Hunter spell Serpent Sting 
   wowmod_dmgspell_ssting( id, enemy ) 

   // Hunter spell Wingclip 
   wowmod_dmgspell_wingclip( id, enemy ) 

   // Hunter spell Counterattack 
   if ( ( spell_dmg = wowmod_dmgspell_counter( id, enemy ) ) > 0 )
      apply_damage( enemy, id, spell_dmg, CLASS_HUNTER, SP_COUNTER, "Counterattack" )

   // Mage spell Scorch 
   if ( ( spell_dmg = wowmod_dmgspell_scorch( id, enemy ) ) > 0 )
      apply_damage( id, enemy, spell_dmg, CLASS_MAGE, SP_SCORCH, "Scorch" )

   // Rogue spell Cheap Shot 
   if ( ( spell_dmg = wowmod_dmgspell_cheapshot( id, enemy ) ) > 0 )
      apply_damage( id, enemy, spell_dmg, CLASS_HUNTER, SP_CHEAPSHOT, "Cheapshot" )

   // Rogue Rupture 
   wowmod_dmgspell_rupture( id, enemy ) 

   // Warlock spell Drain Life 
   if ( ( spell_dmg = wowmod_dmgspell_drainlife( id, enemy ) ) > 0 )
      apply_damage( id, enemy, spell_dmg, CLASS_WARLOCK, SP_DRAINLIFE, "Drainlife" )


   // Warlock spell Curse of Weakness 
   wowmod_dmgspell_weakness( id, enemy ) 

   // Warlock spell Drain Mana 
   wowmod_dmgspell_drainmana( id, enemy ) 

   // Warlock spell Corruption 
   wowmod_dmgspell_corruption( id, enemy ) 

   // Priest spell Smite
   if ( ( spell_dmg = wowmod_dmgspell_smite( id, enemy ) ) > 0 )
      apply_damage( id, enemy, spell_dmg, CLASS_PRIEST, SP_SMITE, "Smite" )

   // Priest spell Shadow Word: Pain 
   wowmod_dmgspell_pain( id, enemy ) 

   // Priest spell Manaburn
   wowmod_dmgspell_manaburn( id, enemy )

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN death subroutine
// Handler for death event - Called when you die
// **************************************************************************
public death(){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new killer_id = read_data(1)
   new victim_id = read_data(2)
   new headshot = read_data(3)

   if ( !is_user_connected( victim_id ) )
      return PLUGIN_CONTINUE

   new kname[32], vname[32]
   get_user_name(killer_id, kname, 31)
   get_user_name(victim_id, vname, 31)

   new weaponname[20]
   read_data(4,weaponname,31)
   new weapon
   get_user_attacker(victim_id,weapon)


   log_amx( "WowMOD death() %s killed by %s", vname, kname )

   if( !deathmessageshown[victim_id] && killer_id != victim_id &&
       !equal( weaponname,"world" ) )
   {
      deathmessageshown[victim_id]=true
      message_begin( MSG_ALL, gmsgDeathMsg,{0,0,0},0)
      write_byte(killer_id)
      write_byte(victim_id)
      write_byte(headshot)
      write_string(weaponname)
      write_string(weaponname)
      message_end()
   }

   xpgiven = xpgiven_lev[p_level[killer_id]]

   if ( killer_id && killer_id != victim_id && victim_id ){
      new name[32]
      get_user_name(victim_id,name,31)

      if ( get_cvar_num("WOW_dmgxpbonus") ){
         //log_amx( "MIH in dmgxpbonus check" )
         // [7-1-04] K2mia => Added partial kill XP for majority dmg dealer
         new mdmg = 0           // current max dmg done to victim_id
         new dmg_id = killer_id // current max damager
         new dmgxp = 0

         new pid, numplayers, players[32]
         get_players(players, numplayers)

         for (new i=0; i< numplayers; i++){
            pid = players[i]
            if (damageto[pid][victim_id] > mdmg){
               mdmg = damageto[pid][victim_id]
               dmg_id = pid
            }

            // [09-14-04] Prevent dmg from being used again - K2mia
            damageto[pid][victim_id] = -1
         }

         new kteam = get_user_team(killer_id)

         // [08-22-04] - Prevent SPEC from getting serious injury XP
         if ((dmg_id != killer_id) && (dmg_id != victim_id) &&
            (kteam == TEAM_T || kteam == TEAM_CT) )
         {
            dmgxp = xpgiven_lev[p_level[dmg_id]]
            new dname[32]
            get_user_name(dmg_id, dname, 31)

            //client_print(0, print_console, 
            //    "[%s] BEFORE serious injury bonus for [%d](%s) xp=(%d) bonus=(%d)", MOD, dmg_id, dname, playerxp[dmg_id], dmgxp)

            p_xp[dmg_id] += dmgxp
            p_gold[dmg_id] += GOLD_DMG
 
            log_amx( "WowMOD Partial Kill XP bonus to %s [ %d ]", dname, dmgxp )
            log_amx( "WowMOD Partial Kill Gold bonus to %s [ %d ]", dname, GOLD_DMG )

            //client_print(0, print_console, 
            //    "[%s] AFTER serious injury bonus for [%d](%s) xp=(%d) bonus=(%d)", MOD, dmg_id, dname, playerxp[dmg_id], dmgxp)

            client_print(dmg_id, print_chat, 
               "[%s] You have scored a kill assist of %s, XP and Gold bonuses earned", 
               MOD, name )

            p_assists[dmg_id] += 1
            p_assists_s[dmg_id] += 1

            //client_print(0, print_console, 
            //   "[%s] You receive an XP bonus of %d for seriously injuring %s", MOD, dmgxp, name)
         }


      }
   }

   if ( killer_id && victim_id && killer_id != victim_id ){
      p_kills[killer_id] += 1
      p_kills_s[killer_id] += 1
      p_deaths[victim_id] += 1
      p_deaths_s[victim_id] += 1
   }

   p_xp[killer_id] +=  xpgiven 
   p_gold[killer_id] += GOLD_KILL

   wowmod_hud_playerdata( victim_id, HUD_MODE_BASIC )

   return PLUGIN_CONTINUE

}


// **************************************************************************
// END WowMOD Damage Events 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

