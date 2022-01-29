#include "stdafx.h"
#include "MarryManager.h"
#include "ObjectManager.h"
#include "WindowIdEnum.h"
#include "./Interface/cWindowManager.h"
#include "ChatManager.h"
#include "cMsgBox.h"
#include "MHMap.h"
#include "Item.h"

CMarryManager::CMarryManager(void)
{
	m_MarryIDX = 0;
	m_dwTargetMapNum = 0;	
	m_dwTargetPos = 0;		
	m_nChannelIdx = -1;
}

CMarryManager::~CMarryManager(void)
{
}
//[夫妻下线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
void CMarryManager::SendLoginOutNotice()
{
	if(m_MarryIDX==0) return;

	MSG_NAME_DWORD  msg;
	msg.Category = MP_MARRY;
	msg.Protocol = MP_MARRY_NOTICE_LOGINOUT_SYN;
	msg.dwObjectID = m_MarryIDX;
	SafeStrCpy(msg.Name,HERO->GetObjectName(),17);
	NETWORK->Send(&msg,sizeof(msg));
	m_MarryIDX =0;
}
//[夫妻上线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
void CMarryManager::LoginNotifyForMarry()
{
	MSGBASE msg;
	msg.Category = MP_MARRY;
	msg.Protocol = MP_MARRY_NOTICE_LOGIN_SYN;
	msg.dwObjectID = HEROID;
	NETWORK->Send(&msg,sizeof(msg));
}
DWORD CMarryManager::GetTargerPos()
{ 
	if (m_dwTargetPos == 0)
		return m_dwTargetPos;
	stMOVEPOINT TempPos;
	TempPos.value = m_dwTargetPos;

	WORD PosX, PosZ;
	TempPos.GetMovePoint(PosX, PosZ);
	int temp;
	temp = rand() % 700 - 350;
	PosX += temp;
	temp = rand() % 700 - 350;
	PosZ += temp;
	TempPos.SetMovePoint(PosX, PosZ);

	return TempPos.value;
}
//[发送传送请求][By:十里坡剑神][QQ:112582793][2018/9/10]
void CMarryManager::SendBeckonMsg(CItem * pItem)
{
	if(m_MarryIDX==0)
	{
		if(HERO->GetGender()==0)
			CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2312));
		else
			CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2313));
		return;
	}
	MSG_DWORD4 msg;
	SetProtocol( &msg, MP_MARRY, MP_MARRY_BECKON_SYN );
	msg.dwObjectID = HEROID;
	msg.dwData1 = m_MarryIDX;
	msg.dwData2 = MAP->GetMapNum();
	VECTOR3 pos = HERO->GetCurPosition();
	stMOVEPOINT stMovePoint;
	stMovePoint.SetMovePoint((WORD)pos.x, (WORD)pos.z);
	msg.dwData3 = stMovePoint.value;
	msg.dwData4 = gChannelNum;

	NETWORK->Send(&msg, sizeof(MSG_DWORD4));
}
//[网络消息解析][By:十里坡剑神][QQ:112582793][2018/9/10]
void CMarryManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_MARRY_ADD_INVITE:
		{
			MSG_MARRY_ADDMSG* pmsg = (MSG_MARRY_ADDMSG*)pMsg;
			HERO->m_mary_msg = *pmsg;
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MARY_FT_ACCEPT_BT, MBT_YESNO, CHATMGR->GetChatMsg(1817),pmsg->Name );
		}
		break;
	case MP_MARRY_ADD_ACK:
		{
			MSG_MARRY_ADDMSG* pmsg = (MSG_MARRY_ADDMSG*)pMsg;

			HERO->SetMarryName(pmsg->Name);

			OBJECTMGR->ApplyOverInfoOptionToAll();

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1806 ) );
		}
		break;
	case MP_MARRY_ADD_DENY_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1852 ) );
		}
		break;
	case MP_MARRY_ADD_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if(pmsg->dwData2==1)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1804));

				return ;
			}
			if(pmsg->dwData2==2)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1849));

				return ;
			}
			if(pmsg->dwData2==3)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1827),pmsg->dwData1);

				return ;
			}
			if(pmsg->dwData2==4)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1828),pmsg->dwData1);

				return ;
			}
			if(pmsg->dwData2==5)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1829),pmsg->dwData1);

				return ;
			}
			if(pmsg->dwData2==6)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1801));

				return ;
			}
			if(pmsg->dwData2==7)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1802));

				return ;
			}
			if(pmsg->dwData2==8)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1803),pmsg->dwData1);

				return ;
			}
			if(pmsg->dwData2==9)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1804));

				return ;
			}

		}
		break;
	case MP_MARRY_DEL_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if(pmsg->dwData2==0)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1807));

				return ;
			}

			if(pmsg->dwData2==1)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2083));

				return ;
			}
			if(pmsg->dwData2==2)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2085),pmsg->dwData1);

				return ;
			}
			if(pmsg->dwData2==3)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2084),pmsg->dwData1);

				return ;
			}
		}
		break;
	case MP_MARRY_DEL_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1808 ) );
			HERO->SetMarryName("0");
			MARRYMGR->SetMarryIDX(0);
		}
		break;
	case MP_MARRY_NOTICE_LOGIN_ACK:
		{//[夫妻上线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
			//[来自对方的消息][By:十里坡剑神][QQ:112582793][2018/9/10]
			if(MARRYMGR->GetMarryIDX()!=0) return;
			MSG_NAME_DWORD * pmsg = (MSG_NAME_DWORD*)pMsg;
			MARRYMGR->SetMarryIDX(pmsg->dwData);
			if(HERO)
			{
				if(HERO->GetGender()==0)
					CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2314),pmsg->Name);
				else
					CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2315),pmsg->Name);
			}
		}
		break;
	case MP_MARRY_NOTICE_LOGIN_SELF:
		{//[夫妻上线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
			//[来自自己的消息][By:十里坡剑神][QQ:112582793][2018/9/10]
			if(MARRYMGR->GetMarryIDX()!=0) return;
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			MARRYMGR->SetMarryIDX(pmsg->dwObjectID);
		}
		break;
	case MP_MARRY_NOTICE_LOGINOUT_TO_CLIENT:
		{//[夫妻下线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
			if(MARRYMGR->GetMarryIDX()==0) return;
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			if(HERO)
			{
				if(HERO->GetGender()==0)
					CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2316),pmsg->Name);
				else
					CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2317),pmsg->Name);
			}
			MARRYMGR->SetMarryIDX(0);
		}
		break;
	case MP_MARRY_BECKON_RECVER_SYN:
		{//[夫妻传送卷][By:十里坡剑神][QQ:112582793][2018/9/10]
		 //[收到传送请求][By:十里坡剑神][QQ:112582793][2018/9/10]
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
			if( !pmsg )
				return;
			m_dwTargetMapNum  = pmsg->dwData2;	
			m_dwTargetPos = pmsg->dwData3;			
			m_nChannelIdx = pmsg->dwData4;	
			if(HERO)
			{
				char buf[64] = { 0, };

				if(HERO->GetGender()==0)
					sprintf(buf, CHATMGR->GetChatMsg(2310), MAP->GetMapName((MAPTYPE)pmsg->dwData2));
				else
					sprintf(buf, CHATMGR->GetChatMsg(2311), MAP->GetMapName((MAPTYPE)pmsg->dwData2));
				WINDOWMGR->MsgBox( MSBOX_MARRY_ACCEPT, MBT_YESNO, buf );
			}
		}
		break;
	case MP_MARRY_RECKON_NO_TO_CLIENT:
		{
			CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2318));
		}
		break;
	}
}

