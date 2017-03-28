// MySock.cpp : implementation file
//

#include "stdafx.h"
#include "Luces.h"
#include "MySock.h"
#include "LucesDlg.h"

// CMySock

CMySock::CMySock(CLucesDlg* p)
{
	pDlg = p;
}

CMySock::~CMySock()
{
}


// CMySock member functions

void CMySock::OnAccept(int nErrorCode)
{
	unsigned char buf[20];

	pDlg->UpdateData(1);
	CSocket cliente; 
	Accept(cliente);

	while(1){
		int len = cliente.Receive(buf,20); 
		if(len == 0 || len == -1) break;

		CModbus mod;
		bool error = mod.Protocol(pDlg->luces, buf);

		if(!error) cliente.Send(buf, 20);
		else cliente.Send("", 20); // send any message, the other part will interpret as error
	}

	cliente.Close();

	CSocket::OnAccept(nErrorCode);
}
