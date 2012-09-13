//
// GdiPlusWnd.h  : Version 1.2
//
// Author:  Frédéric PAILLOUX
//          
//
// Description: GDI+ custom draw control template
//
//
//
// History
//     Version 1.0 - 2010 December 20
//     - Initial public release
//	   Version 1.1 - 2011 April 10
//		- Visual C++ 6 Compatible
//		- Static DLL Compatible
//		- SetWindowText bug fixed
//	   Version 1.2 - 2011 April 21
//		- dc.BitBlt removed in the OnPaint method
//
// License:
//     This software is released under the Code Project Open License (CPOL),
//     which may be found here:  http://www.codeproject.com/info/eula.aspx
//     You are free to use this software in any way you like, except that you 
//     may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "gdiplus.h"
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
#include "MemDC.h"

#ifndef _UNICODE
#define STRCPY strcpy_s
#else
#define STRCPY wcscpy_s 
#endif //#ifndef _UNICODE

///////////////////////////////////////////////////////////////////////////////////
// GDILoader, Load an unload automatically GDI+ library
// Usage: Add the marco DECLARE_GDI_PLUS in the header of your CWinApp derived class 
///////////////////////////////////////////////////////////////////////////////////
class GDILoader
{
private:
	ULONG_PTR m_gdiplusToken;
public:
	GDILoader() { Gdiplus::GdiplusStartupInput gdiplusStartupInput;
				  Status TmpStatus = Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
				  if( TmpStatus != Gdiplus::Ok)
				  {
					    m_gdiplusToken = NULL;
						CString TmpMsg;
						TmpMsg.Format(_T("GdiplusStartup failed code %d"),TmpStatus );
						AfxMessageBox( TmpMsg );
				  }
				};
	~GDILoader() { if(m_gdiplusToken)
					Gdiplus::GdiplusShutdown(m_gdiplusToken);
				 };
};

#define DECLARE_GDI_PLUS GDILoader m_GDILoader; 



