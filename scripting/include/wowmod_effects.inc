/** *************************************************************************
*** AMXX Plugin:   World of Warcraft Mod (WowMOD)
*** Plugin Author: K2mia
*** WowMOD Module: Effects
*** Date:          Sep 11 , 2007
*** Last Update:   Sep 27, 2007
*
*   World of Warcraft Mod continues in the tradition of UWC3 for CS 1.6, providing
*  a level-based RPG experience within the realm of the CS FPS. Many aspects are
*  borrowed from UWC3 (also by K2mia) including some skills and underlying themes.
*
*  Module: Main
*  Module Description:
*  This module provides the plugin initialization, client connection/disconnection
*  handling and other misc shared routines that are used throughout the mod.
*
*  Licensing Info:
*  You are free to use and modify this plugin to your liking with the following
*  restrictions:
*  a) You cannot altar the header/license/credit info in the sourcecode
*  b) You cannot altar/remove/replace the credits screen defined in the
*     wowmod_info_credits subroutine
*  c) You cannot altar/remove/replace the register_plugin() call in wowmod.sma
*----------------------------------------------------------------------------
*
*  WowMOD is written exclusively for AMX Mod X by K2mia
*  
****************************************************************************/

// **************************************************************************
// BEGIN WowMOD Effects Module 
// **************************************************************************


