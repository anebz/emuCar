
// CentralitaDlg.cpp: archivo de implementaci�n
//

#include "stdafx.h"
#include "Centralita.h"
#include "CentralitaDlg.h"
#include "afxdialogex.h"
#include <ctime>
#include <mutex>

#include <windows.h>
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_FIN_HILO WM_USER+100
std::mutex mtx;
//bool firststr[3] = {false};
//bool connected[3] = {false};

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
  ON_MESSAGE (WM_FIN_HILO, OnFinHilo)
	ON_WM_TIMER()
	ON_BN_CLICKED(bnClear, &CCentralitaDlg::OnBnClickedbnclear)
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
  m_flag = false;
  m_fin = false;
  m_life = true;
	m_setTimer2 = m_setTimer3 = false;
	m_numMsg = 1;

	for(size_t i = 0; i<5; i++) luces[i] = false;

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

// Controladores de mensaje de CCentralitaDlg
UINT Motor(LPVOID lp){
	static bool connected = false;
	static bool firststr = false;
  auto *pDlg = (CCentralitaDlg*) lp;
  while(pDlg->m_life){
    while(!pDlg->m_flag){}
    
		// connection algorithm
		unsigned char buf[20];
    mtx.lock();
    int checker = pDlg->m_numMsg;
		short n_data = 2;
    pDlg->ModBusObj.constructBuffer(buf, pDlg->m_numMsg, 21, 4, 400, n_data);
    mtx.unlock();
		CSocket misoc;
    mtx.lock();
		if(!misoc.Create()){ 
			pDlg->writeOnLog("Error al crear socket");
      mtx.unlock();
      pDlg->m_statusMotor.m_color = 1;
      pDlg->m_statusMotor.Invalidate(true);
      pDlg->PostMessage(WM_FIN_HILO,1);
			continue;
		}
		if(!misoc.Connect(pDlg->m_ipMotor, pDlg->m_portMotor)){
			if(!connected) pDlg->writeOnLog("No conecta con el puerto de motor"); 
			connected = true;
			firststr = false;
      mtx.unlock();
      pDlg->m_statusMotor.m_color = 1;
      pDlg->m_statusMotor.Invalidate(true);
      pDlg->PostMessage(WM_FIN_HILO,1);
			continue;
		}
		else connected = false;
    mtx.unlock();
    pDlg->m_statusMotor.m_color = 0;
    pDlg->m_statusMotor.Invalidate(true);
		misoc.Send(buf, 20);
		unsigned char rec_buf[20];
		int len = misoc.Receive(rec_buf,20); 

		if(rec_buf[7] == 0x04 && rec_buf[8] == 0x04 && rec_buf[0]*256+rec_buf[1] == buf[0]*256+buf[1] && rec_buf[5] == (3+2*n_data)){
			// data is correct
			int temp = rec_buf[9]*256 + rec_buf[10]; 
      pDlg->m_imTemperatura.m_nivel = temp/3;

			int rpm = rec_buf[11]*256 + rec_buf[12];
      pDlg->m_imRPM.m_nivel = rpm/70;
			if(!firststr) pDlg->writeOnLog("Motor OK");
			firststr = true;
      pDlg->m_imTemperatura.Invalidate(true);
      pDlg->m_imRPM.Invalidate(true);
		}else if(firststr){
			pDlg->writeOnLog("Error en comunicaci�n con el motor. No se han recibido 3 datos");
			firststr = false;
		}
		pDlg->m_numMsg++;
    pDlg->PostMessage(WM_FIN_HILO,1); 
    mtx.lock();
		misoc.Close();
    mtx.unlock();
  }
	return 0;
}

