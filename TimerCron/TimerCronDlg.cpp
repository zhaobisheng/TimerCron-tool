
// TimerCronDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TimerCron.h"
#include "TimerCronDlg.h"
#include "afxdialogex.h"
#include "EXIT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���





CTimerCronDlg::CTimerCronDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTimerCronDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimerCronDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_NOWTIME, m_Time_scroll);
	DDX_Control(pDX, IDC_COMBO_SECOND, m_List_Second);
	DDX_Control(pDX, IDC_COMBO_MINUTE, m_List_Minute);
	DDX_Control(pDX, IDC_COMBO_HOUR, m_List_Hour);
}

BEGIN_MESSAGE_MAP(CTimerCronDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SET, &CTimerCronDlg::OnBnClickedButtonSet)
	ON_MESSAGE(WM_GET_NOW_TIME, ShowNowTime)
	ON_MESSAGE(WM_GET_COUNTDOWN,SetCountdown)
	ON_MESSAGE(UM_ICONNOTIFY, OnIconNotify)
	ON_MESSAGE(WM_DELETE_NOTIFY, (LRESULT(__thiscall CWnd::*)(WPARAM, LPARAM))DeleteNotify)
	ON_MESSAGE(WM_HIDE_WINDOWS, HideWindows)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO1, &CTimerCronDlg::OnBnClickedRadio1)

	ON_BN_CLICKED(IDC_RADIO2, &CTimerCronDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CTimerCronDlg::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CTimerCronDlg ��Ϣ�������

BOOL CTimerCronDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	SkinH_Attach();
	SkinH_AttachEx("SkinH.she",NULL);

	this->SetListInfo();
	this->EnableWindowTiming();
	this->EnableWindowDelay();	
	this->GetNowTime();
	//SetCountdown();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd;           //������
	nid.uID = IDR_MAINFRAME;     //icon  ID
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;   //������ӵ�е�״̬
	nid.uCallbackMessage = UM_ICONNOTIFY;            //�ص���Ϣ
	nid.hIcon = m_hIcon;                            //icon ����
	CString str = NidTps;       //������ʾ
	lstrcpyn(nid.szTip, (LPCSTR)str, sizeof(nid.szTip) / sizeof(nid.szTip[0]));
	Shell_NotifyIcon(NIM_ADD, &nid);   //��ʾ����

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTimerCronDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	switch (nType)
	{
	case SIZE_MINIMIZED:
		ShowWindow(SW_HIDE);
		break;
	}
}



// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTimerCronDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTimerCronDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTimerCronDlg::OnBnClickedButtonSet()
{

	BOOL FlagSet=false;
	DWORD TotalSecond;
	if (FlagTiming)
	{
		FlagSet = true;
		TotalSecond = this->GetTimingPlan();
	}
	else if (FlagDelay){
		FlagSet = true;
		TotalSecond = this->GetDelayPlan();
	}
	else
	{
		AfxMessageBox("��ѡ��һ�ֹػ��ƻ���ʽ!");
	}
	if (FlagSet)
	{
		this->UpdateNotifyStatus("Bison��ʾ:����ǰ��1�ʱ����");
		Plan.PlanSecond = TotalSecond;
		GetCountdownThread = CreateThread(NULL, 0, CountdownThread, &Plan, NULL, NULL);
		
	}
}

DWORD CTimerCronDlg::GetTimingPlan()
{
	CTime TodayTime, OtherTime;
	CTimeSpan TotalSecond;
	DWORD SpanSecond;
	CDateTimeCtrl* PickerTime1 = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER1);
	PickerTime1->GetTime(TodayTime);
	CDateTimeCtrl* PickerTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER2);
	PickerTime2->GetTime(OtherTime);
	SpanSecond = TodayTime.GetHour() * 3600 + TodayTime.GetMinute() * 60 + TodayTime.GetSecond() - (OtherTime.GetHour() * 3600 + OtherTime.GetMinute() * 60 + OtherTime.GetSecond());
	OtherTime += SpanSecond;
	TotalSecond = OtherTime - CTime::GetCurrentTime();
	return TotalSecond.GetTotalSeconds();
}

DWORD CTimerCronDlg::GetDelayPlan()
{
	CString StrHour,StrMinute,StrSecond;
	DWORD TotalSecond;
	GetDlgItem(IDC_COMBO_HOUR)->GetWindowTextA(StrHour);
	GetDlgItem(IDC_COMBO_MINUTE)->GetWindowTextA(StrMinute);
	GetDlgItem(IDC_COMBO_SECOND)->GetWindowTextA(StrSecond);
	TotalSecond = atoi(StrHour) * 3600 + atoi(StrMinute) * 60 + atoi(StrSecond);
	return TotalSecond;
}

HBRUSH CTimerCronDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID()==IDC_STATIC_NOWTIME)
	{
		pDC->SetTextColor(RGB(200, 15, 250));
	}
	
	return hbr;
}

