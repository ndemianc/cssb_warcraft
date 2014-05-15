// MetaHook2 Include
// Version 1.0
// Author: Martin
// Editor: Csoldjb (Thanks to Martin!!!)

//For Counter-Strike 1.6 3248-4554 (PR47/48)

#if defined _metahook_included
	#endinput
#endif
#define _metahook_included

#if AMXX_VERSION_NUM >= 175
 #pragma reqlib metahook
 #if !defined AMXMODX_NOAUTOLOAD
  #pragma loadlib metahook
 #endif
#else
 #pragma library metahook
#endif

// �������Ƿ�ʹ�� MetaHook
// ע��: ������������������ʹ��, ���� "Putinserver" ��ʱ���������� 0.15 ����ӳ�.
// �������к������þ�������жϼ���.
//
// iIndex = �������
//
// ����: ʹ�÷��� 1, δʹ�÷��� 0.
//
native MH_IsMetaHookPlayer(iIndex)

// �������ʾһ��TGAͼƬ
// iMode: ͼƬ��Ϣ��ʾģʽ [0 - ���� // 1 - ���� // 2 - ���� // 3 - ���뵭��]
// iCenter = �Ƿ�ͼƬ����
// szTga = ͼƬ�ļ��� [Tga�������cstrike\djb_tgaĿ¼ ���ʱ�����ļ������� ��Ҫ��.tga��׺]
// x = �������� (0.5 Ϊ��Ļ����λ��)
// y = �������� (0.5 Ϊ��Ļ����λ��)
// r = ͼƬ��Ϣ�ĺ�ɫֵ
// g = ͼƬ��Ϣ����ɫֵ
// b = ͼƬ��Ϣ����ɫֵ
// flTime = ͼƬ��Ϣ����ʾʱ��
// iChannel = ͼƬ��Ϣ��ͨ��ֵ (С�� 0 ���ߴ��� 19 Ϊ���) //18-19ͨ��Ϊ����ͨ�� ����ʹ��
native MH_DrawTargaImage(id,szTga[],iFunc,iCenter,r,g,b,Float:x,Float:y,iMode,iChannel,Float:flTime)

// �������ʾ TGA ����Ĥ��Ч
// iToggle �Ƿ�����
// iClass ����Ĥ���� //��1-6 6�֣����ֶ�����
native MH_DrawRetina(id,iToggle,iClass)

// ��SPR���������
// iToggle = �Ƿ���
// fTime = ʱ�� ��ʱ����Զ���ʧ
native MH_RespawnBar(iIndex,iToggle,Float:fTime)

// ����ģʽҹ����
// [iType 1=Human iType 2=Zombie iType3=Close]
//
native MH_ZombieModNV(iIndex,iType)

// ����ģʽ��UI����(TGA �ļ�ʵ��)
// iIndex = �������
// szSprite = ���ܵ�SPR
// iSlot = SPR��Ӧ�ļ��ܵ���
// iDisplayMod = ��ʾ��ʽ1-3 ���� ���� ���µ������
// iChannel = Ƶ�� (1-4)
// iType = ���� [1-HUMAN 2-ZOMBIE]
// flDisplayTime = ��ʾ��Чʱ��
// iZombieClass = ��ʬͷ���趨 [��������� 1-9]
native MH_NewZBUI(iIndex,szSprite[],iSlot,iDisplayMod,iChannel,iType,Float:flDisplayTime,iZombieClass)

//���ظ�ͼ��
// iIndex = �������
// szSprite = �ظ�ʱ��SPR(hud.txt)
// x,y = ��Ļ�ٷֱ�����
// r,g,b,alpha = ��ɫ����
// flTime = �ظ�ʱ��
// iChannel = ��ʾƵ��
native MH_DrawFadeIcon(iIndex,const szSprite[],Float:x ,Float:y, r, g, b, alpha, Float:flTime, iChannel )

//�����ֱ�ȼ�ϵͳ
// iIndex = �������
// iToggle = ����
// iHealth = Ѫ���ȼ�
// iDamage = �������ȼ�
// iPercent = �������ٷֱ�
native MH_DrawZbsLevel(iIndex,iToggle,iHealth,iDamage,iPercent)

//����ʬŭ��
// iIndex = �������
// iRageStat = ŭ��״̬(1-3) ����Ϊ�ر�
// iLength = ŭ���۳���(2-219)
native MH_DrawZombieRage(iIndex,iRageStat,iLength);

