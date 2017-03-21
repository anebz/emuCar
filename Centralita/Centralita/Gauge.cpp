// Gauge.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Centralita.h"
#include "Gauge.h"


// CGauge

IMPLEMENT_DYNAMIC(CGauge, CWnd)

CGauge::CGauge()
{
  m_nivel = 0;
}

CGauge::~CGauge()
{
}


BEGIN_MESSAGE_MAP(CGauge, CWnd)
  ON_WM_PAINT()
END_MESSAGE_MAP()



// Controladores de mensajes de CGauge




void CGauge::OnPaint()
{
  CPaintDC dc(this);
  CRect r;
  GetClientRect(r);
  dc.Rectangle(r);
  dc.SetArcDirection(AD_CLOCKWISE);
  dc.Arc(r.left, r.top, r.right, r.bottom * 2, r.left, r.bottom, r.right, r.bottom);
}