UINT Acondicionamiento(LPVOID lp){
	static bool connected = false;
	static bool firststr = false;
  auto *pDlg = (CCentralitaDlg*) lp;
  while(pDlg->m_life){
		pDlg->m_fin = false;
    while(!pDlg->m_flag){}
   
		// connection algorithm
		unsigned char buf[20];
		int checker = pDlg->m_numMsg;
    mtx.lock();
		short n_data = 3;
    pDlg->ModBusObj.constructBuffer(buf, pDlg->m_numMsg, 22, 4, 400, n_data);
    mtx.unlock();
    CSocket misoc;
    mtx.lock();
	  if(!misoc.Create()){ 
		  pDlg->writeOnLog("Error al crear socket"); 
      mtx.unlock();
      pDlg->m_statusAcondicionamiento.m_color = 1;
      pDlg->m_statusAcondicionamiento.Invalidate(true);
			pDlg->m_setTimer2 = pDlg->m_setTimer3 = false;
			pDlg->KillTimer(2); pDlg->KillTimer(3);
			pDlg->m_freno.m_color = pDlg->m_izquierdo.m_color = pDlg->m_derecho.m_color = 5;
			pDlg->m_freno.Invalidate(true); pDlg->m_izquierdo.Invalidate(true); pDlg->m_derecho.Invalidate(true);
      pDlg->PostMessage(WM_FIN_HILO,2);
		  continue;
	  }
		if(!misoc.Connect(pDlg->m_ipAcondicionamiento, pDlg->m_portAcondicionamiento)){
		  if(!connected) pDlg->writeOnLog("No conecta con el puerto de accionamientos"); 
			connected = true;
			firststr = false;
      mtx.unlock();
      pDlg->m_statusAcondicionamiento.m_color = 1;
      pDlg->m_statusAcondicionamiento.Invalidate(true);
			pDlg->m_setTimer2 = pDlg->m_setTimer3 = false;
			pDlg->KillTimer(2); pDlg->KillTimer(3);
			pDlg->m_freno.m_color = pDlg->m_izquierdo.m_color = pDlg->m_derecho.m_color = 5;
			pDlg->m_freno.Invalidate(true); pDlg->m_izquierdo.Invalidate(true); pDlg->m_derecho.Invalidate(true);
      pDlg->PostMessage(WM_FIN_HILO,2);
		  continue;
	  }	
		else connected = false;
    mtx.unlock(); 
    pDlg->m_statusAcondicionamiento.m_color = 0;
    pDlg->m_statusAcondicionamiento.Invalidate(true);
		misoc.Send(buf, 20);
		unsigned char rec_buf[20];
		int len = misoc.Receive(rec_buf,20); 
		if(rec_buf[7] == 0x04 && rec_buf[8] == 0x06 && rec_buf[5] == (3+2*n_data) && rec_buf[0]*256+rec_buf[1] == buf[0]*256+buf[1]){
			// data is correct
			bool freno = rec_buf[10];
			pDlg->luces[0] = freno;
			bool izq = rec_buf[12];
			pDlg->luces[1] = izq; pDlg->luces[3] = izq; 
			bool der = rec_buf[14];
			pDlg->luces[2] = der; pDlg->luces[4] = der; 
			if(freno) pDlg->m_freno.m_color = 1;
			else pDlg->m_freno.m_color = 5;
			pDlg->m_freno.Invalidate(true);
			if(izq){
				if(!pDlg->m_setTimer2){
					pDlg->m_flag2 = false;
					if (!pDlg->m_setTimer3) pDlg->SetTimer(2, 500, NULL);
					pDlg->m_setTimer2 = true;
				}
			}else{
				if(!pDlg->m_setTimer3) pDlg->KillTimer(2);
				pDlg->m_izquierdo.m_color = 5;
				pDlg->m_izquierdo.Invalidate(true);
				pDlg->m_setTimer2 = false;
			}
      if(der){
				if(!pDlg->m_setTimer3){
					pDlg->m_flag3 = false;
					if(!pDlg->m_setTimer2) pDlg->SetTimer(2, 500, NULL);
					pDlg->m_setTimer3 = true;
				}
			}else{
				if(!pDlg->m_setTimer2)pDlg->KillTimer(2);
				pDlg->m_derecho.m_color = 5;
				pDlg->m_derecho.Invalidate(true);
				pDlg->m_setTimer3 = false;
			}
			if(!firststr) pDlg->writeOnLog("Accionamientos OK");
			firststr = true;
		}else if(firststr){
			pDlg->writeOnLog("Error en comunicaci�n con los accionamientos. No se han recibido 3 datos");
			firststr = false;
		}
		pDlg->m_numMsg++;
    pDlg->PostMessage(WM_FIN_HILO,2);
    mtx.lock();
	  misoc.Close();
	  mtx.unlock();
  }
	return 0;
}