//���ظ���ͼ��
// iIndex = �������
// iToggle = ����
//
native MH_DrawFollowIconToggle(iIndex,iToggle);

//�������и���ͼ��
//
// iIndex = �������
//
native MH_DrawFollowIconReset(iIndex);

//�������ʾ��̬����ͼ��
//
// iIndex = �������
// szSprotes = hud.txt�е�SPR
// iOrigin.x,y,z = ��������
// iToggle = �Ƿ���ʾ
// iDistance = �Ƿ���ʾ����
// iChannel = Ƶ��(1-64)
// r,g,b ��ɫֵ
//
native MH_DrawFollowIcon(iIndex, const szSprite[] , iOriginX, iOriginY , iOriginZ, iToggle, iDistance, iChannel , r ,g ,b )


// �������ʾ���ⵯҩ
//
// iIndex = �������
// szAmmotype = Hud�е��ӵ��ͺ�
// iAmmo = �ӵ�����(SHORT����)����iAmmoС��0ʱ������ʾ
// 
native MH_DrawExtraAmmo(iIndex,const szAmmotypeSprite[],iAmmo)

// ��ʾ�����һ��ͼƬ (SPR)
//
// iIndex = �������
// iCheck = �Ƿ��ȡhud.txt�е�SPR(1Ϊhud�е�SPR)����֮��SPR�ļ�
// iCenter = �Ƿ�ͼƬ����
// szSprite = ͼƬ�ļ���
// x = �������� (0.5 Ϊ��Ļ����λ��)
// y = �������� (0.5 Ϊ��Ļ����λ��)
// r = ͼƬ��Ϣ�ĺ�ɫֵ
// g = ͼƬ��Ϣ����ɫֵ
// b = ͼƬ��Ϣ����ɫֵ
// iMode = ͼƬ��Ϣ��ʾģʽ [0 - ���� // 1 - ���� // 2 - ���� // 3 - ���뵭��]
// flTime = ͼƬ��Ϣ����ʾʱ��
// iChanne = ͼƬ��Ϣ��ͨ��ֵ (С�� 0 ���ߴ��� 63 Ϊ���)
// iLength = ͼƬ��ʾ�ĳ���(Ϊ����ֻ֧��hud�е�SPR��������ͨ��������㣬�ʺ��ù�����̬Ѫ����) //ȫ������-1
//
native MH_DrawImage(iIndex, iCheck = 1, iCenter = 1, const szSprite[], Float:x = 0.5, Float:y = 0.5, r = 255, g = 255, b = 255, iMode = 0, Float:flDisplayTime = 1.0, iChanne = 0, iLength)

// ��ʾ�����һ�� Additive ģʽͼƬ (SPR)
//
// iIndex = �������
// iCheck = �Ƿ��ȡhud.txt�е�SPR(1Ϊhud�е�SPR)����֮��SPR�ļ�
// iCenter = �Ƿ�ͼƬ����
// szSprite = ͼƬ�ļ���
// x = �������� (0.5 Ϊ��Ļ����λ��)
// y = �������� (0.5 Ϊ��Ļ����λ��)
// r = ͼƬ��Ϣ�ĺ�ɫֵ
// g = ͼƬ��Ϣ����ɫֵ
// b = ͼƬ��Ϣ����ɫֵ
// iMode = ͼƬ��Ϣ��ʾģʽ [0 - ���� // 1 - ���� // 2 - ���� // 3 - ���뵭��]
// flTime = ͼƬ��Ϣ����ʾʱ��
// iChanne = ͼƬ��Ϣ��ͨ��ֵ (С�� 0 ���ߴ��� 63 Ϊ���)
// iLength = ͼƬ��ʾ�ĳ���(Ϊ����ֻ֧��hud�е�SPR��������ͨ��������㣬�ʺ��ù�����̬Ѫ����) //ȫ������-1
native MH_DrawAdditiveImage(iIndex, iCheck = 1, iCenter = 1, const szSprite[], Float:x = 0.5, Float:y = 0.5, r = 255, g = 255, b = 255, iMode = 0, Float:flDisplayTime = 1.0, iChanne = 0, iLength)

