/*
*	Race: Crypt Lord Functions
*/

#define LOCUSTSWARM_DMG_MIN				30
#define LOCUSTSWARM_DMG_MAX				60

#define CARRIONBEETLE_DAMAGE			10
#define IMPALE_INTENSITY				10.0		// Intensity of impale

// Returns victim idUser
//  -1 if no target
//  -2 if ult blocked
CL_ULT_LocustGetTarget( idUser )
{
	new iTeam = get_user_team( idUser );

	// First thing we need to do is find an alive player that isn't immune to target :)
	new players[32], numberofplayers, i, iTargetID;
	new iTargets[33], iTotalTargets = 0;

	get_players( players, numberofplayers, "a" );

	for ( i = 0; i < numberofplayers; i++ )
	{
		iTargetID = players[i];

		if ( get_user_team( iTargetID ) != iTeam )
		{
			iTargets[iTotalTargets++] = iTargetID;
		}
	}
	
	// No victims found
	if ( iTotalTargets == 0 )
	{
		return -1;
	}

	// Lets find ourselves a victim
	new iVictim = 0, iRandomSpot;
	while ( iVictim == 0 )
	{
		iRandomSpot = random_num( 0, iTotalTargets );

		iVictim = iTargets[iRandomSpot];
	}

	// Then the user's ultimate was blocked :/
	if ( ULT_CanUserBlockUlt( iVictim ) )
	{
		ULT_RemoveCharge( iVictim );
		ULT_Blocked( idUser );

		// Reset the user's ultimate
		fResetUltimateTimerDivider(idUser);

		return -2;
	}

	return iVictim;
}

