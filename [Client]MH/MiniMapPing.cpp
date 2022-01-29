#include "stdafx.h"
#include <stdio.h>
#include <MMSystem.h>
#include "MiniMapPing.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cFont.h"
#include "GameResourceManager.h"
#include "ChatManager.h"
#include "mhMap.h"
// 2014-11-12 
GLOBALTON(CMiniMapPing);
CMiniMapPing::CMiniMapPing()
{
	p_SendTime=0;
    p_RecvTime=0;
	p_LastTime=0;
}
CMiniMapPing::~CMiniMapPing()
{
}
void CMiniMapPing::SendPingPacket()
{
	MSG_DWORD	msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_PING_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData=1234; // 2016-02-28 ping°üÑéÖ¤
	NETWORK->Send( &msg, sizeof(msg) );
}
void CMiniMapPing::Process()
{
	DWORD Currtime=timeGetTime();
	if(Currtime-p_LastTime>36000)
	{
		SendPingPacket();
		p_SendTime=timeGetTime();
        p_LastTime=timeGetTime();
	}
}
void CMiniMapPing::SetPingShow()
{
	cDialog * m_pDlg	= WINDOWMGR->GetWindowForID(MNM_DIALOG);
	if(m_pDlg)
	{
		cStatic * m_pTitle = (cStatic*)m_pDlg->GetWindowForID( MNM_MAPNAME );
		if(m_pTitle)
		{
			p_RecvTime=timeGetTime();
			int   PingMs = (p_RecvTime-p_SendTime);
			char  TimerBuffer[64];
			PingMs = (PingMs/2 > 1 ? PingMs /2 : PingMs);
			sprintf(TimerBuffer,CHATMGR->GetChatMsg(2112),MAP->GetMapName(),gChannelNum+1,PingMs);
			if (PingMs <= 200)
			{
				m_pTitle->SetFGColor( RGBA_MAKE(0, 255, 0, 0) );
			}
			else if (PingMs >200 && PingMs <= 300)
			{
				m_pTitle->SetFGColor( RGBA_MAKE(255, 255, 0, 0) );
			}
			else if (PingMs > 300)
			{
				m_pTitle->SetFGColor( RGBA_MAKE(255, 0, 0, 0) );
			}
			m_pTitle->SetStaticText(TimerBuffer);
			m_pTitle->SetFGColor(RGBA_MAKE(0, 255, 60, 0));
			p_SendTime=0;
			p_RecvTime=0;
		}
	}
}
