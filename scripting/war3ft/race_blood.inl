/*
*	Race: Blood Mage Functions
*/

#define IMMOLATE_DAMAGE			35		// Initial damage done to target players
#define IMMOLATE_DOT_DAMAGE		5		// Damage done on each tick of the ultimate
#define IMMOLATE_DOT			4		// Number of times ultimate ticks

#define BM_PHOENIX_RANGE		750		// Range to award money
#define BANISH_DAMAGE			1
#define BANISH_HOLDTIME			0.7

new bool:g_bPlayerBanished[33];

//Супер навык - Ultimate
public BM_ULT_Immolate( iCaster, iTarget )
{
	emit_sound( iCaster, CHAN_STATIC, g_szSounds[SOUND_IMMOLATE], 0.5, ATTN_NORM, 0, PITCH_NORM );

	// Emit sound and create the fire exploding effect on the player

	new vTargetOrigin[3];
	get_user_origin( iTarget, vTargetOrigin );

	Create_TE_EXPLOSION( vTargetOrigin, vTargetOrigin, g_iSprites[SPR_IMMOLATE], 20, 24, 4 );
	Create_TE_EXPLOSION( vTargetOrigin, vTargetOrigin, g_iSprites[SPR_BURNING], 30, 24, 4 );

	Create_ScreenShake( iTarget, (10<<12), (2<<12), (5<<12) );

	// Do initial immolate damage and make their screen shake a little

	WC3_Damage( iTarget, iCaster, IMMOLATE_DAMAGE, CSW_IMMOLATE, -1 )

	new parm_DoT[3];
	parm_DoT[0] = iCaster;
	parm_DoT[1] = iTarget;
	parm_DoT[2] = 1;

	// Start the ultimate DoT

	new TaskId = TASK_BURNING + iTarget;
	set_task( 1.0, "BM_ULT_Immolate_DoT", TaskId, parm_DoT, 3 );
	
	return PLUGIN_HANDLED;
}

public BM_ULT_Immolate_DoT( parm_DoT[3] )
{
	new iCaster = parm_DoT[0];
    new iTarget = parm_DoT[1];
	new iCounter = parm_DoT[2];
	
	// Stop DoT if the max number of ticks is reached

	if ( iCounter > IMMOLATE_DOT ) 
	{
		BM_ULT_Immolate_Remove( iTarget );
		return PLUGIN_HANDLED;
	}

	// Emit sound and show the burning effect on the player

    new vTargetOrigin[3];
    get_user_origin( iTarget, vTargetOrigin );

	emit_sound( iTarget, CHAN_STATIC, g_szSounds[SOUND_IMMOLATE_BURNING], 0.5, ATTN_NORM, 0, PITCH_NORM );

	Create_TE_SPRITE( vTargetOrigin, g_iSprites[SPR_FIRE], 3, 200 );

	// Do the DoT damage

	WC3_Damage( iTarget, iCaster, IMMOLATE_DOT_DAMAGE, CSW_IMMOLATE, -1 )

	// If the target is still alive after this, make their screen glow orange and start the task again

    if ( is_user_alive( iTarget ) )
    {
		p_data[iTarget][PB_ISBURNING] = true;

        Create_ScreenFade( iTarget, (1<<10), (1<<10), (1<<12), 255, 108, 0, 160 );

		parm_DoT[2]++;

		set_task( 1.0, "BM_ULT_Immolate_DoT", TASK_BURNING + iTarget, parm_DoT, 3 );

	}

	return PLUGIN_HANDLED;
}


BM_ULT_Immolate_Remove( iTarget ) 
{
	// Clear global variables and stop the DoT task

	p_data[iTarget][PB_ISBURNING] = false;

	new TaskId = TASK_BURNING + iTarget;
    remove_task( TaskId, 0 );

    return PLUGIN_HANDLED;
}

// Check to see if a player will become a Phoenix
//Скил 1 уровня - Феникс (Phoenix)
BM_PhoenixCheck( idUser )
{
	
	new iSkillLevel = SM_GetSkillLevel( idUser, SKILL_PHOENIX );
	
	//client_print(idUser,print_chat,"S: %d",iSkillLevel);

	// Then the user could become a phoenix!
	if ( iSkillLevel > 0 && p_data_b[idUser][PB_ISFROZENSKILL] == false)
	{
		// Should the user be a Phoenix
		if ( random_float(MIN_PERCENT_PHOENIX, MAX_PERCENT_PHOENIX ) <= p_phoenix[iSkillLevel-1] ) 
		{
			p_data_b[idUser][PB_PHOENIX] = true;
		}
	}

	// Then the user shouldn't be a phoenix!
	else
	{
		p_data_b[idUser][PB_PHOENIX] = false;
	}
}

