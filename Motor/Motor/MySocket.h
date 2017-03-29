#pragma once

// CMySocket command target
class CMotorDlg;

class CMySocket : public CSocket
{
public:
	CMySocket(CMotorDlg*);
	CMotorDlg* pDlg;

	virtual ~CMySocket();
	virtual void OnAccept(int nErrorCode);
};


