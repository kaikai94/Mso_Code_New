#include "stdafx.h"
#include "MonitorAdapter.h"

MonitorAdapter::MonitorAdapter(void)
{
}

MonitorAdapter::~MonitorAdapter(void)
{
}


int CALLBACK MonitorAdapter::MonitorEnumProc(HMONITOR hMonitor, 
											 HDC hdc,
											 LPRECT lpRMonitor,
											 LPARAM dwData)
{
	g_hMonitorGroup.push_back(hMonitor);
	return 1;
}


// 得到所有显示器的名称
void MonitorAdapter::GetAllMonitorName(VEC_MONITORMODE_INFO& vecMonitorListInfo)
{
	g_hMonitorGroup.clear();
	::EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);

	//vector<HMONITOR>::iterator ithMoniter = g_hMonitorGroup.begin();
	for(unsigned int i = 0; i < g_hMonitorGroup.size();i++)
	{
		MONITORINFOEX mixTemp;
		memset(&mixTemp, 0, sizeof(MONITORINFOEX));
		mixTemp.cbSize = sizeof(MONITORINFOEX);

		GetMonitorInfo(g_hMonitorGroup[i], &mixTemp);
		VEC_MONITORMODE_INFO::iterator itBeg = vecMonitorListInfo.begin();
		VEC_MONITORMODE_INFO::iterator itEnd = vecMonitorListInfo.end();
		for(; itBeg != itEnd; ++itBeg)
		{
			if( 0 == _tcscmp(mixTemp.szDevice, itBeg->szDevice))
			{
				break;
			}
		}

		//没有在列表中找到,则需要添加
		if (itBeg == itEnd)	   
		{
			MonitorInfo tmpMonitorInfo;
			memset(&tmpMonitorInfo.szDevice,0,sizeof(tmpMonitorInfo.szDevice));
			_tcscpy_s(tmpMonitorInfo.szDevice, sizeof(mixTemp.szDevice), mixTemp.szDevice);
			vecMonitorListInfo.push_back(tmpMonitorInfo);
		}
	} 
}


// 得到所有显示器的模式
void MonitorAdapter::GetAllDisplayMode(VEC_MONITORMODE_INFO& vecMonitorListInfo)
{
	GetAllMonitorName(vecMonitorListInfo);

	BOOL		 bRetVal;
	DEVMODE 	 devmode;
	info a;
	VEC_MONITORMODE_INFO::iterator itBeg = vecMonitorListInfo.begin();
	VEC_MONITORMODE_INFO::iterator itEnd = vecMonitorListInfo.end();

	for (NULL; itBeg != itEnd; ++itBeg)
	{
		int iMode = 0;
		do
		{
			bRetVal = ::EnumDisplaySettings(itBeg->szDevice, iMode, &devmode);
			iMode++;
			if (bRetVal)
			{
				bool bFind = false;

				vector<MonitorModeInfo>::iterator itBeg_Mode = itBeg->m_vecModeInfo.begin();
				vector<MonitorModeInfo>::iterator itEnd_Mode = itBeg->m_vecModeInfo.end();
				for (NULL; itBeg_Mode != itEnd_Mode; ++itBeg_Mode)
				{
					// 如果已经在列表中找到,则结束本次循环
					if ((itBeg_Mode->m_nWidth == devmode.dmPelsWidth) && (itBeg_Mode->m_nHeight == devmode.dmPelsHeight))
					{
						bFind = true;
						break;
					}

					// 插入数据时,从 大到小排列 (按windows 分辨率设置,优先比较 宽) 
					if (
						(itBeg_Mode->m_nWidth < devmode.dmPelsWidth) ||
						((itBeg_Mode->m_nWidth == devmode.dmPelsWidth) && (itBeg_Mode->m_nHeight < devmode.dmPelsHeight))
						)
					{ 							
						break;
					}
				}

				if(!bFind)
				{
					if (itBeg_Mode == itEnd_Mode)
					{
						a.m_nHeight = devmode.dmPelsHeight;
						a.m_nWidth=devmode.dmPelsWidth;
						itBeg->m_vecModeInfo.push_back(MonitorModeInfo(devmode.dmPelsWidth, devmode.dmPelsHeight)); 
						m_info.push_back(a);
					}
					else
					{
						a.m_nHeight = devmode.dmPelsHeight;
						a.m_nWidth=devmode.dmPelsWidth;
						m_info.push_back(a);
						itBeg->m_vecModeInfo.insert(itBeg_Mode, MonitorModeInfo(devmode.dmPelsWidth, devmode.dmPelsHeight)); 
					}
				}
			}
		}
		while (bRetVal); 
	} 
}


int MonitorAdapter::ChangMonitorReselotion(HMONITOR hMonitor,const int nWidth,const int nHight,const int nFre,const int nColorBits)
{
	if ( NULL == hMonitor )
	{
		return -1;
	}
	MONITORINFOEX mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo( hMonitor , &mi);
	DEVMODE DeviceMode;
	ZeroMemory(&DeviceMode, sizeof(DEVMODE));
	DeviceMode.dmSize = sizeof(DEVMODE); 

	BOOL bFlag = TRUE;
	bFlag = EnumDisplaySettings(mi.szDevice, ENUM_CURRENT_SETTINGS, &DeviceMode);
	if ( bFlag != TRUE )
	{
		return -1;
	}	
	if (DeviceMode.dmPelsWidth == nWidth && DeviceMode.dmPelsHeight == nHight )
	{
		return 0;
	}
	DeviceMode.dmDisplayFlags = 0;
	DeviceMode.dmPelsWidth= nWidth;		
	DeviceMode.dmPelsHeight = nHight;

	DeviceMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY ;

	int nRet = ChangeDisplaySettingsEx(mi.szDevice, &DeviceMode, NULL, CDS_GLOBAL | CDS_NORESET | CDS_UPDATEREGISTRY, NULL);
	if (DISP_CHANGE_BADMODE == nRet)
	{
		ChangeDisplaySettingsEx(mi.szDevice, &DeviceMode, NULL, CDS_GLOBAL | CDS_NORESET | CDS_UPDATEREGISTRY, NULL);		

	}
	if ( DISP_CHANGE_SUCCESSFUL == nRet )
	{
		return 0;
	}
	return -1;
}

void MonitorAdapter::GetCurrentReselotion(int& nWidth,int& nHeight,int& nFreq,int& nBits)
{
	DEVMODE DeviceMode;
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DeviceMode);
	nWidth = DeviceMode.dmPelsWidth;
	nHeight = DeviceMode.dmPelsHeight;
	nFreq = DeviceMode.dmDisplayFrequency;
	nBits = DeviceMode.dmBitsPerPel;
}

void MonitorAdapter::GetCurrentReselotion(LPCWSTR lpszDeviceName, int& nWidth, int& nHeight, int& nFreq, int& nBits)
{
	DEVMODE DeviceMode;
	EnumDisplaySettings(lpszDeviceName, ENUM_CURRENT_SETTINGS, &DeviceMode);
	nWidth = DeviceMode.dmPelsWidth;
	nHeight = DeviceMode.dmPelsHeight;
	nFreq = DeviceMode.dmDisplayFrequency;
	nBits = DeviceMode.dmBitsPerPel;
}
