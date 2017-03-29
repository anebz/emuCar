
// MotorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Motor.h"
#include "MotorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMotorDlg dialog



CMotorDlg::CMotorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMotorDlg::IDD, pParent)
	, m_temp(_T(""))
	, m_rpm(_T(""))
	, m_sl_temp(0)
	, m_sl_rpm(0)
	, m_port(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMotorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_M1, m_temp);
	DDX_Text(pDX, IDC_M2, m_rpm);
	DDX_Slider(pDX, IDC_SL1, m_sl_temp);
	DDX_Slider(pDX, IDC_SL2, m_sl_rpm);
	DDX_Text(pDX, IDC_PORT, m_port);
}

BEGIN_MESSAGE_MAP(CMotorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CMotorDlg::OnBnClickedStart)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SL1, &CMotorDlg::OnNMReleasedcaptureSl1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SL2, &CMotorDlg::OnNMReleasedcaptureSl2)
	ON_BN_CLICKED(IDCANCEL, &CMotorDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMotorDlg message handlers

BOOL CMotorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_port = 502;
	m_temp.Format("Temperatura actual: 0");
	m_rpm.Format("RPM actual: 0");
	UpdateData(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMotorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMotorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMotorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMotorDlg::OnBnClickedStart()
{
	UpdateData(1);
	pSock = new CMySocket(this);
	pSock->Create(m_port, SOCK_STREAM);
	pSock->Listen();
}


void CMotorDlg::OnNMReleasedcaptureSl1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(1);
	m_temp.Format("Temperatura actual: %d", m_sl_temp*3);
	UpdateData(0);
	*pResult = 0;
}


void CMotorDlg::OnNMReleasedcaptureSl2(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(1);
	m_rpm.Format("RPM actual: %d", m_sl_rpm*70);
	UpdateData(0);
	*pResult = 0;
}


void CMotorDlg::OnBnClickedCancel()
{
	pSock->Close();
	CDialogEx::OnCancel();
}
