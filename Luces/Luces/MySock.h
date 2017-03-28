#pragma once
// CMySock command target
#include "Modbus.h"
class CLucesDlg;

class CMySock : public CSocket
{
public:
	CMySock(CLucesDlg*);
	CLucesDlg* pDlg;

	virtual ~CMySock();
	virtual void OnAccept(int nErrorCode);
};


