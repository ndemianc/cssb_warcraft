
//const KEYSMENU = MENU_KEY_1|MENU_KEY_2|MENU_KEY_3|MENU_KEY_4|MENU_KEY_5|MENU_KEY_6|MENU_KEY_7|MENU_KEY_8|MENU_KEY_9|MENU_KEY_0;

//Изменение талисманов Ожерелье и Шлем
#define TALISMAN_ITEM_NECKLACE_NUM	get_pcvar_num(CVAR_wc3_t_necklace_num)  // Количество зарядов иммунитета от ульты (Ожерелье) для талисмана
new arrTalismanNecklaceNum[33];												// Массив игроков для количества зарядов иммунитета от ульты (Ожерелье)

#define TALISMAN_ITEM_HELM_NUM	get_pcvar_num(CVAR_wc3_t_helm_num)  // Количество зарядов (Шлем) для талисмана
new arrTalismanHelmNum[33];											// Массив игроков для количества зарядов  (Шлем)


#define	MAX_TALISMANS 25	//Макимальное количество талисманов


new arrTalismanNum[33];					//Сколько всего талисманов у игрока
new arrTalismanChange[33];				//Выбор другого талисмана
new arrTalismanActive[33];				//Какой талисман активен у игрока
new arrListTalisman[33][MAX_TALISMANS];	//Список всех талисманов имеющихся у игрока (9 талисманов)
new bool:arrPlayerInitTalisman[33];		//Загрузка всех талисманов когда игрок зашел на сервер и появилось меню выбора рас (грузится один раз)


new stSyncHudCreate;

//Дефайны
#define	MAX_SPAWNS_TALISMAN	60	//Максимальное количество точек появления на карте талисмана
#define	MAX_MODE_TALISMAN	get_pcvar_num( CVAR_wc3_mode_talisman )	//Режимы работы системы талисманов
#define	TIME_SPAWN_TALISMAN			get_pcvar_float( CVAR_wc3_time_spawn_talisman )		//Через какое время будет появляться талисман
#define	TIME_TASK_SPAWN_TALISMAN	600.0												//Отсчет времени

#define RING_MAX	5 //Максимальное число колец восстановления HP

//Задачи
#define	TASK_SPAWN_TALISMAN	985645

new iTotalSpawnsTalisman = 0;	//Сколько всего добавлено респаунов в файл


//Координаты респаунов
new arrSpawnOrigin[MAX_SPAWNS_TALISMAN][3];

new Float:fTimeSpawnTalismanGlobal;	//Глобальный счетчик времени через которое появится талисман			

new szClassNameTalisman[] = "war3ft_talisman";					//Имя класса талисмана
new szModelTalisman[64] = "models/CSSB/Items/fun_supplybox.mdl"	//Путь к моделе талисмана

#define	TALISMAN_ITEM_NONE						-1
#define	TALISMAN_ITEM_ANKH						0	// Ankh of Reincarnation
#define	TALISMAN_ITEM_BOOTS						1	// Boots of Speed
#define	TALISMAN_ITEM_CLAWS						2	// Claws of Attack
#define	TALISMAN_ITEM_CLOAK						3	// Cloak of Shadows
#define	TALISMAN_ITEM_MASK						4	// Mask of Death
#define	TALISMAN_ITEM_NECKLACE					5	// Necklace of Immunity
#define	TALISMAN_ITEM_FROST						6	// Orb of Frost
#define	TALISMAN_ITEM_HEALTH					7	// Periapt of Health
#define	TALISMAN_ITEM_PROTECTANT				10	// Mole Protectant
#define	TALISMAN_ITEM_HELM						11	// Helm of Excellence
#define	TALISMAN_ITEM_AMULET					12	// Amulet of the Cat
#define	TALISMAN_ITEM_SOCK						13	// Sock of the Feather
#define	TALISMAN_ITEM_GLOVES					14	// Flaming Gloves of Warmth
#define	TALISMAN_ITEM_RING						15	// Ring of Regeneration + 1
#define	TALISMAN_ITEM_CHAMELEON					16	// Chameleon
#define	TALISMAN_ITEM_INTELLECT					18	// Intellect
#define	TALISMAN_ITEM_GRENADE_PROTECTION		19	// Grenade Protection
#define	TALISMAN_ITEM_MIRROR_SHIELD				20	// Mirror Shield
#define	TALISMAN_ITEM_ESP						21	// ESP
#define	TALISMAN_ITEM_FROST_ARMOR				22	// Frost Armor
#define	TALISMAN_ITEM_ULTIMATE_TIMER_DIVIDER	25	// Ultimate Timer Divider
#define	TALISMAN_ITEM_DEVOURER_LIGHT			27	//Пожиратель змей
#define	TALISMAN_ITEM_ICE_BLOCK					28	//Ледяная глыба
#define	TALISMAN_ITEM_THIEF						29	//Похититель денег
#define	TALISMAN_ITEM_MIRROR_ULTIMATE			30	//Отражатель супер навыка

//Список талисманов
new const LIST_TALISMANS[MAX_TALISMANS] = 
{
	TALISMAN_ITEM_ANKH,						// Ankh of Reincarnation
	TALISMAN_ITEM_BOOTS,					// Boots of Speed
	TALISMAN_ITEM_CLAWS,					// Claws of Attack
	TALISMAN_ITEM_CLOAK,					// Cloak of Shadows
	TALISMAN_ITEM_MASK,						// Mask of Death
	TALISMAN_ITEM_NECKLACE,					// Necklace of Immunity
	TALISMAN_ITEM_FROST,					// Orb of Frost
	TALISMAN_ITEM_HEALTH,					// Periapt of Health
	TALISMAN_ITEM_PROTECTANT,				// Mole Protectant
	TALISMAN_ITEM_HELM,						// Helm of Excellence
	TALISMAN_ITEM_AMULET,					// Amulet of the Cat
	TALISMAN_ITEM_SOCK,						// Sock of the Feather
	TALISMAN_ITEM_GLOVES,					// Flaming Gloves of Warmth
	TALISMAN_ITEM_RING,						// Ring of Regeneration + 1
	TALISMAN_ITEM_CHAMELEON,				// Chameleon
	TALISMAN_ITEM_INTELLECT,				// Intellect
	TALISMAN_ITEM_GRENADE_PROTECTION,		// Grenade Protection
	TALISMAN_ITEM_MIRROR_SHIELD,			// Mirror Shield
	TALISMAN_ITEM_ESP,						// ESP
	TALISMAN_ITEM_FROST_ARMOR,				// Frost Armor
	TALISMAN_ITEM_ULTIMATE_TIMER_DIVIDER,	// Ultimate Timer Divider
	TALISMAN_ITEM_DEVOURER_LIGHT,			//Пожиратель змей
	TALISMAN_ITEM_ICE_BLOCK,				//Ледяная глыба
	TALISMAN_ITEM_THIEF,					//Похититель денег
	TALISMAN_ITEM_MIRROR_ULTIMATE			//Отражатель супер навыка
	
};

//Функции баз данных MYSQL и SQLITE
forward DB_GetAllTalisman( idUser );
forward MYSQLX_GetAllTalisman(idUser);
forward SQLITE_GetAllTalisman(idUser);









