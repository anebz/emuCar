#pragma once


// CModBus

class CModBus : public CWnd
{
	DECLARE_DYNAMIC(CModBus)

public:
	CModBus();
	virtual ~CModBus();
  unsigned char* constructBuffer(int messageID, int slaveID, int functionCode, int dataAddress, int value);
protected:
	DECLARE_MESSAGE_MAP()
};