void CTimerCronDlg::GetNowTime()
{
     CString CountDown = "00  :  00  :  00";
	GetDlgItem(IDC_STATIC_COUNTDOWN)->SetWindowText(CountDown);
	font.CreateFont(28, 20, 0, 0, FW_BLACK, FALSE, FALSE,
		FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, _T("΢���ź�"));
	GetDlgItem(IDC_STATIC_NOWTIME)->SetFont(&font);
	RedrawWindow();

	GetNowTimeThread = CreateThread(NULL, 0, TimeThread, NULL, NULL, NULL);

	TipsFont.CreateFont(28, 25, 0, 0, FW_BLACK, FALSE, FALSE,
		FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, _T("΢���ź�"));
	GetDlgItem(IDC_STATIC_COUNTDOWN)->SetFont(&TipsFont);
}

void CTimerCronDlg::SetListInfo()
{
	CString Num;
	for (int Start = 0; Start < 60; Start++)
	{
		Num.Format("%d", Start);
		
		m_List_Minute.InsertString(Start, Num);
		m_List_Second.InsertString(Start, Num);
	}
	for (int Start = 0; Start < 24; Start++)
	{
		Num.Format("%d", Start);
		m_List_Hour.InsertString(Start, Num);
	}
	m_List_Second.SetCurSel(0);
	m_List_Minute.SetCurSel(0);
	m_List_Hour.SetCurSel(0);
	CButton *pBtn = NULL;
	pBtn = (CButton*)GetDlgItem(IDC_RADIO2);
	pBtn->SetCheck(1);
}

LRESULT CTimerCronDlg::ShowNowTime(WPARAM wParam, LPARAM lParam)
{
	CString StrTime;
	StrTime.Format("%s", (LPCTSTR)wParam);
	GetDlgItem(IDC_STATIC_NOWTIME)->SetWindowText(StrTime);
	return 1;
}

void CTimerCronDlg::OnBnClickedRadio1()
{
	
	
		FlagTiming = TRUE;
		FlagDelay = FALSE;
		CTime CurrentTime=CTime::GetCurrentTime();
		CDateTimeCtrl* PickerTime = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER1);
		PickerTime->SetTime(&CurrentTime);
		this->EnableWindowTiming();
		this->EnableWindowDelay();
}




void CTimerCronDlg::OnBnClickedRadio2()
{

	FlagTiming = FALSE;
	FlagDelay = TRUE;
	this->EnableWindowTiming();
	this->EnableWindowDelay();
}

void CTimerCronDlg::EnableWindowTiming()
{
	GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(FlagTiming);
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(FlagTiming);
}

void CTimerCronDlg::EnableWindowDelay()
{
	GetDlgItem(IDC_COMBO_HOUR)->EnableWindow(FlagDelay);
	GetDlgItem(IDC_COMBO_MINUTE)->EnableWindow(FlagDelay);
	GetDlgItem(IDC_COMBO_SECOND)->EnableWindow(FlagDelay);

}

LRESULT CTimerCronDlg::SetCountdown(WPARAM wParam, LPARAM lParam)
{
	
	
	CString CountDown;
	CountDown = "00  :  00  :  00";
	CountDown.Format("%s", (LPCTSTR)wParam);
	GetDlgItem(IDC_STATIC_COUNTDOWN)->SetWindowText(CountDown);
	return 0;
}

void CTimerCronDlg::OnClose()
{
	CEXIT CExitDlg;
	CExitDlg.DoModal();

}

LRESULT CTimerCronDlg::DeleteNotify(WPARAM wParam, LPARAM lParam)
{
	Shell_NotifyIcon(NIM_DELETE, &nid); //��������ɾ��ͼ�� 
	UpdateWindow();
	UpdateData();
	OnBnClickedButtonCancel();
	TerminateThread(GetNowTimeThread, NULL);
	ExitTools();
	return 0;
}

void CTimerCronDlg::ExitTools()
{
	Sleep(100);
	CDialogEx::OnClose();
	exit(0);
}

void CTimerCronDlg::OnBnClickedButtonCancel()
{
	
	TerminateThread(GetCountdownThread, NULL);
//	CloseHandle(GetCountdownThread);
	CString CountDown = "00 : 00 : 00";
	GetDlgItem(IDC_STATIC_COUNTDOWN)->SetWindowText(CountDown);
	this->UpdateNotifyStatus("Bison��ʾ:����ǰû����������");
}

void CTimerCronDlg::UpdateNotifyStatus(CString Tips)
{
	NidTps = Tips ;
	lstrcpyn(nid.szTip, (LPCSTR)NidTps, sizeof(nid.szTip) / sizeof(nid.szTip[0]));
	Shell_NotifyIcon(NIM_MODIFY, &nid);   //��ʾ����
}

LRESULT CTimerCronDlg::OnIconNotify(WPARAM wParam, LPARAM lParam)
{

	switch ((UINT)lParam)
	{
	case WM_LBUTTONDOWN: // click or dbclick left button on icon
	case WM_LBUTTONDBLCLK: // should show desktop
		if (!IsWindowVisible())
		{
			this->ShowWindow(SW_SHOWNORMAL);
			this->SetActiveWindow();
			this->SetForegroundWindow();
			this->BringWindowToTop();
		}
		else
			ShowWindow(SW_HIDE);
		break;
	}
	return 0;
}

LRESULT CTimerCronDlg::HideWindows(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_HIDE);
	return 1;
}