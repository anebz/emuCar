
// LucesDlg.h : header file
//

#pragma once
#include "MySock.h"
#include "Led.h"
#include <vector>

// CLucesDlg dialog
class CLucesDlg : public CDialogEx
{
// Construction
public:
	CLucesDlg(CWnd* pParent = NULL);	// standard constructor
	CMySock* pSock;
	std::vector<CLed*> luces;


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
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
