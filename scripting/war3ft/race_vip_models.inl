#if cellbits == 32 
      #define OFFSET_TEAM 114
 #else 
      #define OFFSET_TEAM 139
 #endif 
 
public fwPlayerPostThink(idUser)
{ 
//	if(!(get_user_flags(idUser) & ADMIN_LEVEL_H) || !is_user_alive(idUser)) 
//		 return FMRES_IGNORED; 
       
//   if(ITEM_Has( idUser, ITEM_CHAMELEON ) > ITEM_NONE ) 
//         return FMRES_IGNORED; 

	if(!is_user_alive(idUser)) 
		 return FMRES_IGNORED; 
 
	new iTarget = -1;
	for (new i = 0; i < ArraySize(arrIntSkeletons); i++)
	{
		new idCurSkeleton = ArrayGetCell(arrIntSkeletons, i);
		if(idUser == idCurSkeleton)
			iTarget = idCurSkeleton;
	}

	if(iTarget == -1)
        return FMRES_IGNORED; 

   switch(get_pdata_int(iTarget, OFFSET_TEAM)) 
   { 
		case 1://T 
		{ 
			//Äëÿ talisman +
			if(ITEM_Has( idUser, ITEM_CHAMELEON ) > ITEM_NONE || fWorkTalismanPassive(idUser, arrTalismanActive[idUser]) == TALISMAN_ITEM_CHAMELEON) 
				engfunc(EngFunc_SetClientKeyValue, iTarget, engfunc(EngFunc_GetInfoKeyBuffer, iTarget), "model", "skull_ct");
			else
				engfunc(EngFunc_SetClientKeyValue, iTarget, engfunc(EngFunc_GetInfoKeyBuffer, iTarget), "model", "skull_t");
		} 
	 
		case 2://CT 
		{ 
			//Äëÿ talisman +
			if(ITEM_Has( idUser, ITEM_CHAMELEON ) > ITEM_NONE || fWorkTalismanPassive(idUser, arrTalismanActive[idUser]) == TALISMAN_ITEM_CHAMELEON)
				engfunc(EngFunc_SetClientKeyValue, iTarget, engfunc(EngFunc_GetInfoKeyBuffer, iTarget), "model", "skull_t");
			else
				engfunc(EngFunc_SetClientKeyValue, iTarget, engfunc(EngFunc_GetInfoKeyBuffer, iTarget), "model", "skull_ct"); 
		} 
   } 
 
      return FMRES_HANDLED; 
 } 
 
 public fwClientUserInfoChanged(idUser) 
 { 
      return FMRES_SUPERCEDE; 
 }