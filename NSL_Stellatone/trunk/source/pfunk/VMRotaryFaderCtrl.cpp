/**
* @class		CVMRotaryFaderCtrl
* --------------------------------------------------------------------------------------------------------
* @system		VMMFC
* @desc			Extended MFC CSliderCtrl class
*				This is part of the VMMFC SampleCode
* @location		http://www.visiomedia.com/rooms/labor/src/vmmfc/index.htm
* @version		0170
* @author		Frank Luchs
* --------------------------------------------------------------------------------------------------------
* @history		1999-05-01	first draft
*				2000-01-21	extended keyboard control
*				2002-08-01	extended to 3 bitmaps (scale,knob,dot)
* --------------------------------------------------------------------------------------------------------
* @copyright	© 1985-2001 Visiomedia Software Corporation, All Rights Reserved.
* --------------------------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include <math.h>

#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define M_PI 3.14159265358979323846


/////////////////////////////////////////////////////////////////////////////
// CVMRotaryFaderCtrl


CVMRotaryFaderCtrl::CVMRotaryFaderCtrl()
{
	m_nKnobRadius = 0;
	m_nZero = 0;
	m_bDragging = false;
	
	m_ptKnob.x = 0;
	m_ptKnob.y = 0;
	m_ptCenter.x = 0;
	m_ptCenter.y = 0;
	
}

CVMRotaryFaderCtrl::~CVMRotaryFaderCtrl()
{
}

BEGIN_MESSAGE_MAP(CVMRotaryFaderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CVMRotaryFaderCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CVMRotaryFaderCtrl::PreSubclassWindow() 
{
	CSliderCtrl::PreSubclassWindow();

	SetRange(30, 329, FALSE);
	SetLineSize(3);
	SetPageSize(30);

}

BOOL CVMRotaryFaderCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	if(!CSliderCtrl::PreCreateWindow(cs)) return FALSE;

	SetRange(30, 329, FALSE);
	SetLineSize(3);
	SetPageSize(30);

	return TRUE;
}

HBITMAP	CVMRotaryFaderCtrl::GetBitmap( int nWhich)
{
	return(m_Bitmap[nWhich]);
}


/**
* @name	SetBitmap
* @desc	This method sets the handle of a loaded bitmap
*		to the bitmap object. It then calculates some layout
*		values depending on the part associated with the bitmap 
*/
void CVMRotaryFaderCtrl::SetBitmap(HBITMAP hbmp, int nWhich)
{
	if(m_Bitmap[nWhich].GetSafeHandle())
		m_Bitmap[nWhich].Detach();

	m_Bitmap[nWhich].Attach(hbmp);


	int wScale=0;
	int hScale=0;
	int wKnob=0;
	int hKnob=0;
	int wDot=0;
	int hDot=0;

	if(nWhich == Scale) {
		BOOL bValid = m_Bitmap[nWhich].GetSize(wScale, hScale);

		if(bValid == TRUE) {
			SetWindowPos(&wndTopMost,0,0,wScale,hScale,SWP_NOZORDER|SWP_NOMOVE);
			m_ptCenter.x = wScale / 2;
			m_ptCenter.y = hScale / 2;
		}

	}
	else if(nWhich == Knob) {
		BOOL bValid = m_Bitmap[nWhich].GetSize(wKnob, hKnob);

		if(bValid == TRUE) {
			m_Bitmap[Scale].GetSize(wScale, hScale);

			m_ptKnob.x = (wScale - wKnob) / 2;
			m_ptKnob.y = (hScale - hKnob) / 2;
			m_nKnobRadius = wKnob/2;
		}
	}
	else if(nWhich == Dot) {
		BOOL bValid = m_Bitmap[nWhich].GetSize(wDot, hDot);

		if(bValid == TRUE) {
			m_Bitmap[Knob].GetSize(wKnob, hKnob);

			m_nKnobRadius = wKnob/2 - wDot/2 -3;
				UpdateKnobPos(0);

		}
	}

	RedrawWindow();
}


BOOL CVMRotaryFaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE; // don't draw background
}


