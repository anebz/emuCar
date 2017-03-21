
// CentralitaDlg.h: archivo de encabezado
//

#pragma once
#include "afxwin.h"
#include "Led.h"

// Cuadro de diálogo de CCentralitaDlg
class CCentralitaDlg : public CDialogEx
{
// Construcción
public:
	CCentralitaDlg(CWnd* pParent = NULL);	// Constructor estándar

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
  CString m_temperatura;
  CString m_RPM;
};
