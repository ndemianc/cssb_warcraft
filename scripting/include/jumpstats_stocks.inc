#if defined _jumpstats_stocks_included
	#endinput
#endif

#define _jumpstats_stocks_included

#include <jumpstats_const>

#include <amxmodx>
#include <amxmisc>
#include <fakemeta>
#include <cstrike>
#include <fun>
#include <xs>

stock Color:CvarToColor(cvar)
{
	switch( get_pcvar_num(cvar) )
	{
		case 1: return GREEN;
		case 2: return TEAM_COLOR;
		case 3: return GREY;
		case 4: return RED;
		case 5: return BLUE;
	}
	
	return NORMAL;
}

stock CvarToRGB(cvar, &r, &g, &b)
{
	/*static color[16];
	get_pcvar_string(cvar, color, sizeof(color) - 1);
	
	if( contain( color, " " ) != -1 )
	{
		static piece[5];
		strbreak(color, piece, sizeof(piece) - 1, color, sizeof(color) - 1);
		r = str_to_num(piece);
		
		strbreak(color, piece, sizeof(piece) - 1, color, sizeof(color) - 1);
		g = str_to_num(piece);
		b = str_to_num(color);
	}
	else
	{
		new iValue = str_to_num( color );
		
		r = iValue / 1000000;
		iValue %= 1000000;
		g = iValue / 1000;
		b = iValue % 1000;
	}
	
	r = clamp( r, 0, 255 );
	g = clamp( g, 0, 255 );
	b = clamp( b, 0, 255 );*/
	
	static color[16];
	get_pcvar_string(cvar, color, sizeof(color) - 1);
	
	static piece[5];
	strbreak(color, piece, sizeof(piece) - 1, color, sizeof(color) - 1);
	r = str_to_num(piece);
	
	strbreak(color, piece, sizeof(piece) - 1, color, sizeof(color) - 1);
	g = str_to_num(piece);
	b = str_to_num(color);
}

stock LoadResetCommands(function[])
{
	new filename[64];
	get_configsdir(filename, sizeof(filename) - 1);
	
	add(filename, sizeof(filename) - 1, "/jumpstats_reset.ini");
	
	// dont need to check if file exists, because in jumpstats_main, it creates one
	
	new f = fopen(filename, "rt");
	
	new data[64];
	while( !feof(f) )
	{
		fgets(f, data, sizeof(data) - 1);
		
		if( !data[0]
		|| data[0] == ';'
		|| data[0] == '/' && data[1] == '/' ) continue;
		
		register_clcmd(data, function);
	}
	
	fclose(f);
}

stock FormatSaveFiles()
{
	static bool:executed;
	if( executed ) return;
	executed = true;
	
	new dir[64];
	get_datadir(dir, sizeof(dir) - 1);
	add(dir, sizeof(dir) - 1, "/jumpstats");
	
	if( !dir_exists(dir) )
	{
		mkdir(dir);
	}
	
	for( new i = 0; i < JUMP_TYPES; i++ )
	{
		format(g_jump_filenames[i], sizeof(g_jump_filenames[]) - 1, "%s/%s.txt", dir, g_jump_filenames[i]);
	}
}

stock GetDirection(client)
{
	static Float:angles[3];
	pev(client, pev_v_angle, angles);
	
	engfunc(EngFunc_MakeVectors, angles);
	global_get(glb_v_forward, angles);
	angles[2] = 0.0;
	xs_vec_normalize(angles, angles);
	xs_vec_mul_scalar(angles, 250.0, angles);
	
	static Float:velocity[3];
	pev(client, pev_velocity, velocity);
	
	new Float:angle = AngleBetweenVectors(angles, velocity, 2, degrees);
	
	if( 45.5 <= angle < 120.0 )
	{
		return DIR_SIDEWAYS;
	}
	else if( 134.5 <= angle <= 180.0 )
	{
		return DIR_BACKWARDS;
	}
	
	return DIR_FORWARDS;
}

