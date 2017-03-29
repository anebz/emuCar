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

unsigned char* CModBus::constructBuffer(int messageID, int slaveID, int functionCode, int dataAddress, int value, unsigned char* buf){
  if(functionCode == 4){ // PARA READ
	  buf[0] = messageID >> 8;
	  buf[1] = messageID & 0xFF; // transaction identifier
	  buf[2] = 0x00;
	  buf[3] = 0x00; // protocol identifier, 0 in Modbus

	  /* HACER DINAMICO!!!!*/
    buf[4] = 0x00;
	  buf[5] = 0x06; // length
	  /*********************/

    buf[6] = slaveID & 0xFF; // ID motor
	  buf[7] = 0x04; // function code, read
	  short add = dataAddress; 
	  buf[8] = add >> 8;
	  buf[9] = add & 0xFF; // data address
    short value_ = value;
	  buf[10] = value_ >> 8;
	  buf[11] = value_ & 0xFF;
    return buf;
  }else if(functionCode == 6){
  // TO DO para el write
  }
}


// Controladores de mensajes de CModBus


