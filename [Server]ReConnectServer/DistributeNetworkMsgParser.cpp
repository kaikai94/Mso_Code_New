#include "stdafx.h"
#include "DistributeNetworkMsgParser.h"
#include "ServerTable.h"
#include "Network.h"
#include "UserTable.h"
#include "BootManager.h"
#include "ServerListManager.h"
#include "cFileManager.h"
extern int g_nServerSetNum;
extern HWND g_hWnd;
DWORD g_dwMaxUser = 10000;



void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* TempPmsg = (MSGROOT*)pMsg;
	switch(TempPmsg->Protocol)
	{
	case MP_AUTOPATH_GETVER_SYN:
		{
			if(!FILEMGR->SendVerFile(dwConnectionIndex))
			{
				MSGROOT msg;
				msg.Category = MP_AUTOPATH;
				msg.Protocol = MP_AUTOPATH_GETVER_NACK;
				g_Network.Send2User(dwConnectionIndex,(char*)&msg,sizeof(msg));
				g_Console.LOGEX(4,RGB(255,0,0),"版本文件发送失败");
			}
		}
		break;
	case MP_AUTOPATH_GETFILE_SYN:
		{
			MSGFILEINFO* pmsg = (MSGFILEINFO*)pMsg;
			if(!FILEMGR->SendFile(dwConnectionIndex,pmsg->strFileName))
			{
				MSGFILEINFO msg;
				msg.Category = MP_AUTOPATH;
				msg.Protocol = MP_AUTOPATH_GETFILE_NACK;
				strcpy_s(msg.strFileName,MAX_PATH,pmsg->strFileName);
				g_Network.Send2User(dwConnectionIndex,(char*)&msg,sizeof(msg));
				g_Console.LOGEX(4,RGB(255,0,0),"文件发送失败:[%s]",pmsg->strFileName);
			}
		}
		break;
	}
}
