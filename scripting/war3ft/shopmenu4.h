//�������� ��� shopmenu4
#define ITEM_DEVOURER_LIGHT			27	//���������� ����
#define ITEM_ICE_BLOCK				28	//������� �����
#define ITEM_THIEF					29	//���������� �����
#define ITEM_MIRROR_ULTIMATE		30	//���������� ����� ������
#define ITEM_DEATHCOIL				31	//��� ������
#define ITEM_IMPALE					32	//���������� ���
#define ITEM_SLEEP					33	//������ ����
#define ITEM_BOOK_DEAD				34	//����� �������
#define ITEM_TRANQUILITY			35	//�����������

//new szTag[64];
//#define tag(%1) formatex(%1, sizeof(%1) - 1, "!y%L!t%L!y%L ",LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT")
//#define tag2 %1 (tag(%1))
//#define tag(%1)  (formatex(%1, sizeof(%1) - 1, "!y%L!t%L!y%L ",LANG_PLAYER,"WC3_BRACKET_LEFT",LANG_PLAYER,"WC3_MODCLIENT",LANG_PLAYER,"WC3_BRACKET_RIGHT"))
//#define tag2 %1 (tag(%1))


//����� ��� shopmenu4
#define SOUND_ICE_BLOCK_EXPLODE		38		//����� ������� �����
#define SOUND_ICE_BLOCK_FROZEN		39		//��������� ������� �����
#define SOUND_ICE_BLOCK_UNFROZEN	40		//���������� ������� �����
#define SOUND_ICE_BLOCK_CHILLED		41		//��������� ������� ������
#define SOUND_MIRROR_ULTIMATE_BLOCK	42		//���������� ���������� ����� ������
#define SOUND_DEATHCOIL				43		//��� ������
#define SOUND_DEATHCOIL_MISSILE		44		//������������ ��� ������
#define SOUND_SLEEP					45		//������ ����
#define SOUND_BOOK_DEAD_RAISE		46		//����� ��������� ��������
#define SOUND_BOOK_DEAD_WARLOCK		47		//���� ��� �������� ��������
#define SOUND_TRANQUILITY			48		//�����������
#define SOUND_PRECAST_HOLY			49
#define SOUND_CAST_HOLY				50

//������� ��� shopmenu4
#define SPR_ICE_BLOCK_TRAIL		24	//"�����" ������� �����
#define SPR_MIRROR_ULTIMATE		25	//������������ ���������� ����� ������
#define SPR_DEATHCOIL			26	//��� ������
#define SPR_BLOODSPRAY			27	//Impale
#define SPR_BLOODDROP			28	//Impale 2 
#define SPR_SLEEP				29	//������ ����

// Item Charge Defines - �������� � ��������
#define DEVOURER_LIGHT_CHARGES get_pcvar_num( CVAR_wc3_light_charges )					//���������� ������� ���������� ����
#define THIEF_TIME	get_pcvar_num( CVAR_wc3_thief_time )								//����� ����� ������� ������������ ������
#define THIEF_MONEY	get_pcvar_num( CVAR_wc3_thief_money )								//������� ����� ���������� (������ ��������� �� ���-�� �������)
#define MIRROR_ULTIMATE_CHARGES get_pcvar_num( CVAR_wc3_mirror_ult_charges )			//���������� ������� ���������� ����� ������
#define DEATHCOIL_CHARGES get_pcvar_num( CVAR_wc3_deathcoil_charges )					//���������� ������� ��� ������
#define IMPALE_CHARGES get_pcvar_num( CVAR_wc3_impale_charges )							//���������� ������� ���������� ���
#define SLEEP_CHARGES get_pcvar_num( CVAR_wc3_sleep_charges )							//���������� ������� ������ ����
#define BOOK_DEAD_CHARGES get_pcvar_num( CVAR_wc3_book_dead_charges )					//���������� ���������� �������� ����� �������
#define BUY_ITEM_ONE_ROUND_BOOK_DEAD get_pcvar_num( CVAR_wc3_buy_round_book_dead )		//������� ��� ����� �������� ������� ����� ������� �� ���� �����
#define BOOK_DEAD_PERCENT_HEALTH get_pcvar_num( CVAR_wc3_percent_hp_book_dead )			//������� HP �� �������� ���������� �������� ������� 
#define BUY_ITEM_ONE_ROUND_TRANQUILITY get_pcvar_num( CVAR_wc3_buy_round_tranquility )	//������� ��� ����� �������� ������� ����������� �� ���� �����

