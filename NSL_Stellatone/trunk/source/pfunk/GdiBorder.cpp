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

#include "stdafx.h"
#include "GdiBorder.h"
#include "roundrect.h"
#include <math.h>

// Chrome border //////////////////////////////////////////////////////////////////
Color colorsChrom[] = { Color(255,140,136,135),
Color(255,255,255,255),
Color(255,255,255,255),
Color(255,120,120,120),
Color(255,247,241,243),
Color(255,148,148,148) };
REAL positionsChrom[] = {0.0f, 0.05f, 0.5f, 0.6f, 0.7f, 1.0f}; 

Color colors2Chrom[] = { Color(255,83,83,83),
Color(255,232,232,232),
Color(255,77,77,77),
Color(255,244,245,247) };
REAL positions2Chrom[] = {0.0f, 0.25f, 0.5f, 1.0f}; 
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
CGdiBorder::CGdiBorder()
{
	m_iBorderSize = 0;
	m_iRound	  = 0;
	m_BorderStyle = None;
	m_BorderShape = Rectange;
	m_pclientPath = NULL;
	m_ColorArray[GDIBORDER_COLOR_START] = 0XFFF0F0F0;
	m_ColorArray[GDIBORDER_COLOR_END]	= 0xFF101010;
}

///////////////////////////////////////////////////////////////////////////////////
CGdiBorder::~CGdiBorder(void)
{
	if ( m_pclientPath )
		delete m_pclientPath;
}

///////////////////////////////////////////////////////////////////////////////////
void CGdiBorder::Copy( CGdiBorder *pSrc )
{
	m_RectClient  = pSrc->m_RectClient;					// Client Rect	
	m_iBorderSize = pSrc->m_iBorderSize;				// Border size
	m_iRound	  = pSrc->m_iRound;						// Round border
	m_BorderStyle = pSrc->m_BorderStyle;				// Border style
	m_BorderShape = pSrc->m_BorderShape;				// Border shape

	// Copy colors ////////////////////////////////////////////////////////////////
	memcpy( m_ColorArray, pSrc->m_ColorArray, sizeof(m_ColorArray) );
}

///////////////////////////////////////////////////////////////////////////////////
void CGdiBorder::Draw( Graphics* pGraphics, CRect &WindowRect)
{
	// Define client rect /////////////////////////////////////////////////////////
	m_RectClient.X		=  WindowRect.left;
	m_RectClient.Y		=  WindowRect.top;
	m_RectClient.Width  =  WindowRect.Width();
	m_RectClient.Height =  WindowRect.Height();

	if( m_pclientPath )
	{
		delete m_pclientPath;
		m_pclientPath = NULL;
	}

	// Bush object ////////////////////////////////////////////////////////////////
	Brush *pBrush1 = NULL;
	Brush *pBrush2 = NULL;

	REAL RadioPath = 1.0f;

	// If no border then nothing to do ////////////////////////////////////////////
	switch( m_BorderStyle )
	{
		case None: return;	// Nothing to do 
		case Chrome:
				pBrush1 = new LinearGradientBrush(  m_RectClient, 0,  0, LinearGradientModeVertical);
				((LinearGradientBrush*)pBrush1)->SetInterpolationColors(colorsChrom, positionsChrom, 6);

				pBrush2 = new LinearGradientBrush(  m_RectClient, 0,  0, LinearGradientModeVertical);
				((LinearGradientBrush*)pBrush2)->SetInterpolationColors(colors2Chrom, positions2Chrom, 4);

				RadioPath = 2.0f/3.0f;
				break;
		case Lower:
				pBrush1 = new LinearGradientBrush(  m_RectClient, m_ColorArray[GDIBORDER_COLOR_END], m_ColorArray[GDIBORDER_COLOR_START], LinearGradientModeVertical);
				break;
		case Raised:
				pBrush1 = new LinearGradientBrush(  m_RectClient, m_ColorArray[GDIBORDER_COLOR_START], m_ColorArray[GDIBORDER_COLOR_END], LinearGradientModeVertical);
				break;
		case Both:
				pBrush1 = new LinearGradientBrush(  m_RectClient, m_ColorArray[GDIBORDER_COLOR_START], m_ColorArray[GDIBORDER_COLOR_END], LinearGradientModeVertical);
				pBrush2 = new LinearGradientBrush(  m_RectClient, m_ColorArray[GDIBORDER_COLOR_END], m_ColorArray[GDIBORDER_COLOR_START], LinearGradientModeVertical);
				RadioPath = 2.0f/3.0f;
				break;
	}


	GraphicsPath Path,PathBorder1,PathBorder2,PathBorder3;
	GetClientPath(&PathBorder1);

	m_RectClient.Inflate( (int) (-m_iBorderSize * RadioPath), (int)(-m_iBorderSize * RadioPath) );
	GetClientPath(&PathBorder2);

	Path.AddPath( &PathBorder1, TRUE );
	if( m_BorderStyle ==Bump )
	{
		ASSERT( pBrush1 == NULL );
		pBrush1 = new PathGradientBrush(  &Path );
		int iNbColor = 1;
		((PathGradientBrush*)pBrush1)->SetCenterPoint( Point(m_RectClient.Width/2+m_RectClient.X, m_RectClient.Height/2+m_RectClient.Y));
		((PathGradientBrush*)pBrush1)->SetCenterColor(   m_ColorArray[GDIBORDER_COLOR_START] );
		((PathGradientBrush*)pBrush1)->SetSurroundColors( &m_ColorArray[GDIBORDER_COLOR_END], &iNbColor );
		((PathGradientBrush*)pBrush1)->SetBlendTriangularShape(0.5f,1.0f);
		REAL fScaleX = (REAL) 1- (m_iBorderSize*2 / (REAL) m_RectClient.Width);
		REAL fScaleY = (REAL) 1- (m_iBorderSize*2 / (REAL) m_RectClient.Height);
		((PathGradientBrush*)pBrush1)->SetFocusScales(fScaleX,fScaleY);
	}
	else
	{
		Path.AddPath( &PathBorder2, FALSE );
	}

	if ( pBrush1 )
		pGraphics->FillPath(pBrush1, &Path);

	m_RectClient.Inflate( (int) (-m_iBorderSize * (1-RadioPath)), (int) (-m_iBorderSize* (1-RadioPath)) );
	GetClientPath(&PathBorder3);

	Path.Reset();
	Path.AddPath( &PathBorder2, FALSE );
	Path.AddPath( &PathBorder3, FALSE );
	if ( pBrush2 )
		pGraphics->FillPath(pBrush2, &Path);

	// Save client path ///////////////////////////////////////////////////////////
	m_pclientPath = PathBorder3.Clone();
	WindowRect = CRect( m_RectClient.X, (int)m_RectClient.Y, 
		 				m_RectClient.X + m_RectClient.Width, 
		 				m_RectClient.Y + m_RectClient.Height  );

	// Delete brush ///////////////////////////////////////////////////////////////
	if ( pBrush1 ) delete pBrush1;
	if ( pBrush2 ) delete pBrush2;
}

