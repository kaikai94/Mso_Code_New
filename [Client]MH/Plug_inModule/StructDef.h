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

struct Msg
{
	int op;
};

struct SendMsg:public Msg
{
	char file_path[MAX_PATH];
	char PlayerName[16];
	int file_count;
};

struct FileInfo
{
	SOCKADDR_IN sockAddr;
	char file_path[MAX_PATH];
	char PlayerName[16];
	int file_count;
	bool GetFromTem;
};

struct RecPrarm
{
	RecPrarm()
	{
		msg=NULL;
		socket=INVALID_SOCKET;
	}
	~RecPrarm()
	{
		if(msg!=NULL)
		{
			delete msg;
		}
	}
	SendMsg* msg;
	SOCKET socket;
};