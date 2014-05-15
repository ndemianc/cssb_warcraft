// SuperHero nVault Include File
// Version 1.18e

//----------------------------------------------------------------------------------------------
// Abstract Methods that must be implemented by a save superhero include file...
//
// writeMemoryTable()
// loadXP( id, key[35] )
// cleanXP(clearAll = false)
//----------------------------------------------------------------------------------------------

#if defined _shsave_included || !defined SHCORE || !defined AMXX_VERSION
	#endinput
#endif
#define _shsave_included

//Include the nvault file
#include <nvault>

//Name for binary vault file
#define VAULTNAME "superhero"

//Global to save the vault handle
new gVaultHandle

//----------------------------------------------------------------------------------------------
public saving_init()
{
	gVaultHandle = nvault_open(VAULTNAME)
	if (gVaultHandle == INVALID_HANDLE) {
		format(debugt,255,"Error opening nVault file: %s",VAULTNAME)
		debugMessage(debugt,0,0)
	}
}
//----------------------------------------------------------------------------------------------
public parseXP(id, const data[] )
{
	// "name" XP USERFLAGS "SKILL1" "SKILL2" ...
	new heroIndex
	new Left[32], Right[1501]

	copy(Right, 1500, data)

	//Get the name out
	strbrkqt(Right, Left, 31, Right, 1500)

	//Get the XP out and save it
	strbrkqt(Right, Left, 31, Right, 1500)
	gPlayerXP[id] = str_to_num(Left)
	gPlayerLevel[id] = getLevel(id)
	setLevel(id, gPlayerLevel[id] )

	//Get the User Flags out and save them
	strbrkqt(Right, Left, 31, Right, 1500)
	gPlayerFlags[id] = str_to_num(Left)

	//Now load in all the skills
	new powerCount = 0
	new MaxPowers = min(get_cvar_num("sh_maxpowers"), SH_MAXLEVELS)

	while ( strlen(Right) > 0 && powerCount < MaxPowers ) {
		strbrkqt(Right, Left, 31, Right, 1500)
		heroIndex = findHero(Left)
		if ( heroIndex >= 0 )  {
			if ( getHeroLevel(heroIndex) <= gPlayerLevel[id] ) {
				gPlayerPowers[id][0] = ++powerCount
				gPlayerPowers[id][powerCount] = heroIndex
				initHero(id, heroIndex)
			}
		}
	}
}
//----------------------------------------------------------------------------------------------
public writeMemoryTable()
{
	if ( !get_cvar_num("sh_savexp") ) return

	debugMessage("Writing XP Data to Vault file",0,3)

	// Write Memory Table to File
	for (new x = 1; x < gMemoryTableCount; x++) {
		if ( strlen(gMemoryTableKeys[x]) > 0 ) {
			writeData(x)
		}
		// Can even clear the MemoryKey - if a player is disconnected no reason to save again and again...
		copy(gMemoryTableKeys[x], 34, "")
	}

	// No need to keep disconnected players around if this has been flushed...
	gMemoryTableCount = 33
}
//----------------------------------------------------------------------------------------------
public writeData(id)
{
	// Formats Player Information and writes to file:
	// "name" XP USERFLAGS "SKILL1" "SKILL2" ...
	new data[1501], heroIndex

	format(data, 1500, "^"%s^" %d %d", gMemoryTableNames[id], gMemoryTableXP[id], gMemoryTableFlags[id] )
	for ( new x = 1; x <= gMemoryTablePowers[id][0] && x < gSuperHeroCount; x++ ) {
		heroIndex = gMemoryTablePowers[id][x]
		if ( heroIndex >= 0 && heroIndex < gSuperHeroCount ) {
			add(data, 1500, " ^"")
			add(data, 1500, gSuperHeros[heroIndex][hero])
			add(data, 1500, "^"")
		}
	}

	nvault_set(gVaultHandle, gMemoryTableKeys[id], data)

	if (id > 0 && id <= SH_MAXSLOTS) {
		gChangedHeroes[id] = false
	}
}
//----------------------------------------------------------------------------------------------
public loadXP( id, key[35] )
{
	new data[1501], timestamp

	if (nvault_lookup(gVaultHandle, key, data, 1500, timestamp)) {
		parseXP(id, data)
	}

	updateMemoryTable(id)
	return true
}
//----------------------------------------------------------------------------------------------
public cleanXP(bool:clearAll)
{
	if ( !clearAll && (!get_cvar_num("sh_savexp") || get_cvar_num("sh_xpsavedays") < 0)) return

	if (clearAll)	nvault_prune(gVaultHandle, 0, 0)
	else			nvault_prune(gVaultHandle, 0, get_systime() - (get_cvar_num("sh_xpsavedays") * 24 * 3600))
}
//----------------------------------------------------------------------------------------------
public saving_end()
{
	//Close the vault to save all the data
	nvault_close(gVaultHandle)
}
//----------------------------------------------------------------------------------------------