//Предметы Для shopmenu4
#define ITEM_DEVOURER_LIGHT			27	//Пожиратель змей
#define ITEM_ICE_BLOCK				28	//Ледяная глыба
#define ITEM_THIEF					29	//Похититель денег
#define ITEM_MIRROR_ULTIMATE		30	//Отражатель супер навыка
#define ITEM_DEATHCOIL				31	//Лик смерти
#define ITEM_IMPALE					32	//Пронзающий шип
#define ITEM_SLEEP					33	//Сонная аура
#define ITEM_BOOK_DEAD				34	//Книга мертвых
#define ITEM_TRANQUILITY			35	//Спокойствие

//new szTag[64];
//#define tag(%1) formatex(%1, sizeof(%1) - 1, "!y%L!t%L!y%L ",LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT")
//#define tag2 %1 (tag(%1))
//#define tag(%1)  (formatex(%1, sizeof(%1) - 1, "!y%L!t%L!y%L ",LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT"))
//#define tag2 %1 (tag(%1))


//Звуки Для shopmenu4
#define SOUND_ICE_BLOCK_EXPLODE		38		//Взрыв Ледяной глыбы
#define SOUND_ICE_BLOCK_FROZEN		39		//Заморозка Ледяной глыбы
#define SOUND_ICE_BLOCK_UNFROZEN	40		//Разморозка Ледяной глыбы
#define SOUND_ICE_BLOCK_CHILLED		41		//Заморожен Ледяной глыбой
#define SOUND_MIRROR_ULTIMATE_BLOCK	42		//Блокировка отражателя супер навыка
#define SOUND_DEATHCOIL				43		//Лик смерти
#define SOUND_DEATHCOIL_MISSILE		44		//Срабатывание Лик смерти
#define SOUND_SLEEP					45		//Сонная аура
#define SOUND_BOOK_DEAD_RAISE		46		//Вызов мертвецов хозяином
#define SOUND_BOOK_DEAD_WARLOCK		47		//Звук для ожившего мертвеца
#define SOUND_TRANQUILITY			48		//Спокойствие
#define SOUND_PRECAST_HOLY			49
#define SOUND_CAST_HOLY				50

//Спрайты Для shopmenu4
#define SPR_ICE_BLOCK_TRAIL		24	//"Хвост" Ледяной глыбы
#define SPR_MIRROR_ULTIMATE		25	//Срабатывание отражателя супер навыка
#define SPR_DEATHCOIL			26	//Лик смерти
#define SPR_BLOODSPRAY			27	//Impale
#define SPR_BLOODDROP			28	//Impale 2 
#define SPR_SLEEP				29	//Сонная аура

// Item Charge Defines - Предметы с зарядами
#define DEVOURER_LIGHT_CHARGES get_pcvar_num( CVAR_wc3_light_charges )					//Количество зарядов пожирателя змей
#define THIEF_TIME	get_pcvar_num( CVAR_wc3_thief_time )								//Время через которое прибавляются деньги
#define THIEF_MONEY	get_pcvar_num( CVAR_wc3_thief_money )								//Сколько денег прибавлять (учесть умножение на кол-во игроков)
#define MIRROR_ULTIMATE_CHARGES get_pcvar_num( CVAR_wc3_mirror_ult_charges )			//Количество зарядов отражателя супер навыка
#define DEATHCOIL_CHARGES get_pcvar_num( CVAR_wc3_deathcoil_charges )					//Количество зарядов Лик смерти
#define IMPALE_CHARGES get_pcvar_num( CVAR_wc3_impale_charges )							//Количество зарядов Пронзающий Шип
#define SLEEP_CHARGES get_pcvar_num( CVAR_wc3_sleep_charges )							//Количество зарядов Сонная аура
#define BOOK_DEAD_CHARGES get_pcvar_num( CVAR_wc3_book_dead_charges )					//Количество вызываемых скелетов Книга мертвых
#define BUY_ITEM_ONE_ROUND_BOOK_DEAD get_pcvar_num( CVAR_wc3_buy_round_book_dead )		//Сколько раз можно покупать предмет Книга мертвых за один раунд
#define BOOK_DEAD_PERCENT_HEALTH get_pcvar_num( CVAR_wc3_percent_hp_book_dead )			//Процент HP от здоровья вызвавшего дающийся скелету 
#define BUY_ITEM_ONE_ROUND_TRANQUILITY get_pcvar_num( CVAR_wc3_buy_round_tranquility )	//Сколько раз можно покупать предмет Спокойствие за один раунд

//Переменные
new arriTotalRingsDevourerLight[33];	// Массив зарядов пожирателя змей
new arriThiefTimer[33];					// Массив для Похититель денег 
new arriTotalChargeMirrorUltimate[33];	// Массив зарядов отражателя супер навыка
new arriTotalChargeDeathCoil[33];		// Массив зарядов Лик смерти
new arriTotalChargeImpale[33];			// Массив зарядов Пронзающий Шип
new arriTotalChargeSleep[33];			// Массив зарядов Сонная аура

