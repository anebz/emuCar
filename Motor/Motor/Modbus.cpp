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

	if(buf[0] == 0x04){ // read more
			
		int add = buf[1]*256 + buf[2];
		int num = buf[3]*256 + buf[4];

		if(num == 1){
			Bus[0] = 0x04; // function mode
			Bus[1] = (num*2) & 0xFF; // byte count

			if(add == 400){
				Bus[2] = pDlg->m_sl_temp*30 % 0xFF; // input registers
				Bus[3] = pDlg->m_sl_temp*30 >> 8 ;// input registers
			}else if(add == 401){
				Bus[2] = pDlg->m_sl_rpm*700 % 0xFF; // input registers
				Bus[3] = pDlg->m_sl_rpm*700 >> 8; // input registers
			}else error = 1;

		}else if(num == 2 && add == 400){
			Bus[0] = 0x04; // function mode
			Bus[1] = (num*2) & 0xFF; // byte count
			Bus[2] = pDlg->m_sl_temp*30 >> 8; // input registers
			Bus[3] = pDlg->m_sl_temp*30 & 0xFF ;// input registers
			Bus[4] = pDlg->m_sl_rpm*700 >> 8; // input registers
			Bus[5] = pDlg->m_sl_rpm*700 & 0xFF; // input registers
		}else error = 1;
	}else error = 1;

	if(!error) cliente.Send(Bus, 20);
	else cliente.Send("", 20); // send any message, the other part will interpret as error

	cliente.Close();
	CSocket::OnAccept(nErrorCode);
}
