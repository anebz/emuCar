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
	switch(on){
		case true: 
			if(red) dc.FillSolidRect(r,RGB(255,7,0)); // red
			else dc.FillSolidRect(r,RGB(255,118,0)); // orange
			return;
		case false: 
			dc.FillSolidRect(r,RGB(160,160,160)); // gray
	}
}