// This function is called when a user dies
BM_PhoenixSpawn( idUser )
{
	
	// If this user is about to respawn, then we don't even want to check
	if ( p_data[idUser][P_RESPAWNBY] || g_EndRound )
	{
		return;
	}

	new iTeam = get_user_team( idUser );
	new iPhoenixID = BM_PhoenixExists( iTeam );
	
	// Then we have a Phoenix!!
	if ( iPhoenixID != -1 )
	{

		// Lets make sure they're not going to revive themself
		if ( iPhoenixID != idUser )
		{
			// The iAttacker is no longer a Phoenix
			p_data_b[iPhoenixID][PB_PHOENIX] = false;
			
			// The user is about to respawn
			p_data[idUser][P_RESPAWNBY] = RESPAWN_PHOENIX;
			
			// Respawn the user
			set_task( SPAWN_DELAY, "_SHARED_Spawn", TASK_SPAWN + idUser );

			new szCastername[32], szSpawnerName[32];

			get_user_name( iPhoenixID, szCastername,31 )
			get_user_name( idUser, szSpawnerName, 31 )

			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x03%s ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				szCastername,
				LANG_PLAYER,"HAS_REVIVED_YOU");	
			
			cssbColoredPrint(idUser, szMessage);	
		
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"YOU_HAVE_REVIVED",
				szSpawnerName);	
			
			cssbColoredPrint(iPhoenixID, szMessage);
			

		}
	}

	return;
}

// Check to see if a Phoenix exists on a given team
BM_PhoenixExists( iTeam )
{

	new players[32], numberofplayers, idUser, i;
	get_players(players, numberofplayers, "a");

	// Loop through all the players to find someone with phoenix
	for ( i = 0; i < numberofplayers; i++ )
	{
		idUser = players[i];

		if ( get_user_team( idUser ) == iTeam )
		{
			if ( p_data_b[idUser][PB_PHOENIX] && !p_data_b[idUser][PB_HEXED] )
			{
				return idUser;
			}
		}
	}

	return -1;
}

//Скил 2 уровня - Изгнание (Banish)
public _BM_BanishReturn( parm[] )
{
	
	new idUser = parm[0];
	
	if ( !p_data_b[idUser][PB_ISCONNECTED] )
	{
		return;
	}

	// If the round is over we want to move the player back - otherwise they might be moved after they have respawned - that'd be bad
	if ( parm[1] > 0 && !g_EndRound )
	{
		parm[1]--;
		
		// Black screen the user!
		Create_ScreenFade( idUser, 2, (1<<10), (1<<12), 0, 0, 0, 255 );

		set_task( 0.1, "_BM_BanishReturn", TASK_BANISH + idUser, parm, 5 );
	}
	else
	{
		new vOrigin[3];
		vOrigin[0] = parm[2];
		vOrigin[1] = parm[3];
		vOrigin[2] = parm[4];

		// Move the user back!
		SHARED_Teleport( idUser, vOrigin );

		// Make the user glow!
		SHARED_Glow( idUser, 0, 0, 0, 100 );

		// Create a screen fade
		Create_ScreenFade( idUser, 0, 0, 0, 0, 0, 0, 0 );

		// User is no longer banished
		g_bPlayerBanished[idUser] = false;
	}
	
}

