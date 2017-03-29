#pragma once


// CGauge

class CGauge : public CWnd
{
	DECLARE_DYNAMIC(CGauge)

public:
	CGauge();
	virtual ~CGauge();
  int m_nivel;
  int m_angulo;
protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnPaint();
};


