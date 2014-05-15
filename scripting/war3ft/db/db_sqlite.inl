/*
*	SQLite
*/

#define TOTAL_SQLITE_TABLES		5

new const szTablesSQLite[TOTAL_SQLITE_TABLES][] = 
{
	"CREATE TABLE `wc3_player` ( `player_id` INTEGER PRIMARY KEY AUTOINCREMENT, `player_steamid` varchar(25) NOT NULL default '', `player_ip` varchar(20) NOT NULL default '', `player_name` varchar(35) NOT NULL default '', `time` timestamp(14) NOT NULL DEFAULT CURRENT_TIMESTAMP );",
	"CREATE TABLE `wc3_player_race` ( `player_id` INT NOT NULL default '0', `race_id` tinyint(4) NOT NULL default '0', `race_xp` INT default NULL, PRIMARY KEY  (`player_id`,`race_id`) );",
	"CREATE TABLE `wc3_player_skill` ( `player_id` INT NOT NULL default '0', `skill_id` tinyint(4) NOT NULL default '0', `skill_level` tinyint(4) NOT NULL default '0', PRIMARY KEY  (`player_id`,`skill_id`) );",
	//Для talisman +
	"CREATE TABLE `wc3_player_talisman` ( `player_id` INT NOT NULL default '0', `talisman_id` tinyint(4) NOT NULL default '0',PRIMARY KEY  (`player_id`,`talisman_id`) );",
	"CREATE TABLE `wc3_num_talisman` ( `player_id` INT NOT NULL default '0', `talisman_num` tinyint(4) NOT NULL default '0',PRIMARY KEY  (`player_id`,`talisman_num`) );"
};

new const szTableNames[TOTAL_SQLITE_TABLES][] = 
{
	"wc3_player",
	"wc3_player_race",
	"wc3_player_skill",
	"wc3_player_talisman",	//Для talisman +
	"wc3_num_talisman"		//Для talisman +
};

// Initiate the connection to the SQLite database
SQLITE_Init()
{
	new szError[256], iErrNum;

	// Set up the tuple that will be used for threading
	g_DBTuple = SQL_MakeDbTuple( "", "", "", "war3ft" );

	// Attempt to connect
	g_DBConn = SQL_Connect( g_DBTuple, iErrNum, szError, 255 );

	if ( !g_DBConn )
	{
		WC3_Log( true, "[SQLITE] Database Connection Failed: [%d] %s", iErrNum, szError );

		return;
	}


	server_print( "[WAR3FT] SQLite database connection successful" );

	new Handle:query;

	// Create the default tables if we need to
	for ( new i = 0; i < TOTAL_SQLITE_TABLES; i++ )
	{
		query = SQL_PrepareQuery( g_DBConn, szTablesSQLite[i] );

		if ( !sqlite_TableExists( g_DBConn, szTableNames[i] ) )
		{
			if ( !SQL_Execute( query ) )
			{
				SQLITE_Error( query, szTablesSQLite[i], 1 );

				return;
			}
		}

		SQL_FreeHandle( query );
	}

	/*
		These probably should be subject to a CVAR
		Lets fine tune the database:
			"synchronous = NORMAL"	- Put back the 2.x behaviour (faster than the defalt
						  for 3.x)
			"synchronous = OFF"	- Way faster, but it might get corrupted data if a
						  server os system crash occurs
			"integrity_check"	- well it's what it says, we do have to check the
						  value it returns since it's important
		PRAGMA commands don't return anything so no need to check the result of the query
	*/	

	query = SQL_PrepareQuery( g_DBConn, "PRAGMA integrity_check" );
	
	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, "PRAGMA integrity_check", 2 );

		return;
	}
	
	// Get the integrity check value
	new szIntegrityCheck[64];
	if ( SQL_NumResults( query ) > 0 )
	{
		SQL_ReadResult( query, 0, szIntegrityCheck, 63 )
	}

	// Free the result
	SQL_FreeHandle( query );

	// Check to make sure the integrity check passed
	if ( !equali(szIntegrityCheck, "ok") )
	{
		// Should we disable saving here?
		WC3_Log( true, "[SQLITE] SQL Lite integrity check failed, disabling saving XP." );

		set_pcvar_num( CVAR_wc3_save_xp, 0 );

		return;
	}
	
	// Do some synchronous crap
	new szQuery[128];
	format( szQuery, 127, "PRAGMA synchronous = %d", SQLITE_SYNC_OFF );
	query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, szQuery, 3 );

		return;
	}

	bDBAvailable = true;
}

