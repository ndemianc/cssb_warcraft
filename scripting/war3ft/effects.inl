// ***************************************************************************
// Author: B. Josh Becker
// Description: This page contains stocks, mostly functions that create messages
//		that are sent to the client
// ***************************************************************************

stock Create_TE_Smoke(originSight[3], position[3], iSprite, scale, framerate){

	message_begin( MSG_PVS, SVC_TEMPENTITY, originSight )
	write_byte( TE_SMOKE )
	write_coord( position[0] )			// Position
	write_coord( position[1] )
	write_coord( position[2] )
	write_short( iSprite )				// Sprite index
	write_byte( scale )					// scale * 10
	write_byte( framerate  )			// framerate
	message_end()
}

stock Create_DeathMsg_CS(killer_id,victim_id,headshot,weaponname[])
{
	
	message_begin(MSG_ALL,gmsgDeathMsg)
	write_byte(killer_id)
	write_byte(victim_id)
	write_byte(headshot)
	write_string(weaponname)
	message_end()

	//client_print(killer_id,print_chat,"%s",weaponname);

}

stock Create_TE_IMPLOSION(position[3], radius, count, life)
{

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
	write_byte ( TE_IMPLOSION )
	write_coord( position[0] )			// position (X)
	write_coord( position[1] )			// position (Y)
	write_coord( position[2] )			// position (Z)
	write_byte ( radius )				// radius
	write_byte ( count )				// count
	write_byte ( life )					// life in 0.1's
	message_end()
}

stock Create_TE_BEAMCYLINDER(origin[3], center[3], axis[3], iSprite, startFrame, frameRate, life, width, amplitude, red, green, blue, brightness, speed){

	message_begin( MSG_PAS, SVC_TEMPENTITY, origin )
	write_byte( TE_BEAMCYLINDER )
	write_coord( center[0] )			// center position (X)
	write_coord( center[1] )			// center position (Y)
	write_coord( center[2] )			// center position (Z)
	write_coord( axis[0] )				// axis and radius (X)
	write_coord( axis[1] )				// axis and radius (Y)
	write_coord( axis[2] )				// axis and radius (Z)
	write_short( iSprite )				// sprite index
	write_byte( startFrame )			// starting frame
	write_byte( frameRate )				// frame rate in 0.1's
	write_byte( life )					// life in 0.1's
	write_byte( width )					// line width in 0.1's
	write_byte( amplitude )				// noise amplitude in 0.01's
	write_byte( red )					// color (red)
	write_byte( green )					// color (green)
	write_byte( blue )					// color (blue)
	write_byte( brightness )			// brightness
	write_byte( speed )					// scroll speed in 0.1's
	message_end()
}

stock Create_TE_EXPLOSION( origin[3], origin2[3], iSprite, scale, frameRate, flags ){

	message_begin( MSG_PVS, SVC_TEMPENTITY, origin )
	write_byte( TE_EXPLOSION )
	write_coord( origin2[0] )			// position (X)
	write_coord( origin2[1] )			// position (Y)
	write_coord( origin2[2]	)			// position (Z)
	write_short( iSprite )				// sprite index
	write_byte( scale )					// scale in 0.1's
	write_byte( frameRate )				// framerate
	write_byte( flags )					// flags
	message_end()
}

stock Create_ScreenShake(idUser, amount, duration, frequency){

	message_begin(MSG_ONE,gmsgScreenShake,{0,0,0},idUser) 
	write_short( amount )				// ammount 
	write_short( duration )				// lasts this long 
	write_short( frequency )			// frequency
	message_end()
}

stock Create_ScreenFade(idUser, duration, holdtime, fadetype, red, green, blue, alpha){

	message_begin( MSG_ONE,gmsgScreenFade,{0,0,0},idUser )			
	write_short( duration )			// fade lasts this long duration
	write_short( holdtime )			// fade lasts this long hold time
	write_short( fadetype )			// fade type (in / out)
	write_byte( red )				// fade red
	write_byte( green )				// fade green
	write_byte( blue )				// fade blue
	write_byte( alpha )				// fade alpha
	message_end()
}

