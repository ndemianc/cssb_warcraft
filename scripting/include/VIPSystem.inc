/* Access flags */
#define VIP_FLAG_ALL 0      // Any flag
#define VIP_FLAG_A (1<<0)   // Flag "a"
#define VIP_FLAG_B (1<<1)   // Flag "b"
#define VIP_FLAG_C (1<<2)   // Flag "c"
#define VIP_FLAG_D (1<<3)   // Flag "d"
#define VIP_FLAG_E (1<<4)   // Flag "e"
#define VIP_FLAG_F (1<<5)   // Flag "f"
#define VIP_FLAG_G (1<<6)   // Flag "g"
#define VIP_FLAG_H (1<<7)   // Flag "h"
#define VIP_FLAG_I (1<<8)   // Flag "i"
#define VIP_FLAG_J (1<<9)   // Flag "j"
#define VIP_FLAG_K (1<<10)  // Flag "k"
#define VIP_FLAG_L (1<<11)  // Flag "l"
#define VIP_FLAG_M (1<<12)  // Flag "m"
#define VIP_FLAG_N (1<<13)  // Flag "n"
#define VIP_FLAG_O (1<<14)  // Flag "o"
#define VIP_FLAG_P (1<<15)  // Flag "p"
#define VIP_FLAG_Q (1<<16)  // Flag "q"
#define VIP_FLAG_R (1<<17)  // Flag "r"
#define VIP_FLAG_S (1<<18)  // Flag "s"
#define VIP_FLAG_T (1<<19)  // Flag "t"
#define VIP_FLAG_U (1<<20)  // Flag "u"
#define VIP_FLAG_V (1<<21)  // Flag "v"
#define VIP_FLAG_W (1<<22)  // Flag "w"
#define VIP_FLAG_X (1<<23)  // Flag "x"
#define VIP_FLAG_Y (1<<24)  // Flag "y"
#define VIP_FLAG_Z (1<<25)  // Flag "z"

/* Returns 1 if player is VIP, otherwise 0 */
native VSGetUserVip(id)

/* Returns 1 if player have a flag, otherwise 0 */
native VSGetVipFlag(id, flag)

/* Returns flags the player as sum of bits */
native VSGetVipFlags(id)

/* Register new item
Parameters: 
	name - item name
	flag - access flag
Returns:
	id of item
*/
native VSRegisterItem(name[], flag)

/* Called when the player has chosen an element */
forward VSItemSelected(id, itemid)

/* Called when a VIP connects */
forward VSVipConnect(id)