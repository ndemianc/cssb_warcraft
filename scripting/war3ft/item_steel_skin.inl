

ITEM_SteelSkin( idUser )
{
	if (!is_user_alive(idUser)) 
		return;

	format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"INFO_SHOPMENU3_9",
				RESTORES_ARMOR_NUM);

	cssbColoredPrint(idUser, szMessage);	

	//new CsArmorType:ArmorType;
	//new iCurArmor = cs_get_user_armor(idUser, ArmorType );
	//new iMaxArmor = SHARED_GetMaxArmor(idUser);
	
	//cs_set_user_armor(idUser,200, CS_ARMOR_VESTHELM );
	fm_cs_set_user_armor(idUser,RESTORES_ARMOR_NUM, CS_ARMOR_VESTHELM );
	
	EffectRestoreArmor(idUser);
	emit_sound(idUser, CHAN_STATIC, g_szSounds[SOUND_STEEL_SKIN], 1.0, ATTN_NORM, 0, PITCH_NORM );

	arrIsByuItemOneRound[idUser][ITEM_STEEL_SKIN]++;

}

// Effect
EffectRestoreArmor(idUser)
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
	write_short(g_iSprites[SPR_STEEL_SKIN]); // sprites
	write_byte(15); // scale in 0.1's
	write_byte(12); // framerate
	write_byte(14); // flags 
	message_end(); // message end
	
	message_begin(MSG_ONE_UNRELIABLE, gmsgScreenFade,_, idUser);
	write_short(1<<10);
	write_short(1<<10);
	write_short(0x0000);
	write_byte(238);//r
	write_byte(232);  //g
	write_byte(170);  //b
	write_byte(75);
	message_end();

}

stock fm_cs_get_user_armor(client, &CsArmorType:armortype)
{
    armortype = CsArmorType:get_pdata_int(client, OFFSET_ARMORTYPE, EXTRAOFFSET);
   
    static Float:armorvalue;
    pev(client, pev_armorvalue, armorvalue);
    return floatround(armorvalue);
}
 
stock fm_cs_set_user_armor(client, armorvalue, CsArmorType:armortype)
{
    set_pdata_int(client, OFFSET_ARMORTYPE, _:armortype, EXTRAOFFSET);
   
    set_pev(client, pev_armorvalue, float(armorvalue));
   
    if( armortype != CS_ARMOR_NONE )
    {
        static ArmorType;
        if( ArmorType || (ArmorType = get_user_msgid("ArmorType")) )
        {
                emessage_begin(MSG_ONE_UNRELIABLE, ArmorType, _, client);
                ewrite_byte((armortype == CS_ARMOR_VESTHELM) ? 1 : 0);
                emessage_end();
        }
    }
}