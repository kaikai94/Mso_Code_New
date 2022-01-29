#pragma once
#include "gamestate.h"
#include "cImageSelf.h"
#define REDISCONNECT USINGTON(CReConnect)
enum REDISCONNECSTATE
{
	eToConnectDis,		//[���ӵ�¼����][BY:ʮ���½���][QQ:112582793][2019-4-11][15:53:50]
	eSendLoginMsg,		//[���͵�¼��Ϣ][BY:ʮ���½���][QQ:112582793][2019-4-11][15:54:01]
	eToConnectAG,		//[���ӽ�ɫ����][BY:ʮ���½���][QQ:112582793][2019-4-11][15:54:10]
	eSendGetCharList,	//[���ͻ�ȡ��ɫ�б���Ϣ��Ϣ][BY:ʮ���½���][QQ:112582793][2019-4-11][15:54:49]
	eSendGetMapInfo,	//[���ͻ�ȡ��ͼƵ����Ϣ��Ϣ][BY:ʮ���½���][QQ:112582793][2019-4-11][15:55:35]
	eEnterGame,			//[������Ϸ][BY:ʮ���½���][QQ:112582793][2019-4-11][15:56:02]
	eNull,				//[�������κβ���][BY:ʮ���½���][QQ:112582793][2019-4-11][20:27:37]
};
struct RECONNINFO
{
	RECONNINFO(){ memset(this, 0, sizeof(RECONNINFO)); }
	char  UserAccount[MAX_NAME_LENGTH + 1];	//[�˺�][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:02]
	char  UserPwd[MAX_NAME_LENGTH + 1];		//[����][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:14]
	char SafePwd[20];						//[��ȫ��][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:02]
	char ReConncetDistrIP[32];				//[��½����IP][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:02]
	char ReConnectAgentIP[32];				//[��ɫ����IP][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:02]
	WORD wDistrPort;						//[��½���ض˿�][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:02]
	WORD wAgentPort;						//[��ɫ���ض˿�][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:02]
	DWORD dwHeroPos;						//[��½��ͼ������][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:02]
	BOOL bAIWorking;						//[�ڹҿ���״̬][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:02]
	DWORD dwPetDBIndex;						//[�ٻ�����ı��][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:02]
	BYTE AddType;							//[�Զ��ӵ�����][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:02]
	WORD	wMapNum;						//[��½��ͼ���][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:02]
	DWORD	dwPlayerID;						//[��ɫID][BY:ʮ���½���][QQ:112582793][2019-4-11][9:54:02]
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
	BOOL	m_bIsWaiting;//[�Ƿ�ȴ�����][BY:ʮ���½���][QQ:112582793][2019-4-11][10:03:43]
	BYTE	m_Step;		 //[����][BY:ʮ���½���][QQ:112582793][2019-4-11][10:12:58]
	DWORD   dwStepTime;
	DWORD	m_DistAuthKey;
	DWORD   dwSendMsgTime;
	DWORD	dwTimeCount;
public:
	CReConnect(void);
	~CReConnect(void);
	//[��Ϸ״̬���麯����д][BY:ʮ���½���][QQ:112582793][2019-4-11][9:40:40]
	virtual BOOL Init(void* pInitParam);
	virtual void Release(CGameState* pNextGameState);	
	virtual void Process();
	virtual void BeforeRender();
	virtual void AfterRender();
	virtual void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);
	virtual void OnDisconnect();
	/****************************************************************************************/
	//[�����߼�����][BY:ʮ���½���][QQ:112582793][2019-4-11][9:41:07]
	BOOL IsInReCnnMode(){return m_bReConnecting;}
	void SetRecnnMode(BOOL ReConnect){m_bReConnecting=!ReConnect;}
	//[��¼DIS�˿ڣ�IP][BY:ʮ���½���][QQ:112582793][2019-4-11][9:45:58]
	void OnConnecToDistServer(char* ip,WORD port);
	//[��¼�˺�����][BY:ʮ���½���][QQ:112582793][2019-4-11][9:46:29]
	void OnLoginAccount(char *pAccoutn, char *pPswrd);
	//[��¼AG�˿�,IP][BY:ʮ���½���][QQ:112582793][2019-4-11][9:47:08]
	void OnLoginAccountOK(char* ip,WORD port);
	//[��¼��ȫ��][BY:ʮ���½���][QQ:112582793][2019-4-11][9:47:52]
	void OnOpenSafeLock(char *Pwd);
	//[��¼ѡ���ɫID][BY:ʮ���½���][QQ:112582793][2019-4-11][9:48:53]
	void OnSelectPlayer(DWORD SelPlayerID){m_ReDisInfo.dwPlayerID=SelPlayerID;}
	//[��¼��ͼ���][BY:ʮ���½���][QQ:112582793][2019-4-11][9:49:30]
	void OnEnterMap(WORD newmap){m_ReDisInfo.wMapNum=newmap;}
	//[��Ϸ��Դ����][BY:ʮ���½���][QQ:112582793][2019-4-11][9:51:28]
	//void ClearGameData();
	//[��¼�����ٻ���Ϣ][BY:ʮ���½���][QQ:112582793][2019-4-11][12:11:05]
	void SetPetDBIndex(DWORD index){m_ReDisInfo.dwPetDBIndex=index;}
	//[������������][BY:ʮ���½���][QQ:112582793][2019-4-11][15:43:50]
	void RetrunStep(BYTE);

	//[��Ҽ��ģ���ȡ�˺źͽ�ɫID][BY:ʮ���½���][QQ:112582793][2019-4-11][9:50:26]
	char * GetUserAccount(){return m_ReDisInfo.UserAccount;}
	char * GetUserPassWord(){return m_ReDisInfo.UserPwd;}
	DWORD GetCharID(){return m_ReDisInfo.dwPlayerID;}

	//[�����ȴ�֪ͨ��Ϣ����][BY:ʮ���½���][QQ:112582793][2019-4-11][10:03:11]
	char m_MaintainaceNoteMsg[256];//[�ȴ�����֪ͨ��Ϣ][BY:ʮ���½���][QQ:112582793][2019-4-11][10:04:50]
	void InitForReConnect(BOOL bIsWait = FALSE);
	//[���Ӻ���][BY:ʮ���½���][QQ:112582793][2019-4-11][11:51:01]
	void RunAutoPatch(BOOL bVal = TRUE);
	void ReadReDisInfo();
	void WirteReDisInfo();
	void SendOpenSafeLock();
	void SetAutoAddPoint(BYTE val){m_ReDisInfo.AddType = val;}
	void StopReCnnMode();
	DWORD GetHeroPos(){return m_ReDisInfo.dwHeroPos;}
	WORD GetMapNum(){return m_ReDisInfo.wMapNum;}
	//[������Ϣ��������][BY:ʮ���½���][QQ:112582793][2019-4-11][20:06:18]
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