// ��ʾ�����һ�� Holes ģʽͼƬ (SPR)
//
// iIndex = �������
// iCheck = �Ƿ��ȡhud.txt�е�SPR(1Ϊhud�е�SPR)����֮��SPR�ļ�
// iCenter = �Ƿ�ͼƬ����
// szSprite = ͼƬ�ļ���
// x = �������� (0.5 Ϊ��Ļ����λ��)
// y = �������� (0.5 Ϊ��Ļ����λ��)
// r = ͼƬ��Ϣ�ĺ�ɫֵ
// g = ͼƬ��Ϣ����ɫֵ
// b = ͼƬ��Ϣ����ɫֵ
// iMode = ͼƬ��Ϣ��ʾģʽ [0 - ���� // 1 - ���� // 2 - ���� // 3 - ���뵭��]
// flTime = ͼƬ��Ϣ����ʾʱ��
// iChanne = ͼƬ��Ϣ��ͨ��ֵ (С�� 0 ���ߴ��� 63 Ϊ���)
// iLength = ͼƬ��ʾ�ĳ���(Ϊ����ֻ֧��hud�е�SPR��������ͨ��������㣬�ʺ��ù�����̬Ѫ����)
//
native MH_DrawHolesImage(iIndex, iCheck = 1, iCenter = 1, const szSprite[], Float:x = 0.5, Float:y = 0.5, r = 255, g = 255, b = 255, iMode = 0, Float:flDisplayTime = 1.0, iChanne = 0, iLength)

// ��ʾ�����һ������
//
// iIndex = �������
// iCenter = �Ƿ����־���
// szText = ��Ҫ��ʾ������
// x = �������� (0.5 Ϊ��Ļ����λ��)
// y = �������� (0.5 Ϊ��Ļ����λ��)
// r = ������Ϣ�ĺ�ɫֵ
// g = ������Ϣ����ɫֵ
// b = ������Ϣ����ɫֵ
// flTime = ������Ϣ����ʾʱ��
// iChanne = ������Ϣ��ͨ��ֵ (С�� 0 ���ߴ��� 63 Ϊ���)
// iLength = ͼƬ��ʾ�ĳ���(Ϊ����ֻ֧��hud�е�SPR��������ͨ��������㣬�ʺ��ù�����̬Ѫ����)
//
native MH_DrawText(iIndex, iCenter = 1, const szText[], Float:x = 0.5, Float:y = 0.5, r = 255, g = 255, b = 255, Float:flDisplayTime = 1.0, iChanne = 0)

// ��������ӽ�ģ�͵����岿λ
//
// iIndex = �������
// iBody = ���岿λ (-1 �����κθı�)
//
native MH_SetViewEntityBody(iIndex, iBody = -1)

// ��������ӽ�ģ�͵���Ⱦ����
// ��Ⱦ���Բ�������� "amxconst.inc".
//
// iIndex = �������
// iMode = ��Ⱦģʽ (-1 �����κθı�)
// iEffect = ��Ⱦ��Ч
// iColorR = ��Ⱦ��ɫֵ
// iColorG = ��Ⱦ��ɫֵ
// iColorB = ��Ⱦ��ɫֵ
// iAmout = ��Ⱦֵ
//
native MH_SetViewEntityRender(iIndex, iMode = -1, iEffect, iColorR, iColorG, iColorB, iAmout)

// ������ṩһ���Ƿ���
// 
// iIndex = �������
// szScoreBoardName = �Ƿ���SPR
// iTopLeftNum = �Ƿ����������� (SHORT֧�ִ���10000)
// iTopCenterNum = �Ƿ����м����� (BYTE ���255)
// iTopRightNum = �Ƿ����������� (SHORT)
// iDownLeftNum = �Ƿ����������� (SHORT)
// iDownRightNum = �Ƿ����������� (SHORT)
// iMode = �����ڼ�¼ģʽ (SHORT)
// ��iDownLeftNum ���� -1 ʱ���ɹر��·���������ʾ�����������ŶӸ���ģʽ��
// ��iMode = -1 ʱ���ԹرռǷ��� :D �Ƿ��Ƴ�ʼ��ʱiModeĬ��Ϊ -1(1Ϊ���� ������ʽ 2 Ϊ���ֱ���ʽ 3 Ϊ �ŶӸ�����ʽ)
//
native MH_DrawScoreBoard(iIndex, const szScoreBoardNameSprite[], iTopLeftNum, iTopCenterNum, iTopRightNum, iDownLeftNum, iDownRightNum ,iMode)
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1033\\ f0\\ fs16 \n\\ par }
*/