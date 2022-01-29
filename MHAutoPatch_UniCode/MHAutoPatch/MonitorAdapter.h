#include <atlstr.h>
#include <vector>
#include <WinDef.h>
#include <tchar.h>
using std::vector;

using namespace std;

#define MAX_MONITOR_NAME 256

static std::vector<HMONITOR> g_hMonitorGroup;  

// ��ʾ��ģʽ��Ϣ
typedef struct MonitorModeInfo_t
{
	unsigned int m_nWidth;			    
	unsigned int m_nHeight;

	MonitorModeInfo_t(int nWidth, int nHeight) : m_nWidth(nWidth), m_nHeight(nHeight) {}
}MonitorModeInfo;

// ��ʾ����Ϣ
struct MonitorInfo
{
	TCHAR szDevice[MAX_MONITOR_NAME];					// ��ʾ������
	std::vector<MonitorModeInfo> m_vecModeInfo;			// ��ǰ���Ƶ���ʾ��֧�ֵķֱ���ģʽ
};
struct info
{
	unsigned int m_nWidth;			    
	unsigned int m_nHeight;
};
typedef std::vector<MonitorInfo> VEC_MONITORMODE_INFO;  // ���е���ʾ����Ϣ	


class MonitorAdapter
{
public:
	MonitorAdapter();
	~MonitorAdapter();
	std::vector<info> m_info;
	// �ص�����
	static int CALLBACK MonitorEnumProc(HMONITOR hMonitor, 
		HDC hdc,
		LPRECT lpRMonitor,
		LPARAM dwData);
	std::vector<info>* GetInfo(){return &m_info;}
	// �õ�������ʾ��������
	void GetAllMonitorName(VEC_MONITORMODE_INFO& m_vecMonitorListInfo);

	// �õ�������ʾ����ģʽ
	void GetAllDisplayMode(VEC_MONITORMODE_INFO& m_vecMonitorListInfo);

	//�õ���Ļ��ǰ�ֱ���
	void GetCurrentReselotion(int& nWidth,int& nHeight,int& nFreq,int& nBits);

	//������ĻIDȡ��ȡ��Ļ�Ķ�Ӧ�ֱ���
	void GetCurrentReselotion(LPCWSTR lpszDeviceName, int& nWidth, int& nHeight, int& nFreq, int& nBits);

	//�޸ķֱ���
	int ChangMonitorReselotion(HMONITOR hMonitor,const int nWidth,const int nHight,const int nFre,const int nColorBits);
};