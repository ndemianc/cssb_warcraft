/*
*	Race: Warden Functions
*/

#define VENGEANCE_HEALTH		50			// Health the user should have after using his ult
#define SHADOWSTRIKE_DAMAGE		10			// Amount of damage dealt with shadow strike

WA_ULT_Vengeance( idUser )
{
	
	// User can't use ultimate while planting or defusing!
	if ( p_data_b[idUser][PB_ISPLANTING] )
	{
		format(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_WA_ULT_VENGEANCE");
		client_print( idUser, print_center, szMessage);
				
		return;
	}

	// If the user's ultimate is not available, then why are we here?
	if ( !ULT_Available( idUser ) )
	{		
		return;
	}

	// We need to respawn the user!!
	if ( !is_user_alive( idUser ) )
	{

		// Make sure the user isn't changing teams and we're not at the end of the round
		if ( !p_data[idUser][P_RESPAWNBY] )
		{

			// Remove the ultimate delay if it existed (in theory it shouldn't)
			if ( task_exists( TASK_UDELAY + idUser ) )
			{
				remove_task( TASK_UDELAY + idUser );
			}

			// Set up a new ultimate delay
			fResetUltimateTimerDivider(idUser);

			// Set up the respawn task
			set_task( SPAWN_DELAY, "_SHARED_Spawn", TASK_SPAWN + idUser );

			// We need to set that we're about to respawn
			p_data[idUser][P_RESPAWNBY] = RESPAWN_VENGEANCE;
		}
	}
	
	// Otherwise teleport back to spawn
	else
	{		
		// Lets find a free spawn and obey immunity rules
		new ent = SHARED_FindFreeSpawn( idUser, true );
		
		// No spawn found
		if ( ent < 0 )
		{
			//set_hudmessage( 255, 255, 10, -1.0, -0.4, 1, 0.5, 1.5, 0.2, 0.2 ,5 );
			
			// Immune person nearby
			if ( ent == -2 )
			{
				// The user's ultimate was blocked!
				ULT_Blocked( idUser );

				// Set up a new ultimate delay
				fResetUltimateTimerDivider(idUser);

				WC3_StatusText( idUser, 0, "%L", LANG_PLAYER, "VENGEANCE_FAILED_ENEMY_IMMUNITY" );
			}

			// No spawn found
			else
			{
				WC3_StatusText( idUser, 0, "%L", LANG_PLAYER, "NO_FREE_SPAWN_FOUND" );
			}
		}

		// Spawn found!
		else
		{
			new vOrigin[3], vNewOrigin[3], Float:fSpawnOrigin[3];
			
			// Get the user's origin
			get_user_origin( idUser, vOrigin );

			// Get the origin of the spawn
			entity_get_vector( ent, EV_VEC_origin, fSpawnOrigin );
			
			// Convert float vector to int vector
			FVecIVec( fSpawnOrigin, vNewOrigin );
			
			// Move the user
			set_user_origin( idUser, vNewOrigin );

			// Play teleport animation at old location
			Create_TE_TELEPORT( vOrigin );

			// Change user's health
			set_user_health( idUser, VENGEANCE_HEALTH );

			// Play the Vengeance sound		
			emit_sound( idUser, CHAN_STATIC, g_szSounds[SOUND_VENGEANCE], 1.0, ATTN_NORM, 0, PITCH_NORM );

			// Set up a new ultimate delay
			fResetUltimateTimerDivider(idUser);
		}
	}

	return;
}

// User's Blink Ability
WA_Blink( idUser )
{
	
	static iSkillLevel;
	iSkillLevel = SM_GetSkillLevel( idUser, SKILL_BLINK );

	// User has the ability, lets initiate a "check"
	if ( iSkillLevel > 0 && p_data_b[idUser][PB_ISFROZENSKILL] == false)
	{
		// We don't need to check if they already have this!
		if ( p_data_b[idUser][PB_WARDENBLINK] )
		{
			return;
		}

		if ( random_float(MIN_PERCENT_BLINK, MAX_PERCENT_BLINK ) <= p_blink[iSkillLevel-1] )
		{
			p_data_b[idUser][PB_WARDENBLINK] = true;
			
			format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_WA_BLINK");	
			
			cssbColoredPrint(idUser, szMessage);	
		
		}
	}

	// User shouldn't have blink!
	else
	{
		p_data_b[idUser][PB_WARDENBLINK] = false;
	}
}
//Закаленная кожа
WA_HardenedSkin( iVictim, iDamage )
{
	static iSkillLevel;

	// Hardened Skin
	iSkillLevel = SM_GetSkillLevel( iVictim, PASS_HARDENEDSKIN );
	if ( iSkillLevel > 0 && p_data_b[iVictim][PB_ISFROZENSKILL] == false)
	{
		static iLevel;
		iLevel = p_data[iVictim][P_LEVEL];

		return ( floatround( float( iDamage ) * ( MAX_PERCENT_HARDEN - p_harden[iLevel] ) ) );
//		return ( floatround( float( iDamage ) * ( 1 - p_harden[iLevel] ) ) );

	}

	return iDamage;	
}

//Skill 3
//Навык: Скрытый удар (Shadow Strike), Раса: Шпион (Warden)
WA_SkillsOffensive( iAttacker, iVictim, iHitPlace )
{

	static iSkillLevel;

	// Shadow Strike
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_SHADOWSTRIKE );
	if ( iSkillLevel > 0 && p_data_b[iAttacker][PB_ISFROZENSKILL] == false)
	{
		if ( random_float(MIN_PERCENT_SHADOW, MAX_PERCENT_SHADOW ) <= p_shadow[iSkillLevel-1] )		
		{
			new vVictimOrigin[3], vAttackerOrigin[3]
			get_user_origin( iVictim, vVictimOrigin );
			get_user_origin( iAttacker, vAttackerOrigin );
			
			// Create the shadow strike effect
			Create_TE_SPRITETRAIL( vAttackerOrigin, vVictimOrigin, g_iSprites[SPR_SHADOWSTRIKE], 50, 15, 1, 2, 6 );
			
			// Emit the shadow strike sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_SHADOWSTRIKE], 1.0, ATTN_NORM, 0, PITCH_NORM );

			// Damage the user
			//WC3_Damage( iVictim, iAttacker, SHADOWSTRIKE_DAMAGE, CSW_SHADOW, iHitPlace );
			new iShadowStrikeDamage = floatround( 10 * p_shadow[iSkillLevel-1] );
			WC3_Damage( iVictim, iAttacker, iShadowStrikeDamage, CSW_SHADOW, iHitPlace );
		}
		else if ( get_pcvar_num( CVAR_wc3_psychostats ) )
		{
			new WEAPON = CSW_SHADOW - CSW_WAR3_MIN;

			iStatsShots[iAttacker][WEAPON]++;
		}
	}
}