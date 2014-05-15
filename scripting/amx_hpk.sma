/* AMX Mod X script. 
* 
* This file is provided as is (no warranties).
*
* (c) 2002-2003, OLO 
* Modified by shadow
* Modified again by Bo0m!
* For use alongside with the AMX Super All-In-One Plugin. :)
*
*****************************************************************************
* Changelog:
*
*
* 6/15/07 - v0.16.2:
* - changed cvar
* amx_hpk_immunity -> Weather or not to have it use Immunity (X-olent)
*
* 4/28/07 - v0.17 (Bo0m!):
* - fixed check cvar never being used
* - fixed delay cvar check
* - switched to pcvars
* - adjusted some messages
* - added definable immunity flag
* - added logging when admins change HPK settings
*
*
* 4/19/04 - v0.16.2:
* - changed cvars
* amx_maxping -> amx_hpk_ping
* amx_maxping_check -> amx_hpk_check
* amx_maxping_tests -> amx_hpk_tests
*
* - added amx_command
* amx_hpk - displays status of the plugin and syntax to configure it
*
* - added cvar
* amx_hpk_delay - Delays ping checking after connect (default 1min, use amx_hpk to configure)
*
*
* 4/15/04 - v0.16.1:
* - added cvars
* amx_maxping //lowest average ping to be kicked
* amx_maxping_check //time between checks
* amx_maxping_tests //number of checks to be performed before kick
*
* - modified the version from 0.9.4 (OLO) to 0.16.1 (amxx)
*
* - added log entry for amxx logging
*
*
*****************************************************************************
* Original plugin: http://forums.alliedmods.net/showthread.php?p=10259
*
* This modified plugin can be found at:
* http://forums.alliedmods.net/forumdisplay.php?f=111
*/ 

#include <amxmodx> 
#include <amxmisc>

new const PLUGIN[]  = "High Ping Kicker"
new const VERSION[] = "1.0"
new const AUTHOR[]  = "Shadow/Bo0m!"

// Feel free to change this flag
#define HPK_IMMUNE ADMIN_IMMUNITY

// PCvars
new hpk_ping, hpk_check, hpk_tests, hpk_delay, hpk_immunity

new g_Ping[33]
new g_Samples[33]

public plugin_init() {

	register_plugin(PLUGIN, VERSION, AUTHOR)

	register_concmd("amx_hpk","cmdHpk",ADMIN_CVAR,"- configures high ping kicker")

	hpk_ping = register_cvar("amx_hpk_ping","150")
	hpk_check = register_cvar("amx_hpk_check","12")
	hpk_tests = register_cvar("amx_hpk_tests","5")
	hpk_delay = register_cvar("amx_hpk_delay","60")
	hpk_immunity = register_cvar("amx_hpk_immunity","1")

	if (get_pcvar_num(hpk_check) < 5) set_pcvar_num(hpk_check,5)
	if (get_pcvar_num(hpk_tests) < 3) set_pcvar_num(hpk_tests,3)
}

public client_disconnect(id) 
	remove_task(id)

public client_putinserver(id) {    
	g_Ping[id] = 0 
	g_Samples[id] = 0

	if ( !is_user_bot(id) ) 
	{
		new param[1]
		param[0] = id 
		set_task( 15.0 , "showWarn" , id , param , 1 )
    
		if (get_pcvar_num(hpk_delay) != 0) {
			set_task( float(get_pcvar_num(hpk_delay)), "taskSetting", id, param , 1)
		}
		else {	    
			set_task( float(get_pcvar_num(hpk_check)) , "checkPing" , id , param , 1 , "b" )
		}
	}
}

public showWarn(param[])
	client_print( param[0] ,print_chat,"[HPK] Players with ping higher than %d will be kicked!", get_cvar_num( "amx_hpk_ping" ) )

public taskSetting(param[]) {
	new name[32]
	get_user_name(param[0],name,31)
	set_task( float(get_pcvar_num(hpk_check)) , "checkPing" , param[0] , param , 1 , "b" )
}

