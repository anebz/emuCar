// Modbus.cpp : implementation file
//

#include "stdafx.h"
#include "Accionamientos.h"
#include "Modbus.h"


// CModbus

CModbus::CModbus()
{
}

CModbus::~CModbus()
{
}


// CModbus member functions


void CModbus::OnAccept(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class

	CSocket::OnAccept(nErrorCode);
}
