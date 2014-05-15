/*
*	Race: Black Book Functions
*/

#define BLACKSTRIKE_DAMAGE 20
#define SHADOW_HOLD_TIME  1.0
// Initial Ultimate call
public BB_ULT_BlackLightning( iCaster, iTarget, iBodyPart )
{
	
	new cOrigin[3];
	new tOrigin[3];
	get_user_origin( iCaster, cOrigin );
	get_user_origin( iTarget, tOrigin );
	
	Create_TE_TELEPORT( tOrigin );
	set_user_origin( iCaster, tOrigin );
	Create_TE_TELEPORT( cOrigin );
	set_user_origin( iTarget, cOrigin );
	
	ULT_ResetCooldown( iCaster, get_pcvar_num( CVAR_wc3_ult_cooldown ) );
}

BB_SkillsOffensive( iAttacker, iVictim, iHitPlace)
{

	static iSkillLevel;

	// Black Shadow
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_BLACKSHADOW );
	if ( iSkillLevel > 0 )
	{

		// Cannot bash if already bashed or user is slowed
		if ( random_float( 0.0, 1.0 ) <= p_bshadow[iSkillLevel-1] && !SHARED_IsPlayerSlowed( iVictim ) )
		{		

			p_data_b[iVictim][PB_STUNNED] = true;
			SHARED_SetSpeed( iVictim );
			
			new parm[5];
			
			set_task( SHADOW_HOLD_TIME, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + iVictim );

			// Make the user glow!
			SHARED_Glow( iVictim, 225, 20, 147, 0 );
			
			 Create_ScreenShake( iVictim, (10<<12), (3<<12), (6<<12) );	
			 
			  Create_ScreenFade( iVictim, 4, (1<<10), (1<<12), 148, 0, 211, 255 );
			 
			 set_task( 0.5, "_BB_SuicideBlastCircles", TASK_BEAMCYLINDER + iVictim, parm, 5 );
			
		}
	}

	// Black Strike
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_BLACKSTRIKE );
	if( iSkillLevel > 0 )
  {
    if ( random_float( 0.0, 1.0 ) <= p_bstrike[iSkillLevel-1] )
	{
	new vTargetOrigin[3], vAttackerOrigin[3], vVictimOrigin[3]; 
	get_user_origin( iVictim, vTargetOrigin );

	Create_TE_EXPLOSION( vTargetOrigin, vTargetOrigin, g_iSprites[SPR_BLACKFIRE], 20, 24, 4 );
	Create_TE_EXPLOSION( vTargetOrigin, vTargetOrigin, g_iSprites[SPR_BURNFIRE], 30, 24, 4 );
	
    get_user_origin( iVictim, vVictimOrigin );
    get_user_origin( iAttacker, vAttackerOrigin );
	
	Create_TE_SPRITETRAIL( vAttackerOrigin, vVictimOrigin, g_iSprites[SPR_BLACKSTRIKE], 50, 15, 2, 2, 6 );

	// Damage the user
	WC3_Damage( iVictim, iAttacker, BLACKSTRIKE_DAMAGE, CSW_BLACKPOWER, iHitPlace );

  }
  
 }
 }
 
 	BB_SkillsDefensive( iAttacker, iVictim )
	{

	static iSkillLevel;
	iSkillLevel = SM_GetSkillLevel( iVictim, SKILL_BLACKPOWER );
	if ( iSkillLevel > 0 )
	{

		if ( random_float( 0.0, 1.0 ) <= p_bpower[iSkillLevel-1] )
		{

			// Play the impale sound
			emit_sound( iAttacker, CHAN_STATIC, g_szSounds[SOUND_IMPALE], 1.0, ATTN_NORM, 0, PITCH_NORM );
			
			new Float:vVelocity[3];
			entity_get_vector( iAttacker, EV_VEC_velocity, vVelocity );

			vVelocity[0] = random_float( 100.0, 400.0 );
			vVelocity[1] = random_float( 100.0, 400.0 );
			vVelocity[2] = random_float( 400.0, 700.0 );

			entity_set_vector( iAttacker, EV_VEC_velocity, vVelocity );
	
	}   }
}

