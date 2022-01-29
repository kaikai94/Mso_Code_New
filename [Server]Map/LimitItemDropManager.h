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
//         ���汣��       ����BUG     �����޸�                    //
////////////////////////////////////////////////////////////////////
#pragma once
#include <map>
#include <string>
#define LIMITITEMDROPMGR LimitItemDropManager::GetInstance()
struct  LIMITDROPINFO 
{
	WORD		wCount;			//[������Ʒ����][By:ʮ���½���][QQ:112582793][2018/5/25]
	DWORD		dwItemIdx;		//[��Ʒ���][By:ʮ���½���][QQ:112582793][2018/5/25]
	std::string strItemName;	//[��Ʒ����][By:ʮ���½���][QQ:112582793][2018/5/25]
	DWORD		dwDropTimes;	//[�������][By:ʮ���½���][QQ:112582793][2018/5/25]
	DWORD       dwCurDropTimes;	//[��ǰ��ɱ����][By:ʮ���½���][QQ:112582793][2018/5/25]
};
class LimitItemDropManager
{
	std::map<WORD,LIMITDROPINFO*> m_LimitItemDropInfo;
	LimitItemDropManager(void);

	//[ϡ�в��ϵ������][By:ʮ���½���][QQ:112582793][2018/5/12]
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
