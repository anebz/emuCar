
// MotorDlg.h : header file
//

#pragma once
#include "MySocket.h"

// CMotorDlg dialog
class CMotorDlg : public CDialogEx
{
// Construction
public:
	CMotorDlg(CWnd* pParent = NULL);	// standard constructor
	CMySocket *pSock;

// Dialog Data
	enum { IDD = IDD_MOTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_temp, m_rpm;
	int m_sl_temp, m_sl_rpm, m_port;
	afx_msg void OnBnClickedStart();
	afx_msg void OnNMReleasedcaptureSl1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSl2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
};
