
// calendar2Dlg.h : ͷ�ļ�
//

#pragma once

#include "WzCalendarCtrl.h"


// Ccalendar2Dlg �Ի���
class Ccalendar2Dlg : public CDialog
{
// ����
public:
	Ccalendar2Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CALENDAR2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
// ʵ��
protected:
	HICON m_hIcon;
    int m_CurrentMonth;
    CWzCalendarCtrl m_CalendarCtrl;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnSetYear();
    afx_msg void OnBnClickedBtnLastMonth();
    afx_msg void OnBnClickedBtnNextMonth();
    afx_msg void OnBnClickedBtnInformation();
    afx_msg void OnBnClickedBtnTest();
    afx_msg void OnBnClickedBtnSolarterm();
    afx_msg void OnBnClickedBtnNewmoon();
};