UINT Luces(LPVOID lp){
	static bool connected = false;
	static bool firststr = false;
  auto *pDlg = (CCentralitaDlg*) lp;
	while(pDlg->m_life){
		pDlg->m_fin = false;
		while(!pDlg->m_flag){}
		
		// connection algorithm
		unsigned char buf[20];
		unsigned char rec_buf[20];
    CSocket misoc;
		mtx.lock();
		if(!misoc.Create()){ 
			pDlg->writeOnLog("Error al crear socket"); 
      mtx.unlock(); 
      pDlg->m_statusLuces.m_color = 1;
      pDlg->m_statusLuces.Invalidate(true);
      pDlg->PostMessage(WM_FIN_HILO,3);
			continue;
		}
		if(!misoc.Connect(pDlg->m_ipLuces, pDlg->m_portLuces)){
			if(!connected) pDlg->writeOnLog("No conecta con el puerto de luces"); 
			connected = true;
			firststr = false;
      mtx.unlock(); 
      pDlg->m_statusLuces.m_color = 1;
      pDlg->m_statusLuces.Invalidate(true);
      pDlg->PostMessage(WM_FIN_HILO,3);
			continue;
		}
		else connected = false;
    mtx.unlock();
    pDlg->m_statusLuces.m_color = 0;
    pDlg->m_statusLuces.Invalidate(true);

		int ok = 0;
		for(size_t i = 0; i<5; i++){
			pDlg->ModBusObj.constructBuffer(buf, pDlg->m_numMsg, 23, 6, 500+i, pDlg->luces[i]);
			misoc.Send(buf, 20);
			misoc.Receive(rec_buf,20); 
			if(memcmp(buf, rec_buf, 20) == 0) ok++;
			pDlg->m_numMsg++;
		}

		if(ok == 5){
			// data is correct
			if(!firststr){
				pDlg->writeOnLog("Luces OK"); 
				firststr = true;
			}
		}
		else{
			pDlg->writeOnLog("Error en comunicaci�n con las luces. No se han recibido 3 datos"); firststr = false;
		}
		pDlg->PostMessage(WM_FIN_HILO,3); 
		mtx.lock();
		misoc.Close();
		mtx.unlock();
	}
	return 0;
}

LRESULT CCentralitaDlg::OnFinHilo(WPARAM wParam, LPARAM lParam)
{
	m_flag = false;
	return 0;
}

void CCentralitaDlg::OnBnClickedbnstart()
{
	UpdateData(1);
	static bool isStart = false;
	TCHAR s[100];
	DWORD a = GetCurrentDirectory(100, s);

	if(!isStart){
		GetDlgItem(bnStart)->SetWindowText("Stop");
		writeOnLog("Polling started"); 
		std::string cs = std::string(s) + "\\ping.wav";
		PlaySound(cs.c_str(), NULL, SND_ASYNC);
		for(CWinThread* thread:threads) thread->ResumeThread();
		SetTimer(1, m_tiempo, NULL);

	}else{
		GetDlgItem(bnStart)->SetWindowText("Start");
		writeOnLog("Polling stopped"); 
		std::string cs = std::string(s) + "\\buzzer.wav";
		PlaySound(cs.c_str(), NULL, SND_ASYNC);
		for(CWinThread* thread:threads) thread->SuspendThread();
		KillTimer(1);
	}
	isStart = !isStart;
}

void CCentralitaDlg::OnTimer(UINT_PTR nIDEvent)
{
  if(nIDEvent == 1) m_flag = true; // unblock the threads
	else{
		if(m_setTimer2 && !m_setTimer3){
			if(m_flag2) m_izquierdo.m_color = 2;
			else m_izquierdo.m_color = 5;
			m_izquierdo.Invalidate(true);
			m_flag2 = !m_flag2;
		}
		if(m_setTimer3 && !m_setTimer2){
			if(m_flag3)m_derecho.m_color = 2;
			else m_derecho.m_color = 5;
			m_derecho.Invalidate(true);
			m_flag3 = !m_flag3;
		}
		if(m_setTimer3 && m_setTimer2){
			if(m_flag3){
				m_derecho.m_color = m_izquierdo.m_color = 2;
			}else{
				m_derecho.m_color = m_izquierdo.m_color = 5;
			}
			m_derecho.Invalidate(true); m_izquierdo.Invalidate(true);
			m_flag3 = !m_flag3;
			m_flag2 = m_flag3;
		}
	}
}

 void CCentralitaDlg::writeOnLog(CString str){
	std::time_t result = std::time(nullptr);
	CString toWrite = std::asctime(std::localtime(&result));
	toWrite += ": " + str;
	m_log.AddString(toWrite);
 }


 void CCentralitaDlg::OnBnClickedbnclear()
 {
	 m_log.ResetContent();
 }


 void CCentralitaDlg::OnCancel()
 {
	 m_life = false;
	 CDialogEx::OnCancel();
 }