kickPlayer(id) { 
	new name[32],authid[36]
	get_user_name(id,name,31)
	get_user_authid(id,authid,35)
	client_print(0,print_chat,"[HPK] Player %s disconnected due to high ping",name)
	server_cmd("kick #%d ^"Sorry but your ping is too high, try again later...^"",get_user_userid(id))
	log_amx("HPK: ^"%s<%d><%s>^" was kicked due high ping (Average Ping ^"%d^")", name,get_user_userid(id),authid,(g_Ping[id] / g_Samples[id]))
}

public checkPing(param[]) { 

	if (get_pcvar_num(hpk_tests) < 3)
		set_pcvar_num(hpk_tests,3)

	new id = param[ 0 ] 

	if ( get_user_flags(id) & HPK_IMMUNE && get_pcvar_num(hpk_immunity) == 1 ) {
		remove_task(id)
		client_print(id, print_chat, "[HPK] Ping checking disabled due to immunity...")
		return PLUGIN_CONTINUE
	}

	new ping, loss

	get_user_ping(id,ping,loss) 

	g_Ping[ id ] += ping
	++g_Samples[ id ]

	if ( (g_Samples[ id ] > get_pcvar_num(hpk_tests)) && (g_Ping[id] / g_Samples[id] > get_pcvar_num(hpk_ping))  )    
		kickPlayer(id)

	return PLUGIN_CONTINUE
}

  
public cmdHpk(id,level,cid) {
	if (!cmd_access(id,level,cid,1))
		return PLUGIN_HANDLED

	if (read_argc() < 6) {
		console_print(id,"Usage: amx_hpk <max ping> <total ping checks> <time between checks> <delay before starting checks> <1 to allow immunity|0 to disallow")
		console_print(id,"Current High Ping Kicker Settings:")
		console_print(id,"Max Ping: %d | Ping Checks: %d | Check Frequency: %d | Start Delay: %d | Immunity: %d",get_pcvar_num(hpk_ping),get_pcvar_num(hpk_tests),get_pcvar_num(hpk_check),get_pcvar_num(hpk_delay),get_pcvar_num(hpk_immunity))
		return PLUGIN_HANDLED
	}

	new name[32], authid[36]
	get_user_name(id,name,31)
	get_user_authid(id,authid,35)

	new ping_arg[5], check_arg[5], tests_arg[5], delay_arg[5], immune_arg[5]
	read_argv(1,ping_arg,4)
	read_argv(2,tests_arg,4)
	read_argv(3,check_arg,4)
	read_argv(4,delay_arg,4)
	read_argv(5,immune_arg,4)
  
	new ping = str_to_num(ping_arg)
	new tests = str_to_num(tests_arg)
	new check = str_to_num(check_arg)
	new delay = str_to_num(delay_arg)
	new immune = str_to_num(immune_arg)

	if ( check < 5 ) check = 5
	if ( tests < 3 ) tests = 3

	set_pcvar_num(hpk_ping,ping)
	set_pcvar_num(hpk_tests,tests)
	set_pcvar_num(hpk_check,check)
	set_pcvar_num(hpk_delay,delay)
	set_pcvar_num(hpk_immunity,immune)

	console_print(id,"The following HPK Settings have been set:")
	console_print(id,"Max Ping: %d | Ping Checks: %d | Check Frequency: %d | Start Delay: %d | Immunity: %d",get_pcvar_num(hpk_ping),get_pcvar_num(hpk_tests),get_pcvar_num(hpk_check),get_pcvar_num(hpk_delay),get_pcvar_num(hpk_immunity))
	log_amx("HPK: ^"%s<%d><%s>^" has configured the HPK - Max Ping: %d | Ping Checks: %d | Check Frequency: %d | Start Delay: %d | Immunity: %d", name,get_user_userid(id),authid,get_pcvar_num(hpk_ping),get_pcvar_num(hpk_tests),get_pcvar_num(hpk_check),get_pcvar_num(hpk_delay),get_pcvar_num(hpk_immunity))

	return PLUGIN_HANDLED    
}


