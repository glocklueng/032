#include "stdafx.h"
#include "SplashScreenEx.h"

#ifndef AW_HIDE
	#define AW_HIDE 0x00010000
	#define AW_BLEND 0x00080000
#endif

#ifndef CS_DROPSHADOW
	#define CS_DROPSHADOW   0x00020000
#endif

// CSplashScreenEx

IMPLEMENT_DYNAMIC(CSplashScreenEx, CWnd)
CSplashScreenEx::CSplashScreenEx()
{
	m_pWndParent=NULL;
	m_strText="";
	m_hRegion=0;
	m_nBitmapWidth=0;
	m_nBitmapHeight=0;
	m_nxPos=0;
	m_nyPos=0;
	m_dwTimeout=2000;
	m_dwStyle=0;
	m_rcText.SetRect(0,0,0,0);
	m_crTextColor=RGB(0,0,0);
	m_uTextFormat=DT_CENTER | DT_VCENTER | DT_WORDBREAK;

	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	if (hUser32!=NULL)
		m_fnAnimateWindow = (FN_ANIMATE_WINDOW)GetProcAddress(hUser32, ("AnimateWindow"));
	else
		m_fnAnimateWindow = NULL;

	SetTextDefaultFont();
}

CSplashScreenEx::~CSplashScreenEx()
{
}

BOOL CSplashScreenEx::Create(CWnd *pWndParent,LPCTSTR szText,DWORD dwTimeout,DWORD dwStyle)
{
	ASSERT(pWndParent!=NULL);
	m_pWndParent = pWndParent;
	
	if (szText!=NULL)
		m_strText = szText;
	else
		m_strText="";

	m_dwTimeout = dwTimeout;
	m_dwStyle = dwStyle;
	
	WNDCLASSEX wcx; 

	wcx.cbSize = sizeof(wcx);
	wcx.lpfnWndProc = AfxWndProc;
	wcx.style = CS_DBLCLKS|CS_SAVEBITS;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = AfxGetInstanceHandle();
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL,IDC_ARROW);
	wcx.hbrBackground=::GetSysColorBrush(COLOR_WINDOW);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = _T("SplashScreenExClass");
	wcx.hIconSm = NULL;

	if (m_dwStyle & CSS_SHADOW)
		wcx.style|=CS_DROPSHADOW;

	ATOM classAtom = RegisterClassEx(&wcx);
      
	// didn't work? try not using dropshadow (may not be supported)

	if (classAtom==NULL)
	{
		if (m_dwStyle & CSS_SHADOW)
		{
			wcx.style &= ~CS_DROPSHADOW;
			classAtom = RegisterClassEx(&wcx);
		}
		else
			return FALSE;
	}

	if (!CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,_T("SplashScreenExClass"),NULL,WS_POPUP,0,0,0,0,pWndParent->m_hWnd,NULL))
		return FALSE;

	return TRUE;
}

BOOL CSplashScreenEx::SetBitmap(UINT nBitmapID,short red,short green,short blue)
{
	BITMAP bm;

	m_bitmap.DeleteObject();
	if (!m_bitmap.LoadBitmap(nBitmapID))
		return FALSE;
	
	GetObject(m_bitmap.GetSafeHandle(), sizeof(bm), &bm);
	m_nBitmapWidth=bm.bmWidth;
	m_nBitmapHeight=bm.bmHeight;
	m_rcText.SetRect(0,0,bm.bmWidth,bm.bmHeight);
	
	if (m_dwStyle & CSS_CENTERSCREEN)
	{
		m_nxPos=(GetSystemMetrics(SM_CXFULLSCREEN)-bm.bmWidth)/2;
		m_nyPos=(GetSystemMetrics(SM_CYFULLSCREEN)-bm.bmHeight)/2;
	}
	else if (m_dwStyle & CSS_CENTERAPP)
	{
		CRect rcParentWindow;
		ASSERT(m_pWndParent!=NULL);
		m_pWndParent->GetWindowRect(&rcParentWindow);
		m_nxPos=rcParentWindow.left+(rcParentWindow.right-rcParentWindow.left-bm.bmWidth)/2;
		m_nyPos=rcParentWindow.top+(rcParentWindow.bottom-rcParentWindow.top-bm.bmHeight)/2;
	}

	if (red!=-1 && green!=-1 && blue!=-1)
	{
		m_hRegion=CreateRgnFromBitmap((HBITMAP)m_bitmap.GetSafeHandle(),RGB(red,green,blue));
		SetWindowRgn(m_hRegion, TRUE);
	}

	return TRUE;
}

