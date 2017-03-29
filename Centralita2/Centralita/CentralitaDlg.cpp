
// CentralitaDlg.cpp: archivo de implementaci�n
//

#include "stdafx.h"
#include "Centralita.h"
#include "CentralitaDlg.h"
#include "afxdialogex.h"
#include <ctime>
#include <mutex>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_FIN_HILO WM_USER+100
std::mutex mtx;

// Cuadro de di�logo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Datos del cuadro de di�logo
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementaci�n
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


// Cuadro de di�logo de CCentralitaDlg
UINT Motor(LPVOID lp);
UINT Acondicionamiento(LPVOID lp);
UINT Luces(LPVOID lp);


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
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CCentralitaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Agregar el elemento de men� "Acerca de..." al men� del sistema.

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

	// Establecer el icono para este cuadro de di�logo. El marco de trabajo realiza esta operaci�n
	//  autom�ticamente cuando la ventana principal de la aplicaci�n no es un cuadro de di�logo
	SetIcon(m_hIcon, TRUE);			// Establecer icono grande
	SetIcon(m_hIcon, FALSE);		// Establecer icono peque�o

	// TODO: agregar aqu� inicializaci�n adicional
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
	m_numMsg = 1;

  m_statusMotor.SubclassDlgItem(imStatusMotor, this);
  m_statusAcondicionamiento.SubclassDlgItem(imStatusAcondicionamiento, this);
  m_statusLuces.SubclassDlgItem(imStatusLuces, this);
  m_izquierdo.SubclassDlgItem(imIzquierda, this);
  m_derecho.SubclassDlgItem(imDerecha, this);
  m_freno.SubclassDlgItem(imFreno, this);
  m_imTemperatura.SubclassDlgItem(imTemperatura, this);
  m_imRPM.SubclassDlgItem(imRPM, this);

  
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

// Si agrega un bot�n Minimizar al cuadro de di�logo, necesitar� el siguiente c�digo
//  para dibujar el icono. Para aplicaciones MFC que utilicen el modelo de documentos y vistas,
//  esta operaci�n la realiza autom�ticamente el marco de trabajo.

void CCentralitaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Contexto de dispositivo para dibujo

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrar icono en el rect�ngulo de cliente
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

