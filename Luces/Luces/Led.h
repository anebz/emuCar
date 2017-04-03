#pragma once


// CLed

class CLed : public CWnd
{
	DECLARE_DYNAMIC(CLed)

public:
	CLed();
	virtual ~CLed();
	bool on;
	bool red;
	bool flag;
	bool flag2;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


