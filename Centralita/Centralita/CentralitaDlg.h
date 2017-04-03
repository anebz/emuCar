
// CentralitaDlg.h: archivo de encabezado
//

#pragma once
#include "afxwin.h"
#include "Led.h"
#include "Gauge.h"
#include <vector>
#include "ModBus.h"

// Cuadro de diálogo de CCentralitaDlg
class CCentralitaDlg : public CDialogEx
{
// Construcción
public:
	CCentralitaDlg(CWnd* pParent = NULL);	// Constructor estándar
  ~CCentralitaDlg(){m_life = false;}
	int m_numMsg;

// Datos del cuadro de diálogo
	enum { IDD = IDD_CENTRALITA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV


// Implementación
protected:
	HICON m_hIcon;

	// Funciones de asignación de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
  CString m_ipMotor;
//  CString m_portMotor;
  CString m_ipAcondicionamiento;
  int m_portMotor;
  int m_portAcondicionamiento;
  CString m_ipLuces;
  int m_portLuces;
  int m_tiempo;
  CListBox m_log;
  CLed m_statusMotor, m_statusAcondicionamiento, m_statusLuces;
  CLed m_izquierdo, m_derecho, m_freno;
  CGauge m_imTemperatura, m_imRPM;
  CString m_temperatura;
  CString m_RPM;
	afx_msg void OnBnClickedbnstart();
  afx_msg LRESULT OnFinHilo(WPARAM wParam, LPARAM lParam); 
  std::vector<CWinThread*> threads;
  bool m_start_stop;
  bool m_flag;
  bool m_life;
  bool m_fin;
	bool m_flag2;
	bool m_flag3;
	bool m_setTimer2;
	bool m_setTimer3;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
  void writeOnLog(CString str);
  CModBus ModBusObj;
	afx_msg void OnBnClickedbnclear();
};
