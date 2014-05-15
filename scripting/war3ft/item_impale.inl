

public sm4_4_6_Impale( iAttacker, iVictim )//Добавлена в events.inl
{

	if(get_user_team( iVictim ) != get_user_team( iAttacker ) )
	{
		InitImpale( iAttacker, iVictim );
		ITEM_RemoveCharge(iAttacker, ITEM_IMPALE );
	}

	return PLUGIN_HANDLED;
}

/* - Impale ------------------------------------------------- */
static InitImpale( iAttacker, iVictim )
{
	emit_sound(iVictim, CHAN_STATIC, g_szSounds[SOUND_IMPALE], 1.0, ATTN_NORM, 0, PITCH_NORM );

	// Raise claw
	new Float:fOrigin[3], Origin[3];
	entity_get_vector( iVictim, EV_VEC_origin, fOrigin );

	fOrigin[2] += 180.0;
	new claw = WC3_ENTITY_create( "IMPALE_CLAW", "models/tentacle2.mdl", fOrigin, MOVETYPE_TOSS, SOLID_NOT, 1.5 );

	new Float:fVelocity[3] = { 0.0, 0.0, 500.0 };
	entity_set_vector( claw, EV_VEC_velocity, fVelocity );
	entity_set_float( claw, EV_FL_gravity, 1.5 );

	set_entity_rendering( claw, kRenderFxGlowShell, 128, 96, 160, kRenderTransTexture, 32 );

	// Launch into air
	new Velocity[3];
	get_entity_velocity( iVictim, Velocity );

	Velocity[2] = IMPALE_VELOCITY;
	set_entity_velocity( iVictim, Velocity );

	// Beam trail
	Create_TE_BEAMFOLLOW(iVictim, g_iSprites[SPR_TRAIL], 5, 3, 160, 0, 48, 128 );

	// Blood sprites
	for ( new i = 0; i < 2; i++ )
	{
		get_user_origin( iVictim, Origin );

		for ( new j = 0; j < 5; j++ )
		{
			Origin[0] += random_num( -100,100 ) * j;
			Origin[1] += random_num( -100,100 ) * j;
			Origin[2] += random_num( 0,100 ) * j;

			Create_TE_BLOODSPRITE(Origin, g_iSprites[SPR_BLOODSPRAY],g_iSprites[SPR_BLOODDROP], 248, 15 );
		}
	}

	// Blood decals
	for ( new i = 0; i < 5; i++ )
	{
		static const BLOOD_SMALL[7] = {190,191,192,193,194,195,197};

		get_user_origin( iVictim, Origin );

		Origin[0] += random_num( -100,100 );
		Origin[1] += random_num( -100,100 );
		Origin[2] -= 36;

		Create_TE_WORLDDECAL(Origin, BLOOD_SMALL[random_num( 0,6 )] );
	}

	// Remove armor
	if ( get_user_armor( iVictim ) )
	{
		new iNewArmor = get_user_armor( iVictim ) - IMPALE_ARMORDAMAGE;

		if ( iNewArmor < 0 )
			iNewArmor = 0;

		set_user_armor( iVictim, iNewArmor );
	}

	// Remove health
	WC3_Damage(iVictim,iAttacker,IMPALE_DAMAGE, CSW_IMPALE, -1 );

	if ( is_user_alive( iVictim ) )
	{
		// Screen Fade
		Create_ScreenFade( iVictim, (1<<10), (1<<10), FADE_OUT, 160, 0, 48, 160 );

		new arg_write[3];
		arg_write[0] = iVictim;
		arg_write[1] = Origin[2];

		g_PlayerImpaled[iVictim] = iAttacker;

		new task = TASK_IMPALE + iVictim;
		set_task( 0.1, "fImpaleThink", task, arg_write, 3 );
	}

	return PLUGIN_HANDLED;

}


public fImpaleThink( arg_read[3] ) 
{

	new iVictim = arg_read[0];
	new iAttacker = g_PlayerImpaled[iVictim];

	new OldOrigin = arg_read[1];
	new OldVelocity = arg_read[2];

	new Velocity[3];
	get_entity_velocity( iVictim, Velocity );

	// Checks player height on descent, assumes collision if < IMPALE_MINDISTANCE
	if ( OldVelocity >= 0 && Velocity[2] <= 0 )
	{
		new Origin[3];
		get_user_origin( iVictim, Origin );

		new iDistance = Origin[2] - OldOrigin;

		if ( iDistance < IMPALE_MINDISTANCE )
		{
			// Collision sound

			new szSoundfile[32];

			switch ( random_num( 1, 3 ) )
			{
				case 1: copy( szSoundfile, 31, "player/headshot1.wav" );
				case 2: copy( szSoundfile, 31, "player/headshot2.wav" );
				case 3: copy( szSoundfile, 31, "player/headshot3.wav" );
			}

			emit_sound( iVictim, CHAN_STATIC, szSoundfile, 1.0, ATTN_NORM, 0, PITCH_NORM );

			// Blood streams
			Origin[2] += 10;

			for ( new i = 0; i < 2; i++ )
			{
				new Vector[3];
				Vector[0] = random_num( 20,300 );
				Vector[1] = random_num( 20,300 );
				Vector[2] = 0;

				Create_TE_BLOODSTREAM(Origin, Vector, 70, random_num( 100,200 ) );
			}

			// Apply damage
			WC3_Damage(iVictim,iAttacker, IMPALE_COLLISIONDAMAGE, CSW_IMPALE, -1 );

			// Screen fade
			if ( is_user_alive( iVictim ) )
				Create_ScreenFade( iVictim, (1<<10), (1<<10), FADE_MODULATE_OUT, 0, 0, 0, 160 );
		}
	}

	if ( !( get_entity_flags( iVictim ) & FL_ONGROUND ) && g_PlayerImpaled[iVictim] )
	{
		arg_read[2] = Velocity[2];

		new task = TASK_IMPALE + iVictim;
		set_task( 0.1, "fImpaleThink", task, arg_read, 3 );
	}

	else
	{
		fImpaleRemove( iVictim );
	}

	return PLUGIN_HANDLED;
}


public fImpaleRemove( id ) 
{

	new task = TASK_IMPALE + id;
	remove_task( task, 0 );

	g_PlayerImpaled[id] = 0;
	Remove_TE_BEAMFOLLOW(id );
}