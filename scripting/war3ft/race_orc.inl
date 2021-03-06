/*
*	Race: Orcish Horde Functions
*/

#define CHAINLIGHTNING_DAMAGE			75		// Initial ultimate damage
#define CHAINLIGHTNING_LINEWIDTH		80		// Width of the lightning sprite
#define CRITICAL_STRIKE_CHANCE			0.15	// 15% chance of Critical Strike working

// Initial Ultimate call
public OR_ULT_ChainLightning( iCaster, iTarget, iBodyPart )
{

	// Display the lightning effect
	OR_ULT_ChainEffect( iCaster, iTarget, CHAINLIGHTNING_LINEWIDTH, CHAINLIGHTNING_DAMAGE, iBodyPart );
	

	// Now we need to search for the next "jump"
	new parm[5];
	parm[0] = iTarget;
	parm[1] = CHAINLIGHTNING_DAMAGE;
	parm[2] = CHAINLIGHTNING_LINEWIDTH;
	parm[3] = iCaster;
	parm[4] = iBodyPart;

	set_task( 0.2, "_OR_ULT_ChainLightning", TASK_LIGHTNING + iTarget, parm, 5 );
}

// Called on subsequent ultimate calls ( i.e. lightninghitnext )
public _OR_ULT_ChainLightning( parm[5] )
{
	// parm[0] = enemy
	// parm[1] = damage
	// parm[2] = linewidth
	// parm[3] = iAttacker
	// parm[4] = bodypart

	new iEnemy = parm[0];

	if ( !p_data_b[iEnemy][PB_ISCONNECTED] )
	{
		return;
	}

	new iCaster		= parm[3];
	new iBodyPart	= parm[4];
	new iCasterTeam	= get_user_team( iCaster )

	new vOrigin[3];
	get_user_origin( iEnemy, vOrigin );

	new players[32], numberofplayers;
	get_players( players, numberofplayers, "a" );


	new i, iTarget = 0;
	new iClosestTarget = 0, iClosestDistance = 0;
	new iDistanceBetween = 0;
	new vTargetOrigin[3]

	new bool:bFound = false;  

	// Loop through every player
	for ( i = 0; i < numberofplayers; i++ )
	{
		iTarget = players[i];
		
		// Make sure our target player isn't on the same team!
		if ( get_user_team( iTarget ) != iCasterTeam )
		{
			get_user_origin( iTarget, vTargetOrigin )
			
			iDistanceBetween = get_distance( vOrigin, vTargetOrigin );

			// Verify the user is within range and has not been previously hit
			if ( iDistanceBetween < LIGHTNING_RANGE && !p_data_b[iTarget][PB_LIGHTNINGHIT] )
			{

				// Make sure target isn't immune!
				if ( ULT_CanUserBlockUlt( iTarget )  )
				{
					ULT_RemoveCharge( iTarget );
					
					// Let the iAttacker know the ultimate was blocked!
					ULT_Blocked( iCaster );
					
					// Just say the user was hit so they aren't tried to be hit the next round (possibility of removing multiple charges)
					p_data_b[iTarget][PB_LIGHTNINGHIT] = true;

					 bFound = true;
				}

				// Then we can hit this user!
				else
				{
					
					// This user is closest!! Lets make a note of this...
					if ( iDistanceBetween < iClosestDistance || iClosestTarget == 0 )
					{
						iClosestDistance = iDistanceBetween;
						iClosestTarget = iTarget;
					}

				}//end immune check
			}//end distance/hit check
		}//end team check
	}//end for

	// Then we have a valid target!!!
	if ( iClosestTarget && bFound == false ) 
	{
		// Damage should be decreased by 2/3 on each jump
		parm[1] = floatround(float(parm[2])*2/3);

		// Decrease line width as well
		parm[2] = floatround(float(parm[2])*2/3);
		
		// Display the actual lightning
		OR_ULT_ChainEffect( iCaster, iClosestTarget, parm[2], parm[1], iBodyPart );

		// Lets call this again on our next target!
		parm[0] = iClosestTarget;
		set_task( 0.2, "_OR_ULT_ChainLightning", TASK_LIGHTNINGNEXT + iCaster, parm, 5 );
	}

	// No valid target found - reset all lightning hit variables
	else
	{
		for ( i = 0; i < numberofplayers; i++ )
		{
			iTarget = players[i];

			p_data_b[iTarget][PB_LIGHTNINGHIT] = false;
		}
	}

	return;
}

