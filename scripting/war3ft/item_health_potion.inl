
ITEM_HealthPotion( idUser )
{
	if (!is_user_alive(idUser)) 
		return;

	new iHealthMax = get_user_maxhealth( idUser );

	format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU3_7");
	cssbColoredPrint(idUser, szMessage);

	SHARED_SetUserHealth(idUser, iHealthMax);

	EffectRestoreHealth(idUser);
	//Create_ScreenFade( idUser, (1<<10), (1<<10), (1<<12), 0, 255, 0, 255 );
	emit_sound(idUser, CHAN_STATIC, g_szSounds[SOUND_HEALTH_POTION], 1.0, ATTN_NORM, 0, PITCH_NORM );

	arrIsByuItemOneRound[idUser][ITEM_HEALTH_POTION]++;

}

// Effect
EffectRestoreHealth(idUser)
{
	if (!is_user_alive(idUser)) 
		return;
	
	static Float:origin[3];
	pev(idUser,pev_origin,origin);
    
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY); 
	write_byte(TE_EXPLOSION); // TE_EXPLOSION
	write_coord(floatround(origin[0])); // origin x
	write_coord(floatround(origin[1])); // origin y
	write_coord(floatround(origin[2])); // origin z
	write_short(g_iSprites[SPR_HEALTH_POTION]); // sprites
	write_byte(15); // scale in 0.1's
	write_byte(12); // framerate
	write_byte(14); // flags 
	message_end(); // message end
	
	message_begin(MSG_ONE_UNRELIABLE, gmsgScreenFade,_, idUser);
	write_short(1<<10);
	write_short(1<<10);
	write_short(0x0000);
	write_byte(0);//r
	write_byte(255);  //g
	write_byte(0);  //b
	write_byte(75);
	message_end();
}