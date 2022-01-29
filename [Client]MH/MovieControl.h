



#ifndef _MOVIECONTROL_H
#define _MOVIECONTROL_H



#include <dshow.h>
#include <commctrl.h>

#include <commdlg.h>

#pragma comment(lib, "Strmiids")
#pragma comment(lib, "Quartz")


#define WM_GRAPHNOTIFY WM_APP+1

class MovieControl
{
public:
	MovieControl();
	~MovieControl();

	bool SetWindowHandle(HWND hWnd); //������ �ڵ� ����
	HWND GetWindowHandle(); //������ �ڵ� ���
	bool ResisterFile(LPCWSTR sFile); //���ڿ� �տ� L�� ���̸� �����ڵ幮���� LPCWSTR�� �ν�


	virtual void Destroy(); 
	virtual void PlayMovie(); //������ ����	

	virtual void MovieStop(); // ������ ����
	virtual bool HandleEvent(); //������ ���� Ȯ��

private:

	bool m_bMovieOver; //������ ���� ���� Ŭ���� ����

	IGraphBuilder   *m_pGraph; 
	IVideoWindow    *m_pVidWin; //�ش� �������̽��� �ػ�, Ǯ��ũ�� ���� ����
	IMediaControl   *m_pMediaControl; 
	IMediaEventEx   *m_pEvent;



	LPCWSTR m_lsFile;
	//int m_nLeft, m_nTop, m_nRight, m_nBottom;


	HWND m_hWnd;

};


#endif _MOVIECONTROL_H