///////////////////////////////////////////////////////////////////////////////////
template<class baseClass>
class CGdiPlusWnd : public baseClass
{
protected:
	BOOL	m_bReGenBackGnd;			// If TRUE the RebuildGDIbkGnd method will be call on next print
	CDC		m_dcBackGnd;				// BackGround DC	
	CRect	m_RectClient;
	CString m_CStringWindowText;		// Current window text
	BOOL	m_UpdateBkGndOnTxtChanged;	// TRUE if background must be updated when text changed
public:
		 CGdiPlusWnd() { ASSERT( !m_dcBackGnd.m_hDC ); m_bReGenBackGnd = TRUE; m_UpdateBkGndOnTxtChanged = FALSE; }
virtual ~CGdiPlusWnd() { if( !m_dcBackGnd.m_hDC ) m_dcBackGnd.DeleteDC(); }

protected:
///////////////////////////////////////////////////////////////////////////////////
// Draw the back ground or your control in the pGraphics object.
// You can draw a complex image (and take lot of time) because this method is call 
// only when the control is moved, resized, or displayed for the first time.
// Usually this method is called only one time.
// CRect &WindowRect provides to you the size of you control
// You should update the WindowRect rect if you draw a border. (otherwise you risk
// to erase it in the OnPaint() method. You could also manage this case yourself.
///////////////////////////////////////////////////////////////////////////////////
	virtual BOOL	RebuildGDIbkGnd(Graphics* pGraphics,
									CRect &WindowRect) {m_bReGenBackGnd = FALSE; 
														return TRUE; }

////////////////////////////////////////////////////////////////////////////////////
// This function is called when the WM_PAINT message occurred, the background is already painted.
// Just draw the mobiles part of you control.
// At the end you should add the glossiness if you control has this option.
////////////////////////////////////////////////////////////////////////////////////
	virtual	void	OnPaint(Graphics* pGraphics, CRect &WindowRect ) = 0;	
							
///////////////////////////////////////////////////////////////////////////////////
// If you need to update the background of you control, you should use this method.
///////////////////////////////////////////////////////////////////////////////////
void InvalidateBkGnd()
{
	m_bReGenBackGnd = TRUE;
	if ( m_hWnd )
		Invalidate();
}

///////////////////////////////////////////////////////////////////////////////////
void PreSubclassWindow()
{
	
	m_bReGenBackGnd = TRUE;
	baseClass::PreSubclassWindow();
}

///////////////////////////////////////////////////////////////////////////////////
afx_msg	int OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (baseClass::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bReGenBackGnd = TRUE;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
afx_msg void OnPaint()
{
	CPaintDC dc(this); // device context for painting

 	CRect WindowRect;
 	GetClientRect( WindowRect );

	// Regenerate background if needed ////////////////////////////////////////////
	if ( m_bReGenBackGnd )
	{
		MemDC pDC(&dc, WindowRect, TRUE);

		Graphics gdi(pDC->m_hDC);

		// default smoothing mode
		if( gdi.SetSmoothingMode(SmoothingModeAntiAlias) != Gdiplus::Ok )
		{
			ASSERT( 0 );
		}

		gdi.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

		// Build the background ///////////////////////////////////////////////////
		m_RectClient = WindowRect;
		RebuildGDIbkGnd( &gdi, m_RectClient );

		// Delete previous DC memory //////////////////////////////////////////////
		m_dcBackGnd.DeleteDC();

		// Save new background ////////////////////////////////////////////////////
		CBitmap bmp;
		m_dcBackGnd.CreateCompatibleDC(pDC);
		bmp.CreateCompatibleBitmap(pDC, WindowRect.Width(), WindowRect.Height());
		m_dcBackGnd.SelectObject(&bmp);
		m_dcBackGnd.BitBlt(0, 0, WindowRect.Width(), WindowRect.Height(), pDC, 0, 0, SRCCOPY);
		bmp.DeleteObject();

		// Back ground is updated /////////////////////////////////////////////////
		m_bReGenBackGnd = FALSE;
	}

	// Create temporary DC to draw the control ////////////////////////////////////
	MemDC pDC(&dc, WindowRect, TRUE);
	Gdiplus::Graphics gdi(pDC->m_hDC);

	// default smoothing mode
	if( gdi.SetSmoothingMode(SmoothingModeAntiAlias) != Gdiplus::Ok )
	{
		ASSERT( 0 );
	}

	// Draw the background ////////////////////////////////////////////////////////
	ASSERT( m_dcBackGnd.m_hDC );
	pDC->BitBlt(0,0,WindowRect.Width(),WindowRect.Height(), &m_dcBackGnd, 0, 0, SRCCOPY );

	// Adjust the client rect according the background (usually when border used)//
	if( m_RectClient.left || m_RectClient.top )
	{
		Matrix TmpMatrix;
		TmpMatrix.Translate( (REAL) m_RectClient.left , (REAL)  m_RectClient.top );
		gdi.SetTransform( &TmpMatrix );
	}

	// Custom draw variables parts of the control (like pointers, bar, ... ////////
	OnPaint( &gdi, m_RectClient );

	///////////////////////////////////////////////////////////////////////////////
	// Draw all the control (and avoid flicking)///////////////////////////////////
	// Not needed, the job will be done in the destructor of the MemDC object /////
	///////////////////////////////////////////////////////////////////////////////
	//dc.BitBlt(0,0, WindowRect.Width(), WindowRect.Height(), &pDC, 0, 0, SRCCOPY );
}

///////////////////////////////////////////////////////////////////////////////////
afx_msg	void OnSize(UINT nType, int cx, int cy)
{
	m_bReGenBackGnd = TRUE; // Background must be resized
	baseClass::OnSize(nType, cx, cy);
}

///////////////////////////////////////////////////////////////////////////////////
afx_msg void OnMove(int x, int y)
{
	m_bReGenBackGnd = TRUE; // Background must be resized
	baseClass::OnMove(x, y);
}

///////////////////////////////////////////////////////////////////////////////////
afx_msg void OnDestroy()
{
	baseClass::OnDestroy();
	m_dcBackGnd.DeleteDC();
}

///////////////////////////////////////////////////////////////////////////////////
afx_msg BOOL OnEraseBkgnd(CDC* pDC)
{
	// Don't care I will do the job in the OnPaint method /////////////////////////
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
LRESULT OnGetText(WPARAM wParam,LPARAM lParam)
{
	int iStrlen = m_CStringWindowText.GetLength();


	if( (UINT) iStrlen < (UINT) wParam )
	{
		STRCPY( (PTCHAR) lParam, (UINT) wParam, (LPCTSTR) m_CStringWindowText );
		return iStrlen;
	}
	else
	{
		iStrlen = ((UINT) wParam) -1;
		memcpy( (PTCHAR) lParam, (LPCTSTR) m_CStringWindowText, iStrlen );
		*((PTCHAR) lParam + iStrlen) = 0;
	}

	return iStrlen;
}

///////////////////////////////////////////////////////////////////////////////////
LRESULT OnSetText(WPARAM wParam,LPARAM lParam)
{
	BOOL bChange = m_CStringWindowText.Compare( (LPCTSTR) lParam );

	if ( bChange )
	{
		m_CStringWindowText = CString( (LPCTSTR) lParam );
		if( m_UpdateBkGndOnTxtChanged )
			InvalidateBkGnd();
		else
			Invalidate();
		return 0L;
	}
	return 0L;
	//return  Default(); // Do not draw the text
}

///////////////////////////////////////////////////////////////////////////////////
LRESULT OnGetTextLength(WPARAM wParam,LPARAM lParam)
{
	return  m_CStringWindowText.GetLength();
}

///////////////////////////////////////////////////////////////////////////////////
// Message map members functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
static const AFX_MSGMAP_ENTRY* GetMessageEntries()
{ 
	typedef CGdiPlusWnd<baseClass> ThisClass;	
	static const AFX_MSGMAP_ENTRY Entries[] =
	{
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_SETTEXT,&ThisClass::OnSetText)
	ON_MESSAGE(WM_GETTEXT,&ThisClass::OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH,&ThisClass::OnGetTextLength)
	{0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
	}; 
	return Entries;
}

virtual const AFX_MSGMAP* GetMessageMap() const 
{
  	return &CGdiPlusWnd<baseClass>::messageMap; 
}
 
protected: 
	  	static const AFX_MSGMAP messageMap;

#if _MFC_VER < 0x0700
	static const AFX_MSGMAP* PASCAL _GetBaseMessageMap()
	{ 
		return &baseClass::messageMap; 
	}; 
#else
	static const AFX_MSGMAP* PASCAL GetThisMessageMap()
	{
		return &CGdiPlusWnd<baseClass>::messageMap; 
	} 
#endif

};

#ifdef _AFXDLL
#if _MFC_VER < 0x0700 
template<class baseClass>
const AFX_MSGMAP CGdiPlusWnd<baseClass>::messageMap = { &CGdiPlusWnd<baseClass>::_GetBaseMessageMap, GetMessageEntries()};
#else
template<class baseClass>
const AFX_MSGMAP CGdiPlusWnd<baseClass>::messageMap = { &baseClass::GetThisMessageMap, GetMessageEntries()};
#endif
#else
	template<class baseClass>
	const AFX_MSGMAP CGdiPlusWnd<baseClass>::messageMap = { &baseClass::GetThisMessageMap, GetMessageEntries()};
#endif