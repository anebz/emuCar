
// CentralitaDlg.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Centralita.h"
#include "CentralitaDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_FIN_HILO WM_USER+100

// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
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


// Cuadro de diálogo de CCentralitaDlg



CCentralitaDlg::CCentralitaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCentralitaDlg::IDD, pParent)
  , m_ipMotor(_T(""))
  , m_ipAcondicionamiento(_T(""))
  , m_portMotor(0)
  , m_portAcondicionamiento(0)
  , m_ipLuces(_T(""))
  , m_portLuces(0)
  , m_tiempo(0)
  , m_temperatura(_T(""))
  , m_RPM(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCentralitaDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, txIPMotor, m_ipMotor);
  //  DDX_Text(pDX, txPortMotor, m_portMotor);
  DDX_Text(pDX, txIPAcondicionamiento, m_ipAcondicionamiento);
  DDX_Text(pDX, txPortMotor, m_portMotor);
  DDX_Text(pDX, txPortAcondicionamiento, m_portAcondicionamiento);
  DDX_Text(pDX, txIPLuces, m_ipLuces);
  DDX_Text(pDX, txPortLuces, m_portLuces);
  DDX_Text(pDX, txTiempo, m_tiempo);
  DDX_Control(pDX, lsLog, m_log);
  DDX_Text(pDX, txTemperatura, m_temperatura);
  DDX_Text(pDX, txRPM, m_RPM);
}

BEGIN_MESSAGE_MAP(CCentralitaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(bnStart, &CCentralitaDlg::OnBnClickedbnstart)
  ON_MESSAGE (WM_FIN_HILO, OnFinHilo)
END_MESSAGE_MAP()


// Controladores de mensaje de CCentralitaDlg
UINT Motor(LPVOID lp){
  auto *pDlg = (CCentralitaDlg*) lp;
  while(pDlg->m_life){
    while(!pDlg->m_flag){}
    /* ALGORITMO DE CONEXIÓN!!! */
    CSocket misoc;
	  if(!misoc.Create()){ 
		  pDlg->MessageBox("Error"); return 0;
	  }
	  if(!misoc.Connect("127.0.0.1", 502)){
		  pDlg->MessageBox("No conecta.."); return 0;
	  }	
	  misoc.Send("Ane", 20);
	  unsigned char rec_buf[20];
	  int len = misoc.Receive(rec_buf,20); 
	  // process rec_buf --> temperature
	  len = misoc.Receive(rec_buf,20); 
	  // process rec_buf --> rpm
    pDlg->PostMessage(WM_FIN_HILO,1); // CAMBIAR ESTO DEPENDIENDO DEL PROTOCOLO
    while(pDlg->m_fin){}
  }
	return 0;
}
UINT Acondicionamiento(LPVOID lp){
  auto *pDlg = (CCentralitaDlg*) lp;
  while(pDlg->m_life){
    while(!pDlg->m_flag){}
    /* ALGORITMO DE CONEXIÓN!!! */
    CSocket misoc;
	  if(!misoc.Create()){ 
		  pDlg->MessageBox("Error"); return 0;
	  }
	  if(!misoc.Connect("127.0.0.1", 502)){
		  pDlg->MessageBox("No conecta.."); return 0;
	  }	
	  misoc.Send("Ane", 20);
	  unsigned char rec_buf[20];
	  int len = misoc.Receive(rec_buf,20); 
	  // process rec_buf --> temperature
	  len = misoc.Receive(rec_buf,20); 
	  // process rec_buf --> rpm
    pDlg->PostMessage(WM_FIN_HILO,2); // CAMBIAR ESTO DEPENDIENDO DEL PROTOCOLO
    while(pDlg->m_fin){}
  }
	return 0;
}

UINT Luces(LPVOID lp){
  auto *pDlg = (CCentralitaDlg*) lp;
  while(pDlg->m_life){
    while(!pDlg->m_flag){}
    /* ALGORITMO DE CONEXIÓN!!! */
    CSocket misoc;
	  if(!misoc.Create()){ 
		  pDlg->MessageBox("Error"); return 0;
	  }
	  if(!misoc.Connect("127.0.0.1", 502)){
		  pDlg->MessageBox("No conecta.."); return 0;
	  }	
	  misoc.Send("Ane", 20);
	  unsigned char rec_buf[20];
	  int len = misoc.Receive(rec_buf,20); 
	  // process rec_buf --> temperature
	  len = misoc.Receive(rec_buf,20); 
	  // process rec_buf --> rpm
    pDlg->PostMessage(WM_FIN_HILO,3); // CAMBIAR ESTO DEPENDIENDO DEL PROTOCOLO
    while(pDlg->m_fin){}
  }
	return 0;
}



BOOL CCentralitaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Agregar el elemento de menú "Acerca de..." al menú del sistema.

	// IDM_ABOUTBOX debe estar en el intervalo de comandos del sistema.
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

	// Establecer el icono para este cuadro de diálogo. El marco de trabajo realiza esta operación
	//  automáticamente cuando la ventana principal de la aplicación no es un cuadro de diálogo
	SetIcon(m_hIcon, TRUE);			// Establecer icono grande
	SetIcon(m_hIcon, FALSE);		// Establecer icono pequeño

	// TODO: agregar aquí inicialización adicional
  m_ipMotor = m_ipAcondicionamiento = m_ipLuces = "127.0.0.1";
  m_portMotor = 502;
  m_portAcondicionamiento = 503;
  m_portLuces = 504;
  m_tiempo = 250;
  m_temperatura = "Temperatura";
  m_RPM = "RPMs";
  UpdateData(0);
  m_start_stop = false;
  m_flag = false;
  m_fin = false;
  m_life = true;
  m_statusMotor.SubclassDlgItem(imStatusMotor, this);
  m_statusAcondicionamiento.SubclassDlgItem(imStatusAcondicionamiento, this);
  m_statusLuces.SubclassDlgItem(imStatusLuces, this);
  m_izquierdo.SubclassDlgItem(imIzquierda, this);
  m_derecho.SubclassDlgItem(imDerecha, this);
  m_freno.SubclassDlgItem(imFreno, this);
  m_imTemperatura.SubclassDlgItem(imTemperatura, this);
  m_imRPM.SubclassDlgItem(imRPM, this);
  threads.push_back(AfxBeginThread(Motor,this));
  threads.push_back(AfxBeginThread(Luces,this));
  threads.push_back(AfxBeginThread(Acondicionamiento,this));
  for(size_t ii = 0; ii < threads.size(); ii++){
    threads.at(ii)->SuspendThread();
  }
  
	return TRUE;  // Devuelve TRUE  a menos que establezca el foco en un control
}

void CCentralitaDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// Si agrega un botón Minimizar al cuadro de diálogo, necesitará el siguiente código
//  para dibujar el icono. Para aplicaciones MFC que utilicen el modelo de documentos y vistas,
//  esta operación la realiza automáticamente el marco de trabajo.

void CCentralitaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Contexto de dispositivo para dibujo

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrar icono en el rectángulo de cliente
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dibujar el icono
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// El sistema llama a esta función para obtener el cursor que se muestra mientras el usuario arrastra
//  la ventana minimizada.
HCURSOR CCentralitaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CCentralitaDlg::OnFinHilo(WPARAM wParam, LPARAM lParam){
  //Código a implementar dependiendo del protocolo
	return 0;
}


void CCentralitaDlg::OnBnClickedbnstart()
{
	/* COMUNICACION CON MOTOR, problemas con address: se manda 400 pero el motor recibe -112 wtf?
	unsigned char buf[20];
	buf[0] = 0x04;
	short add = 400;
	buf[1] = add >> 8;
	buf[2] = add & 0xFF;
	buf[3] = 0;
	buf[4] = 0x02;

	CSocket misoc;
	if(!misoc.Create()){ 
		MessageBox("Error"); return;
	}
	if(!misoc.Connect("127.0.0.1", 502)){
		MessageBox("No conecta.."); return;
	}	
	misoc.Send(buf, 20);
	unsigned char rec_buf[20];
	int len = misoc.Receive(rec_buf,20); 
	*/



  if(m_start_stop){
    for(size_t ii = 0; ii < threads.size(); ii++){
      threads.at(ii)->SuspendThread();
    }
    m_start_stop = false;
  }
  else{
    for(size_t ii = 0; ii < threads.size(); ii++){
      threads.at(ii)->ResumeThread();
    }
    m_start_stop = true;
  }
}
