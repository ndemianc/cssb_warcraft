/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Help System
*** Date:          Aug 20, 2007
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
// BEGIN WowMOD Help System 
// **************************************************************************


// **************************************************************************
// BEGIN wowmod_help subroutine
// Help interface front end
// **************************************************************************
public wowmod_help( id, topic[] ){
   new temp[3072]
   new temp2[2048]
   new list[1024]
   new pos = 0
   new spindex = 0
   new index = 0

   if ( equali( topic, "" ) ){
      wowmod_info_helpmain( id )
      return PLUGIN_HANDLED
   }

   client_print( id, print_chat, "[%s] You requested help for the topic '%s'", MOD, topic )

   if ( equali( topic, "Races" ) ){
      copy( topic, 31, "Races" )
      copy( temp2, 2048, help_text[HELP_RACES] )
   }else if ( equali( topic, "Classes" ) ){
      copy( topic, 31, "Classes" )
      copy( temp2, 2048, help_text[HELP_CLASSES] )
   }else if ( equali( topic, "Spells" ) ){
      copy( topic, 31, "Spells" )
      copy( temp2, 2048, help_text[HELP_SPELLS] )
   }else if ( equali( topic, "Nightelf" ) || equali( topic, "Nightelves" ) ){
      copy( topic, 31, "Nightelf Race" )
      wowmod_info_classlist( list, RACE_NIGHTELF )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_RACE_NIGHTELF], list )
   }else if ( equali( topic, "Gnome" ) || equali( topic, "Gnomes" ) ){
      copy( topic, 31, "Gnomish Race" )
      wowmod_info_classlist( list, RACE_GNOME )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_RACE_GNOME], list )
   }else if ( equali( topic, "Dwarf" ) || equali( topic, "Dwarves" ) ){
      copy( topic, 31, "Dwarven Race" )
      wowmod_info_classlist( list, RACE_DWARF )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_RACE_DWARF], list )
   }else if ( equali( topic, "Human" ) || equali( topic, "Humans" ) ){
      copy( topic, 31, "Human Race" )
      wowmod_info_classlist( list, RACE_HUMAN )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_RACE_HUMAN], list )
   }else if ( equali( topic, "Orc" ) || equali( topic, "Orcs" ) ){
      copy( topic, 31, "Orcish Race" )
      wowmod_info_classlist( list, RACE_ORC )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_RACE_ORC], list )
   }else if ( equali( topic, "Tauren" ) ){
      copy( topic, 31, "Tauren Race" )
      wowmod_info_classlist( list, RACE_TAUREN )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_RACE_TAUREN], list )
   }else if ( equali( topic, "Undead" ) ){
      copy( topic, 31, "Undead Race" )
      wowmod_info_classlist( list, RACE_UNDEAD )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_RACE_UNDEAD], list )
   }else if ( equali( topic, "Bloodelf" ) || equali( topic, "Bloodelves" ) ){
      copy( topic, 31, "Bloodelf Race" )
      wowmod_info_classlist( list, RACE_BLOODELF )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_RACE_BLOODELF], list )
   }else if ( equali( topic, "Druid" ) || equali( topic, "Druids" ) ){
      copy( topic, 31, "Druid Class" )
      wowmod_info_spelllist( list, CLASS_DRUID )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_CLASS_DRUID], list )
   }else if ( equali( topic, "Hunter" ) || equali( topic, "Hunters" ) ){
      copy( topic, 31, "Hunter Class" )
      wowmod_info_spelllist( list, CLASS_HUNTER )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_CLASS_HUNTER], list )
   }else if ( equali( topic, "Mage" ) || equali( topic, "Mages" ) ){
      copy( topic, 31, "Mage Class" )
      wowmod_info_spelllist( list, CLASS_MAGE )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_CLASS_MAGE], list )
   }else if ( equali( topic, "Rogue" ) || equali( topic, "Rogues" ) ){
      copy( topic, 31, "Rogue Class" )
      wowmod_info_spelllist( list, CLASS_ROGUE )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_CLASS_ROGUE], list )
   }else if ( equali( topic, "Warlock" ) || equali( topic, "Warlocks" ) ){
      copy( topic, 31, "Warlock Class" )
      wowmod_info_spelllist( list, CLASS_WARLOCK )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_CLASS_WARLOCK], list )
   }else if ( equali( topic, "Priest" ) || equali( topic, "Priests" ) ){
      copy( topic, 31, "Priest Class" )
      wowmod_info_spelllist( list, CLASS_PRIEST )
      format( temp2, 2048, "%s <br />%s", help_text[HELP_CLASS_PRIEST], list )
   }else if ( equali( topic, druid_spells[SP_HEALTOUCH] ) ){
      spindex = SP_HEALTOUCH
      index = HELP_SPELLS_BASE + SPELL_DRUID_BASE_IDX + spindex 
      copy( topic, 31, druid_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, druid_spells[SP_WRATH] ) ){
      spindex = SP_WRATH
      index = HELP_SPELLS_BASE + SPELL_DRUID_BASE_IDX + spindex 
      copy( topic, 31, druid_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, druid_spells[SP_REMCURSE] ) ){
      spindex = SP_REMCURSE
      index = HELP_SPELLS_BASE + SPELL_DRUID_BASE_IDX + spindex 
      copy( topic, 31, druid_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, druid_spells[SP_GIFTWILD] ) ){
      spindex = SP_GIFTWILD
      index = HELP_SPELLS_BASE + SPELL_DRUID_BASE_IDX + spindex 
      copy( topic, 31, druid_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, druid_spells[SP_THORNS] ) ){
      spindex = SP_THORNS
      index = HELP_SPELLS_BASE + SPELL_DRUID_BASE_IDX + spindex 
      copy( topic, 31, druid_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, druid_spells[SP_TRANQ] ) ){
      spindex = SP_TRANQ
      index = HELP_SPELLS_BASE + SPELL_DRUID_BASE_IDX + spindex 
      copy( topic, 31, druid_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, druid_spells[SP_BARKSKIN] ) ){
      spindex = SP_BARKSKIN
      index = HELP_SPELLS_BASE + SPELL_DRUID_BASE_IDX + spindex 
      copy( topic, 31, druid_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, druid_spells[SP_ROOTS] ) ){
      spindex = SP_ROOTS
      index = HELP_SPELLS_BASE + SPELL_DRUID_BASE_IDX + spindex 
      copy( topic, 31, druid_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, hunter_spells[SP_SSTING] ) ){
      spindex = SP_SSTING
      index = HELP_SPELLS_BASE + SPELL_HUNTER_BASE_IDX + spindex 
      copy( topic, 31, hunter_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, hunter_spells[SP_ACHEETAH] ) ){
      spindex = SP_ACHEETAH
      index = HELP_SPELLS_BASE + SPELL_HUNTER_BASE_IDX + spindex 
      copy( topic, 31, hunter_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, hunter_spells[SP_WINGCLIP] ) ){
      spindex = SP_WINGCLIP
      index = HELP_SPELLS_BASE + SPELL_HUNTER_BASE_IDX + spindex 
      copy( topic, 31, hunter_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, hunter_spells[SP_APACK] ) ){
      spindex = SP_APACK
      index = HELP_SPELLS_BASE + SPELL_HUNTER_BASE_IDX + spindex 
      copy( topic, 31, hunter_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, hunter_spells[SP_AWILD] ) ){
      spindex = SP_AWILD
      index = HELP_SPELLS_BASE + SPELL_HUNTER_BASE_IDX + spindex 
      copy( topic, 31, hunter_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, hunter_spells[SP_COUNTER] ) ){
      spindex = SP_COUNTER
      index = HELP_SPELLS_BASE + SPELL_HUNTER_BASE_IDX + spindex 
      copy( topic, 31, hunter_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, hunter_spells[SP_AMONKEY] ) ){
      spindex = SP_AMONKEY
      index = HELP_SPELLS_BASE + SPELL_HUNTER_BASE_IDX + spindex 
      copy( topic, 31, hunter_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, hunter_spells[SP_AIMED] ) ){
      spindex = SP_AIMED
      index = HELP_SPELLS_BASE + SPELL_HUNTER_BASE_IDX + spindex 
      copy( topic, 31, hunter_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, mage_spells[SP_SCORCH] ) ){
      spindex = SP_SCORCH
      index = HELP_SPELLS_BASE + SPELL_MAGE_BASE_IDX + spindex 
      copy( topic, 31, mage_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, mage_spells[SP_ARCINT] ) ){
      spindex = SP_ARCINT
      index = HELP_SPELLS_BASE + SPELL_MAGE_BASE_IDX + spindex 
      copy( topic, 31, mage_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, mage_spells[SP_FROSTBOLT] ) ){
      spindex = SP_FROSTBOLT
      index = HELP_SPELLS_BASE + SPELL_MAGE_BASE_IDX + spindex 
      copy( topic, 31, mage_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, mage_spells[SP_MAGEARMOR] ) ){
      spindex = SP_MAGEARMOR
      index = HELP_SPELLS_BASE + SPELL_MAGE_BASE_IDX + spindex 
      copy( topic, 31, mage_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, mage_spells[SP_FLAMESTRIKE] ) ){
      spindex = SP_FLAMESTRIKE
      index = HELP_SPELLS_BASE + SPELL_MAGE_BASE_IDX + spindex 
      copy( topic, 31, mage_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, mage_spells[SP_ARCBRIL] ) ){
      spindex = SP_ARCBRIL
      index = HELP_SPELLS_BASE + SPELL_MAGE_BASE_IDX + spindex 
      copy( topic, 31, mage_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, mage_spells[SP_BLIZZARD] ) ){
      spindex = SP_BLIZZARD
      index = HELP_SPELLS_BASE + SPELL_MAGE_BASE_IDX + spindex 
      copy( topic, 31, mage_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, mage_spells[SP_BLINK] ) ){
      spindex = SP_BLINK
      index = HELP_SPELLS_BASE + SPELL_MAGE_BASE_IDX + spindex 
      copy( topic, 31, mage_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, rogue_spells[SP_CHEAPSHOT] ) ){
      spindex = SP_CHEAPSHOT
      index = HELP_SPELLS_BASE + SPELL_ROGUE_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, rogue_spells[SP_SPRINT] ) ){
      spindex = SP_SPRINT
      index = HELP_SPELLS_BASE + SPELL_ROGUE_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, rogue_spells[SP_VANISH] ) ){
      spindex = SP_VANISH
      index = HELP_SPELLS_BASE + SPELL_ROGUE_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, rogue_spells[SP_BACKSTAB] ) ){
      spindex = SP_BACKSTAB
      index = HELP_SPELLS_BASE + SPELL_ROGUE_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, rogue_spells[SP_RUPTURE] ) ){
      spindex = SP_RUPTURE
      index = HELP_SPELLS_BASE + SPELL_ROGUE_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, rogue_spells[SP_EVASION] ) ){
      spindex = SP_EVASION
      index = HELP_SPELLS_BASE + SPELL_ROGUE_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, rogue_spells[SP_GARROTTE] ) ){
      spindex = SP_GARROTTE
      index = HELP_SPELLS_BASE + SPELL_ROGUE_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, rogue_spells[SP_EVISCERATE] ) ){
      spindex = SP_EVISCERATE
      index = HELP_SPELLS_BASE + SPELL_ROGUE_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, warlock_spells[SP_DRAINLIFE] ) ){
      spindex = SP_DRAINLIFE
      index = HELP_SPELLS_BASE + SPELL_WARLOCK_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, warlock_spells[SP_CURSEWEAK] ) ){
      spindex = SP_CURSEWEAK
      index = HELP_SPELLS_BASE + SPELL_WARLOCK_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, warlock_spells[SP_DRAINMANA] ) ){
      spindex = SP_DRAINMANA
      index = HELP_SPELLS_BASE + SPELL_WARLOCK_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, warlock_spells[SP_CORRUPTION] ) ){
      spindex = SP_CORRUPTION
      index = HELP_SPELLS_BASE + SPELL_WARLOCK_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, warlock_spells[SP_DEMONSKIN] ) ){
      spindex = SP_DEMONSKIN
      index = HELP_SPELLS_BASE + SPELL_WARLOCK_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, warlock_spells[SP_LIFETAP] ) ){
      spindex = SP_LIFETAP
      index = HELP_SPELLS_BASE + SPELL_WARLOCK_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, warlock_spells[SP_IMMOLATE] ) ){
      spindex = SP_IMMOLATE
      index = HELP_SPELLS_BASE + SPELL_WARLOCK_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, warlock_spells[SP_SHADOWBOLT] ) ){
      spindex = SP_SHADOWBOLT
      index = HELP_SPELLS_BASE + SPELL_WARLOCK_BASE_IDX + spindex 
      copy( topic, 31, rogue_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, priest_spells[SP_HEAL] ) ){
      spindex = SP_HEAL
      index = HELP_SPELLS_BASE + SPELL_PRIEST_BASE_IDX + spindex 
      copy( topic, 31, priest_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, priest_spells[SP_SMITE] ) ){
      spindex = SP_SMITE
      index = HELP_SPELLS_BASE + SPELL_PRIEST_BASE_IDX + spindex 
      copy( topic, 31, priest_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, priest_spells[SP_LESSERHEAL] ) ){
      spindex = SP_LESSERHEAL
      index = HELP_SPELLS_BASE + SPELL_PRIEST_BASE_IDX + spindex 
      copy( topic, 31, priest_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, priest_spells[SP_PWSHIELD] ) ){
      spindex = SP_PWSHIELD
      index = HELP_SPELLS_BASE + SPELL_PRIEST_BASE_IDX + spindex 
      copy( topic, 31, priest_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, priest_spells[SP_SWPAIN] ) ){
      spindex = SP_SWPAIN
      index = HELP_SPELLS_BASE + SPELL_PRIEST_BASE_IDX + spindex 
      copy( topic, 31, priest_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, priest_spells[SP_MANABURN] ) ){
      spindex = SP_MANABURN
      index = HELP_SPELLS_BASE + SPELL_PRIEST_BASE_IDX + spindex 
      copy( topic, 31, priest_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, priest_spells[SP_PRAYERHEAL] ) ){
      spindex = SP_PRAYERHEAL
      index = HELP_SPELLS_BASE + SPELL_PRIEST_BASE_IDX + spindex 
      copy( topic, 31, priest_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, priest_spells[SP_RESURRECT] ) ){
      spindex = SP_RESURRECT
      index = HELP_SPELLS_BASE + SPELL_PRIEST_BASE_IDX + spindex 
      copy( topic, 31, priest_spells[spindex] )
      format( temp2, 2048, "%s", help_text[index] )
   }else if ( equali( topic, "Inventory" ) || equali( topic, "Equipment" ) ){
      copy( topic, 31, "Inventory" )
      format( temp2, 2048, "%s", help_text[HELP_INVENTORY] )
   }else if ( equali( topic, "Merchants" ) || equali( topic, "Merchant" ) ){
      copy( topic, 31, "Merchants" )
      format( temp2, 2048, "%s", help_text[HELP_MERCHANTS] )
   }else if ( equali( topic, "Attributes" ) || equali( topic, "Attrib" ) || equali( topic, "Strength" ) ||
              equali( topic, "Agility" ) || equali( topic, "Stamina" ) || equali( topic, "Intellect" ) ||
              equali( topic, "Spirit" ) )
   {
      copy( topic, 31, "Attributes" )
      format( temp2, 2048, "%s", help_text[HELP_ATTRIBUTES] )
   }else if ( equali( topic, "Proficiencies" ) || equali( topic, "Prof" ) || equali( topic, "Melee" ) ||
              equali( topic, "Marksman" ) || equali( topic, "Arcane" ) || equali( topic, "Fire" ) ||
              equali( topic, "Frost" ) || equali( topic, "Shadow" ) || equali( topic, "Nature" ) ||
              equali( topic, "Holy" ) )
   {
      copy( topic, 31, "Proficiencies" )
      format( temp2, 2048, "%s", help_text[HELP_PROFICIENCIES] )
   }else if ( equali( topic, "Racial Talents" ) || equali( topic, "Racial Abilities" ) || 
              equali( topic, "Racials" ) || equali( topic, "Racial" ) ) 
   {
      copy( topic, 31, "Racial Abilities" )
      format( temp2, 2048, "%s", help_text[HELP_RACIALS] )
   }else if ( equali( topic, "Shadow Meld" ) || equali( topic, "shadowmeld" ) ){ 
      copy( topic, 31, "Shadow Meld" )
      format( temp2, 2048, "%s", help_text[HELP_RACIAL_SMELD] )
   }else if ( equali( topic, "Escape Artist" ) || equali( topic, "escape" ) || 
              equali( topic, "escapeartist" ) )
   { 
      copy( topic, 31, "Escape Artist" )
      format( temp2, 2048, "%s", help_text[HELP_RACIAL_ESCAPE] )
   }else if ( equali( topic, "Stoneform" ) || equali( topic, "stone" ) ){ 
      copy( topic, 31, "Stoneform" )
      format( temp2, 2048, "%s", help_text[HELP_RACIAL_STONE] )
   }else if ( equali( topic, "Perception" ) ){ 
      copy( topic, 31, "Perception" )
      format( temp2, 2048, "%s", help_text[HELP_RACIAL_PERC] )
   }else if ( equali( topic, "Berzerk" ) ){ 
      copy( topic, 31, "Berzerk" )
      format( temp2, 2048, "%s", help_text[HELP_RACIAL_BERZ] )
   }else if ( equali( topic, "War Stomp" ) || equali( topic, "Stomp" ) || equali( topic, "WarStomp" )){ 
      copy( topic, 31, "War Stomp" )
      format( temp2, 2048, "%s", help_text[HELP_RACIAL_STOMP] )
   }else if ( equali( topic, "Cannibalize" ) || equali( topic, "Cannibal" ) ){ 
      copy( topic, 31, "Cannibalize" )
      format( temp2, 2048, "%s", help_text[HELP_RACIAL_CANN] )
   }else if ( equali( topic, "Mana Tap" ) || equali( topic, "ManaTap" ) || equali( topic, "Tap" ) ){ 
      copy( topic, 31, "Mana Tap" )
      format( temp2, 2048, "%s", help_text[HELP_RACIAL_MANA] )
   }else if ( equali( topic, "Crit" ) || equali( topic, "Critical" ) || 
              equali( topic, "Critical Strike" ) )
   { 
      copy( topic, 31, "Critical Strike" )
      format( temp2, 2048, "%s", help_text[HELP_CRIT] )
   }else if ( equali( topic, "Spell Crit" ) || equali( topic, "Spell Critical" ) || 
              equali( topic, "Spell Critical Strike" ) )
   { 
      copy( topic, 31, "Spell Crit" )
      format( temp2, 2048, "%s", help_text[HELP_SPCRIT] )
   }else if ( equali( topic, "Spell Damage" ) || equali( topic, "Spell Damage Ability" ) ){ 
      copy( topic, 31, "Spell Damage" )
      format( temp2, 2048, "%s", help_text[HELP_SPDMG] )
   }else if ( equali( topic, "Autocast" ) || equali( topic, "Auto Cast" ) ){ 
      copy( topic, 31, "Autocast" )
      format( temp2, 2048, "%s", help_text[HELP_AUTOCAST] )
   }else 
      format( temp2, 32, 
         "No helpfile found for topic '%s', say 'help' to see a list of starting help topics", 
         topic )


   pos += format(temp[pos], 3072-pos, "<body bgcolor=#000000><font color=#FFB000>")
   pos += format(temp[pos], 3072-pos, "<center><table border=1 cellpadding=3 cellspacing=3><tr><td><font color=#FFB000>")

   pos += format(temp[pos], 3072-pos, "<center><b>World of Warcraft Mod (WowMOD)</b><br />" )
   pos += format(temp[pos], 3072-pos, "Help Topic: %s</center><br />", topic )
   pos += format(temp[pos], 3072-pos, "<p>%s</p>", temp2 )
   pos += format(temp[pos], 3072-pos, "</font></td></tr></table></center><br /><br />")

   show_motd( id, temp, "WowMOD Help System" )

   return PLUGIN_HANDLED
}


// **************************************************************************
// END WowMOD Help System 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

