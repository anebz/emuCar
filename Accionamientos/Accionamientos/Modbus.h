#pragma once

// CModbus command target

class CModbus : public CSocket
{
public:
	CModbus();
	virtual ~CModbus();
	virtual void OnAccept(int nErrorCode);
};


