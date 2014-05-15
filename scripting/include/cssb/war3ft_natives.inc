/**
* Добавляет предмет в меню WC3.
 
* @param name		Название
* @param cost		Деньги
* @param onoff		Включить/выключить предмет
* @param messagebuy	Сообщение о покупке	
* @return		An internal extra item ID, or -1 on failure.
 */
native wc3_register_extra_item(const name[], cost,onoff,messagebuy);
native wc3_buy_extra_item(idUser, idItem);