public OR_ULT_ChainEffect( iCaster, iTarget, iLineWidth, iDamage, iBodyPart )
{

	// Make sure we set this user as hit, otherwise we'll hit him again
	p_data_b[iTarget][PB_LIGHTNINGHIT] = true;
	
	// Damage the user
	WC3_Damage( iTarget, iCaster, iDamage, CSW_LIGHTNING, iBodyPart );
	
	// Create the lightning
	Create_TE_BEAMENTS( iCaster, iTarget, g_iSprites[SPR_LIGHTNING], 0, 15, 10, iLineWidth, 10, 255, 255, 255, 255, 0 );
	
	// Get the target's origin
	new vOrigin[3]
	get_user_origin( iTarget, vOrigin );
	
	// Create an elight on the target
	Create_TE_ELIGHT( iTarget, vOrigin, 100, 255, 255, 255, 10, 0 );

	// Play the lightning sound
	emit_sound( iCaster, CHAN_STATIC, g_szSounds[SOUND_LIGHTNING], 1.0, ATTN_NORM, 0, PITCH_NORM );

	return;
}

// Function checks to see if critical grenades are allowed
bool:OR_CriticalGrenadeAllowed( idUser )
{

	// Need to do the glove check
	if ( ITEM_Has( idUser, ITEM_GLOVES ) > ITEM_NONE && !get_pcvar_num( CVAR_wc3_glove_orc_damage ) )
		return false;

	if (fWorkTalismanPassive(idUser, arrTalismanActive[idUser] == TALISMAN_ITEM_GLOVES) && !get_pcvar_num( CVAR_wc3_glove_orc_damage ) )
		return false;

	// Check to see if we are on a disabled map
	if ( g_bOrcNadesDisabled )
	{
		return false;
	}

	return true;
}

OR_SkillsOffensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace )
{
	static iSkillLevel;

	// Critical Strike
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_CRITICALSTRIKE );
	if ( iSkillLevel > 0 && p_data_b[iAttacker][PB_ISFROZENSKILL] == false)
	{

		if ( random_float(0.01, 1.0 ) <= CRITICAL_STRIKE_CHANCE )
		{
			new iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_CRITICALSTRIKE );

			new iBonusDamage = floatround( float( iDamage ) * p_critical[iSkillLevel-1] );
			
			// Damage our victim
			WC3_Damage( iVictim, iAttacker, iBonusDamage, iWeapon, iHitPlace );
			
			// Make the user glow
			SHARED_Glow( iVictim, iBonusDamage, 0, 0, 0 );
			
			// Lets make a screenfade
			Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 0, 0, g_GlowLevel[iVictim][0] );
		}
	}

	// Critical Grenade
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_CRITICALGRENADE );
	if ( iSkillLevel > 0 && ITEM_Has(iVictim, ITEM_GRENADE_PROTECTION ) == ITEM_NONE 
		&& p_data_b[iAttacker][PB_ISFROZENSKILL] == false)
	{		
		
		// Can only do this if the user has a grenade
		if ( SHARED_IsGrenade( iWeapon ) )
		{
			
			// Then we're clear!!
			if ( OR_CriticalGrenadeAllowed( iAttacker ) )
			{
				//static iMaxHealth,iHealth, iBonusDamage;
				//iMaxHealth = get_user_maxhealth( iVictim );
			//	iHealth = get_user_health( iVictim );
				static iBonusDamage;
				iBonusDamage = floatround( float(iDamage) * p_grenade[iSkillLevel-1] );
				
				//client_print(0, print_chat,"MH: %d | BD: %d | D/MH: %f )",iHealth,iBonusDamage,float( iDamage / iMaxHealth ));
				//client_print(0, print_chat,"H: %d |BD: %d |D: %d |D/H: %f )",iHealth,iBonusDamage,iDamage,float( iDamage / iHealth ));

				// We don't want to do more damage than the user's maximum health
				//if ( iBonusDamage + iDamage >= iMaxHealth )
				//	iBonusDamage = iMaxHealth - ( iDamage + 1 );
				
				// Damage the user!
				WC3_Damage( iVictim, iAttacker, iBonusDamage, iWeapon, iHitPlace );

				// Make the user glow
				SHARED_Glow( iVictim, iBonusDamage, 0, 0, 0 );

				// Lets make a screenfade
				Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 0, 0, g_GlowLevel[iVictim][0] );
			}
		}
	}
}