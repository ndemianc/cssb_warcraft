/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Spells Module
*** Date:          Aug 15, 2007
*** Last Update:   Oct 23, 2007
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
// BEGIN WowMOD Spells Module 
// **************************************************************************


// **************************************************************************
// BEGIN wowmod_dmgspell_ subroutine
// Conditions handler for damage spells, returns true if spell lands
// **************************************************************************
public wowmod_dmgspell_( id, enemy ){
}



// **************************************************************************
// BEGIN wowmod_dmgspell_smite subroutine
// Conditions handler for smite damage spell
// **************************************************************************
public wowmod_dmgspell_smite( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new smite_spell = p_spells[enemy][SPELL_PRIEST_BASE_IDX + SP_SMITE]
   if ( smite_spell <= 0 )
      return NO_DMG 

   if ( p_mana[enemy] >= priest_spmana[SP_SMITE] ){
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( id, PF_HOLY )
      new smite_dmg = 0

      new rnum = random_num( 0, 100 )
      new chance = floatround( ( p_int[enemy] + p_holy[enemy] - victim_resist ) / 2.0 )

      //client_print( enemy, print_chat, "smite> id=(%d) rnum=(%d) chance=(%d)",
      //               enemy, rnum, chance )

      if ( rnum < chance ){
         smite_dmg = DMG_SMITE_BASE + smite_spell

         //client_print( enemy, print_chat, "Smite doing extra dmg=(%d)", smite_dmg )

         p_casts[enemy] += 1
         p_casts_s[enemy] += 1
         p_mana[enemy] -= priest_spmana[SP_SMITE]

         client_print( id, print_chat, "[%s] %s smites you", MOD, name2 )
         client_print( enemy, print_chat, "[%s] You have smote %s", MOD, name1 )

         return smite_dmg
      }
   }

   return NO_DMG 
}


// **************************************************************************
// BEGIN wowmod_dmgspell_scorch subroutine
// Conditions handler for scorch damage spell
// **************************************************************************
public wowmod_dmgspell_scorch( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new scorch_spell = p_spells[enemy][SPELL_MAGE_BASE_IDX + SP_SCORCH]
   if ( scorch_spell <= 0 )
      return NO_DMG

   if ( p_mana[enemy] >= mage_spmana[SP_SCORCH] ){
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( id, PF_FIRE )
      new scorch_dmg = 0

      new rnum = random_num( 0, 100 )
      new chance = floatround( ( p_fire[enemy] + p_int[enemy] - victim_resist ) / 2.0 )

      //client_print( enemy, print_chat, "scorch> id=(%d) rnum=(%d) chance=(%d)",
      //              enemy, rnum, chance )

      if ( rnum < chance ){
         scorch_dmg = DMG_SCORCH_BASE + scorch_spell

         // Award extra dmg for masters in scorch
         if ( scorch_spell == mage_spmax[SP_SCORCH] )
            scorch_dmg += DMG_SCORCH_BASE

         //client_print( enemy, print_chat, 
         //            "Scorch doing extra dmg=(%d)", scorch_dmg )

         wowmod_effect_burn( id )

         p_casts[enemy] += 1
         p_casts_s[enemy] += 1
         p_mana[enemy] -= mage_spmana[SP_SCORCH]

         client_print( id, print_chat, "[%s] %s causes your skin to scorch", 
                       MOD, name2 )
         client_print( enemy, print_chat, "[%s] Your scorch burns %s", MOD, name1 )

         return scorch_dmg
      }
   }

   return NO_DMG 
}


// **************************************************************************
// BEGIN wowmod_dmgspell_flamestrike subroutine
// Conditions handler for flamestrike damage spell
// **************************************************************************
public wowmod_dmgspell_flamestrike( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD )
      return PLUGIN_HANDLED
   }

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   new fs_spell = p_spells[id][SPELL_MAGE_BASE_IDX + SP_FLAMESTRIKE]
   if ( fs_spell <= 0 )
      return NO_DMG

   if ( p_mana[id] < ( mage_spmana[SP_FLAMESTRIKE] + ( fs_spell * SPELL_MANA_RANK2 )  ) ){
      client_print( id, print_chat, 
         "[%s] You do not have enough mana to cast Flamestrike", MOD )
      return PLUGIN_HANDLED
   }

   new name[32], tname[32]
   get_user_name( id, name, 31 )

   p_mana[id] -= ( mage_spmana[SP_FLAMESTRIKE] + ( fs_spell * SPELL_MANA_RANK2 )  )
   p_casts[id] += 1
   p_casts_s[id] += 1

   new numplayers
   new players[32]
   new target[3]

   get_user_origin( id, target, 3 )

   get_players( players, numplayers )

   client_print( id, print_chat, "[%s] You call down a pillar of flames", MOD )

   wowmod_sound_playsound( id, SOUND_FLAMESTRIKE )

   wowmod_effect_sprite( target, blast )

   for ( new i = 0; i < numplayers; i++ ){
      new tid = players[i]
      new torigin[3]
      get_user_origin( tid, torigin )

      if ( get_user_team( id ) == get_user_team( tid ) )
         continue

      new distance = get_distance( target, torigin )         

      get_user_name( tid, tname, 31 )

      if ( distance >= FLAMESTRIKE_RADIUS )
         continue

      wowmod_sound_playsound( tid, SOUND_FLAMESTRIKE )

      new victim_resist = wowmod_calc_resist( tid, PF_FIRE )
      new rnum = random_num( 0, 100 )

      if ( rnum < ( p_fire[id] + p_int[id] - victim_resist ) / 2.0 ){
         client_print( id, print_chat, "[%s] Your Flamestrike burns %s", MOD, tname )
         client_print( tid, print_chat, "[%s] You are burned by a Flamestrike cast by %s", 
            MOD, name )

         new spell_dmg = DMG_FLAMESTRIKE_BASE + fs_spell

         client_print( id, print_chat, "fstrike> target=(%s) dmg=(%d)", tname, spell_dmg )
         wowmod_effect_burn( tid )
         apply_damage( tid, id, spell_dmg, CLASS_MAGE, SP_FLAMESTRIKE, "FlameStrike" )
      }else{
         client_print( id, print_chat, "[%s] Your Flamestrike is resisted by %s", MOD, tname )
         client_print( tid, print_chat, "[%s] You resist the effects of a Flamestrike cast by %s", MOD, name )
      }

   }

   return NO_DMG 
}


// **************************************************************************
// BEGIN wowmod_dmgspell_blizzard subroutine
// Conditions handler for blizzard damage spell
// **************************************************************************
public wowmod_dmgspell_blizzard( id ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD )
      return PLUGIN_HANDLED
   }

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   new bliz_spell = p_spells[id][SPELL_MAGE_BASE_IDX + SP_BLIZZARD]
   if ( bliz_spell <= 0 )
      return NO_DMG

   if ( p_mana[id] < ( mage_spmana[SP_BLIZZARD] + ( bliz_spell * SPELL_MANA_RANK2 )  ) ){
      client_print( id, print_chat, 
         "[%s] You do not have enough mana to cast Blizzard", MOD )
      return PLUGIN_HANDLED
   }

   new name[32], tname[32]
   get_user_name( id, name, 31 )

   p_mana[id] -= ( mage_spmana[SP_BLIZZARD] + ( bliz_spell * SPELL_MANA_RANK2 )  )
   p_casts[id] += 1
   p_casts_s[id] += 1

   new numplayers
   new players[32]
   new target[3]

   get_user_origin( id, target, 3 )

   get_players( players, numplayers )

   client_print( id, print_chat, "[%s] You invoke a Blizzard", MOD )

   wowmod_sound_playsound( id, SOUND_BLIZZARD )

   wowmod_effect_sprite( target, blizzard )

   for ( new i = 0; i < numplayers; i++ ){
      new tid = players[i]
      new torigin[3]
      get_user_origin( tid, torigin )

      if ( get_user_team( id ) == get_user_team( tid ) )
         continue

      new distance = get_distance( target, torigin )         

      get_user_name( tid, tname, 31 )

      if ( distance >= BLIZZARD_RADIUS )
         continue

      wowmod_sound_playsound( tid, SOUND_BLIZZARD )

      new victim_resist = wowmod_calc_resist( tid, PF_FROST )

      // Add in bonus resist for Orcs
      if ( p_race[tid] == RACE_ORC )
         victim_resist += RACIAL_ORC_RESIST

      new rnum = random_num( 0, 100 )

      if ( rnum < ( p_frost[id] + p_int[id] - victim_resist ) / 2.0 ){
         client_print( id, print_chat, "[%s] Your Blizzard chills %s", MOD, tname )
         client_print( tid, print_chat, "[%s] You are chilled by a Blizzard cast by %s", 
            MOD, name )

         blizzardon[tid] = true 

         new spell_dmg = DMG_BLIZZARD_BASE + bliz_spell

         client_print( id, print_chat, "blizzard> target=(%s) dmg=(%d)", tname, spell_dmg )
         //wowmod_effect_burn( tid )
         apply_damage( tid, id, spell_dmg, CLASS_MAGE, SP_BLIZZARD, "Blizzard" )

         set_user_maxspeed( tid, BLIZZARD_SPEED )

         new args[2]
         args[0] = tid
         new Float:ftimer = BLIZZARD_BASE_TIMER + ( bliz_spell * BLIZZARD_BASE_TIMER )
         set_task( ftimer, "wowmod_unbuff_blizzard", TASK_BLIZZARD+tid, args, 2 );
      }else{
         client_print( id, print_chat, "[%s] Your Blizzard is resisted by %s", MOD, tname )
         client_print( tid, print_chat, "[%s] You resist the effects of a Blizzard cast by %s", MOD, name )
      }

   }

   return NO_DMG 
}


// **************************************************************************
// BEGIN wowmod_dmgspell_ssting subroutine
// Conditions handler for serpent sting damage spell
// **************************************************************************
public wowmod_dmgspell_ssting( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new ssting_spell = p_spells[enemy][SPELL_HUNTER_BASE_IDX + SP_SSTING]
   if ( ssting_spell <= 0 )
      return NO_DMG

   new cast_mana = hunter_spmana[SP_SSTING] + ( SPELL_MANA_RANK3 * ssting_spell )

   if ( !serpstingon[id] && ( p_mana[enemy] >= cast_mana ) ){ 
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( id, PF_NATURE )

      if ( has_stoneform[id] ){
         client_print( id, print_chat, "[%s] Your Stoneform protects you from a Serpent Sting",
            MOD, name2 )
         client_print( enemy, print_chat, "[%s] Stoneform protects %s from your Serpent Sting",
            MOD, name1 )
         return NO_DMG
      }

      new rnum = random_num( 0, 100 )

      if ( rnum < floatround( ( p_int[enemy] +  p_nature[enemy] - victim_resist ) / 4.0 ) ){
         serpstingon[id] = true
         client_print( id, print_chat, "[%s] %s hits you with the Serpent's Sting",
                        MOD, name2 )
         client_print( enemy, print_chat, 
                        "[%s] Your Serpent Sting takes hold on %s", MOD, name1 )

         p_casts[enemy] += 1
         p_casts_s[enemy] += 1
         p_mana[enemy] -= cast_mana 

         new Float:ftimer = SSTING_BASE_TIMER - ( 0.20 * ssting_spell )
         new dmg = DMG_SSTING * ssting_spell
         log_amx( "Serpent Sting about to start dot damage %d, ftimer=(%f)", dmg, ftimer )
         new args[4]
         args[0] = id
         args[1] = enemy
         args[2] = 0
         args[3] = dmg
         set_task( ftimer, "wowmod_damage_ssting", TASK_SSTING+id, args, 4, "b" );
      }
   }
   return NO_DMG
}


// **************************************************************************
// BEGIN wowmod_dmgspell_wingclip subroutine
// Conditions handler for wingclip spell
// **************************************************************************
public wowmod_dmgspell_wingclip( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new wclip_spell = p_spells[enemy][SPELL_HUNTER_BASE_IDX + SP_WINGCLIP]
   if ( wclip_spell <= 0 )
      return NO_DMG

   new cast_mana = hunter_spmana[SP_WINGCLIP] + ( SPELL_MANA_RANK2 * wclip_spell )

   if ( !wingclipon[id] && ( p_mana[enemy] >= cast_mana ) ){ 
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( id, PF_MARKSMAN )

      // Add in bonus resist for Orcs
      if ( p_race[id] == RACE_ORC )
         victim_resist += RACIAL_ORC_RESIST

      new rnum = random_num( 0, 100 )
      new chance = floatround( ( p_int[enemy] + p_marksman[enemy] - victim_resist ) / 4.0 )

      //client_print( enemy, print_chat, 
      //              "wingclip> enemy=(%d) rnum=(%d) chance=(%d) vresist", 
      //              enemy, rnum, chance, victim_resist )

      if ( rnum < chance ){
         //Uncomment the next line for debug testing
         //id = enemy

         wingclipon[id] = true
         client_print( id, print_chat, "[%s] %s clips your wings, slowing you down",
                        MOD, name2 )
         client_print( enemy, print_chat, 
                        "[%s] Your Wing Clip slows down %s", MOD, name1 )
         
         p_casts[enemy] += 1
         p_casts_s[enemy] += 1
         p_mana[enemy] -= cast_mana

         new args[2]
         args[0] = id
         args[1] = floatround( p_maxspeed[id] )
         set_user_maxspeed( id, WINGCLIP_SPEED )
         new Float:ftimer = WINGCLIP_BASE_TIMER + ( wclip_spell * WINGCLIP_BASE_TIMER )
         set_task( ftimer, "wowmod_unbuff_wingclip", TASK_WINGCLIP+id, args, 2 );
      }
   }
   return NO_DMG
}


// **************************************************************************
// BEGIN wowmod_dmgspell_wrath subroutine
// Conditions handler for wrath damage spell, returns -1 .. dmg amount
// **************************************************************************
public wowmod_dmgspell_wrath( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new wrath_spell = p_spells[enemy][SPELL_DRUID_BASE_IDX + SP_WRATH]
   if ( wrath_spell <= 0 )
      return NO_DMG

   new cast_mana = druid_spmana[SP_WRATH] + ( SPELL_MANA_RANK * wrath_spell )

   if ( p_mana[enemy] >= cast_mana ){
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( id, PF_NATURE )

      //client_print( enemy, print_chat,  
      // "wrath> en_nat=(%d) en_int=(%d) id_nat=(%d)", 
      // p_nature[enemy], p_int[enemy], p_nature[id] )
      
      new rnum = random_num( 0, 100 )
      new chance = floatround( ( p_nature[enemy] + p_int[enemy] - victim_resist ) / 2.0 )
      //client_print( enemy, print_chat,  "wrath> id=(%d) rnum=(%d) chance=(%d)", 
      // enemy, rnum, chance )

      if ( rnum < chance ){
         new wrath_dmg = DMG_WRATH_BASE + wrath_spell

         // Award extra dmg for masters in wrath
         if ( wrath_spell == druid_spmax[SP_WRATH] )
            wrath_dmg += DMG_WRATH_BASE

         wowmod_sound_playsound( id, SOUND_WRATH )

         //wowmod_effect_attachsprite( id, wrath, 0 )

         p_casts[enemy] += 1
         p_casts_s[enemy] += 1
         p_mana[enemy] -= cast_mana 

         client_print( id, print_chat, "[%s] %s unleashes Wrath on you", MOD, name2 )
         client_print( enemy, print_chat, "[%s] Your Wrath is unleashed on %s", MOD, name1 )

         return wrath_dmg
      }
   }

   return NO_DMG 
}


// **************************************************************************
// BEGIN wowmod_dmgspell_thorns subroutine
// Conditions handler for thorns damage spell, returns -1 .. dmg amount
// **************************************************************************
public wowmod_dmgspell_thorns( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new thorns_spell = p_spells[id][SPELL_DRUID_BASE_IDX + SP_THORNS]
   if ( thorns_spell <= 0 )
      return NO_DMG

   new thorns_dmg = 0

   new cast_mana = druid_spmana[SP_THORNS] + ( SPELL_MANA_RANK * thorns_spell )

   if ( p_mana[id] >= cast_mana ){
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      //new victim_resist = wowmod_calc_resist( enemy, PF_NATURE )

      thorns_dmg = DMG_THORNS_BASE + thorns_spell
 
      //client_print( id, print_chat,  "thorns> enemyid=(%d) rdiff=(%d) tdm=(%d)", 
      //   enemy, resist_diff, thorns_dmg )

      p_casts[id] += 1
      p_casts_s[id] += 1
      p_mana[id] -= cast_mana 

      client_print( id, print_chat, "[%s] Your Thorns cause damage to %s", MOD, name2 )
      client_print( enemy, print_chat, "[%s] You are injured by the Thorns protecting %s", MOD, name1 )

   }

   return thorns_dmg 
}


