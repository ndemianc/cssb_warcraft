/*
*	Race: Night Elf Functions
*/

#define	ENTANGLE_TIME		10.0

// Initiate Ultimate
public NE_ULT_Entangle( iCaster, iEnemy )
{

	// Follow the user until they stop moving...
	Create_TE_BEAMFOLLOW( iEnemy, g_iSprites[SPR_TRAIL], 10, 5, 10, 108, 23, 255 );
	
	// User is now stunned so we can't do any other stun abilities
	p_data_b[iEnemy][PB_STUNNED] = true;
	
	// Set the speed of the enemy (this will auto-stun them)
	SHARED_SetSpeed( iEnemy );
	
	// Start waiting for the user to stop...
	new parm[4];
	parm[0] = iEnemy;
	parm[1] = 0;
	parm[2] = 0;
	parm[3] = 0;
	_NE_ULT_EntangleWait( parm );
	
	// Drop the user's weapon
	if ( get_pcvar_num( CVAR_wc3_entangle_drop ) )
	{
		new ammo, clip;
		new iWeapon = get_user_weapon( iEnemy, ammo, clip );
		
		// Only drop the weapon if it is the user's primary weapon
		if ( SHARED_IsPrimaryWeapon( iWeapon ) )
		{
			client_cmd( iEnemy, "drop" );
		}
	}
}



// Wait for the user to stop moving
public _NE_ULT_EntangleWait( parm[4] )
{

	new idUser = parm[0];

	if ( !p_data_b[idUser][PB_ISCONNECTED] )
	{
		return;
	}

	new vOrigin[3];
	get_user_origin( idUser, vOrigin );
	
	// Checking to see if the user has actually stopped yet?
	if ( vOrigin[0] == parm[1] && vOrigin[1] == parm[2] && vOrigin[2] == parm[3] )
	{
		SHARED_SetSpeed( idUser );

		// Reset the user's speed in ENTANGLE_TIME amount of time
		set_task( ENTANGLE_TIME, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + idUser );
		
		// Entangle the user
		NE_ULT_EntangleEffect( idUser )
	}

	// If not lets run another check in 0.1 seconds
	else
	{
		parm[1] = vOrigin[0];
		parm[2] = vOrigin[1];
		parm[3] = vOrigin[2];

		set_task( 0.1, "_NE_ULT_EntangleWait", TASK_ENTANGLEWAIT + idUser, parm, 4 );
	}
	return;
}

public NE_ULT_EntangleEffect( idUser )
{

	// Get the user's origin
	new vOrigin[3];
	get_user_origin( idUser, vOrigin );
	
	// Play the entangle sound
	emit_sound( idUser, CHAN_STATIC, g_szSounds[SOUND_ENTANGLING], 1.0, ATTN_NORM, 0, PITCH_NORM );

	new iStart[3], iEnd[3], iHeight;
	new iRadius	= 20, iCounter = 0;
	new x1, y1, x2, y2;

	// Some sweet crap that I don't understand courtesy of SpaceDude - draws the "cylinder" around the player
	while ( iCounter <= 7 )
	{
		if ( iCounter == 0 || iCounter == 8 )
			x1 = -iRadius;
		else if ( iCounter == 1 || iCounter == 7 )
			x1 = -iRadius * 100/141;
		else if ( iCounter == 2 || iCounter == 6 )
			x1 = 0;
		else if ( iCounter == 3 || iCounter == 5 )
			x1 = iRadius*100/141
		else if ( iCounter == 4 )
			x1 = iRadius

		if ( iCounter <= 4 )
			y1 = sqroot( iRadius*iRadius-x1*x1 );
		else
			y1 = -sqroot( iRadius*iRadius-x1*x1 );

		++iCounter;

		if ( iCounter == 0 || iCounter == 8 )
			x2 = -iRadius;
		else if ( iCounter == 1 || iCounter==7 )
			x2 = -iRadius*100/141;
		else if ( iCounter == 2 || iCounter==6 )
			x2 = 0;
		else if ( iCounter == 3 || iCounter==5 )
			x2 = iRadius*100/141;
		else if ( iCounter == 4 )
			x2 = iRadius;
		
		if ( iCounter <= 4 )
			y2 = sqroot( iRadius*iRadius-x2*x2 );
		else
			y2 = -sqroot( iRadius*iRadius-x2*x2 );

		iHeight = 16 + 2 * iCounter;

		while ( iHeight > -40 )
		{
			
			iStart[0]	= vOrigin[0] + x1;
			iStart[1]	= vOrigin[1] + y1;
			iStart[2]	= vOrigin[2] + iHeight;
			iEnd[0]		= vOrigin[0] + x2;
			iEnd[1]		= vOrigin[1] + y2;
			iEnd[2]		= vOrigin[2] + iHeight + 2;
			
			Create_TE_BEAMPOINTS( iStart, iEnd, g_iSprites[SPR_BEAM], 0, 0, ( floatround( ENTANGLE_TIME ) * 10 ), 10, 5, 10, 108, 23, 255, 0 );

			iHeight -= 16;
		}
	}

	return;
}

