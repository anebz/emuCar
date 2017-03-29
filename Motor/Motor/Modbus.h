#pragma once

class CModbus
{

public:
	CModbus(void);
	~CModbus(void);

	bool Protocol(const unsigned char* buf, unsigned char* Bus, int temp, int rpm);
};
