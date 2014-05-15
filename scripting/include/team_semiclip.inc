/* Team Semiclip 1.8.8
*
* This file is provided as is (no warranties).
*/

#if defined _tsc_included
  #endinput
#endif
#define _tsc_included

/**
 * Set on player, special renders.
 * 
 * @param id		Player index. [1-32]
 * @param special	Special renders on/off. [0-1]
 * @param fx		look amxconst.inc -> Render for set_user_rendering [0-20]
 * @param r			Set red color. [0-255]
 * @param g			Set green color. [0-255]
 * @param b			Set blue color. [0-255]
 * @param render	look amxconst.inc -> Render for set_user_rendering [0-5]
 * @param amount	Set amount. [0-255]
 * @return			True if it is, false otherwise.
 */
native tsc_set_user_rendering(id, special = 0, fx = kRenderFxNone, r = 255, g = 255, b = 255, render = kRenderNormal, amount = 16)

