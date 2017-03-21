// Modbus.cpp : implementation file
//

#include "stdafx.h"
#include "Motor.h"
#include "Modbus.h"
#include "MotorDlg.h"
// CModbus

CModbus::CModbus(CMotorDlg* p)
{
	pDlg = p;
	msg = 1;
}

CModbus::~CModbus()
{
}


// CModbus member functions

void CModbus::Protocol(short add, int slider, unsigned char* Bus)
{
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

}


void CModbus::OnAccept(int nErrorCode)
{
	unsigned char rec_buf[20];
	unsigned char Bus[20];
	pDlg->UpdateData(1);
	char buf[50];
	CSocket cliente; 
	Accept(cliente);
	while(1){
		int len = cliente.Receive(buf,50); 
		if (len == 0 || len == -1) break;

		Protocol(400, pDlg->m_sl_temp*30, Bus);
		cliente.Send(Bus, 20);

		Protocol(401, pDlg->m_sl_rpm*700, Bus);
		cliente.Send(Bus, 20);

	}
	pDlg->UpdateData(0);
	cliente.Close();

	msg++; // # mensajes mandados se incrementa
	CSocket::OnAccept(nErrorCode);
}
