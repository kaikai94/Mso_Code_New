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
#include "..\[CC]Header\GameResourceStruct.h"
#include "MHFile.h"
#include "Date.h"

#define KILLTOPMGR KillTopManager::GetInstance()

class CPlayer;

class KillTopManager
{
	static KillTopManager * g_Instance;	//[����ģʽʵ��][By:ʮ���½���][QQ:112582793][2018/4/22]
	
	Date m_NewTime;						//[����ת��������][By:ʮ���½���][QQ:112582793][2018/4/22]

	eSINGED m_day;						//[����ö��][By:ʮ���½���][QQ:112582793][2018/4/22]

	std::string m_dayChar;				//[���ڴ�ű���][By:ʮ���½���][QQ:112582793][2018/4/22]

	std::map<DWORD,SINGEDINFO> m_KillTopList;  //[�洢��һ�ɱ������Ϣ][By:ʮ���½���][QQ:112582793][2018/4/22]

	std::map<std::string,SINGEDGIFTINFO> m_TopListGiftInfo;  //[�����Ϣ][By:ʮ���½���][QQ:112582793][2018/4/22]
public:
	KillTopManager(void);
	~KillTopManager(void);
	GETINSTANCE(KillTopManager);
	//[���������Ϣ��map][By:ʮ���½���][QQ:112582793][2018/4/22]
	void AddKillTopListInfo(const DWORD& UserId,const SINGEDINFO& info);
	//[�Ƴ�������Ϣ][By:ʮ���½���][QQ:112582793][2018/4/22]
	void RemoveKillTopListInfo(const DWORD& UserId);
	//[�ж�����Ƿ����ȡ���][By:ʮ���½���][QQ:112582793][2018/4/22]
	BOOL CanGetGift(const DWORD& UserID,eSINGED Day);
	//[��ȡ��������ݿ�][By:ʮ���½���][QQ:112582793][2018/4/22]
	void SetTopListInfo(const DWORD& UserID,eSINGED Day);
	//[���ؽ�����Ʒ��Ϣ][By:ʮ���½���][QQ:112582793][2018/4/23]
	void LoadGiftInfo();
	//[���û�ɱ������Ϣ][By:ʮ���½���][QQ:112582793][2018/4/23]
	void RestKillTopListInfo();
	//[��������][By:ʮ���½���][QQ:112582793][2018/4/23]
	eSINGED GiftGiving(CPlayer* pPlayer,eSINGED Day);
	//[������Ϣ����][By:ʮ���½���][QQ:112582793][2018/4/23]
	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
};
