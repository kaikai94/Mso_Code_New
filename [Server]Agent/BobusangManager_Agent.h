



#pragma once

#define BOBUSANGMGR	USINGTON(BobusangManager_Agent)


#define BOBUSANG_POSNUM_MAX	4	//[����ˢ��λ�õ��������][BY:ʮ���½���][QQ:112582793][2019-4-25][14:03:28]
#define DEALITEM_BIN_TABNUM	7	//[���˳�����Ʒҳ��][BY:ʮ���½���][QQ:112582793][2019-4-25][14:03:46]

#define BOBUSANG_CHECKTIME	10000	//[����״̬�����ʱ��][BY:ʮ���½���][QQ:112582793][2019-4-25][14:04:06]

struct BobusangPosPerMap
{
	BobusangPosPerMap() {memset(this, 0, sizeof(this));};
	VECTOR3	ApprPos;
	float	ApprDir;
};


struct BOBUSANG_MAPINFO
{//[�������˵�ͼ��Ϣ�ṹ][BY:ʮ���½���][QQ:112582793][2019-4-25][14:05:05]
	BOBUSANG_MAPINFO() : dataIdx(0), mapNum(0) {};
	DWORD dataIdx;	//[������Ʒ��ҳ���][BY:ʮ���½���][QQ:112582793][2019-4-25][14:05:21]
	DWORD mapNum;	//[��ͼ���][BY:ʮ���½���][QQ:112582793][2019-4-25][14:05:32]
	std::string strMap;

	BobusangPosPerMap Pos[BOBUSANG_POSNUM_MAX];	//[ˢ��������][BY:ʮ���½���][QQ:112582793][2019-4-25][14:05:40]
};

enum bobusangInfotime{ eBBSIT_CUR, eBBSIT_NEXT, eBBSIT_KINDNUM, };
enum bobusangAppearedState{
	eBBSAS_NONE = -1,
	eBBSAS_DISAPPEAR			= 0,
	eBBSAS_APPEAR				= 2,
	eBBSAS_APPEAR_DELAYED		= 4,	
	eBBSAS_DISAPPEAR_DELAYED	= 8,	
	eBBSAS_TIME_DELAYED			= 16,	
};

class BobusangManager_Agent
{
	BOOL				m_bManager;
	BOOL				m_bOnProcessing;
	DWORD				m_dwBobusangCheckTime;

	int*				m_piAppearedState;		
	DWORD				m_nChannelTotalNum;		
	DWORD				m_dwAppearTimeMin;		
	DWORD				m_dwAppearTimeMax;		
	DWORD				m_dwDurationTimeMin;	
	DWORD				m_dwDurationTimeMax;	

	CYHHashTable<BOBUSANG_MAPINFO>	m_mapInfo;

	BOBUSANGINFO*		m_pBobusangInfo;
	WORD	m_wMapIndex;
public:
	BobusangManager_Agent(void);
	~BobusangManager_Agent(void);

	void	CheckManager();
	void	SetManager(BOOL bVal)
	{
		m_bManager = bVal;
	}

	void	BobusangMgr_Start();
	void	BobusangMgr_Init();
	void	BobusangMgr_Process();		
	void	BobusangMgr_Release();

	BOOL	LoadChannelInfo();		
	BOOL	LoadBobusangInfo();		
	void	InitBobusangInfo();
	void	SetBobusangInfo(DWORD iChannel);			
	void	SendBobusangInfoMsgToMap(BOBUSANGINFO* pInfo);
	void	SendBobusangLeaveMsgToMap(BOBUSANGINFO* pInfo);	
	void	SendBobusangLeaveNotifyToMap(BOBUSANGINFO* pInfo);

	void	SetChannelState(DWORD iChannel, int bobusangAppearedState);
	void	DeveloperRequest(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
	BOOL	SetProcessing(BOOL bVal);
	stTIME*	SetstTIMEfromMinValue(DWORD min);
	void	OutPutBobusangInfo();
	void	RloadBobusangInfo();
};

EXTERNGLOBALTON(BobusangManager_Agent)


