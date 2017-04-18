#include "stdafx.h"
#include "Modbus.h"

IMPLEMENT_DYNAMIC(CModbus, CWnd)

CModbus::CModbus()
{

}


CModbus::~CModbus(void)
{
}

BEGIN_MESSAGE_MAP(CModbus, CWnd)
END_MESSAGE_MAP()


bool CModbus::Protocol(std::vector<CLed*> lights, unsigned char* buf)
{
	if(buf[6] == 23 && buf[7] == 0x06){ // correct ID and write mode
		int add = buf[8]*256 + buf[9];
		short data = buf[10]*256 + buf[11];
		if(data == 1 || data == 0){ // boolean received, 1 or 0
			if(add == 500){
				lights[add-500]->red = 1;
				lights[add-500]->on = (bool)data;
			}
			lights[add-500]->activado = (bool)data;
			lights[add-500]->Invalidate();
		}else return 1;
	}else return 1;

	return 0;
}
