
// AccionamientosDlg.h : header file
//

#pragma once
#include "MySocket.h"

// CAccionamientosDlg dialog
class CAccionamientosDlg : public CDialogEx
{
// Construction
public:
	CAccionamientosDlg(CWnd* pParent = NULL);	// standard constructor
	CMySocket* pSock;

// Dialog Data
	enum { IDD = IDD_ACCIONAMIENTOS_DIALOG };

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
	int m_port;
	afx_msg void OnBnClickedStart();
	BOOL m_button[3];
	afx_msg void OnBnClickedCancel();
};
