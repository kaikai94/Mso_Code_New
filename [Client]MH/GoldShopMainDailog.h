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
	//[货币图标][By:十里坡剑神][QQ:112582793][2018/9/30]
	cImage   m_MoneyImage[eMaxType];
	//[主界面控件变量][By:十里坡剑神][QQ:112582793][2018/9/30]
	cStatic* m_GoldMoney;
	cStatic* m_MallMoney;
	cStatic* m_pItemPrice[15];
	cStatic* m_pItemName[15];
	cStatic* m_pItemTip[15];
	cButton* m_pBuyBtton[15];
	CIndexGenerator m_IconIndexCreator;
	//[子界面][By:十里坡剑神][QQ:112582793][2018/9/30]
	CGoldShopSubDialog* m_SubDlg[11];	
	GoldDealItem * m_BuyItem;
	DWORD m_dwBuyPrice;
public:
	CGoldShopMainDailog(void);
	~CGoldShopMainDailog(void);
	virtual void Add(cWindow * window);
	virtual void Linking();
	//[初始化货币图标][By:十里坡剑神][QQ:112582793][2018/9/30]
	void InitImage();
	//[鼠标消息响应函数][By:十里坡剑神][QQ:112582793][2018/9/30]
	void OnActionEvent(LONG lId, void * p, DWORD we);
	//[打开商城界面][By:十里坡剑神][QQ:112582793][2018/9/30]
	void SetActive(BOOL val);
	//[设置界面控件信息][By:十里坡剑神][QQ:112582793][2018/9/30]
	void SetStatic();
	void SetGoldMoney(MONEYTYPE val);
	void SetMallMoney(MONEYTYPE val);
	//[购买选中商品][By:十里坡剑神][QQ:112582793][2018/9/30]
	void OnSelectItem();
	//[购买逻辑处理][By:十里坡剑神][QQ:112582793][2018/9/30]
	void FakeBuyItem(LONG x, LONG y);
	static void OnFakeBuyItem( LONG iId, void* p, DWORD param1, void * vData1, void* vData2 );  // 确认购买消息顶
	static void OnCancelBuyItem( LONG iId, void* p, DWORD param1, void * vData1, void* vData2 ); // 取消购买消息
	void SendBuyMsg();
	void ClearBuyMsgInfo();
	//[添加出售物品信息][By:十里坡剑神][QQ:112582793][2018/9/30]
	void AddGoldShopItem(GOLD_SHOP_ITEM_INFO& pInfo);
	//[清理物品信息][By:十里坡剑神][QQ:112582793][2018/9/30]
	void ClearShopItem();
	DWORD GetBuyItemPrice()
	{
		if(m_dwBuyPrice==0)
			return 0;
		else
			return m_dwBuyPrice;
	}
	//[购买暂存数据包][By:十里坡剑神][QQ:112582793][2018/9/30]
	MSG_ITEM_BUY_SYN	m_buyMsg; 
protected:
	BOOL	m_bShow;
	int		m_nCurMainIndex;
	int		m_nCurSubIndex;
	int		m_nCurItemIndex;
	DWORD   m_DealIdx;
};

