////////////////////////////////////////////////////////////////////
//                          _ooOoo_                               //
//                         o8888888o                              //
//                         88" . "88                              //
//                         (| ^_^ |)                              //
//                         O\  =  /O                              //
//                      ____/`---'\____                           //
//                    .'  \\|     |//  `.                         //
//                   /  \\|||  :  |||//  \                        //
//                  /  _||||| -:- |||||-  \                       //
//                  |   | \\\  -  /// |   |                       //
//                  | \_|  ''\---/''  |   |                       //
//                  \  .-\__  `-`  ___/-. /                       //
//                ___`. .'  /--.--\  `. . ___                     //
//             ."" '<  `.___\_<|>_/___.'  >'"".                   //
//            | | :  `- \`.;`\ _ /`;.`/ - ` : | |                 //
//            \  \ `-.   \_ __\ /__ _/   .-` /  /                 //
//     ========`-.____`-.___\_____/___.-`____.-'========          //
//                          `=---='                               //
//     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^         //
//         佛祖保佑       永无BUG     永不修改                    //
////////////////////////////////////////////////////////////////////
#pragma once
#include <map>
#include <string>
#define LIMITITEMDROPMGR LimitItemDropManager::GetInstance()
struct  LIMITDROPINFO 
{
	WORD		wCount;			//[掉落物品数量][By:十里坡剑神][QQ:112582793][2018/5/25]
	DWORD		dwItemIdx;		//[物品编号][By:十里坡剑神][QQ:112582793][2018/5/25]
	std::string strItemName;	//[物品名称][By:十里坡剑神][QQ:112582793][2018/5/25]
	DWORD		dwDropTimes;	//[掉落次数][By:十里坡剑神][QQ:112582793][2018/5/25]
	DWORD       dwCurDropTimes;	//[当前击杀次数][By:十里坡剑神][QQ:112582793][2018/5/25]
};
class LimitItemDropManager
{
	std::map<WORD,LIMITDROPINFO*> m_LimitItemDropInfo;
	LimitItemDropManager(void);

	//[稀有材料掉落控制][By:十里坡剑神][QQ:112582793][2018/5/12]
	std::map<DWORD,WORD> m_CtrlDropItemList;
	//WORD	m_DropItemList;
	std::map<std::string,WORD> m_DropItemList;
	int nDropItemCount;
public:
	
	~LimitItemDropManager(void);
	GETINSTANCE(LimitItemDropManager);
	void LoadDropItemInfo();
	void DropLimitItem(WORD wMonsterKind,WORD& wDropItemIdx,DWORD dwPlayerID);
	void ReLoadDropItemInfo();
	void OutPutLimitItemDropTimeInfo();
	void Release();
	void AddKillTime(WORD MonsterKind);
	void AddCtrlDropItem(WORD wItemIdx,DWORD dwCharIdx);
	void AddCtrlRandDropItem(WORD wItemIdx);
	void DelCtrlDropItem(DWORD dwCharIdx);
	void DelCtrlDropItem(std::string str);
	void ClearCtrlDropItem(BYTE type);
	BOOL CheckCtrlDropItem(DWORD dwCharIdx,WORD & wItemIdx);
	void WriteItemDropLog(char* pMsg);
	void ShowDropItemList();
};