// Close the SQLite connection
SQLITE_Close()
{
	if ( g_DBTuple )
	{
		SQL_FreeHandle( g_DBTuple );
	}

	if ( g_DBConn )
	{
		SQL_FreeHandle( g_DBConn );
	}
}

SQLITE_FetchUniqueID( idUser)
{
	// Make sure our connection is working
	if ( !SQLITE_Connection_Available() )
	{
		return;
	}

	// Remember how we got this idUser
	g_iDBPlayerSavedBy[idUser] = get_pcvar_num( CVAR_wc3_save_by );

	new szKey[66], szKeyName[32];
	DB_GetKey( idUser, szKey, 65 );
	DB_GetKeyName( szKeyName, 31 );

	new szQuery[512];
	format( szQuery, 511, "SELECT `player_id` FROM `wc3_player` WHERE `%s` = '%s';", szKeyName, szKey );

	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, szQuery, 4 );

		return;
	}

	// If no rows we need to insert!
	if ( SQL_NumResults( query ) == 0 )
	{
		// Free the last handle!
		SQL_FreeHandle( query );

		// Insert this player!
		new szQuery[512];
		format( szQuery, 511, "INSERT INTO `wc3_player` ( `player_id` , `%s` , `time` ) VALUES ( NULL , '%s', julianday('now') );", szKeyName, szKey );
		new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			SQLITE_Error( query, szQuery, 5 );

			return;
		}

		g_iDBPlayerUniqueID[idUser] = SQL_GetInsertId( query );
	}

	// They have been here before - store their idUser
	else
	{
		g_iDBPlayerUniqueID[idUser] = SQL_ReadResult( query, 0 );
	}

	// Free the last handle!
	SQL_FreeHandle( query );
}

SQLITE_Save( idUser )
{
	// Make sure our connection is working
	if ( !SQLITE_Connection_Available() )
	{
		return;
	}

	new iUniqueID = DB_GetUniqueID( idUser );

	// Save the user's XP!
	new szQuery[512];
	format( szQuery, 511, "REPLACE INTO `wc3_player_race` ( `player_id` , `race_id` , `race_xp` ) VALUES ( '%d', '%d', '%d');", iUniqueID, p_data[idUser][P_RACE], p_data[idUser][P_XP] );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, szQuery, 6 );

		return;
	}

	static iCurrentLevel;

	// Only save skill levels if the user does NOT play chameleon
	if ( p_data[idUser][P_RACE] != RACE_CHAMELEON )
	{
		// Now we need to save the skill levels!
		for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
		{
			if ( g_SkillType[iSkillID] != SKILL_TYPE_PASSIVE )
			{
				iCurrentLevel = SM_GetSkillLevel( idUser, iSkillID, 16 );
	
				// Then we need to save this!
				if ( iCurrentLevel >= 0 && g_iDBPlayerSkillStore[idUser][iSkillID] != iCurrentLevel )
				{
					g_iDBPlayerSkillStore[idUser][iSkillID] = iCurrentLevel;
					format( szQuery, 511, "REPLACE INTO `wc3_player_skill` ( `player_id` , `skill_id` , `skill_level` ) VALUES ( '%d', '%d', '%d' );", iUniqueID, iSkillID, iCurrentLevel );
					query = SQL_PrepareQuery( g_DBConn, szQuery );
	
					if ( !SQL_Execute( query ) )
					{
						SQLITE_Error( query, szQuery, 7 );
	
						return;
					}
				}
			}
		}
	}

	//Для talisman +
	if(MAX_MODE_TALISMAN == 1)
		fTalisman_SQLITE_Save( idUser,iUniqueID);
	
	return;
}