// Draw the blast circles
public _BB_SuicideBlastCircles( parm[5] )
{
	new vOrigin[3], vPosition[3];

	vOrigin[0] = parm[2];
	vOrigin[1] = parm[3];
	vOrigin[2] = parm[4] - 16;

	vPosition[0] = vOrigin[0];
	vPosition[1] = vOrigin[1];
	vPosition[2] = vOrigin[2] + 250;

	Create_TE_BEAMCYLINDER( vOrigin, vOrigin, vPosition, g_iSprites[SPR_SHOCKWAVE], 0, 0, 6, 16, 0, 255, 20, 155, 255, 0 );

	vOrigin[2] = ( vOrigin[2] - 250 ) + ( 250 / 2 );

	Create_TE_BEAMCYLINDER( vOrigin, vOrigin, vPosition, g_iSprites[SPR_SHOCKWAVE], 0, 0, 6, 16, 0, 255, 20, 155, 255, 0 );
}

public frieza_kill()  //triggered everytime someone dies
{
	new idUser = read_data(2)  //This tells who the victim is
	if(diskTimer[idUser] > 0){
		diskTimer[idUser] = -1
		new Float: fOrigin[3]
		new origin[3]
		//gets current position of entity
		entity_get_vector(disk[idUser], EV_VEC_origin, fOrigin)
		//converts a floating vector into an integer vector
		FVecIVec(fOrigin, origin)
		decay_effects(disk[idUser], origin)
	}
}
//----------------------------------------------------------------------------------------
public frieza_disklife(){ 
    for(new idUser = 1; idUser <= 33; idUser++){ 
        if(is_user_alive(idUser)){ 
            if(diskTimer[idUser] > 0){ 
                diskTimer[idUser]-- 
		new Float: fVelocity[3]
		//gets the velocity by the direction you are looking at
		velocity_by_aim(idUser, 750, fVelocity)
		//sets the new velocity
		entity_set_vector(disk[idUser], EV_VEC_velocity, fVelocity)
            } 
            else if(diskTimer[idUser] == 0){
		new Float: fOrigin[3]
		new origin[3]
		//gets the current position of entity
                entity_get_vector(disk[idUser], EV_VEC_origin, fOrigin)
		//converts a floating vector to an integer vector
		FVecIVec(fOrigin, origin)
		decay_effects(disk[idUser], origin)
                diskTimer[idUser]-- 
            } 
        } 
    } 
}

