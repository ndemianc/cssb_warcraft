

public sm4_4_5_DeathCoil( iAttacker, iVictim )//Добавлена в events.inl
{

	if(get_user_team( iVictim ) != get_user_team( iAttacker ) )
	{
		InitDeathCoil( iAttacker, iVictim );
		ITEM_RemoveCharge(iAttacker, ITEM_DEATHCOIL );
	}

	return PLUGIN_HANDLED;
}

/* - Death Coil ------------------------------------------------- */
static InitDeathCoil( iAttacker, iVictim )
{
	emit_sound(iAttacker, CHAN_STATIC, g_szSounds[SOUND_DEATHCOIL_MISSILE], 1.0, ATTN_NORM, 0, PITCH_NORM );

	//Создание модели скелета
	new Float:fOrigin[3];
	entity_get_vector( iAttacker, EV_VEC_origin, fOrigin );

	//new skull = WC3_ENTITY_create( "DEATH_COIL", "models/bskull_template1.mdl", fOrigin, MOVETYPE_NOCLIP, SOLID_TRIGGER, DEATHCOIL_DURATION );
	new skull = WC3_ENTITY_create( "DEATH_COIL", "models/bskull_template1.mdl", fOrigin, MOVETYPE_BOUNCE, SOLID_BBOX, DEATHCOIL_DURATION );

	entity_set_edict( skull, EV_ENT_owner, iAttacker );
	entity_set_edict( skull, EV_ENT_enemy, iVictim );

	// Skull effects
	set_entity_rendering( skull, kRenderFxGlowShell, DEATHCOIL_SHELL_RGB[GLOW_R], DEATHCOIL_SHELL_RGB[GLOW_G], DEATHCOIL_SHELL_RGB[GLOW_B], kRenderNormal, 48 );
	Create_TE_BEAMFOLLOW(skull, g_iSprites[SPR_TRAIL], 5, 8, DEATHCOIL_TRAIL_RGB[GLOW_R], DEATHCOIL_TRAIL_RGB[GLOW_G], DEATHCOIL_TRAIL_RGB[GLOW_B], 160 );

	// Seek iVictim
	new arg_write[1];
	arg_write[0] = skull;

	fDeathCoilSeek( arg_write );

	return PLUGIN_HANDLED;
}

public fDeathCoilSeek( arg_read[1] )
{

	new skull = arg_read[0];

	if ( !WC3_is_valid_ent( skull ) )
		return PLUGIN_HANDLED;

	new iVictim = entity_get_edict( skull, EV_ENT_enemy );

	if ( !is_user_alive( iVictim ) )
		return PLUGIN_HANDLED;

	// Project skull towards iVictim
	set_velocity_to_ent( skull, iVictim, DEATHCOIL_VELOCITY );

	// Repeat until iVictim is hit
	new task = TASK_TEMPENTITY + skull;
	set_task( 0.1, "fDeathCoilSeek", task, arg_read, 1 );

	return PLUGIN_HANDLED;
}


static fDeathCoilDamage( iAttacker, iVictim) 
{

	// Play sound
	emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_DEATHCOIL], 1.0, ATTN_NORM, 0, PITCH_NORM );

	// Generate effects
	fDeathCoilEffects( iVictim );

	// Unset velocity
	new Float:fVelocity[3];
	entity_get_vector( iVictim, EV_VEC_velocity, fVelocity );

	fVelocity[0] = 0.0;
	fVelocity[1] = 0.0;

	// Apply damage
	WC3_Damage(iVictim,iAttacker, DEATHCOIL_DAMAGE, CSW_DEATHCOIL, -1 );

	return PLUGIN_HANDLED;
}

static fDeathCoilEffects( iVictim )
{
	Create_ScreenFade( iVictim, (1<<10), (1<<10), FADE_OUT, DEATHCOIL_TRAIL_RGB[GLOW_R], DEATHCOIL_TRAIL_RGB[GLOW_G], DEATHCOIL_TRAIL_RGB[GLOW_B], 160 );

	new Origin[3];
	get_user_origin( iVictim, Origin );

	Create_TE_EXPLOSION( Origin, Origin, g_iSprites[SPR_DEATHCOIL], 20, 24, 4 );

	Create_TE_ELIGHT(iVictim, Origin, 100, DEATHCOIL_TRAIL_RGB[GLOW_R], DEATHCOIL_TRAIL_RGB[GLOW_G],DEATHCOIL_TRAIL_RGB[GLOW_B], 10, 0 );

	return PLUGIN_HANDLED;
}


static fDeathCoilRemove( skull ) 
{
	new task = TASK_TEMPENTITY + skull;
	remove_task( task, 0 );

	if (is_valid_ent(skull)) 
		remove_entity( skull );

	return PLUGIN_HANDLED;
}


