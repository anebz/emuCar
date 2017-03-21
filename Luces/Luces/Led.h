#pragma once


// CLed

class CLed : public CWnd
{
	DECLARE_DYNAMIC(CLed)

public:
	CLed();
	virtual ~CLed();
	short color;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


