#pragma once
#include <vector>
#include "Led.h"

class CModbus : public CWnd
{
	DECLARE_DYNAMIC(CModbus);
public:
	CModbus(void);
	~CModbus(void);

	bool Protocol(std::vector<CLed*> luces, unsigned char* buf);
	protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