stock Create_ScoreInfo(idUser,frags,deaths,playerClass,team ){

	message_begin(MSG_ALL,gmsgScoreInfo)
	write_byte(idUser)
	write_short(frags)
	write_short(deaths)
	write_short(playerClass)
	write_short(team)
	message_end()
}

stock Create_TE_SPRITETRAIL(start[3], end[3], iSprite, count, life, scale, velocity, random ){

	message_begin( MSG_BROADCAST,SVC_TEMPENTITY)
	write_byte( TE_SPRITETRAIL )
	write_coord( start[0] )				// start position (X)
	write_coord( start[1] )				// start position (Y)
	write_coord( start[2] )				// start position (Z)
	write_coord( end[0] )				// end position (X)
	write_coord( end[1] )				// end position (Y)
	write_coord( end[2] )				// end position (Z)
	write_short( iSprite )				// sprite index
	write_byte( count )					// count
	write_byte( life)					// life in 0.1's
	write_byte( scale)					// scale in 0.1's
	write_byte( velocity )				// velocity along vector in 10's
	write_byte( random )				// randomness of velocity in 10's
	message_end()
}

stock Create_StatusText(idUser, linenumber, text[])
{

	message_begin( MSG_ONE, gmsgStatusText, {0,0,0}, idUser );
	write_byte( linenumber )			// line number of status bar text
	write_string( text )				// status bar text
	message_end()
}

stock Create_BarTime(idUser, duration, flag){

	message_begin( MSG_ONE, gmsgBarTime, {0,0,0}, idUser )
	write_byte( duration ) // duration 
	write_byte( flag )
	message_end() 
}

stock Create_TE_BEAMFOLLOW(entity, iSprite, life, width, red, green, blue, alpha){

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
	write_byte( TE_BEAMFOLLOW )
	write_short( entity )			// entity
	write_short( iSprite )			// model
	write_byte( life )				// life
	write_byte( width )				// width
	write_byte( red )				// red
	write_byte( green )				// green
	write_byte( blue )				// blue
	write_byte( alpha )				// brightness
	message_end()
}

stock Create_TE_BEAMPOINTS(start[3], end[3], iSprite, startFrame, frameRate, life, width, noise, red, green, blue, alpha, speed){

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
	write_byte( TE_BEAMPOINTS )
	write_coord( start[0] )
	write_coord( start[1] )
	write_coord( start[2] )
	write_coord( end[0] )
	write_coord( end[1] )
	write_coord( end[2] )
	write_short( iSprite )			// model
	write_byte( startFrame )		// start frame
	write_byte( frameRate )			// framerate
	write_byte( life )				// life
	write_byte( width )				// width
	write_byte( noise )				// noise
	write_byte( red)				// red
	write_byte( green )				// green
	write_byte( blue )				// blue
	write_byte( alpha )				// brightness
	write_byte( speed )				// speed
	message_end()
}

stock Create_TE_BEAMENTS(startEntity, endEntity, iSprite, startFrame, frameRate, life, width, noise, red, green, blue, alpha, speed){

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
	write_byte( TE_BEAMENTS )
	write_short( startEntity )		// start entity
	write_short( endEntity )		// end entity
	write_short( iSprite )			// model
	write_byte( startFrame )		// starting frame
	write_byte( frameRate )			// frame rate
	write_byte( life )				// life
	write_byte( width )				// line width
	write_byte( noise )				// noise amplitude
	write_byte( red )				// red
	write_byte( green )				// green
	write_byte( blue )				// blue
	write_byte( alpha )				// brightness
	write_byte( speed )				// scroll speed
	message_end()
}

stock Create_TE_ELIGHT(entity, start[3], radius, red, green, blue, life, decayRate){

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
	write_byte( TE_ELIGHT )
	write_short( entity )			// entity
	write_coord( start[0] )			// initial position
	write_coord( start[1] )			// initial position
	write_coord( start[2] )			// initial position
	write_coord( radius )			// radius
	write_byte( red )				// red
	write_byte( green )				// green
	write_byte( blue )				// blue
	write_byte( life )				// life
	write_coord( decayRate )		// decay rate
	message_end()
}

