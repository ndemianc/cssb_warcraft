//ќтображение иконки
stock stShowStatusIcon(idUser, status, sprite[], red, green, blue)
{
	message_begin( MSG_ONE, get_user_msgid("StatusIcon"), {0,0,0}, idUser ); 
	write_byte( status );			// status 
	write_string( sprite );			// sprite name 
	write_byte( red );				// red 
	write_byte( green );			// green 
	write_byte( blue );				// blue 
	message_end();
}

//—лед от гранаты
stock stShowTrailGrenade(entity, iSprite, life, width, red, green, blue, alpha)
{
	message_begin( MSG_BROADCAST, SVC_TEMPENTITY );
	write_byte( TE_BEAMFOLLOW );
	write_short( entity );			// entity
	write_short( iSprite );			// model
	write_byte( life );				// life
	write_byte( width );				// width
	write_byte( red );				// red
	write_byte( green );				// green
	write_byte( blue );				// blue
	write_byte( alpha );				// brightness
	message_end();
}
//ќтображение очков	
stock stShowScoreInfo(idUser,frags,deaths,playerClass,team )
{
	message_begin(MSG_ALL,get_user_msgid ( "ScoreInfo" ));
	write_byte(idUser);
	write_short(frags);
	write_short(deaths);
	write_short(playerClass);
	write_short(team);
	message_end();
}

//Ёффект от взрыва
stock stShowExplosion( origin[3], origin2[3], iSprite, scale, frameRate, flags )
{
	message_begin( MSG_PVS, SVC_TEMPENTITY, origin );
	write_byte( TE_EXPLOSION );
	write_coord( origin2[0] );			// position (X)
	write_coord( origin2[1] );			// position (Y)
	write_coord( origin2[2]	);			// position (Z)
	write_short( iSprite );				// sprite index
	write_byte( scale );					// scale in 0.1's
	write_byte( frameRate );				// framerate
	write_byte( flags );					// flags
	message_end();
}

//Ёффект от взрыва 2
stock stShowExplosion2(Float:origin[3], iSprite, scale, frameRate, flags )
{
	engfunc ( EngFunc_MessageBegin, MSG_PVS, SVC_TEMPENTITY, origin, 0);
	write_byte (TE_EXPLOSION);
	engfunc(EngFunc_WriteCoord, origin[0]); // Position X
	engfunc(EngFunc_WriteCoord, origin[1]); // Position Y
	engfunc(EngFunc_WriteCoord, origin[2]); // Position Z
	write_short (iSprite); // Sprite index
	write_byte (scale); // Scale
	write_byte (frameRate); // Frame rate
	write_byte (flags); // Flags
	message_end ();
}


