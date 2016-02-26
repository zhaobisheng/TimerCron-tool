
// TimerCronDlg.h : 头文件
//

#pragma once

struct  PlanInfo
{
	DWORD PlanSecond;

};



// CTimerCronDlg 对话框
class CTimerCronDlg : public CDialogEx
{
// 构造
public:
	CTimerCronDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TIMERCRON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CStatic m_Time_scroll;
	//CStatic m_Time_Countdown;
	HANDLE GetNowTimeThread,GetCountdownThread;
	CFont font;
	CFont TipsFont;
	CComboBox m_List_Second;
	CComboBox m_List_Minute;
	CComboBox m_List_Hour;
	BOOL FlagTiming = FALSE;
	BOOL FlagDelay =TRUE;
	PlanInfo Plan;
	CString NidTps="Bison提示:您当前没有设置任务！";
	NOTIFYICONDATA nid;            //托盘变量
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSet();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void GetNowTime();
	afx_msg LRESULT ShowNowTime(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedRadio1();
	afx_msg void SetListInfo();          
	afx_msg void OnBnClickedRadio2();
	afx_msg void EnableWindowTiming();    //设置定时控件的可操作
	afx_msg void EnableWindowDelay();     // 设置延时控件的可操作
	afx_msg DWORD GetTimingPlan();        //获取定时计划
	afx_msg DWORD GetDelayPlan();         //获取延时计划
	afx_msg LRESULT  SetCountdown(WPARAM wParam, LPARAM lParam);         //设置倒计时提示
	afx_msg void OnBnClickedButtonCancel();
	afx_msg LRESULT OnIconNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT DeleteNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT HideWindows(WPARAM, LPARAM);
	afx_msg void ExitTools();
	afx_msg void UpdateNotifyStatus(CString);

};