stock Create_TE_SPRAY(position[3], direction[3], iSprite, count, speed, noise, rendermode){

	message_begin( MSG_BROADCAST,SVC_TEMPENTITY ) 
	write_byte( TE_SPRAY )
	write_coord( position[0] )		// Position
	write_coord( position[1] ) 
	write_coord( position[2] ) 
	write_coord( direction[0] )		// Direction 
	write_coord( direction[1] )  
	write_coord( direction[2] ) 
	write_short( iSprite )			// Sprite
	write_byte( count )				// count 
	write_byte( speed )				// speed 
	write_byte( noise )				// noise
	write_byte( rendermode )		// rendermode
	message_end() 
}

stock Create_TE_SPRITE(position[3], iSprite, scale, alpha){

	message_begin( MSG_BROADCAST,SVC_TEMPENTITY ) 
	write_byte( TE_SPRITE ) 
	write_coord( position[0] )		// position)
	write_coord( position[1] ) 
	write_coord( position[2] ) 
	write_short( iSprite )			// sprite index
	write_byte( scale)				// scale in 0.1's
	write_byte( alpha )				// brightness
	message_end() 
}

stock Create_StatusIcon(idUser, status, sprite[], red, green, blue)
{

	message_begin( MSG_ONE, gmsgStatusIcon, {0,0,0}, idUser ) 
	write_byte( status )			// status 
	write_string( sprite )			// sprite name 
	write_byte( red )				// red 
	write_byte( green )				// green 
	write_byte( blue )				// blue 
	message_end()
}

stock Create_TE_LARGEFUNNEL(position[3], iSprite, flags){

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )	
	write_byte( TE_LARGEFUNNEL )
	write_coord( position[0] )		// origin, x
	write_coord( position[1] )		// origin, y
	write_coord( position[2] )		// origin, z
	write_short( iSprite )			// sprite (0 for none)
	write_short( flags )			// 0 for collapsing, 1 for sending outward
	message_end() 
}

stock Create_TE_PLAYERATTACHMENT(idUser, entity, vOffset, iSprite, life){

	message_begin( MSG_ONE, SVC_TEMPENTITY, { 0, 0, 0 }, idUser )
	write_byte( TE_PLAYERATTACHMENT )
	write_byte( entity )			// entity
	write_coord( vOffset )			// vertical offset ( attachment origin.z = player origin.z + vertical offset )
	write_short( iSprite )			// model index
	write_short( life )				// (life * 10 )
	message_end()
}

stock Create_TE_TELEPORT(position[3]){

	message_begin( MSG_BROADCAST,SVC_TEMPENTITY) 
	write_byte( TE_TELEPORT ) 
	write_coord( position[0] ) 
	write_coord( position[1] ) 
	write_coord( position[2] ) 
	message_end()
}

stock Create_TE_LAVASPLASH(position[3]){

	message_begin( MSG_BROADCAST,SVC_TEMPENTITY) 
	write_byte( TE_LAVASPLASH ) 
	write_coord( position[0] ) 
	write_coord( position[1] ) 
	write_coord( position[2] ) 
	message_end()
}

stock Create_TE_TAREXPLOSION(position[3]){

	message_begin( MSG_BROADCAST,SVC_TEMPENTITY) 
	write_byte( TE_TAREXPLOSION ) 
	write_coord( position[0] ) 
	write_coord( position[1] ) 
	write_coord( position[2] ) 
	message_end()
}

