
// CentralitaDlg.h: archivo de encabezado
//

#pragma once
#include "afxwin.h"
#include "Led.h"
#include "Gauge.h"
#include <vector>
#include "ModBus.h"
#include <string>

// Cuadro de di�logo de CCentralitaDlg
class CCentralitaDlg : public CDialogEx
{
// Construcci�n
public:
	CCentralitaDlg(CWnd* pParent = NULL);	// Constructor est�ndar
  ~CCentralitaDlg(){m_life = false;}
	int m_numMsg;
	bool luces[5];

// Datos del cuadro de di�logo
	enum { IDD = IDD_CENTRALITA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV


// Implementaci�n
protected:
	HICON m_hIcon;

	// Funciones de asignaci�n de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
  CString m_ipMotor, m_ipAcondicionamiento, m_ipLuces;
	CString m_temperatura, m_RPM;
  int m_portMotor, m_portAcondicionamiento, m_portLuces;
  int m_tiempo;
	bool m_flag, m_life,m_fin;
	bool m_flag2, m_flag3;
	bool m_setTimer2,m_setTimer3;
  CListBox m_log;
  CLed m_statusMotor, m_statusAcondicionamiento, m_statusLuces;
  CLed m_izquierdo, m_derecho, m_freno;
  CGauge m_imTemperatura, m_imRPM;
	CModBus ModBusObj;
	std::vector<CWinThread*> threads;

	afx_msg void OnBnClickedbnstart();
  afx_msg LRESULT OnFinHilo(WPARAM wParam, LPARAM lParam); 
 	afx_msg void OnTimer(UINT_PTR nIDEvent);
  void writeOnLog(CString str);
	afx_msg void OnBnClickedbnclear();
	virtual void OnCancel();
};
