//
// GdiPlusWnd.h  : Version 1.0 
//
// Author:  Frédéric PAILLOUX
//          
//
// Description: GDI+ Border
//
//
//
// History
//     Version 1.0 - 2011 February 19
//     - Initial public release
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

#include "GdiPlusWnd.h"

#define GDIBORDER_MAX_COLOR		2
#define GDIBORDER_COLOR_START	0
#define GDIBORDER_COLOR_END		1

///////////////////////////////////////////////////////////////////////////////////
class CGdiBorder 
{
public:
	enum BorderStyle
	{
		Chrome = 0,
		Lower,
		Raised,
		Both,
		Bump,
		None,
	};

	enum BorderShape
	{
		Rectange = 0,
		Circle,
		Ellipse,
		RoundRect,
	};

protected:
	Rect		m_RectClient;						// Client Rect	
	int			m_iBorderSize;						// Border size
	int			m_iRound;							// Round border
	BorderStyle m_BorderStyle;						// Border style
	BorderShape m_BorderShape;						// Border shape
	GraphicsPath *m_pclientPath;					// Client path

	Color		m_ColorArray[GDIBORDER_MAX_COLOR];	// Border colors

public:
			 CGdiBorder();
	virtual ~CGdiBorder(void);
		void Copy( CGdiBorder *pSrc );			// Copy object
virtual void Draw( Graphics* pGraphics,			// Draw the border
				   CRect &WindowRect);
	BOOL	 IsPointVisible( REAL X, REAL Y);	// Return TRUE if point is inside the client area
	void     GetClientPath(GraphicsPath *pPath);// Return client path (you must call the Draw methode before )
	Rect	 GetClientRect();					// Return client rect
	void 	 SetClientRect( Rect &Rect );		// Set client rect
	void	 SetSize( int size );				// Set border size
	int		 GetSize();							// return border size
	void	 SetStyle( BorderStyle Style );		// Set border size
BorderStyle  GetStyle();						// Return border size
	void	 SetShape( BorderShape Shape,		// Set shape style
					   int round = 0 );	
	int	 GetShapeRound() { return m_iRound; }	// Return shape round (use only with RoundRect style)
BorderShape  GetShape();						// Return shape

	void	 SetColor( Color Col,				// Set border color
						int iIndex );
	Color	 GetColor( int iIndex );			// Return border color
};