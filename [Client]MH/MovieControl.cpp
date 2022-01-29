








#include "StdAfx.h"
#include "MovieControl.h"


MovieControl::MovieControl()
{
	m_hWnd = NULL;
	m_bMovieOver = false;
	m_lsFile = NULL;

	m_pGraph = NULL;
	m_pVidWin = NULL;
	m_pMediaControl = NULL;
	m_pEvent = NULL;
}

MovieControl::~MovieControl()
{
	Destroy();
}

bool MovieControl::SetWindowHandle(HWND hWnd)
{
	if(hWnd == NULL)
	{		
		return false;

	}
	
	m_hWnd = hWnd;

	return true;
}



HWND MovieControl::GetWindowHandle()
{
	return m_hWnd;
}

bool MovieControl::ResisterFile(LPCWSTR sFile)
{
	m_lsFile = sFile;

	if(m_lsFile == NULL)
	{
		return false;
	}

	//DirectShow ����

	//1. Filter Graph Manager�� �ν��Ͻ��� �����Ѵ�.
	//2. ���� �׷����� �����ϱ� ���� ���� �׷��� �Ŵ����� ����Ѵ�.

	//3. �����Ͱ� ���ͷ� ����ϸ鼭 �׷����� �����Ѵ�.

	HWND hWnd = NULL;
	hWnd = MovieControl::GetWindowHandle();

	if(hWnd == NULL)

	{
		return false;
	}


	RECT rc;
	CoInitialize(NULL); //COM���� ���̺귯���� ����Ϸ��� CoInitialize(NUUL)�� ���� ȣ���ؼ� �ʱ�ȭ ��


	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder,	reinterpret_cast <void **>(&m_pGraph));

	HRESULT hr;
	hr = m_pGraph->RenderFile(m_lsFile, NULL); 
	if(FAILED(hr))
	{

		return false;
	}


	//QueryInterface()
	//ù��° ���ڴ� �� COM ��ü�� ����ϴ� �ڵ忡�� COM��ü���� �� �� �޶�� ��û�ϱ� ���� �����Դϴ�.
	//��û�� ���� �ι�° ���ڷ� �ް� �˴ϴ�.
	m_pGraph->QueryInterface(IID_IVideoWindow, (void **)&m_pVidWin);  
	m_pVidWin->HideCursor(OATRUE);	
	//IVideoWindow �������̽��� ������ �ػ� ����
	m_pVidWin->put_Owner((OAHWND)hWnd); 

	m_pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);	
	GetClientRect(hWnd,&rc);
	m_pVidWin->SetWindowPosition(rc.left, rc.top, rc.right, rc.bottom);


	m_pGraph->QueryInterface(IID_IMediaEventEx, (void **)&m_pEvent); 
	m_pEvent->SetNotifyWindow((OAHWND)hWnd, WM_GRAPHNOTIFY, 0); 

	m_pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl); 

	return true;

}

void MovieControl::PlayMovie()
{
	m_pMediaControl->Run(); 

	long ev;
	m_pEvent->WaitForCompletion(INFINITE, &ev);
}

void MovieControl::MovieStop()
{
	m_pMediaControl->Stop();
}

void MovieControl::Destroy()
{		
	if(m_bMovieOver)
	{
		m_pMediaControl->Stop();
	}

	if(m_pMediaControl)
		m_pMediaControl->Release(); 
		
	if(m_pVidWin)
	{

		m_pVidWin->put_Visible(OAFALSE);
		m_pVidWin->put_Owner(NULL);
		m_pVidWin->Release();
	}
	
	if(m_pEvent)
		m_pEvent->Release();	
	
	if(m_pGraph)
		m_pGraph->Release(); 	
	
	CoUninitialize();
}

bool MovieControl::HandleEvent() 
{ 
	long evCode, param1, param2; 
	if(m_pEvent != NULL)
	{
		HRESULT hr; 
		while (hr = m_pEvent->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(hr)) 
		{ 
			hr = m_pEvent->FreeEventParams(evCode, param1, param2); 
			if ((EC_COMPLETE == evCode) || (EC_USERABORT == evCode)) 
			{ 			
				m_bMovieOver = true;

				break; 
			} 
		} 
	}	

	if(m_bMovieOver)
	{

		return true;
	}
	else
	{

		return false;
	}
} 




