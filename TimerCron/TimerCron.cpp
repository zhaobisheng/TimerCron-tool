
// TimerCron.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "TimerCron.h"
#include "TimerCronDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTimerCronApp

BEGIN_MESSAGE_MAP(CTimerCronApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTimerCronApp ����

CTimerCronApp::CTimerCronApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTimerCronApp ����

CTimerCronApp theApp;


// CTimerCronApp ��ʼ��

BOOL CTimerCronApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	WNDCLASS wndMyClass;
	::GetClassInfo(AfxGetInstanceHandle(), "#32770", &wndMyClass);
	wndMyClass.lpszClassName ="TimerCron";
	AfxRegisterClass(&wndMyClass); //ע�ᴰ����

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CTimerCronDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

DWORD WINAPI TimeThread(LPVOID lpParameter)
{
	HWND hWnd = FindWindow("TimerCron", NULL);
	CTime NowTime;
	CString CurrentTime,Second;
	while (true)
	{
	NowTime = CTime::GetCurrentTime();
	NowTime.GetSecond() > 10 ? Second.Format("%d", NowTime.GetSecond()) : Second.Format("0%d", NowTime.GetSecond());
	CurrentTime.Format("%d-%d-%d %d:%d:%s", NowTime.GetYear(), NowTime.GetMonth(), NowTime.GetDay(), NowTime.GetHour(), NowTime.GetMinute(), Second);
	::PostMessage(hWnd, WM_GET_NOW_TIME, (WPARAM)CurrentTime.GetBuffer(), 0);
	Sleep(1000);
	}
}

DWORD WINAPI CountdownThread(LPVOID lpParameter)
{
	HWND hwnd = FindWindow("TimerCron", NULL);
	PlanInfo *plan = (PlanInfo *)lpParameter;
	CString CountdownTips;
	DoCountdown *Countdown = new DoCountdown(plan->PlanSecond);
	while (true)
	{   

		CountdownTips.Format("%s : %s : %s", Countdown->GetStrHour(), Countdown->GetStrMinute(), Countdown->GetStrSecond());
		::PostMessage(hwnd, WM_GET_COUNTDOWN, (WPARAM)CountdownTips.GetBuffer(), NULL);
		if (*Countdown)
		{	
			DoShutdown();
			break;
		}
		*Countdown - 1;
		Sleep(1000);
	}
	return 0;
}




DoCountdown::DoCountdown(DWORD Second)
{
	this->second = Second;
}

DoCountdown::~DoCountdown()
{

}

void DoCountdown::operator =(DWORD Second)
{
	this->second = Second;
}

int DoCountdown::GetHour()
{
	return this->second / 3600;
}

int DoCountdown::GetMinute()
{
	return this->second % 3600 / 60;
}

int DoCountdown::GetSecond()
{
	return this->second % 60;
}
void DoCountdown::operator -(int x)
{
	this->second -= x;
	
}

CString DoCountdown::GetStrHour()
{
	CString StrHour;
	this->GetHour() > 10 ? StrHour.Format("%d", this->GetHour()) : StrHour.Format("0%d", this->GetHour());
	return StrHour;
}

CString DoCountdown::GetStrMinute()
{
	CString StrMinute;
	this->GetMinute() > 10 ? StrMinute.Format("%d", this->GetMinute()) : StrMinute.Format("0%d", this->GetMinute());
	return StrMinute;
}

CString DoCountdown::GetStrSecond()
{
	CString StrSecond;
	this->GetSecond() > 10 ? StrSecond.Format("%d", this->GetSecond()) : StrSecond.Format("0%d", this->GetSecond());
	return StrSecond;
}

void DoShutdown()
{
	{
		Sleep(1000);
		HANDLE hToken;
		TOKEN_PRIVILEGES tkp;
		// Get a token for this process.
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			AfxMessageBox("OpenProcessToken Error!");
			return;
		}
		// Get the LUID for the shutdown privilege.
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1; // one privilege to set
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL);
		if (GetLastError() != ERROR_SUCCESS)
		{
			AfxMessageBox("����ʧ��");
			return;
		}
		// Shut down the system and force all applications to close.
		ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);
		
	}
}