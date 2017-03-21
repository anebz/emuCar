// Modbus.cpp : implementation file
//

#include "stdafx.h"
#include "Luces.h"
#include "Modbus.h"
#include "LucesDlg.h"

// CModbus

CModbus::CModbus(CLucesDlg* p)
{
	pDlg = p;
	msg = 1;
}

CModbus::~CModbus()
{
}


// CModbus member functions

unsigned char* CModbus::Protocol(short add, int slider)
{
	unsigned char Bus[20];

	Bus[0] = msg >> 8; // MSB
	Bus[1] = msg & 0xFF; // LSB

	Bus[2] = 0x00; // Protocol Identifier
	Bus[3] = 0x00;	

	// ID slave
	Bus[6] = 2 & 0XFF; // ID esclavo3 = 2

	// function code (write)
	Bus[7] = 0x06; // = 6 (dec)

	// Data address
	short Trans = add;
	Bus[8] = Trans >> 8;
	Bus[9] = Trans & 0xFF;

	// value
	Bus[10] = slider >> 8;
	Bus[11] = slider & 0xFF;

	// meessage length
	short length = 12;
	Bus[4] = (length - 1 - 5) >> 8;
	Bus[5] = (length - 1 - 5) & 0xFF;

	return Bus;
}


void CModbus::OnAccept(int nErrorCode)
{
	pDlg->UpdateData(1);
	CSocket misoc;
	if(!misoc.Create()){ 
		pDlg->MessageBox("Error"); return;
	}
	if(!misoc.Connect("127.0.0.1", pDlg->m_port)){
		pDlg->MessageBox("No conecta.."); return;
	}	

	unsigned char rec_buf[20];
	int len = misoc.Receive(rec_buf,20); 

	// interpretar protocolo


	CSocket::OnAccept(nErrorCode);
}