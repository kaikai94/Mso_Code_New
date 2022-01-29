




#pragma once



#include "./interface/cDialog.h"
#include "VirtualItem.h"

class cIconDialog;
class CItem;


class CRareCreateDialog : public cDialog
{
	cIconDialog*	m_pIconDlg;
	CVirtualItem	m_VirtualItem;

	DWORD		m_ItemIdx;

	DWORD		m_ItemPos;
protected:
	cStatic * Rare_Info;
	cStatic * RareMax_Info;

	ITEM_RARE_Max_INFO RARE_Max_INFO[3];

	//[  ��ܯ��֭ȸԱ�  ] [By:�PVs�P] [QQ:137037798] [2021-05-26]	
	void SetRareInfo(ITEM_RARE_OPTION_INFO* RareInfo);


	//[  �[���̤j��  ] [By:�PVs�P] [QQ:137037798] [2021-05-26]	
	void LoadRARE_Max_INFO();


	//[  ����̤j�ȫH��  ] [By:�PVs�P] [QQ:137037798] [2021-05-26]	
	ITEM_RARE_Max_INFO GetRARE_Max_INFO(int Index);


	//[  �]�m�̤j��  ] [By:�PVs�P] [QQ:137037798] [2021-05-26]	
	void SetRARE_Max_INFO(int Index);

public:

	void Clear_RARE_INFO();

	CRareCreateDialog(void);


	~CRareCreateDialog(void);

	void Linking();
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);



	void ReleaseItem();

	void AddVirtualItem( CItem* pItem );


	void SetShopItemInfo(DWORD ItemIdx, DWORD ItemPos)		{	m_ItemIdx=ItemIdx;	m_ItemPos=ItemPos;		}

	void ItemRareCreateSyn();
	void ItemRareCreateAck();
	void ItemRareCreateNAck();

	BOOL Ishint();
};


