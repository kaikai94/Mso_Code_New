
#include "./interface/cDialog.h"
#include "itemManager.h"
#include "VirtualItem.h"
#include "Item.h"

class CItemLockDialog : public cDialog
{
	cDialog * pDlg; 

	cIconDialog*	m_pIconDlg;

	CVirtualItem	m_VirtualItem;

	CItem *         m_LockItem;

public:

	 CItemLockDialog();

	 virtual ~CItemLockDialog();

	void Linking();

	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	void AddVirtualItem( CItem* pItem );

	BOOL VirtualItemIsNull();

	void SetLockItem(CItem * pItem);

	CItem * GetLockItem();

	void SendSynMsg();

	void Release();


};