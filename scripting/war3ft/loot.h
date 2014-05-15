
new szClassNameItems[] = "war3ft_loot_items";

new szModelItems[64];
new szModelArXpHpItems[64];
//new szSoundLootItems[64];
new szSpriteItems[64];
new idSpriteItems;
new szSpriteNoItems[64];
new idSpriteNoItems;

new Array:arrIntTotalLootEntity;	//Массив сущностей
new Array:arrIntOriginsLootX;		//Массив координат для отображения спрайта по X
new Array:arrIntOriginsLootY;		//Массив координат для отображения спрайта по Y
new Array:arrIntOriginsLootZ;		//Массив координат для отображения спрайта по Z

new Array:arrIntItemVictimShop1;	//Массив вещей, которые были у жертвы в первом слоте
new Array:arrIntItemVictimShop2;	//Массив вещей, которые были у жертвы во втором слоте
new Array:arrIntItemVictimShop3;	//Массив вещей, которые были у жертвы в третьем слоте

new Array:arrIntDrawSpriteItems;	//Массив спрайтов отрисованных вещей
new Array:arrIntScaleSpriteItems;	//Массив масштабов спрайтов отрисованных вещей

new Array:arrIntLootRemove;			//Массив таймеров отсчета удаления предметов

new arrColorItemsRGB[3];	//Цвет выпавших вещей

new msgSyncObj;

#define	TASK_LOOTITEM	90865		// ID задачи
#define	TASK_LOOTREMOVE	90866		// ID задачи

#define task_set(%1)				set_task(%1)
#define task_remove(%1)				remove_task(%1)


enum enTypeLoot{

	enMoney = 0,
	enHealth = 1,
	enArmor = 2,
	enXP = 3,
	enOther = 4

}