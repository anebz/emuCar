#pragma once

// CMySocket command target
class CAccionamientosDlg;

class CMySocket : public CSocket
{
public:
	CMySocket(CAccionamientosDlg*);
	CAccionamientosDlg* pDlg;

	virtual ~CMySocket();
	virtual void OnAccept(int nErrorCode);
};


