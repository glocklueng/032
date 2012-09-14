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


#ifndef _CVMBitmap_
#define _CVMBitmap_

class CVMBitmap : public CBitmap
{
public:

    BOOL Draw(HDC hDC, int x, int y);
	BOOL Draw(HDC hDC, int x, int y,int w,int h);
    BOOL DrawTransparent(HDC hDC, int x, int y);
 	BOOL DrawTransparent(HDC hDC, int x, int y, COLORREF clrTrans=RGB(255,0,255));
 
	BOOL DrawTiles(HDC hDC, int nWidth, int nHeight);

    int GetWidth();
    int GetHeight();
    BOOL GetSize(int& nWidth, int& nHeight);


    CVMBitmap();
    virtual ~CVMBitmap();

protected:

    BOOL Update();
	BOOL CreateMask(HDC hDC,COLORREF clrTrans=RGB(255,0,255));

private:

    int			m_nWidth;
    int			m_nHeight;
    HBITMAP		m_hMask;  
	COLORREF	m_clrTransparent;

};



#endif // _CVMBitmap_
