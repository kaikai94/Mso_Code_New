



#ifndef _CHARACTERMAKE_MANAGER_
#define _CHARACTERMAKE_MANAGER_

#include "PtrList.h"
#include "MHfile.h"

class cStatic;

enum CHAR_M
{
//	CE_JOB,
//#ifdef _JAPAN_LOCAL_
//	CE_ATTRIBUTE,	//SW050811 ���߰�
//#endif
	CE_SEX, 
	CE_MHAIR, 
	CE_WMHAIR, 
	CE_MFACE, 
	CE_WMFACE, 
	CE_WEAR, 
	CE_BOOT, 

	CE_WEAPON,
	CE_AREA,

	CE_MAX

};

enum{CM_PREV, CM_NEXT};


struct CM_OPTION
{
	char		strName[20];
	DWORD		dwData;
	DWORD		dwHelperData;
//	void*		pDescCombo;
	cStatic*	pDescStatic;
	VECTOR2		vTextPos;
};



#define CHARMAKEMGR 	USINGTON(cCharMakeManager)
#define PATH_UOPTION		"./Resource/Client/CharMake_SelectOption.bin"
#define	CHARROTATE_RATE		0.01f

class CPlayer;
class cGuageBar;
class cStatic;
struct sCHARMAKECFG
{
	VECTOR3	vCreatePos;
	WORD	wMapNum;
	VECTOR3 vCameraPos;
	WORD	wCameraNum; 
	VECTOR3 vAnglePos;
	VECTOR3 vEyePos;
	VECTOR3 vRotate;
	WORD	wCurCamera;
	WORD	wCameraMode;
	VECTOR3 vCameraEye;
	VECTOR3	vCameraTo;
	VECTOR3 vCameraUp;
	float	fZoom[2];
	float	fRotPitch[2];
	float	fCameraAngleX;
	VECTOR3 vAxis;
	float	fMonveDistance;
	float	fRotateAngle;
	float	fPI[2];
};
class cCharMakeManager
{
protected:
	CPlayer*				m_pNewPlayer;				
	CHARACTERMAKEINFO		m_CharMakeInfo;

	cPtrList				m_UserSelectOption[CE_MAX];		


	DWORD					m_dwCurIndex[CE_MAX];		

	FLOAT					m_fAngle;
	FLOAT					m_fOldX;
	
	BOOL					m_bInit;

	cGuageBar*				m_pHeight;
	cGuageBar*				m_pWidth;
public:

	cCharMakeManager();
	virtual ~cCharMakeManager();
	sCHARMAKECFG sCharMake;
	sCHARMAKECFG & GetMakeCfg(){return sCharMake;}
	void Init();
	void Release();
	void InitConfig();

	void CreateNewCharacter(VECTOR3* pv3Pos);

	
	BOOL ReplaceCharMakeInfo( int idx, WORD wValue );
	void ComboBoxResetAll();

	void ListBoxResetAll();

	//SW050812
	void RotateSelection( int idx, int Dir );


	CPlayer* GetNewPlayer() { return m_pNewPlayer; }

	CHARACTERMAKEINFO* GetCharacterMakeInfo() { return &m_CharMakeInfo; }

	void NewCharacterRotate( int x );
	
	void ApplyScale();

private:
	void LoadUserSelectOption();		// È­?Ï?» ?Ð¾î¿Â´Ù.
//	void SetUserSelectOption();			// ¸®½ºÆ®¿¡ ¼ÂÆÃÇÑ´Ù.
	void SetUserSelections();
};
EXTERNGLOBALTON(cCharMakeManager)
#endif // _CHARACTERMAKE_MANAGER_


