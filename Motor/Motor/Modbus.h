#pragma once

// CModbus command target
class CMotorDlg;

class CModbus : public CSocket
{
public:
	CModbus(CMotorDlg*);
	CMotorDlg* pDlg;

	virtual ~CModbus();
	virtual void OnAccept(int nErrorCode);
};


