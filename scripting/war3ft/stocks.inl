

stock get_user_maxhealth( idUser )
{
	new maxHealth = 100

	static iSkillLevel;
	iSkillLevel = SM_GetSkillLevel( idUser, SKILL_DEVOTION );

	// Human Devotion Skill
	if ( iSkillLevel > 0 )
	{
		maxHealth += iSkillLevel * p_devotion;
	}

	//ƒл€ talisman +
	// Player has a health bonus from the Periapt of Health
	if ( ITEM_Has( idUser, ITEM_HEALTH ) > ITEM_NONE || fWorkTalismanPassive(idUser, arrTalismanActive[idUser]) == TALISMAN_ITEM_HEALTH )
			maxHealth += get_pcvar_num( CVAR_wc3_health );

	if(maxHealth > 255)
		maxHealth = 255;

	return maxHealth
}

// Function checks to see if the weapon is a pistol
stock SHARED_IsSecondaryWeapon( iWeaponID )
{
	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( iWeaponID == CSW_ELITE || iWeaponID == CSW_FIVESEVEN || iWeaponID == CSW_USP || iWeaponID == CSW_GLOCK18 || iWeaponID == CSW_DEAGLE || iWeaponID == CSW_P90 )
		{
			return true;
		}
	}
	
	return false;
}


stock get_user_actualhealth(idUser)
{

	new health = get_user_health(idUser);

	// Player has god mode
	if ( health > 1500 )
		return health - 2048;

	// Player will evade the next shot
	if ( health > 500 )
		return health - 1024;

	return health;
}


// ‘ункци€ провер€юща€ доступ к расе ’амелеон
public fVipChameleon()
{
    new szFlags[24];
    get_pcvar_string(CVAR_wc3_vip_chameleon_flag, szFlags, 23 );

    return ( read_flags( szFlags ) );
}

// ‘ункци€ провер€юща€ доступ к расе Ћед€ной ћаг
public fVipIceMagician()
{
    new szFlags[24];
    get_pcvar_string(CVAR_wc3_vip_ice_magician_flag, szFlags, 23 );

    return ( read_flags( szFlags ) );
}


// ‘ункци€ провер€юща€ доступ к бонусу XP
public fBonusXP()
{
    new szFlags[24];
    get_pcvar_string(CVAR_wc3_bonus_xp_flag, szFlags, 23 );

    return ( read_flags( szFlags ) );
}

//ƒл€ shopmenu4
stock set_entity_rendering( ent, iRenderFx, iR, iG, iB, iRenderType, iAmmount )
{
	entity_set_int( ent, EV_INT_renderfx, iRenderFx );

	new Float:fRGB[3];
	fRGB[0] = float( iR );
	fRGB[1] = float( iG );
	fRGB[2] = float( iB );

	entity_set_vector( ent, EV_VEC_rendercolor, fRGB );
	entity_set_int( ent, EV_INT_rendermode, iRenderType );

	new Float:fAmmount = float( iAmmount );

	entity_set_float( ent, EV_FL_renderamt, fAmmount );

	return ( 1 );
}

stock set_velocity_to_ent( ent1, ent2, Float:fSpeed )
{
	new Float:fOrigin[3], Float:fVelocity[3];

	entity_get_vector( ent2, EV_VEC_origin, fOrigin );
	get_velocity_to_origin( ent1, fOrigin, fSpeed, fVelocity );

	entity_set_vector( ent1, EV_VEC_velocity, fVelocity );

	return ( 1 );
}

stock get_velocity_to_origin( ent, Float:fOrigin[3], Float:fSpeed, Float:fVelocity[3] )
{
	new Float:fEntOrigin[3];
	entity_get_vector( ent, EV_VEC_origin, fEntOrigin );

	// Velocity = Distance / Time
	new Float:fDistance[3];
	fDistance[0] = fOrigin[0] - fEntOrigin[0];
	fDistance[1] = fOrigin[1] - fEntOrigin[1];
	fDistance[2] = fOrigin[2] - fEntOrigin[2];

	new Float:fTime = ( vector_distance( fEntOrigin,fOrigin ) / fSpeed );

	fVelocity[0] = fDistance[0] / fTime;
	fVelocity[1] = fDistance[1] / fTime;
	fVelocity[2] = fDistance[2] / fTime;

	return ( fVelocity[0] && fVelocity[1] && fVelocity[2] );
}


stock get_entity_velocity( ent, velocity[3] )
{
	new Float:fVelocity[3];
	entity_get_vector( ent, EV_VEC_velocity, fVelocity );

	velocity[0] = floatround( fVelocity[0] );
	velocity[1] = floatround( fVelocity[1] );
	velocity[2] = floatround( fVelocity[2] );

	return ( velocity[0] && velocity[1] && velocity[2] );
}

stock set_entity_velocity( ent, velocity[3] )
{
	new Float:fVelocity[3];

	fVelocity[0] = float( velocity[0] );
	fVelocity[1] = float( velocity[1] );
	fVelocity[2] = float( velocity[2] );

	entity_set_vector( ent, EV_VEC_velocity, fVelocity );

	return ( 1 );
}


stock Remove_TE_BEAMFOLLOW(id)
{
	message_begin( MSG_ALL,SVC_TEMPENTITY );
	write_byte( TE_KILLBEAM );          // TE_KILLBEAM (99)
	write_short( id );                  // entity
	message_end();

	return PLUGIN_HANDLED;
}

// Sets maxspeed of an entity ( corrects issues with setting maxspeed on bots )
stock set_entity_maxspeed( ent, Float:fMaxSpeed ) 
{
	entity_set_float( ent, EV_FL_maxspeed, fMaxSpeed );
	return ( 1 );
}

// ‘ункци€ провер€юща€ доступ к низким ценам
public fBuyItemCost()
{
    new szFlags[24];
    get_pcvar_string(CVAR_wc3_cost_vip_flag, szFlags, 23 );

    return ( read_flags( szFlags ) );
}