SQLITE_Save_T( idUser )
{

	//SQLITE_Save( idUser ); //Была расскоментирована строка и закоментирован весь нижний код функции
	
	// Make sure our connection is working
	if ( !SQLITE_Connection_Available() )
	{
		return;
	}

	new iUniqueID = DB_GetUniqueID( idUser );

	// Save the user's XP!
	new szQuery[512];
	format( szQuery, 511, "REPLACE INTO `wc3_player_race` ( `player_id` , `race_id` , `race_xp` ) VALUES ( '%d', '%d', '%d');", iUniqueID, p_data[idUser][P_RACE], p_data[idUser][P_XP] );
	SQL_ThreadQuery( g_DBTuple, "_SQLITE_Save_T", szQuery );

	static iCurrentLevel;

	// Only save skill levels if the user does NOT play chameleon
	if ( p_data[idUser][P_RACE] != RACE_CHAMELEON )
	{
		// Now we need to save the skill levels!
		for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
		{
			if ( g_SkillType[iSkillID] != SKILL_TYPE_PASSIVE )
			{
				iCurrentLevel = SM_GetSkillLevel( idUser, iSkillID, 17 );
	
				// Then we need to save this!
				if ( iCurrentLevel >= 0 && g_iDBPlayerSkillStore[idUser][iSkillID] != iCurrentLevel )
				{
					g_iDBPlayerSkillStore[idUser][iSkillID] = iCurrentLevel;
					format( szQuery, 511, "REPLACE INTO `wc3_player_skill` ( `player_id` , `skill_id` , `skill_level` ) VALUES ( '%d', '%d', '%d' );", iUniqueID, iSkillID, iCurrentLevel );
					SQL_ThreadQuery( g_DBTuple, "_SQLITE_Save_T", szQuery );
				}
			}
		}
	}
	
	//Для talisman +
	if(MAX_MODE_TALISMAN == 1)
		fTalisman_SQLITE_Save_T( idUser,iUniqueID );
	
	return;
}

public _SQLITE_Save_T( failstate, Handle:query, error[], errnum, data[], size )
{

	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		SQL_GetQueryString( query, szQuery, 255 );
		
		SQLITE_ThreadError( query, szQuery, error, errnum, failstate, 1 );
	}
}

SQLITE_GetAllXP( idUser)
{
	// Make sure our connection is working
	if ( !SQLITE_Connection_Available() )
	{
		return;
	}

	new iUniqueID = DB_GetUniqueID( idUser );

	// Then we have a problem and cannot retreive the user's XP
	if ( iUniqueID <= 0 )
	{
		format(szMessage, sizeof(szMessage) - 1, "^x01%L^x03%L^x01%L ^x04%L",
				LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT",
				LANG_PLAYER,"CLIENT_PRINT_MYSQL_UN_RET_XP");	
			
		cssbColoredPrint(idUser, szMessage);		

		WC3_Log( true, "[ERROR] Unable to retreive user's Unique idUser" );

		return;
	}

	new szQuery[256];
	format(szQuery, 255, "SELECT `race_id`, `race_xp` FROM `wc3_player_race` WHERE ( `player_id` = '%d' );", iUniqueID );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, szQuery, 8 );

		return;
	}

	// Set last saved XP to 0
	for ( new i = 0; i < MAX_RACES; i++ )
	{
		g_iDBPlayerXPInfoStore[idUser][i] = 0;
	}

	new iXP, iRace;

	// Loop through all of the records to find the XP data
	while ( SQL_MoreResults( query ) )
	{
		iRace	= SQL_ReadResult( query, 0 );
		iXP		= SQL_ReadResult( query, 1 );
		
		// Save the user's XP in an array
		if ( iRace > 0 && iRace < MAX_RACES + 1 )
		{
			g_iDBPlayerXPInfoStore[idUser][iRace-1] = iXP;
		}

		SQL_NextRow( query );
	}

	// Free the handle
	SQL_FreeHandle( query );

	// Call the function that will display the "select a race" menu
	WC3_ChangeRaceShowMenu( idUser, g_iDBPlayerXPInfoStore[idUser]);

	return;
}

