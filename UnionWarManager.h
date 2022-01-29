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
	//[�����Ӫ��][BY:ʮ���½���][QQ:112582793][2019-10-22][23:31:32]
	WORD	wMaxTeamNum;
	//[ÿ��Ӫ��������][BY:ʮ���½���][QQ:112582793][2019-10-22][23:31:45]
	WORD	wMaxTeamPlayerNum;
	//[�����볡�ص���ͼ���][BY:ʮ���½���][QQ:112582793][2019-10-28][16:33:55]
	WORD	wBackToMapNum;
	//[�����볡�ص���ͼ����][BY:ʮ���½���][QQ:112582793][2019-10-28][16:34:31]
	WORD	wBackToMapKind;
	//[��ս���Ƶȼ�][BY:ʮ���½���][QQ:112582793][2019-10-29][16:05:22]
	WORD	wLimitLevel;
	//[���⽱��������][BY:ʮ���½���][QQ:112582793][2019-10-31][12:39:47]
	WORD	wRewardRank;
	//[���⽱������][BY:ʮ���½���][QQ:112582793][2019-10-31][12:41:17]
	WORD	wRewardRatio;
	//[��������][BY:ʮ���½���][QQ:112582793][2019-10-31][13:06:37]
	WORD	wBasicReward;
	//[�����������][BY:ʮ���½���][QQ:112582793][2019-10-31][14:12:57]
	WORD	wMinPlayer;
	//[�Ƿ����������������][BY:ʮ���½���][QQ:112582793][2019-10-31][14:10:32]
	BOOL	bIsClear;
	//[�Ƿ�Ϊ��ս��ͼ][BY:ʮ���½���][QQ:112582793][2019-10-22][23:29:18]
	BOOL	bUnionWarMap;
};
#pragma pack(pop)
#define UNIONWARMGR	USINGTON(CUnionWarManager)

class CPlayer;
class CMonster;

class CUnionWarManager
{
protected:	

	//[��ս״̬][BY:ʮ���½���][QQ:112582793][2019-10-22][23:29:30]
	eUnionWarState	m_dwWarState;
	//[��ս����ʱ��][BY:ʮ���½���][QQ:112582793][2019-10-23][0:59:04]
	DWORD			m_dwWarTime;	
	//[��սʱ���][BY:ʮ���½���][QQ:112582793][2019-10-22][23:36:36]
	sUnionWarTimeInfo	m_UnionWarTimeInfo[eUnionWarDay_Max];
	//[������Ϣ��¼��][BY:ʮ���½���][QQ:112582793][2019-10-22][23:28:45]
	CYHHashTable<sUnionWarInfo>	m_UnionWarUnionTable;	
	//[��;�볡��Ҽ�¼��][BY:ʮ���½���][QQ:112582793][2019-10-22][23:28:32]
	CYHHashTable<CPlayer> m_UnionLeavePlayer;
	//[��ս������Ϣ][BY:ʮ���½���][QQ:112582793][2019-10-29][16:02:26]
	sUnionSet m_UnionSet;
	//[��ս��Ϣ������ʱ����][BY:ʮ���½���][QQ:112582793][2019-10-29][16:02:50]
	UNIONWARINFO UnionInfo[10];
protected:
	//[���㿪ʼʱ������][BY:ʮ���½���][QQ:112582793][2019-10-23][14:41:42]
    int GetSecondInterval( SYSTEMTIME curTime, SYSTEMTIME basicTime );			
public:
	CUnionWarManager(void);
	~CUnionWarManager(void);
	//[��ʼ����ս��Ϣ����ʼǰ�����ȵ���][BY:ʮ���½���][QQ:112582793][2019-10-23][10:21:35]
	void Init();
	//[�ͷ���ս��Դ��������������][BY:ʮ���½���][QQ:112582793][2019-10-23][10:21:59]
	void Release();
	//[������ս������Ϣ��ʼ��ʱ�Զ�����][BY:ʮ���½���][QQ:112582793][2019-10-23][10:22:25]
	BOOL LoadUnionWarInfo();
	//[�ж���ս�Ƿ�ʼ][BY:ʮ���½���][QQ:112582793][2019-10-23][10:22:50]
	BOOL IsUnionWaring(CPlayer* pPlayer);
	//[��ս����][BY:ʮ���½���][QQ:112582793][2019-10-23][10:23:00]
	void ProcessUnionWar();
	//[��ӽ�ɫ����ս][BY:ʮ���½���][QQ:112582793][2019-10-23][10:23:12]
	void AddPlayer( CPlayer* pPlayer );
	//[��ɫ�˳���ս��ͼ���Ƴ���ɫ��Ϣ������ָ��ʧЧ��ɵ�ͼ����][BY:ʮ���½���][QQ:112582793][2019-10-23][10:23:24]
	void RemovePlayer( CPlayer* pPlayer );	
	//[�������������Ϣ���ͻ���][BY:ʮ���½���][QQ:112582793][2019-10-23][10:23:56]
	void DiePlayer(CPlayer* pDiePlayer,CPlayer* pAttacker);
	//[������ʱ�ͷ�][BY:ʮ���½���][QQ:112582793][2019-10-31][20:43:41]
	void DiePlayerToLong(CPlayer* pPlayer);
	//[�����ж��Ƿ�Ϊ��ս��ͼ����ս�رտ���״̬][BY:ʮ���½���][QQ:112582793][2019-10-23][10:24:26]
	BOOL IsUnionWarProcess();
	BOOL IsUnionWarStart();
	//[�ж��Ƿ�ɽ�����ս��ͼ][BY:ʮ���½���][QQ:112582793][2019-10-23][12:36:31]
	WORD IsCanMoveIn(CPlayer* pPlayer);
	WORD IsCanMoveIn(DWORD dwPlayerID);
	//[�жϵ�ǰ��ͼ�Ƿ�Ϊ��ս��ͼ][BY:ʮ���½���][QQ:112582793][2019-10-23][14:54:05]
	BOOL IsUnionWarMap(){ return m_UnionSet.bUnionWarMap; }
	//[��������������������][BY:ʮ���½���][QQ:112582793][2019-10-25][23:49:35]
	void ClearPlayerNotEnough();
	//[������ս��Ϣ�����в������][BY:ʮ���½���][QQ:112582793][2019-10-28][16:58:33]
	void SendUnionWarInfoToAllPlayer();
	void SendUnionWarInfo(sUnionWarInfo* pInfo);
	void SendUnionWarInfo(CPlayer* pPlayer);
	//[������ս][BY:ʮ���½���][QQ:112582793][2019-10-28][21:53:56]
	void EndUnionWar(BOOL bIsComplete);
	//[���Ų������˽���][BY:ʮ���½���][QQ:112582793][2019-10-31][13:14:06]
	void SendLostReward(sUnionWarInfo * pUnionInfo);
	//[���㽱��][BY:ʮ���½���][QQ:112582793][2019-10-31][13:22:46]
	void CalcReward();
};

EXTERNGLOBALTON(CUnionWarManager)


