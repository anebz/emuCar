#include "stdafx.h"
#include "Modbus.h"


CModbus::CModbus()
{
}


CModbus::~CModbus(void)
{
}


bool CModbus::Protocol(const unsigned char* buf, unsigned char* Bus, int temp, int rpm)
{
	bool error = 0;
	if(buf[6] != 21) error = 1;
	else if(buf[7] == 0x04){ // read mode
			
		int add = buf[8]*256 + buf[9];
		int num = buf[10]*256 + buf[11];

		for(size_t i = 0; i<7; i++){
			Bus[i] = buf[i];
		}

		Bus[7] = 0x04; // function mode
		Bus[8] = (num*2) & 0xFF; // byte count

		if((add == 400 || add == 401) && num <= 2){
			int pos = 9;
			for(size_t i = 400 + (num-2); i <= 401; i++){
				if(i == 400){
					Bus[pos++] = temp*3 >> 8; 
					Bus[pos++] = temp*3 & 0xFF;
				}else if(i == 401){
					Bus[pos++] = rpm*70 >> 8; 
					Bus[pos] = rpm*70 & 0xFF;
				}
			}
			Bus[5] = pos - 5; // length
		}else error = 1;
	}else error = 1;

	return error;
}