void CVMRotaryFaderCtrl::Paint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect WindowRect;
 	GetClientRect( WindowRect );

	MemDC memDC(&dc, WindowRect, TRUE );

	m_Bitmap[Scale].Draw(memDC,0,0);
	m_Bitmap[Knob].DrawTransparent(memDC,m_ptKnob.x,m_ptKnob.y, RGB(255,0,255));
	m_Bitmap[Dot].DrawTransparent(memDC,m_ptDot.x,m_ptDot.y, RGB(255,0,255));

}



void CVMRotaryFaderCtrl::OnPaint() 
{
	Paint();
	return;
}


/**
*
* @desc		Set the absolute position in degrees
*/
bool CVMRotaryFaderCtrl::SetKnobPos(int nPos)
{
	int nOldPos = GetPos();

	int nMin = GetRangeMin();
	int nMax = GetRangeMax()+1;
	
	if(nPos < nMin)
		nPos =nMin;
	else if(nPos > nMax)
		nPos = nMax;

	SetPos(nPos);

	double dbDegree = 360 - (double)(nPos);
	double radPos = dbDegree*M_PI/180.0; // degrees to radians

	int nwDot=2;
	int nhDot=2;
	m_Bitmap[Dot].GetSize(nwDot, nhDot);
	
	// radians
	m_ptDot.x = m_ptCenter.x - (nwDot/2) + m_nKnobRadius * sin(radPos);
	m_ptDot.y = m_ptCenter.y - (nhDot/2) + m_nKnobRadius * cos(radPos);

	CString str;
	str.Format(" Pos=%d (%f=%f)\r\n", nPos, (float)radPos, (float)dbDegree);
	::OutputDebugString(str);

	return(nOldPos != nPos);



}

/**
*
* @desc		MOve by the specified delta position in degrees
*/
bool CVMRotaryFaderCtrl::UpdateKnobPos(int nDelta)
{
	int nPos = GetPos();
	nPos+=nDelta;
	
	bool b = SetKnobPos(nPos);

	return(b);

}


// mouse
void CVMRotaryFaderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_ptHit = point;

	if(!m_bDragging)
	{
		m_bDragging = true;
		SetCapture();
		SetFocus();
		RedrawWindow();
	}
	else
	{
		CSliderCtrl::OnLButtonDown(nFlags, point);
	}
}


void CVMRotaryFaderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bDragging)
	{
		if(UpdateKnobPos(m_ptHit.y-point.y))
		{
			RedrawWindow();
			m_ptHit = point;
		}
	}
	else
	{
		CSliderCtrl::OnMouseMove(nFlags, point);
	}
}

void CVMRotaryFaderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bDragging)
	{
		m_bDragging = false;
		::ReleaseCapture();
		UpdateKnobPos(m_ptHit.y-point.y);
		RedrawWindow();
	}
	else
	{
		CSliderCtrl::OnLButtonUp(nFlags, point);
	}
}

// keyboard

void CVMRotaryFaderCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int nMin = GetRangeMin();
	int nMax = GetRangeMax()+1;

	switch(nChar)
	{
	case VK_LEFT:
		{
			SetKnobPos(90);
			RedrawWindow();
		}
		break;
	case VK_UP:
		{
			UpdateKnobPos(+1);
			RedrawWindow();
		}
		break;
	
	case VK_RIGHT:
		{
			SetKnobPos(270);
			RedrawWindow();
		}
		break;
	case VK_DOWN:
		{
			UpdateKnobPos(-1);
			RedrawWindow();
		}
		break;
	case VK_SPACE:
		{
			SetKnobPos(180);
			RedrawWindow();
		}
		break;

	case VK_HOME:
		{
			SetKnobPos(nMin);
			RedrawWindow();
		}
			break;
	case VK_END:
		{
			SetKnobPos(nMax);
			RedrawWindow();
		}
		break;

	default:
		CSliderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}


void CVMRotaryFaderCtrl::SetKnobRadius(int nKnobRadius)
{
	m_nKnobRadius = nKnobRadius;
	UpdateKnobPos(0);
	RedrawWindow();

}


int CVMRotaryFaderCtrl::GetKnobRadius() const
{
	return m_nKnobRadius;
}

/**
* @todo		Zero is not yet implemented
*
*/
void CVMRotaryFaderCtrl::SetZero(int nZero)
{
	m_nZero = nZero;
}

int CVMRotaryFaderCtrl::GetZero() const
{
	return m_nZero;
}




