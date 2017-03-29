// MySocket.cpp : implementation file
//

#include "stdafx.h"
#include "Motor.h"
#include "MySocket.h"
#include "MotorDlg.h"
#include "Modbus.h"
// CMySocket

CMySocket::CMySocket(CMotorDlg* p)
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
	unsigned char rec_buf[20];

	pDlg->UpdateData(1);
	CSocket cliente; 
	Accept(cliente);

	int len = cliente.Receive(rec_buf,20); 

	CModbus mod;
	bool error = mod.Protocol(rec_buf, Bus, pDlg->m_sl_temp, pDlg->m_sl_rpm);

	if(!error) cliente.Send(Bus, 20);
	else cliente.Send("", 20); // send any message, the other part will interpret as error

	cliente.Close();
	CSocket::OnAccept(nErrorCode);
}
