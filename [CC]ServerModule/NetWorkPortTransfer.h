#define SERVERNAME "ZXPortMap"
#define VERSION "v1.1"

#define MAXBUFSIZE 8192
#define ADDRSIZE 32


struct SOCKINFO
{
    SOCKET ClientSock;
    SOCKET ServerSock;
};
struct ADDRESS
{
    char szIP[ADDRSIZE];
    WORD wPort;
    SOCKET s;
};


////////A simple class of stack operator.  code start.....
template<typename T>
class STACK
{
#define MAXSTACK 1024*2

private:
    int top;
    T Data[MAXSTACK];
public:

STACK()
{
    top = -1;
}

bool IsEmpty()
{
    return top < 0;
}

bool IsFull()
{
    return top >= MAXSTACK;
}

bool Push(T data)
{
    if(IsFull())
        return false;
    top++;
    Data[top] = data;
    return true;
}

T Pop()
{
    return Data[top--];
}
   
};/////////////////////stack    end
//////////////////////////////////////////////////////////////////Transfer some Parameters
template<typename X, typename Y>
class TransferParam
{
public:
    X GlobalData;
    STACK<Y> LocalData;
public:
    TransferParam();
    virtual ~TransferParam();
    bool Push(Y data);
    Y Pop();

};


template<typename X, typename Y>
TransferParam<X, Y>::TransferParam()
{
    memset(this, 0, sizeof(TransferParam));
}

template<typename X, typename Y>
TransferParam<X, Y>::~TransferParam()
{

}

template<typename X, typename Y>
bool TransferParam<X, Y>::Push(Y data)
{
    return LocalData.Push(data);
}

template<typename X, typename Y>
Y TransferParam<X, Y>::Pop()
{
    return LocalData.Pop(data);
}

char *chstohex ( char* chs );

BOOL PortTransfer_1(WORD ListenPort, char *szIP, WORD wPort);

BOOL PortTransfer_2(char *szCtrlIP, WORD wCtrlPort, char *szIP, WORD wPort);

BOOL PortTransfer_3(WORD wCtrlPort, WORD wServerPort);