BOOL CSplashScreenEx::SetBitmap(LPCTSTR szFileName,short red,short green,short blue)
{
	BITMAP bm;
	HBITMAP hBmp;

	hBmp=(HBITMAP)::LoadImage(AfxGetInstanceHandle(),szFileName,IMAGE_BITMAP,0,0, LR_LOADFROMFILE);
	if (!hBmp)
		return FALSE;

	m_bitmap.DeleteObject();
	m_bitmap.Attach(hBmp);
	
	GetObject(m_bitmap.GetSafeHandle(), sizeof(bm), &bm);
	m_nBitmapWidth=bm.bmWidth;
	m_nBitmapHeight=bm.bmHeight;
	m_rcText.SetRect(0,0,bm.bmWidth,bm.bmHeight);
	
	if (m_dwStyle & CSS_CENTERSCREEN)
	{
		m_nxPos=(GetSystemMetrics(SM_CXFULLSCREEN)-bm.bmWidth)/2;
		m_nyPos=(GetSystemMetrics(SM_CYFULLSCREEN)-bm.bmHeight)/2;
	}
	else if (m_dwStyle & CSS_CENTERAPP)
	{
		CRect rcParentWindow;
		ASSERT(m_pWndParent!=NULL);
		m_pWndParent->GetWindowRect(&rcParentWindow);
		m_nxPos=rcParentWindow.left+(rcParentWindow.right-rcParentWindow.left-bm.bmWidth)/2;
		m_nyPos=rcParentWindow.top+(rcParentWindow.bottom-rcParentWindow.top-bm.bmHeight)/2;
	}

	if (red!=-1 && green!=-1 && blue!=-1)
	{
		m_hRegion=CreateRgnFromBitmap((HBITMAP)m_bitmap.GetSafeHandle(),RGB(red,green,blue));
		SetWindowRgn(m_hRegion,TRUE);
	}

	return TRUE;
}

void CSplashScreenEx::SetTextFont(LPCTSTR szFont,int nSize,int nStyle)
{
	LOGFONT lf;
	m_myFont.DeleteObject();
	m_myFont.CreatePointFont(nSize,szFont);
	m_myFont.GetLogFont(&lf);
	
	if (nStyle & CSS_TEXT_BOLD)
		lf.lfWeight = FW_BOLD;
	else
		lf.lfWeight = FW_NORMAL;
	
	if (nStyle & CSS_TEXT_ITALIC)
		lf.lfItalic=TRUE;
	else
		lf.lfItalic=FALSE;
	
	if (nStyle & CSS_TEXT_UNDERLINE)
		lf.lfUnderline=TRUE;
	else
		lf.lfUnderline=FALSE;

	m_myFont.DeleteObject();
	m_myFont.CreateFontIndirect(&lf);
}

void CSplashScreenEx::SetTextDefaultFont()
{
	LOGFONT lf;
	CFont *myFont=CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	myFont->GetLogFont(&lf);
	m_myFont.DeleteObject();
	m_myFont.CreateFontIndirect(&lf);
}

void CSplashScreenEx::SetText(LPCTSTR szText)
{
	m_strText=szText;
	RedrawWindow();
}

void CSplashScreenEx::SetTextColor(COLORREF crTextColor)
{
	m_crTextColor=crTextColor;
	RedrawWindow();
}

void CSplashScreenEx::SetTextRect(CRect& rcText)
{
	m_rcText=rcText;
	RedrawWindow();
}

void CSplashScreenEx::SetTextFormat(UINT uTextFormat)
{
	m_uTextFormat=uTextFormat;
}

void CSplashScreenEx::Show()
{
	SetWindowPos(NULL,m_nxPos,m_nyPos,m_nBitmapWidth,m_nBitmapHeight,SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);

	if ((m_dwStyle & CSS_FADEIN) && (m_fnAnimateWindow!=NULL))
	{
		m_fnAnimateWindow(m_hWnd,500,AW_BLEND);
	}
	else
		ShowWindow(SW_SHOW);
	
	if (m_dwTimeout!=0)
		SetTimer(0,m_dwTimeout,NULL);
}

