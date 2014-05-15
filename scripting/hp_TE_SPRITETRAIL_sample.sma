#include <amxmodx>
#include <amxmisc>
 
#define PLUGIN "[help plugin] TE_SPRITETRAIL sample"
#define VERSION "1.0"
#define AUTHOR "sdemian"

new cached_sprite;

public plugin_init() {
  register_plugin(PLUGIN, VERSION, AUTHOR);
  
  register_clcmd("say /sprite", "startsprite");
}

public plugin_precache() {
  cached_sprite = precache_model("sprites/test_sprite.spr");
}

public startsprite(idUser) {
  new vUserOrigin[3], vUserOffset[3];
  new offset_value = 100;
  get_user_origin( idUser, vUserOrigin );
  
  for (new i = 0; i <= 2; i++) {
    vUserOffset[i] = vUserOrigin[i] + offset_value;
  }
  Create_TE_SPRITETRAIL( vUserOrigin, vUserOffset, cached_sprite, 50, 15, 1, 2, 6 );
}

stock Create_TE_SPRITETRAIL(start[3], end[3], iSprite, count, life, scale, velocity, random ){

  message_begin( MSG_BROADCAST,SVC_TEMPENTITY)
  write_byte( TE_SPRITETRAIL )
  write_coord( start[0] )       // start position (X)
  write_coord( start[1] )       // start position (Y)
  write_coord( start[2] )       // start position (Z)
  write_coord( end[0] )       // end position (X)
  write_coord( end[1] )       // end position (Y)
  write_coord( end[2] )       // end position (Z)
  write_short( iSprite )        // sprite index
  write_byte( count )         // count
  write_byte( life)         // life in 0.1's
  write_byte( scale)          // scale in 0.1's
  write_byte( velocity )        // velocity along vector in 10's
  write_byte( random )        // randomness of velocity in 10's
  message_end()
}
