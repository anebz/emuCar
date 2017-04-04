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
//	ON_WM_TIMER()
END_MESSAGE_MAP()


bool CModbus::Protocol(std::vector<CLed*> luces, unsigned char* buf)
{
	bool error = 0;
	if(buf[6] != 23) error = 1;
	else if(buf[7] == 0x06){ // write mode
		int add = buf[8]*256 + buf[9];
		int num = buf[10]*256 + buf[11];
		if(num == 1 || num == 0){
			if(add == 500){
				luces[add-500]->red = 1;
				luces[add-500]->on = (bool)num;
			}
			luces[add-500]->activado = (bool)num;
			luces[add-500]->Invalidate();
		}else error = 1;
	}else error = 1;

	return error;
}
