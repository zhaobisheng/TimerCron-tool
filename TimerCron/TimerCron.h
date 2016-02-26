
// TimerCron.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

void DoShutdown();
DWORD WINAPI TimeThread(LPVOID lpParameter);
DWORD WINAPI CountdownThread(LPVOID lpParameter);
// CTimerCronApp: 
// 有关此类的实现，请参阅 TimerCron.cpp
//

class DoCountdown
{
private: DWORD second;
public:
	DoCountdown(DWORD Second);
	~DoCountdown();
	void operator =(DWORD Second);
	void operator -(int x);
	int GetHour();
	int GetMinute();
	int GetSecond();
	CString GetStrHour();
	CString GetStrMinute();
	CString GetStrSecond();
	operator BOOL()
	{
		return this->second == 0 ? true : false;
	}
};

class CTimerCronApp : public CWinApp
{
public:
	CTimerCronApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CTimerCronApp theApp;