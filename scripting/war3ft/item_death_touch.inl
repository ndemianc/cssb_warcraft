


// Draw the death touch
public fDrawDeathTouchExplode( parm[5] )
{
	//new idUser = parm[0];

	new vOrigin[3], vPosition[3];
	vOrigin[0] = parm[2];
	vOrigin[1] = parm[3];
	vOrigin[2] = parm[4];

	vPosition[0] = vOrigin[0] + random_num( -100, 100 );
	vPosition[1] = vOrigin[1] + random_num( -100, 100 );
	vPosition[2] = vOrigin[2] + random_num( -50, 50 );

	Create_TE_EXPLOSION(  vOrigin, vPosition, g_iSprites[SPR_DEATH_TOUCH], (random_num(0,20) + 20), 12, 4 );
	Create_TE_Smoke( vOrigin, vPosition, g_iSprites[SPR_SMOKE], 60, 10 );
		
}

