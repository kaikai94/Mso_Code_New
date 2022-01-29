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
//         佛祖保佑       永无BUG     永不修改				      //
////////////////////////////////////////////////////////////////////
#pragma once
class CPlayer;
class BillBoardManager
{
	TOP_LIST_INFO m_TopListInfo;
	WORD m_wTopListCount;
	WORD m_wTopListGuildCount;
	WORD m_wTopListKillerCount;
	SYSTEMTIME	UpdataTime;
public:
	GETINSTANCE(BillBoardManager);
	WORD GetTopListCount(){ return m_wTopListCount; }
	WORD GetTopListGuildCount(){ return m_wTopListGuildCount; }
	WORD GetTopListKillerCount(){ return m_wTopListKillerCount; }

	void SetTopList(const MSG_TOP_LIST_HERO* msg);

	void ResetTopList();

	TOPLISTINFOHERO * GetHeroList(){ return m_TopListInfo.HeroList; }
	TOPLISTINFOGUILD* GetGuildList(){ return m_TopListInfo.GuildList; }
	KILLPLAYERTOPLIST* GetKillList(){ return m_TopListInfo.KillerList; }
	TOPLISTINFOHERO*  GetHeroTop(){ return &m_TopListInfo.HeroList[0]; }
	TOPLISTINFOGUILD*  GetGuildTop(){ return &m_TopListInfo.GuildList[0]; }
	KILLPLAYERTOPLIST* GetKillTop(){ return &m_TopListInfo.KillerList[0]; }
	SYSTEMTIME GetUpdateTime(){ return UpdataTime; }
	//[排行标志设置][By:十里坡剑神][QQ:112582793][2018/2/25]
	void SetHeroTopListFlg(CPlayer* pPlayer);
	bool IsCanUpdate();
	BillBoardManager(void);
	~BillBoardManager(void);
};
#define sBOARDMGR BillBoardManager::GetInstance()