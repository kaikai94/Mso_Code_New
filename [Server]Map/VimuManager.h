




#ifndef _VIMUMANAGER_H

#define _VIMUMANAGER_H


#define VIMUMGR CVimuManager::GetInstance()


class CVimuManager

{

protected:

	
public:


	MAKESINGLETON( CVimuManager );	


	CVimuManager();
	virtual ~CVimuManager();

	void UserLogOut( CPlayer* pPlayer );
	BOOL CanApplyVimu( CPlayer* pApplyer, CPlayer* pAccepter );
	BOOL CanAcceptVimu( CPlayer* pApplyer, CPlayer* pAccepter );

	void NetworkMsgParse(BYTE Protocol,void* pMsg);
public:
	void Battle_VimuWithGold_Apply_Syn(void * pMsg);
	void Battle_VimuWithGold_Accept_Syn(void * pMsg);
	void Battle_Vimu_Apply_Syn(void* pMsg);
	void Battle_Vimu_Accept_Syn(void* pMsg);
	void Battle_Vimu_Reject_Syn(void* pMsg);
	void Battle_Vimu_Waiting_Cancel_Syn(void* pMsg);
	void Battle_Vimu_Error(void* pMsg);
};
#endif



