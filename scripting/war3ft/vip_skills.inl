
//—читывание файла users_vips.ini при ициализации
public fInitVipsUsersSkills()
{
	new szConfigsDir[64];
	get_configsdir(szConfigsDir, charsmax(szConfigsDir) );

	format(szConfigsDir, charsmax(szConfigsDir), "%s/war3ft/users_skills.ini", szConfigsDir);

	fLoadSettings(szConfigsDir);

}

fLoadSettings(szFilename[])
{
	new File = fopen(szFilename,"r");
	
	if (File)
	{
		new Text[512];
		new szFlags[32];
		new szAuthData[64];
		new szWc3ChamSkill1[32];
		new szWc3ChamSkill2[32];
		new szWc3ChamSkill3[32];
		new szWc3ChamUltimate[32];
		new szWc3ChamPassive[32];
			
		while (!feof(File))
		{
			fgets(File,Text,sizeof(Text)-1);
			
			trim(Text);
			
			// comment
			if (Text[0]==';') 
			{
				continue;
			}
			
			
			szFlags[0]=0;
			szAuthData[0]=0;
			szWc3ChamSkill1[0]=0;
			szWc3ChamSkill2[0]=0;
			szWc3ChamSkill3[0]=0;
			szWc3ChamUltimate[0]=0;
			szWc3ChamPassive[0]=0;

			// not enough parameters
			if (parse(Text,
				szAuthData,sizeof(szAuthData)-1,
				szWc3ChamSkill1,sizeof(szWc3ChamSkill1)-1,
				szWc3ChamSkill2,sizeof(szWc3ChamSkill2)-1,
				szWc3ChamSkill3,sizeof(szWc3ChamSkill3)-1,
				szWc3ChamUltimate,sizeof(szWc3ChamUltimate)-1,
				szWc3ChamPassive,sizeof(szWc3ChamPassive)-1,
				szFlags,sizeof(szFlags)-1)	< 2)
			{
				continue;
			}

			ArrayPushString(arrStrAuthData, szAuthData);	
			ArrayPushCell(arrIntWc3ChamSkill1,str_to_num(szWc3ChamSkill1));
			ArrayPushCell(arrIntWc3ChamSkill2,str_to_num(szWc3ChamSkill2));
			ArrayPushCell(arrIntWc3ChamSkill3,str_to_num(szWc3ChamSkill3));
			ArrayPushCell(arrIntWc3ChamUltimate,str_to_num(szWc3ChamUltimate));
			ArrayPushCell(arrIntWc3ChamPassive,str_to_num(szWc3ChamPassive));
			ArrayPushString(arrStrFlags, szFlags);	

//log_amx("%s == %s|%s|%s|%s|%s flag = %s",szAuthData,szWc3ChamSkill1,szWc3ChamSkill2,szWc3ChamSkill3,szWc3ChamUltimate,szWc3ChamPassive,szFlags);
				
			iVipCountRace++;
		}
		
		fclose(File);
	}
		
	return 1;
}

bool:VIP_SetPlayerSkills( idUser, iChamSkills[5])
{
	new bool:bIsListEmpty = false;

	for (new i = 0; i < iVipCountRace; i++)
	{
		new szUserIP[32], szUserAuthid[32],szUserName[32];
		get_user_ip(idUser, szUserIP, charsmax(szUserIP), 1);
		get_user_authid(idUser, szUserAuthid, charsmax(szUserAuthid));
		get_user_name(idUser, szUserName, charsmax(szUserName));

		new szAuthData[64];
		ArrayGetString(arrStrAuthData,i, szAuthData, charsmax(szAuthData));

		new szFlags[64];
		ArrayGetString(arrStrFlags,i, szFlags, charsmax(szFlags));

	//	log_amx("%s == %s",szAuthData,szUserName);

		new bitFlagVip = read_flags(szFlags);
		if(equali(szAuthData,szUserAuthid) && (bitFlagVip & FLAG_AUTHID) )//Steam ID
		{
			iChamSkills[0] = ArrayGetCell(arrIntWc3ChamSkill1,i);
			iChamSkills[1] = ArrayGetCell(arrIntWc3ChamSkill2,i);
			iChamSkills[2] = ArrayGetCell(arrIntWc3ChamSkill3,i);
			iChamSkills[3] = ArrayGetCell(arrIntWc3ChamUltimate,i);
			iChamSkills[4] = ArrayGetCell(arrIntWc3ChamPassive,i);

			//log_amx("iChamSkills[3]_ID = %d",ArrayGetCell(arrIntWc3ChamUltimate,i));

			bIsListEmpty = true;

			break;
		}

		if(equali(szAuthData,szUserName) && (bitFlagVip & FLAG_KICK) )//Name
		{
			iChamSkills[0] = ArrayGetCell(arrIntWc3ChamSkill1,i);
			iChamSkills[1] = ArrayGetCell(arrIntWc3ChamSkill2,i);
			iChamSkills[2] = ArrayGetCell(arrIntWc3ChamSkill3,i);
			iChamSkills[3] = ArrayGetCell(arrIntWc3ChamUltimate,i);
			iChamSkills[4] = ArrayGetCell(arrIntWc3ChamPassive,i);

			//log_amx("iChamSkills[3]_name = %d",ArrayGetCell(arrIntWc3ChamUltimate,i));

			bIsListEmpty = true;

			break;
		}
	}

	return bIsListEmpty;

}