public MM_Blast(idUser)
{
   if( g_friezadisk[idUser] > 0 && is_user_alive(idUser) && g_bFriezaDisabled == false)
   {
    //Makes an array of origin in the (x,y,z) coordinate system.
    new origin[3]
	g_friezadisk[idUser] = g_friezadisk[idUser] - 1

    //Makes an array of velocity, specifically in the (x,y,z) coordinate system 
    new velocity[3] 

    new Float:fOrigin[3], Float:fVelocity[3]
    get_user_origin(idUser, origin, 1)
    new Float: minBound[3] = {-50.0, -50.0, 0.0}  //sets the minimum bound of entity
    new Float: maxBound[3] = {50.0, 50.0, 0.0}    //sets the maximum bound of entity
    IVecFVec(origin, fOrigin)

    //This will make it so that the disk appears in front of the user
    new Float:viewing_angles[3]
    new distance_from_user = 70
    entity_get_vector(idUser, EV_VEC_angles, viewing_angles)
    fOrigin[0] += floatcos(viewing_angles[1], degrees) * distance_from_user
    fOrigin[1] += floatsin(viewing_angles[1], degrees) * distance_from_user
    fOrigin[2] += floatsin(-viewing_angles[0], degrees) * distance_from_user

    new NewEnt = create_entity("info_target")  //Makes an object 
    entity_set_string(NewEnt, EV_SZ_classname, "disk") //sets the classname of the entity
    disk[idUser] = NewEnt

    //This tells what the object will look like 
    entity_set_model(NewEnt, "models/shmod/frieza_friezadisc.mdl") 

    //This will set the origin of the entity 
    entity_set_origin(NewEnt, fOrigin) 

    //This will set the movetype of the entity 
    entity_set_int(NewEnt,EV_INT_movetype, MOVETYPE_BOUNCE) 

	set_pev(NewEnt, pev_gravity, 0.01);

    //This makes the entity touchable
    entity_set_int(NewEnt, EV_INT_solid, SOLID_BBOX)

    //This will set the velocity of the entity 
    velocity_by_aim(idUser, 750, fVelocity) 
    FVecIVec(fVelocity, velocity)

    //Sets the size of the entity
    entity_set_size(NewEnt, minBound, maxBound)

    //Sets who the owner of the entity is
    entity_set_edict(NewEnt, EV_ENT_owner, idUser)

    //This will set the entity in motion 
    entity_set_vector(NewEnt, EV_VEC_velocity, fVelocity) 

    //This will make the entity have sound.
    emit_sound(NewEnt, CHAN_VOICE, "shmod/frieza_destructodisc.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM)	
	
    new lifetime = 5

    message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
    write_byte(22)       //TE_BEAMFOLLOW
    write_short(NewEnt)  //The entity to attach the sprite to
    write_short(flash)  //sprite's model
    write_byte(lifetime)   //life in 0.1 seconds
    write_byte(50)   //width of sprite
    write_byte(255)  //red
    write_byte(0)    //green
    write_byte(255)  //blue
    write_byte(255)  //brightness
    message_end()
	
    return
  }

   else if( g_friezadisk[idUser] == 0 && is_user_alive(idUser) )
   {
   client_print( idUser, print_chat, "%L", "CLIENT_PRINT_BB_DISK" );
   }

}

public touch_event(entDisk, idWorldPlayer)  //This is triggered when two entites touch
{ 
    new aimvec[3], Float:fAimvec[3]  //This is the position where the disk collides 
    entity_get_vector(idWorldPlayer, EV_VEC_origin, fAimvec) 
    FVecIVec(fAimvec, aimvec) 
    new self_immune = 50 - 2 //Gives split-second immunity

    if(idWorldPlayer == entity_get_edict(entDisk, EV_ENT_owner) && diskTimer[idWorldPlayer] > self_immune)
		return PLUGIN_HANDLED
         
	special_effects(entDisk, 0, aimvec);

	//≈сли попадает в какое-нибудь место (стена,пол и т.д.)
	if(idWorldPlayer == 0) 
	{
		if (is_valid_ent(entDisk)) 
			remove_entity( entDisk );
		
		return PLUGIN_CONTINUE;
	}
	//≈сли попадает в игрока
	else if(is_user_connected(idWorldPlayer))
	{
		new iAttacker = entity_get_edict( entDisk, EV_ENT_owner );
//		new iVictim = entity_get_edict( entDisk, EV_ENT_enemy );
		//≈сли попадает в игрока и он не из твоей команды
		if(get_user_team(idWorldPlayer) != get_user_team(iAttacker))
		{
			special_effects(entDisk, idWorldPlayer, aimvec)

			if (is_valid_ent(entDisk)) 
				remove_entity( entDisk );


		}
		//≈сли попадает в игрока и он из твоей команды
		else 
		{
		}
	} 
	//≈сли игрок запустил стрелу и вышел с сервера
	else
	{
		special_effects(entDisk, idWorldPlayer, aimvec);

		if (is_valid_ent(entDisk)) 
			remove_entity( entDisk );
		
	}

	return PLUGIN_CONTINUE;

} 
//----------------------------------------------------------------------------------------- 
public special_effects(pToucher, pTouched, aimvec[3]) //effects for when disk touch
{
	new Float:fVelocity[3]
	new velocity[3]
	new damage
	entity_get_vector(pToucher, EV_VEC_velocity, fVelocity)
	FVecIVec(fVelocity, velocity)

	new killer = entity_get_edict(pToucher, EV_ENT_owner)
	new victim = pTouched


	if(is_user_alive(pTouched) && get_user_team( killer ) != get_user_team( victim ) )
	{
		message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
		write_byte(101)  //BLOODSTREAM
		write_coord(aimvec[0])
		write_coord(aimvec[1])
		write_coord(aimvec[2])
		write_coord(velocity[0])
		write_coord(velocity[1])
		write_coord(velocity[2])
		write_byte(95)
		write_byte(100)
		message_end()

		damage = 25
		new victim = pTouched
		WC3_Damage(victim, killer, damage, CSW_LIGHTNING, -1 )
	}
	
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(9)  //SPARKS
	write_coord(aimvec[0])
	write_coord(aimvec[1])
	write_coord(aimvec[2])
	message_end()

}

public decay_effects(NewEnt, origin[3])  //removes the entity plus adds a decaying effect
{
	if(is_valid_ent(NewEnt)){
		remove_entity(NewEnt)
		message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
		write_byte(14) //IMPLOSION
		write_coord(origin[0])
		write_coord(origin[1])
		write_coord(origin[2])
		write_byte(50)
		write_byte(10)
		write_byte(10)
		message_end()
	}
} 