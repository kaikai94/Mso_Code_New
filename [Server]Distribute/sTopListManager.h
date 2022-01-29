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
//         佛祖保佑       永无BUG     永不修改					  //
////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include "Date.h"
#define TOPLISTMGR sTopListManager::GetInstance()
class sTopListManager
{
	SYSTEMTIME m_tBoardTime;
	WORD	m_BoardUpdate;
	BOOL	m_bCanSend;

	static sTopListManager* g_Instance;
	std::string m_dayChar;
	Date m_NewTime;
	//[排行榜查询][By:十里坡剑神][QQ:112582793][2017/11/23]

	TOP_LIST_INFO m_TopListInfo;
	WORD m_wTopListCount;
	WORD m_wTopListGuildCount;
	WORD m_wTopListKillerCount;

public:
	sTopListManager(void);
	~sTopListManager(void);
	static sTopListManager * GetInstance();
	void SetTopList(TOPLISTINFOHERO& TopList);
	void SetTopList(TOPLISTINFOGUILD& TopList);
	void SetTopList(KILLPLAYERTOPLIST& TopList);

	void SendTopList(DWORD dwConnectionIndex);
	void SendTopListToAG(DWORD dwConnectionIndex, DWORD dwClientConnIndex);
	void TopListPorcess();
	void ReleseTopList();
	void ReloadTopList();
};
