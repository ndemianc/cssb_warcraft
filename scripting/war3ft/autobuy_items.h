/*
*	Авто закупка предметов
*/

//Общее количество менюшек
#define	MAX_NUM_AUTO_MENUS 2

//Заголовки менюшек
new const MENU_TITLES_AUTOBUY_ITEMS[MAX_NUM_AUTO_MENUS][] = 
{
	"MENU_ID_BASE_AUTOBUY_ITEMS",
	"MENU_ID_DELETE_AUTOBUY_ITEMS"
};
//Функции выбора пунктов в меню
new const MENU_CALLBACK_AUTOBUY_ITEMS_ALL[MAX_NUM_AUTO_MENUS][] = 
{
	"_Callback_MENU_BaseABI",
	"_Callback_SUB_MENU_DeleteABI"
};
/*
#define AUTO_ITEM_SLOT_ONE       0
#define AUTO_ITEM_SLOT_TWO       1
#define AUTO_ITEM_SLOT_THREE     2
*/
new arriShopAutoItems[33][MAX_PLAYER_ITEMS];			// Массив авто закупочных предметов

new bool:bAutoBuyOnOff[33];	// (включение\отключение авто-закупки)