//Скил 3 уровня - Похищение брони (Siphon Mana)
BM_SkillsOffensive( iAttacker, iVictim, iDamage )
{
	
	static iSkillLevel;

	// Siphon Mana
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_SIPHONMANA );
	if ( iSkillLevel > 0 && ((get_user_team( iAttacker ) != get_user_team( iVictim ))) && p_data_b[iAttacker][PB_ISFROZENSKILL] == false )
	{
		
		new iMoney = floatround( p_mana[iSkillLevel-1] * SHARED_GetUserMoney(iVictim) );
		
		// Remove the money from the victim
		SHARED_SetUserMoney( iVictim, SHARED_GetUserMoney( iVictim ) - iMoney, 1 );
		
		// Give the money to the attacker
		SHARED_SetUserMoney( iAttacker, SHARED_GetUserMoney( iAttacker ) + iMoney, 1 );

		// Make the user glow!
		//SHARED_Glow( iVictim, 0, iDamage, 0, 0 );
		SHARED_GlowShell(iVictim,0,255,iDamage,get_pcvar_float( CVAR_wc3_glowshell_time ),get_pcvar_num( CVAR_wc3_glowshell_amount ));
		
		// Create a screen fade (purplish)
		Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 144, 58, 255, g_GlowLevel[iAttacker][1] );
	}
}

//Скил 4 уровня -  Шипованная кожа (Resistant Skin)
BM_SkillsDefensive( iAttacker, iVictim, iDamage )
{
	
	static iSkillLevel;
	
/*
	// Resistant Skin - Стандарт
	iSkillLevel = SM_GetSkillLevel( iVictim, PASS_RESISTANTSKIN );
	if ( iSkillLevel > 0 && p_data_b[iAttacker][PB_ISFROZENSKILL] == false)
	{
		new iBonusHealth = floatround( float( iDamage ) * p_resistant[p_data[iVictim][P_LEVEL]] );
		
		if ( p_data_b[iVictim][PB_ISCONNECTED] )
		{
			set_user_health( iVictim, get_user_health( iVictim ) + iBonusHealth );
		}
	}*/

	// Resistant Skin - Не Стандарт
	iSkillLevel = SM_GetSkillLevel( iVictim, PASS_RESISTANTSKIN );
	if ( iSkillLevel > 0 && p_data_b[iAttacker][PB_ISFROZENSKILL] == false)
	{
		new iBonusHealth = floatround( float( iDamage ) * p_resistant[p_data[iVictim][P_LEVEL]] );

		new iHealth = get_user_health( iVictim );
		if((iDamage < iHealth) && (iHealth < 100))
		{
			
			if ( p_data_b[iVictim][PB_ISCONNECTED] )
			{
			
				if((get_user_health( iVictim ) + iBonusHealth) <= 100)
				{
						set_user_health( iVictim, get_user_health( iVictim ) + random_num(1, iBonusHealth ) );
					//	client_print(iVictim, print_chat, "D: %d | BH: %d | PD: %d",floatround( float( iDamage )),iBonusHealth,p_data[iVictim][P_LEVEL]);
				
				}
				
			}
		}
	}

	// Banish
	iSkillLevel = SM_GetSkillLevel( iVictim, SKILL_BANISH );
	if ( iSkillLevel > 0 && p_data_b[iAttacker][PB_ISFROZENSKILL] == false)
	{
		if ( random_float(MIN_PERCENT_BANISH, MAX_PERCENT_BANISH ) <= p_banish[iSkillLevel-1] )
		{

			// Make sure the user isn't banished and that the enemy isn't in the victim's view (victim might be attacking them)
			if ( !g_bPlayerBanished[iAttacker] && !UTIL_CanSeePlayer( iVictim, iAttacker ) )
			{

				// Deal some damage
				WC3_Damage( iAttacker, iVictim, BANISH_DAMAGE, CSW_BANISH, 0 );
				
				// Play the Banish sound
				emit_sound( iAttacker, CHAN_STATIC, g_szSounds[SOUND_BANISH], 1.0, ATTN_NORM, 0, PITCH_NORM );

				// Black screen the user!
				Create_ScreenFade( iAttacker, 2, (1<<10), (1<<12), 0, 0, 0, 255 );

				new vOrigin[3];
				get_user_origin( iAttacker, vOrigin );

				new parm[5];
				parm[0] = iAttacker;
				parm[1] = floatround( BANISH_HOLDTIME * 10.0 );
				parm[2] = vOrigin[0];
				parm[3] = vOrigin[1];
				parm[4] = vOrigin[2];

				// Create the "banish" sprite
				Create_TE_SPRITE( vOrigin, g_iSprites[SPR_BANISH], 10, 255 );

				vOrigin[2] -= 2000;

				set_user_origin( iAttacker, vOrigin );

				set_task( 0.1, "_BM_BanishReturn", TASK_BANISH + iAttacker, parm, 5 );
			}
		}
	}
}