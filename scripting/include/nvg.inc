/* G4U NVG */

#if defined _nvg_included
  #endinput
#endif
#define _nvg_included

/* Give NVG to an user */
native g4u_set_user_nvg(id, nvgid)
/* Get NVG id of an user */
native g4u_get_user_nvg(id)
/* Get NVG cost and cost type */
native g4u_get_nvg_cost(nvgid, cost, cost_type)
/* Get NVG status of an user */
native g4u_user_nvg_status(id)