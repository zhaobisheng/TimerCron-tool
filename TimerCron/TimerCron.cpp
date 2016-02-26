
// TimerCron.cpp : 定义应用程序的类行为。
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


// CTimerCronApp 构造

CTimerCronApp::CTimerCronApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTimerCronApp 对象

CTimerCronApp theApp;


// CTimerCronApp 初始化

BOOL CTimerCronApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	WNDCLASS wndMyClass;
	::GetClassInfo(AfxGetInstanceHandle(), "#32770", &wndMyClass);
	wndMyClass.lpszClassName ="TimerCron";
	AfxRegisterClass(&wndMyClass); //注册窗口类

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CTimerCronDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
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
			AfxMessageBox("重启失败");
			return;
		}
		// Shut down the system and force all applications to close.
		ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);
		
	}
}