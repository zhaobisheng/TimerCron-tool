
// TimerCronDlg.h : ͷ�ļ�
//

#pragma once

struct  PlanInfo
{
	DWORD PlanSecond;

};



// CTimerCronDlg �Ի���
class CTimerCronDlg : public CDialogEx
{
// ����
public:
	CTimerCronDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TIMERCRON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
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
	CString NidTps="Bison��ʾ:����ǰû����������";
	NOTIFYICONDATA nid;            //���̱���
	// ���ɵ���Ϣӳ�亯��
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
	afx_msg void EnableWindowTiming();    //���ö�ʱ�ؼ��Ŀɲ���
	afx_msg void EnableWindowDelay();     // ������ʱ�ؼ��Ŀɲ���
	afx_msg DWORD GetTimingPlan();        //��ȡ��ʱ�ƻ�
	afx_msg DWORD GetDelayPlan();         //��ȡ��ʱ�ƻ�
	afx_msg LRESULT  SetCountdown(WPARAM wParam, LPARAM lParam);         //���õ���ʱ��ʾ
	afx_msg void OnBnClickedButtonCancel();
	afx_msg LRESULT OnIconNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT DeleteNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT HideWindows(WPARAM, LPARAM);
	afx_msg void ExitTools();
	afx_msg void UpdateNotifyStatus(CString);

};
