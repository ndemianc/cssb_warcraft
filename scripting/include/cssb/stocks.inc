//Расчет отталкивания (отброс)
stock stSetKickBack( ent, Float:fOrigin[3], Float:fSpeed, Float:fVelocity[3])
{
	// Find origin
	new Float:fEntOrigin[3];
	entity_get_vector( ent, EV_VEC_origin, fEntOrigin );

	// Do some calculations
	new Float:fDistance[3];
	fDistance[0] = fEntOrigin[0] - fOrigin[0];
	fDistance[1] = fEntOrigin[1] - fOrigin[1];
	fDistance[2] = fEntOrigin[2] - fOrigin[2];
	new Float:fTime = (vector_distance( fEntOrigin,fOrigin ) / fSpeed);
	fVelocity[0] = fDistance[0] / fTime;
	fVelocity[1] = fDistance[1] / fTime;
	fVelocity[2] = fDistance[2] / fTime;

	return (fVelocity[0] && fVelocity[1] && fVelocity[2]);
}

//Считывание сущности (оружия)
stock stGetWeaponEntity(idUser,idWeapon=0,szWeaponName[]="")
{
	new szWeaponNameNew[24];

	if(idWeapon) 
		get_weaponname(idWeapon,szWeaponNameNew,charsmax(szWeaponNameNew));
	else 
		formatex(szWeaponNameNew,charsmax(szWeaponNameNew),"%s",szWeaponName);

	if(!equal(szWeaponNameNew,"weapon_",7))
		format(szWeaponNameNew,charsmax(szWeaponNameNew),"weapon_%s",szWeaponNameNew);

	return fm_find_ent_by_owner(get_maxplayers(),szWeaponNameNew,idUser);
} 