SQLITE_SetDataForRace( idUser )
{
	// Make sure our connection is working
	if ( !SQLITE_Connection_Available() )
	{
		return;
	}

	new szQuery[256];
	format( szQuery, 255, "SELECT `skill_id`, `skill_level` FROM `wc3_player_skill` WHERE `player_id` = '%d';", DB_GetUniqueID( idUser ) );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, szQuery, 9 );

		return;
	}

	//client_print(0,print_chat,"p_data[idUser][P_RACE]-1 : %d",p_data[idUser][P_RACE]-1);
	p_data[idUser][P_XP] = g_iDBPlayerXPInfoStore[idUser][p_data[idUser][P_RACE]-1];

	// Reset all skill data to 0!
	for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
	{
		if ( g_SkillType[iSkillID] != SKILL_TYPE_PASSIVE )
		{
			SM_SetSkillLevel( idUser, iSkillID, 0, 4 );
		}
	}

	new iSkillID, iSkillLevel;
	// While we have a result!
	while ( SQL_MoreResults( query ) )
	{
		iSkillID = SQL_ReadResult( query, 0 );
		iSkillLevel = SQL_ReadResult( query, 1 );
		SM_SetSkillLevel( idUser, iSkillID, iSkillLevel, 5 );
		g_iDBPlayerSkillStore[idUser][iSkillID] = iSkillLevel;

		SQL_NextRow( query );
	}

	// Free the handle
	SQL_FreeHandle( query );
	
	// Set the race up
	WC3_SetRaceUp( idUser );

	// This user's XP has been retrieved! We can save now
	bDBXPRetrieved[idUser] = true;


	return;
}

/*SQLITE_SetDataForRace_T( idUser )
{
	// Make sure our connection is working
	if ( !SQLITE_Connection_Available() )
	{
		return;
	}

	new szQuery[256], data[1];
	format( szQuery, 255, "SELECT `skill_id`, `skill_level` FROM `wc3_player_skill` WHERE `player_id` = '%d';", DB_GetUniqueID( idUser ) );

	data[0] = idUser;

	SQL_ThreadQuery( g_DBTuple, "_SQLITE_SetDataForRace", szQuery, data, 1 );

	return;
}

// Callback function once SQLITE Thread has completed
public _SQLITE_SetDataForRace_T( failstate, Handle:query, error[], errnum, data[], size )
{
	new idUser = data[0];

	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		SQL_GetQueryString( query, szQuery, 255 );
		
		SQLITE_ThreadError( query, szQuery, error, errnum, failstate, 3 );
	}

	// Query successful, we can do stuff!
	else
	{
		// Set the user's XP!
		if ( !SHARED_ValidPlayer( idUser ) || !p_data_b[idUser][PB_ISCONNECTED] || p_data[idUser][P_RACE] <=0 || p_data[idUser][P_RACE] > MAX_RACES )
		{
			WC3_Log( true, "[ERROR] WTF MATE?!? %d", idUser );

			return;
		}

		p_data[idUser][P_XP] = g_iDBPlayerXPInfoStore[idUser][p_data[idUser][P_RACE]-1];

		// Reset all skill data to 0!
		for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
		{
			if ( g_SkillType[iSkillID] != SKILL_TYPE_PASSIVE )
			{
				SM_SetSkillLevel( idUser, iSkillID, 0, 4 );
			}
		}

		// While we have a result!
		while ( SQL_MoreResults( query ) )
		{
			SM_SetSkillLevel( idUser, SQL_ReadResult( query, 0 ), SQL_ReadResult( query, 1 ), 5 );
			
			SQL_NextRow( query );
		}

		// Free the handle
		SQL_FreeHandle( query );
		
		// Set the race up
		WC3_SetRaceUp( idUser );

		// This user's XP has been retrieved! We can save now
		bDBXPRetrieved[idUser] = true;
	}

	return;
}*/

