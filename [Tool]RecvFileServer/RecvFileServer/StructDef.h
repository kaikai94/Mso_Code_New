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
