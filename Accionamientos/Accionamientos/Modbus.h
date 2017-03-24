#pragma once

// CModbus command target

class CAccionamientosDlg;

class CModbus : public CSocket
{
public:
	CModbus(CAccionamientosDlg*);
	CAccionamientosDlg* pDlg;
	virtual ~CModbus();
	virtual void OnAccept(int nErrorCode);
};


