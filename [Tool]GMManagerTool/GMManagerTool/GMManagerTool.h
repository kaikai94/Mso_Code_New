
/***数据库访问实现全局变量 by:OpU_Eye QQ:418914674***/

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif
#include <map>
#include <string>
#include "resource.h"		
#include "MyDBWork.h"
#include "MHFile.h"

#define _path_ "./GMToolConfigFile/SqlIDCofnig.ini"  //数据库配置文件路径   by:OpU_Eye  QQ:418914674

struct GUILDINFO
{
	int GuildIdx;
	CString GuildName;
	DWORD MasterIdx;
	CString MasterName;
	int GuildLevel;
	CString Location;
	CString UnionIdxName;
	int UnionIdx;
	DWORD GuildMoney;
	DWORD GuildPoint;
	DWORD GuildKillMonster;
};

struct ITEMINFO
{
	int Num;
	std::string Name;
	int  kindnum;
};
typedef std::pair<std::wstring, int> PAIR;
struct CmpByValue {
	bool operator()(const PAIR& lhs, const PAIR& rhs) {
		return lhs.second < rhs.second;
	}
};
class CGMManagerToolApp : public CWinApp
{
public:
	CGMManagerToolApp();
	~CGMManagerToolApp();
	CString TempStr;
	CMyDBWork m_IDDB;
	CMyDBWork m_ChrDB;
	CMyDBWork m_LogDB;
	
public:
	virtual BOOL InitInstance();
	std::map<int, ITEMINFO> m_ChangeUseConfig;
	std::map<int, std::wstring> m_StateChange;
	std::map<std::wstring, int>m_MapInfo;
	void LoadItemInfo();
	void LoadMapInfo();

	/*设置数据库连接字符串 by:OpU_Eye QQ:418914674*/
	BOOL SetIDDBInfo(CString m_DBUser, CString m_DBPassWord, CString m_DBip);
	BOOL SetChrDBInfo(CString m_DBUser, CString m_DBPassWord, CString m_DBip);
	BOOL SetLogDBInfo(CString m_DBUser, CString m_DBPassWord, CString m_DBip);
	/*账号查询函数 by:OpU_Eye QQ:418914674*/
	BOOL SelectLoginIdByID(CString id_loginid);
	BOOL SelectSafeCodeByID(CString id_loginid);
	BOOL SelectLonginIdByIp(CString m_IPSel);
	BOOL SelectLonginIdByLevel(int m_Level);
	BOOL SelectLonginIdByMall(long long m_Mall);
	BOOL SelectLonginIdByGold(long long m_Gold);
	/*账号修改函数 by:OpU_Eye QQ:418914674*/
	BOOL GetMax();
	BOOL InsertNewID(int m_propid, CString m_UserId, CString m_UserPassWord, CString m_IP, CString m_Email, CString tel);
	BOOL AddGoldMoney(CString UserId,long GoldMoney);
	BOOL CLGoldMoney(CString UserID);
	BOOL UpdateUerID(CString m_UserId, CString m_UserPassWord,CString m_IP, CString m_Email,CString tel);
	/*角色查询函数	by:OpU_Eye QQ:418914674*/
	BOOL SelectCharacterByUserId(int m_UserId);
	BOOL SelectCharacterByName(CString m_CharacterName);
	BOOL SelectChrItemForChr(int m_CharaIdx);
	BOOL SelectChrByChrID(int m_UserId);
	/*角色修改函数	by:OpU_Eye QQ:418914674*/
	BOOL DeleteChrByID(int m_CharaIdx);
	BOOL UpdateCharacterInfo(int m_CharaIdx, int m_GenGoal, int m_Dex, int m_Sta, int m_Simmak, int m_Fame, int m_Grade, int m_MaxGrade, int m_GradePoint, int m_State, int m_BadFame, int m_Reset, long m_Money, int m_Map, long m_AbilityExp, CString m_NickName, CString m_MarryName);
	/*物品查询函数	by:OpU_Eye QQ:418914674*/
	BOOL SelectItemByDBID(int DBID);
	BOOL SelectItemByItemID(int ItemID);
	BOOL SelectItemByCharID(int CharID);
	BOOL SelectItemRareInfo(int DBID);
	BOOL SelectItemOptionInfo(int DBID);
	/*物品修改函数	by:OpU_Eye QQ:418914674*/
	BOOL UpdataItemRareInfo(int DBID, int GenGol, int MinChub, int Cheryuk, int Simmek, int Life, int NaeRyuk, int Shield, int FireAttr, int WaterAttr, int TreeAttr, int GoldAttr, int EarthAttr, int Phyattack, int FireAttack, int WaterAttack, int TreeAttack, int GoldAttack, int EarthAttack, int Phydefense);
	BOOL UpdataItemOptionInfo(int DBID, int GenGol, int MinChub, int Cheryuk, int Simmek, int Life, int NaeRyuk, int Shield, int FireAttr, int WaterAttr, int TreeAttr, int GoldAttr, int EarthAttr, int Phyattack, int Critical, int FireAttack, int WaterAttack, int TreeAttack, int GoldAttack, int EarthAttack, int Phydefense);
	BOOL DeleteItem(int DBID);
	BOOL DeleteItemRareInfo(int DBID);
	BOOL DeleteItemOptionInfo(int DBID);
	/*帮会信息查询函数  by:OpU_Eye  QQ:418914674*/
	BOOL SelectGuildInfoForGuildName(CString GuildName);
	BOOL SelectGuildInfoForGuildMasterName(CString GuildMasterName);
	BOOL SelectGuildInfoForMasterIdx(int MasterIdx);
	BOOL GetGuildData();
	/*帮会信息修改函数  by:OpU_Eye  QQ:418914674*/
	BOOL UpdataGuildInfo(GUILDINFO* GuildInfo);
	BOOL UpddataGUildPointInfo(GUILDINFO* GuildInfo);
	DECLARE_MESSAGE_MAP()
};

extern CGMManagerToolApp theApp;