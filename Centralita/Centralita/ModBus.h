#pragma once


// CModBus

class CModBus : public CWnd
{
	DECLARE_DYNAMIC(CModBus)

public:
	CModBus();
	virtual ~CModBus();
  void constructBuffer(unsigned char* buf, int messageID, int slaveID, int functionCode, short dataAddress, int value);
protected:
	DECLARE_MESSAGE_MAP()
};