CL_ULT_LocustSwarm( iAttacker )
{
	new iVictim = CL_ULT_LocustGetTarget( iAttacker );

// No target was found :/
	if ( iVictim == -1 )
	{
		//set_hudmessage( 178, 14, 41, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, -1 );
		format(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_CL_ULT_LOCUST_EFFECT_1");
		WC3_StatusText( iAttacker, TXT_BLINK_CENTER,szMessage);
		
		return;
	}

	// Ultimate was blocked - do nothing
	if ( iVictim == -2 )
	{
		return;
	}

	//Для talisman +
	//Для shopmenu4
	//ITEM_MIRROR_ULTIMATE - Отражатель супер навыка - Mirror Ultimate
	if ( (ITEM_Has(iVictim, ITEM_MIRROR_ULTIMATE ) > ITEM_NONE && ITEM_HasItem( iVictim, ITEM_NECKLACE ) != ITEM_NECKLACE) ||
		 (fWorkTalismanPassive(iVictim, arrTalismanActive[iVictim]) == TALISMAN_ITEM_MIRROR_ULTIMATE && ITEM_HasItem( iVictim, ITEM_NECKLACE ) != ITEM_NECKLACE)
	   )
	{

		if(ITEM_Has(iAttacker, ITEM_NECKLACE ) > ITEM_NONE)
		{
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_MIRROR_ULT_BLOCKED");
			cssbChatColor(iAttacker,"%s%s",fTagWar3ft(),szMessage);

			emit_sound(iVictim, CHAN_STATIC, g_szSounds[SOUND_MIRROR_ULTIMATE_BLOCK], 1.0, ATTN_NORM, 0, PITCH_NORM );
			emit_sound(iAttacker, CHAN_STATIC, g_szSounds[SOUND_MIRROR_ULTIMATE_BLOCK], 1.0, ATTN_NORM, 0, PITCH_NORM );

			ITEM_RemoveCharge(iAttacker, ITEM_NECKLACE );
			ITEM_RemoveCharge(iVictim, ITEM_MIRROR_ULTIMATE );

			ULT_Icon( iAttacker, ICON_HIDE );

			// Reset the cooldown
			fResetUltimateTimerDivider(iAttacker,0,false);

			return;
		}

		formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_MIRROR_ULT");
		cssbChatColor(iAttacker,"%s%s",fTagWar3ft(),szMessage);

		sm4_EffectMirrorUltimate(iVictim);

		// Lets get the origin of the iAttacker
		new vCasterOrigin[3];
		get_user_origin( iAttacker, vCasterOrigin );
		
		ULT_Icon( iAttacker, ICON_HIDE );

		new parm[10];
		parm[0] = iVictim;	// Attacker
		parm[1] = iAttacker;	// Victim
		parm[2] = vCasterOrigin[0];
		parm[3] = vCasterOrigin[1];
		parm[4] = vCasterOrigin[2];
		parm[5] = 1;

		// Reset the cooldown
		fResetUltimateTimerDivider(iAttacker,0,false);

		// Start the effects!!
		_CL_ULT_LocustEffect( parm );

		ITEM_RemoveCharge(iVictim, ITEM_MIRROR_ULTIMATE );
	}
	else
	{
		// Lets get the origin of the iAttacker
		new vCasterOrigin[3];
		get_user_origin( iAttacker, vCasterOrigin );
		
		// Flash their ultimate!
		ULT_Icon( iAttacker, ICON_FLASH );

		new parm[10];
		parm[0] = iAttacker;		// iAttacker
		parm[1] = iVictim;	// victim
		parm[2] = vCasterOrigin[0];
		parm[3] = vCasterOrigin[1];
		parm[4] = vCasterOrigin[2];
		parm[5] = 0;

		// Reset the cooldown
		fResetUltimateTimerDivider(iAttacker,0,false);

		// Start the effects!!
		_CL_ULT_LocustEffect( parm );
	}

	return;
}

public _CL_ULT_LocustEffect( parm[] )
{
	new iAttacker	= parm[0];
	new iVictim		= parm[1];

	if ( !is_user_alive( iVictim ) || !p_data_b[iVictim][PB_ISCONNECTED] )
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_CL_ULT_LOCUST_EFFECT_0");	
			
		cssbColoredPrint(iAttacker, szMessage);	
		
		// Need to find a new victim here...
		//new iVictim = CL_ULT_LocustGetTarget( iAttacker );

		// No target was found :/
		if ( iVictim == -1 )
		{
			//set_hudmessage( 178, 14, 41, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, -1 );
			format(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_CL_ULT_LOCUST_EFFECT_1");
			WC3_StatusText( iAttacker, TXT_BLINK_CENTER,szMessage);
		
			// Set ultimate timer back to 0
			p_data[iAttacker][P_ULTIMATEDELAY] = 0;

			// Show icon + play ultimate ready sound
			ULT_IconHandler( iAttacker );

			return;
		}

		// Ultimate was blocked - do nothing
		else if ( iVictim == -2 )
		{
			ULT_Icon( iAttacker, ICON_HIDE );

			return;
		}

/*
		// Lets get the new origin of the iAttacker
		new vCasterOrigin[3];
		get_user_origin( iAttacker, vCasterOrigin );
		parm[1] = iVictim;	// victim
		parm[2] = vCasterOrigin[0];
		parm[3] = vCasterOrigin[1];
		parm[4] = vCasterOrigin[2];

		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_CL_ULT_LOCUST_EFFECT_2");	
			
		cssbColoredPrint(iAttacker, szMessage);	*/
		
		/*client_print( iAttacker, print_chat, "Victim is no longer targetable, try casting again!" );

		p_data[iAttacker][P_ULTIMATEDELAY] = 0;

		Ultimate_Ready( iAttacker );

		return;*/
	}

	new MULTIPLIER = 150 // the lower the number the faster it reaches the target

	new vVictimOrigin[3], vFunnel[3];
	get_user_origin( iVictim, vVictimOrigin );

	// Origin of the funnel
	vFunnel[0] = parm[2];
	vFunnel[1] = parm[3];
	vFunnel[2] = parm[4];
	
	// Draw a funnel
	Create_TE_LARGEFUNNEL( vFunnel, g_iSprites[SPR_SNOW], 0 );
	
	// Now we need to calculate where the next funnel will be drawn
	new vDist[3];
	vDist[XPOS] = CL_HLP_Diff( vVictimOrigin[0], vFunnel[0] );
	vDist[YPOS] = CL_HLP_Diff( vVictimOrigin[1], vFunnel[1] );
	vDist[ZPOS] = CL_HLP_Diff( vVictimOrigin[2], vFunnel[2] );	
	
	new i;
	for ( i = 0; i < 3; i++ )
	{
		
		if ( CL_HLP_Diff( vVictimOrigin[i], vFunnel[i] - MULTIPLIER ) < vDist[i] )
		{
			vFunnel[i] -= MULTIPLIER;
		}
		else if ( CL_HLP_Diff( vVictimOrigin[i], vFunnel[0] + MULTIPLIER ) < vDist[i] )
		{
			vFunnel[i] += MULTIPLIER;
		}
		else
		{
			vFunnel[i] = vVictimOrigin[i];
		}
	}

	// Now we need to set the parms so (if/when) we call this function again it knows the next origin
	parm[2] = vFunnel[0];
	parm[3] = vFunnel[1];
	parm[4] = vFunnel[2];


	// Still not close enough to our target
	if ( !( vDist[XPOS] < 50 && vDist[YPOS] < 50 && vDist[ZPOS] < 50 ) )
	{
		new Float:fTime = 0.2;

		set_task( fTime, "_CL_ULT_LocustEffect", iAttacker + TASK_FUNNELS, parm, 5 );
	}

	// We're close enough, we can damage them!
	else
	{
		new iDamage = random_num( LOCUSTSWARM_DMG_MIN, LOCUSTSWARM_DMG_MAX );

		WC3_Damage( iVictim, iAttacker, iDamage, CSW_LOCUSTS, -1 );

		if(parm[5] == 0)
		{
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_LOCUSTSWARM], 1.0, ATTN_NORM, 0, PITCH_NORM );

			ULT_Icon( iAttacker, ICON_HIDE );

			new szName[32];
			get_user_name( iVictim, szName, 31 );
		
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_CL_ULT_LOCUST_EFFECT_3",szName, iDamage);
			cssbChatColor(iAttacker,"%s%s",fTagWar3ft(),szMessage);
		}

		if(parm[5] == 1) //Если сработал Отражатель
		{
			emit_sound(iAttacker, CHAN_STATIC, g_szSounds[SOUND_LOCUSTSWARM], 1.0, ATTN_NORM, 0, PITCH_NORM );
/*
			new szName[32];
			get_user_name(iVictim, szName, 31 );
		
			formatex(szMessage, sizeof(szMessage) - 1, "%L",LANG_PLAYER,"CLIENT_PRINT_CL_ULT_LOCUST_EFFECT_3",szName, iDamage);
			cssbChatColor(iAttacker,"%s%s",fTagWar3ft(),szMessage);*/

		}

			

		
		
	
		
	}
}

