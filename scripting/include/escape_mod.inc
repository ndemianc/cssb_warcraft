/* G4U ESCAPE MODE - THIS PLUGIN CONTACT WITH escape_mod.amxx */

#if defined _escape_mod_included
  #endinput
#endif
#define _escape_mod_included

/* NATIVE */
/* Get the running state of this mod */
native g4u_escape_mod_running()
/* Get escaping team's score */
native g4u_escape_team_score()
/* Get max escape time */
native g4u_get_max_escape()
/* Get user's escape point */
native g4u_get_user_escape(id)
/* Make user reach escape zone */
native g4u_reach_escape_zone(id)
native Float:g4u_get_escape_roundtime()
/* FORWARD */
/* Called before player spawn */
forward g4u_user_spawn_pre(id)
/* Called after player spawn */
forward g4u_user_spawn_post(id)
/* Called before player escape */
forward g4u_user_escaped_pre(escaper)
/* Called after player escape */
forward g4u_user_escaped_post(escaper)
/* Called before player change his/her team information */
forward g4u_team_change_pre(id)
/* Called after player change his/her team information */
forward g4u_team_change_post(id)

forward g4u_escape_success()
forward g4u_no_escape()