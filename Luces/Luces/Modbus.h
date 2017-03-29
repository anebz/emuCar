#pragma once
#include <vector>
#include "Led.h"

class CModbus
{
public:
	CModbus(void);
	~CModbus(void);

	bool Protocol(std::vector<CLed*> luces, const unsigned char* buf);
};

