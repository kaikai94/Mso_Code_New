
#include "./interface/cDialog.h"
#include "itemManager.h"
#include "VirtualItem.h"
#include "Item.h"
#include "./interface/cButton.h"
#include "./interface/cStatic.h"
class CItemUnLockDialog : public cDialog
{
	cDialog * pDlg; 

	cIconDialog*	m_pIconDlg;

	CVirtualItem	m_VirtualItem;

	CItem *         m_LockItem;
	eUNLOCKTYPE	m_UnLockType;
	cButton* m_OkBtn;
	cStatic* m_CapText;
	cStatic* m_Tip1;
	cStatic* m_Tip2;
public:

	 CItemUnLockDialog();

	 virtual ~CItemUnLockDialog();
	
	void Linking();

	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	void AddVirtualItem( CItem* pItem );

	BOOL VirtualItemIsNull();

	void SetLockItem(CItem * pItem);

	CItem * GetLockItem();

	void SendSynMsg();

	void Release();

	void SetLockType(eUNLOCKTYPE type);
};