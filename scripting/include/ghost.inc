/* This file contacts with zombiemod.amxx */
#if defined _ghost_included
  #endinput
#endif
#define _ghost_included

/* Check if user is a ghost */
native g4u_get_user_ghost(id)
/* Check if player is an alone soldier */
native g4u_get_alone_soldier(id)
/* Get state of ghost mode */
native g4u_ghost_toggle()
/* Get round of ghost mode */
native g4u_ghost_get_round()
/* Get ghost score */
native g4u_get_ghost_score()
/* Get human score */
native g4u_get_human_score()
/* Get ghost mode */
native g4u_get_ghost_mode()
/* Get Ghost toggle */
native g4u_get_ghost_toggle()

native Float:g4u_get_ghost_roundtime()

forward g4u_team_ghost_win()
forward g4u_team_human_win()
forward g4u_ghost_rounddraw()