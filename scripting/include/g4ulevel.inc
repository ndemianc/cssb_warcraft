/*================================================================================
	
	This file is contact with level_up.amxx
	
=================================================================================*/

#if defined _g4ulevel_included
  #endinput
#endif
#define _g4ulevel_included

/* Returns level of a player
Level return by integer
0 - Hoc vien 
1 - Binh bet 
2 - Binh nhi 
3- Binh nhat 
4 - Ha si 
5 - Trung si 
6- Thuong si
7 - Thieu uy
8 - Trung uy 
9 - Thuong uy
10 - Dai uy 
11 - Thieu ta 
12 - Trung ta 
13 - Thuong ta 
14- Dai ta
15 - Thieu tuong 
16- Trung tuong 
17- Thuong tuong
18 - Dai tuong
19 - Tong thong */
native g4u_get_level(id)
/* Returns total point of a player */
native g4u_get_total_point(id)
/* Returns defusing bomb point of a player */
native g4u_get_defuse_bomb_point(id)
/* Returns planting bomb point of a player */
native g4u_get_plant_bomb_point(id)
/* Returns knife killing point of a player */
native g4u_get_knife_point(id)
/* Returns headshot points of a player */
native g4u_get_headshot_point(id)
/* Returns infecting point of a player */
native g4u_get_infect_point(id)
/* Returns normal killing point of a player */
native g4u_get_normal_kill_point(id)
/* Returns zombie killing point of a player */
native g4u_get_zombie_point(id)
/* Forward call when a player is level up */
forward g4u_protomote(id, level)
/* Check ace status of a player */
native g4u_get_player_ace(id)
/* Get the name of level */
native g4u_get_level_name(levelid, output[], len)