// El sistema llama a esta funci�n para obtener el cursor que se muestra mientras el usuario arrastra
//  la ventana minimizada.
HCURSOR CCentralitaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//COMUNICACION CON MOTOR
	/*unsigned char buf[20];

	buf[0] = m_numMsg >> 8;
	buf[1] = m_numMsg & 0xFF; // transaction identifier
	buf[2] = 0x00;
	buf[3] = 0x00; // protocol identifier, 0 in Modbus
	buf[4] = 0x00;
	buf[5] = 0x06; // length
	buf[6] = 0x21; // ID motor

	buf[7] = 0x04; // function code, read
	short add = 400; 
	buf[8] = add >> 8;
	buf[9] = add & 0xFF; // data address
	buf[10] = 0;
	buf[11] = 0x02;

	CSocket misoc;
	if(!misoc.Create()){ 
		m_log.AddString("Error al crear socket"); return;
	}
	if(!misoc.Connect("127.0.0.1", 502)){
		m_log.AddString("No conecta con puerto 502"); return;
	}	
	misoc.Send(buf, 20);
	unsigned char rec_buf[20];
	int len = misoc.Receive(rec_buf,20); 

	if(rec_buf[7] == 0x04 && rec_buf[8] == 0x04 && rec_buf[0]*256 + rec_buf[1] == m_numMsg){
		int temp = rec_buf[9]*256 + rec_buf[10]; 
		int rpm = rec_buf[11]*256 + rec_buf[12];
		m_log.AddString("Motor OK");
	}else m_log.AddString("Error en comunicaci�n con el motor. No se han recibido 3 datos");
	m_numMsg++;*/
	
	//COMUNICACION CON ACCIONAMIENTOS
	/*unsigned char buf[20];

	buf[0] = m_numMsg >> 8;
	buf[1] = m_numMsg & 0xFF; // transaction identifier
	buf[2] = 0x00;
	buf[3] = 0x00; // protocol identifier, 0 in Modbus
	buf[4] = 0x00;
	buf[5] = 0x06; // length
	buf[6] = 0x22; // ID accionamientos

	buf[7] = 0x04; // function code, read
	short add = 400; 
	buf[8] = add >> 8;
	buf[9] = add & 0xFF; // data address
	buf[10] = 0;
	buf[11] = 0x03;

	CSocket misoc;
	if(!misoc.Create()){ 
		m_log.AddString("Error al crear socket"); return;
	}
	if(!misoc.Connect("127.0.0.1", 503)){
		m_log.AddString("No conecta con puerto 503"); return;
	}	
	misoc.Send(buf, 20);
	unsigned char rec_buf[20];
	int len = misoc.Receive(rec_buf,20); 

	if(rec_buf[7] == 0x04 && rec_buf[8] == 0x06){
		bool freno = rec_buf[10];
		bool izq = rec_buf[12];
		bool der = rec_buf[14];
		m_log.AddString("Accionamientos OK");
	}else m_log.AddString("Error en comunicaci�n con los accionamientos. No se han recibido 3 datos");
	m_numMsg++;*/
	
	//COMUNICACION CON LUCES
	/*unsigned char buf[20];
	unsigned char rec_buf[20];

	buf[0] = m_numMsg >> 8;
	buf[1] = m_numMsg & 0xFF; // transaction identifier
	buf[2] = 0x00;
	buf[3] = 0x00; // protocol identifier, 0 in Modbus
	buf[4] = 0x00;
	buf[5] = 0x06; // length
	buf[6] = 0x23; // ID luces

	buf[7] = 0x06; // function code, write

	CSocket misoc;
	if(!misoc.Create()){ 
		m_log.AddString("Error al crear socket"); return;
	}
	if(!misoc.Connect("127.0.0.1", 504)){
		m_log.AddString("No conecta con puerto 504"); return;
	}

	int ok = 0;

	// addresses and values for each light
	buf[8] = 0x01;
	buf[10] = 0;
	// freno
	buf[9] = 500 & 0xFF; // data address
	buf[11] = 0x01; // example, on
	misoc.Send(buf, 20);
	misoc.Receive(rec_buf,20); 
	if(memcmp(buf, rec_buf, 20) == 0) ok++;

	// intermitente izq delantero
	buf[9] = 501 & 0xFF; // data address
	buf[11] = 0x01; // example, on
	misoc.Send(buf, 20);
	misoc.Receive(rec_buf,20); 
	if(memcmp(buf, rec_buf, 20) == 0) ok++;

	// intermitente der delantero
	buf[9] = 502 & 0xFF; // data address
	buf[11] = 0x00; // example, off
	misoc.Send(buf, 20);
	misoc.Receive(rec_buf,20); 
	if(memcmp(buf, rec_buf, 20) == 0) ok++;

	// intermitente izq trasero
	buf[9] = 503 & 0xFF; // data address
	buf[11] = 0x00; // example, off
	misoc.Send(buf, 20);
	misoc.Receive(rec_buf,20); 
	if(memcmp(buf, rec_buf, 20) == 0) ok++;

	// intermitente der trasero
	buf[9] = 504 & 0xFF; // data address
	buf[11] = 0x01; // example, on
	misoc.Send(buf, 20);
	misoc.Receive(rec_buf,20); 
	if(memcmp(buf, rec_buf, 20) == 0) ok++;

	if(ok == 5) m_log.AddString("Luces OK");
	else m_log.AddString("Error en comunicaci�n con las luces");
	m_numMsg++;*/