stock DirectionNameToValue(const direction[])
{
	switch( direction[0] )
	{
		//case 'F', 'f':	return DIR_FORWARDS;
		case 'S', 's':	return DIR_SIDEWAYS;
		case 'B', 'b':	return DIR_BACKWARDS;
	}
	
	return DIR_FORWARDS;
}

stock bool:IsTeamAllowed(client, teamflags)
{
	static const req_flags[CsTeams] =
	{
		(1 << 2), // c = spectator
		(1 << 0), // a = t
		(1 << 1), // b = ct
		(1 << 2)  // c = spectator
	};
	
	new CsTeams:team = cs_get_user_team(client);
	
	if( !( teamflags & req_flags[ team ] ) )
	{
		static max_players;
		if( !max_players )
		{
			max_players = get_maxplayers();
		}
		
		for( new i = 1; i <= max_players; i++ )
		{
			if( is_user_alive(i) && cs_get_user_team(i) != team )
			{
				return false;
			}
		}
	}
	
	return true;
	
	/*new team = max(0, (_:cs_get_user_team(client)) - 1);
	new flag = 1 << team;
	
	return bool:(!!(teamflags & flag));*/
	
	//return !!(teamflags & (1 << max(0, (_:cs_get_user_team(client)) - 1)));
}

stock bool:IsTechAllowed(jump_type, techflags)
{
	return !!(techflags & (1 << jump_type));
}

stock Float:AngleBetweenVectors(Float:vector1[3], Float:vector2[3], dimensions, anglemode:angletype)
{
	if( !(2 <= dimensions <= 3) ) return 0.0;
	
	static Float:v1[3], Float:v2[3];
	xs_vec_copy(vector1, v1);
	xs_vec_copy(vector2, v2);
	
	if( dimensions == 2 )
	{
		v1[2] = v2[2] = 0.0;
	}
	
	// cos angle = (v1 * v2) / (v1.length * v2.length)
	// angle = acos((v1 * v2) / (v1.length * v2.length))
	// v1 * v2 = dot product
	
	new Float:lengths = vector_length(v1) * vector_length(v2);
	if( lengths == 0.0 )
	{
		lengths = 1.0;
	}
	
	return floatacos(xs_vec_dot(v1, v2) / lengths, angletype);
}

stock GetSpectatedPlayer(client)
{
	if( !is_user_alive(client)
	&& ((1 << pev(client, pev_iuser1)) & ((1 << 1)|(1 << 2)|(1 << 4))) )
	{
		new player = pev(client, pev_iuser2);
		if( is_user_alive(player) )
		{
			return player;
		}
	}
	
	return client;
}

stock bool:IsUserSpectatingPlayer(spectater, player)
{
	return (GetSpectatedPlayer(spectater) == player);
}

stock bool:IsUserDucking(client)
{
	static Float:absmin[3], Float:absmax[3];
	
	pev(client, pev_absmin, absmin);
	pev(client, pev_absmax, absmax);
	
	return !((absmin[2] + 64.0) < absmax[2]);
}

stock bool:IsUserOnGround(client)
{
	return !!(pev(client, pev_flags) & FL_ONGROUND2);
}

// Distance calculations are courtesy of SchlumPF, which he optimized the code he got from NumB, who got them from eDark.

// frame_x[0] = data out of the first frame after jumping ; frame_x[1] = data out of the last frame before landing
stock CalculateLandOrigin(bool:ducking, Float:gravity, Float:current_origin[3], Float:frame_origin[2][3], Float:frame_velocity[2][3], Float:land_origin[3])
{
	static Float:airtime;
	static Float:distancex, Float:distancey;
	
	airtime = floatdiv(((floatsqroot(frame_velocity[0][2] * frame_velocity[0][2] + (2 * gravity * (frame_origin[0][2] - current_origin[2]))) * -1) - frame_velocity[1][2]), (gravity * -1));
	
	frame_velocity[1][0] = floatabs(frame_velocity[1][0]);
	frame_velocity[1][1] = floatabs(frame_velocity[1][1]);
	
	distancex = airtime * frame_velocity[1][0];
	distancey = airtime * frame_velocity[1][1];
	
	if( frame_origin[1][0] < current_origin[0] )	land_origin[0] = frame_origin[1][0] + distancex;
	else						land_origin[0] = frame_origin[1][0] - distancex;
	
	if( frame_origin[1][1] < current_origin[1] )	land_origin[1] = frame_origin[1][1] + distancey;
	else						land_origin[1] = frame_origin[1][1] - distancey;
	
	if( ducking ) current_origin[2] += 18.0;
	land_origin[2] = current_origin[2];
}

