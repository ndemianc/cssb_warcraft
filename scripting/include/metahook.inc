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

// 获得玩家是否使用 MetaHook
// 注意: 如果这个函数不能正常使用, 请在 "Putinserver" 的时候增加至少 0.15 秒的延迟.
// 以下所有函数都得经过这个判断即可.
//
// iIndex = 玩家索引
//
// 返回: 使用返回 1, 未使用返回 0.
//
native MH_IsMetaHookPlayer(iIndex)

// 给玩家显示一张TGA图片
// iMode: 图片消息显示模式 [0 - 禁用 // 1 - 淡入 // 2 - 淡出 // 3 - 淡入淡出]
// iCenter = 是否图片居中
// szTga = 图片文件名 [Tga必须放在cstrike\djb_tga目录 填的时候填文件名即可 不要加.tga后缀]
// x = 横向坐标 (0.5 为屏幕中心位置)
// y = 纵向坐标 (0.5 为屏幕中心位置)
// r = 图片消息的红色值
// g = 图片消息的绿色值
// b = 图片消息的蓝色值
// flTime = 图片消息的显示时间
// iChannel = 图片消息的通道值 (小于 0 或者大于 19 为随机) //18-19通道为保留通道 请勿使用
native MH_DrawTargaImage(id,szTga[],iFunc,iCenter,r,g,b,Float:x,Float:y,iMode,iChannel,Float:flTime)

// 给玩家显示 TGA 视网膜特效
// iToggle 是否启用
// iClass 视网膜类型 //共1-6 6种，请手动尝试
native MH_DrawRetina(id,iToggle,iClass)

// 画SPR复活进度条
// iToggle = 是否开启
// fTime = 时间 到时间后自动消失
native MH_RespawnBar(iIndex,iToggle,Float:fTime)

// 生化模式夜视仪
// [iType 1=Human iType 2=Zombie iType3=Close]
//
native MH_ZombieModNV(iIndex,iType)

// 生化模式新UI界面(TGA 文件实现)
// iIndex = 玩家索引
// szSprite = 技能的SPR
// iSlot = SPR对应的技能的栏
// iDisplayMod = 显示方式1-3 正常 闪动 由下到上填充
// iChannel = 频道 (1-4)
// iType = 类型 [1-HUMAN 2-ZOMBIE]
// flDisplayTime = 显示特效时间
// iZombieClass = 僵尸头像设定 [主插件定义 1-9]
native MH_NewZBUI(iIndex,szSprite[],iSlot,iDisplayMod,iChannel,iType,Float:flDisplayTime,iZombieClass)

//画回复图标
// iIndex = 玩家索引
// szSprite = 回复时的SPR(hud.txt)
// x,y = 屏幕百分比坐标
// r,g,b,alpha = 颜色参数
// flTime = 回复时间
// iChannel = 显示频道
native MH_DrawFadeIcon(iIndex,const szSprite[],Float:x ,Float:y, r, g, b, alpha, Float:flTime, iChannel )

//画大灾变等级系统
// iIndex = 玩家索引
// iToggle = 开关
// iHealth = 血量等级
// iDamage = 攻击力等级
// iPercent = 攻击力百分比
native MH_DrawZbsLevel(iIndex,iToggle,iHealth,iDamage,iPercent)

//画僵尸怒气
// iIndex = 玩家索引
// iRageStat = 怒气状态(1-3) 其他为关闭
// iLength = 怒气槽长度(2-219)
native MH_DrawZombieRage(iIndex,iRageStat,iLength);

//开关跟随图标
// iIndex = 玩家索引
// iToggle = 开关
//
native MH_DrawFollowIconToggle(iIndex,iToggle);

//重置所有跟随图标
//
// iIndex = 玩家索引
//
native MH_DrawFollowIconReset(iIndex);

//给玩家显示动态跟随图标
//
// iIndex = 玩家索引
// szSprotes = hud.txt中的SPR
// iOrigin.x,y,z = 物体坐标
// iToggle = 是否显示
// iDistance = 是否显示距离
// iChannel = 频道(1-64)
// r,g,b 颜色值
//
native MH_DrawFollowIcon(iIndex, const szSprite[] , iOriginX, iOriginY , iOriginZ, iToggle, iDistance, iChannel , r ,g ,b )


// 给玩家显示额外弹药
//
// iIndex = 玩家索引
// szAmmotype = Hud中的子弹型号
// iAmmo = 子弹数量(SHORT类型)，当iAmmo小于0时不再显示
// 
native MH_DrawExtraAmmo(iIndex,const szAmmotypeSprite[],iAmmo)

// 显示给玩家一张图片 (SPR)
//
// iIndex = 玩家索引
// iCheck = 是否读取hud.txt中的SPR(1为hud中的SPR)，反之是SPR文件
// iCenter = 是否图片居中
// szSprite = 图片文件名
// x = 横向坐标 (0.5 为屏幕中心位置)
// y = 纵向坐标 (0.5 为屏幕中心位置)
// r = 图片消息的红色值
// g = 图片消息的绿色值
// b = 图片消息的蓝色值
// iMode = 图片消息显示模式 [0 - 禁用 // 1 - 淡入 // 2 - 淡出 // 3 - 淡入淡出]
// flTime = 图片消息的显示时间
// iChanne = 图片消息的通道值 (小于 0 或者大于 63 为随机)
// iLength = 图片显示的长度(为方便只支持hud中的SPR，长度请通过坐标计算，适合用过做动态血条等) //全部请填-1
//
native MH_DrawImage(iIndex, iCheck = 1, iCenter = 1, const szSprite[], Float:x = 0.5, Float:y = 0.5, r = 255, g = 255, b = 255, iMode = 0, Float:flDisplayTime = 1.0, iChanne = 0, iLength)