//����������
new arriTotalRingsDevourerLight[33];	// ������ ������� ���������� ����
new arriThiefTimer[33];					// ������ ��� ���������� ����� 
new arriTotalChargeMirrorUltimate[33];	// ������ ������� ���������� ����� ������
new arriTotalChargeDeathCoil[33];		// ������ ������� ��� ������
new arriTotalChargeImpale[33];			// ������ ������� ���������� ���
new arriTotalChargeSleep[33];			// ������ ������� ������ ����

//������������ �������
new Array:arrIntSkeletons;					//	ID ������� ������� ������� ���������

//���������� �������
//#define P_OWNER_SKELETON		25		// ID ������� ���������� ��������
//#define P_ID_PLAYER_SKELETON	26		// ID ������� ������� ������� ���������


//Task - ������
#define TASK_ITEM_THIEF 46753

//������ ��������� �� ������������ ������
new bool:bDisabledDevourerLight = false;	//���������� ����
new bool:bDisabledIceBlock = false;			//������� �����
new bool:bDisabledThief = false;			//���������� �����
new bool:bDisabledMirrorUltimate = false;	//���������� ����� ������
new bool:bDisabledDeathcoil = false;		//��� ������
new bool:bDisabledImpale = false;			//���������� ���
new bool:bDisabledSleep = false;			//������ ����
new bool:bDisabledBookDead = false;			//����� �������
new bool:bDisabledTranquility = false;		//�����������


//��� shopmenu4
//��������� ��������� �� shopmenu4:
new CVAR_wc3_light_charges;			//���������� ������� ���������� ����
new CVAR_wc3_iceblock_icon;			//��������/��������� ������ ������� �����
new CVAR_wc3_iceblock_color;		//���� ������� �����
new CVAR_wc3_iceblock_fteam;		//����� �� ������������ ����� �� �������
new CVAR_wc3_iceblock_damage;		//����������� �� ������� �����
new CVAR_wc3_iceblock_timer;		//������ �� ��������� ������� �����
new CVAR_wc3_thief_time;			//����� ����� ������� ����������� ������
new CVAR_wc3_thief_money;			//������� ����� ���������� (������ ��������� �� ���-�� �������)
new CVAR_wc3_mirror_ult_charges;	//���������� ������� ���������� ����� ������
new CVAR_wc3_deathcoil_charges;		//���������� ������� ��� ������
new CVAR_wc3_deathcoil_damage;		//���������� ����������� ��������� ��� ������ ( ������ wc3_deathcoil_damage*(0.7) )
new CVAR_wc3_deathcoil_percent;		//������� ����������� ������������ ��� ������
new CVAR_wc3_impale_damage;			//����������� ��� ��������
new CVAR_wc3_impale_armordmg;		//����������� ����� ��� ��������
new CVAR_wc3_impale_colldmg;		//����������� ��� �������� (��� ������������ � ��������)
new CVAR_wc3_impale_charges;		//���������� ������� ���������� ���
new CVAR_wc3_impale_percent;		//������� ����������� ������������ ���������� ���
new CVAR_wc3_sleep_charges;			//���������� ������� ������ ����
new CVAR_wc3_sleep_percent;			//������� ����������� ������������ ������ ����
new CVAR_wc3_sleep_duration;		//����������������� ��� ��� ����������� ������ ����
new CVAR_wc3_book_dead_charges;		//���������� ���������� �������� ����� �������
new CVAR_wc3_buy_round_book_dead;	//������� ��� ����� �������� ������� ����� ������� �� ���� �����
new CVAR_wc3_percent_hp_book_dead;	//������� HP �� �������� ���������� �������� ������� 
new CVAR_wc3_tranquility_time;		//����� ������ �������� �����������
new CVAR_wc3_tranquility_range;		//������ � ������� ����� �������� ������
new CVAR_wc3_buy_round_tranquility;	//������� ��� ����� �������� ������� ����������� �� ���� �����
new CVAR_wc3_tranquility_hp;		//�� ������� HP ��������������� ������� �����������


//���� �� �������� shopmenu4:
new CVAR_wc3_cost_devourer_light;		//Devourer of lightnings - ���������� ����
new CVAR_wc3_cost_ice_block;			//Ice Block - ������� �����
new CVAR_wc3_cost_thief;				//Thief of money - ���������� �����
new CVAR_wc3_cost_mirror_ult;			//Mirror Ultimate - ���������� ����� ������
new CVAR_wc3_cost_death_coil;			//Death Coil - ��� ������
new CVAR_wc3_cost_impale;				//Impale - ���������� ���
new CVAR_wc3_cost_sleep;				//Sleep - ������ ����
new CVAR_wc3_cost_book_dead;			//Book of The Dead - ����� �������
new CVAR_wc3_cost_tranquility;			//����������� - Tranquility

