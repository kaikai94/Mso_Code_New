



#pragma once
#include "interface\cdialog.h"

//�����ʴ뼱��â. ���Ŀ� or ���ϻ�
class CGuildJoinDialog :	public cDialog
{
public:
	CGuildJoinDialog(void);
	virtual ~CGuildJoinDialog(void);

	void Linking();
	void OnActionEvent(LONG lId, void* p, DWORD we);

};


