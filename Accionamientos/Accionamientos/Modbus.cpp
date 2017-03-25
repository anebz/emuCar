// Modbus.cpp : implementation file
//

#include "stdafx.h"
#include "Accionamientos.h"
#include "Modbus.h"
#include "AccionamientosDlg.h"

// CModbus

CModbus::CModbus(CAccionamientosDlg* p)
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

		if(num > 0 && num < 4 && add + num < 404){ // en el rango de 400 y 3 add, o 401 y 2 add
			Bus[4] = 0x04; // function mode
			Bus[5] = (num*2) & 0xFF; // byte count
			int pos = 5;
			pDlg->UpdateData(1);
			for(size_t i = 0; i<num; i++){
				Bus[++pos] = 0x00;
				Bus[++pos] = pDlg->m_button[i + add - 400] & 0xFF;
			}
		}else error = 1;
	}else error = 1;

	if(!error) cliente.Send(Bus, 20);
	else cliente.Send("", 20); // send any message, the other part will interpret as error

	cliente.Close();
	
	CSocket::OnAccept(nErrorCode);
}
