#if defined _g4u_bag_included
  #endinput
#endif
#define _g4u_bag_included

enum
{
	EQUIP_FULL = 1,
	EQUIP_KNIFE,
	EQUIP_PISTOL,
	EQUIP_SNIPER
}
/* Get user account */
native g4u_get_user_account(id, account[], len)
/* Get user nick name */
native g4u_get_user_nickname(id, nickname[], len)
/* Get bag file */
native g4u_get_user_bag_file(id, file[], len)
/* Get user loged in status */
native g4u_get_user_loged_in(id)
/* Get user current bag */
native g4u_get_user_bag(id)
/* Update client's bag */
native g4u_update_my_bag(id)
/* Get equip type */
native g4u_get_equip_type()
/* Called when player successfully logged in */
forward g4u_user_loged_in(id, nickname[], bag_name[], info_file[])