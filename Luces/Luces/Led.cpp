// Led.cpp : implementation file
//

#include "stdafx.h"
#include "Luces.h"
#include "Led.h"


// CLed

IMPLEMENT_DYNAMIC(CLed, CWnd)

CLed::CLed()
{
	color = 0;
}

CLed::~CLed()
{
}


BEGIN_MESSAGE_MAP(CLed, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CLed message handlers




void CLed::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect r;
	GetClientRect(r);
	switch(color){
		case 1: 
			dc.FillSolidRect(r,RGB(255,7,0)); // red
		case 2: 
			dc.FillSolidRect(r,RGB(255,118,0)); // orange
		case 3: 
			dc.FillSolidRect(r,RGB(160,160,160)); // gray
		default:
			dc.FillSolidRect(r,RGB(255,255,255)); // white
	}
}