UINT Luces(LPVOID lp)
{
	CCentralitaDlg *pDlg = (CCentralitaDlg*)lp;
	CSocket misoc;
	mtx.lock();
	misoc.Create();
	misoc.Connect("127.0.0.1", 504);
	mtx.unlock();

	unsigned char buf[20];
	unsigned char rec_buf[20];

	while(1){

		buf[0] = 1 >> 8;
		buf[1] = 1 & 0xFF; // transaction identifier
		buf[2] = 0x00;
		buf[3] = 0x00; // protocol identifier, 0 in Modbus
		buf[4] = 0x00;
		buf[5] = 0x06; // length
		buf[6] = 0x23; // ID luces

		buf[7] = 0x06; // function code, write
		int ok = 0;

		// addresses and values for each light
		buf[8] = 0x01;
		buf[10] = 0;
		// freno
		buf[9] = 500 & 0xFF; // data address
		buf[11] = 0x01; // example, on
		misoc.Send(buf, 20);
		misoc.Receive(rec_buf,20); 
		if(memcmp(buf, rec_buf, 20) == 0) ok++;

		// intermitente izq delantero
		buf[9] = 501 & 0xFF; // data address
		buf[11] = 0x01; // example, on
		misoc.Send(buf, 20);
		misoc.Receive(rec_buf,20); 
		if(memcmp(buf, rec_buf, 20) == 0) ok++;

		// intermitente der delantero
		buf[9] = 502 & 0xFF; // data address
		buf[11] = 0x00; // example, off
		misoc.Send(buf, 20);
		misoc.Receive(rec_buf,20); 
		if(memcmp(buf, rec_buf, 20) == 0) ok++;

		// intermitente izq trasero
		buf[9] = 503 & 0xFF; // data address
		buf[11] = 0x00; // example, off
		misoc.Send(buf, 20);
		misoc.Receive(rec_buf,20); 
		if(memcmp(buf, rec_buf, 20) == 0) ok++;

		// intermitente der trasero
		buf[9] = 504 & 0xFF; // data address
		buf[11] = 0x01; // example, on
		misoc.Send(buf, 20);
		misoc.Receive(rec_buf,20); 
		if(memcmp(buf, rec_buf, 20) == 0) ok++;

		if(ok == 5) pDlg->m_log.AddString("Luces OK");
		else pDlg->m_log.AddString("Error en comunicaci�n con las luces");

		Sleep(500);
	}

	
	mtx.lock();
	misoc.Close();
	mtx.unlock();
	return 0;
}

UINT Acc(LPVOID lp)
{
	CCentralitaDlg *pDlg = (CCentralitaDlg*)lp;
	CSocket c2;
	mtx.lock();
	c2.Create();
	c2.Connect("127.0.0.1", 503);
	mtx.unlock();

	unsigned char buf[20];

	while(1){

		buf[0] = 1 >> 8;
		buf[1] = 1 & 0xFF; // transaction identifier
		buf[2] = 0x00;
		buf[3] = 0x00; // protocol identifier, 0 in Modbus
		buf[4] = 0x00;
		buf[5] = 0x06; // length
		buf[6] = 0x22; // ID accionamientos

		buf[7] = 0x04; // function code, read
		short add = 400; 
		buf[8] = add >> 8;
		buf[9] = add & 0xFF; // data address
		buf[10] = 0;
		buf[11] = 0x03;

		c2.Send(buf, 20);
		unsigned char rec_buf[20];
		int len = c2.Receive(rec_buf,20); 

		if(rec_buf[7] == 0x04 && rec_buf[8] == 0x06){
			bool freno = rec_buf[10];
			bool izq = rec_buf[12];
			bool der = rec_buf[14];
			pDlg->m_log.AddString("Accionamientos OK" + freno + izq + der);
		}else pDlg->m_log.AddString("Error en comunicaci�n con los accionamientos. No se han recibido 3 datos");

		Sleep(500);
	}

	mtx.lock();
	c2.Close();
	mtx.unlock();
	return 0;
}

void CCentralitaDlg::OnBnClickedbnstart()
{
	CWinThread *p1 = AfxBeginThread(Luces,this);
	CWinThread *p2 = AfxBeginThread(Acc,this);
}


void CCentralitaDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_flag = true;
}

 void CCentralitaDlg::writeOnLog(CString str){
	std::time_t result = std::time(nullptr);
	CString toWrite = std::asctime(std::localtime(&result));
	toWrite += ": " + str;
	m_log.AddString(toWrite);
 }