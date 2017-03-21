// Led.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Centralita.h"
#include "Led.h"


// CLed

IMPLEMENT_DYNAMIC(CLed, CWnd)

CLed::CLed()
{
  m_color = 5;
}

CLed::~CLed()
{
}


BEGIN_MESSAGE_MAP(CLed, CWnd)
  ON_WM_PAINT()
END_MESSAGE_MAP()



// Controladores de mensajes de CLed




void CLed::OnPaint()
{
  CPaintDC dc(this);
  CRect r;
  GetClientRect(r);
  CBrush b1;
  switch(m_color){
  case 0:
    b1.CreateSolidBrush(RGB(0, 201, 13));
  case 1:
    b1.CreateSolidBrush(RGB(255, 7, 0));
  case 2:
    b1.CreateSolidBrush(RGB(255, 118, 0));
  default:
    b1.CreateSolidBrush(RGB(160, 160, 160));
  }
  dc.SelectObject(&b1);
  dc.Rectangle(r);
}
