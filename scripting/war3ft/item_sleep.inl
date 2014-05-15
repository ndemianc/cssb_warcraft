

public sm4_4_7_Sleep( iAttacker, iVictim )//Добавлена в events.inl
{

	if(get_user_team( iVictim ) != get_user_team( iAttacker ) )
	{
		if(g_bPlayerSleeping[iVictim] || g_bPlayerCantMove[iVictim])
			return PLUGIN_HANDLED;

		InitSleep( iVictim );
		ITEM_RemoveCharge(iAttacker, ITEM_SLEEP );
	}

	return PLUGIN_HANDLED;
}

public InitSleep( iVictim ) 
{	
	// Play sound
	emit_sound(iVictim, CHAN_STATIC, g_szSounds[SOUND_SLEEP], 1.0, ATTN_NORM, 0, PITCH_NORM );

	// Switch to knife
	new weapon, iClip, iAmmo;
	weapon = get_user_weapon( iVictim, iClip, iAmmo );

	if ( weapon != CSW_KNIFE )
		engclient_cmd( iVictim, "weapon_knife" );

	// Immobilize
	g_bPlayerSleeping[iVictim] = true;
	g_bPlayerCantMove[iVictim] = true;
	arrfDuration[iVictim] = 0.0;

	set_task(0.1, "WC3_set_speed",(TASK_SLEEP_WEAPON + iVictim), _,_, "b" );

	// Hit zones ( remove )
	set_user_hitzones( iVictim, 0, 0 );
	entity_set_float( iVictim, EV_FL_takedamage, 0.0 );

	// Render
	set_user_rendering( iVictim, kRenderFxNone, 0, 0, 0, kRenderTransAdd, 128 );

	new iHoldTime = floatround( SLEEP_DURATION );

	Create_ScreenFade( iVictim, (1<<12), (iHoldTime<<12), FADE_MODULATE_IN, 0, 0, 0, 160 );

	// Progress bar
	Create_BarTime( iVictim, iHoldTime, 0 );

	// Effects
	new arg_Z1[3], arg_Z2[3];
	arg_Z1[0] = iVictim;
	arg_Z2[0] = iVictim;

	fSleepEffect( arg_Z1 );

	new task = TASK_SLEEP + iVictim;
	set_task( 0.1, "fSleepEffect", task, arg_Z2, 3 );

	return PLUGIN_HANDLED;
}


public fSleepEffect( arg_read[3] ) 
{
	new iVictim = arg_read[0];
	new sleep_z = arg_read[1];
	new iCounter  = arg_read[2];

	//client_print(0,print_chat,"fSleepEffect");

	if ( !iCounter )
	{
		// Create Z's
		new Float:fTargetOrigin[3];
		entity_get_vector( iVictim, EV_VEC_origin, fTargetOrigin );

		fTargetOrigin[2] += 36.0;

		sleep_z = WC3_ENTITY_create( "SLEEP_Z",g_szSprites[SPR_SLEEP], fTargetOrigin, MOVETYPE_NOCLIP, SOLID_NOT, 1.5 );

		entity_set_float( sleep_z, EV_FL_renderamt, 128.0 );
		entity_set_int( sleep_z, EV_INT_rendermode, kRenderTransAdd );
		entity_set_int( sleep_z, EV_INT_renderfx, kRenderFxClampMinScale );

		entity_set_edict( sleep_z, EV_ENT_owner, iVictim );

		// Project Upwards
		new Float:fVelocity[3];
		fVelocity[0] = random_float( -10.0, 10.0 );
		fVelocity[1] = random_float( -10.0, 10.0 );
		fVelocity[2] = 25.0;

		entity_set_vector( sleep_z, EV_VEC_velocity, fVelocity );
		entity_set_float( sleep_z, EV_FL_scale, random_float( 0.3, 0.4 ) );
	}

	if ( iCounter <= 10 )
	{
		if ( iCounter > 5 && is_valid_ent( sleep_z ) )
		{
			new Float:fScale = entity_get_float( sleep_z, EV_FL_scale );

			fScale -= 0.02;
			entity_set_float( sleep_z, EV_FL_scale, fScale );
		}

		// Re-render player
		//set_user_rendering( iVictim, kRenderFxNone, 0, 0, 0, kRenderTransAdd, 128 );

		// Do it all again!
		iCounter++;

		arg_read[1] = sleep_z;
		arg_read[2] = iCounter;

//		client_print(0,print_chat,"iCounter <= 10");

		new task = TASK_SLEEP_TEMPENTITY + sleep_z;
		set_task( 0.1, "fSleepEffect", task, arg_read, 3 );
	}

	else if ( g_bPlayerSleeping[iVictim] == true )
	{
//		client_print(0,print_chat,"fSleepEffect = g_bPlayerSleeping[iVictim]");

		arg_read[1] = 0;
		arg_read[2] = 0;

		new task = TASK_SLEEP_TEMPENTITY + sleep_z;
		set_task( 0.5, "fSleepEffect", task, arg_read, 3 );
	}

	return PLUGIN_HANDLED;
}

public fSleepRemove( idTask )
{
	new iVictim;
	if (idTask >= TASK_SLEEP_WEAPON )
		iVictim = idTask - TASK_SLEEP_WEAPON;

	if(task_exists(idTask))
		remove_task(idTask);

	new task = TASK_SLEEP + iVictim;
	remove_task( task, 0 );

	g_bPlayerSleeping[iVictim] = false;
	g_bPlayerCantMove[iVictim] = false;
	arrfDuration[iVictim] = 0.0;

	// Remove z's
	WC3_ENTITY_remove_class( "SLEEP_Z", iVictim );

	// Hit zones ( restore )
	set_user_hitzones( iVictim, 0, 255 );
	entity_set_float( iVictim, EV_FL_takedamage, 2.0 );

	SHARED_SetSpeed( iVictim );
	SHARED_SetGravity( iVictim );
	SHARED_INVIS_Set( iVictim );
	
}


// Set player speed
public WC3_set_speed( idTask ) 
{
	new iVictim = idTask - TASK_SLEEP_WEAPON;

	if (!is_user_connected( iVictim ) )
		return PLUGIN_HANDLED;
		
//	client_print(0,print_chat,"WC3_set_speed");

		// Immobilized
	if ( g_bPlayerCantMove[iVictim] == true)                        
	{
		set_user_maxspeed( iVictim, SPEED_IMMOBILIZE );
	}
	
	if(arrfDuration[iVictim] >= SLEEP_DURATION)
	{
		fSleepRemove( idTask );
	}
	else
	{
		arrfDuration[iVictim] += 0.1;
	}
	
	return PLUGIN_HANDLED;
}

public WC3_ENTITY_remove_class( szClassname[32], owner ) 
{

	new ent = find_ent_by_class( -1, szClassname );

	while ( ent > 0 )
	{
		if ( ( !owner || entity_get_edict( ent, EV_ENT_owner ) == owner ) && WC3_is_valid_ent( ent ) )
		{
			new task = TASK_SLEEP_TEMPENTITY + ent;
			remove_task( task, 0 );

			remove_entity( ent );

			ent = find_ent_by_class( -1, szClassname );
		}
		else
		{
			ent = find_ent_by_class( ent, szClassname );
		}
	}

	return PLUGIN_HANDLED;
}