#pragma once
#define MARRYMGR CMarryManager::GetInstance()
class CItem;
class CMarryManager
{
	//[������ID][By:ʮ���½���][QQ:112582793][2018/9/8]
	DWORD	m_MarryIDX;
	DWORD	m_dwTargetMapNum;	
	DWORD	m_dwTargetPos;		
	int		m_nChannelIdx;	
public:
	CMarryManager(void);
	~CMarryManager(void);
	GETINSTANCE(CMarryManager);
	//[������ID][By:ʮ���½���][QQ:112582793][2018/9/8]
	void SetMarryIDX(DWORD val){m_MarryIDX = val;}
	DWORD GetMarryIDX(){return m_MarryIDX;}
	//[����������ʾ][By:ʮ���½���][QQ:112582793][2018/9/10]
	void SendLoginOutNotice();
	//[����������ʾ][By:ʮ���½���][QQ:112582793][2018/9/10]
	void LoginNotifyForMarry();
	//[���ʹ�������][By:ʮ���½���][QQ:112582793][2018/9/10]
	void SendBeckonMsg(CItem * pItem);
	//[���ô�����Ϣ][By:ʮ���½���][QQ:112582793][2018/9/10]
	void SetTargetMapNum(DWORD val){m_dwTargetMapNum = val;}
	DWORD GetTargerMapNum(){return m_dwTargetMapNum;}
	void SetTargerPos(DWORD val){m_dwTargetPos = val;}
	DWORD GetTargerPos();
	void SetChannelIdx(int val){m_nChannelIdx = val;}
	int GetChannelIdx(){return m_nChannelIdx;}
	void NetworkMsgParse(BYTE Protocol,void* pMsg);
};
