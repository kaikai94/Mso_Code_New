// 4DyuchiFileCreate.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "4DyuchiFileCreate.h"
#include "4DyuchiFileCreateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDyuchiFileCreateApp

BEGIN_MESSAGE_MAP(CDyuchiFileCreateApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDyuchiFileCreateApp ����

CDyuchiFileCreateApp::CDyuchiFileCreateApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDyuchiFileCreateApp ����

CDyuchiFileCreateApp theApp;


// CDyuchiFileCreateApp ��ʼ��

BOOL CDyuchiFileCreateApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CDyuchiFileCreateDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