// **************************************************************************
// BEGIN wowmod_dmgspell_cheapshot subroutine
// Conditions handler for cheap shot damage spell, returns -1 .. dmg amount
// **************************************************************************
public wowmod_dmgspell_cheapshot( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new cheap_spell = p_spells[enemy][SPELL_ROGUE_BASE_IDX + SP_CHEAPSHOT]
   if ( cheap_spell <= 0 )
      return NO_DMG

   new cast_mana = rogue_spenergy[SP_CHEAPSHOT] + ( SPELL_MANA_RANK * cheap_spell )

   if ( p_energy[enemy] >= cast_mana ){
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( id, PF_MELEE )
      new cheap_dmg = 0

      new rnum = random_num( 0, 100 )
      new chance = floatround( ( p_agi[enemy] + p_melee[enemy] - victim_resist ) / 4.0 )

      //client_print( enemy, print_chat, "cheap> id=(%d) rnum=(%d) chance=(%d)", enemy, rnum, chance )
      //client_print( enemy, print_chat, "cheap> agi=(%d) mel_en=(%d) mel_id=(%d)",
      //               p_agi[enemy], p_melee[enemy], victim_resist )

      if ( rnum < chance ){
         cheap_dmg = DMG_CHEAPSHOT_BASE + cheap_spell

         // Award extra dmg for masters in Cheap shot
         if ( cheap_spell == rogue_spmax[SP_CHEAPSHOT] )
            cheap_dmg += DMG_CHEAPSHOT_BASE

         //client_print( enemy, print_chat, "Cheapshot doing extra dmg=(%d)", cheap_dmg )

         p_casts[enemy] += 1
         p_casts_s[enemy] += 1
         p_energy[enemy] -= cast_mana 
         p_energy[enemy] += cheap_dmg

         cheapshoton[id] = true

         client_print( id, print_chat, "[%s] %s hits you with a Cheap Shot", MOD, name2 )
         client_print( enemy, print_chat, "[%s] You hit %s with a Cheap Shot", MOD, name1 )

         new args[2]
         args[0] = id
         args[1] = floatround( p_maxspeed[id] )
         set_user_maxspeed( id, CHEAPSHOT_SPEED )
         new Float:ftimer = CHEAPSHOT_BASE_TIMER + cheap_spell
         set_task( ftimer, "wowmod_unbuff_cheapshot", TASK_CHEAPSHOT+id, args, 2 );

         return cheap_dmg
      }
   }   

   return NO_DMG 
}


// **************************************************************************
// BEGIN wowmod_dmgspell_rupture subroutine
// Conditions handler for rogue rupture spell
// **************************************************************************
public wowmod_dmgspell_rupture( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   new rup_spell = p_spells[enemy][SPELL_ROGUE_BASE_IDX + SP_RUPTURE]
   if ( rup_spell <= 0 )
      return NO_DMG

   new cast_mana = rogue_spenergy[SP_RUPTURE]

   if ( !ruptureon[id] && ( p_energy[enemy] >= cast_mana ) ){ 
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( id, PF_MELEE )

      if ( has_stoneform[id] ){
         client_print( id, print_chat, "[%s] Your Stoneform protects you from Rupture",
            MOD )
         client_print( enemy, print_chat, "[%s] Stoneform protects %s from your Rupture",
            MOD, name1 )
         return NO_DMG
      }

      new rnum = random_num( 0, 100 )

      //client_print( enemy, print_chat, "rupture> rnum=(%d) chance=(%d)",
      //               rnum, floatround( ( p_agi[enemy] +  p_melee[enemy] - victim_resist ) / 4.0 )  )

      if ( rnum < floatround( ( p_agi[enemy] +  p_melee[enemy] - victim_resist ) / 4.0 ) ){
         ruptureon[id] = true
         client_print( id, print_chat, "[%s] %s ruptures a blood vessel, you begin to bleed out...",
                        MOD, name2 )
         client_print( enemy, print_chat, 
                        "[%s] You Rupture %s, causing bleed damage", MOD, name1 )

         p_casts[enemy] += 1
         p_casts_s[enemy] += 1
         p_energy[enemy] -= cast_mana 

         new Float:ftimer = RUPTURE_BASE_TIMER - ( 0.20 * rup_spell )
         new dmg = DMG_RUPTURE * rup_spell
         //log_amx( "Rupture about to start dot damage %d, ftimer=(%f)", dmg, ftimer )
         new args[5]
         args[0] = id
         args[1] = enemy
         args[2] = 0
         args[3] = dmg
         args[4] = SP_RUPTURE
         set_task( ftimer, "wowmod_damage_rupture", TASK_RUPTURE+id, args, 5, "b" );
      }
   }
   return NO_DMG
}


