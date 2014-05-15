
//Ускорение таймера если есть предмет ITEM_ULTIMATE_TIMER_DIVIDER
fResetUltimateTimerDivider( idUser, iDecrement = 0,iHideIcon = true )
{
//	if (!is_user_alive(idUser)) 
//		return;

	// Reset the user's ultimate
	fTimeCoolDown = float(get_pcvar_num( CVAR_wc3_ult_cooldown ));
//	if ( ITEM_Has( idUser, ITEM_ULTIMATE_TIMER_DIVIDER ) > ITEM_NONE )//Сброс таймера(деление счетчика сразу на RESET_ULTIMATE_TIMER)
//		ULT_ResetCooldown( idUser, floatround( (fTimeCoolDown + iDecrement) / RESET_ULTIMATE_TIMER),iHideIcon);
//	else
		ULT_ResetCooldown( idUser, floatround( (fTimeCoolDown + iDecrement) ),iHideIcon);

}

fSetUltimateDelay(idUser)
{
//	if (!is_user_alive(idUser)) 
//		return;

	fTimeCoolDown = float(get_pcvar_num( CVAR_wc3_ult_delay ));
//	if ( ITEM_Has( idUser, ITEM_ULTIMATE_TIMER_DIVIDER ) > ITEM_NONE )//Запуск таймера(деление счетчика сразу на RESET_ULTIMATE_TIMER)
//		p_data[idUser][P_ULTIMATEDELAY] = floatround( fTimeCoolDown / RESET_ULTIMATE_TIMER);
//	else
		p_data[idUser][P_ULTIMATEDELAY] = floatround(fTimeCoolDown);

}

