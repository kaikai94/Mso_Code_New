





// BailDialog.h: interface for the CBailDialog class.
//

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAILDIALOG_H__BEC1839F_4BA9_4149_898D_5983A18EF3C8__INCLUDED_)
#define AFX_BAILDIALOG_H__BEC1839F_4BA9_4149_898D_5983A18EF3C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./interface/cDialog.h"




class cEditBox;

class cTextArea;



class CBailDialog : public cDialog
{
	cEditBox*		m_pBailEdtBox;
	cTextArea*		m_pBailText;
	DWORD			m_BadFame;	
	BOOL			m_IsGoldClearFAME;
public:
	CBailDialog();
	virtual ~CBailDialog();
	virtual void SetActive( BOOL val );

	void Close();
	void Open();
	void Linking();

	void SetFame();
	void SetFame(DWORD BadFame);//[增加洗恶名快捷键调用][By:十里坡剑神][QQ:112582793][2018/4/26]
	void SetBadFrameSync();
	//[在线洗恶元宝判断设置][By:十里坡剑神][QQ:112582793][2017/11/22]
	void SetUseGoldClear(BOOL bVal){m_IsGoldClearFAME=bVal;}
};

#endif // !defined(AFX_BAILDIALOG_H__BEC1839F_4BA9_4149_898D_5983A18EF3C8__INCLUDED_)


