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
}

CModbus::~CModbus()
{
}


// CModbus member functions


void CModbus::OnAccept(int nErrorCode)
{
	unsigned char Bus[20];
	unsigned char buf[20];

	pDlg->UpdateData(1);
	CSocket cliente; 
	Accept(cliente);

	bool error = 0;
	int len = cliente.Receive(buf,20); 

	if(buf[4] == 0x04){ // read more
			
		int add = buf[5]*256 + buf[6];
		int num = buf[7]*256 + buf[8];

		Bus[0] = buf[0];
		Bus[1] = buf[1];
		Bus[2] = buf[2];
		Bus[3] = buf[3];

		Bus[4] = 0x04; // function mode
		Bus[5] = (num*2) & 0xFF; // byte count

		if(num == 1){
			if(add == 400){
				Bus[6] = pDlg->m_sl_temp*30 % 0xFF; // input registers
				Bus[7] = pDlg->m_sl_temp*30 >> 8 ;// input registers
			}else if(add == 401){
				Bus[6] = pDlg->m_sl_rpm*700 % 0xFF; // input registers
				Bus[7] = pDlg->m_sl_rpm*700 >> 8; // input registers
			}else error = 1;

		}else if(num == 2 && add == 400){
			Bus[6] = pDlg->m_sl_temp*30 >> 8; // input registers
			Bus[7] = pDlg->m_sl_temp*30 & 0xFF ;// input registers
			Bus[8] = pDlg->m_sl_rpm*700 >> 8; // input registers
			Bus[9] = pDlg->m_sl_rpm*700 & 0xFF; // input registers
		}else error = 1;
	}else error = 1;

	if(!error) cliente.Send(Bus, 20);
	else cliente.Send("", 20); // send any message, the other part will interpret as error

	cliente.Close();
	CSocket::OnAccept(nErrorCode);
}