//Динамические массивы
new Array:arrIntSkeletons;					//	ID игроков которых вызвали скелетами

//Глобальные дефайны
//#define P_OWNER_SKELETON		25		// ID хозяина вызвавшего скелетов
//#define P_ID_PLAYER_SKELETON	26		// ID игроков которых вызвали скелетами


//Task - задачи
#define TASK_ITEM_THIEF 46753

//Запрет предметов на определенных картах
new bool:bDisabledDevourerLight = false;	//Пожиратель змей
new bool:bDisabledIceBlock = false;			//Ледяная глыба
new bool:bDisabledThief = false;			//похититель денег
new bool:bDisabledMirrorUltimate = false;	//Отражатель супер навыка
new bool:bDisabledDeathcoil = false;		//Лик смерти
new bool:bDisabledImpale = false;			//Пронзающий шип
new bool:bDisabledSleep = false;			//Сонная аура
new bool:bDisabledBookDead = false;			//Книга мертвых
new bool:bDisabledTranquility = false;		//Спокойствие


//Для shopmenu4
//Настройки предметов из shopmenu4:
new CVAR_wc3_light_charges;			//Количество зарядов пожирателя змей
new CVAR_wc3_iceblock_icon;			//Включить/выключить иконку ледяной глыбы
new CVAR_wc3_iceblock_color;		//Цвет ледяной глыбы
new CVAR_wc3_iceblock_fteam;		//Будет ли замораживать своих по команде
new CVAR_wc3_iceblock_damage;		//Повреждения от ледяной глыбы
new CVAR_wc3_iceblock_timer;		//Таймер до следующей ледяной глыбы
new CVAR_wc3_thief_time;			//Время через которое прибаляются деньги
new CVAR_wc3_thief_money;			//Сколько денег прибавлять (учесть умножение на кол-во игроков)
new CVAR_wc3_mirror_ult_charges;	//Количество зарядов отражателя супер навыка
new CVAR_wc3_deathcoil_charges;		//Количество зарядов Лик смерти
new CVAR_wc3_deathcoil_damage;		//Количество повреждений наносимые Лик смерти ( расчет wc3_deathcoil_damage*(0.7) )
new CVAR_wc3_deathcoil_percent;		//Процент вероятности срабатывания Лик смерти
new CVAR_wc3_impale_damage;			//Повреждения при подбросе
new CVAR_wc3_impale_armordmg;		//Повреждения брони при подбросе
new CVAR_wc3_impale_colldmg;		//Повреждения при подбросе (при столкновении с потолком)
new CVAR_wc3_impale_charges;		//Количество зарядов Пронзающий Шип
new CVAR_wc3_impale_percent;		//Процент вероятности срабатывания Пронзающий Шип
new CVAR_wc3_sleep_charges;			//Количество зарядов Сонная аура
new CVAR_wc3_sleep_percent;			//Процент вероятности срабатывания Сонная аура
new CVAR_wc3_sleep_duration;		//Продолжительность сна при воздействии Сонная аура
new CVAR_wc3_book_dead_charges;		//Количество вызываемых скелетов Книга мертвых
new CVAR_wc3_buy_round_book_dead;	//Сколько раз можно покупать предмет Книга мертвых за один раунд
new CVAR_wc3_percent_hp_book_dead;	//Процент HP от здоровья вызвавшего дающийся скелету 
new CVAR_wc3_tranquility_time;		//Время работы предмета Спокойствие
new CVAR_wc3_tranquility_range;		//Радиус в котором могут лечиться игроки
new CVAR_wc3_buy_round_tranquility;	//Сколько раз можно покупать предмет Спокойствие за один раунд
new CVAR_wc3_tranquility_hp;		//По сколько HP восстанавливает предмет Спокойствие


//Цены на предметы shopmenu4:
new CVAR_wc3_cost_devourer_light;		//Devourer of lightnings - Пожиратель змей
new CVAR_wc3_cost_ice_block;			//Ice Block - Ледяная глыба
new CVAR_wc3_cost_thief;				//Thief of money - Похититель денег
new CVAR_wc3_cost_mirror_ult;			//Mirror Ultimate - Отражатель супер навыка
new CVAR_wc3_cost_death_coil;			//Death Coil - Лик смерти
new CVAR_wc3_cost_impale;				//Impale - Пронзающий Шип
new CVAR_wc3_cost_sleep;				//Sleep - Сонная аура
new CVAR_wc3_cost_book_dead;			//Book of The Dead - Книга мертвых
new CVAR_wc3_cost_tranquility;			//Спокойствие - Tranquility

