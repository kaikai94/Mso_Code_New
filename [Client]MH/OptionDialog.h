#ifndef _OPTIONDIALOG_H
#define _OPTIONDIALOG_H



#include "./Interface/cTabDialog.h"
#include "OptionManager.h"

class CSkillInfo;
class COptionDialog : public cTabDialog
{
protected:
	sGAMEOPTION		m_GameOption;
	LONG ControlId;
protected:
	void UpdateData( BOOL bSave );
public:
	COptionDialog();
	virtual ~COptionDialog();
	virtual void Add(cWindow * window);
	virtual void SetActive(BOOL val);
	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void DisableGraphicTab(BOOL bDisable);
	void CheckHideSet();
	int GetEffectSnow() { return m_GameOption.nEffectSnow;}
	void UpdataSreenAI(BOOL Val);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	BOOL checkNotMainSkill(DWORD idx);
	void AddSkillListItem(CSkillInfo* SkillInfo);
	void AddClearItemList(cIcon * icon);
	void AddPortectItem(cIcon * icon);
	void UpdataSreenPortect();
	void UpdataSreenSet();
	void UpdataSreenClear();
	void UpdataSreenSkill();
	void GetSetProtect();
	void GetSet();
	void GetClear();
	void GetSkill();
	void GetTimerSet();
	void UpdataTimerSet();

	void DeleteClearItem();
	void DeleteSkillItem();
	void DeleteLifeItem();
	void DeleteNaeRyukItem();
	void DeleteShieldItem();
	void DeleteEtcItem();

	
	void RestSet();
};

#endif


