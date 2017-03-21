#pragma once


// CGauge

class CGauge : public CWnd
{
	DECLARE_DYNAMIC(CGauge)

public:
	CGauge();
	virtual ~CGauge();
  int m_nivel;
protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnPaint();
};


