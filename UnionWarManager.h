#pragma once

#include "hashtable.h"

#pragma pack(push,1)



enum eUnionWarDay
{
	eUnionWarDay_Sun = 0,
	eUnionWarDay_Mon,
	eUnionWarDay_Tue,
	eUnionWarDay_Wed,

	eUnionWarDay_Thur,
	eUnionWarDay_Fri,
	eUnionWarDay_Sat,
	eUnionWarDay_Max,
};

struct sUnionWarTimeInfo
{
	BOOL		bOn;
	SYSTEMTIME	UnionWarStartTime;
	DWORD		dwWarTime;
};

struct sUnionWarInfo
{
	sUnionWarInfo(WORD wMaxNum)
	{
		UnionWarPlayerTable.Initialize(wMaxNum);
		dwKillTimeCount = 0;
		dwDieTimeCount = 0;
		memset(sUnionName, 0, MAX_GUILD_NAME + 1);
	}
	CYHHashTable<CPlayer> UnionWarPlayerTable;
	DWORD dwKillTimeCount;
	DWORD dwDieTimeCount;
	DWORD dwUnionID;
	char sUnionName[MAX_GUILD_NAME + 1];
};
struct sUnionSet
{
	sUnionSet():wMaxTeamNum(5),
		wMaxTeamPlayerNum(150), bUnionWarMap(FALSE),
		wBackToMapNum(17), wLimitLevel(90), wRewardRank(15),
		wRewardRatio(2), wBasicReward(300), bIsClear(TRUE){}
	//[最大阵营数][BY:十里坡剑神][QQ:112582793][2019-10-22][23:31:32]
	WORD	wMaxTeamNum;
	//[每阵营最大玩家数][BY:十里坡剑神][QQ:112582793][2019-10-22][23:31:45]
	WORD	wMaxTeamPlayerNum;
	//[清理离场回到地图编号][BY:十里坡剑神][QQ:112582793][2019-10-28][16:33:55]
	WORD	wBackToMapNum;
	//[清理离场回到地图类型][BY:十里坡剑神][QQ:112582793][2019-10-28][16:34:31]
	WORD	wBackToMapKind;
	//[盟战限制等级][BY:十里坡剑神][QQ:112582793][2019-10-29][16:05:22]
	WORD	wLimitLevel;
	//[额外奖励的名次][BY:十里坡剑神][QQ:112582793][2019-10-31][12:39:47]
	WORD	wRewardRank;
	//[额外奖励倍数][BY:十里坡剑神][QQ:112582793][2019-10-31][12:41:17]
	WORD	wRewardRatio;
	//[基础奖励][BY:十里坡剑神][QQ:112582793][2019-10-31][13:06:37]
	WORD	wBasicReward;
	//[参与最低人数][BY:十里坡剑神][QQ:112582793][2019-10-31][14:12:57]
	WORD	wMinPlayer;
	//[是否清理人数不足队伍][BY:十里坡剑神][QQ:112582793][2019-10-31][14:10:32]
	BOOL	bIsClear;
	//[是否为盟战地图][BY:十里坡剑神][QQ:112582793][2019-10-22][23:29:18]
	BOOL	bUnionWarMap;
};
#pragma pack(pop)
#define UNIONWARMGR	USINGTON(CUnionWarManager)

class CPlayer;
class CMonster;

class CUnionWarManager
{
protected:	

