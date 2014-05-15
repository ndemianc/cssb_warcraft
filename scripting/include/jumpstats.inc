#if defined _jumpstats_included
	#endinput
#endif

#define _jumpstats_included


#include <jumpstats_const>
#include <jumpstats_stocks>


#if AMXX_VERSION_NUM >= 175
	#pragma reqlib jumpstats
	#if !defined AMXMODX_NOAUTOLOAD
		#pragma loadlib jumpstats
	#endif
#else
	#pragma library jumpstats
#endif


native bool:js_user_has_colorchat(client);
native bool:js_user_has_sounds(client);
native bool:js_user_has_jumpstats(client);
native bool:js_user_has_strafestats(client);
native bool:js_user_has_beam(client);
native bool:js_user_has_specstats(client);
native bool:js_user_has_prestrafe(client);
native bool:js_user_has_dirhud(client);
native bool:js_user_has_blockdist(client);
native bool:js_user_has_edgedist(client);

native js_check_user_top(client, jump_type, direction, Float:distance, Float:prestrafe, Float:maxspeed, strafes, sync);
native js_check_user_best(client, jump_type, direction, Float:distance, Float:prestrafe, Float:maxspeed, strafes, sync);

native js_get_top(jump_type, top_num, name[], len, &direction, &Float:distance, &Float:prestrafe, &Float:maxspeed, &strafes, &sync);
native js_get_top_num(jump_type, steamid[]);

native js_get_weaponflags();

forward js_update_cvars();

forward js_reset_jump(client, bool:func_door);

forward js_load_sounds(sound_index, sound_file[]);
