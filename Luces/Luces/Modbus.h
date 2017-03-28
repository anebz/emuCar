#pragma once
#include <vector>
#include "Led.h"

class CLucesDlg;

class CModbus
{
public:
	CModbus(void);
	~CModbus(void);

	bool Protocol(std::vector<CLed*> luces, const unsigned char* buf);
};

