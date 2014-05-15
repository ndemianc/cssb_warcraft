/*
* Удаление нужного оружия у игрока
 
* @idUser		ID игрока
* @idWeapon 	ID оружия (CSW_AK47 и др.)
*/
public fRemoveGun(idUser,idWeapon)
{ 
	new szWeaponList[32] ;
	new iNumber;
	get_user_weapons(idUser,szWeaponList,iNumber);
	for (new i = 0; i < iNumber ;i++)
	{ 
		if (szWeaponList[i] == idWeapon)
		{
			fm_strip_user_gun(idUser, szWeaponList[i]);
		}
	}
}

//Надо доработать
/*
* Удаление автоматов у игрока
 
* @idUser		ID игрока
*/
public fRemoveGunRifles(idUser)
{ 
	new szWeaponList[32];
	new iNumber;
	get_user_weapons(idUser,szWeaponList,iNumber);
	for (new i = 0; i < iNumber ;i++)
	{ 
		if (!stIsSecondaryWeapons(szWeaponList[i]))
			fm_strip_user_gun(idUser,szWeaponList[i]);
	/*	
		if (stIsSecondaryWeapons(szWeaponList[i]) == true)
		{
			new szWeaponName[32];
			get_weaponname(szWeaponList[i],szWeaponName,charsmax(szWeaponName));
			fm_give_item(idUser,szWeaponName);
		}*/
			
	}
} 

// Проверка оружия на наличие пистолетов
stock stIsSecondaryWeapons(idWeapon)
{
	if ( idWeapon == CSW_ELITE || idWeapon == CSW_FIVESEVEN || idWeapon == CSW_USP || idWeapon == CSW_GLOCK18 || idWeapon == CSW_DEAGLE || idWeapon == CSW_P90 )
	{
		return true;
	}
	
	return false;
}
