#ifndef __DISTRIBUTEDBMSGPARSER_H__
#define __DISTRIBUTEDBMSGPARSER_H__

#include "db.h"

#include "CommonDBMsgParser.h"

enum 
{
	eNull = 0,
	eLoginCheckQuery,
	eIpCheckQuery,
	eLoginCheckQueryTest,
	eCheckDynamicPW,
	eLoginCheckQueryForJP,
	eRegister,		//[账号注册回调][By:十里坡剑神][QQ:112582793][2017/11/23]
	eRestpwd,		//[密码修改回调][By:十里坡剑神][QQ:112582793][2017/11/23]
	eGetGuildRank,	//[帮会排行查询][By:十里坡剑神][QQ:112582793][2017/11/23]
	eGetHeroRank,	//[角色排行查询][By:十里坡剑神][QQ:112582793][2017/11/24]
	eGetSiegeWarMasterInfo,	//[获取洛阳城主信息][By:十里坡剑神][QQ:112582793][2018/2/23]
	eGetKillRank,	//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
};

void LoginCheckInit();
BOOL IPCheckQuery(char* ip, DWORD AuthKey, DWORD dwConnectionIndex);
void RIpCheckQuery(LPQUERY pData, LPDBMESSAGE pMessage);
BOOL IPCheckQueryHK(char* ip, DWORD AuthKey, DWORD dwConnectionIndex);

BOOL LoginCheckQueryTest(char* id, char* pw, DWORD AuthKey, DWORD dwConnectionIndex);
void RLoginCheckQueryTest(LPQUERY pData, LPDBMESSAGE pMessage);

BOOL LoginGetDynamicPWQuery(char* id, DWORD dwConnectionIndex );
void RLoginGetDynamicPWQuery(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);

BOOL CheckDynamicPW( char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum );

void RCheckDynamicPW( LPQUERY pData, LPDBMESSAGE pMessage );

void LoginError(DWORD dwConnectionIdx, DWORD AuthKey,DWORD ErrorCode,DWORD dwParam = 0);

BOOL LoginCheckQuery(char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum);
BOOL LoginCheckQueryforMD5(char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum, char* pPN);
BOOL LoginCheckQueryEx(char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum, char* pPN,char * Mac);
void LoginCheckDelete(DWORD UserID);
void RLoginCheckQuery(LPQUERY pData, LPDBMESSAGE pMessage);
void RLoginCheckQueryforMD5(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);

void LoginCheckQueryForJP( char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum );
void RLoginCheckQueryForJP( LPQUERY pData, LPDBMESSAGE pMessage );
//[帐号注册,密码修改相关函数定义][By:十里坡剑神][QQ:112582793][2017/11/23]
void RegQueryEx(DWORD dwConnectionIndex,MSGREG* pmsg);
void RregQuery(LPQUERY pData, LPDBMESSAGE pMessage);

void RestPwd(DWORD dwConnectionIndex,MSGREG* pmsg);
void Rrestpwd(LPQUERY pData, LPDBMESSAGE pMessage);
//[角色排行查询][By:十里坡剑神][QQ:112582793][2017/11/23]
void TopListInfoForHero(WORD Type);
void RTopListInfoForHero(LPQUERY pData, LPDBMESSAGE pMessage );
//[帮会排行查询][By:十里坡剑神][QQ:112582793][2017/11/23]
void TopListInfoForGuild();
void RTopListInfoForGuild(LPQUERY pData, LPDBMESSAGE pMessage );
void TopListCheck(DWORD dwConnectionIndex,char * msg);
void RTopListCheck(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
//[击杀排行加载][By:十里坡剑神][QQ:112582793][2018/4/22]
void TopListInfoForKiller();
void RopListInfoForKiller(LPQUERY pData, LPDBMESSAGE pMessage);
void UpdateTopListForKiller(const char * strDay);
BOOL CheckIP(char * szIP);
#ifdef _NPROTECT_
//---for Hacktool User Check
void HackToolUser( DWORD UserIdx );
#endif

#endif //__DISTRIBUTEDBMSGPARSER_H__


