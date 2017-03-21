
// LucesDlg.h : header file
//

#pragma once
#include "Led.h"
#include "Modbus.h"

// CLucesDlg dialog
class CLucesDlg : public CDialogEx
{
// Construction
public:
	CLucesDlg(CWnd* pParent = NULL);	// standard constructor
	CLed freno, i1, i2, i3, i4;
	CModbus* pSock;

// Dialog Data
	enum { IDD = IDD_LUCES_DIALOG };

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
	CString m_freno;
	afx_msg void OnBnClickedStart();
};
