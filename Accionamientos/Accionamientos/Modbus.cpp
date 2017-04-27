#include "stdafx.h"
#include "Modbus.h"


CModbus::CModbus()
{
}


CModbus::~CModbus(void)
{
}


bool CModbus::Protocol(const unsigned char* buf, unsigned char* Bus, const BOOL* m_button)
{
	bool error = 0;
	if(buf[6] != 22) error = 1;
	else if(buf[7] == 0x04){ // read more
			
		int add = buf[8]*256 + buf[9];
		int num = buf[10]*256 + buf[11];

		for(size_t i = 0; i<7; i++) Bus[i] = buf[i];

		Bus[7] = 0x04; // function mode
		Bus[8] = (num*2) & 0xFF; // byte count

		if(num > 0 && num < 4 && add + num < 404){ // en el rango de 400 y 3 add, o 401 y 2 add
			int pos = 8;
			for(size_t i = 0; i<num; i++){
				Bus[++pos] = 0x00;
				Bus[++pos] = m_button[i + add - 400] & 0xFF;
			}
			Bus[5] = pos - 5; // length
		}else error = 1;
	}else error = 1;
	return error;
}