// frame_x[0] = data out of the first frame after jumping ; failed_x[1] = data out of the last frame in which the player could have landed
stock Float:GetFailedDistance( bool:ducking, Float:gravity, Float:jumpoff_origin[3], Float:current_velocity[3], Float:failed_origin[3], Float:failed_velocity[3] )
{
	static Float:airtime, Float:land_origin[3], Float:distance;
	
	if( ducking ) jumpoff_origin[2] -= 18.0;
			
	airtime = ((floatsqroot((failed_velocity[2] * failed_velocity[2]) - (2.0 * -gravity * (failed_origin[2] - jumpoff_origin[2])))*-1) - failed_velocity[2] ) / -gravity;
	
	land_origin[0] = floatabs( failed_origin[0] - jumpoff_origin[0] ) + floatabs( current_velocity[0] * airtime );
	land_origin[1] = floatabs( failed_origin[1] - jumpoff_origin[1] ) + floatabs( current_velocity[1] * airtime );
	
	distance = vector_length(land_origin) + EXTRA_DISTANCE;
	
	if( ducking ) jumpoff_origin[2] += 18.0;
	
	return distance;
}

stock Float:GetClientWeaponMaxspeed(client)
{
	static const OFFSET_SHIELD = 510;
	static const HAS_SHIELD = 1 << 24;
	static const USING_SHIELD = 1 << 16;
	
	static Float:maxspeed;
	
	new shield = get_pdata_int(client, OFFSET_SHIELD);
	if( shield & USING_SHIELD )
	{
		maxspeed = 180.0;
	}
	else if( shield & HAS_SHIELD )
	{
		maxspeed = 250.0;
	}
	else
	{
		static const m_pActiveItem = 373;
		
		ExecuteHamB(Ham_CS_Item_GetMaxSpeed, get_pdata_cbase(client, m_pActiveItem), maxspeed);
		
		/*switch( weapon )
		{
			case CSW_SG550, CSW_AWP, CSW_G3SG1:
			{
				maxspeed = 210.0;
			}
			case CSW_M249:
			{
				maxspeed = 220.0;
			}
			case CSW_AK47:
			{
				maxspeed = 221.0;
			}
			case CSW_M3, CSW_M4A1:
			{
				maxspeed = 230.0;
			}
			case CSW_SG552:
			{
				maxspeed = 235.0;
			}
			case CSW_XM1014, CSW_AUG, CSW_GALIL, CSW_FAMAS:
			{
				maxspeed = 240.0;
			}
			case CSW_P90:
			{
				maxspeed = 245.0;
			}
			case CSW_SCOUT:
			{
				maxspeed = 260.0;
			}
			// in case there is no weapon, or something bugged
			// I like to have precautions =)
			//case CSW_P228, CSW_HEGRENADE, CSW_C4, CSW_MAC10, CSW_SMOKEGRENADE, CSW_ELITE, CSW_FIVESEVEN,
			//	CSW_UMP45, CSW_USP, CSW_GLOCK18, CSW_MP5NAVY, CSW_TMP, CSW_FLASHBANG, CSW_DEAGLE, CSW_KNIFE:
			default:
			{
				maxspeed = 250.0;
			}
		}*/
	}
	
	if( cs_get_user_vip(client) )
	{
		static const Float:VIP_MAXSPEED = 227.0;
		
		// fix for plugins using VIP status but didn't set maxspeed
		// also fix for plugins using cs_set_user_vip() but player's maxspeed hasn't changed yet
		if( get_user_maxspeed(client) == maxspeed )
		{
			set_user_maxspeed(client, VIP_MAXSPEED);
		}
		
		return VIP_MAXSPEED;
	}
	
	return maxspeed;
}