//Evasion - вероятность в % избежать получение ущерба от выстрела противника.
//Навык: Уклонение (Evasion), Раса: Ночные Эльфы (Night Elves of Kalimdor)
NE_Evasion( idUser, iHitZone )
{
	static iSkillLevel;

	// Check to see if they should evade this shot?
	iSkillLevel = SM_GetSkillLevel( idUser, SKILL_EVASION );

	if ( !p_data_b[idUser][PB_HEXED] && iSkillLevel > 0 && random_float( MIN_PERCENT_EVASION, MAX_PERCENT_EVASION ) <= p_evasion[iSkillLevel-1] 
		&& p_data_b[idUser][PB_ISFROZENSKILL] == false)
	{
		new iGlowIntensity = random_num( 20, 50 );
		
		// Head shot
		if ( iHitZone & (1 << HITGROUP_HEAD) )
		{
			iGlowIntensity += 250;
		}

		// Chest
		else if ( iHitZone & (1 << HITGROUP_CHEST) )
		{
			iGlowIntensity += 75;
		}
		
		// Make the user glow!
		SHARED_Glow( idUser, 0, 0, iGlowIntensity, 0 );

		Create_ScreenFade( idUser, (1<<10), (1<<10), (1<<12), 0, 0, 255, g_GlowLevel[idUser][2] );

		return 1;
		
	}

	return 0;
}

//Trueshot Aura - дает дополнительный % к наносимому вашими пулями ущербу.
//Навык: Аура правдивости (Trueshot Aura), Раса: Ночные Эльфы (Night Elves of Kalimdor)
//Скилл 3 уровня
NE_SkillsOffensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace )
{
	static iSkillLevel;

	// Trueshot Aura
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_TRUESHOT );
	if ( iSkillLevel > 0 && p_data_b[iAttacker][PB_ISFROZENSKILL] == false)
	{
		static iTempDamage;
		iTempDamage = floatround( float( iDamage / random_num( 2, 25 ) ) * p_trueshot[iSkillLevel-1] );
		
		// Damage the user
		WC3_Damage( iVictim, iAttacker, iTempDamage, iWeapon, iHitPlace );

		// Make the user glow!
		SHARED_Glow( iVictim, ( 2 * iTempDamage ), 0, 0, 0 );

		// Create a screen fade
		Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 0, 0, iTempDamage );
	}
}

//Thorns Aura - наносит противнику % ущерба от нанесенного им вам.
//Навык: Шипастая аура (Thorns Aura), Раса: Ночные Эльфы (Night Elves of Kalimdor)
//Скилл 2 уровня
NE_SkillsDefensive( iAttacker, iVictim, iDamage, iHitPlace )
{
	static iSkillLevel;

	// Thorns Aura ( attacker could be dead... i.e. nade )
	iSkillLevel = SM_GetSkillLevel( iVictim, SKILL_THORNS );
	if ( iSkillLevel > 0 && is_user_alive( iAttacker ) && p_data_b[iAttacker][PB_ISFROZENSKILL] == false)
	{
		static iAdditionalDamage;
		iAdditionalDamage = floatround( float( iDamage / random_num( 2, 25 ) ) * p_thorns[iSkillLevel-1] );
		
		// Damage the user
		WC3_Damage( iAttacker, iVictim, iAdditionalDamage, CSW_THORNS, iHitPlace );

		// Make the user glow!
		SHARED_Glow( iAttacker, ( 3 * iAdditionalDamage ), 0, 0, 0 );
		
		// Create a screen fade
		Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 0, 0, 255, iAdditionalDamage )
	}
}