void CSplashScreenEx::Hide()
{
	if ((m_dwStyle & CSS_FADEOUT) && (m_fnAnimateWindow!=NULL))
		m_fnAnimateWindow(m_hWnd,200,AW_HIDE | AW_BLEND);
	else
		ShowWindow(SW_HIDE);

	DestroyWindow();
}
HRGN CSplashScreenEx::CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color)
{
	if (!hBmp) return NULL;

	BITMAP bm;
	GetObject( hBmp, sizeof(BITMAP), &bm ); // get bitmap attributes

	CDC dcBmp;
	dcBmp.CreateCompatibleDC(GetDC()); //Creates a memory device context for the bitmap
	dcBmp.SelectObject(hBmp); //selects the bitmap in the device context

	CArray<CRect,CRect&> Regions;

	int nState=-1;
	int nStart=0;
	bool bIsMask;

	int x;

	// scan the image to find areas of solid regions to draw
	for ( int y = 0; y < bm.bmHeight; ++y )
	{
		nState = -1;
		nStart = 0;
		for ( x = 0; x < bm.bmWidth; ++x )
		{
			// get color
			bIsMask=(dcBmp.GetPixel(x,bm.bmHeight-y-1)==color);

			switch( nState )
			{
			case -1: // first pixel
				if( bIsMask )
					nState = 0;
				else 
					nState = 1;

				nStart = 0;
				break;

			case 1: // solid
				if( bIsMask )
				{
					// create region
					Regions.Add( CRect( nStart, bm.bmHeight - y - 1, x, bm.bmHeight - y) );
					nState = 0;
				} 
				break;

			case 0: // transaparent

				if( !bIsMask )
				{
					// start region
					nStart = x;
					nState = 1;
				}
				break;
			}
		}

		// solid last pixel?
		if( nStart != x-1 && nState==1)
		{
			// add region
			Regions.Add( CRect( nStart, bm.bmHeight - y - 1, bm.bmWidth, bm.bmHeight - y) );
		}
	}

	dcBmp.DeleteDC(); //release the bitmap


	// create region
	// Under WinNT the ExtCreateRegion returns NULL (by Fable@aramszu.net) 
	// HRGN hRgn = ExtCreateRegion( NULL, RDHDR + pRgnData->nCount * sizeof(RECT), (LPRGNDATA)pRgnData );
	// ExtCreateRegion replacement { 
	HRGN hRgn=CreateRectRgn(0, 0, 0, 0);
	ASSERT( hRgn!=NULL );

	for( int i=0;i<(int)Regions.GetSize();++i )
	{
		const CRect& rt = Regions[i];
		HRGN hr=CreateRectRgn(rt.left, rt.top, rt.right, rt.bottom);
		VERIFY(CombineRgn(hRgn, hRgn, hr, RGN_OR)!=ERROR);
		if (hr) DeleteObject(hr);
	}

	ASSERT( hRgn!=NULL );

	return hRgn;
}

void CSplashScreenEx::DrawWindow(CDC *pDC)
{
	CDC memDC;
	CBitmap *pOldBitmap;
	
	// Blit Background
	memDC.CreateCompatibleDC(pDC);
	pOldBitmap=memDC.SelectObject(&m_bitmap);
	pDC->BitBlt(0,0,m_nBitmapWidth,m_nBitmapHeight,&memDC,0,0,SRCCOPY);
	memDC.SelectObject(pOldBitmap);

	// Draw Text
	CFont *pOldFont;
	pOldFont=pDC->SelectObject(&m_myFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_crTextColor);

	pDC->DrawText(m_strText,-1,m_rcText,m_uTextFormat);

	pDC->SelectObject(pOldFont);

}
BEGIN_MESSAGE_MAP(CSplashScreenEx, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSplashScreenEx message handlers

BOOL CSplashScreenEx::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSplashScreenEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawWindow(&dc);
}

LRESULT CSplashScreenEx::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	DrawWindow(pDC);
	return 1;
}

BOOL CSplashScreenEx::PreTranslateMessage(MSG* pMsg)
{
	// If a key is pressed, Hide the Splash Screen and destroy it
	if (m_dwStyle & CSS_HIDEONCLICK)
	{
		if (pMsg->message == WM_KEYDOWN ||
			pMsg->message == WM_SYSKEYDOWN ||
			pMsg->message == WM_LBUTTONDOWN ||
			pMsg->message == WM_RBUTTONDOWN ||
			pMsg->message == WM_MBUTTONDOWN ||
			pMsg->message == WM_NCLBUTTONDOWN ||
			pMsg->message == WM_NCRBUTTONDOWN ||
			pMsg->message == WM_NCMBUTTONDOWN)
		{
			Hide();
			return TRUE;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CSplashScreenEx::OnTimer(UINT nIDEvent)
{
	KillTimer(0);
	Hide();
}

void CSplashScreenEx::PostNcDestroy()
{
	CWnd::PostNcDestroy();
	delete this;
}