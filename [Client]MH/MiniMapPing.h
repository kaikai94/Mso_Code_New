#include "stdafx.h"
#include "./Interface/cDialog.h"
// 2014-11-12 小地图ping 开发!
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
	void Process();  // 2014-11-12 小地图快速Ping 进程
};
EXTERNGLOBALTON(CMiniMapPing);