// **************************************************************************
// BEGIN wowmod_dmgspell_eviscerate subroutine
// Rogue Eviscerate spell
// **************************************************************************
public wowmod_dmgspell_eviscerate( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new evis_spell = p_spells[id][SPELL_ROGUE_BASE_IDX + SP_EVISCERATE]
   if ( evis_spell <= 0 )
      return PLUGIN_HANDLED 

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   if( u_delay ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may cast this spell only after a %d second delay", get_cvar_num("FT_ultimatedelay"))
      return PLUGIN_HANDLED
   }

   if ( eviscerateon[id] ){
      client_print( id, print_chat, "[%s] You are already preparing to Eviscerate your enemy", MOD )
      return PLUGIN_CONTINUE
   }

   new cast_mana = rogue_spenergy[SP_EVISCERATE]
  
   if ( p_energy[id] < cast_mana ){
      client_print( id, print_chat, "[%s] You do not have energy to Eviscerate your enemy", MOD )
      return PLUGIN_CONTINUE
   }

   p_energy[id] -= cast_mana
   p_casts[id] += 1 
   p_casts_s[id] += 1 

   eviscerateon[id] = true

   client_print( id, print_chat, "[%s] You prepare your body for a chance to Eviscerate your next opponent",
      MOD )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_dmgspell_drainlife subroutine
// Conditions handler for drain life damage spell, returns -1 .. dmg amount
// **************************************************************************
public wowmod_dmgspell_drainlife( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new dlife_spell = p_spells[enemy][SPELL_WARLOCK_BASE_IDX + SP_DRAINLIFE]
   if ( dlife_spell <= 0 )
      return NO_DMG

   new cast_mana = warlock_spmana[SP_DRAINLIFE] + ( SPELL_MANA_RANK * dlife_spell )

   if ( p_mana[enemy] >= cast_mana ){
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( id, PF_SHADOW )
      new dlife_dmg = 0

      new rnum = random_num( 0, 100 )
      new chance = floatround( ( p_int[enemy] + p_shadow[enemy] - victim_resist ) / 2.0 )

      //client_print( enemy, print_chat, "drainlife> id=(%d) rnum=(%d) chance=(%d)", enemy, rnum, chance )
      //client_print( enemy, print_chat, "drainlife> int=(%d) shad_en=(%d) shad_id=(%d)",
      //               p_shadow[enemy], p_shadow[enemy], victim_resist )

      if ( rnum < chance ){
         dlife_dmg = DMG_DRAINLIFE_BASE + dlife_spell

         //client_print( enemy, print_chat, "Drainlife doing extra dmg=(%d)", dlife_dmg )

         p_casts[enemy] += 1
         p_casts_s[enemy] += 1
         p_mana[enemy] -= cast_mana

         client_print( id, print_chat, "[%s] %s drains your life", MOD, name2 )
         client_print( enemy, print_chat, "[%s] You drain life from %s", MOD, name1 )

         return dlife_dmg
      }
   }   

   return NO_DMG 
}


// **************************************************************************
// BEGIN wowmod_dmgspell_weakness subroutine
// Conditions handler for weakness damage spell, returns -1 .. dmg amount
// **************************************************************************
public wowmod_dmgspell_weakness( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new weak_spell = p_spells[enemy][SPELL_WARLOCK_BASE_IDX + SP_CURSEWEAK]
   if ( weak_spell <= 0 )
      return NO_DMG

   new cast_mana = warlock_spmana[SP_CURSEWEAK] + ( SPELL_MANA_RANK2 * weak_spell )

   if ( !isweakened[id] && ( p_mana[enemy] >= cast_mana )){
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( id, PF_SHADOW )

      new rnum = random_num( 0, 100 )
      new chance = floatround( ( p_int[enemy] + p_shadow[enemy] - victim_resist ) / 2.0 )

      //client_print( enemy, print_chat, "weakness> id=(%d) rnum=(%d) chance=(%d)",
      //               enemy, rnum, chance )

      if ( rnum < chance ){
         p_casts[enemy] += 1
         p_casts_s[enemy] += 1
         p_mana[enemy] -= cast_mana 

         isweakened[id] = true

         new Float:ftimer = SPELL_BASE_TIMER + ( 5.0 * weak_spell )
         new args[2]
         args[0] = id
         args[1] = enemy
         set_task( ftimer, "wowmod_unbuff_weakness", TASK_WEAKNESS+id, args, 2 );

         client_print( id, print_chat, "[%s] %s curses you with weakness", MOD, name2 )
         client_print( enemy, print_chat, "[%s] You curse %s with weakness", MOD, name1 )
      }
   }
   return NO_DMG

}


// **************************************************************************
// BEGIN wowmod_dmgspell_drainmana subroutine
// Conditions handler for drainmana damage spell, returns -1 .. dmg amount
// **************************************************************************
public wowmod_dmgspell_drainmana( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new drainm_spell = p_spells[enemy][SPELL_WARLOCK_BASE_IDX + SP_DRAINMANA]
   if ( drainm_spell <= 0 )
      return NO_DMG

   new cast_mana = warlock_spmana[SP_DRAINMANA] + ( SPELL_MANA_RANK * drainm_spell )

   if ( ( p_mana[id] > 0 ) && ( p_mana[enemy] >= cast_mana )){
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( id, PF_SHADOW )

      new rnum = random_num( 0, 100 )
      new chance = floatround( ( p_int[enemy] + p_shadow[enemy] - victim_resist ) / 2.0 )

      //client_print( enemy, print_chat, "drainmana> id=(%d) rnum=(%d) chance=(%d)",
      //               enemy, rnum, chance )

      if ( rnum < chance ){
         new drainm_amt = DMG_DRAINMANA_BASE + ( 25 * drainm_spell )

         //client_print( enemy, print_chat, "DrainMana taking mana amt=(%d)", drainm_amt )

         p_casts[enemy] += 1
         p_casts_s[enemy] += 1
         p_mana[enemy] += drainm_amt - cast_mana
         p_mana[id] -= drainm_amt

         client_print( id, print_chat, "[%s] %s drains your mana", MOD, name2 )
         client_print( enemy, print_chat, "[%s] You drain mana from %s", MOD, name1 )
      }
   }
   return NO_DMG
}


// **************************************************************************
// BEGIN wowmod_dmgspell_corruption subroutine
// Conditions handler for warlock spell Corruption
// **************************************************************************
public wowmod_dmgspell_corruption( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new cor_spell = p_spells[enemy][SPELL_WARLOCK_BASE_IDX + SP_CORRUPTION]
   if ( cor_spell <= 0 )
      return NO_DMG

   new cast_mana = warlock_spmana[SP_CORRUPTION] + ( SPELL_MANA_RANK2 * cor_spell )

   if ( !corruptionon[id] && ( p_mana[enemy] >= cast_mana ) ){ 
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( id, PF_SHADOW )

      if ( has_stoneform[id] ){
         client_print( id, print_chat, "[%s] Your Stoneform protects you from Corruption",
            MOD, name2 )
         client_print( enemy, print_chat, "[%s] Stoneform protects %s from your Corruption",
            MOD, name1 )
         return NO_DMG
      }

      new rnum = random_num( 0, 100 )

      if ( rnum < floatround( ( p_int[enemy] + p_shadow[enemy] - victim_resist ) / 4.0 ) ){
         corruptionon[id] = true
         client_print( id, print_chat, "[%s] %s hits you with Corruption, you are diseased" ,
                        MOD, name2 )
         client_print( enemy, print_chat, 
                        "[%s] Your Corruption afflicts %s with disease", MOD, name1 )

         p_casts[enemy] += 1
         p_casts_s[enemy] += 1
         p_mana[enemy] -= cast_mana

         new Float:ftimer = CORRUPT_BASE_TIMER - ( 0.20 * cor_spell )
         new dmg = DMG_CORRUPT
         log_amx( "Corruption about to start dot damage %d, ftimer=(%f)", dmg, ftimer )
         new args[4]
         args[0] = id
         args[1] = enemy
         args[2] = 0
         args[3] = dmg
         set_task( ftimer, "wowmod_damage_corrupt", TASK_CORRUPT+id, args, 4, "b" );
      }
   }
   return NO_DMG
}


// **************************************************************************
// BEGIN wowmod_autocast subroutine
// Autocast routine
// **************************************************************************
public wowmod_autocast( parm[] ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new id = parm[0]
   new cmd[64]

   for ( new j = 0; j < 8; j++){
      if ( p_class1autocast[id][j] ){
         switch( p_class1[id] ){
            case CLASS_DRUID: format( cmd, 63, "say_team cast %s", druid_saycmds[j] )
            case CLASS_HUNTER: format( cmd, 63,  "say_team cast %s", hunter_saycmds[j] )
            case CLASS_MAGE: format( cmd, 63, "say_team cast %s", mage_saycmds[j] )
            case CLASS_ROGUE: format( cmd, 63, "say_team cast %s", rogue_saycmds[j] )
            case CLASS_WARLOCK: format( cmd, 63, "say_team cast %s", warlock_saycmds[j] )
            case CLASS_PRIEST: format( cmd, 63, "say_team cast %s", priest_saycmds[j] )
         }
         client_print( id, print_console, "[%s] Autocasting: %s", MOD, cmd )
         client_cmd( id, cmd )
      }
   }

   if ( p_class2[id] < CLASS_DRUID )
      return PLUGIN_CONTINUE

   for ( new j = 0; j < 8; j++){
      if ( p_class2autocast[id][j] ){
         switch( p_class2[id] ){
            case CLASS_DRUID: format( cmd, 63, "say_team cast %s", druid_saycmds[j] )
            case CLASS_HUNTER: format( cmd, 63,  "say_team cast %s", hunter_saycmds[j] )
            case CLASS_MAGE: format( cmd, 63, "say_team cast %s", mage_saycmds[j] )
            case CLASS_ROGUE: format( cmd, 63, "say_team cast %s", rogue_saycmds[j] )
            case CLASS_WARLOCK: format( cmd, 63, "say_team cast %s", warlock_saycmds[j] )
            case CLASS_PRIEST: format( cmd, 63, "say_team cast %s", priest_saycmds[j] )
         }
         client_print( id, print_console, "[%s] Autocasting: %s", MOD, cmd )
         client_cmd( id, cmd )
      }
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_cast subroutine
// Frontend for spell-casting system
// **************************************************************************
public wowmod_cast( id, spell[] ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new caster[32]
   new class = -1
   new spellname[32]
   new cast_mana = 0
   new spell_index = -1
   new spell_level = 0
   new bool:bValid = true
   new spell_is_specabil = 0 

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD)
      return PLUGIN_HANDLED
   }

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   spell_is_specabil = wowmod_cast_specabil( id, spell )

   if ( spell_is_specabil == 1 )
      return PLUGIN_CONTINUE

   // Next check for normal class-based spell casts

   if ( equali( spell, "arcint" ) ){
      class = CLASS_MAGE
      copy( caster, 31, "Mage" )
      copy( spellname, 31, "Arcane Intellect" )
      spell_index = SPELL_MAGE_BASE_IDX + SP_ARCINT
      spell_level = p_spells[id][spell_index]
      cast_mana = mage_spmana[SP_ARCINT] * spell_level 
   }else if ( equali( spell, "brilliance" ) ){
      class = CLASS_MAGE
      copy( caster, 31, "Mage" )
      copy( spellname, 31, "Arcane Brilliance" )
      spell_index = SPELL_MAGE_BASE_IDX + SP_ARCBRIL
      spell_level = p_spells[id][spell_index]
      cast_mana = mage_spmana[SP_ARCBRIL] * spell_level 
   }else if ( equali( spell, "tranquility" ) || equali( spell, "tranq" ) ){
      class = CLASS_DRUID
      copy( caster, 31, "Druid" )
      copy( spellname, 31, "Tranquility" )
      spell_index = SPELL_DRUID_BASE_IDX + SP_TRANQ
      spell_level = p_spells[id][spell_index]
      cast_mana = druid_spmana[SP_TRANQ] * spell_level 
   }else if ( equali( spell, "giftwild" ) ){
      class = CLASS_DRUID
      copy( caster, 31, "Druid" )
      copy( spellname, 31, "Gift of the Wild" )
      spell_index = SPELL_DRUID_BASE_IDX + SP_GIFTWILD
      spell_level = p_spells[id][spell_index]
      cast_mana = druid_spmana[SP_GIFTWILD] + ( SPELL_MANA_RANK * spell_level )
   }else if ( equali( spell, "barkskin" ) ){
      class = CLASS_DRUID
      copy( caster, 31, "Druid" )
      copy( spellname, 31, "Barkskin" )
      spell_index = SPELL_DRUID_BASE_IDX + SP_BARKSKIN
      spell_level = p_spells[id][spell_index]
      cast_mana = druid_spmana[SP_BARKSKIN] + ( SPELL_MANA_RANK * spell_level )
   }else if ( equali( spell, "pack" ) ){
      class = CLASS_HUNTER
      copy( caster, 31, "Hunter" )
      copy( spellname, 31, "Aspect of the Pack" )
      spell_index = SPELL_HUNTER_BASE_IDX + SP_APACK
      spell_level = p_spells[id][spell_index]
      cast_mana = hunter_spmana[SP_APACK] + ( SPELL_MANA_RANK * spell_level )
   }else if ( equali( spell, "wild" ) ){
      class = CLASS_HUNTER
      copy( caster, 31, "Hunter" )
      copy( spellname, 31, "Aspect of the Wild" )
      spell_index = SPELL_HUNTER_BASE_IDX + SP_AWILD
      spell_level = p_spells[id][spell_index]
      cast_mana = hunter_spmana[SP_AWILD] + ( SPELL_MANA_RANK * spell_level )
   }else if ( equali( spell, "vanish" ) ){
      class = CLASS_ROGUE
      copy( caster, 31, "Rogue" )
      copy( spellname, 31, "Vanish" )
      spell_index = SPELL_ROGUE_BASE_IDX + SP_VANISH
      spell_level = p_spells[id][spell_index]
      cast_mana = rogue_spenergy[SP_VANISH]
   }else if ( equali( spell, "sprint" ) ){
      class = CLASS_ROGUE
      copy( caster, 31, "Rogue" )
      copy( spellname, 31, "Sprint" )
      spell_index = SPELL_ROGUE_BASE_IDX + SP_SPRINT
      spell_level = p_spells[id][spell_index]
      cast_mana = rogue_spenergy[SP_SPRINT]
   }else if ( equali( spell, "evasion" ) ){
      class = CLASS_ROGUE
      copy( caster, 31, "Rogue" )
      copy( spellname, 31, "Evasion" )
      spell_index = SPELL_ROGUE_BASE_IDX + SP_EVASION
      spell_level = p_spells[id][spell_index]
      cast_mana = rogue_spenergy[SP_EVASION]
   }else if ( equali( spell, "magearmor" ) ){
      class = CLASS_MAGE
      copy( caster, 31, "Mage" )
      copy( spellname, 31, "Mage Armor" )
      spell_index = SPELL_MAGE_BASE_IDX + SP_MAGEARMOR
      spell_level = p_spells[id][spell_index]
      cast_mana = mage_spmana[SP_MAGEARMOR] + ( SPELL_MANA_RANK * spell_level )
   }else if ( equali( spell, "demonskin" ) || equali( spell, "demon skin" ) ){
      class = CLASS_WARLOCK
      copy( caster, 31, "Warlock" )
      copy( spellname, 31, "Demon Skin" )
      spell_index = SPELL_WARLOCK_BASE_IDX + SP_DEMONSKIN
      spell_level = p_spells[id][spell_index]
      cast_mana = warlock_spmana[SP_DEMONSKIN] + ( SPELL_MANA_RANK * spell_level )
   }else if ( equali( spell, "lifetap" ) || equali( spell, "life tap" ) ){
      class = CLASS_WARLOCK
      copy( caster, 31, "Warlock" )
      copy( spellname, 31, "Lifetap" )
      spell_index = SPELL_WARLOCK_BASE_IDX + SP_LIFETAP
      spell_level = p_spells[id][spell_index]
      cast_mana = warlock_spmana[SP_LIFETAP] + ( SPELL_MANA_RANK * spell_level )
   }else if ( equali( spell, "shield" ) ){
      class = CLASS_PRIEST
      copy( caster, 31, "Priest" )
      copy( spellname, 31, "Power Word: Shield" )
      spell_index = SPELL_PRIEST_BASE_IDX + SP_PWSHIELD
      spell_level = p_spells[id][spell_index]
      cast_mana = priest_spmana[SP_PWSHIELD] + ( SPELL_MANA_RANK * spell_level )
   }else if ( equali( spell, "heal" ) ){
      class = CLASS_PRIEST
      copy( caster, 31, "Priest" )
      copy( spellname, 31, "Heal" )
      spell_index = SPELL_PRIEST_BASE_IDX + SP_HEAL
      spell_level = p_spells[id][spell_index]
      cast_mana = priest_spmana[SP_HEAL] + ( SPELL_MANA_RANK * spell_level )
   }else if ( equali( spell, "prayer" ) ){
      class = CLASS_PRIEST
      copy( caster, 31, "Priest" )
      copy( spellname, 31, "Prayer of Healing" )
      spell_index = SPELL_PRIEST_BASE_IDX + SP_PRAYERHEAL
      spell_level = p_spells[id][spell_index]
      cast_mana = priest_spmana[SP_PRAYERHEAL] + ( SPELL_MANA_RANK2 * spell_level )
   }else if ( equali( spell, "res" ) || equali( spell, "rez" ) ){
      class = CLASS_PRIEST
      copy( caster, 31, "Priest" )
      copy( spellname, 31, "Resurrection" )
      spell_index = SPELL_PRIEST_BASE_IDX + SP_RESURRECT
      spell_level = p_spells[id][spell_index]
      cast_mana = priest_spmana[SP_RESURRECT] + ( SPELL_MANA_RANK2 * spell_level )
   }else{
      bValid = false
   }

   if ( !bValid ){
      client_print(id, print_chat,
         "[%s] Invalid spell cast attempted (%s)", MOD, spell )
      return PLUGIN_HANDLED
   }

   if ( p_class1[id] != class && p_class2[id] != class ){
      client_print( id, print_chat, "[%s] Only a %s may cast that spell", MOD, caster )
      return PLUGIN_HANDLED
   }

   if ( spell_level <= 0 ){
      client_print(id, print_chat,
         "[%s] You have not yet learned the spell %s", MOD, spellname )
      return PLUGIN_HANDLED
   }

   if ( ( class == CLASS_ROGUE ) && ( p_energy[id] < cast_mana ) ){
      client_print(id, print_chat,
         "[%s] You do not have enough energy for the spell %s", MOD, spellname )
      return PLUGIN_HANDLED
   }else if ( p_mana[id] < cast_mana ){
      client_print(id, print_chat,
         "[%s] You do not have enough mana to cast %s", MOD, spellname )
      return PLUGIN_HANDLED
   }

   // If we made it this far we can call the spell's subroutine

   // Increment player casts stat data
   p_casts[id] += 1
   p_casts_s[id] += 1

   new origin[3]
   get_user_origin( id, origin )
   wowmod_sound_playsound( id, SOUND_SPELLCAST )
   wowmod_effect_spellcast( origin )

   if ( equali( spell, "arcint" ) ){
      wowmod_spell_arcint( id, spell_level, cast_mana )
   }else if ( equali( spell, "tranquility" ) || equali( spell, "tranq" ) ){
      wowmod_spell_tranq( id, spell_level, cast_mana )
   }else if ( equali( spell, "giftwild" ) ){
      wowmod_spell_giftwildself( id, spell_level, cast_mana )
   }else if ( equali( spell, "barkskin" ) ){
      wowmod_spell_barkskin( id, spell_level, cast_mana )
   }else if ( equali( spell, "pack" ) ){
      wowmod_spell_apack( id, spell_level, cast_mana )
   }else if ( equali( spell, "wild" ) ){
      wowmod_spell_awild( id, spell_level, cast_mana )
   }else if ( equali( spell, "sprint" ) ){
      wowmod_spell_sprint( id, spell_level, cast_mana )
   }else if ( equali( spell, "vanish" ) ){
      wowmod_spell_vanish( id, spell_level, cast_mana )
   }else if ( equali( spell, "evasion" ) ){
      wowmod_spell_evasion( id, spell_level, cast_mana )
   }else if ( equali( spell, "magearmor" ) ){
      wowmod_spell_magearmor( id, spell_level, cast_mana )
   }else if ( equali( spell, "brilliance" ) ){
      wowmod_spell_brilliance( id, spell_level, cast_mana )
   }else if ( equali( spell, "demonskin" ) || equali( spell, "demon skin" ) ){
      wowmod_spell_demonskin( id, spell_level, cast_mana )
   }else if ( equali( spell, "lifetap" ) || equali( spell, "life tap" ) ){
      wowmod_spell_lifetap( id, spell_level, cast_mana )
   }else if ( equali( spell, "shield" ) ){
      wowmod_spell_pwshield( id, spell_level, cast_mana )
   }else if ( equali( spell, "heal" ) ){
      wowmod_spell_priestselfheal( id, spell_level, cast_mana )
   }else if ( equali( spell, "prayer" ) ){
      wowmod_spell_healprayer( id, spell_level, cast_mana )
   }else if ( equali( spell, "res" ) || equali( spell, "rez" ) ){
      wowmod_spell_resurrect( id, spell_level, cast_mana )
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_cast_specabil supporting routine
// Check for special ability cast
// **************************************************************************
public wowmod_cast_specabil( id, spell[] ){
   new magic_item_ok = 0      // If magic item found with designated specabil

   // First check for special abilities from magical equipment
   if ( equali( spell, "hearthstone" ) ){
      new ring1 = p_ring1[id]
      new ring2 = p_ring2[id]
      if ( ( ring1 > 0 ) && ( g_spec_idx[ring1] == SPA_HSTONE ) ){
         //client_print( id, print_chat, ">> Found worn ring %s", gear[ring1] ) 
         magic_item_ok = 1
      }
      if ( ( ring2 > 0 ) && ( g_spec_idx[ring2] == SPA_HSTONE ) ){
         //client_print( id, print_chat, ">> Found worn ring %s", gear[ring2] ) 
         magic_item_ok = 1
      }
      if ( magic_item_ok )
         wowmod_specabil_hearthstone( id )
   }else if ( equali( spell, "speed" ) ){
      new boots = p_feet[id]
      if ( ( boots > 0 ) && ( g_spec_idx[boots] == SPA_SPEED1 ) ){
         //client_print( id, print_chat, ">> Found worn boots %s", gear[boots] ) 
         magic_item_ok = 1
         wowmod_specabil_speed( id, SPECABIL_SPEED1_SPEED )
      }else if ( ( boots > 0 ) && ( g_spec_idx[boots] == SPA_SPEED2 ) ){
         //client_print( id, print_chat, ">> Found worn boots %s", gear[boots] ) 
         magic_item_ok = 1
         wowmod_specabil_speed( id, SPECABIL_SPEED2_SPEED )
      }else if ( ( boots > 0 ) && ( g_spec_idx[boots] == SPA_SPEED3 ) ){
         //client_print( id, print_chat, ">> Found worn boots %s", gear[boots] ) 
         magic_item_ok = 1
         wowmod_specabil_speed( id, SPECABIL_SPEED3_SPEED )
      }
   }

   return magic_item_ok
}


// **************************************************************************
// BEGIN wowmod_spell_healtouch supporting routine
// Druid Healing Touch spell
// **************************************************************************
public wowmod_spell_healtouch( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new spell_index = SPELL_DRUID_BASE_IDX + SP_HEALTOUCH

   if ( !p_spells[id][spell_index]){
      client_print( id, print_chat, "[%s] You have not learned the Healing Touch spell", MOD )
      return PLUGIN_HANDLED
   }

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   if( u_delay ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may cast this spell only after a %d second delay", get_cvar_num("FT_ultimatedelay"))
      return PLUGIN_HANDLED
   }

   if ( healdown[id] ){
      client_print( id, print_chat, "[%s] Your heal spell is not ready yet", MOD )
      return PLUGIN_HANDLED
   }

   new friend, body
   get_user_aiming(id, friend, body)

   if ( (0 < friend <= MAX_PLAYERS) && (friend != id) &&
       (get_user_team(id) == get_user_team(friend)) && is_user_alive(id) &&
        is_user_alive(friend))
   {
      // id has a friend to check for wounds

      // Mana spent to cast spell
      new cast_mana = druid_spmana[SP_HEALTOUCH] * p_spells[id][spell_index]

      //client_print( id, print_console,
      //   "sp_mana=(%d) cast_mana=(%d)",  druid_spmana[SP_HEALTOUCH], cast_mana )

      if ( p_mana[id] < cast_mana ){
         client_print(id, print_chat,
            "[%s] You do not have enough mana to cast Healing Touch", MOD )
         return PLUGIN_HANDLED
      }

      new origin[3]
      get_user_origin( id, origin )
      wowmod_sound_playsound( id, SOUND_SPELLCAST )
      wowmod_effect_spellcast( origin )

      new name[32], fname[32]
      get_user_name(id, name, 31)
      get_user_name(friend, fname, 31)

      if ( get_user_health(friend) >= p_maxhealth[friend] ){
         client_print(id, print_chat,
            "[%s] %s is not in need of healing right now", MOD, fname )
         return PLUGIN_HANDLED
      }

      get_user_origin( friend, origin )
      wowmod_effect_dlight( origin, 25, COLOR_WHITE, 50 )

      p_casts[id] += 1
      p_casts_s[id] += 1
      p_heals[id] += 1
      p_heals_s[id] += 1
      p_mana[id] -= cast_mana

      new hbonus = p_spells[id][spell_index] * HEALTHBONUS
      new xpbonus = p_spells[id][spell_index] * MENDXP 

      set_user_health_log( friend, get_user_health(friend) + hbonus, false, "wow_healtouch()" )

      if ( get_user_health(friend) > p_maxhealth[friend] )
         set_user_health_log( friend, p_maxhealth[friend], false, "wow_healtouch() p_maxhealth adjustment" )

      // Handle messages for healing touch
      client_print( id, print_chat, "[%s] You cast Healing Touch on %s, XP and GOLD bonuses earned", 
         MOD, fname )
      client_print( friend, print_chat, "[%s] %s has cast a healing spell on you", MOD, name )

      // Award the player XP for their service
      p_xp[id] += xpbonus
      p_gold[id] += GOLD_ASSIST

      wowmod_sound_playsound( id, SOUND_HEAL )
      wowmod_sound_playsound( friend, SOUND_HEAL )

      healdown[id] = true
      new parm[2]
      parm[0] = id
      new Float:ftimer = HEAL_COOLDOWN_TIMER 
      set_task( ftimer, "wowmod_heal_cooldown", TASK_HEALCOOL+id, parm, 2 )

   }else if (!is_user_alive(id)){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD)
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_spell_heal subroutine
// Priest Heal spell
// **************************************************************************
public wowmod_spell_heal( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new spell_index = SPELL_PRIEST_BASE_IDX + SP_HEAL

   if ( !p_spells[id][spell_index]){
      client_print( id, print_chat, "You have not learned the Heal spell" )
      return PLUGIN_HANDLED
   }

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   if( u_delay ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may cast this spell only after a %d second delay", get_cvar_num("FT_ultimatedelay"))
      return PLUGIN_HANDLED
   }

   if ( healdown[id] ){
      client_print( id, print_chat, "[%s] Your heal spell is not ready yet", MOD )
      return PLUGIN_HANDLED
   }


   // Mana spent to cast spell
   new cast_mana = priest_spmana[SP_HEAL] * p_spells[id][spell_index]

   //client_print( id, print_chat,
   //   "sp_mana=(%d) cast_mana=(%d)", priest_spmana[SP_HEAL], cast_mana )

   new friend, body
   get_user_aiming(id, friend, body)

   if ( (0 < friend <= MAX_PLAYERS) && (friend != id) &&
       (get_user_team(id) == get_user_team(friend)) && is_user_alive(id) &&
        is_user_alive(friend))
   {
      // id has a friend to check for wounds

      if ( p_mana[id] < cast_mana ){
         client_print(id, print_chat,
            "[%s] You do not have enough mana to cast Heal", MOD )
         return PLUGIN_HANDLED
      }

      new name[32], fname[32]
      get_user_name(id, name, 31)
      get_user_name(friend, fname, 31)

      if ( get_user_health(friend) >= p_maxhealth[friend] ){
         client_print(id, print_chat,
            "[%s] %s is not in need of healing right now", MOD, fname )
         return PLUGIN_HANDLED
      }

      new origin[3]
      get_user_origin( id, origin )
      wowmod_sound_playsound( id, SOUND_SPELLCAST )
      wowmod_effect_spellcast( origin )

      get_user_origin( friend, origin )
      wowmod_effect_dlight( origin, 25, COLOR_WHITE, 50 )

      p_casts[id] += 1
      p_casts_s[id] += 1
      p_heals[id] += 1
      p_heals_s[id] += 1
      p_mana[id] -= cast_mana

      new hbonus = p_spells[id][spell_index] * floatround( HEALTHBONUS * 1.33 )
      new xpbonus = p_spells[id][spell_index] * floatround( MENDXP * 1.33 )

      set_user_health_log( friend, get_user_health(friend) + hbonus, false, "wow_heal()" )

      if ( get_user_health(friend) > p_maxhealth[friend] )
         set_user_health_log( friend, p_maxhealth[friend], false, "wow_heal() p_maxhealth adjustment" )

      // Handle messages for healing touch
      client_print( id, print_chat, "[%s] You cast Heal on %s, XP and GOLD bonuses earned", MOD, fname )
      client_print( friend, print_chat, "[%s] %s has cast a healing spell on you", MOD, name )

      // Award the player XP for their service
      p_xp[id] += xpbonus
      p_gold[id] += GOLD_ASSIST 

      wowmod_sound_playsound( id, SOUND_HEAL )
      wowmod_sound_playsound( friend, SOUND_HEAL )

      healdown[id] = true
      new parm[2]
      parm[0] = id
      new Float:ftimer = HEAL_COOLDOWN_TIMER
      set_task( ftimer, "wowmod_heal_cooldown", TASK_HEALCOOL+id, parm, 2 )

   }else if (!is_user_alive(id)){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD)
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_spell_lesserheal subroutine
// Priest Lesser Heal spell
// **************************************************************************
public wowmod_spell_lesserheal( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new spell_index = SPELL_PRIEST_BASE_IDX + SP_LESSERHEAL

   if ( !p_spells[id][spell_index]){
      client_print( id, print_chat, "You have not learned the Lesser Heal spell" )
      return PLUGIN_HANDLED
   }

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   if( u_delay ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may cast this spell only after a %d second delay", get_cvar_num("FT_ultimatedelay"))
      return PLUGIN_HANDLED
   }

   if ( healdown[id] ){
      client_print( id, print_chat, "[%s] Your heal spell is not ready yet", MOD )
      return PLUGIN_HANDLED
   }

   new friend, body
   get_user_aiming(id, friend, body)

   if ( (0 < friend <= MAX_PLAYERS) && (friend != id) &&
       (get_user_team(id) == get_user_team(friend)) && is_user_alive(id) &&
        is_user_alive(friend))
   {
      // id has a friend to check for wounds


      new name[32], fname[32]
      get_user_name(id, name, 31)
      get_user_name(friend, fname, 31)

      if ( get_user_health(friend) >= p_maxhealth[friend] ){
         client_print(id, print_chat,
            "[%s] %s is not in need of healing right now", MOD, fname )
         return PLUGIN_HANDLED
      }

      new origin[3]
      get_user_origin( id, origin )
      wowmod_sound_playsound( id, SOUND_SPELLCAST )
      wowmod_effect_spellcast( origin )

      get_user_origin( friend, origin )
      wowmod_effect_dlight( origin, 25, COLOR_WHITE, 50 )

      p_casts[id] += 1
      p_casts_s[id] += 1
      p_heals[id] += 1
      p_heals_s[id] += 1

      new hbonus = p_spells[id][spell_index] * floatround( HEALTHBONUS * 0.20 )

      set_user_health_log( friend, get_user_health(friend) + hbonus, false, "wow_lesserheal()" )

      if ( get_user_health(friend) > p_maxhealth[friend] )
         set_user_health_log( friend, p_maxhealth[friend], false, "wow_lesserheal() p_maxhealth adjustment" )

      // Handle messages for healing touch
      client_print( id, print_chat, "[%s] You cast Lesser Heal on %s", MOD, fname )
      client_print( friend, print_chat, "[%s] %s has cast a healing spell on you", MOD, name )

      wowmod_sound_playsound( id, SOUND_HEAL )
      wowmod_sound_playsound( friend, SOUND_HEAL )

      healdown[id] = true
      new parm[2]
      parm[0] = id
      new Float:ftimer = HEAL_COOLDOWN_TIMER
      set_task( ftimer, "wowmod_heal_cooldown", TASK_HEALCOOL+id, parm, 2 )

   }else if (!is_user_alive(id)){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD)
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN draw_medicspr - Kept from UWC3
// Draw the medic alert sprites for injured players
// **************************************************************************
public draw_medicspr(){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if (get_cvar_num("UW_medicalerts") != 2)
      return PLUGIN_CONTINUE

   new players[32], numofplayers, idself, id, i, j
   new origin[3]

   new wsprite // Which sprite to display for a player -> teammate

   get_players(players, numofplayers)

   for (i = 0; i < numofplayers; i++){
      idself = players[i] // Player id to display medic sprites to

      if ( !is_user_alive(idself) )
         // If idself player is dead skip to next
         continue

      new healspell1 = p_spells[idself][SPELL_DRUID_BASE_IDX + SP_HEALTOUCH]
      new healspell2 = p_spells[idself][SPELL_PRIEST_BASE_IDX + SP_HEAL]
      new healspell3 = p_spells[idself][SPELL_PRIEST_BASE_IDX + SP_LESSERHEAL]

      new idself_skill = 0

   
      // Determine extent of skill (mend, repair or both)
      if ( healspell1 || healspell2 || healspell3 ){
         idself_skill = 1
      }else{
         // If player is not a medic or armorsmith skip to next
         idself_skill = 0
         continue
      }

      // If we make it here idself is either medic, armorsmith or both
      // Scan through players and isolate living teammates
      for (j = 0; j < numofplayers; j++){
         id = players[j]  // Player id to see if medic sprite should be displayed

         if ( (idself != id) && (get_user_team(idself) == get_user_team(id)) &&
              is_user_alive(id) && ((get_user_health(id) < p_maxhealth[id])) )
         {

            //client_print(idself, print_chat, "evaluating id=(%d)", id)

            // Determine extent of injury (health, armor or both)
            new winjury = 0

            if ( get_user_health(id) < p_maxhealth[id] ){
               winjury = 1
            }else{
               // Player id has no injuries or damaged armor
               continue
            }

            if (idself_skill == 3){
               if (winjury == 3)
                  wsprite = medshieldspr
               else if (winjury == 1)
                  wsprite = medicspr
               else if (winjury == 2)
                  wsprite = shieldspr
               else
                  continue
            }else if ( (idself_skill == 1) && ((winjury == 3) || (winjury == 1)) ){
               wsprite = medicspr
            }else if ( (idself_skill == 2) && ((winjury == 3) || (winjury == 2)) ){
               wsprite = shieldspr
            }else{
               // Nothing to show for this player -> teammate combo
               continue
            }

            //client_print(idself, print_chat, "idself_skill=(%d) winjury=(%d)",
            //   idself_skill, winjury)

            get_user_origin(id, origin)

            message_begin(MSG_ONE,SVC_TEMPENTITY,{0,0,0},idself)
            write_byte(TE_PLAYERATTACHMENT)
            write_byte(id)
            write_coord( 30 )       // z axis for sprite
            write_short( wsprite )  // which sprite to display
            write_short(50)
            message_end()
         }
      }
   }

   set_task( 2.0, "draw_medicspr", TASK_MEDICSPR )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_cheetah subroutine
// Increases run speed for aspect of cheetah spell
// **************************************************************************
public wowmod_cheetah( parm[1] ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new id = parm[0]
   new Float:speed = 0.0

   if ( !is_user_connected( id ) )
      return PLUGIN_CONTINUE

   new spell_index = SPELL_HUNTER_BASE_IDX + SP_ACHEETAH
   new cheetah = p_spells[id][spell_index]

   //client_print(id, print_chat,
   // "[%s] cheetah(): id=(%d) class=(%d) spell_val=(%d) maxspeed=(%f)", 
   // MOD, id, p_class1[id], p_spells[id][spell_index], get_user_maxspeed(id) )

   if ( sprinton[id] ){
      // Player is Rogue in Sprint mode
      speed = SPEED_SPRINT
   }else if ( cheetah ){
      // Player is Hunter w/ aspect of cheetah
      speed = SPEED_ACHEETAH_BASE + floatround( float(p_spells[id][spell_index]) * SPEED_ACHEETAH )
   }else if ( spa_speed[id] > 0.0 ){
      speed = spa_speed[id]
   }else{
      speed = p_maxspeed[id]
   }

   if ( stunned[id] || isrooted[id] || blizzardon[id] || wingclipon[id] || isfrosted[id] || cheapshoton[id] ){
      //client_print( id, print_chat, "[%s] Your runspeed is affected by another spell right now", MOD ) 
      return PLUGIN_CONTINUE
   }

   if ( get_user_maxspeed(id) > 10 && !freezetime && ( get_user_maxspeed(id) < speed ) ){
      set_user_maxspeed( id, speed )

      if ( cheetah ){
         p_maxspeed[id] = speed
         client_print( id, print_console,
            "[%s] Cheetah setting new maxspeed id=(%d) maxspeed=(%f)", MOD, id, speed )
      }else if ( spa_speed[id] > 0.0 ){
         client_print( id, print_console,
            "[%s] Boots of Speed setting new maxspeed id=(%d) maxspeed=(%f)", MOD, id, speed )
      }else if ( sprinton[id] ){
         client_print( id, print_console,
            "[%s] Sprint setting new maxspeed id=(%d) maxspeed=(%f)", MOD, id, speed )
      }
   }
 
   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_damage_ssting supporting routine
// Hunter Serpent Sting spell
// **************************************************************************
public wowmod_damage_ssting( parm[] ){
   new id = parm[0]
   new enemy = parm[1]
   new dmg = parm[3]

   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !is_user_alive(id) || !serpstingon[id] )
      return PLUGIN_CONTINUE

   //log_amx( "WowMOD %d feels the sting of the serpent for %d dmg", id, dmg )

   client_print( id, print_chat, "[%s] You feel the sting of the serpent", MOD )

   wowmod_effect_glow( id, GLOW_WHITE )
   wowmod_effect_bloodspray( id, 199 )
   //wowmod_effect_attachsprite( id, ssting, 0 )

   apply_damage( id, enemy, dmg, CLASS_HUNTER, SP_SSTING, "Serpent Sting" )
   //set_user_health_log( id, get_user_health(id) - dmg, false, "wow_serpentsting()" )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_damage_rupture supporting routine
// Rogue Rupture spell
// **************************************************************************
public wowmod_damage_rupture( parm[] ){
   new id = parm[0]
   new enemy = parm[1]
   new dmg = parm[3]
   new spell = parm[4]

   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !is_user_alive(id) || ( !ruptureon[id] && !garrotteon[id] ) )
      return PLUGIN_CONTINUE

   client_print( id, print_chat, "[%s] You are beeding...", MOD )

   wowmod_effect_glow( id, GLOW_WHITE )
   wowmod_effect_bloodspray( id, 226 )

   if ( spell == SP_RUPTURE )
      apply_damage( id, enemy, dmg, CLASS_ROGUE, SP_RUPTURE, "Rupture" )
   else if ( spell == SP_GARROTTE )
      apply_damage( id, enemy, dmg, CLASS_ROGUE, SP_GARROTTE, "Garrotte" )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_damage_corrupt supporting routine
// Warlock Corruption 
// **************************************************************************
public wowmod_damage_corrupt( parm[] ){
   new id = parm[0]
   new enemy = parm[1]
   new dmg = parm[3]

   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !is_user_alive(id) || !corruptionon[id] )
      return PLUGIN_CONTINUE

   //log_amx( "WowMOD %d feels the sting of the serpent for %d dmg", id, dmg )

   client_print( id, print_chat, "[%s] You are caused pain by a disease...", MOD )

   wowmod_effect_glow( id, GLOW_WHITE )
   wowmod_effect_bloodspray( id, 83 )
   //wowmod_effect_attachsprite( id, swpain, 0 )

   apply_damage( id, enemy, dmg, CLASS_WARLOCK, SP_CORRUPTION, "Corruption" )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_damage_pain supporting routine
// Priest Shadow Word: Pain spell
// **************************************************************************
public wowmod_damage_pain( parm[] ){
   new id = parm[0]
   new enemy = parm[1]
   new dmg = parm[3]

   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !is_user_alive(id) || !swpainon[id] )
      return PLUGIN_CONTINUE

   client_print( id, print_chat, "[%s] The shadow delivers you holy pain", MOD )

   wowmod_effect_glow( id, GLOW_WHITE )
   wowmod_effect_bloodspray( id, 50 )
   //wowmod_effect_attachsprite( id, swpain, 0 )

   apply_damage( id, enemy, dmg, CLASS_PRIEST, SP_SWPAIN, "Shadow Word: Pain" )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_spell_arcint subroutine
// Mage intellect buff spell
// **************************************************************************
public wowmod_spell_arcint( id, arcint_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !arcint_spell ){
      client_print( id, print_chat, "You have not learned the Arcane Intellect spell" )
      return PLUGIN_HANDLED
   }

   if ( arcinton[id] ){
      client_print( id, print_chat, "You already have Arcane Intellect" )
      return PLUGIN_HANDLED
   }

   arcinton[id] = true

   // How much int to buff
   new buff = BUFF_ARCINT * arcint_spell
   p_int[id] += buff
   p_mana[id] -= cast_mana

   client_print( id, print_chat,
      "[%s] You feel your mind expand with Arcane Intellect", MOD )

   //log_amx( "arcint> cast_mana=(%d) buff=(%d) after-int=(%d)", cast_mana, buff, p_int[id] )

   new parm[2]
   parm[0] = id
   new Float:ftimer = SPELL_BASE_TIMER + ( 5.0 * arcint_spell )
   set_task( ftimer, "wowmod_unbuff_arcint", TASK_ARCINT+id, parm, 2 )

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_spell_brilliance subroutine
// Mage full team intellect buff spell
// **************************************************************************
public wowmod_spell_brilliance( id, bril_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !bril_spell ){
      client_print( id, print_chat, "You have not learned the Arcane Brilliance spell" )
      return PLUGIN_HANDLED
   }

   if ( brillianceon[id] ){
      client_print( id, print_chat, "You already have Arcane Intellect" )
      return PLUGIN_HANDLED
   }

   p_casts[id] += 1
   p_casts_s[id] += 1
   p_teamacts[id] += 1 
   p_teamacts_s[id] += 1 

   p_mana[id] -= cast_mana
   p_xp[id] += ASSISTXP
   p_gold[id] += GOLD_ASSIST

   // How much int to buff
   new buff = BUFF_BRILLIANCE * bril_spell

   client_print( id, print_chat,
      "[%s] Your Arcane Brilliance expands the minds of your teamates, XP and GOLD bonuses earned", MOD )

   new tid
   new parm[2]
   new player[32]
   new numplayers
   get_players( player, numplayers )

   for ( new j = 0; j < numplayers; j++ ){
      tid = player[j]
      if ( get_user_team( id ) != get_user_team( tid ) )
         continue

      if ( !is_user_alive( tid ) )
         continue

      if ( brillianceon[tid] )
         continue

      brillianceon[tid] = true
      p_int[tid] += buff

      if ( id != tid )
         client_print( tid, print_chat,
            "[%s] You feel your mind expand with Arcane Brilliance", MOD )

      parm[0] = tid
      new Float:ftimer = SPELL_BASE_TIMER + ( 10.0 * bril_spell )
      set_task( ftimer, "wowmod_unbuff_brilliance", TASK_ARCINT+tid, parm, 2 )
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_spell_magearmor subroutine
// Mage mage armor spell
// **************************************************************************
public wowmod_spell_magearmor( id, marmor_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !marmor_spell ){
      client_print( id, print_chat, "You have not learned the Mage Armor spell" )
      return PLUGIN_HANDLED
   }

   if ( magearmoron[id] ){
      client_print( id, print_chat, "You already are protected by Mage Armor" )
      return PLUGIN_HANDLED
   }

   magearmoron[id] = true

   p_mana[id] -= cast_mana

   client_print( id, print_chat,
      "[%s] You feel protected by Mage Armor", MOD )

   new parm[2]
   parm[0] = id
   new Float:ftimer = SPELL_BASE_TIMER + ( 5.0 * marmor_spell )
   set_task( ftimer, "wowmod_unbuff_magearmor", TASK_MAGEARMOR+id, parm, 2 )

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_spell_sprint subroutine
// Rogue sprint spell
// **************************************************************************
public wowmod_spell_sprint( id, spr_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !spr_spell ){
      client_print( id, print_chat, "You have not learned the Sprint spell" )
      return PLUGIN_HANDLED
   }

   if ( sprinton[id] ){
      client_print( id, print_chat, "You are already Sprinting" )
      return PLUGIN_HANDLED
   }

   sprinton[id] = true
   p_energy[id] -= cast_mana

   client_print( id, print_chat,
      "[%s] You start to sprint, you feel VERY fast", MOD )

   set_user_maxspeed( id, SPEED_SPRINT )

   new parm[2]
   parm[0] = id
   new Float:ftimer = SPELL_BASE_TIMER2 + ( 5.0 * spr_spell ) 
   set_task( ftimer, "wowmod_unbuff_sprint", TASK_SPRINT+id, parm, 2 )

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_spell_vanish subroutine
// Rogue vanish spell
// **************************************************************************
public wowmod_spell_vanish( id, van_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !van_spell ){
      client_print( id, print_chat, "You have not learned the Vanish spell" )
      return PLUGIN_HANDLED
   }

   if ( vanishon[id] ){
      client_print( id, print_chat, "You already are Invisible" )
      return PLUGIN_HANDLED
   }

   vanishon[id] = true
   p_energy[id] -= cast_mana

   wowmod_effect_glow( id, GLOW_INVIS )

   client_print( id, print_chat,
      "[%s] You vanish into thin air", MOD )

   new parm[2]
   parm[0] = id

   new Float:ftimer = SPELL_BASE_TIMER + ( 5.0 * van_spell ) - 10.0
   parm[1] = 10 
   set_task( ftimer, "wowmod_warning_vanish", TASK_VANISHWARN1+id, parm, 2 )

   ftimer = SPELL_BASE_TIMER + ( 5.0 * van_spell ) - 5.0
   parm[1] = 5 
   set_task( ftimer, "wowmod_warning_vanish", TASK_VANISHWARN2+id, parm, 2 )

   ftimer = SPELL_BASE_TIMER + ( 5.0 * van_spell )
   set_task( ftimer, "wowmod_unbuff_vanish", TASK_VANISH+id, parm, 2 )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_spell_evasion subroutine
// Rogue evasion spell
// **************************************************************************
public wowmod_spell_evasion( id, evade_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !evade_spell ){
      client_print( id, print_chat, "You have not learned the Evasion spell" )
      return PLUGIN_HANDLED
   }

   if ( evasionon[id] ){
      client_print( id, print_chat, "You already are Evading" )
      return PLUGIN_HANDLED
   }

   evasionon[id] = true
   p_energy[id] -= cast_mana

   client_print( id, print_chat,
      "[%s] You brace yourself for danger and begin to Evade", MOD )

   new parm[2]
   parm[0] = id

   new Float:ftimer = SPELL_BASE_TIMER2 + evade_spell 
   set_task( ftimer, "wowmod_unbuff_evasion", TASK_EVASION+id, parm, 2 )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_spell_demonskin subroutine
// Warlock demon skin buff 
// **************************************************************************
public wowmod_spell_demonskin( id, dskin_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !dskin_spell ){
      client_print( id, print_chat, "You have not learned the Demon Skin spell" )
      return PLUGIN_HANDLED
   }

   if ( demonskinon[id] ){
      client_print( id, print_chat, "You already have Demon Skin" )
      return PLUGIN_HANDLED
   }

   demonskinon[id] = true
   p_mana[id] -= cast_mana

   client_print( id, print_chat,
      "[%s] You feel the protection of Demons", MOD )

   new parm[2]
   parm[0] = id
   new Float:ftimer = SPELL_BASE_TIMER + ( 5.0 * dskin_spell )
   set_task( ftimer, "wowmod_unbuff_demonskin", TASK_DEMONSKIN+id, parm, 2 )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_spell_lifetap subroutine
// Warlock lifetap spell
// **************************************************************************
public wowmod_spell_lifetap( id, life_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !life_spell ){
      client_print( id, print_chat, "You have not learned the Lifetap spell" )
      return PLUGIN_HANDLED
   }

   p_mana[id] -= cast_mana

   new life_mod = ( ( p_maxhealth[id] * life_spell ) / warlock_spmax[SP_LIFETAP] ) 
   new health = ( ( get_user_health(id) + life_mod ) > p_maxhealth[id] ) ? p_maxhealth[id] :
                  ( get_user_health(id) + life_mod )
   
   client_print( id, print_chat, "lifetap> life_mod=(%d) cur_health=(%d), health=(%d)",
      life_mod, get_user_health(id), health )
 
   switch ( life_spell ){
      case 1: p_mana[id] = 0
      case 2: p_mana[id] = floatround( p_mana[id] / 10.0 ) 
      case 3: p_mana[id] = floatround( p_mana[id] / 8.0 ) 
      case 4: p_mana[id] = floatround( p_mana[id] / 6.0 ) 
      case 5: p_mana[id] = floatround( p_mana[id] / 4.0 ) 
   }

   new mana = ( ( p_mana[id] * life_spell ) / warlock_spmax[SP_LIFETAP] ) 

   set_user_health_log( id, health, false,  "wowmod_spell_lifetap()" )

   p_mana[id] = mana

   client_print( id, print_chat,
      "[%s] You tap into your mana reserves, you feel better...", MOD )


   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_spell_tranq subroutine
// Druid tranquility regeneration spell
// **************************************************************************
public wowmod_spell_tranq( id, tranq_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !tranq_spell ){
      client_print( id, print_chat, "You have not learned the Tranquility spell" )
      return PLUGIN_HANDLED
   }

   if ( tranqon[id] ){
      client_print( id, print_chat, "You already are channeling Tranquility" )
      return PLUGIN_HANDLED
   }

   tranqon[id] = true

   client_print( id, print_chat, "You begin channeling Tranquility" )

   p_mana[id] -= cast_mana
   p_teamacts[id] += 1 
   p_teamacts_s[id] += 1 

   new parm[2]
   parm[0] = id
   new Float:ftimer = SPELL_BASE_TIMER + ( 5.0 * tranq_spell )
   set_task( ftimer, "wowmod_unbuff_tranq", TASK_TRANQ+id, parm, 2 )

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_spell_barkskin subroutine
// Druid barkskin spell
// **************************************************************************
public wowmod_spell_barkskin( id, bark_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !bark_spell ){
      client_print( id, print_chat, "You have not learned the Barkskin spell" )
      return PLUGIN_HANDLED
   }

   if ( barkon[id] ){
      client_print( id, print_chat, "You already are protected by Barkskin" )
      return PLUGIN_HANDLED
   }

   barkon[id] = true

   client_print( id, print_chat, "You cast Barskin and shroud yourself in a skin of protective Bark" )

   p_mana[id] -= cast_mana

   new parm[2]
   parm[0] = id
   new Float:ftimer = SPELL_BASE_TIMER + ( 5.0 * bark_spell )
   set_task( ftimer, "wowmod_unbuff_barkskin", TASK_BARK+id, parm, 2 )

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_spell_apack subroutine
// Druid aspect of the pack spell
// **************************************************************************
public wowmod_spell_apack( id, pack_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !pack_spell ){
      client_print( id, print_chat, "You have not learned the Aspect of the Pack spell" )
      return PLUGIN_HANDLED
   }

   if ( apackon[id] ){
      client_print( id, print_chat, "[%s] You already have Aspect of the Pack", MOD )
      return PLUGIN_HANDLED
   }

   apackon[id] = true

   client_print( id, print_chat, "[%s] You cast Aspect of the Pack and feel a surge of speed, XP and Gold bonuses earned", MOD )

   p_casts[id] += 1
   p_casts_s[id] += 1
   p_teamacts[id] += 1 
   p_teamacts_s[id] += 1 
   p_mana[id] -= cast_mana

   //client_print( id, print_chat, "Before pack xp=(%d) gold=(%d)", p_xp[id], p_gold[id] )
   p_xp[id] += ASSISTXP
   p_gold[id] += GOLD_ASSIST
   //client_print( id, print_chat, "After pack xp=(%d) gold=(%d)", p_xp[id], p_gold[id] )
   
   new friend 
   new player[32]
   new numplayers

   get_players( player, numplayers )

   new Float:speed = SPEED_APACK + ( pack_spell * SPEED_APACK )

   for ( new i = 0; i < numplayers; ++i ){
      friend = player[i]

      if ( get_user_team( id ) != get_user_team( friend ) )
         continue

      apackon[friend] = true
      client_print( friend, print_chat, "[%s] You feel a surge of speed", MOD )

      set_user_maxspeed( friend, get_user_maxspeed( friend ) + speed )

      //client_print( id, print_chat, "Your teamate (%d) has new runspeed [%.2f]",
      //   friend, get_user_maxspeed( friend ) )
   }

   new parm[2]
   parm[0] = id
   parm[1] = floatround( p_maxspeed[id] )
   new Float:ftimer = SPELL_BASE_TIMER + ( 5.0 * pack_spell )
   set_task( ftimer, "wowmod_unbuff_apack", TASK_APACK+id, parm, 2 )

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_spell_awild subroutine
// Druid aspect of the wild spell
// **************************************************************************
public wowmod_spell_awild( id, wild_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !wild_spell ){
      client_print( id, print_chat, "[%s] You have not learned the Aspect of the Wild spell", MOD )
      return PLUGIN_HANDLED
   }

   if ( awildon[id] ){
      client_print( id, print_chat, "[%s] You already have Aspect of the Wild", MOD )
      return PLUGIN_HANDLED
   }

   awildon[id] = true

   client_print( id, print_chat, "[%s] You cast Aspect of the Wild and feel an affinity to nature, XP and Gold bonuses earned", MOD )

   p_casts[id] += 1
   p_casts_s[id] += 1
   p_teamacts[id] += 1 
   p_teamacts_s[id] += 1 
   p_mana[id] -= cast_mana

   p_xp[id] += ASSISTXP
   p_gold[id] += GOLD_ASSIST

   new friend 
   new player[32]
   new numplayers

   get_players( player, numplayers )

   new nature = AWILD_NATBUFF_BASE + ( wild_spell * AWILD_NATBUFF )

   for ( new i = 0; i < numplayers; ++i ){
      friend = player[i]

      if ( get_user_team( id ) != get_user_team( friend ) )
         continue

      awildon[friend] = true

      p_nature[friend] += nature

      client_print( friend, print_chat, "[%s] You feel an affinity to nature", MOD )

      //client_print( id, print_chat, "Your teamate (%d) has new nature [%d]",
      //   friend, p_nature[friend] )
   }

   new parm[2]
   parm[0] = id
   parm[1] = nature
   new Float:ftimer = SPELL_BASE_TIMER + ( 5.0 * wild_spell )
   set_task( ftimer, "wowmod_unbuff_awild", TASK_AWILD+id, parm, 2 )

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_dmgspell_counter subroutine
// Conditions handler for counter attack damage spell, returns -1 .. dmg amount
// **************************************************************************
public wowmod_dmgspell_counter( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new counter_spell = p_spells[id][SPELL_HUNTER_BASE_IDX + SP_COUNTER]
   if ( counter_spell <= 0 )
      return NO_DMG

   new counter_dmg = 0

   if ( p_mana[id] >= hunter_spmana[SP_COUNTER] ){
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( enemy, PF_NATURE )

      new resist_diff = ( p_nature[id] - victim_resist )
 
      counter_dmg = DMG_COUNTER_BASE + floatround( ( resist_diff / 100.0 ) * counter_spell )
 
      //client_print( id, print_chat,  "counter> id=(%d) rdiff=(%d) tdm=(%d)", 
      //   enemy, resist_diff, counter_dmg )

      p_casts[id] += 1
      p_casts_s[id] += 1
      p_mana[id] -= hunter_spmana[SP_COUNTER]

      client_print( id, print_chat, "[%s] Your Counterattack causes damage to %s", MOD, name2 )
      client_print( enemy, print_chat, "[%s] %s successfully counters your attack", MOD, name1 )

   }

   return counter_dmg 
}


// **************************************************************************
// BEGIN wowmod_dmgspell_aimed subroutine
// Conditions handler for aimed shot spell 
// **************************************************************************
public wowmod_dmgspell_aimed( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD )
      return PLUGIN_HANDLED
   }

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   new aimed_spell = p_spells[id][SPELL_HUNTER_BASE_IDX + SP_AIMED]
   if ( aimed_spell <= 0 )
      return PLUGIN_HANDLED 

   if( u_delay ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may cast this spell only after a %d second delay", get_cvar_num("FT_ultimatedelay"))
      return PLUGIN_HANDLED
   }

   if ( p_mana[id] < ( hunter_spmana[SP_AIMED] + ( aimed_spell * SPELL_MANA_RANK2 ) ) ){
      client_print( id, print_chat, "[%s] You do not have enough mana to cast Aimed Shot",
         MOD )
      return PLUGIN_HANDLED
   }

   if ( is_user_alive(id) && !issearching[id] ){ 
      new parm[3]
      parm[0] = id 
      parm[1] = SPELL_SEARCH_TIME
      parm[2] = SPELL_HUNTER_BASE_IDX + SP_AIMED 
      wowmod_search_target(parm)
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_dmgspell_frosbolt subroutine
// Conditions handler for frostbolt spell 
// **************************************************************************
public wowmod_dmgspell_frostbolt( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD )
      return PLUGIN_HANDLED
   }

   new frost_spell = p_spells[id][SPELL_MAGE_BASE_IDX + SP_FROSTBOLT]
   if ( frost_spell <= 0 )
      return PLUGIN_HANDLED 

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   if( u_delay ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may cast this spell only after a %d second delay", get_cvar_num("FT_ultimatedelay"))
      return PLUGIN_HANDLED
   }

   if ( p_mana[id] < ( mage_spmana[SP_FROSTBOLT] + ( frost_spell * SPELL_MANA_RANK ) ) ){
      client_print( id, print_chat, "[%s] You do not have enough mana to cast Frostbolt",
         MOD )
      return PLUGIN_HANDLED
   }

   if ( is_user_alive(id) && !issearching[id] ){ 
      new parm[3]
      parm[0] = id 
      parm[1] = SPELL_SEARCH_TIME
      parm[2] = SPELL_MAGE_BASE_IDX + SP_FROSTBOLT
      wowmod_search_target(parm)
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_dmgspell_immolate subroutine
// Conditions handler for Immolate spell
// **************************************************************************
public wowmod_dmgspell_immolate( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD )
      return PLUGIN_HANDLED
   }

   new imm_spell = p_spells[id][SPELL_WARLOCK_BASE_IDX + SP_IMMOLATE]
   if ( imm_spell <= 0 )
      return PLUGIN_HANDLED 

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   if( u_delay ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may cast this spell only after a %d second delay", get_cvar_num("FT_ultimatedelay"))
      return PLUGIN_HANDLED
   }

   if ( p_mana[id] < ( warlock_spmana[SP_IMMOLATE] + ( imm_spell * SPELL_MANA_RANK2 ) ) ){
      client_print( id, print_chat, "[%s] You do not have enough mana to cast Immolate",
         MOD )
      return PLUGIN_HANDLED
   }

   if ( is_user_alive(id) && !issearching[id] ){ 
      new parm[3]
      parm[0] = id 
      parm[1] = SPELL_SEARCH_TIME
      parm[2] = SPELL_WARLOCK_BASE_IDX + SP_IMMOLATE
      wowmod_search_target(parm)
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_dmgspell_shadowbolt subroutine
// Conditions handler for Shadow Bolt spell
// **************************************************************************
public wowmod_dmgspell_shadowbolt( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD )
      return PLUGIN_HANDLED
   }

   new bolt_spell = p_spells[id][SPELL_WARLOCK_BASE_IDX + SP_SHADOWBOLT]
   if ( bolt_spell <= 0 )
      return PLUGIN_HANDLED 

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   if( u_delay ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may cast this spell only after a %d second delay", get_cvar_num("FT_ultimatedelay"))
      return PLUGIN_HANDLED
   }

   if ( p_mana[id] < ( warlock_spmana[SP_SHADOWBOLT] + ( bolt_spell * SPELL_MANA_RANK2 ) ) ){
      client_print( id, print_chat, "[%s] You do not have enough mana to cast Shadow Bolt",
         MOD )
      return PLUGIN_HANDLED
   }

   if ( is_user_alive(id) && !issearching[id] ){ 
      new parm[3]
      parm[0] = id 
      parm[1] = SPELL_SEARCH_TIME
      parm[2] = SPELL_WARLOCK_BASE_IDX + SP_SHADOWBOLT
      wowmod_search_target(parm)
   }

   return PLUGIN_CONTINUE
}



// **************************************************************************
// BEGIN wowmod_dmgspell_pain subroutine
// Conditions handler for shadow word pain spell 
// **************************************************************************
public wowmod_dmgspell_pain( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new pain_spell = p_spells[enemy][SPELL_PRIEST_BASE_IDX + SP_SWPAIN]
   if ( pain_spell <= 0 )
      return PLUGIN_CONTINUE

   new cast_mana = priest_spmana[SP_SWPAIN] + ( pain_spell * SPELL_MANA_RANK2 ) 

   if ( p_mana[enemy] < cast_mana )
      return PLUGIN_CONTINUE

   if ( swpainon[id] )
      return PLUGIN_CONTINUE

   new name1[32]
   new name2[32]

   get_user_name(id, name1, 31)
   get_user_name(enemy, name2, 31)

   p_casts[id] += 1
   p_casts_s[id] += 1
   p_mana[id] -= ( priest_spmana[SP_SWPAIN] + ( pain_spell * SPELL_MANA_RANK2 ) )

   new victim_resist = wowmod_calc_resist( enemy, PF_HOLY )
   new rnum = random_num( 0, 100 )

   if ( has_stoneform[id] ){
      client_print( id, print_chat, "[%s] Your Stoneform protects you from Shadow Word: Pain",
         MOD )
      client_print( enemy, print_chat, "[%s] Stoneform protects %s from your Shadow Word: Pain",
         MOD, name1 )
      return NO_DMG
   }

   if ( rnum < floatround( ( p_int[enemy] + p_holy[enemy] - victim_resist ) / 4.0 ) ){
      swpainon[id] = true

      client_print( id, print_chat, "[%s] A shadow falls over you inflicting holy punishment",
                  MOD )
      client_print( enemy, print_chat, 
                  "[%s] A shadow falls over %s delivering your holy punishment", MOD, name1 )

      new Float:ftimer = SWPAIN_BASE_TIMER - ( 0.20 * pain_spell )
      new dmg = DMG_SWPAIN * pain_spell
      log_amx( "Shadow Word: Pain about to start dot damage %d, ftimer=(%f)", dmg, ftimer )
      new args[4]
      args[0] = id 
      args[1] = enemy 
      args[2] = 0
      args[3] = dmg
      set_task( ftimer, "wowmod_damage_pain", TASK_SWPAIN+id, args, 4, "b" );
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_dmgspell_manaburn subroutine
// Conditions handler for manaburn spell
// **************************************************************************
public wowmod_dmgspell_manaburn( id, enemy ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new mana_spell = p_spells[enemy][SPELL_PRIEST_BASE_IDX + SP_MANABURN]

   if ( mana_spell <= 0 )
      return NO_DMG

   if ( ( p_mana[id] > 0 ) && ( p_mana[enemy] >= priest_spmana[SP_MANABURN] )){
      new name1[32], name2[32]
      get_user_name(id, name1, 31)
      get_user_name(enemy, name2, 31)
      new victim_resist = wowmod_calc_resist( id, PF_HOLY )

      new rnum = random_num( 0, 100 )
      new chance = floatround( ( p_int[enemy] + p_holy[enemy] - victim_resist ) / 2.0 )

      //client_print( enemy, print_chat, "manaburn> id=(%d) rnum=(%d) chance=(%d)",
      //               enemy, rnum, chance )

      if ( rnum < chance ){
         new burn_amt = DMG_MANABURN_BASE * mana_spell

         p_casts[enemy] += 1
         p_casts_s[enemy] += 1
         p_mana[enemy] -= priest_spmana[SP_MANABURN]
         p_mana[id] -= burn_amt

         client_print( id, print_chat, "[%s] %s burns some of your mana pool", MOD, name2 )
         client_print( enemy, print_chat, "[%s] You burn some of the mana pool of %s", MOD, name1 )
      }
   }
   return NO_DMG
}


// **************************************************************************
// BEGIN wowmod_search_target subroutine
// Handler for all spells that require search mode for target acquisition
// **************************************************************************
public wowmod_search_target( parm[3] ){

   new id = parm[0]
   new spell = parm[2]
   new enemy, body
   new victim_resist
  
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED
 
   get_user_aiming( id, enemy, body )


   if ( ( 0 < enemy < MAX_PLAYERS ) && ( get_user_team( id ) != get_user_team( enemy  ) ) &&
         ( is_user_alive( id ) && is_user_alive( enemy ) ) )
   {
      // We have a valid living enemy as an elligible search target
      new name[32], ename[32]
      get_user_name( id, name, 31 )
      get_user_name( enemy, ename, 31 )

      new origin[3]
      get_user_origin( id, origin )
      wowmod_sound_playsound( id, SOUND_SPELLCAST )
      wowmod_effect_spellcast( origin )
      
      issearching[id] = false

      if ( spell == ( SP_MANATAP ) ){
         // Bloodelf Mana Tap racial ability

         if ( p_mana[enemy] <= 0 ){
            client_print( id, print_chat, "[%s] %s has no mana to steal", MOD, ename )
         }else{
            new mana = RACIAL_MANATAP_BASE + ( floatround( p_level[id] / 10.0 ) * RACIAL_MANATAP_BASE )
            mana = ( mana > p_mana[enemy] ) ? p_mana[enemy] : mana
            p_mana[id] += mana
            p_mana[enemy] -= mana

            client_print( id, print_chat, 
               "[%s] Your Mana Tap ability drains some mana from %s", MOD, ename )
            client_print( enemy, print_chat, "[%s] %s has drained some of your mana", MOD, name )
         }
         wowmod_effect_beaments( id, enemy, lightning, COLOR_GREEN, 80, 10 )
      }else if ( spell == ( SPELL_HUNTER_BASE_IDX + SP_AIMED ) ){
         // Hunter Aimed Shot
         new aimed_spell = p_spells[id][spell]
         p_casts[id] += 1
         p_casts_s[id] += 1
         p_mana[id] -= ( hunter_spmana[SP_AIMED] + ( aimed_spell * SPELL_MANA_RANK2 ) )

         victim_resist = wowmod_calc_resist( enemy, PF_MARKSMAN )
         new rnum = random_num( 0, 100 )

         if ( rnum < floatround( ( p_agi[id] + p_marksman[id] - victim_resist ) / 2.0 ) ){
            new spell_dmg = DMG_AIMED_BASE + ( 2 * aimed_spell )

            apply_damage( enemy, id, spell_dmg, CLASS_HUNTER, SP_AIMED, "Aimed Shot" )

            //client_print( id, print_chat, "aimed> mana_cost=(%d) dmg=(%d)", 
            //   ( p_spells[id][spell] * hunter_spmana[SP_AIMED] ), spell_dmg )

            wowmod_effect_stsplash( id, enemy )
            wowmod_sound_playsound( id, SOUND_BIGGUN )
            wowmod_sound_playsound( enemy, SOUND_BIGGUN )
 
            // Hunter Aimed Shot search for target
            client_print( id, print_chat, "[%s] You hit %s with your Aimed Shot", 
               MOD, ename )
            client_print( enemy, print_chat, "[%s] %s hits you with an Aimed Shot", 
               MOD, name )
         }else{
            client_print( id, print_chat, "[%s] Your Aimed Shot misses its intended target",
               MOD )
         }
      }else if ( spell == ( SPELL_WARLOCK_BASE_IDX + SP_IMMOLATE ) ){
         // Warlock Immolate 
         new imm_spell = p_spells[id][spell]
         p_casts[id] += 1
         p_casts_s[id] += 1
         p_mana[id] -= ( warlock_spmana[SP_IMMOLATE] + ( imm_spell * SPELL_MANA_RANK2 ) )

         victim_resist = wowmod_calc_resist( enemy, PF_FIRE )
         new rnum = random_num( 0, 100 )

         if ( rnum < floatround( ( p_int[id] + p_fire[id] - victim_resist ) / 2.0 ) ){
            new spell_dmg = DMG_IMMOLATE_BASE + ( 2 * imm_spell )

            apply_damage( enemy, id, spell_dmg, CLASS_WARLOCK, SP_IMMOLATE, "Immolate" )

            wowmod_effect_burn( enemy )
 
            client_print( id, print_chat, "[%s] Your Immolate spell burns %s", 
               MOD, ename )
            client_print( enemy, print_chat, "[%s] %s burns you with Immolate", 
               MOD, name )
         }else{
            client_print( id, print_chat, "[%s] Your Immolate spell was resisted",
               MOD )
         }
      }else if ( spell == ( SPELL_WARLOCK_BASE_IDX + SP_SHADOWBOLT ) ){
         // Warlock Shadow Bolt 
         new sb_spell = p_spells[id][spell]
         p_casts[id] += 1
         p_casts_s[id] += 1
         p_mana[id] -= ( warlock_spmana[SP_SHADOWBOLT] + ( sb_spell * SPELL_MANA_RANK2 ) )

         victim_resist = wowmod_calc_resist( enemy, PF_SHADOW )
         new rnum = random_num( 0, 100 )

         if ( rnum < floatround( ( p_int[id] + p_shadow[id] - victim_resist ) / 2.0 ) ){
            new spell_dmg = DMG_SHADOWBOLT_BASE + ( 2 * sb_spell )

            apply_damage( enemy, id, spell_dmg, CLASS_WARLOCK, SP_SHADOWBOLT, "Shadow Bolt" )

            wowmod_effect_beaments( id, enemy, lightning, COLOR_GRAY, 100, 10 )
 
            client_print( id, print_chat, "[%s] Your spell consumes %s in shadow", 
               MOD, ename )
            client_print( enemy, print_chat, "[%s] You are consumed by shadow", 
               MOD, name )
         }else{
            client_print( id, print_chat, "[%s] Your Shadow Bolt spell was resisted",
               MOD )
         }
      }else if ( spell == ( SPELL_DRUID_BASE_IDX + SP_ROOTS ) ){
         // Druid Roots Spell
         new roots_spell = p_spells[id][spell]
         p_casts[id] += 1
         p_casts_s[id] += 1
         p_mana[id] -= ( druid_spmana[SP_ROOTS] + ( roots_spell * SPELL_MANA_RANK ) )

         if ( isrooted[enemy] ){
            client_print(id, print_chat,
               "[%s] %s already has been rooted", MOD, ename )
            return PLUGIN_HANDLED
         }

         // Resist check for Orcs
         if ( p_race[enemy] == RACE_ORC ){
            victim_resist = RACIAL_ORC_RESIST
            new rnum = random_num( 0, 100 )
            if ( rnum < victim_resist ){
               client_print( id, print_chat, "[%s] %s resists your Roots spell", MOD, ename )
               client_print( enemy, print_chat, "[%s] You resist a Root spell cast by %s", MOD, name )
            }
            return PLUGIN_CONTINUE
         }

         victim_resist = wowmod_calc_resist( enemy, PF_NATURE )
         new rnum = random_num( 0, 100 )

         if ( rnum >= floatround( ( p_int[id] + p_nature[id] - victim_resist ) / 2.0 ) ){
            client_print( id, print_chat, "[%s] %s resists your Roots spell", MOD, ename )
            client_print( enemy, print_chat, "[%s] You resist a Root spell cast by %s", MOD, name )
            return PLUGIN_CONTINUE
         }
         
         isrooted[enemy] = true
         stunned[enemy] = true
         set_user_maxspeed( enemy, 1.0 )

         wowmod_sound_playsound( id, SOUND_ROOTS )
         wowmod_sound_playsound( enemy, SOUND_ROOTS )

         // ****************************************************************************
         // [from UWC3] - Entangling Roots gfx redone, root model by steve french - K2mia
         new vec[3]
         new Float:rsize[2][3] = { {-50.0, -50.0, -50.0}, {50.0, 50.0, 50.0} }
         get_user_origin( enemy, vec )

         new Float:fOrigin[3]
         fOrigin[0] = float( vec[0] )
         fOrigin[1] = float( vec[1] )
         fOrigin[2] = float( vec[2] ) - 35.0

         new roots_ent = create_entity( "info_target" )

         entity_set_string( roots_ent, EV_SZ_classname, "roots" )
         entity_set_model( roots_ent, "models/roots2.mdl" )
         entity_set_int( roots_ent, EV_INT_body, 3 )
         entity_set_int( roots_ent, EV_INT_sequence, 0 )
         set_size( roots_ent, rsize[0], rsize[1] )
         entity_set_origin( roots_ent, fOrigin )

         // [from UWC3] - Entangling Roots gfx redone, root model by steve french - K2mia
         // ****************************************************************************

         new parm[3]
         parm[0] = enemy 
         parm[1] = floatround( p_maxspeed[enemy] )
         parm[2] = roots_ent
         p_rootent[enemy] = roots_ent

         // Handle messages for roots spell 
         client_print( id, print_chat, "[%s] You root %s to the ground", MOD, ename )
         client_print( enemy, print_chat, "[%s] %s has rooted you to the ground", MOD, name )

         new Float:ftimer = ROOTS_BASE_TIMER + ( 10.0 * (roots_spell-1) )
         set_task( ftimer, "wowmod_unbuff_roots", TASK_ROOTS+enemy, parm, 3 )
      }else if ( spell == ( SPELL_MAGE_BASE_IDX + SP_FROSTBOLT ) ){
         // Mage Frostbolt Spell
         new frost_spell = p_spells[id][spell]
         p_casts[id] += 1
         p_casts_s[id] += 1
         p_mana[id] -= ( mage_spmana[SP_FROSTBOLT] + ( frost_spell * SPELL_MANA_RANK ) )

         victim_resist = wowmod_calc_resist( enemy, PF_FROST )

         // Add in bonus resist for Orcs
         if ( p_race[enemy] == RACE_ORC )
            victim_resist += RACIAL_ORC_RESIST

         rnum = random_num( 0, 100 )

         if ( rnum < floatround( ( p_int[id] + p_frost[id] - victim_resist ) / 2.0 ) ){
            new spell_dmg = DMG_FROSTBOLT_BASE + frost_spell

            apply_damage( enemy, id, spell_dmg, CLASS_MAGE, SP_FROSTBOLT, "Frostbolt" )

            isfrosted[enemy] = true
            stunned[enemy] = true
            set_user_maxspeed( enemy, SPEED_FROSTBOLT )

            wowmod_effect_frostbolt( id, enemy )

            // Handle messages for frostbolt spell 
            client_print( id, print_chat, "[%s] Your Frostbolt slows down %s", MOD, ename )
            client_print( enemy, print_chat, 
               "[%s] %s hits you with a Frostbolt and slows you down", MOD, name )

            new parm[2]
            parm[0] = enemy 
            parm[1] = floatround( p_maxspeed[enemy] )
            new Float:ftimer = FROSTBOLT_BASE_TIMER + ( 10.0 * (frost_spell-1) )
            set_task( ftimer, "wowmod_unbuff_frostbolt", TASK_FROSTBOLT+id, parm, 3 )
         }else{
            client_print( id, print_chat, "[%s] Your Frostbolt misses its intended target", 
               MOD )
         }
   
      } 
   }else{
      if ( parm[1] > 0 && get_user_health(id) > 0 ){
         issearching[id] = true
         parm[1] -= 1

         new counter = parm[1]
         while ( counter >= 0 ){
            counter -= 10
            if ( counter == 0 )
               wowmod_sound_playsound( id, SOUND_TUPING )
         }

         set_task( 0.1, "wowmod_search_target", TASK_AIMED+id, parm, 3)
      }else{
         if ( spell == SP_MANATAP )
            // Reset racial_used if failed to find target for mana tap
            racial_used[id] = false

         issearching[id] = false
         client_print( id, print_chat, 
            "[%s] You failed to find a target for your spell", MOD )
      }
 
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_tranq_health subroutine
// Druid tranquility regeneration effect 
// **************************************************************************
public wowmod_tranq_health( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !is_user_alive( id ) )
      return PLUGIN_CONTINUE

   new targetid
   new tname[32]
   new origin[3]
   new player[32]
   new numplayers
   new targetorigin[3]
         
   get_user_origin( id, origin )
   get_players( player, numplayers )

   //client_print( id, print_chat, "MIH tranq_health() 1" )

   for ( new i = 0; i < numplayers; ++i ){
      targetid = player[i]
      get_user_name( targetid, tname, 31 )

      if ( !is_user_alive( targetid ) )
         continue 
 
      if ( get_user_team( id ) != get_user_team( targetid ) )
         continue 

      if ( get_user_health(targetid) >= p_maxhealth[targetid] )
         continue 

      get_user_origin( targetid, targetorigin )

      if ( get_distance( origin, targetorigin ) <= get_cvar_num("FT_xp_radius") ){ 
         new thealth = ( get_user_health(targetid) + TRANQ_HEALTH_BONUS < p_maxhealth[targetid] ) ?
                        ( get_user_health(targetid) + TRANQ_HEALTH_BONUS ) : p_maxhealth[targetid]

         set_user_health_log( targetid, thealth, false,  "wowmod_tranq()" )

         client_print( targetid, print_chat, "[%s] You feel tranquil...", MOD )


         if ( id != targetid ){
            client_print( id, print_chat, "[%s] Your Tranquility benefits %s, XP and GOLD bonuses earned",
               MOD, tname )
            p_xp[id] += TRANQXP
            p_gold[id] += GOLD_ASSIST2
         }
      }
   } 

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_spell_remcurse supporting routine
// Druid Remove Curse spell
// **************************************************************************
public wowmod_spell_remcurse( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new spell_index = SPELL_DRUID_BASE_IDX + SP_REMCURSE

   if ( !p_spells[id][spell_index]){
      client_print( id, print_chat, "You have not learned the Remove Curse spell" )
      return PLUGIN_HANDLED
   }

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   if( u_delay ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may cast this spell only after a %d second delay", get_cvar_num("FT_ultimatedelay"))
      return PLUGIN_HANDLED
   }

   new friend, body
   get_user_aiming(id, friend, body)

   if ( (0 < friend <= MAX_PLAYERS) && (friend != id) &&
       (get_user_team(id) == get_user_team(friend)) && is_user_alive(id) &&
        is_user_alive(friend))
   {
      // id has a friend to check for wounds

      // Mana spent to cast spell
      new cast_mana = druid_spmana[SP_REMCURSE] * p_spells[id][spell_index]

      client_print( id, print_console,
         "sp_mana=(%d) cast_mana=(%d)",  druid_spmana[SP_REMCURSE], cast_mana )

      if ( p_mana[id] < cast_mana ){
         client_print(id, print_chat,
            "[%s] You do not have enough mana to cast Remove Curse", MOD )
         return PLUGIN_HANDLED
      }

      new origin[3]
      get_user_origin( id, origin )
      wowmod_sound_playsound( id, SOUND_SPELLCAST )
      wowmod_effect_spellcast( origin )

      new name[32], fname[32]
      get_user_name(id, name, 31)
      get_user_name(friend, fname, 31)

      if ( !isweakened[friend] ){
         client_print(id, print_chat,
            "[%s] %s is not cursed", MOD, fname )
         return PLUGIN_HANDLED
      }

      new rnum = random_num( 0, 100 )

      client_print( id, print_console, "remcurse> rnum=(%d) chance=(%d)",
         rnum, ( p_spells[id][spell_index] * REMCURSE_CHANCE ) )
     
 
      if ( rnum < ( p_spells[id][spell_index] * REMCURSE_CHANCE ) ){
         isweakened[friend] = false

         // Award the player XP for their service
         new xpbonus = p_spells[id][spell_index] * REMCURSEXP 
         p_xp[id] += xpbonus
         p_gold[id] += GOLD_ASSIST

         // Handle messages for remove curse 
         client_print( id, print_chat, "[%s] You cast Remove Curse on %s, XP and GOLD bonuses earned", 
            MOD, fname )
         client_print( friend, print_chat, "[%s] %s has removed a curse from you", MOD, name )
      }else{
         client_print( id, print_chat, "[%s] You failed to remove the curse on %s",
            MOD, fname )
      }

      p_casts[id] += 1
      p_casts_s[id] += 1
      p_mana[id] -= cast_mana

      wowmod_sound_playsound( id, SOUND_HEAL )
      wowmod_sound_playsound( friend, SOUND_HEAL )
   }else if (!is_user_alive(id)){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD)
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_spell_giftwild supporting routine
// Druid Gift of the Wild spell
// **************************************************************************
public wowmod_spell_giftwild( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new spell_index = SPELL_DRUID_BASE_IDX + SP_GIFTWILD

   if ( !p_spells[id][spell_index]){
      client_print( id, print_chat, "You have not learned the Gift of the Wild spell" )
      return PLUGIN_HANDLED
   }

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   if( u_delay ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may cast this spell only after a %d second delay", get_cvar_num("FT_ultimatedelay"))
      return PLUGIN_HANDLED
   }

   // Mana spent to cast spell
   new cast_mana = druid_spmana[SP_GIFTWILD] + ( SPELL_MANA_RANK * p_spells[id][spell_index] )

   if ( p_mana[id] < cast_mana ){
      client_print(id, print_chat,
         "[%s] You do not have enough mana to cast Gift of the Wild", MOD )
      return PLUGIN_HANDLED
   }

   new friend, body
   get_user_aiming(id, friend, body)


   if ( (0 < friend <= MAX_PLAYERS) && (friend != id) &&
       (get_user_team(id) == get_user_team(friend)) && is_user_alive(id) &&
        is_user_alive(friend))
   {
      // id has a friend to give gift to

      new origin[3]
      get_user_origin( id, origin )
      wowmod_sound_playsound( id, SOUND_SPELLCAST )
      wowmod_effect_spellcast( origin )

      new name[32], fname[32]
      get_user_name(id, name, 31)
      get_user_name(friend, fname, 31)

      if ( gifton[friend] ){
         client_print(id, print_chat,
            "[%s] %s already has been gifted", MOD, fname )
         return PLUGIN_HANDLED
      }

      p_casts[id] += 1
      p_casts_s[id] += 1
      p_teamacts[id] += 1 
      p_teamacts_s[id] += 1 
      p_mana[id] -= cast_mana

      new gift_spell = p_spells[id][spell_index]

      gifton[friend] = true

      // Buff armor and attributes
      p_armor[friend] += GIFTWILD_ARMOR_BUFF
      wowmod_modify_armor( friend )
      p_str[friend] += GIFTWILD_ATT_BUFF
      p_agi[friend] += GIFTWILD_ATT_BUFF
      p_stam[friend] += GIFTWILD_ATT_BUFF
      p_int[friend] += GIFTWILD_ATT_BUFF
      p_spir[friend] += GIFTWILD_ATT_BUFF
      p_melee[friend] += GIFTWILD_ATT_BUFF
      p_marksman[friend] += GIFTWILD_ATT_BUFF
      p_arcane[friend] += GIFTWILD_ATT_BUFF
      p_fire[friend] += GIFTWILD_ATT_BUFF
      p_frost[friend] += GIFTWILD_ATT_BUFF
      p_shadow[friend] += GIFTWILD_ATT_BUFF
      p_nature[friend] += GIFTWILD_ATT_BUFF
      p_holy[friend] += GIFTWILD_ATT_BUFF

      // Award the player XP for their service
      new xpbonus = p_spells[id][spell_index] * GIFTXP 
      p_xp[id] += xpbonus
      p_gold[id] += GOLD_ASSIST

      // Handle messages for remove curse 
      client_print( id, print_chat, "[%s] You cast Gift of the Wild on %s, XP and GOLD bonuses earned", 
         MOD, fname )
      client_print( friend, print_chat, "[%s] %s has cast Gift of the Wild on you", MOD, name )

      new parm[2]
      parm[0] = friend 
      new Float:ftimer = SPELL_BASE_TIMER + ( 5.0 * gift_spell )
      set_task( ftimer, "wowmod_unbuff_giftwild", TASK_GIFTWILD+friend, parm, 2 )

   }else if (!is_user_alive(id)){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD)
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_spell_giftwildself supporting routine
// Druid Gift of the Wild spell, self cast
// **************************************************************************
public wowmod_spell_giftwildself( id, gift_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !gift_spell ){
      client_print( id, print_chat, "You have not learned the Gift of the Wild spell" )
      return PLUGIN_HANDLED
   }

   if ( !is_user_alive(id) ){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD)
      return PLUGIN_HANDLED
   }

   if ( gifton[id] ){
      client_print(id, print_chat, "[%s] You already have the Gift of the Wild", MOD )
      return PLUGIN_HANDLED
   }

   new name[32]
   get_user_name(id, name, 31)

   gifton[id] = true

   // Buff armor and attributes
   p_armor[id] += GIFTWILD_ARMOR_BUFF
   wowmod_modify_armor( id )
   p_str[id] += GIFTWILD_ATT_BUFF
   p_agi[id] += GIFTWILD_ATT_BUFF
   p_stam[id] += GIFTWILD_ATT_BUFF
   p_int[id] += GIFTWILD_ATT_BUFF
   p_spir[id] += GIFTWILD_ATT_BUFF
   p_melee[id] += GIFTWILD_ATT_BUFF
   p_marksman[id] += GIFTWILD_ATT_BUFF
   p_arcane[id] += GIFTWILD_ATT_BUFF
   p_fire[id] += GIFTWILD_ATT_BUFF
   p_frost[id] += GIFTWILD_ATT_BUFF
   p_shadow[id] += GIFTWILD_ATT_BUFF
   p_nature[id] += GIFTWILD_ATT_BUFF
   p_holy[id] += GIFTWILD_ATT_BUFF

   p_casts[id] += 1
   p_casts_s[id] += 1
   p_mana[id] -= cast_mana

   // Handle messages for remove curse 
   client_print( id, print_chat, "[%s] You protect yourself with Gift of the Wild", MOD )
   
   new parm[2]
   parm[0] = id 
   new Float:ftimer = SPELL_BASE_TIMER + ( 5.0 * gift_spell )
   set_task( ftimer, "wowmod_unbuff_giftwild", TASK_GIFTWILD+id, parm, 2 )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_spell_roots subroutine
// Druid Roots spell
// **************************************************************************
public wowmod_spell_roots( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD )
      return PLUGIN_HANDLED
   }

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   if( u_delay ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may cast this spell only after a %d second delay", get_cvar_num("FT_ultimatedelay"))
      return PLUGIN_HANDLED
   }

   new roots_spell = p_spells[id][SPELL_DRUID_BASE_IDX + SP_ROOTS]
   if ( roots_spell <= 0 )
      return PLUGIN_HANDLED 

   // Mana spent to cast spell
   new cast_mana = ( druid_spmana[SP_ROOTS] + ( roots_spell * SPELL_MANA_RANK ) )

   if ( p_mana[id] < cast_mana ){
      client_print( id, print_chat, "[%s] You do not have enough mana to cast Roots",
         MOD )
      return PLUGIN_HANDLED
   }

   if ( is_user_alive(id) && !issearching[id] ){ 
      new parm[3]
      parm[0] = id 
      parm[1] = SPELL_SEARCH_TIME
      parm[2] = SPELL_DRUID_BASE_IDX + SP_ROOTS
      wowmod_search_target(parm)
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_spell_blink subroutine
// Mage blink spell
// **************************************************************************
public wowmod_spell_blink( id ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cast no spells!", MOD )
      return PLUGIN_HANDLED
   }

   new blink_spell = p_spells[id][SPELL_MAGE_BASE_IDX + SP_BLINK]

   if ( blink_spell <= 0 ){
      client_print( id, print_chat, "[%s] You have not yet learned the Blink spell", MOD )
      return PLUGIN_HANDLED
   }

   if ( blinkdown[id] ){
      client_print( id, print_chat, "[%s] You have not yet recovered from your last Blink", 
         MOD )
      return PLUGIN_HANDLED
   }

   new cast_mana = mage_spmana[SP_BLINK] + ( blink_spell * SPELL_MANA_RANK2 )

   if ( p_mana[id] < cast_mana ){
      client_print( id, print_chat, "[%s] You do not have enough mana to cast Blink",
         MOD )
      return PLUGIN_HANDLED
   }

   if ( freezetime == 1 ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may not cast this spell during freezetime" )
      return PLUGIN_HANDLED
   }

   if( u_delay ){
      set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
      show_hudmessage( id, "You may cast this spell only after a %d second delay", get_cvar_num("FT_ultimatedelay"))
      return PLUGIN_HANDLED
   }

   new origin[3]
   new destloc[3]
   get_user_origin( id, origin )
   get_user_origin( id, destloc, 3 )

   // Adjust dest coords
   if ( ( destloc[0] - destloc[0] ) > 0 ){
      destloc[0] -= 50
   }else{
      destloc[0] += 50
   }
   if ( ( destloc[1] - destloc[1] ) > 0 ){
      destloc[1] -= 50
   }else{
      destloc[1] += 50
   }
   destloc[2] += 40

   wowmod_sound_playsound( id, SOUND_BLINK )

   wowmod_effect_beamcyl( origin, 60, COLOR_RED )

   // Stop bomb planting...
   client_cmd(id,"-use")

   if ( get_cvar_num( "WOW_blink_punish" ) ){
      new mapname[32]
      get_mapname( mapname,32 )
      if ( equali( mapname, "as_oilrig" )){
         if ( ( destloc[0] > 1530 ) && ( get_user_team(id) == TEAM_CT ) ){
            client_cmd( id, "kill" )
            client_print( id, print_chat, 
               "[%s] You have been punished for blinking to a restricted area", MOD )
            return PLUGIN_HANDLED
         }
      }
   }

   set_user_origin( id, destloc)
   new Float:PlayerOrigin[3]
   entity_get_vector( id, EV_VEC_origin, PlayerOrigin )

   if ( get_cvar_num( "WOW_blink_punish" ) ){
      new parm[2]
      parm[0] = id
      set_task( 1.5, "wowmod_check_blinkrest", TASK_BLINKCHECK, parm, 2 )
   }

   new parm[9]
   parm[0] = id
   parm[1] = origin[0]
   parm[2] = origin[1]
   parm[3] = origin[2]
   parm[4] = destloc[0]
   parm[5] = destloc[1]
   parm[6] = destloc[2]
   parm[7] = cast_mana
   parm[8] = blink_spell
   set_task( 0.1, "wowmod_blink_test", TASK_BLINKTEST, parm, 8 )

   wowmod_sound_playsound( id, SOUND_BLINK )

   wowmod_effect_beamcyl( destloc, 60, COLOR_RED )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_spell_pwshield subroutine
// Priest power word: shield 
// **************************************************************************
public wowmod_spell_pwshield( id, pws_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !pws_spell ){
      client_print( id, print_chat, "You have not learned the Power Word: Shield spell" )
      return PLUGIN_HANDLED
   }

   if ( pwshieldon[id] ){
      client_print( id, print_chat, "You already have Power Word: Shield" )
      return PLUGIN_HANDLED
   }

   p_mana[id] -= cast_mana      
   pwshieldon[id] = true
   wowmod_effect_pwshield( id )

   client_print( id, print_chat,
      "[%s] You briefly shimmer and glow, you feel protected", MOD )

   //log_amx( "arcint> cast_mana=(%d) buff=(%d) after-int=(%d)", cast_mana, buff, p_int[id] )

   new parm[2]
   parm[0] = id
   new Float:ftimer = SPELL_BASE_TIMER + ( 5.0 * pws_spell )
   set_task( ftimer, "wowmod_unbuff_pwshield", TASK_PWSHIELD+id, parm, 2 )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_spell_priestselfheal subroutine
// Priest self heal 
// **************************************************************************
public wowmod_spell_priestselfheal( id, heal_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_HANDLED

   if ( !heal_spell ){
      client_print( id, print_chat, "You have not learned Heal spell" )
      return PLUGIN_HANDLED
   }

   if ( healdown[id] ){
      client_print( id, print_chat, "[%s] Your heal spell is not ready yet", MOD )
      return PLUGIN_HANDLED
   }

   if ( get_user_health(id) >= p_maxhealth[id] ){
      client_print(id, print_chat,
         "[%s] You are not in need of healing right now", MOD )
      return PLUGIN_HANDLED
   }

   p_mana[id] -= cast_mana      

   new origin[3]
   get_user_origin( id, origin )
   wowmod_effect_dlight( origin, 25, COLOR_WHITE, 50 )
   wowmod_sound_playsound( id, SOUND_HEAL )

   new hbonus = heal_spell * HEALTHBONUS_DEF2

   set_user_health_log( id, get_user_health(id) + hbonus, false, "priestselfheal()" )

   client_print( id, print_chat,
      "[%s] You cast a healing spell and feel better...", MOD )

   healdown[id] = true
   new parm[2]
   parm[0] = id
   new Float:ftimer = HEAL_COOLDOWN_TIMER
   set_task( ftimer, "wowmod_heal_cooldown", TASK_HEALCOOL+id, parm, 2 )

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_spell_healprayer subroutine
// Priest team heal spell 
// **************************************************************************
public wowmod_spell_healprayer( id, heal_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !heal_spell ){
      client_print( id, print_chat, "You have not learned Prayer of Healing spell" )
      return PLUGIN_HANDLED
   }

   p_casts[id] += 1
   p_casts_s[id] += 1
   p_teamacts[id] += 1 
   p_teamacts_s[id] += 1 
   p_mana[id] -= cast_mana      

   client_print( id, print_chat,
      "[%s] You cast a Prayer of healing for your entire team, XP and Gold bonuses earned", MOD )

   new xpbonus = heal_spell * PRAYERXP 
   new hbonus = heal_spell * HEALTHBONUS_DEF2
   set_user_health_log( id, get_user_health(id) + hbonus, false, "healprayer()" )

   new friend
   new origin[3]
   new player[32]
   new name[32]
   new fname[32]
   new numplayers

   get_user_name( id, name, 31 )
   get_players( player, numplayers )

   for ( new i = 0; i < numplayers; ++i ){
      friend = player[i]

      if ( id == friend )
         continue

      if ( get_user_team( id ) != get_user_team( friend ) )
         continue

      if ( !is_user_alive( friend ) )
         continue

      //client_print( id, print_chat, ">> fr=(%d) health=(%d) max=(%d)", 
      //   friend, get_user_health(friend), p_maxhealth[friend] )

      if ( get_user_health(friend) < p_maxhealth[friend] ){
         get_user_name( friend, fname, 31 )
         get_user_origin( friend, origin )
         wowmod_effect_dlight( origin, 25, COLOR_WHITE, 50 )
         wowmod_sound_playsound( friend, SOUND_HEAL )

         p_heals[id] += 1
         p_heals_s[id] += 1

         p_xp[id] += xpbonus
         p_gold[id] += GOLD_ASSIST
      
         client_print( id, print_chat,
            "[%s] Your Prayer of healing provides benefits to %s", MOD, fname )
         client_print( friend, print_chat,
            "[%s] %s casts a healing prayer for your team", MOD, name )

         set_user_health_log( friend, get_user_health(friend) + hbonus, false, "healprayer()" )
      }
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_spell_resurrect subroutine
// Priest Resurrection spell front end
// **************************************************************************
public wowmod_spell_resurrect( id, res_spell, cast_mana ){
   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if ( !res_spell ){
      client_print( id, print_chat, "You have not learned the Resurrection spell" )
      return PLUGIN_HANDLED
   }

   if ( !is_user_alive( id ) ){
      client_print( id, print_chat, "[%s] The dead cannot cast resurrect!", MOD )
      return PLUGIN_HANDLED
   }

   wowmod_display_resurrect( id, 1 )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_heal_cooldown subroutine
// Reset healdown[] to make heal spells ready again
// **************************************************************************
public wowmod_heal_cooldown( parm[] ){

   new id = parm[0]

   healdown[id] = false

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Your heal spell is ready", MOD )
 
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_unbuff_arcint subroutine
// Reset buff from mage arcane intellect spell
// **************************************************************************
public wowmod_unbuff_arcint( parm[] ){

   new id = parm[0]

   arcinton[id] = false

   p_int[id] = p_int_unb[id]

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Your Arcane Intellect fades, your mind returns to normal", MOD )
 
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_unbuff_brilliance subroutine
// Reset buff from mage arcane brilliance spell
// **************************************************************************
public wowmod_unbuff_brilliance( parm[] ){

   new id = parm[0]

   brillianceon[id] = false

   p_int[id] = p_int_unb[id]

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Your Arcane Brilliance fades, your mind returns to normal", MOD )
 
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_unbuff_magearmor subroutine
// Reset buff from mage mage armor spell
// **************************************************************************
public wowmod_unbuff_magearmor( parm[] ){

   new id = parm[0]

   magearmoron[id] = false

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Your Mage Armor fades", MOD )
 
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_unbuff_vanish subroutine
// Reset buff from rogue vanish spell
// **************************************************************************
public wowmod_unbuff_vanish( parm[] ){

   new id = parm[0]

   vanishon[id] = false
   wowmod_effect_glow( id, GLOW_NORMAL )

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Vanish has faded, you begin to re-appear", MOD )
 
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_unbuff_evasion subroutine
// Reset buff from rogue evasion spell
// **************************************************************************
public wowmod_unbuff_evasion( parm[] ){

   new id = parm[0]

   evasionon[id] = false

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Your Evasion has faded, your dodge rating returns to normal", MOD )
 
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_unbuff_sprint subroutine
// Reset buff from rogue sprint spell
// **************************************************************************
public wowmod_unbuff_sprint( parm[] ){

   new id = parm[0]

   sprinton[id] = false
   set_user_maxspeed( id, p_maxspeed[id] )

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] You stop sprinting, your runspeed returns to normal", MOD )
 
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_unbuff_demonskin subroutine
// Reset buff from warlock demon skin spell
// **************************************************************************
public wowmod_unbuff_demonskin( parm[] ){

   new id = parm[0]

   demonskinon[id] = false

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Your Demonic protection fades, you are now more vulnerable to harm", MOD )
 
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_unbuff_tranq subroutine
// Reset buff from druid tranq spell
// **************************************************************************
public wowmod_unbuff_tranq( parm[] ){

   new id = parm[0]

   tranqon[id] = false

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Your Tranquility fades", MOD )
 
   return PLUGIN_HANDLED
   
}


// **************************************************************************
// BEGIN wowmod_unbuff_barkskin subroutine
// Reset buff from druid barkkin spell
// **************************************************************************
public wowmod_unbuff_barkskin( parm[] ){

   new id = parm[0]

   barkon[id] = false

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Your Barkskin dissolves away", MOD )
 
   return PLUGIN_HANDLED
   
}


// **************************************************************************
// BEGIN wowmod_unbuff_roots subroutine
// Reset druid roots spell
// **************************************************************************
public wowmod_unbuff_roots( parm[] ){

   new enemy = parm[0]
   //new speed = parm[1] // replaced by p_maxspeed[]
   new ent = parm[2]

   remove_entity( ent )

   p_rootent[enemy] = 0 
   isrooted[enemy] = false
   stunned[enemy] = false
   set_user_maxspeed( enemy, p_maxspeed[enemy] )
   
   if ( is_user_alive( enemy ) )
      client_print( enemy, print_chat,
         "[%s] The roots fall away and dissolve", MOD )
 
   return PLUGIN_HANDLED
   
}


// **************************************************************************
// BEGIN wowmod_unbuff_giftwild subroutine
// Reset buff from druid Gift of Wild spell
// **************************************************************************
public wowmod_unbuff_giftwild( parm[] ){

   new id = parm[0]

   gifton[id] = false

   // Unbuff armor and attributes
   new abonus = floatround( p_armor[id] / 10.0 ) * ARMOR_BONUS
   p_armor[id] -= GIFTWILD_ARMOR_BUFF
   new aunbuff = ( get_user_armor(id) - abonus > 0 ) ? 
      ( get_user_armor(id) - abonus ) : 0 
   set_user_armor( id, aunbuff )
   p_str[id] -= GIFTWILD_ATT_BUFF
   p_agi[id] -= GIFTWILD_ATT_BUFF
   p_stam[id] -= GIFTWILD_ATT_BUFF
   p_int[id] -= GIFTWILD_ATT_BUFF
   p_spir[id] -= GIFTWILD_ATT_BUFF
   p_melee[id] -= GIFTWILD_ATT_BUFF
   p_marksman[id] -= GIFTWILD_ATT_BUFF
   p_arcane[id] -= GIFTWILD_ATT_BUFF
   p_fire[id] -= GIFTWILD_ATT_BUFF
   p_frost[id] -= GIFTWILD_ATT_BUFF
   p_shadow[id] -= GIFTWILD_ATT_BUFF
   p_nature[id] -= GIFTWILD_ATT_BUFF
   p_holy[id] -= GIFTWILD_ATT_BUFF

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Your Gift of the Wild fades", MOD )
 
   return PLUGIN_HANDLED
   
}


// **************************************************************************
// BEGIN wowmod_unbuff_weakness subroutine
// Reset curse of weakness from mage
// **************************************************************************
public wowmod_unbuff_weakness( parm[] ){

   new id = parm[0]

   isweakened[id] = false

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] You feel your strength return", MOD )
 
   return PLUGIN_HANDLED
   
}


// **************************************************************************
// BEGIN wowmod_unbuff_wingclip subroutine
// Reset curse of wingclip 
// **************************************************************************
public wowmod_unbuff_wingclip( parm[] ){

   new id = parm[0]

   wingclipon[id] = false
   set_user_maxspeed( id, p_maxspeed[id] )

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] You feel your normal runspeed return", MOD )
 
   return PLUGIN_HANDLED
   
}



// **************************************************************************
// BEGIN wowmod_unbuff_apack subroutine
// Reset buff from hunter aspect of the pack spell
// **************************************************************************
public wowmod_unbuff_apack( parm[] ){

   new id = parm[0]

   apackon[id] = false

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] The Aspect of the Pack fades, you feel your speed return to normal", MOD )

   set_user_maxspeed( id, p_maxspeed[id] )

   new friend
   new player[32]
   new numplayers

   get_players( player, numplayers )

   for ( new i = 0; i < numplayers; ++i ){
      friend = player[i]

      if ( id == friend )
         continue

      if ( get_user_team( id ) != get_user_team( friend ) )
         continue

      if ( is_user_alive( friend ) )
         client_print( friend, print_chat, 
            "[%s] The Aspect of the Pack fades, you feel your speed return to normal", MOD )

      set_user_maxspeed( friend, p_maxspeed[friend] )
   }
 
   return PLUGIN_HANDLED
   
}


// **************************************************************************
// BEGIN wowmod_unbuff_awild subroutine
// Reset buff from hunter aspect of the wild spell
// **************************************************************************
public wowmod_unbuff_awild( parm[] ){

   new id = parm[0]
   new nature = parm[1]

   awildon[id] = false

   if ( is_user_alive( id ) )
      client_print( id, print_chat, 
         "[%s] The Aspect of the Wild fades, You feel your affinity to nature lessen", MOD )

   p_nature[id] -= nature

   new friend
   new player[32]
   new numplayers

   get_players( player, numplayers )

   for ( new i = 0; i < numplayers; ++i ){
      friend = player[i]

      if ( id == friend )
         continue

      if ( get_user_team( id ) != get_user_team( friend ) )
         continue

      if ( is_user_alive( friend ) )
         client_print( friend, print_chat, 
            "[%s] The Aspect of the Wild fades, You feel your affinity to nature lessen", MOD )

      p_nature[friend] -= nature
   }
 
   return PLUGIN_HANDLED
   
}


// **************************************************************************
// BEGIN wowmod_unbuff_cheapshot subroutine
// Reset cheapshot 
// **************************************************************************
public wowmod_unbuff_cheapshot( parm[] ){

   new id = parm[0]

   if ( !is_user_connected( id ) )
      return PLUGIN_CONTINUE

   cheapshoton[id] = false
   set_user_maxspeed( id, p_maxspeed[id] )

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] You feel your normal runspeed return", MOD )
 
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_unbuff_frostbolt subroutine
// Reset frostbolt 
// **************************************************************************
public wowmod_unbuff_frostbolt( parm[] ){

   new id = parm[0]

   isfrosted[id] = false
   set_user_maxspeed( id, p_maxspeed[id] )

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] You feel your normal runspeed return", MOD )
 
   return PLUGIN_HANDLED
   
}


// **************************************************************************
// BEGIN wowmod_unbuff_blizzard subroutine
// Reset blizzard runspeed mod 
// **************************************************************************
public wowmod_unbuff_blizzard( parm[] ){

   new id = parm[0]

   blizzardon[id] = false
   set_user_maxspeed( id, p_maxspeed[id] )

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] The chill subsides, you feel your normal runspeed return", MOD )
 
   return PLUGIN_HANDLED
   
}


// **************************************************************************
// BEGIN wowmod_unbuff_blinkdown subroutine
// Reset blinkdown
// **************************************************************************
public wowmod_unbuff_blinkdown( parm[] ){

   new id = parm[0]

   blinkdown[id] = false

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] You have recovered from your last Blink", MOD )
 
   return PLUGIN_HANDLED
   
}


// **************************************************************************
// BEGIN wowmod_unbuff_pwshield subroutine
// Reset buff from priest power word: shield spell
// **************************************************************************
public wowmod_unbuff_pwshield( parm[] ){

   new id = parm[0]

   pwshieldon[id] = false

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Your Power Word: Shield spell fades, you feel less protected", MOD )
 
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_unbuff_speed1 subroutine
// Reset buff from spec abil speed1 spell
// **************************************************************************
public wowmod_unbuff_speed1( parm[] ){

   new id = parm[0]

   spa_speed[id] = 0.0 
   set_user_maxspeed( id, p_maxspeed[id] )

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Your Boots stop glowing, your runspeed returns to normal", MOD )
 
   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_warning_vanish subroutine
// Warning that vanish will fade soon
// **************************************************************************
public wowmod_warning_vanish( parm[] ){

   new id = parm[0]
   new time_left = parm[1]

   if ( is_user_alive( id ) )
      client_print( id, print_chat,
         "[%s] Warning: Your Vanish will fade in %d seconds", MOD, time_left )
 
   return PLUGIN_HANDLED
}




// **************************************************************************
// BEGIN wowmod_calc_resist subroutine
// Calculate the resistance for a specific spell or magic type
// **************************************************************************
public wowmod_calc_resist( id, pf ){

   //log_amx( "calc_resists() B id=(%d) pf=(%d)", id, pf )

   new res = 0

   switch( pf ){
      case PF_ARCANE: res = ( p_arcane[id] > 0 ) ? p_arcane[id] : 0
      case PF_FIRE: res = ( p_fire[id] > 0 ) ? p_fire[id] : 0
      case PF_FROST: res = ( p_frost[id] > 0 ) ? p_frost[id] : 0
      case PF_MARKSMAN: res = ( p_marksman[id] > 0 ) ? p_marksman[id] : 0
      case PF_MELEE: res = ( p_melee[id] > 0 ) ? p_melee[id] : 0
      case PF_NATURE: res = ( p_nature[id] > 0 ) ? p_nature[id] : 0
      case PF_SHADOW: res = ( p_shadow[id] > 0 ) ? p_shadow[id] : 0
      case PF_HOLY: res = ( p_holy[id] > 0 ) ? p_holy[id] : 0
   }

   //log_amx( "calc_resists() A1 id=(%d) pf=(%d) res=(%d)", id, pf, res )

   if ( p_race[id] == RACE_BLOODELF ){
      res += BELF_BASE_RESIST
   }else if ( ( p_race[id] == RACE_UNDEAD ) && ( pf == PF_SHADOW ) ){
      res += UNDEAD_BASE_RESIST
   }else if ( ( p_race[id] == RACE_HUMAN ) && ( pf == PF_MELEE ) ){
      res += HUMAN_BASE_RESIST
   }else if ( ( p_race[id] == RACE_GNOME ) && ( pf == PF_ARCANE ) ){
      res += GNOME_BASE_RESIST
   }else if ( ( (p_race[id] == RACE_NIGHTELF) || (p_race[id] == RACE_TAUREN) ) && ( pf == PF_NATURE ) ){
      res += NATURE_BASE_RESIST
   }

   if ( p_spells[id][SPELL_MAGE_BASE_IDX+SP_MAGEARMOR] > 0 ){
      new magearmor = p_spells[id][SPELL_MAGE_BASE_IDX+SP_MAGEARMOR]
      new buff = BUFF_MAGEARMOR * magearmor
      res += buff

      //client_print( id, print_chat, "calc_resists() buff=(%d) res=(%d)", buff, res )
   }

   //log_amx( "calc_resists() A2 id=(%d) pf=(%d) res=(%d)", id, pf, res )
   
   return res
}



// **************************************************************************
// BEGIN wowmod_blink_test 
// Check for bad blink destination
// **************************************************************************
public wowmod_blink_test( parm[] ){
   new id = parm[0]
   new curloc[3]
   new origin[3]
   new destloc[3]
   origin[0] = parm[1]
   origin[1] = parm[2]
   origin[2] = parm[3]
   destloc[0] = parm[4]
   destloc[1] = parm[5]
   destloc[2] = parm[6]
   new cast_mana = parm[7]
   new blink_spell = parm[8]

   get_user_origin( id, curloc )

   if ( curloc[2] == destloc[2] ){
      // Failed blink, z axis should adjust as player falls to ground
      set_hudmessage( 255, 255, 10, -1.0, -0.4, 1, 0.5, 3.0, 0.2, 0.2,5);
      show_hudmessage( id, "Blink Destination Failure, Aborting!!!" );
      set_user_origin( id, origin)
   }else{
      // Blink worked
      blinkdown[id] = true
      p_mana[id] -= cast_mana      
      set_hudmessage( 255, 255, 10, -1.0, -0.4, 1, 0.5, 3.0, 0.2, 0.2,5);
      show_hudmessage( id, "Blink Successful" );

      new args[2]
      args[0] = id
      new Float:ftimer = BLINKDOWN_BASE_TIMER - ( 2.0 * blink_spell )
      set_task( ftimer, "wowmod_unbuff_blinkdown", TASK_BLINK+id, args, 2 );
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_check_blinkrest - Kept from UWC3
// Check for restricted blink destination
// **************************************************************************
public wowmod_check_blinkrest( parm[] ){
   new id = parm[0]
   new mapname[32]
   new bool:punish = false 
   new origin[3]
   get_user_origin( id, origin )
   new x = origin[0]
   new y = origin[1]
   new z = origin[2]
   get_mapname( mapname, 32 )

   if ( !is_user_connected( id ) )
      return

   new Float:porigin[3]
   entity_get_vector( id, EV_VEC_origin, porigin)

   if( PointContents( porigin ) == -6 )
      punish = true

   if ( equali( mapname,"de_dust" ) ){
      if ( z > 220 )
         punish = true
   }else if ( equali( mapname,"awp_assault" ) ){
      if ( z > 520 )
         if ( ( y > 2400 && y < 2600 ) )
            punish = true 
   }else if (equali(mapname,"de_dust_cz")){
      if ( z > 220 )
         punish = true
   }else if (equali(mapname,"de_aztec_cz")){
      if ( z > 300 )
         punish = true
   }else if ( equali( mapname,"cs_assault_upc" ) ){
      if ( z > 650 )
         punish = true
   }else if ( equali( mapname,"de_aztec" ) ){
      if ( z > 300 )
         punish = true
   }else if ( equali( mapname,"de_cbble" ) ){
      if ( z > 315 )
         if ( !( ( x > -1320 && x < -1150 ) && ( y > 2600 && y < 2900 ) ) )
            punish = true
   }else if ( equali( mapname,"de_cbble_cz" ) ){
      if ( z > 315 )
         if ( !( ( x > -1320 && x < -1150 ) && ( y > 2600 && y < 2900 ) ) )
            punish = true
   }else if ( equali( mapname,"cs_assault" ) ){
      if ( z > 700 )
         punish = true
   }else if ( equali( mapname,"cs_militia" ) ){
      if( z > 500 )
         punish = true
   }else if ( equali( mapname,"cs_militia_cz" ) ){
      if ( z > 500 )
         punish = true
   }else if ( equali( mapname,"cs_italy" ) ){
      if ( ( z >-220 ) && ( y < -2128 ) )
         punish = true
      if ( z > 250 ){
         if ( ( x < -1000 && x > -1648 ) && ( y > 1900 && y < 2050 ) )
            punish = true
         if ( ( x < -1552 && x > -1648 ) && ( y > 1520 && y < 2050 ) )
            punish = true
      }
   }else if ( equali( mapname,"cs_italy_cz" ) ){
      if ( y > 2608 )
         punish = true
   }else if ( equali( mapname,"de_dust2" ) ){
      if ( z > 270 )
         punish = true
   }else if ( equali( mapname,"de_dust2_cz" ) ){
      if ( z > 270 )
         punish = true
   }else if ( equali( mapname,"fy_dustworld" ) ){
      if ( z > 82 )
         punish = true
   }else if ( equali( mapname,"fy_pool_day" ) ){
      if ( z > 190 )
         punish = true
   }else if ( equali( mapname,"as_oilrig" ) ){
      if ( x > 1530 && get_user_team(id) == TEAM_CT )
         punish = true
      else if ( x > 1700 )
         punish = true
   }else if ( equali( mapname,"cs_mice_final" ) ){
      if ( ( ( x >= 798 ) && ( x <= 860 ) ) && ( y < -1430 ) )
         punish = true
   }

   if( punish ){
      new name[32]
      get_user_name( id, name,31 )
      client_print( id, print_chat, 
         "[%s] You have been punished for entering a restricted area", MOD )
      set_user_health_log( id, -1, true, "wowmod_check_blinkrest() condition=slay")
   }

}


// **************************************************************************
// BEGIN wowmod_specabil_speed1 subroutine
// Runspeed enhancing boots
// **************************************************************************
public wowmod_specabil_speed( id, Float:speed ){

   if ( get_user_maxspeed( id ) > SPECABIL_SPEED1_SPEED ){
      client_print( id, print_chat,
         "[%s] You are already affected by a spell with a faster runspeed", MOD )
      return PLUGIN_HANDLED
   }

   spa_speed[id] = speed 

   client_print( id, print_chat,
      "[%s] Your Boots glow, you start to run faster", MOD )

   set_user_maxspeed( id, speed )

   new parm[2]
   parm[0] = id
   new Float:ftimer = SPECABIL_DURATION1
   set_task( ftimer, "wowmod_unbuff_speed1", TASK_SPA_SPEED1+id, parm, 2 )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_specabil_hearthstone - Old UWC3 do_gate() routine
// Hearthstone teleportation routine
// **************************************************************************
public wowmod_specabil_hearthstone( id ){

   new numplayers = 0
   new Float:vicinity = 96.0
   new entsFound[1]
   new Float:origin[3]
   new porigin[3]
   new szEnt[32]
   new bool:spotOpen = false
   new team = get_user_team(id)

   if (team == TEAM_T)
      copy(szEnt, 31, "info_player_deathmatch")
   else if (team == TEAM_CT)
      copy(szEnt, 31, "info_player_start")
   else
      return PLUGIN_HANDLED

   new curEnt = find_ent_by_class( -1, szEnt );

   while ( curEnt > 0 ){
      numplayers = 0

      entity_get_vector( curEnt, EV_VEC_origin, origin)
      numplayers = find_sphere_class(0, "player", vicinity, entsFound, 1, origin)

      if (numplayers == 0){
         spotOpen = true
         break
      }
      curEnt = find_ent_by_class( curEnt, szEnt );
   }

   if (spotOpen){
      porigin[0] = floatround(origin[0])
      porigin[1] = floatround(origin[1])
      porigin[2] = floatround(origin[2])

      new parm2[6]
      parm2[0] = porigin[0]
      parm2[1] = porigin[1]
      parm2[2] = porigin[2]
      parm2[3] = 0
      parm2[4] = team
      parm2[5] = id
      set_task(0.1, "wowmod_transport_player", 2, parm2, 6)

      // [09-05-04] - Display gate sprite (tele) 1 cycle
      new idorigin[3]
      get_user_origin(id, idorigin)

      message_begin(MSG_ALL, SVC_TEMPENTITY)
      write_byte(TE_SPRITE)
      write_coord( idorigin[0] )
      write_coord( idorigin[1] )
      write_coord( idorigin[2] )
      write_short( gatespr )
      write_byte( 10 )
      write_byte( 255 )
      message_end()

   }else{
      client_print(id, print_chat, "[%s] Your gateway failed to open", MOD)
      return PLUGIN_HANDLED
   }

   return PLUGIN_HANDLED
}


// **************************************************************************
// BEGIN wowmod_transport_player - Old UWC3 transport_gate() routine
// Hearthstone teleportation routine continued
// **************************************************************************
public wowmod_transport_player( parm[] ){
   new id = parm[5]
   new origin[3]
   new oldLocation[3]
   new Float:forigin[3]
   new Float:vicinity = 96.0
   new entsFound[1]

   origin[0] = parm[0]
   origin[1] = parm[1]
   origin[2] = parm[2]

   forigin[0] = float(parm[0])
   forigin[1] = float(parm[1])
   forigin[2] = float(parm[2])

   // Perform a final check to make sure this MOLE spot wasn't taken
   new numplayers = find_sphere_class(0, "player", vicinity, entsFound, 1, forigin)

   if (numplayers > 0){
      client_print(id, print_chat, "[%s] Your Hearthstone Ring shimmers briefly then darkens, it is unable to open a gateway home", MOD)
      return PLUGIN_HANDLED
   }

   client_print(id, print_chat, "[%s] Your Hearthestone Ring shimmers brightly, a gateway opens to take you home", MOD)

   get_user_origin(id, oldLocation)
   oldLocation[2] += 30

   wowmod_sound_playsound_allnear( id, SOUND_HEARTHSTONE )

   // [09-05-04] - Display gate sprite (tele) 1 cycle
   message_begin( MSG_ALL, SVC_TEMPENTITY )
   write_byte( TE_SPRITE )
   write_coord( origin[0] )
   write_coord( origin[1] )
   write_coord( origin[2] )
   write_short( gatespr )
   write_byte( 10 )
   write_byte( 255 )
   message_end()

   // blast circles
   message_begin( MSG_PAS, SVC_TEMPENTITY, oldLocation )
   write_byte( TE_BEAMCYLINDER )
   write_coord( oldLocation[0])
   write_coord( oldLocation[1])
   write_coord( oldLocation[2] + 10)
   write_coord( oldLocation[0])
   write_coord( oldLocation[1])
   write_coord( oldLocation[2] + 10 + TELEPORT_RADIUS)
   write_short( m_iSpriteTexture )
   write_byte( 0 )      // startframe
   write_byte( 0 )      // framerate
   write_byte( 3 )      // life
   write_byte( 60 )     // width
   write_byte( 0 )      // noise
   write_byte( 255 )    // red
   write_byte( 255 )    // green
   write_byte( 255 )    // blue
   write_byte( 255 )    //brightness
   write_byte( 0 )      // speed
   message_end()

   set_user_origin(id,origin)

   wowmod_sound_playsound( id, SOUND_BLINK )

   message_begin(MSG_ONE,gmsgShake,{0,0,0},id)
   write_short(255<< 14 ) //ammount
   write_short(10 << 14) //lasts this long
   write_short(255<< 14) //frequency
   message_end()

   return PLUGIN_CONTINUE
}


// **************************************************************************
// END WowMOD Spells Module 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

