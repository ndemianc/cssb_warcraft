
new szMessage[256];

stock cssbColoredPrint(target, const message[], any:...)
{
	static buffer[512], i, argscount;
	argscount = numargs();
	
	// Send to everyone
	if (!target)
	{
		static player;
		for (player = 1; player <= get_maxplayers(); player++)
		{
			// Not connected
			if (!is_user_connected(player))
				continue;
			
			// Remember changed arguments
			static changed[5], changedcount; // [5] = max LANG_PLAYER occurencies
			changedcount = 0;
			
			// Replace LANG_PLAYER with player id
			for (i = 2; i < argscount; i++)
			{
				if (getarg(i) == LANG_PLAYER)
				{
					setarg(i, 0, player);
					changed[changedcount] = i;
					changedcount++;
				}
			}
			
			// Format message for player
			vformat(buffer, charsmax(buffer), message, 3);
			
			// Send it
			message_begin(MSG_ONE_UNRELIABLE, get_user_msgid("SayText"), _, player);
			write_byte(player);
			write_string(buffer);
			message_end();
			
			// Replace back player id's with LANG_PLAYER
			for (i = 0; i < changedcount; i++)
				setarg(changed[i], 0, LANG_PLAYER);
		}
	}
	// Send to specific target
	else
	{
				
		// Format message for player
		vformat(buffer, charsmax(buffer), message, 3);
		
		// Send it
		message_begin(MSG_ONE, get_user_msgid("SayText"), _, target);
		write_byte(target);
		write_string(buffer);
		message_end();
	}
}

stock cssbChatColor(const id, const input[], any:...)
{
    new count = 1, players[32]
    static msg[191]
    vformat(msg, 190, input, 3)
    
    replace_all(msg, 190, "!g", "^4")
    replace_all(msg, 190, "!y", "^1")
    replace_all(msg, 190, "!t", "^3")
    
    if (id) players[0] = id; else get_players(players, count, "ch")
    {
        for (new i = 0; i < count; i++)
        {
            if (is_user_connected(players[i]))
            {
                message_begin(MSG_ONE_UNRELIABLE, get_user_msgid("SayText"), _, players[i]);
                write_byte(players[i]);
                write_string(msg);
                message_end();
            }
        }
    }
} 