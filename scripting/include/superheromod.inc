// SuperHero Main Include File
// Version 1.18e

#if defined _shero_included
  #endinput
#endif
#define _shero_included

//----------------------------------------------------------------------------------------------
// Changeable DEFINE Settings
//----------------------------------------------------------------------------------------------

//Max player slots, should not really be changed from 32
#define SH_MAXSLOTS 32

//Max levels that can be loaded from the superhero.ini
#define SH_MAXLEVELS 100

//Max Amount of heros that can be loaded
#define SH_MAXHEROS 100

//Potential of power1-power9 but limited by sh_maxbinds (3 default)
#define SH_MAXBINDPOWERS 9

#if !defined AMXX_VERSION
//This define must be un-commented to compile on AMX 0.9.9+
//The define may also be added to the amxconst.inc instead
//#define AMX_NEW

//----------------------------------------------------------------------------------------------
// DO NOT EDIT BELOW HERE
//----------------------------------------------------------------------------------------------

//Sets up a define for AMX 0.9.8a
#if !defined AMX_NEW
  #define AMX98
#endif

#else
  #include <cstrike>
  #include <fun>
#endif

//Sets up AMXX_VERSION_NUM is not defined
#if !defined AMXX_VERSION_NUM
#define AMXX_VERSION_NUM 0
#endif