stock MakeNameMOTDSafe(name[128], maxchars)
{
	name[32] = 0;
	
	replace_all(name, 127, "<", "&lt;");
	replace_all(name, 127, ">", "&gt;");
	
	if( maxchars > 128 ) return;
	
	new last;
	for( new i = 0; i < 128 && name[i]; )
	{
		if( name[i] == '&'
		&& (name[i + 1] == 'l' || name[i + 1] == 'g')
		&& name[i + 2] == 't'
		&& name[i + 3] == ';' )
		{
			i += 3;
		}
		
		if( ++i > maxchars )
		{
			break;
		}
		
		last = i;
	}
	
	name[last] = 0;
}

stock bool:IsUserSkywalking(client)
{
	if( pev(client, pev_flags) & FL_ONGROUND )
	{
		static Float:start[3];
		pev(client, pev_origin, start);
		
		static Float:end[3];
		pev(client, pev_mins, end);
		end[0] = end[1] = 0.0;
		xs_vec_add(start, end, end);
		end[2] -= 70.0;
		
		engfunc(EngFunc_TraceLine, start, end, 0, client);
		
		global_get(glb_trace_fraction, end[0]);
		
		if( end[0] == 1.0 )
		{
			return true;
		}
	}
	
	return false;
}

stock bool:IsWeaponAllowed(client, weapon, weaponflags)
{
	/*new real = cs_get_user_shield(client) ? 2 : weapon;
	
	return !!(weaponflags & (1 << real));*/
	
	return !!(weaponflags & (1 << (cs_get_user_shield(client) ? 2 : weapon)));
}