CL_HLP_Diff( iNum, iNum2 )
{
	if ( iNum > iNum2 )
	{
		return (iNum-iNum2);
	}
	else
	{
		return (iNum2-iNum);
	}

	return 0;
}

CL_SkillsOffensive( iAttacker, iVictim, iHitPlace )
{
	if ( !p_data_b[iVictim][PB_ISCONNECTED] || !p_data_b[iAttacker][PB_ISCONNECTED] )
	{
		return;
	}

	static iSkillLevel;
	
	// Orb of Annihilation
	iSkillLevel = SM_GetSkillLevel( iAttacker, PASS_ORB );
	if ( iSkillLevel > 0 && p_data_b[iAttacker][PB_ISFROZENSKILL] == false)
	{

		if ( random_float(MIN_PERCENT_ORB, MAX_PERCENT_ORB ) <= p_orb[p_data[iAttacker][P_LEVEL]] )
		{
			new vVictimOrigin[3];
			get_user_origin( iVictim, vVictimOrigin );
			
			vVictimOrigin[2] -= 20;
			
			// Create the orb effect (отображение сферы)
			Create_TE_SPRITE( vVictimOrigin, g_iSprites[SPR_WAVE], 10, 200 );
			
			// Play the orb sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_ANNIHILATION], 1.0, ATTN_NORM, 0, PITCH_NORM );

			// Damage the user
			if (is_user_alive( iAttacker ) && is_user_alive( iVictim ))
				WC3_Damage( iVictim, iAttacker, ORB_DAMAGE, CSW_ORB, iHitPlace );
		}
		else if ( get_pcvar_num( CVAR_wc3_psychostats ) )
		{
			new WEAPON = CSW_ORB - CSW_WAR3_MIN;

			iStatsShots[iAttacker][WEAPON]++;
		}
	}
	
	// Carrion Beetles 
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_CARRIONBEETLES );
	if ( iSkillLevel > 0 && p_data_b[iAttacker][PB_ISFROZENSKILL] == false)
	{
		if ( random_float(MIN_PERCENT_CARRION, MAX_PERCENT_CARRION ) <= p_carrion[iSkillLevel-1] )
		{
			new vVictimOrigin[3], vAttackerorigin[3];
			get_user_origin( iVictim, vVictimOrigin );
			get_user_origin( iAttacker, vAttackerorigin );
			
			// Create the Carrion Beetle effect (отображение человечков)
			Create_TE_SPRITETRAIL( vAttackerorigin, vVictimOrigin, g_iSprites[SPR_BEETLE], 15, 15, 1, 2, 6 );
			
			// Play the carrion beetle sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_CARRION], 1.0, ATTN_NORM, 0, PITCH_NORM );

			WC3_Damage( iVictim, iAttacker, CARRIONBEETLE_DAMAGE, CSW_CARRION, iHitPlace );
		}

		else if ( get_pcvar_num( CVAR_wc3_psychostats ) )
		{
			new WEAPON = CSW_CARRION - CSW_WAR3_MIN;

			iStatsShots[iAttacker][WEAPON]++;
		}
	}
	
	
	// Impale (Запутывние скил 1 уровня)
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_IMPALE );
	if ( iSkillLevel > 0 && p_data_b[iAttacker][PB_ISFROZENSKILL] == false)
	{
		if ( random_float(MIN_PERCENT_IMPALE, MAX_PERCENT_IMPALE ) <= p_impale[iSkillLevel-1] )
		{
			// Play the impale sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_IMPALE], 1.0, ATTN_NORM, 0, PITCH_NORM );
			
			new Float:vVelocity[3];
			entity_get_vector( iVictim, EV_VEC_velocity, vVelocity );

			vVelocity[0] = random_float(100.0, 400.0 );
			vVelocity[1] = random_float(100.0, 400.0 );
			vVelocity[2] = random_float(400.0, 700.0 );

			entity_set_vector( iVictim, EV_VEC_velocity, vVelocity );
		}
	}
}

