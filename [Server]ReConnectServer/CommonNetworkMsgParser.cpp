





#include "stdafx.h"

#include "CommonNetworkMsgParser.h"




//---------------------------------------------------------------------------------------------
//don't changed -------------------------------------------------------------------------------
MSGPARSER g_pServerMsgParser[MP_MAX];
MSGPARSER g_pUserMsgParser[MP_MAX];
//---------------------------------------------------------------------------------------------

void ErrorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
}
