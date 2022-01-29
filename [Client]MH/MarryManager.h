#pragma once
#define MARRYMGR CMarryManager::GetInstance()
class CItem;
class CMarryManager
{
	//[结婚对象ID][By:十里坡剑神][QQ:112582793][2018/9/8]
	DWORD	m_MarryIDX;
	DWORD	m_dwTargetMapNum;	
	DWORD	m_dwTargetPos;		
	int		m_nChannelIdx;	
public:
	CMarryManager(void);
	~CMarryManager(void);
	GETINSTANCE(CMarryManager);
	//[结婚对象ID][By:十里坡剑神][QQ:112582793][2018/9/8]
	void SetMarryIDX(DWORD val){m_MarryIDX = val;}
	DWORD GetMarryIDX(){return m_MarryIDX;}
	//[夫妻下线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
	void SendLoginOutNotice();
	//[夫妻上线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
	void LoginNotifyForMarry();
	//[发送传送请求][By:十里坡剑神][QQ:112582793][2018/9/10]
	void SendBeckonMsg(CItem * pItem);
	//[设置传送信息][By:十里坡剑神][QQ:112582793][2018/9/10]
	void SetTargetMapNum(DWORD val){m_dwTargetMapNum = val;}
	DWORD GetTargerMapNum(){return m_dwTargetMapNum;}
	void SetTargerPos(DWORD val){m_dwTargetPos = val;}
	DWORD GetTargerPos();
	void SetChannelIdx(int val){m_nChannelIdx = val;}
	int GetChannelIdx(){return m_nChannelIdx;}
	void NetworkMsgParse(BYTE Protocol,void* pMsg);
};
