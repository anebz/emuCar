#pragma once

// CModbus command target

class CLucesDlg;

class CModbus : public CSocket
{
public:
	CModbus(CLucesDlg*);
	CLucesDlg* pDlg;
	short msg;

	virtual ~CModbus();
	virtual void OnAccept(int nErrorCode);
	unsigned char* Protocol(short add, int slider);
};