// **************************************************************************
// BEGIN wowmod_effect_bloodspray subroutine - Code adapted from Gore plugin 
// Handler for the TE_BLOODSPRITE effect
// **************************************************************************
public wowmod_effect_bloodspray( id, color ) {

   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new origin[3]
   get_user_origin( id, origin )

   message_begin( MSG_BROADCAST,SVC_TEMPENTITY )
   write_byte( TE_BLOODSPRITE )
   write_coord( origin[0] )
   write_coord( origin[1] )
   write_coord( origin[2] )
   write_short( blood1 )
   write_short( blood2 )
   write_byte( color )
   write_byte( 5 )
   message_end()

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_effect_burn subroutine - Code adapted from UWC3
// Handler for the burn effect
// **************************************************************************
public wowmod_effect_burn( id ) {

   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   new rx, ry, rz, forigin[3]

   if( !is_user_alive(id) )
      return PLUGIN_CONTINUE

   rx = random_num( -10, 10 )
   ry = random_num( -10, 10 )
   rz = random_num( -30, 30 )
   get_user_origin( id, forigin )

   //TE_SPRITE - additive sprite, plays 1 cycle
   message_begin( MSG_BROADCAST,SVC_TEMPENTITY)
   write_byte( 17 )
   write_coord(forigin[0]+rx) // coord, coord, coord (position)
   write_coord(forigin[1]+ry)
   write_coord(forigin[2]+10+rz)
   write_short( burning ) // short (sprite index)
   write_byte( 30 ) // byte (scale in 0.1's)
   write_byte( 200 ) // byte (brightness)
   message_end()

   //Smoke
   message_begin( MSG_BROADCAST,SVC_TEMPENTITY)
   write_byte( 5 )
   write_coord(forigin[0]+(rx*2)) // coord, coord, coord (position)
   write_coord(forigin[1]+(ry*2))
   write_coord(forigin[2]+100+(rz*2))
   write_short( g_sModelIndexSmoke )// short (sprite index)
   write_byte( 60 ) // byte (scale in 0.1's)
   write_byte( 15 ) // byte (framerate)
   message_end()


   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_effect_frostbolt subroutine 
// Handler for the frostbolt effect
// **************************************************************************
public wowmod_effect_frostbolt( id, targetid ) {

   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if( !is_user_alive(id) )
      return PLUGIN_CONTINUE

   new iorigin[3]
   new torigin[3]
   new vector[3]
   get_user_origin( id, iorigin )
   get_user_origin( targetid, torigin )

   vector[0] = torigin[0] - iorigin[0]
   vector[1] = torigin[1] - iorigin[1]
   vector[2] = torigin[2] - iorigin[2]
   
   wowmod_effect_beaments( id, targetid, frostbolt, COLOR_BLUE, FB_LINEWIDTH, 5 )
   wowmod_effect_elight( targetid, COLOR_BLUE, 10 )

   // Works, green exploding snow outwards
   //message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
   //write_byte( TE_LARGEFUNNEL )
   //write_coord( origin[0] )  // initial position
   //write_coord( origin[1] )  // initial position
   //write_coord( origin[2] )  // initial position
   //write_short( snow )
   //write_short( 1 ) 
   //message_end()

   // Works, lights up whole area around target, then fades
   //message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
   //write_byte( TE_DLIGHT )
   //write_coord( origin[0] )  // initial position
   //write_coord( origin[1] )  // initial position
   //write_coord( origin[2] )  // initial position
   //write_byte( 100  )
   //write_byte( 255  )
   //write_byte( 255 )
   //write_byte( 255 )
   //write_byte( 100 )
   //write_byte( 100 )
   //message_end()

   // Cylindrical explosion
   message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
   write_byte( TE_TELEPORT )
   write_coord( torigin[0] )
   write_coord( torigin[1] )
   write_coord( torigin[2] )
   message_end()

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_effect_attachsprite subroutine 
// Handler for the TE_ATTACHSPRITE effect
// **************************************************************************
public wowmod_effect_attachsprite( id, sprite, z ) {

   message_begin( MSG_ALL, SVC_TEMPENTITY, {0,0,0} )
   write_byte( TE_PLAYERATTACHMENT )
   write_byte( id )
   write_coord( z )       // z axis for sprite
   write_short( sprite )  // which sprite to display
   write_short( 25 )
   message_end()

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_effect_sprite subroutine 
// Handler for the TE_SPRITE effect
// **************************************************************************
public wowmod_effect_spellcast( origin[] ) {

   message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
   write_byte( TE_TELEPORT )
   write_coord( origin[0] )
   write_coord( origin[1] )
   write_coord( origin[2] )
   message_end()

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_effect_beamcyl subroutine 
// Handler for the TE_BEAMCYLINDER effect
// **************************************************************************
public wowmod_effect_beamcyl( origin[3], width, color ) {

   new cparm[3]
   if ( color == COLOR_BLUE ){
      cparm[0] = 0
      cparm[1] = 0
      cparm[2] = 255 
   }else if ( color == COLOR_RED ){
      cparm[0] = 255 
      cparm[1] = 0 
      cparm[2] = 0 
   }else if ( color == COLOR_WHITE ){
      cparm[0] = 255 
      cparm[1] = 255 
      cparm[2] = 255 
   }

   message_begin( MSG_PAS, SVC_TEMPENTITY, origin )
   write_byte( TE_BEAMCYLINDER )
   write_coord( origin[0])
   write_coord( origin[1])
   write_coord( origin[2] + 10)
   write_coord( origin[0])
   write_coord( origin[1])
   write_coord( origin[2] + 10 + BLINK_RADIUS)
   write_short( m_iSpriteTexture )
   write_byte( 0 ) // startframe
   write_byte( 0 ) // framerate
   write_byte( 3 ) // life
   write_byte( width )  // width
   write_byte( 0 ) // noise
   write_byte( cparm[0] )  // red
   write_byte( cparm[1] )  // green
   write_byte( cparm[2] )  // blue
   write_byte( 255 ) //brightness
   write_byte( 0 ) // speed
   message_end()

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_effect_sprite subroutine 
// Handler for the TE_SPRITE effect
// **************************************************************************
public wowmod_effect_sprite( target[], sprite ) {

   message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
   write_byte( TE_SPRITE )
   write_coord( target[0] )
   write_coord( target[1] )
   write_coord( target[2] )
   write_short( sprite )
   write_byte( 50 )
   write_byte( 150 )
   message_end()

   return PLUGIN_CONTINUE

}


// **************************************************************************
// BEGIN wowmod_effect_stsplash subroutine 
// Handler for the TE_STREAK_SPLASH effect
// **************************************************************************
public wowmod_effect_stsplash( id, targetid ) {

   if ( !wowmod_enabled )
      return PLUGIN_CONTINUE

   if( !is_user_alive(id) )
      return PLUGIN_CONTINUE

   new iorigin[3]
   new torigin[3]
   new vector[3]
   get_user_origin( id, iorigin )
   get_user_origin( targetid, torigin )

   vector[0] = torigin[0] - iorigin[0]
   vector[1] = torigin[1] - iorigin[1]
   vector[2] = torigin[2] - iorigin[2]

   message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
   write_byte( TE_STREAK_SPLASH )
   write_coord( iorigin[0] )  // initial position
   write_coord( iorigin[1] )  // initial position
   write_coord( iorigin[2] )  // initial position
   write_coord( vector[0] )  // initial position
   write_coord( vector[1] )  // initial position
   write_coord( vector[2] )  // initial position
   write_byte( 255  )
   write_short( 200 )
   write_short( 8 )
   write_short( 8 ) 
   message_end()

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_effect_elight subroutine 
// Handler for the TE_ELIGHT effect
// **************************************************************************
public wowmod_effect_elight( targetid, color, life ) {

   new origin[3]
   get_user_origin( targetid, origin )

   new cparm[3]
   if ( color == COLOR_BLUE ){
      cparm[0] = 0
      cparm[1] = 0
      cparm[2] = 255 
   }else if ( color == COLOR_RED ){
      cparm[0] = 255 
      cparm[1] = 0 
      cparm[2] = 0 
   }else if ( color == COLOR_WHITE ){
      cparm[0] = 255 
      cparm[1] = 255 
      cparm[2] = 255 
   }


   message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
   write_byte( TE_ELIGHT )
   write_short( targetid )   // entity
   write_coord( origin[0] )  // initial position
   write_coord( origin[1] )  // initial position
   write_coord( origin[2] )  // initial position
   write_coord( 200)         // radius
   write_byte( cparm[0] )    // r, g, b
   write_byte( cparm[1] )    // r, g, b
   write_byte( cparm[2] )    // r, g, b
   write_byte( life )        // life
   write_coord( 10 )          // decay rate
   message_end()


}


// **************************************************************************
// BEGIN wowmod_effect_dlight subroutine 
// Handler for the TE_DLIGHT effect
// **************************************************************************
public wowmod_effect_dlight( origin[3], radius, color, life ) {

   new cparm[3]
   if ( color == COLOR_BLUE ){
      cparm[0] = 0
      cparm[1] = 0
      cparm[2] = 255 
   }else if ( color == COLOR_WHITE ){
      cparm[0] = 255 
      cparm[1] = 255 
      cparm[2] = 255 
   }

   message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
   write_byte( TE_DLIGHT )
   write_coord( origin[0] )  // initial position
   write_coord( origin[1] )  // initial position
   write_coord( origin[2] )  // initial position
   write_byte( 25)         // radius
   write_byte( cparm[0] )    // r, g, b
   write_byte( cparm[1] )    // r, g, b
   write_byte( cparm[2] )    // r, g, b
   write_byte( life )        // life
   write_byte( 10 )          // decay rate
   message_end()

}


// **************************************************************************
// BEGIN wowmod_effect_pwshield subroutine 
// Handler for the power word: shield effect
// **************************************************************************
public wowmod_effect_pwshield( id ) {

   new decay = 1
   new origin[3]
   get_user_origin( id, origin )

   message_begin( MSG_ALL, SVC_TEMPENTITY, {0,0,0} )
   write_byte( TE_DLIGHT )
   write_coord( origin[0] )  // initial position
   write_coord( origin[1] )  // initial position
   write_coord( origin[2] )  // initial position
   write_byte( 10)           // radius
   write_byte( 0 )           // r, g, b
   write_byte( 0 )           // r, g, b
   write_byte( 255 )         // r, g, b
   write_byte( 100 )          // life
   write_byte( decay )       // decay rate
   message_end()

   message_begin( MSG_ALL, SVC_TEMPENTITY, {0,0,0} )
   write_byte( TE_ELIGHT )
   write_short( id )         // entity
   write_coord( origin[0] )  // initial position
   write_coord( origin[1] )  // initial position
   write_coord( origin[2] )  // initial position
   write_coord( 10 )         // radius
   write_byte( 0 )           // r, g, b
   write_byte( 0 )           // r, g, b
   write_byte( 255 )         // r, g, b
   write_byte( 100 )         // life
   write_coord( decay )      // decay rate
   message_end()

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_effect_perception subroutine 
// Handler for the perception effect
// **************************************************************************
public wowmod_effect_perception( parm[3] ) {
   // Light up enemy players

   new id = parm[0]
   new target = parm[1]
   new life = parm[2]
 
   new decay = 1
   new origin[3]
   get_user_origin( target, origin )

   //message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
   message_begin( MSG_ONE, SVC_TEMPENTITY, {0,0,0}, id )
   write_byte( TE_DLIGHT )
   write_coord( origin[0] )  // initial position
   write_coord( origin[1] )  // initial position
   write_coord( origin[2] )  // initial position
   write_byte( 10)            // radius
   write_byte( 255 )         // r, g, b
   write_byte( 0 )           // r, g, b
   write_byte( 0 )           // r, g, b
   write_byte( life )        // life
   write_byte( decay )       // decay rate
   message_end()

   //message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
   message_begin( MSG_ONE, SVC_TEMPENTITY, {0,0,0}, id )
   write_byte( TE_ELIGHT )
   write_short( target )     // entity
   write_coord( origin[0] )  // initial position
   write_coord( origin[1] )  // initial position
   write_coord( origin[2] )  // initial position
   write_coord( 10 )          // radius
   write_byte( 255 )         // r, g, b
   write_byte( 0 )           // r, g, b
   write_byte( 0 )           // r, g, b
   write_byte( life )        // life
   write_coord( decay )      // decay rate
   message_end()

   if ( has_perception[id] )
      set_task( 0.2, "wowmod_effect_perception", TASK_RACIAL_PERCEPTION+id, parm, 3 )

   return PLUGIN_CONTINUE
}



// **************************************************************************
// BEGIN wowmod_effect_beaments subroutine 
// Handler for the TE_BEAMENTS effect
// **************************************************************************
public wowmod_effect_beaments( id, targetid, sprite, color, width, life ) {

   new cparm[3]
   if ( color == COLOR_GREEN ){
      cparm[0] = 0
      cparm[1] = 255 
      cparm[2] = 0 
   }else if ( color == COLOR_BLUE ){
      cparm[0] = 0
      cparm[1] = 0
      cparm[2] = 255 
   }else if ( color == COLOR_GRAY ){
      cparm[0] = 0 
      cparm[1] = 150
      cparm[2] = 150 
   }

   message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
   write_byte( TE_BEAMENTS )
   write_short( id)        // start entity
   write_short( targetid)  // entity
   write_short( sprite )   // model
   write_byte( 0 )         // starting frame
   write_byte( 15 )        // frame rate
   write_byte( life )      // life
   write_byte( width )     // line width
   write_byte( 0 )         // noise amplitude
   write_byte( cparm[0] )  // r, g, b
   write_byte( cparm[1] )  // r, g, b
   write_byte( cparm[2] )  // r, g, b
   write_byte( 255 )       // brightness
   write_byte( 0 )         // scroll speed
   message_end()

}



// **************************************************************************
// BEGIN wowmod_sprite_druid subroutine 
// Handler for the druid class sprite
// **************************************************************************
public wowmod_sprite_class( parm[2] ) {

   new idself = parm[0]
   new id = parm[1]
   new class = p_class1[id] 
   new sprite

   switch ( class ){
      case CLASS_DRUID: sprite = class_druid
      case CLASS_HUNTER: sprite = class_hunter
      case CLASS_MAGE: sprite = class_mage
      case CLASS_ROGUE: sprite = class_rogue
      case CLASS_WARLOCK: sprite = class_warlock
      case CLASS_PRIEST: sprite = class_priest
      default: return PLUGIN_CONTINUE
   }

   //client_print( 0, print_chat, "Attaching sprite for class (%d) to %d", class, id )

   message_begin( MSG_ONE, SVC_TEMPENTITY, {0,0,0}, idself )
   write_byte( TE_PLAYERATTACHMENT )
   write_byte( id )
   write_coord( 30 )       // z axis for sprite
   write_short( sprite )  // which sprite to display
   write_short( 50 )
   message_end()

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_effect_glow subroutine 
// Set user rendering handler
// **************************************************************************
public wowmod_effect_glow( id, glow ) {

   if ( !wowmod_enabled || !is_user_connected( id ) )
      return PLUGIN_HANDLED

   new color[3]

   if ( isinvis[id] )
      return PLUGIN_HANDLED

   if ( glow == GLOW_RED || glow == GLOW_REDPULSE ){
      color[0] = 255
      color[1] = 0
      color[2] = 0
   }else if ( glow == GLOW_GREEN || glow == GLOW_GREENPULSE ){
      color[0] = 0 
      color[1] = 255 
      color[2] = 0
   }else if ( glow == GLOW_BLUE || glow == GLOW_BLUEPULSE ){
      color[0] = 0 
      color[1] = 0 
      color[2] = 255 
   }else if ( glow == GLOW_ORANGE || glow == GLOW_ORANGEPULSE ){
      color[0] = 255 
      color[1] = 69 
      color[2] = 0
   }else if ( glow == GLOW_PURPLE || glow == GLOW_PURPLEPULSE ){
      color[0] = 160 
      color[1] = 32 
      color[2] = 240 
   }else if ( glow == GLOW_YELLOW || glow == GLOW_YELLOWPULSE ){
      color[0] = 255 
      color[1] = 255 
      color[2] = 0 
   }else if ( glow == GLOW_WHITE ){
      color[0] = 255 
      color[1] = 255 
      color[2] = 255 
   }else if ( glow == GLOW_INVIS ){
      color[0] = 0 
      color[1] = 0  
      color[2] = 0 
   }else if ( glow == GLOW_NORMAL ){
      color[0] = 0 
      color[1] = 0  
      color[2] = 0 
   }

   new invis   // Used for setting invisibility level

   if ( ( glow >= GLOW_WHITE ) && ( glow <= GLOW_YELLOW ) ){
      set_user_rendering( id, kRenderFxGlowShell, color[0], color[1], color[2], kRenderNormal, 16)
   //}else if ( ( glow >= GLOW_REDPULSE ) && ( glow <= GLOW_YELLOWPULSE ) ){
   //   set_user_rendering( id, kRenderFxHologram, color[0], color[1], color[2], kRenderGlow, 16)
   }else if ( glow == GLOW_INVIS ){
      if ( has_shadow[id] && vanishon[id] ){
         invis = ALL_INVIS
         //client_print( id, print_chat, ">> All Invis On [%d]", invis )
      }else if ( vanishon[id] ){
         invis = VANISH_INVIS
         //client_print( id, print_chat, ">> Vanish Invis On [%d]", invis )
      }else if ( has_shadow[id] ){
         invis = ( ( SHADOWMELD_INVIS - p_level[id] ) > INVIS_MAX ) ? 
            ( SHADOWMELD_INVIS - p_level[id] ) : INVIS_MAX 
         //client_print( id, print_chat, ">> Shadowmeld Invis On [%d]", invis )
      }else{
         invis = BASE_VISIBILITY 
      }

      set_user_rendering( id, kRenderFxNone, color[0], color[1], color[2],  
         kRenderTransTexture , invis )
   }else if ( glow == GLOW_NORMAL ){
      if ( vanishon[id] ){
         invis = VANISH_INVIS
         //client_print( id, print_chat, ">> Vanish Invis Still On [%d]", invis )
         set_user_rendering( id, kRenderFxNone, color[0], color[1], color[2],  
            kRenderTransTexture , invis )
      }else if ( has_shadow[id] ){
         invis = ( ( SHADOWMELD_INVIS - p_level[id] ) > INVIS_MAX ) ? 
            ( SHADOWMELD_INVIS - p_level[id] ) : INVIS_MAX 
         //client_print( id, print_chat, ">> Shadowmeld Invis Still On [%d]", invis )
         set_user_rendering( id, kRenderFxNone, color[0], color[1], color[2],  
            kRenderTransTexture , invis )
      }else{
         //client_print( id, print_chat, ">> Invis Off" )
         set_user_rendering( id )
      }
   }

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_effect_glowoff subroutine 
// Turn off player glow
// **************************************************************************
public wowmod_effect_glowoff( parm[2] ) {
   new id = parm[0]

   if ( !wowmod_enabled || !is_user_connected( id ) )
      return PLUGIN_HANDLED

   set_user_rendering( id )

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_sound_playsound subroutine 
// Handler for playing specific sounds
// **************************************************************************
public wowmod_sound_playsound( id, sound ) {

   if ( sound == SOUND_HEAL ){
      if ( file_exists( "sound/wowmod/medshot5.wav" ) == 1 )
         emit_sound( id, CHAN_ITEM, "wowmod/medshot5.wav", 1.0, ATTN_NORM, 0, PITCH_NORM )
   }else if ( sound == SOUND_SPELLCAST ){
      if ( file_exists( "sound/wowmod/spellcast.wav" ) == 1 )
         emit_sound( id, CHAN_ITEM, "wowmod/spellcast.wav", 1.0, ATTN_NORM, 0, PITCH_NORM )
   }else if ( sound == SOUND_TUPING ){
      if ( file_exists( "sound/wowmod/searching.wav" ) == 1 )
         emit_sound( id, CHAN_ITEM, "wowmod/searching.wav", 1.0, ATTN_NORM, 0, PITCH_NORM )
   }else if ( sound == SOUND_LEVELUP ){
      if ( file_exists( "sound/wowmod/levelupcaster.wav" ) == 1 )
         emit_sound( id, CHAN_ITEM, "wowmod/levelupcaster.wav", 1.0, ATTN_NORM, 0, PITCH_NORM )
   }else if ( sound == SOUND_LEVBELL ){
      if ( file_exists( "sound/plats/elevbell1.wav" ) == 1 )
         emit_sound( id, CHAN_ITEM, "plats/elevbell1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM )
   }else if ( sound == SOUND_WRATH ){
      if ( file_exists( "sound/wowmod/banishcaster.wav" ) == 1 )
         emit_sound( id, CHAN_ITEM, "wowmod/banishcaster.wav", 1.0, ATTN_NORM, 0, PITCH_NORM )
   }else if ( sound == SOUND_ROOTS ){
      if ( file_exists( "sound/wowmod/entanglingrootstarget1.wav" ) == 1 )
         emit_sound( id, CHAN_ITEM, "wowmod/entanglingrootstarget1.wav", 1.0, ATTN_NORM, 0, 
            PITCH_NORM )
   }else if ( sound == SOUND_BIGGUN ){
      if ( file_exists( "sound/wowmod/biggun2.wav" ) == 1 )
         emit_sound( id, CHAN_ITEM, "wowmod/biggun2.wav", 1.0, ATTN_NORM, 0, PITCH_NORM )
   }else if ( sound == SOUND_FLAMESTRIKE ){
      if ( file_exists( "sound/wowmod/flamestrike.wav" ) == 1 )
         emit_sound( id, CHAN_ITEM, "wowmod/flamestrike.wav", 1.0, ATTN_NORM, 0, PITCH_NORM )
   }else if ( sound == SOUND_BLIZZARD ){
      if ( file_exists( "sound/wowmod/blizzard.wav" ) == 1 )
         emit_sound( id, CHAN_ITEM, "wowmod/blizzard.wav", 1.0, ATTN_NORM, 0, PITCH_NORM )
   }else if ( sound == SOUND_BLINK ){
      if ( file_exists( "sound/wowmod/blinkarrival.wav" ) == 1 )
         emit_sound( id, CHAN_ITEM, "wowmod/blinkarrival.wav", 1.0, ATTN_NORM, 0, PITCH_NORM )
   }else if ( sound == SOUND_BERZERK ){
      if ( file_exists( "sound/wowmod/berzerk4.wav" ) == 1 ){
         emit_sound( id, CHAN_ITEM, "wowmod/berzerk4.wav", 1.0, ATTN_NORM, 0, PITCH_NORM )
         //client_print( id, print_chat, "playsound> playing sound berzerk4.wav" )
      }
   }else if ( sound == SOUND_HEARTHSTONE ){
      if ( file_exists( "sound/wowmod/massteleporttarget.wav" ) == 1 ){
         emit_sound( id, CHAN_ITEM, "wowmod/massteleporttarget.wav", 1.0, ATTN_NORM, 0, PITCH_NORM )
      }
   }
   

   return PLUGIN_CONTINUE
}


// **************************************************************************
// BEGIN wowmod_sound_playsound_allnear subroutine 
// Handler for playing specific sounds to all nearby players
// **************************************************************************
public wowmod_sound_playsound_allnear( id, sound ) {
   new origin[3]
   new torigin[3]
   new player[32]
   new numplayers
   new tid
 
   get_user_origin( id, origin )
   get_players( player, numplayers )

   for ( new i = 0; i < numplayers; ++i ){
      tid = player[i]
      get_user_origin( tid, torigin )

      if ( is_user_alive(tid) && ( get_distance( origin, torigin ) <= get_cvar_num("FT_xp_radius") ) ){
         //client_print( 0, print_chat, "playsound_nearby> playing sound %d to %d", sound, tid )
         wowmod_sound_playsound( tid, sound )
      }
   }

   return PLUGIN_CONTINUE
}

// **************************************************************************
// END WowMOD Effects Module 
// **************************************************************************


// **************************************************************************
// END World of Warcraft Mod WowMOD Module 
// **************************************************************************