// Credits to SchlumPF
stock GetEdgeDistances( iJumpType, bool:bDucking, bool:bFailed, Float:vJumpedAt[ 3 ], Float:vLandedAt[ 3 ], bool:bJumpEntity,\
	Float:vEntityOrigin[ 3 ], Float:vEntityAngles[ 3 ], Float:vEntityMins[ 3 ], Float:vEntityMaxs[ 3 ],\
	&Float:fJumpEdge, &Float:fLandEdge, &iBlock )
{
	fJumpEdge = -1.0;
	fLandEdge = -1.0;
	iBlock = -1;
	
	vJumpedAt[ 2 ] -= 36.0;
	vLandedAt[ 2 ] = vJumpedAt[ 2 ];
	
	static Float:vMiddle[ 3 ];
	xs_vec_add( vJumpedAt, vLandedAt, vMiddle );
	xs_vec_div_scalar( vMiddle, 2.0, vMiddle );
	vMiddle[ 2 ] = vLandedAt[ 2 ] - 1.0;
	
	if( engfunc( EngFunc_PointContents, vMiddle ) != CONTENTS_EMPTY )
	{
		//client_print( 0, print_chat, "[EdgeDistances] Middle origin is not empty." );
		
		return 0;
	}
	
	static iHull;
	iHull = ( bDucking || bFailed ) ? HULL_HEAD : HULL_HUMAN;
	
	vJumpedAt[ 2 ] = vLandedAt[ 2 ] = vMiddle[ 2 ];
	
	static Float:vPlaneNormal[ 3 ], Float:vJumpPos[ 3 ], Float:vLandPos[ 3 ];
	
	if( bJumpEntity )
	{
		static Float:vStop[ 3 ];
		xs_vec_sub( vJumpedAt, vMiddle, vStop );
		vStop[ 2 ] = 0.0;
		new Float:fLength = vector_length( vStop );
		vStop[ 0 ] = vStop[ 0 ] / fLength * ( fLength + EXTRA_DISTANCE );
		vStop[ 1 ] = vStop[ 1 ] / fLength * ( fLength + EXTRA_DISTANCE );
		xs_vec_add( vStop, vMiddle, vStop );
		
		static Float:vJumpPos[ 3 ], Float:vPlaneNormal[ 3 ];
		
		new Float:fDistance = GetBoxHitPos(
			vEntityOrigin,\
			vEntityAngles,\
			vEntityMins,\
			vEntityMaxs,\
			vMiddle,\
			vStop,\
			vJumpPos,\
			vPlaneNormal
			);
		
		if( !fDistance )
		{
			//client_print( 0, print_chat, "[EdgeDistances] Could not find the edge of the jump entity");
			
			return 0;
		}
		
		if( vPlaneNormal[ 1 ] == 0.0 )
		{
			if( iJumpType != JUMP_LADDERJUMP )
			{
				fJumpEdge = floatabs( vJumpPos[ 0 ] - vJumpedAt[ 0 ] );
			}
		}
		else if( vPlaneNormal[ 0 ] == 0.0 )
		{
			if( iJumpType != JUMP_LADDERJUMP )
			{
				fJumpEdge = floatabs( vJumpPos[ 1 ] - vJumpedAt[ 1 ] );
			}
		}
		else
		{
			//client_print( 0, print_chat, "[EdgeDistances] Invalid vPlaneNormal for jump entity { %f, %f }", vPlaneNormal[ 0 ], vPlaneNormal[ 1 ]);
			
			return 0;
		}
	}
	else
	{
		engfunc( EngFunc_TraceHull, vMiddle, vJumpedAt, IGNORE_MONSTERS, iHull, -1, 0 );
		get_tr2( 0, TR_vecPlaneNormal, vPlaneNormal );
		
		if( vPlaneNormal[ 2 ] != 0.0 )
		{
			//client_print( 0, print_chat, "[EdgeDistances] vPlaneNormal[ 2 ] is not 0.0 for jump edge");
			
			return 0;
		}
		
		get_tr2( 0, TR_vecEndPos, vJumpPos );
		
		if( floatabs( vPlaneNormal[ 0 ] ) == 1.0 && vPlaneNormal[ 1 ] == 0.0 )
		{
			if( iJumpType != JUMP_LADDERJUMP )
			{
				fJumpEdge = floatabs( vJumpPos[ 0 ] - vJumpedAt[ 0 ] );
			}
		}
		else if( vPlaneNormal[ 0 ] == 0.0 && floatabs( vPlaneNormal[ 1 ] ) == 1.0 )
		{
			if( iJumpType != JUMP_LADDERJUMP )
			{
				fJumpEdge = floatabs( vJumpPos[ 1 ] - vJumpedAt[ 1 ] );
			}
		}
		else
		{
			//client_print( 0, print_chat, "[EdgeDistances] Invalid vPlaneNormal for jump wall { %f, %f }", vPlaneNormal[ 0 ], vPlaneNormal[ 1 ]);
			
			return 0;
		}
	}
	
	fJumpEdge -= HLBSP_EXTRA;
	
	if( bFailed )
	{
		static Float:vEnd[ 3 ];
		xs_vec_copy( vLandedAt, vEnd );
		
		if( vPlaneNormal[ 1 ] == 0.0 )
		{
			vEnd[ 0 ] += ( vPlaneNormal[ 0 ] * 300.0 );
		}
		else //if( vPlaneNormal[ 0 ] == 0.0 )
		{
			vEnd[ 1 ] += ( vPlaneNormal[ 1 ] * 300.0 );
		}
		
		engfunc( EngFunc_TraceHull, vMiddle, vEnd, IGNORE_MONSTERS, iHull, -1, 0 );
		get_tr2( 0, TR_vecPlaneNormal, vPlaneNormal );
		
		if( vPlaneNormal[ 2 ] != 0.0 )
		{
			//client_print( 0, print_chat, "[EdgeDistances] vPlaneNormal[ 2 ] is not 0.0 for land edge");
			
			return 0;
		}
		
		get_tr2( 0, TR_vecEndPos, vLandPos );
		
		if( floatabs( vPlaneNormal[ 0 ] ) == 1.0 && vPlaneNormal[ 1 ] == 0.0 )
		{
			vEnd[ 0 ] = vLandPos[ 0 ] - ( vPlaneNormal[ 0 ] * 16.1 );
			vEnd[ 1 ] = vLandPos[ 1 ];
			vEnd[ 2 ] = vLandPos[ 2 ] + 37.0;
		}
		else if( vPlaneNormal[ 0 ] == 0.0 && floatabs( vPlaneNormal[ 1 ] ) == 1.0 )
		{
			vEnd[ 0 ] = vLandPos[ 0 ];
			vEnd[ 1 ] = vLandPos[ 1 ] - ( vPlaneNormal[ 1 ] * 16.1 );
			vEnd[ 2 ] = vLandPos[ 2 ] + 37.0;
		}
		else
		{
			//client_print( 0, print_chat, "[EdgeDistances] Invalid vPlaneNormal for land wall { %f, %f }", vPlaneNormal[ 0 ], vPlaneNormal[ 1 ]);
			
			return 0;
		}
		
		vMiddle[ 2 ] += 37.0;
		
		engfunc( EngFunc_TraceHull, vMiddle, vEnd, IGNORE_MONSTERS, iHull, -1, 0 );
		
		static Float:fFraction;
		get_tr2( 0, TR_flFraction, fFraction );
		
		if( fFraction < 1.0 )
		{
			//client_print( 0, print_chat, "[EdgeDistances] Something wrong with failed");
			
			return 0;
		}
	}
	else
	{
		engfunc( EngFunc_TraceHull, vMiddle, vLandedAt, IGNORE_MONSTERS, iHull, -1, 0 );
		get_tr2( 0, TR_vecPlaneNormal, vPlaneNormal );
		
		if( vPlaneNormal[ 2 ] != 0.0 )
		{
			//client_print( 0, print_chat, "[EdgeDistances] vPlaneNormal[ 2 ] is not 0.0 for land edge");
			
			return 0;
		}
		
		get_tr2( 0, TR_vecEndPos, vLandPos );
	}
	
	if( iJumpType == JUMP_LADDERJUMP )
	{
		xs_vec_copy( vJumpedAt, vJumpPos );
	}
	
	static Float:fBlock;
	
	if( floatabs( vPlaneNormal[ 0 ] ) == 1.0 && vPlaneNormal[ 1 ] == 0.0 )
	{
		fLandEdge = floatabs( vLandPos[ 0 ] - vLandedAt[ 0 ] );
		fBlock = floatabs( vJumpPos[ 0 ] - vLandPos[ 0 ] );
	}
	else if( vPlaneNormal[ 0 ] == 0.0 && floatabs( vPlaneNormal[ 1 ] ) == 1.0 )
	{
		fLandEdge = floatabs( vLandPos[ 1 ] - vLandedAt[ 1 ] );
		fBlock = floatabs( vJumpPos[ 1 ] - vLandPos[ 1 ] );
	}
	else
	{
		//client_print( 0, print_chat, "[EdgeDistances] Invalid vPlaneNormal for land wall { %f, %f }. floatabs( 1.000000 ) = %f", vPlaneNormal[ 0 ], vPlaneNormal[ 1 ], floatabs( 1.000000 ));
		
		return 0;
	}
	
	fLandEdge -= HLBSP_EXTRA;
	
	if( iJumpType != JUMP_LADDERJUMP )
	{
		if( bJumpEntity )
		{
			fBlock += ( ( EXTRA_DISTANCE / 2.0 ) + 1.0 );
		}
		else
		{
			fBlock += EXTRA_DISTANCE;
		}
	}
	/*else
	{
		fBlock += 0.5;
	}*/
	
	//client_print( 0, print_chat, "fBlock = %f", fBlock );
	
	iBlock = floatround( fBlock );
	
	return ( g_iBlockSizes[ iJumpType ][ BLOCK_MIN ] <= iBlock <= g_iBlockSizes[ iJumpType ][ BLOCK_MAX ] ) ? 1 : 0;
}

