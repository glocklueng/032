/**
* @class		CVMBitmap
* --------------------------------------------------------------------------------------------------------
* @system		VMMFC
* @desc			Extended MFC CBitmap class
*				This is part of the VMMFC SampleCode
* @location		http://www.visiomedia.com/rooms/labor/src/vmmfc/index.htm
* @version		0630
* @author		Frank Luchs
* --------------------------------------------------------------------------------------------------------
* @history		1998-03-02	first draft
*				1999-02-11	DrawTiles
* --------------------------------------------------------------------------------------------------------
* @copyright	© 1985-2001 Visiomedia Software Corporation, All Rights Reserved.
* --------------------------------------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "VMBitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CVMBitmap
CVMBitmap::CVMBitmap()
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_hMask = NULL;
	m_clrTransparent = RGB(255,0,255);

}

CVMBitmap::~CVMBitmap()
{
}

/**
* @desc		This method recalcs the size of the loaded bitmap 
* @return	TRUE, if the bitmap size is valid
*/
BOOL CVMBitmap::Update()
{
    // Get the width and height.
    BITMAP bm;
    GetObject(sizeof(bm), &bm);
    m_nWidth = bm.bmWidth;
    m_nHeight = bm.bmHeight;

	BOOL bValid = m_nWidth > 0 && m_nHeight > 0;
	return(bValid);
}

int CVMBitmap::GetWidth()
{
    if (m_nWidth == 0 || m_nHeight == 0)
        Update();
    return m_nWidth;
}

int CVMBitmap::GetHeight()
{
    if (m_nWidth == 0 || m_nHeight == 0)
        Update();
    return m_nHeight;
}

/**
*
* @return	TRUE, if the bitmap size is valid
*/

BOOL CVMBitmap::GetSize(int& nWidth, int& nHeight)
{
	BOOL bValid = m_nWidth > 0 && m_nHeight > 0;

    if (bValid == FALSE)
        bValid = Update();

	nWidth = m_nWidth;
	nHeight = m_nHeight;

    return bValid;
}

/**
*	
* @desc	Draw with original size
*/

BOOL CVMBitmap::Draw(HDC hDC, int x, int y)
{
	BOOL bValid = m_nWidth > 0 && m_nHeight > 0;
	
    if (bValid == TRUE) {

		HDC hdcMem = ::CreateCompatibleDC(hDC);
 
		// Select the bitmap into the memory dc
		HBITMAP hOld = (HBITMAP)::SelectObject(hdcMem,(HBITMAP)(m_hObject));

		::BitBlt(hDC,
				 x, y,
				 m_nWidth, m_nHeight,
				 hdcMem,
				 0, 0,
				 SRCCOPY);

		::SelectObject(hdcMem, hOld);
		::DeleteDC(hdcMem); 
		}
    return bValid;

}

/**
*	
* @desc	Draw scaled
*/
BOOL CVMBitmap::Draw(HDC hDC, int x, int y,int w,int h)
{
	BOOL bValid = m_nWidth > 0 && m_nHeight > 0;
	
    if (bValid == TRUE) {
		HDC hdcMem = ::CreateCompatibleDC(hDC);

		// Select the bitmap into the mem DC.
		HBITMAP hOld =  (HBITMAP)::SelectObject(hdcMem,(HBITMAP)(m_hObject));

		::StretchBlt(hDC,
				 x, y,
				 w, h,
				 hdcMem,
				 0, 0,m_nWidth,m_nHeight,
				 SRCCOPY);

		::SelectObject(hdcMem, hOld);
		::DeleteDC(hdcMem); 
	}

    return bValid;
}




/**
* @name	DrawTransparent
* @desc	Draw bitmap transparent. This creates a mask
*		and uses the topleft pixel as transparent color
*/
BOOL CVMBitmap::DrawTransparent(HDC hDC, int x, int y)
{
	BOOL bValid = m_nWidth > 0 && m_nHeight > 0;

    if (bValid == TRUE) {

		if (!m_hMask)
			bValid = CreateMask(hDC);
		
		// draw using the default transparent color
		// or the color set by CreateMask
		DrawTransparent(hDC, x, y, m_clrTransparent);
	}

	return(bValid);
}

