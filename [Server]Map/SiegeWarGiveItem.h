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
#include <map> 
#include "..\[CC]Header\GameResourceStruct.h"
#define SIEGEWARGIVEITEM SiegeWarGiveItem::GetInstance()
class CPlayer;
class SiegeWarGiveItem
{
	std::multimap<WORD, SIEGEWARGIVEITEMINFO> m_GiveItemInfo;
	typedef std::multimap<WORD, SIEGEWARGIVEITEMINFO>::iterator Gift_multimap;
public:
	SiegeWarGiveItem(void);
	~SiegeWarGiveItem(void);
	GETINSTANCE(SiegeWarGiveItem);
	void LoadGiveItemInfo();
	void SendItem(CPlayer* pPlayer,eSIEGEWARPLAYER type);
	void SendItem(CPlayer* pPlayer, const SIEGEWARGIVEITEMINFO & info);
};