//----------------------------------------------------------------------------------------------
// IMPORTANT FUNCTIONS - WHAT EVENTS CAN THE SUPERHEROS REGISTER FOR?
//----------------------------------------------------------------------------------------------
stock shModActive()
{
	if (get_cvar_num("sv_superheros") == 0 ) return false

	return true
}
//---------------------------------------------------------------------------------------------
stock shGetNumLevels()
{
	return get_cvar_num("sh_numlevels")
}
//---------------------------------------------------------------------------------------------
stock hasRoundStarted()
{
	// Heros that set speed are going to need to check whether the round started or not...
	if ( get_cvar_num("sh_round_started")!= 0 ) return true
	return false
}
//---------------------------------------------------------------------------------------------
stock shCreateHero(heroName[], heroPower[], heroHelp[], bool:requiresKeyEvents, heroLevel[])
{
	if (requiresKeyEvents ) {
		server_cmd("sh_createHero ^"%s^" ^"%s^" ^"%s^" 1 ^"%s^"", heroName, heroPower, heroHelp, heroLevel )
	}
	else {
		server_cmd("sh_createHero ^"%s^" ^"%s^" ^"%s^" 0 ^"%s^"", heroName, heroPower, heroHelp, heroLevel )
	}
}
//---------------------------------------------------------------------------------------------
stock shRegKeyUp(heroName[], keyUpFunction[])
{
	server_cmd("sh_regKeyUp ^"%s^" ^"%s^"", heroName, keyUpFunction)
}
//---------------------------------------------------------------------------------------------
stock shRegKeyDown(heroName[], keyDownFunction[])
{
	server_cmd("sh_regKeyDown ^"%s^" ^"%s^"", heroName, keyDownFunction)
}
//---------------------------------------------------------------------------------------------
stock shRegLevels(heroName[], levelFunction[] )
{
	server_cmd("sh_regLevels ^"%s^" ^"%s^"", heroName, levelFunction)
}
//---------------------------------------------------------------------------------------------
stock shRegMaxHealth(heroName[], healthFunction[] )
{
	server_cmd("sh_regMaxHealth ^"%s^" ^"%s^"", heroName, healthFunction)
}
//---------------------------------------------------------------------------------------------
stock shRegHeroInit(heroName[], initFunction[] )
{
	server_cmd("sh_regInit ^"%s^" ^"%s^"", heroName, initFunction )
}
//---------------------------------------------------------------------------------------------
stock shSetMaxSpeed(heroName[], heroSpeedCVAR[], heroWeapons[] )
{
	// Hero Weapons is a list of weapons this speed is good for in a wierd delimination
	// "[0]" would equal all weapons
	// "[29][30]" would equal weapons 29,30 etc.
	// By making heroSpeed a CVAR - this can be changed on the fly (or at least between rounds)
	server_cmd("sh_setmaxspeed ^"%s^" ^"%s^" ^"%s^"", heroName, heroSpeedCVAR, heroWeapons )
}
//---------------------------------------------------------------------------------------------
stock shRemSpeedPower(id)
{
	//This function is for when a hero loses his superpowers
	//Init function of hero can call this function which will then attempt to set the hero speed
	//Based on the Max Hero Speed that the Hero now qualifies for
	server_cmd("sh_remspeedpower %d", id )
}
//---------------------------------------------------------------------------------------------
stock shResetSpeed(id)
{
	// A hero might want to set the speed of a hero back to his maximum speed
	server_cmd("sh_setspeedpower %d", id)
}
//---------------------------------------------------------------------------------------------
stock shSetMaxHealth(heroName[], heroHPCVAR[] )
{
	//Hero may have additional HP
	server_cmd("sh_setmaxhealth ^"%s^" ^"%s^"", heroName, heroHPCVAR )
}
//---------------------------------------------------------------------------------------------
stock shRemHealthPower(id)
{
	//This function is for when a hero loses his superpowers
	server_cmd("sh_remhealthpower %d", id )
}
//---------------------------------------------------------------------------------------------
stock shSetMinGravity(heroName[], heroGravityCVAR[] )
{
	//Hero may have less Gravity
	server_cmd("sh_setmingravity ^"%s^" ^"%s^"", heroName, heroGravityCVAR )
}
//---------------------------------------------------------------------------------------------
stock shRemGravityPower(id)
{
	//This function is for when a hero loses his superpowers
	server_cmd("sh_remgravitypower %d", id )
}
//---------------------------------------------------------------------------------------------
stock shSetGravityPower(id)
{
	server_cmd("sh_setgravitypower %d", id)
}
//---------------------------------------------------------------------------------------------
stock shSetMaxArmor(heroName[], heroArmorCVAR[] )
{
	//Hero may give less Gravity
	server_cmd("sh_setmaxarmor ^"%s^" ^"%s^"", heroName, heroArmorCVAR )
}
//---------------------------------------------------------------------------------------------
stock shRemArmorPower(id)
{
	//This function is for when a hero loses his superpowers
	server_cmd("sh_remarmorpower %d", id )
}
//---------------------------------------------------------------------------------------------
stock shSetShieldRestrict(heroName[])
{
	server_cmd("sh_setshieldrest ^"%s^"", heroName)
}
//---------------------------------------------------------------------------------------------
stock shResetShield(id)
{
	server_cmd("sh_resetshield %d", id)
}
//---------------------------------------------------------------------------------------------
stock shAddHPs(id, hitPoints, maxHealth)
{
	if ( !shModActive() ) return

	if ( maxHealth < 100) maxHealth = 100

	new currentHealth = get_user_health(id)

	if ( currentHealth > maxHealth ) maxHealth = currentHealth

	else if ( currentHealth < maxHealth  ) {
		new newHealth = min( currentHealth + hitPoints, maxHealth)
		if ( currentHealth != newHealth ) set_user_health(id, newHealth)
	}
}
//----------------------------------------------------------------------------------------------
// **** NOTICE ****
// This function was changed to use a float in 1.18e, if you get a tag mismatch error
// make sure the last parameter is being sent as a float (1.0 as opposed to 1)
stock shAddXP(id, victim, Float:multiplier = 1.0 )
{
	// Use to add XP for a kill initiated by a hero other than standard kill..
	server_cmd("sh_addxp %d %d ^"%f^"", id, victim, multiplier)
}
//---------------------------------------------------------------------------------------------
stock shStun(id, secs)
{
	// Hero may want to stun <id> <howLong in secs>
	// Up to Hero to set the victims Stun Speed (i.e. set_user_speed(id, 70.0) etc.
	server_cmd("sh_stun %d %d", id, secs)
}
//---------------------------------------------------------------------------------------------
stock shSetGodMode(id, secs)
{
	// Hero may want to set god mode for specified # of seconds
	server_cmd("sh_god %d %d", id, secs)
}
//---------------------------------------------------------------------------------------------
stock shExtraDamage(id, attacker, damage, weaponDescription[], headshot = 0 )
{
	server_cmd("sh_extradamage ^"%d^" ^"%d^" ^"%d^" ^"%s^" ^"%d^"", id, attacker, damage, weaponDescription, headshot )
}
//---------------------------------------------------------------------------------------------
stock shOwnsWeaponID(id, weaponID )
{
	#if defined AMXX_VERSION
	return user_has_weapon(id,weaponID)
	#else

	new iwpn, iwpns[32]
	get_user_weapons(id, iwpns,iwpn)

	for(new a = 0; a < iwpn; a++) {
		if ( weaponID == iwpns[a] ) return true
	}
	return false

	#endif
}
//---------------------------------------------------------------------------------------------
stock shOwnsWeapon(id, weaponName[] )
{
	// Yeah just givem the weapon if they don't already have it
	new iwpn, iwpns[32]
	new ownWeapon[32]

	get_user_weapons(id, iwpns,iwpn)

	for(new a = 0; a < iwpn; a++) {
		get_weaponname(iwpns[a],ownWeapon,31)
		if ( equali(ownWeapon, weaponName) ) return true
	}
	return false
}
//---------------------------------------------------------------------------------------------
stock shGiveWeapon(id, weaponName[], bool:switchTo=false )
{
	if ( !is_user_alive(id) )  return
	if ( !shOwnsWeapon(id, weaponName) ) give_item(id, weaponName)
	if ( switchTo ) engclient_cmd(id, weaponName )
}
//---------------------------------------------------------------------------------------------
stock shGiveWeaponID(id, weaponID, bool:switchTo=false )
{
	if ( weaponID < CSW_P228 || weaponID > CSW_P90 || !is_user_alive(id) )  return

	new weaponName[32]
	get_weaponname(weaponID,weaponName,31)

	if ( !shOwnsWeaponID(id, weaponID) ) give_item(id, weaponName)
	if ( switchTo ) engclient_cmd(id, weaponName )
}
//---------------------------------------------------------------------------------------------
stock shSwitchWeaponID(id, weaponID )
{
	if ( weaponID < CSW_P228 || weaponID > CSW_P90 || !is_user_alive(id)  ) return

	if ( shOwnsWeaponID(id, weaponID) ) {
		new weaponName[32]
		get_weaponname(weaponID,weaponName,31)
		engclient_cmd(id, weaponName )
	}
}
//---------------------------------------------------------------------------------------------
stock shReloadAmmo(id, dropwpn = 0)
{
	server_cmd("sh_reloadammo %d %d", id, dropwpn )
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Default Sounds for heros
//----------------------------------------------------------------------------------------------
stock playSoundDenySelect(id)
{
	client_cmd(id,"spk common/wpn_denyselect.wav")
}
//----------------------------------------------------------------------------------------------
// Simple Functions to Help with Power Cooldowns - Public cause they deal with global variables
// The core will cancel the task on a new spawn, but the hero must reset the variable to false
//----------------------------------------------------------------------------------------------
new gPlayerUltimateUsed[SH_MAXSLOTS+1]
#define ULTIMATE_TASKID 572910
//----------------------------------------------------------------------------------------------
public ultimateTimer(id, Float: secs)
{
	gPlayerUltimateUsed[id] = true
	set_task(secs,"enableUltimate",id+ULTIMATE_TASKID)
	debugMessage("Ultimate Timer Started",id,5)
}
//----------------------------------------------------------------------------------------------
public enableUltimate( id )
{
	id -= ULTIMATE_TASKID
	gPlayerUltimateUsed[id] = false
	debugMessage("Ultimate Timer Ended",id,5)
}
//----------------------------------------------------------------------------------------------
// Useful stock functions
//----------------------------------------------------------------------------------------------
stock sh_setScreenFlash(id, red, green, blue, decisecs, alpha)
{
	setScreenFlash(id, red, green, blue, decisecs, alpha)
}
//---------------------------------------------------------------------------------------------
stock setScreenFlash(id, red, green, blue, decisecs, alpha)
{
	if (!is_user_connected(id)) return
	message_begin(MSG_ONE,get_user_msgid("ScreenFade"),{0,0,0},id)
	write_short( 1<<decisecs ) // fade lasts this long duration
	write_short( 1<<decisecs ) // fade lasts this long hold time
	write_short( 1<<12 ) // fade type (in / out)
	write_byte( red ) // fade red
	write_byte( green ) // fade green
	write_byte( blue ) // fade blue
	write_byte( alpha ) // fade alpha
	message_end()
}
//----------------------------------------------------------------------------------------------
stock sh_screenShake(id, amplitude, duration, frequency )
{
	setScreenShake(id, amplitude, duration, frequency )
}
//----------------------------------------------------------------------------------------------
stock setScreenShake(id, amplitude, duration, frequency )
{
	if (!is_user_connected(id)) return
	message_begin(MSG_ONE,get_user_msgid("ScreenShake"),{0,0,0},id)
	write_short( 1<<amplitude )// amplitude
	write_short( 1<<duration )// duration
	write_short( 1<<frequency )// frequency
	message_end()
}
//----------------------------------------------------------------------------------------------
stock shGlow(id,r,g,b)
{
	if (!is_user_alive(id)) return
	set_user_rendering(id,kRenderFxGlowShell,r,g,b,kRenderNormal,16)
}
//----------------------------------------------------------------------------------------------
stock shUnglow(id)
{
	if (!is_user_alive(id)) return
	set_user_rendering(id)
}
//----------------------------------------------------------------------------------------------
stock getMaxBPAmmo(wpnid) {

	new bpammo = 0
	switch (wpnid) {
		case CSW_P228			: bpammo = 52
		case CSW_SCOUT			: bpammo = 90
		case CSW_HEGRENADE		: bpammo = 1
		case CSW_XM1014		: bpammo = 32
		case CSW_C4			: bpammo = 0
		case CSW_MAC10			: bpammo = 100
		case CSW_AUG			: bpammo = 90
		case CSW_SMOKEGRENADE	: bpammo = 1
		case CSW_ELITE			: bpammo = 120
		case CSW_FIVESEVEN		: bpammo = 100
		case CSW_UMP45			: bpammo = 100
		case CSW_SG550			: bpammo = 90
		case CSW_GALI			: bpammo = 90
		case CSW_FAMAS			: bpammo = 90
		case CSW_USP			: bpammo = 100
		case CSW_GLOCK18		: bpammo = 120
		case CSW_AWP			: bpammo = 30
		case CSW_MP5NAVY		: bpammo = 120
		case CSW_M249			: bpammo = 200
		case CSW_M3			: bpammo = 21
		case CSW_M4A1			: bpammo = 90
		case CSW_TMP			: bpammo = 120
		case CSW_G3SG1			: bpammo = 90
		case CSW_FLASHBANG		: bpammo = 2
		case CSW_DEAGLE		: bpammo = 35
		case CSW_SG552			: bpammo = 90
		case CSW_AK47			: bpammo = 90
		case CSW_KNIFE			: bpammo = 0
		case CSW_P90			: bpammo = 100
	}
	return bpammo
}
//----------------------------------------------------------------------------------------------
stock getMaxClipAmmo(wpnid) {

	new clipammo = 0
	switch (wpnid) {
		case CSW_P228			: clipammo = 13
		case CSW_SCOUT			: clipammo = 10
		case CSW_HEGRENADE		: clipammo = 0
		case CSW_XM1014		: clipammo = 7
		case CSW_C4			: clipammo = 0
		case CSW_MAC10			: clipammo = 30
		case CSW_AUG			: clipammo = 30
		case CSW_SMOKEGRENADE	: clipammo = 0
		case CSW_ELITE			: clipammo = 15
		case CSW_FIVESEVEN		: clipammo = 20
		case CSW_UMP45			: clipammo = 25
		case CSW_SG550			: clipammo = 30
		case CSW_GALI			: clipammo = 35
		case CSW_FAMAS			: clipammo = 25
		case CSW_USP			: clipammo = 12
		case CSW_GLOCK18		: clipammo = 20
		case CSW_AWP			: clipammo = 10
		case CSW_MP5NAVY		: clipammo = 30
		case CSW_M249			: clipammo = 100
		case CSW_M3			: clipammo = 8
		case CSW_M4A1			: clipammo = 30
		case CSW_TMP			: clipammo = 30
		case CSW_G3SG1			: clipammo = 20
		case CSW_FLASHBANG		: clipammo = 0
		case CSW_DEAGLE		: clipammo = 7
		case CSW_SG552			: clipammo = 30
		case CSW_AK47			: clipammo = 30
		case CSW_KNIFE			: clipammo = 0
		case CSW_P90			: clipammo = 50
	}
	return clipammo
}
//----------------------------------------------------------------------------------------------
stock debugMessage( message[], id = 0, level = 1 )
{
	new debugMode = get_cvar_num("sh_debug_messages")
	new output[256]

	if (debugMode < level && level != 0) return

	if (id > 0 && id <= SH_MAXSLOTS) {
		new name[32],authid[32]
		get_user_name(id,name,31)
		get_user_authid(id,authid,31)
		new userid = get_user_userid(id)
		if ( userid > 0) format(output,255,"^"%s<%d><%s>^" %s", name,userid,authid,message)
	}
	else {
		copy(output,255,message)
	}

	if (strlen(output) <= 0 ) return

	#if defined AMXX_VERSION
	format(output,255,"DEBUG: %s",output)
	log_amx(output)
	#else
	format(output,255,"[SH] DEBUG: %s",output)
	server_print(output)
	log_message(output)
	#endif

	return
}
//----------------------------------------------------------------------------------------------
stock isDebugOn()
{
	if ( get_cvar_num("sh_debug_messages") != 0 ) return true
	return false
}
//----------------------------------------------------------------------------------------------
stock Float:maxof(Float: a, Float: b)
{
	if ( a >= b ) return a
	return b
}
//----------------------------------------------------------------------------------------------
stock Float:minof(Float: a, Float: b)
{
	if ( a <= b ) return a
	return b
}
//----------------------------------------------------------------------------------------------
stock sqrt(num)
{
	if (num > 0)	return sqroot(num)
	return 0
}
//----------------------------------------------------------------------------------------------
#if AMXX_VERSION_NUM < 170
stock replace_all(string[],len,what[],with[])
{
	new withlen,charnum = 0
	withlen = strlen(with)
	while ( replace( string[charnum],len,what,with) != 0) {
		charnum += contain(string[charnum],with) + withlen
	}
}
#endif
//----------------------------------------------------------------------------------------------
stock logKill(id, victim, weaponDescription[] )
{

	new namea[32],namev[32],authida[35],authidv[35],teama[16],teamv[16]

	//Info On Attacker
	get_user_name(id,namea,31)
	get_user_team(id,teama,15)
	get_user_authid(id,authida,34)

	//Info On Victim
	get_user_name(victim,namev,31)
	get_user_team(victim,teamv,15)
	get_user_authid(victim,authidv,34)

	//Log This Kill
	if ( id != victim ) {
		log_message("^"%s<%d><%s><%s>^" killed ^"%s<%d><%s><%s>^" with ^"%s^"",
			namea,get_user_userid(id),authida,teama,namev,get_user_userid(victim),authidv,teamv, weaponDescription )
	}
	else {
		log_message("^"%s<%d><%s><%s>^" committed suicide with ^"%s^"",
			namea,get_user_userid(id),authida,teama, weaponDescription )
	}
}
//----------------------------------------------------------------------------------------------
stock strbrkqt(const text[], Left[], leftLen, Right[], rightLen)
{
	//Breaks text[] into two parts, Left[], and Right[]
	// Left[] will contain the first parameter (either quoted or non-quoted)
	// Right[] contain the rest of the string after Left[], not including the space
	new bool:in_quotes = false
	new bool:done_flag = false
	new i, left_pos = 0

	for ( i = 0; i < strlen(text); i++) {

		if (equali(text[i], "^"", 1) && !done_flag) {
			if (in_quotes) {
				done_flag = true
				in_quotes = false
			}
			else in_quotes = true
		}
		else if ( isspace(text[i]) && !in_quotes ) {
			if (left_pos > 0) {
				done_flag = true
			}
		}
		else if (!done_flag && left_pos <= leftLen) {
			setc(Left[left_pos], 1, text[i])
			left_pos++
		}
		else if (done_flag) break
	}

	Left[left_pos] = 0
	copy(Right,rightLen,text[i])

	return true
}
//----------------------------------------------------------------------------------------------
// This is ONLY used to debug runtime 3 errors
stock print_heapsize()
{
	new heapsize[48]
	format(heapsize,47,"[SH] HeapSize: %i",heapspace())
	server_print(heapsize)
}
//----------------------------------------------------------------------------------------------
#if !defined AMXX_VERSION
//
stock is_valid_ent(iIndex)
{
	#if defined AMX_NEW
	return is_entity(iIndex)
	#else
	if (iIndex <= 0) return false
	return true
	#endif
}
//----------------------------------------------------------------------------------------------
// This is not working on AMX 0.9.9+ due to translator issues
//
stock is_plugin_loaded(const name[])
{
	new index = -1
	new pluginsnum = get_pluginsnum()
	new filename[128], plugname[128], junk[2]

	for (new x = 0; x <= pluginsnum; x++) {
		#if defined AMX_NEW
		get_plugin(x,filename,127,plugname,127,junk,0,junk,0,junk,0,junk[0])
		#else
		get_plugin(x,filename,127,plugname,127,junk,0,junk,0,junk,0)
		#endif

		if (equali(name,plugname)) {
			index = x
			break
		}
	}
	return index
}
//
#endif
//----------------------------------------------------------------------------------------------
#if defined AMX_NEW
//
stock Float:floatsin( Float:flInput )
	return Float:sin( Float:flInput )
//----------------------------------------------------------------------------------------------
stock Float:floatcos( Float:flInput )
	return Float:cos( Float:flInput )
//----------------------------------------------------------------------------------------------
stock Float:floattan( Float:flInput )
	return Float:tan( Float:flInput )
//
#endif
//----------------------------------------------------------------------------------------------
#if !defined AMXX_VERSION && !defined _vexdum_stock_included
//
/* Changes an integer vec to a floating vec */
stock IVecFVec(IVec[3], Float:FVec[3])
{
	FVec[0] = float(IVec[0])
	FVec[1] = float(IVec[1])
	FVec[2] = float(IVec[2])

	return 1
}
//----------------------------------------------------------------------------------------------
/* Changes a float vec to an integer vec */
stock FVecIVec(Float:FVec[3], IVec[3])
{
	IVec[0] = floatround(FVec[0])
	IVec[1] = floatround(FVec[1])
	IVec[2] = floatround(FVec[2])

	return 1
}
//
#endif
//----------------------------------------------------------------------------------------------
