#pragma once

// CModbus command target
class CMotorDlg;

class CModbus : public CSocket
{
public:
	CModbus(CMotorDlg*);
	CMotorDlg* pDlg;
	unsigned int msg;

	virtual ~CModbus();
	virtual void OnAccept(int nErrorCode);
	void Protocol(short add, int slider, unsigned char* Bus);
};


