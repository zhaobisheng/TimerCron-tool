
// TimerCron.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

void DoShutdown();
DWORD WINAPI TimeThread(LPVOID lpParameter);
DWORD WINAPI CountdownThread(LPVOID lpParameter);
// CTimerCronApp: 
// �йش����ʵ�֣������ TimerCron.cpp
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

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTimerCronApp theApp;