#include "stdafx.h"
#include "./Interface/cDialog.h"
// 2014-11-12 С��ͼping ����!
#define MINIPINGMGR	USINGTON(CMiniMapPing)
class CMiniMapPing : public cDialog
{
	DWORD p_SendTime;
	DWORD p_RecvTime;
	DWORD p_LastTime;
public:
	CMiniMapPing();
    virtual	~CMiniMapPing();
    void SendPingPacket();
	void SetPingShow();
	void Process();  // 2014-11-12 С��ͼ����Ping ����
};
EXTERNGLOBALTON(CMiniMapPing);