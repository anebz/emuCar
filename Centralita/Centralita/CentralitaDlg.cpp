
// CentralitaDlg.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Centralita.h"
#include "CentralitaDlg.h"
#include "afxdialogex.h"
#include <ctime>
#include <mutex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_FIN_HILO WM_USER+100
std::mutex mtx;

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
END_MESSAGE_MAP()


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
	m_numMsg = 1;

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
/*COMUNICACION CON MOTOR
	unsigned char buf[20];

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
	}else m_log.AddString("Error en comunicación con el motor. No se han recibido 3 datos");
	m_numMsg++;*/

/*COMUNICACION CON ACCIONAMIENTOS
	unsigned char buf[20];

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
	}else m_log.AddString("Error en comunicación con los accionamientos. No se han recibido 3 datos");
	m_numMsg++;*/
	
/*COMUNICACION CON LUCES
	unsigned char buf[20];
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
	else m_log.AddString("Error en comunicación con las luces");
	m_numMsg++;*/

// Controladores de mensaje de CCentralitaDlg
UINT Motor(LPVOID lp){
  auto *pDlg = (CCentralitaDlg*) lp;
  while(pDlg->m_life){
    while(!pDlg->m_flag){}
    // ALGORITMO DE CONEXIÓN!!! 
		unsigned char buf[20];
    mtx.lock();
    int checker = pDlg->m_numMsg;
    pDlg->ModBusObj.constructBuffer(pDlg->m_numMsg, 21, 4, 400, 2, buf);
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
		if(!misoc.Connect("127.0.0.1", 502)){
			pDlg->writeOnLog("No conecta con puerto 502"); 
      mtx.unlock();
      pDlg->m_statusMotor.m_color = 1;
      pDlg->m_statusMotor.Invalidate(true);
      pDlg->PostMessage(WM_FIN_HILO,1);
			continue;
		}	
    mtx.unlock();
    pDlg->m_statusMotor.m_color = 0;
    pDlg->m_statusMotor.Invalidate(true);
		misoc.Send(buf, 20);
		unsigned char rec_buf[20];
		int len = misoc.Receive(rec_buf,20); 

		if(rec_buf[7] == 0x04 && rec_buf[8] == 0x04 && rec_buf[0]*256 + rec_buf[1] == checker){
			int temp = rec_buf[9]*256 + rec_buf[10]; 
      temp /= 3; // regla de tres, para ajustarlo a los valores máximos
      pDlg->m_imTemperatura.m_nivel = temp;
			int rpm = rec_buf[11]*256 + rec_buf[12];
      rpm /= 70;
      pDlg->m_imRPM.m_nivel = rpm;
			pDlg->writeOnLog("Motor OK");
      pDlg->m_imTemperatura.Invalidate(true);
      pDlg->m_imRPM.Invalidate(true);
		}else pDlg->writeOnLog("Error en comunicación con el motor. No se han recibido 3 datos");
		pDlg->m_numMsg++;
	  // process rec_buf --> temperature
	  // process rec_buf --> rpm
    pDlg->PostMessage(WM_FIN_HILO,1); // CAMBIAR ESTO DEPENDIENDO DEL PROTOCOLO
    mtx.lock();
		misoc.Close();
    mtx.unlock();
    Sleep(pDlg->m_tiempo / 100);
  }
	return 0;
}