// Verifies that the database connection is ok
bool:SQLITE_Connection_Available()
{
	if ( !bDBAvailable )
	{
		return false;
	}

	return true;
}

#define SQLITE_TOTAL_PRUNE_QUERY 2

SQLITE_Prune()
{
	new const szPruneQuery[SQLITE_TOTAL_PRUNE_QUERY][] = 
	{
		"DELETE FROM wc3_player_race  WHERE player_id IN ( SELECT `player_id` FROM `wc3_player` WHERE ( (julianday(`time`) + %d) < julianday('now') ) );",
		"DELETE FROM wc3_player_skill WHERE player_id IN ( SELECT `player_id` FROM `wc3_player` WHERE ( (julianday(`time`) + %d) < julianday('now') ) );"
	};

	new szQuery[256];

	// Need to run all 3 queries
	for ( new i = 0; i < SQLITE_TOTAL_PRUNE_QUERY; i++ )
	{
		formatex( szQuery, 255, szPruneQuery[i], get_pcvar_num( CVAR_wc3_days_before_delete ) );

		new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			SQLITE_Error( query, szQuery, 10 );

			return;
		}
	}
}

SQLITE_UpdateTimestamp( idUser )
{
	// Make sure our connection is working
	if ( !SQLITE_Connection_Available() )
	{
		return;
	}

	new szKey[66];
	DB_GetKey( idUser, szKey, 65 );

	new szQuery[256];
	format( szQuery, 255, "UPDATE `wc3_player` SET time = julianday('now') WHERE ( `player_id` = '%d' )", g_iDBPlayerUniqueID[idUser] );

	SQL_ThreadQuery( g_DBTuple, "_SQLITE_UpdateTimestamp", szQuery );	
}

public _SQLITE_UpdateTimestamp( failstate, Handle:query, error[], errnum, data[], size )
{
	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		SQL_GetQueryString( query, szQuery, 255 );
		
		SQLITE_ThreadError( query, szQuery, error, errnum, failstate, 4 );
	}

	// Query successful, we can do stuff!
	else
	{
		// Free the handle
		SQL_FreeHandle( query );
	}

	return;
}

// The idUser should be a unique number, so we know what function called it (useful for debugging)
SQLITE_Error( Handle:query, szQuery[], idFunction )
{
	new szError[256];
	new iErrNum = SQL_QueryError( query, szError, 255 );

	WC3_Log( true, "[SQLITE] Error in querying database, location: %d", idFunction );
	WC3_Log( true, "[SQLITE] Message: %s (%d)", szError, iErrNum );
	WC3_Log( true, "[SQLITE] Query statement: %s ", szQuery );

	// Free the handle
	SQL_FreeHandle( query );
}

SQLITE_ThreadError( Handle:query, szQuery[], szError[], iErrNum, failstate, idUser )
{
	WC3_Log( true, "[SQLITE] Threaded query error, location: %d", idUser );
	WC3_Log( true, "[SQLITE] Message: %s (%d)", szError, iErrNum );
	WC3_Log( true, "[SQLITE] Query statement: %s ", szQuery );

	// Connection failed
	if ( failstate == TQUERY_CONNECT_FAILED )
	{	
		WC3_Log( true, "[SQLITE] Fail state: Connection Failed" );
	}

	// Query failed
	else if ( failstate == TQUERY_QUERY_FAILED )
	{
		WC3_Log( true, "[SQLITE] Fail state: Query Failed" );
	}

	// Free the handle
	SQL_FreeHandle( query );
}