///////////////////////////////////////////////////////////////////////////////////
BOOL CGdiBorder::IsPointVisible( REAL X, REAL Y)
{
	GraphicsPath Path;
	GetClientPath( &Path );
	Region TmpRegion( &Path );
	return TmpRegion.IsVisible( X, Y );
}


///////////////////////////////////////////////////////////////////////////////////
void CGdiBorder::GetClientPath(GraphicsPath *pPath)
{
	ASSERT( pPath ); // Can't be NULL;
	pPath->Reset();  // Reset the path

	if ( !m_pclientPath )
	{
		CRoundRect lRoundRect;

		switch( m_BorderShape )
		{
			case 	Rectange:	pPath->AddRectangle(m_RectClient);		break;
			case	Circle:		
			case	Ellipse:	pPath->AddEllipse( GetClientRect() );	break;
			case	RoundRect:	lRoundRect.GetRoundRectPath(pPath, m_RectClient, m_iRound); break;
		}
	}
	else
	{
		pPath->AddPath( m_pclientPath, FALSE );
	}
}

///////////////////////////////////////////////////////////////////////////////////
Rect CGdiBorder::GetClientRect()
{
	if ( m_BorderShape == Circle )
	{
		Rect r( m_RectClient );

		if( r.Width > r.Height )
		{
			r.Inflate( -(r.Width-r.Height)/2,0);
		}
		else if( r.Width < r.Height )
		{
			r.Inflate( 0,-(r.Height-r.Width)/2);
		}
		else
		{
			// Nothing to do //////////////////////////////////////////////////////////
		}

		return r;
	}
	return m_RectClient;
}

///////////////////////////////////////////////////////////////////////////////////
void CGdiBorder::SetClientRect( Rect &Rect )
{
	m_RectClient = Rect;
}
///////////////////////////////////////////////////////////////////////////////////
void CGdiBorder::SetSize( int size )
{
	m_iBorderSize = size;
}

///////////////////////////////////////////////////////////////////////////////////
int CGdiBorder::GetSize()
{
	return m_iBorderSize;
}

///////////////////////////////////////////////////////////////////////////////////
void CGdiBorder::SetStyle( BorderStyle Style )
{
	m_BorderStyle = Style;
}

///////////////////////////////////////////////////////////////////////////////////
CGdiBorder::BorderStyle  CGdiBorder::GetStyle()
{
	return m_BorderStyle;
}

///////////////////////////////////////////////////////////////////////////////////
void CGdiBorder::SetShape( BorderShape Shape,  int round /*=0*/ )
{
	m_BorderShape = Shape;
	m_iRound	  = round;
}

///////////////////////////////////////////////////////////////////////////////////
CGdiBorder::BorderShape CGdiBorder::GetShape()
{
	return m_BorderShape;
}

///////////////////////////////////////////////////////////////////////////////////
void CGdiBorder::SetColor( Color Col, int iIndex )
{
	ASSERT(( iIndex >= 0 )  && (iIndex<GDIBORDER_MAX_COLOR) );
	m_ColorArray[iIndex] = Col;
}

///////////////////////////////////////////////////////////////////////////////////
Color CGdiBorder::GetColor( int iIndex )
{

	ASSERT(( iIndex >= 0 )  && (iIndex<GDIBORDER_MAX_COLOR) );
	return m_ColorArray[iIndex];
}