// Thanks to joaquimandrade for the following code below
stock const g_iAngleDirections[ ] =
{
	ANGLEVECTOR_FORWARD,
	ANGLEVECTOR_RIGHT,
	ANGLEVECTOR_UP
};

enum Size
{
	SIZE_MINS,
	SIZE_MAXS
};

stock Float:GetBoxHitPos( Float:vOrigin[ 3 ], Float:vAngles[ 3 ], Float:vMins[ 3 ], Float:vMaxs[ 3 ], Float:vStart[ 3 ], Float:vStop[ 3 ], Float:vHit[ 3 ], Float:vHitPlane[ 3 ] )
{
	static Float:vAngleVectors[ sizeof( g_iAngleDirections ) ][ 3 ];
	
	for( new i = 0; i < sizeof( g_iAngleDirections ); i++ )
	{
		angle_vector( vAngles, g_iAngleDirections[ i ], vAngleVectors[ i ] );
		xs_vec_normalize( vAngleVectors[ i ], vAngleVectors[ i ] );
	}
	
	static const iPlaneData[ ][ 3 ] =
	{
		{ ANGLEVECTOR_FORWARD, _:SIZE_MINS, 0 },
		{ ANGLEVECTOR_FORWARD, _:SIZE_MAXS, 0 },
		{ ANGLEVECTOR_RIGHT, _:SIZE_MINS, 1 },
		{ ANGLEVECTOR_RIGHT, _:SIZE_MAXS, 1 }
	};
	
	static const Float:fDefaultDistance = 99999.9;
	
	new Float:fSmallestDistance = fDefaultDistance;
	
	static Float:vDirection[ 3 ];
	xs_vec_sub( vStop, vStart, vDirection );
	
	static Float:vSize[ Size ][ 3 ];
	xs_vec_copy( vMins, vSize[ SIZE_MINS ] );
	xs_vec_copy( vMaxs, vSize[ SIZE_MAXS ] );
	
	static Float:vPlane[ 4 ], Float:vHit2[ 3 ], Float:fDistance;
	for( new i = 0; i < sizeof( iPlaneData ); i++ )
	{
		GetPlane( vOrigin, vSize[ Size:iPlaneData[ i ][ 1 ] ][ iPlaneData[ i ][ 2 ] ], vAngleVectors, iPlaneData[ i ][ 0 ], vPlane );
		
		if( xs_plane_rayintersect( vPlane, vStart, vDirection, vHit2 ) )
		{
			fDistance = vector_distance( vStart, vHit2 );
			
			if( fSmallestDistance > fDistance )
			{
				fSmallestDistance = fDistance;
				xs_vec_copy( vHit2, vHit );
				xs_vec_copy( vPlane, vHitPlane );
			}
		}
	}
	
	return ( fSmallestDistance == fDefaultDistance ) ? 0.0 : fSmallestDistance;
}

stock GetPlane( Float:vOrigin[ 3 ], Float:fSizeScalar, Float:vAngleVectors[ sizeof( g_iAngleDirections ) ][ 3 ], iAngle, Float:vPlane[ 4 ] )
{
	static Float:vPath[ 3 ];
	xs_vec_mul_scalar( vAngleVectors[ iAngle - 1 ], fSizeScalar, vPath );
	xs_vec_add( vPath, vOrigin, vPath );
	
	static Float:vExtraPoints[ 2 ][ 3 ], iPoint;
	iPoint = 0;
	
	static Float:vExtraPath[ 3 ];
	for( new i = 0; i < sizeof( g_iAngleDirections ); i++ )
	{
		if( g_iAngleDirections[ i ] != iAngle )
		{
			xs_vec_mul_scalar( vAngleVectors[ i ], 10.0, vExtraPath );
			xs_vec_add( vExtraPath, vPath, vExtraPoints[ iPoint++ ] );
		}
	}
	
	xs_plane_3p( vPlane, vPath, vExtraPoints[ 0 ], vExtraPoints[ 1 ] );
}