UINT Acondicionamiento(LPVOID lp){
  auto *pDlg = (CCentralitaDlg*) lp;
  while(pDlg->m_life){
		pDlg->m_fin = false;
    while(!pDlg->m_flag){}
    // ALGORITMO DE CONEXIÓN!!! 
		unsigned char buf[20];
    mtx.lock();
    pDlg->ModBusObj.constructBuffer(pDlg->m_numMsg, 22, 4, 400, 3, buf);
    mtx.unlock();
    CSocket misoc;
    mtx.lock();
	  if(!misoc.Create()){ 
		  pDlg->writeOnLog("Error al crear socket"); 
      mtx.unlock();
      pDlg->m_statusAcondicionamiento.m_color = 1;
      pDlg->m_statusAcondicionamiento.Invalidate(true);
      pDlg->PostMessage(WM_FIN_HILO,2);
		  continue;
	  }
	  if(!misoc.Connect("127.0.0.1", 503)){
		  pDlg->writeOnLog("No conecta con puerto 503");
      mtx.unlock();
      pDlg->m_statusAcondicionamiento.m_color = 1;
      pDlg->m_statusAcondicionamiento.Invalidate(true);
      pDlg->PostMessage(WM_FIN_HILO,2);
		  continue;
	  }	
    mtx.unlock(); 
    pDlg->m_statusAcondicionamiento.m_color = 0;
    pDlg->m_statusAcondicionamiento.Invalidate(true);
		misoc.Send(buf, 20);
		unsigned char rec_buf[20];
		int len = misoc.Receive(rec_buf,20); 
		if(rec_buf[7] == 0x04 && rec_buf[8] == 0x06){
			bool freno = rec_buf[10];
			bool izq = rec_buf[12];
			bool der = rec_buf[14];
      pDlg->m_freno.m_color = !freno;
      pDlg->m_izquierdo.m_color = !izq;
      pDlg->m_derecho.m_color = !der;
      pDlg->m_freno.Invalidate(true);
      pDlg->m_izquierdo.Invalidate(true);
      pDlg->m_derecho.Invalidate(true);
			pDlg->writeOnLog("Accionamientos OK");
		}else pDlg->writeOnLog("Error en comunicación con los accionamientos. No se han recibido 3 datos");
		pDlg->m_numMsg++;
    pDlg->PostMessage(WM_FIN_HILO,2); // CAMBIAR ESTO DEPENDIENDO DEL PROTOCOLO
    mtx.lock();
	  misoc.Close();
	  mtx.unlock();
		Sleep(pDlg->m_tiempo / 100);
  }
	return 0;
}

UINT Luces(LPVOID lp){
  auto *pDlg = (CCentralitaDlg*) lp;
	while(pDlg->m_life){
		pDlg->m_fin = false;
		while(!pDlg->m_flag){}
		// ALGORITMO DE CONEXIÓN!!! 
		unsigned char buf[20];
		unsigned char rec_buf[20];
		buf[0] = pDlg->m_numMsg >> 8;
		buf[1] = pDlg->m_numMsg & 0xFF; // transaction identifier
		buf[2] = 0x00;
		buf[3] = 0x00; // protocol identifier, 0 in Modbus
		buf[4] = 0x00;
		buf[5] = 0x06; // length
		buf[6] = 0x23; // ID luces
		buf[7] = 0x06; // function code, write
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
		if(!misoc.Connect("127.0.0.1", 504)){
			pDlg->writeOnLog("No conecta con puerto 504"); 
      mtx.unlock(); 
      pDlg->m_statusLuces.m_color = 1;
      pDlg->m_statusLuces.Invalidate(true);
      pDlg->PostMessage(WM_FIN_HILO,3);
			continue;
		}
    mtx.unlock();
    pDlg->m_statusLuces.m_color = 0;
    pDlg->m_statusLuces.Invalidate(true);
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

		if(ok == 5) pDlg->writeOnLog("Luces OK");
		else pDlg->writeOnLog("Error en comunicación con las luces");
		pDlg->m_numMsg++;
		
		pDlg->PostMessage(WM_FIN_HILO,3); // CAMBIAR ESTO DEPENDIENDO DEL PROTOCOLO
		mtx.lock();
		misoc.Close();
		mtx.unlock();
		Sleep(pDlg->m_tiempo / 100);
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
  if(m_start_stop){
    for(size_t ii = 0; ii < threads.size(); ii++){
      threads.at(ii)->SuspendThread();
    }
    m_start_stop = false;
		KillTimer(1);
  }
  else{
    for(size_t ii = 0; ii < threads.size(); ii++){
      threads.at(ii)->ResumeThread();
    }
    m_start_stop = true;
		SetTimer(1, m_tiempo, NULL);
  }
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