/**
* @name	DrawTransparent
* @desc	Draw bitmap transparent. This creates a mask
*		and uses the specified transparent color
*/
BOOL CVMBitmap::DrawTransparent(HDC hDC, int x, int y, COLORREF clrTrans)
{
	BOOL bValid = m_nWidth > 0 && m_nHeight > 0;
	
    if (bValid == TRUE) {
		
		
		if (!m_hMask)
			CreateMask(hDC,clrTrans);
		
		int dx = GetWidth();
		int dy = GetHeight();
		
		// Create a memory DC to which to draw.
		HDC hdcOffScr = ::CreateCompatibleDC(hDC);
		// Create a bitmap for the off-screen DC that is really
		// color-compatible with the destination DC.
		HBITMAP hbmOffScr = ::CreateBitmap(dx, dy, 
			(BYTE)GetDeviceCaps(hDC, PLANES),
			(BYTE)GetDeviceCaps(hDC, BITSPIXEL),
			NULL);
		// Select the buffer bitmap into the off-screen DC.
		HBITMAP hbmOldOffScr = (HBITMAP)::SelectObject(hdcOffScr, hbmOffScr);
		
		// Copy the image of the destination rectangle to the
		// off-screen buffer DC, so we can play with it.
		::BitBlt(hdcOffScr, 0, 0, dx, dy, hDC, x, y, SRCCOPY);
		
		// Create a memory DC for the source image.
		HDC hdcImage = ::CreateCompatibleDC(hDC); 
		HBITMAP hbmOldImage = (HBITMAP)::SelectObject(hdcImage, m_hObject);
		
		// Create a memory DC for the mask.
		HDC hdcMask = ::CreateCompatibleDC(hDC);
		HBITMAP hbmOldMask = (HBITMAP)::SelectObject(hdcMask, m_hMask);
		
		DWORD DSx = SRCINVERT,DSa = SRCAND;
		
		// XOR the image with the destination.
		::SetBkColor(hdcOffScr,RGB(255,255,255));
		::BitBlt(hdcOffScr, 0, 0, dx, dy ,hdcImage, 0, 0, DSx);
		// AND the destination with the mask.
		::BitBlt(hdcOffScr, 0, 0, dx, dy, hdcMask, 0,0, DSa);
		// XOR the destination with the image again.
		::BitBlt(hdcOffScr, 0, 0, dx, dy, hdcImage, 0, 0, DSx);
		
		// Copy the resultant image back to the screen DC.
		::BitBlt(hDC, x, y, dx, dy, hdcOffScr, 0, 0, SRCCOPY);
		// Tidy up.
		::SelectObject(hdcOffScr, hbmOldOffScr);
		::SelectObject(hdcImage, hbmOldImage);
		::SelectObject(hdcMask, hbmOldMask);
		::DeleteObject(hbmOffScr);
		::DeleteDC(hdcOffScr);
		::DeleteDC(hdcImage);
		::DeleteDC(hdcMask);
	}
	return(bValid);
}

/**
* @name	DrawTiles
* @desc	This method draws the bitmap tiled inside the 
*		specified rectangle, starting at pos 0,0
*/

BOOL CVMBitmap::DrawTiles(HDC hDC, int nWidth, int nHeight)
{
	BOOL bValid = m_nWidth > 0 && m_nHeight > 0;
	
	if(bValid == FALSE)
		bValid = Update();

    if (bValid == TRUE) {

		for(int y=0;y < nHeight; y+=m_nHeight) {
			for(int x=0;x < nWidth; x+=m_nWidth) {

				Draw(hDC, x, y);

			}
		}
	}
	return(bValid);
}


// private

BOOL CVMBitmap::CreateMask(HDC hDC, COLORREF clrTrans)
{
	BOOL bValid = m_nWidth > 0 && m_nHeight > 0;

    if (bValid == TRUE) {
		
		if (m_hMask) 
			::DeleteObject(m_hMask);
		
		HDC hdcMask = ::CreateCompatibleDC(hDC);
		HDC hdcImage = ::CreateCompatibleDC(hDC);
		
		// Create a monochrome bitmap for the mask.
		m_hMask = ::CreateBitmap(GetWidth(),
			GetHeight(),
			1,
			1,
			NULL);
		
		// Select the mask into its dc.
		HBITMAP hOldMask = (HBITMAP)::SelectObject(hdcMask, m_hMask);
		// Select the image into its dc.
		HBITMAP hOldImage = (HBITMAP)::SelectObject(hdcImage, m_hObject);
		
		// Take the top-left pixel as transparency color
		if(clrTrans != m_clrTransparent)
			m_clrTransparent = ::GetPixel(hdcImage, 0, 0);

		::SetBkColor(hdcImage, m_clrTransparent);
		
		// Create the mask.
		::BitBlt(hdcMask,
			0, 0,
			m_nWidth, m_nHeight,
			hdcImage,
			0, 0,
			SRCCOPY);
		
		// clean up
		::SelectObject(hdcMask, hOldMask);
		::SelectObject(hdcImage, hOldImage);
		::DeleteDC(hdcMask);
		::DeleteDC(hdcImage);
	}

	return(bValid);
}

