// Gauge.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Centralita.h"
#include "Gauge.h"
#include <math.h>

// CGauge

IMPLEMENT_DYNAMIC(CGauge, CWnd)

CGauge::CGauge()
{
  m_nivel = 0; // SIEMPRE ENTRE 0 y 100!!
  m_angulo = 0;
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
  m_angulo = (-m_nivel) * 180 / 100 - 90;
  CPaintDC dc(this);
  CRect r;
  GetClientRect(r);
  dc.Rectangle(r);
  dc.SetArcDirection(AD_CLOCKWISE);
  dc.Arc(r.left, r.top, r.right, r.bottom * 2, r.left, r.bottom, r.right, r.bottom);
  CPen p1(PS_SOLID,3, RGB(255, 0, 0));
  dc.SelectObject(&p1);
  dc.MoveTo(r.right / 2, r.bottom);
  int rd = r.right / 2 < r.bottom ? r.right / 2 : r.bottom;
  int x = (int)(rd * sin(m_angulo  * 3.1416 / 180));
  int y = (int)(rd * cos(m_angulo * 3.1416 / 180));
  dc.LineTo(r.right / 2 + x, r.bottom + y);
}
