// ****************************************
// Blood Mage's Flame Strike
// ****************************************

Ultimate_FlameStrike(id)
{ 
   emit_sound(id, CHAN_STATIC, g_szSounds[SOUND_FLAMESTRIKE], 1.0, ATTN_NORM, 0, PITCH_NORM) 

   new vec[3] 
   new aimvec[3] 
   new velocityvec[3] 
   new length
   new speed = 10 
   get_user_origin(id,vec) 
   get_user_origin(id,aimvec,2) 
   new dist = get_distance(vec,aimvec) 

   if (p_data[id][P_FLAMECOUNT] == 1)
   {
    //  new parm[2]
     // parm[0]=id

      //p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
	  p_data[id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown );
      //_Ultimate_Delay(parm)
	  _ULT_Delay();
   }
   new speed1 = 160 
   new speed2 = 350 
   new radius = 105 

   if(dist < 50){ 
      radius = 0 
      speed = 5 
   } 
   else if(dist < 150){ 
      speed1 = speed2 = 1 
      speed = 5 
      radius = 50 
   } 
   else if(dist < 200){ 
      speed1 = speed2 = 1 
      speed = 5 
      radius = 90 
   } 
   else if(dist < 250){ 
      speed1 = speed2 = 90 
      speed = 6 
      radius = 90 
   } 
   else if(dist < 300){ 
      speed1 = speed2 = 140 
      speed = 7 
   } 
   else if(dist < 350){ 
      speed1 = speed2 = 190 
      speed = 7 
   } 
   else if(dist < 400){ 
      speed1 = 150 
      speed2 = 240 
      speed = 8 
   } 
   else if(dist < 450){ 
      speed1 = 150 
      speed2 = 290 
      speed = 8 
   } 
   else if(dist < 500){ 
      speed1 = 180 
      speed2 = 340 
      speed = 9 
   } 

   velocityvec[0]=aimvec[0]-vec[0] 
   velocityvec[1]=aimvec[1]-vec[1] 
   velocityvec[2]=aimvec[2]-vec[2] 
   length=sqroot(velocityvec[0]*velocityvec[0]+velocityvec[1]*velocityvec[1]+velocityvec[2]*velocityvec[2]) 
   velocityvec[0]=velocityvec[0]*speed/length 
   velocityvec[1]=velocityvec[1]*speed/length 
   velocityvec[2]=velocityvec[2]*speed/length 

   new args[8] 
   args[0] = vec[0] 
   args[1] = vec[1] 
   args[2] = vec[2] 
   args[3] = velocityvec[0] 
   args[4] = velocityvec[1] 
   args[5] = velocityvec[2] 
	
   set_task(0.1,"te_spray",TASK_ONFIRE+id,args,8,"a",2) 
   check_burnzone(id,vec,aimvec,speed1,speed2,radius) 

} 

public te_spray(args[])
{ 
	new position[3], direction[3]
	position[0] = args[0]
	position[1] = args[1]
	position[2] = args[2]
	direction[0] = args[3]
	direction[1] = args[4]
	direction[2] = args[5]

	Create_TE_SPRAY(position, direction, g_iSprites[SPR_FIRE], 8, 70, 100, 5)

	return PLUGIN_CONTINUE 
} 

check_burnzone(id,vec[],aimvec[],speed1,speed2,radius)
{ 
	
	new maxplayers = get_maxplayers()
	new bodypart, victim 
	
	get_user_aiming(id,victim,bodypart,550) 

	if( victim > 0 && victim <= maxplayers )
	{
		if( get_user_team(victim) != get_user_team(id) )
			burn_victim(victim,id,bodypart)
	}
    
	new burnvec1[3],burnvec2[3],length1 

	burnvec1[0]=aimvec[0]-vec[0] 
	burnvec1[1]=aimvec[1]-vec[1] 
	burnvec1[2]=aimvec[2]-vec[2] 

	length1=sqroot(burnvec1[0]*burnvec1[0]+burnvec1[1]*burnvec1[1]+burnvec1[2]*burnvec1[2]) 
	burnvec2[0]=burnvec1[0]*speed2/length1 
	burnvec2[1]=burnvec1[1]*speed2/length1 
	burnvec2[2]=burnvec1[2]*speed2/length1 
	burnvec1[0]=burnvec1[0]*speed1/length1 
	burnvec1[1]=burnvec1[1]*speed1/length1 
	burnvec1[2]=burnvec1[2]*speed1/length1 
	burnvec1[0] += vec[0] 
	burnvec1[1] += vec[1] 
	burnvec1[2] += vec[2] 
	burnvec2[0] += vec[0] 
	burnvec2[1] += vec[1] 
	burnvec2[2] += vec[2] 

	new origin[3], players[32], numberofplayers
	get_players(players, numberofplayers)

	for (new i=0; i < numberofplayers; i++)
	{
		victim = players[i]
		if( is_user_alive(victim) && get_user_team(victim)!=get_user_team(id) )
		{ 
			get_user_origin(victim,origin) 
			if(get_distance(origin, burnvec1) < radius) 
				burn_victim(victim,id,bodypart) 
			else if(get_distance(origin, burnvec2) < radius) 
				burn_victim(victim,id,bodypart) 
		} 
	} 
	return PLUGIN_CONTINUE       
} 

burn_victim(victim,attacker,bodypart)
{ 
//	if(p_data_b[victim][PB_ISBURNING]) 
	//	return PLUGIN_CONTINUE 
		 
//	if (ITEM_Has( victim, ITEM_NECKLACE ) > ITEM_NONE || p_data_b[victim][PB_WARDENBLINK])
//		return PLUGIN_CONTINUE

//	p_data_b[victim][PB_ISBURNING] = true

	new hp,args[3] 
	hp = get_user_actualhealth(victim) 

	args[0] = victim 
	args[1] = attacker  
	args[2] = bodypart
	set_task(0.3,"on_fire",TASK_ONFIRE,args,3,"a",hp / 10) 

	return PLUGIN_CONTINUE 
} 

public on_fire(args[])
{ 
	
	new victim = args[0]
	new attacker = args[1] 
	new bodypart = args[2]

	if (!bodypart)
		bodypart = random_num(1,8)

//	if(!p_data_b[victim][PB_ISCONNECTED])
//		return PLUGIN_CONTINUE


	//Если уже горел
//	if(!p_data_b[victim][PB_ISBURNING])
//		return PLUGIN_CONTINUE 

	new rx,ry,rz, origin[3], position[3]

	rx = random_num(-30,30) 
	ry = random_num(-30,30) 
	rz = random_num(-30,30) 

	get_user_origin(victim,origin) 

	position[0] = origin[0] + rx
	position[1] = origin[1] + ry
	position[2] = origin[2] + 10 + rz

	Create_TE_SPRITE(position,g_iSprites[SPR_BURNING], 30, 200)

	position[0] = origin[0]+(rx*2)
	position[1] = origin[1]+(ry*2)
	position[2] = origin[2]+100+(rz*2)

	Create_TE_Smoke(position, position, g_iSprites[SPR_SMOKE], 60, 15)


	if(!is_user_alive(victim)) 
		return PLUGIN_CONTINUE; 

	if (ITEM_Has( victim, ITEM_NECKLACE ) > ITEM_NONE )
		return PLUGIN_CONTINUE;

	WC3_Damage( victim, attacker, 10, CSW_CHAM_FLAME_STRIKE, bodypart );


//	SHARED_GlowShell(victim,210,105,30,get_pcvar_float( CVAR_wc3_glowshell_time ),get_pcvar_num( CVAR_wc3_glowshell_amount ));

	return PLUGIN_CONTINUE 
}