// 显示给玩家一张 Additive 模式图片 (SPR)
//
// iIndex = 玩家索引
// iCheck = 是否读取hud.txt中的SPR(1为hud中的SPR)，反之是SPR文件
// iCenter = 是否图片居中
// szSprite = 图片文件名
// x = 横向坐标 (0.5 为屏幕中心位置)
// y = 纵向坐标 (0.5 为屏幕中心位置)
// r = 图片消息的红色值
// g = 图片消息的绿色值
// b = 图片消息的蓝色值
// iMode = 图片消息显示模式 [0 - 禁用 // 1 - 淡入 // 2 - 淡出 // 3 - 淡入淡出]
// flTime = 图片消息的显示时间
// iChanne = 图片消息的通道值 (小于 0 或者大于 63 为随机)
// iLength = 图片显示的长度(为方便只支持hud中的SPR，长度请通过坐标计算，适合用过做动态血条等) //全部请填-1
native MH_DrawAdditiveImage(iIndex, iCheck = 1, iCenter = 1, const szSprite[], Float:x = 0.5, Float:y = 0.5, r = 255, g = 255, b = 255, iMode = 0, Float:flDisplayTime = 1.0, iChanne = 0, iLength)

// 显示给玩家一张 Holes 模式图片 (SPR)
//
// iIndex = 玩家索引
// iCheck = 是否读取hud.txt中的SPR(1为hud中的SPR)，反之是SPR文件
// iCenter = 是否图片居中
// szSprite = 图片文件名
// x = 横向坐标 (0.5 为屏幕中心位置)
// y = 纵向坐标 (0.5 为屏幕中心位置)
// r = 图片消息的红色值
// g = 图片消息的绿色值
// b = 图片消息的蓝色值
// iMode = 图片消息显示模式 [0 - 禁用 // 1 - 淡入 // 2 - 淡出 // 3 - 淡入淡出]
// flTime = 图片消息的显示时间
// iChanne = 图片消息的通道值 (小于 0 或者大于 63 为随机)
// iLength = 图片显示的长度(为方便只支持hud中的SPR，长度请通过坐标计算，适合用过做动态血条等)
//
native MH_DrawHolesImage(iIndex, iCheck = 1, iCenter = 1, const szSprite[], Float:x = 0.5, Float:y = 0.5, r = 255, g = 255, b = 255, iMode = 0, Float:flDisplayTime = 1.0, iChanne = 0, iLength)

// 显示给玩家一段文字
//
// iIndex = 玩家索引
// iCenter = 是否文字居中
// szText = 需要显示的文字
// x = 横向坐标 (0.5 为屏幕中心位置)
// y = 纵向坐标 (0.5 为屏幕中心位置)
// r = 文字消息的红色值
// g = 文字消息的绿色值
// b = 文字消息的蓝色值
// flTime = 文字消息的显示时间
// iChanne = 文字消息的通道值 (小于 0 或者大于 63 为随机)
// iLength = 图片显示的长度(为方便只支持hud中的SPR，长度请通过坐标计算，适合用过做动态血条等)
//
native MH_DrawText(iIndex, iCenter = 1, const szText[], Float:x = 0.5, Float:y = 0.5, r = 255, g = 255, b = 255, Float:flDisplayTime = 1.0, iChanne = 0)

// 设置玩家视角模型的身体部位
//
// iIndex = 玩家索引
// iBody = 身体部位 (-1 不做任何改变)
//
native MH_SetViewEntityBody(iIndex, iBody = -1)

// 设置玩家视角模型的渲染属性
// 渲染属性参数请参照 "amxconst.inc".
//
// iIndex = 玩家索引
// iMode = 渲染模式 (-1 不做任何改变)
// iEffect = 渲染特效
// iColorR = 渲染红色值
// iColorG = 渲染绿色值
// iColorB = 渲染蓝色值
// iAmout = 渲染值
//
native MH_SetViewEntityRender(iIndex, iMode = -1, iEffect, iColorR, iColorG, iColorB, iAmout)

// 给玩家提供一个记分牌
// 
// iIndex = 玩家索引
// szScoreBoardName = 记分牌SPR
// iTopLeftNum = 记分牌左上数字 (SHORT支持大于10000)
// iTopCenterNum = 记分牌中间数字 (BYTE 最大255)
// iTopRightNum = 记分牌右上数字 (SHORT)
// iDownLeftNum = 记分牌左下数字 (SHORT)
// iDownRightNum = 记分牌右下数字 (SHORT)
// iMode = 可用于记录模式 (SHORT)
// 当iDownLeftNum 等于 -1 时，可关闭下方的数字显示，比如用于团队个人模式等
// 当iMode = -1 时可以关闭记分牌 :D 记分牌初始化时iMode默认为 -1(1为生化 竞技样式 2 为大灾变样式 3 为 团队个人样式)
//
native MH_DrawScoreBoard(iIndex, const szScoreBoardNameSprite[], iTopLeftNum, iTopCenterNum, iTopRightNum, iDownLeftNum, iDownRightNum ,iMode)
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1033\\ f0\\ fs16 \n\\ par }
*/
