// ModBus.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Centralita.h"
#include "ModBus.h"


// CModBus

IMPLEMENT_DYNAMIC(CModBus, CWnd)

CModBus::CModBus()
{

}

CModBus::~CModBus()
{
}


BEGIN_MESSAGE_MAP(CModBus, CWnd)
END_MESSAGE_MAP()

void CModBus::constructBuffer(unsigned char* buf, int messageID, int slaveID, int functionCode, short dataAddress, int value){
  
	buf[0] = messageID >> 8;
	buf[1] = messageID & 0xFF; // transaction identifier

	buf[2] = 0x00;
	buf[3] = 0x00; // protocol identifier, 0 in Modbus

	buf[4] = 0x00;
  buf[5] = 0x06; // length, always 6 in write and read modes

	buf[6] = slaveID & 0xFF; // ID

	buf[7] = functionCode; // 0x06 or 0x04

	buf[8] = dataAddress >> 8;
	buf[9] = dataAddress & 0xFF; 

	buf[10] = value >> 8;
	buf[11] = value & 0xFF;

	return;
}