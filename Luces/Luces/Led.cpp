// Led.cpp : implementation file
//

#include "stdafx.h"
#include "Luces.h"
#include "Led.h"


// CLed

IMPLEMENT_DYNAMIC(CLed, CWnd)

CLed::CLed()
{
	on = false;
	red = false;
	flag = false;
	flag2 = false;
	activado = false;
}

CLed::~CLed()
{
}


BEGIN_MESSAGE_MAP(CLed, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CLed message handlers




void CLed::OnPaint()
{
	CPaintDC dc(this);
	CRect r;
	GetClientRect(r);

	if(flag2){
		if(on && !red){
			if(flag) dc.FillSolidRect(r,RGB(160,160,160)); // gray
			else dc.FillSolidRect(r,RGB(255,118,0)); // orange
			flag = !flag;
		}
		flag2 = false;
		return;
	}
	if(on){
		if(red) dc.FillSolidRect(r,RGB(255,7,0)); // red
		else{
			dc.FillSolidRect(r,RGB(255,118,0)); // orange
			SetTimer(2, 500, NULL);
			flag = true;
		}
		return;
	}else{
		dc.FillSolidRect(r,RGB(160,160,160)); // gray
		flag = false;
		KillTimer(2);
	}
}


void CLed::OnTimer(UINT_PTR nIDEvent)
{
	flag2 = true;
	Invalidate(true);
}