stock Create_TE_BUBBLES(start[3], end[3], height, iSprite, count, speed){
/*			
	Example:

	g_sBubble = precache_model("sprites/bubble.spr")

	new origin[3], start[3], end[3]

	get_user_origin(victim, origin)

	start[0] = origin[0] - 64
	start[1] = origin[1] - 64
	start[2] = origin[2] - 64

	end[0] = origin[0] + 64
	end[1] = origin[1] + 64
	end[2] = origin[2] + 64

	Create_TE_BUBBLES(start, end, 96, g_sBubble, 100, 8)
*/

	message_begin( MSG_BROADCAST,SVC_TEMPENTITY) 
	write_byte( TE_BUBBLES )
	write_coord( start[0] )				// start position
	write_coord( start[1] ) 
	write_coord( start[2] ) 
	write_coord( end[0] )				// end position
	write_coord( end[1] ) 
	write_coord( end[2] ) 
	write_coord( height )				// float height
	write_short( iSprite )				// Sprite Index
	write_byte( count )					// count
	write_coord( speed )				// speed
	message_end()
}

//Äëÿ shopmenu4
stock Create_TE_BLOODSPRITE(Origin[3],SpriteIndex1,SpriteIndex2,Color,Scale )
{
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY );
	write_byte( TE_BLOODSPRITE );       // TE_BLOODSPRITE
	write_coord( Origin[0] );           // position (X)
	write_coord( Origin[1] );           // position (Y)
	write_coord( Origin[2] );           // position (Z)
	write_short( SpriteIndex1 );        // sprite1 index
	write_short( SpriteIndex2 );        // sprite2 index
	write_byte( Color );                // color
	write_byte( Scale );                // scale
	message_end();
}

stock Create_TE_WORLDDECAL(Origin[3],DecalIndex)
{
	message_begin(MSG_ALL,SVC_TEMPENTITY );
	write_byte( TE_WORLDDECAL );        // TE_WORLDDECAL
	write_coord( Origin[0] );           // decal position (X)
	write_coord( Origin[1] );           // decal position (Y)
	write_coord( Origin[2] );           // decal position (Z)
	write_byte( DecalIndex );           // texture index of precached decal texture name
	message_end();
}


stock Create_TE_BLOODSTREAM(Origin[3],Vector[3],Color,Speed )
{
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY );
	write_byte( TE_BLOODSTREAM );       // TE_BLOODSTREAM
	write_coord( Origin[0] );           // start position (X)
	write_coord( Origin[1] );           // start position (Y)
	write_coord( Origin[2] );           // start position (Z)
	write_coord( Vector[0] );           // spray vector (X)
	write_coord( Vector[1] );           // spray vector (Y)
	write_coord( Vector[2] );           // spray vector (Z)
	write_byte( Color );                // color
	write_byte( Speed );                // speed
	message_end();
}


stock Create_TE_DLIGHT(origin[3], radius, red, green, blue, life, decayRate )
{
	message_begin( MSG_BROADCAST, SVC_TEMPENTITY );
	write_byte( TE_DLIGHT );
	write_coord( origin[0] );
	write_coord( origin[1] );
	write_coord( origin[2] );
	write_byte( radius );
	write_byte( red );
	write_byte( green );
	write_byte( blue );
	write_byte( life );
	write_byte( decayRate );
	message_end();
}


stock Create_TE_FIREFIELD( origin[3], radius, iSprite, count, flags, duration )
{
	message_begin( MSG_BROADCAST,SVC_TEMPENTITY)
	write_byte(TE_FIREFIELD)
	write_coord(origin[0])
	write_coord(origin[1])
	write_coord(origin[2])
	write_short(radius)
	write_short(iSprite)
	write_byte(count)
	write_byte(flags)
	write_byte(duration)
	message_end()

// Flags
//#define TEFIRE_FLAG_ALLFLOAT        1        // All sprites will drift upwards as they animate
//#define TEFIRE_FLAG_SOMEFLOAT       2        // Some of the sprites will drift upwards. (50% chance)
//#define TEFIRE_FLAG_LOOP            4        // If set, sprite plays at 15 fps, otherwise plays at whatever rate stretches the animation over the sprite's duration.
//#define TEFIRE_FLAG_ALPHA           8        // If set, sprite is rendered alpha blended at 50% else, opaque
//#define TEFIRE_FLAG_PLANAR          16       // If set, all fire sprites have same initial Z instead of randomly filling a cube. 
} 