//Шипованная броня
CL_SkillsDefensive( iAttacker, iVictim, iDamage, iHitPlace )
{
	if ( !p_data_b[iVictim][PB_ISCONNECTED] || !p_data_b[iAttacker][PB_ISCONNECTED] )
	{
		return;
	}

	static iSkillLevel;

	// Spiked Carapace
	iSkillLevel = SM_GetSkillLevel( iVictim, SKILL_SPIKEDCARAPACE );
	if ( iSkillLevel > 0 && p_data_b[iAttacker][PB_ISFROZENSKILL] == false)
	{

		new iTemp = floatround( float( iDamage ) * p_spiked[iSkillLevel-1] );
		
		// Give the victim some armor...
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			new CsArmorType:ArmorType;
			new iCurArmor = cs_get_user_armor( iVictim, ArmorType );
			new iMaxArmor = SHARED_GetMaxArmor( iVictim );
			
			// Then set their armor to be the max
			if ( iCurArmor + iTemp > iMaxArmor )
			{
				cs_set_user_armor( iVictim, iMaxArmor, ArmorType );
			}
			
			// Just give them some bonus armor
			else
			{
				cs_set_user_armor( iVictim, iCurArmor + iTemp, ArmorType );
			}
		}
		
		if ( is_user_alive( iAttacker ) )
		{
			// Damage our attacker!
			WC3_Damage( iAttacker, iVictim, iTemp, CSW_CARAPACE, iHitPlace );

			// Make the user glow!
			SHARED_Glow( iAttacker, ( 3 * iTemp ), 0, 0, 0 );
			
			// Create a screen fade
			Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 255, 0, 0, iTemp );
		}
	}
}