public touchDeathCoil( entSkull, idWorldPlayer ) 
{

	// Sanity checks
	if ( idWorldPlayer < 1 || idWorldPlayer > 32 || entSkull < 1 || !is_user_alive( idWorldPlayer ) )
	{
	//	fCreateExplodeIceArrow(entSkull);
	//	fDeathCoilRemove( entSkull );
	//	return PLUGIN_CONTINUE;
	}
/*
	new iAttacker = entity_get_edict( entSkull, EV_ENT_owner );
	new iVictim = entity_get_edict( entSkull, EV_ENT_enemy );

	if ( idPlayerTouch == iVictim )
	{
		if ( get_user_team( iAttacker ) != get_user_team( iVictim ) )
			fDeathCoilDamage( iAttacker, iVictim );

		fDeathCoilRemove( entSkull );
	}
*/

	//Если попадает в какое-нибудь место (стена,пол и т.д.)
	if(idWorldPlayer == 0) 
	{
		fCreateExplodeIceArrow(entSkull);
		fDeathCoilRemove( entSkull );
		
		return PLUGIN_CONTINUE;
	}
	//Если попадает в игрока
	else if(is_user_connected(idWorldPlayer))
	{
		new iAttacker = entity_get_edict( entSkull, EV_ENT_owner );
		new iVictim = entity_get_edict( entSkull, EV_ENT_enemy );
		//Если попадает в игрока и он не из твоей команды
		if(get_user_team(idWorldPlayer) != get_user_team(iAttacker))
		{
			fDeathCoilDamage( iAttacker, iVictim );
			fDeathCoilRemove( entSkull );

			/*//Создание эффекта заморозки
			message_begin(MSG_ONE, get_user_msgid("ScreenFade"), _, idWorldPlayer);
			write_short(0);// duration
			write_short(0);// hold time
			write_short(FFADE_STAYOUT);// fade type
			write_byte(0);// red
			write_byte(50);// green
			write_byte(200);// blue
			write_byte(100);// alpha
			message_end();

			fm_set_rendering(idWorldPlayer, kRenderFxGlowShell, 0, 100, 200, kRenderNormal, 25);

			// Prevent from jumping
			if (pev(idWorldPlayer, pev_flags) & FL_ONGROUND)
				set_pev(idWorldPlayer, pev_gravity, 999999.9);// set really high
			else
				set_pev(idWorldPlayer, pev_gravity, 0.000001);// no gravity

			set_pev(idWorldPlayer, pev_velocity, Float:{0.0,0.0,0.0});// stop motion
			set_pev(idWorldPlayer, pev_maxspeed, 1.0);// prevent from moving

			//set_user_maxspeed(idWorldPlayer, 1.0 );
				
			fCreateExplodeIceArrow(entIceArrow);

			//if ( task_exists( TASK_GLOW_SHELL + idWorldPlayer ) )
			//	return;

			p_data_b[idWorldPlayer][PB_ISFROZENPLAYER] = true;

			static iSkillLevel;
			iSkillLevel = SM_GetSkillLevel(idAttacker, SKILL_FROST_MAGE_ICE_ARROW );
			set_task(p_ice_arrow[iSkillLevel-1], "fRemoveFreeze", TASK_REMOVE_FROZEN + idWorldPlayer );*/

		}
		//Если попадает в игрока и он из твоей команды
		else 
		{
			//fCreateExplodeIceArrow(entIceArrow)
			//fDeathCoilRemove( entSkull );
			//fCreateExplodeIceArrow(entSkull);
		}
	} 
	//Если игрок запустил стрелу и вышел с сервера
	else
	{
		//fCreateExplodeIceArrow(entIceArrow);
		fCreateExplodeIceArrow(entSkull);
		fDeathCoilRemove( entSkull );
		
	}

	return PLUGIN_CONTINUE;
}




///WC3
public WC3_is_valid_ent( ent ) 
{
	// Don't want to look up info for an invalid ent
	if ( !is_valid_ent( ent ) )
		return ( false );

	new szWar3Entities[][64] = {"DEATH_COIL", "ROOT_PLANT", "IMPALE_CLAW", "SLEEP_Z", "REJUV_FLARE", "FLAME_STRIKE", "HEAL_EFFECT" };

	new szClassName[64];
	entity_get_string( ent, EV_SZ_classname, szClassName, 63 );

	// See if this class is in our list
	for ( new i = 0; i < sizeof( szWar3Entities ); i++ )
	{
		if ( equal( szClassName, szWar3Entities[i] ) )
			return ( true );
	}

	return ( false );
}


public WC3_ENTITY_create( szClassName[32], szModel[64], Float:fOrigin[3], iMoveType, iSolidType, Float:fLife )
{

	new ent = create_entity( "info_target" );
	entity_set_string( ent, EV_SZ_classname, szClassName );

	entity_set_model( ent, szModel );
	entity_set_origin( ent, fOrigin );

	entity_set_int( ent, EV_INT_movetype, iMoveType );
	entity_set_int( ent, EV_INT_solid, iSolidType );

	if ( fLife )
	{
		new arg_write[1];
		arg_write[0] = ent;

		new task = TASK_TEMPENTITY + ent;

		set_task( fLife, "WC3_ENTITY_remove", task, arg_write, 1 );
	}

	return ( ent );
}


public WC3_ENTITY_remove( arg_read[1] ) 
{

	new ent = arg_read[0];

	if ( WC3_is_valid_ent( ent ) )
		remove_entity( ent );

	return PLUGIN_HANDLED;
}

// Healed by WC3 skill
public WC3_heal( iAttacker, iVictim, iAmount ) 
{

	// Calculate new health
	new iNewHealth = SHARED_GetUserHealth( iVictim ) + iAmount;
	new iMaxHealth = get_user_maxhealth( iVictim );

	new iHealthGiven = iAmount;

	if ( iNewHealth > iMaxHealth )
	{
		iHealthGiven -= ( iNewHealth - iMaxHealth );
		iNewHealth = iMaxHealth;
	}

	// Apply health
	set_user_health( iVictim, iNewHealth );
	
	return ( iHealthGiven );
}
