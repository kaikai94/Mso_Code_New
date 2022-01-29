#pragma once
#include "cTabDialog.h"
#include "cStatic.h"
#include "cButton.h"
#include <map>
#include "GoldDealItem.h"
#include "IndexGenerator.h"
class CGoldShopSubDialog;
class cImage;

//class GoldDealItem;
enum IMAGETYPE
{
	eCurrency,
	eMallMoney,
	eGoldMoney,
	eMaxType,
};
class CGoldShopMainDailog : public cTabDialog
{
	//[����ͼ��][By:ʮ���½���][QQ:112582793][2018/9/30]
	cImage   m_MoneyImage[eMaxType];
	//[������ؼ�����][By:ʮ���½���][QQ:112582793][2018/9/30]
	cStatic* m_GoldMoney;
	cStatic* m_MallMoney;
	cStatic* m_pItemPrice[15];
	cStatic* m_pItemName[15];
	cStatic* m_pItemTip[15];
	cButton* m_pBuyBtton[15];
	CIndexGenerator m_IconIndexCreator;
	//[�ӽ���][By:ʮ���½���][QQ:112582793][2018/9/30]
	CGoldShopSubDialog* m_SubDlg[11];	
	GoldDealItem * m_BuyItem;
	DWORD m_dwBuyPrice;
public:
	CGoldShopMainDailog(void);
	~CGoldShopMainDailog(void);
	virtual void Add(cWindow * window);
	virtual void Linking();
	//[��ʼ������ͼ��][By:ʮ���½���][QQ:112582793][2018/9/30]
	void InitImage();
	//[�����Ϣ��Ӧ����][By:ʮ���½���][QQ:112582793][2018/9/30]
	void OnActionEvent(LONG lId, void * p, DWORD we);
	//[���̳ǽ���][By:ʮ���½���][QQ:112582793][2018/9/30]
	void SetActive(BOOL val);
	//[���ý���ؼ���Ϣ][By:ʮ���½���][QQ:112582793][2018/9/30]
	void SetStatic();
	void SetGoldMoney(MONEYTYPE val);
	void SetMallMoney(MONEYTYPE val);
	//[����ѡ����Ʒ][By:ʮ���½���][QQ:112582793][2018/9/30]
	void OnSelectItem();
	//[�����߼�����][By:ʮ���½���][QQ:112582793][2018/9/30]
	void FakeBuyItem(LONG x, LONG y);
	static void OnFakeBuyItem( LONG iId, void* p, DWORD param1, void * vData1, void* vData2 );  // ȷ�Ϲ�����Ϣ��
	static void OnCancelBuyItem( LONG iId, void* p, DWORD param1, void * vData1, void* vData2 ); // ȡ��������Ϣ
	void SendBuyMsg();
	void ClearBuyMsgInfo();
	//[��ӳ�����Ʒ��Ϣ][By:ʮ���½���][QQ:112582793][2018/9/30]
	void AddGoldShopItem(GOLD_SHOP_ITEM_INFO& pInfo);
	//[������Ʒ��Ϣ][By:ʮ���½���][QQ:112582793][2018/9/30]
	void ClearShopItem();
	DWORD GetBuyItemPrice()
	{
		if(m_dwBuyPrice==0)
			return 0;
		else
			return m_dwBuyPrice;
	}
	//[�����ݴ����ݰ�][By:ʮ���½���][QQ:112582793][2018/9/30]
	MSG_ITEM_BUY_SYN	m_buyMsg; 
protected:
	BOOL	m_bShow;
	int		m_nCurMainIndex;
	int		m_nCurSubIndex;
	int		m_nCurItemIndex;
	DWORD   m_DealIdx;
};