	//[盟战状态][BY:十里坡剑神][QQ:112582793][2019-10-22][23:29:30]
	eUnionWarState	m_dwWarState;
	//[盟战进行时间][BY:十里坡剑神][QQ:112582793][2019-10-23][0:59:04]
	DWORD			m_dwWarTime;	
	//[盟战时间表][BY:十里坡剑神][QQ:112582793][2019-10-22][23:36:36]
	sUnionWarTimeInfo	m_UnionWarTimeInfo[eUnionWarDay_Max];
	//[队伍信息记录表][BY:十里坡剑神][QQ:112582793][2019-10-22][23:28:45]
	CYHHashTable<sUnionWarInfo>	m_UnionWarUnionTable;	
	//[中途离场玩家记录表][BY:十里坡剑神][QQ:112582793][2019-10-22][23:28:32]
	CYHHashTable<CPlayer> m_UnionLeavePlayer;
	//[盟战配置信息][BY:十里坡剑神][QQ:112582793][2019-10-29][16:02:26]
	sUnionSet m_UnionSet;
	//[盟战信息发送临时变量][BY:十里坡剑神][QQ:112582793][2019-10-29][16:02:50]
	UNIONWARINFO UnionInfo[10];
protected:
	//[计算开始时间秒数][BY:十里坡剑神][QQ:112582793][2019-10-23][14:41:42]
    int GetSecondInterval( SYSTEMTIME curTime, SYSTEMTIME basicTime );			
public:
	CUnionWarManager(void);
	~CUnionWarManager(void);
	//[初始化盟战信息，开始前必须先调用][BY:十里坡剑神][QQ:112582793][2019-10-23][10:21:35]
	void Init();
	//[释放盟战资源，结束后必须调用][BY:十里坡剑神][QQ:112582793][2019-10-23][10:21:59]
	void Release();
	//[加载盟战配置信息初始化时自动调用][BY:十里坡剑神][QQ:112582793][2019-10-23][10:22:25]
	BOOL LoadUnionWarInfo();
	//[判断盟战是否开始][BY:十里坡剑神][QQ:112582793][2019-10-23][10:22:50]
	BOOL IsUnionWaring(CPlayer* pPlayer);
	//[盟战进程][BY:十里坡剑神][QQ:112582793][2019-10-23][10:23:00]
	void ProcessUnionWar();
	//[添加角色到盟战][BY:十里坡剑神][QQ:112582793][2019-10-23][10:23:12]
	void AddPlayer( CPlayer* pPlayer );
	//[角色退出盟战地图需移除角色信息，否则指针失效造成地图崩溃][BY:十里坡剑神][QQ:112582793][2019-10-23][10:23:24]
	void RemovePlayer( CPlayer* pPlayer );	
	//[玩家死亡发送消息到客户端][BY:十里坡剑神][QQ:112582793][2019-10-23][10:23:56]
	void DiePlayer(CPlayer* pDiePlayer,CPlayer* pAttacker);
	//[死亡超时惩罚][BY:十里坡剑神][QQ:112582793][2019-10-31][20:43:41]
	void DiePlayerToLong(CPlayer* pPlayer);
	//[用于判断是否为盟战地图和盟战关闭开启状态][BY:十里坡剑神][QQ:112582793][2019-10-23][10:24:26]
	BOOL IsUnionWarProcess();
	BOOL IsUnionWarStart();
	//[判断是否可进入盟战地图][BY:十里坡剑神][QQ:112582793][2019-10-23][12:36:31]
	WORD IsCanMoveIn(CPlayer* pPlayer);
	WORD IsCanMoveIn(DWORD dwPlayerID);
	//[判断当前地图是否为盟战地图][BY:十里坡剑神][QQ:112582793][2019-10-23][14:54:05]
	BOOL IsUnionWarMap(){ return m_UnionSet.bUnionWarMap; }
	//[检查清理人数不足的联盟][BY:十里坡剑神][QQ:112582793][2019-10-25][23:49:35]
	void ClearPlayerNotEnough();
	//[发送盟战信息给所有参与玩家][BY:十里坡剑神][QQ:112582793][2019-10-28][16:58:33]
	void SendUnionWarInfoToAllPlayer();
	void SendUnionWarInfo(sUnionWarInfo* pInfo);
	void SendUnionWarInfo(CPlayer* pPlayer);
	//[结束盟战][BY:十里坡剑神][QQ:112582793][2019-10-28][21:53:56]
	void EndUnionWar(BOOL bIsComplete);
	//[发放参与联盟奖励][BY:十里坡剑神][QQ:112582793][2019-10-31][13:14:06]
	void SendLostReward(sUnionWarInfo * pUnionInfo);
	//[计算奖励][BY:十里坡剑神][QQ:112582793][2019-10-31][13:22:46]
	void CalcReward();
};

EXTERNGLOBALTON(CUnionWarManager)


