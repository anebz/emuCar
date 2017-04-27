// MySocket.cpp : implementation file
//

#include "stdafx.h"
#include "Accionamientos.h"
#include "MySocket.h"
#include "AccionamientosDlg.h"
#include "Modbus.h"

// CMySocket

CMySocket::CMySocket(CAccionamientosDlg* p)
{
	pDlg = p;
}

CMySocket::~CMySocket()
{
}


// CMySocket member functions


void CMySocket::OnAccept(int nErrorCode)
{
	unsigned char Bus[20];
	unsigned char buf[20];

	pDlg->UpdateData(1);
	CSocket cliente; 
	Accept(cliente);
	cliente.Receive(buf,20); 

	CModbus mod;
	bool error = mod.Protocol(buf, Bus, pDlg->m_button);

	if(!error) cliente.Send(Bus, 20);
	else cliente.Send("", 20); // send empty string, the other part will interpret as error

	cliente.Close();
	CSocket::OnAccept(nErrorCode);
}
