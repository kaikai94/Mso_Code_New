#pragma once
#include "gamestate.h"
#include "cImageSelf.h"
#define REDISCONNECT USINGTON(CReConnect)
enum REDISCONNECSTATE
{
	eToConnectDis,		//[连接登录网关][BY:十里坡剑神][QQ:112582793][2019-4-11][15:53:50]
	eSendLoginMsg,		//[发送登录消息][BY:十里坡剑神][QQ:112582793][2019-4-11][15:54:01]
	eToConnectAG,		//[连接角色网关][BY:十里坡剑神][QQ:112582793][2019-4-11][15:54:10]
	eSendGetCharList,	//[发送获取角色列表信息消息][BY:十里坡剑神][QQ:112582793][2019-4-11][15:54:49]
	eSendGetMapInfo,	//[发送获取地图频道信息消息][BY:十里坡剑神][QQ:112582793][2019-4-11][15:55:35]
	eEnterGame,			//[进入游戏][BY:十里坡剑神][QQ:112582793][2019-4-11][15:56:02]
	eNull,				//[不进行任何操作][BY:十里坡剑神][QQ:112582793][2019-4-11][20:27:37]
};
struct RECONNINFO
{
	RECONNINFO(){ memset(this, 0, sizeof(RECONNINFO)); }
	char  UserAccount[MAX_NAME_LENGTH + 1];	//[账号][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:02]
	char  UserPwd[MAX_NAME_LENGTH + 1];		//[密码][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:14]
	char SafePwd[20];						//[安全锁][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:02]
	char ReConncetDistrIP[32];				//[登陆网关IP][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:02]
	char ReConnectAgentIP[32];				//[角色网关IP][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:02]
	WORD wDistrPort;						//[登陆网关端口][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:02]
	WORD wAgentPort;						//[角色网关端口][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:02]
	DWORD dwHeroPos;						//[登陆地图的坐标][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:02]
	BOOL bAIWorking;						//[内挂开启状态][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:02]
	DWORD dwPetDBIndex;						//[召唤宠物的编号][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:02]
	BYTE AddType;							//[自动加点类型][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:02]
	WORD	wMapNum;						//[登陆地图编号][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:02]
	DWORD	dwPlayerID;						//[角色ID][BY:十里坡剑神][QQ:112582793][2019-4-11][9:54:02]
};
class CChannelDialog;
extern void SendOutMsg();
class CReConnect :	public CGameState
{
	cImageSelf m_imageLoginBarTop;
	cImageSelf m_imageLoginBarBottom;
	cImageSelf m_imageLoginBackGround;
	RECONNINFO m_ReDisInfo;
	BOOL	m_bReConnecting;
	BOOL	m_bIsWaiting;//[是否等待重连][BY:十里坡剑神][QQ:112582793][2019-4-11][10:03:43]
	BYTE	m_Step;		 //[进度][BY:十里坡剑神][QQ:112582793][2019-4-11][10:12:58]
	DWORD   dwStepTime;
	DWORD	m_DistAuthKey;
	DWORD   dwSendMsgTime;
	DWORD	dwTimeCount;
public:
	CReConnect(void);
	~CReConnect(void);
	//[游戏状态初虚函数重写][BY:十里坡剑神][QQ:112582793][2019-4-11][9:40:40]
	virtual BOOL Init(void* pInitParam);
	virtual void Release(CGameState* pNextGameState);	
	virtual void Process();
	virtual void BeforeRender();
	virtual void AfterRender();
	virtual void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);
	virtual void OnDisconnect();
	/****************************************************************************************/
	//[重连逻辑函数][BY:十里坡剑神][QQ:112582793][2019-4-11][9:41:07]
	BOOL IsInReCnnMode(){return m_bReConnecting;}
	void SetRecnnMode(BOOL ReConnect){m_bReConnecting=!ReConnect;}
	//[记录DIS端口，IP][BY:十里坡剑神][QQ:112582793][2019-4-11][9:45:58]
	void OnConnecToDistServer(char* ip,WORD port);
	//[记录账号密码][BY:十里坡剑神][QQ:112582793][2019-4-11][9:46:29]
	void OnLoginAccount(char *pAccoutn, char *pPswrd);
	//[记录AG端口,IP][BY:十里坡剑神][QQ:112582793][2019-4-11][9:47:08]
	void OnLoginAccountOK(char* ip,WORD port);
	//[记录安全码][BY:十里坡剑神][QQ:112582793][2019-4-11][9:47:52]
	void OnOpenSafeLock(char *Pwd);
	//[记录选择角色ID][BY:十里坡剑神][QQ:112582793][2019-4-11][9:48:53]
	void OnSelectPlayer(DWORD SelPlayerID){m_ReDisInfo.dwPlayerID=SelPlayerID;}
	//[记录地图编号][BY:十里坡剑神][QQ:112582793][2019-4-11][9:49:30]
	void OnEnterMap(WORD newmap){m_ReDisInfo.wMapNum=newmap;}
	//[游戏资源清理][BY:十里坡剑神][QQ:112582793][2019-4-11][9:51:28]
	//void ClearGameData();
	//[记录宠物召唤信息][BY:十里坡剑神][QQ:112582793][2019-4-11][12:11:05]
	void SetPetDBIndex(DWORD index){m_ReDisInfo.dwPetDBIndex=index;}
	//[重置重连进程][BY:十里坡剑神][QQ:112582793][2019-4-11][15:43:50]
	void RetrunStep(BYTE);

	//[外挂检测模块获取账号和角色ID][BY:十里坡剑神][QQ:112582793][2019-4-11][9:50:26]
	char * GetUserAccount(){return m_ReDisInfo.UserAccount;}
	char * GetUserPassWord(){return m_ReDisInfo.UserPwd;}
	DWORD GetCharID(){return m_ReDisInfo.dwPlayerID;}

	//[重连等待通知消息调用][BY:十里坡剑神][QQ:112582793][2019-4-11][10:03:11]
	char m_MaintainaceNoteMsg[256];//[等待重连通知消息][BY:十里坡剑神][QQ:112582793][2019-4-11][10:04:50]
	void InitForReConnect(BOOL bIsWait = FALSE);
	//[附加函数][BY:十里坡剑神][QQ:112582793][2019-4-11][11:51:01]
	void RunAutoPatch(BOOL bVal = TRUE);
	void ReadReDisInfo();
	void WirteReDisInfo();
	void SendOpenSafeLock();
	void SetAutoAddPoint(BYTE val){m_ReDisInfo.AddType = val;}
	void StopReCnnMode();
	DWORD GetHeroPos(){return m_ReDisInfo.dwHeroPos;}
	WORD GetMapNum(){return m_ReDisInfo.wMapNum;}
	//[网络消息解析函数][BY:十里坡剑神][QQ:112582793][2019-4-11][20:06:18]
	void DisConnectSuccess(void*);
	void LoginNAck(void*);
	void LoginAck(void*);
	void CharListAck(void*);
	void CharListNAck();
	void MapChannelInfoAck(void*);
	void MapChannelInfoNack();
	void SendLoginSYN();
	void SendCharListSYN();
	void SendMapInfoSYN();
	void ConnectDisServer();
};
EXTERNGLOBALTON(CReConnect)