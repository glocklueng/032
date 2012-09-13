//
// IsiLcdDisplay.h  : Version 1.1
//
// Author:  Frédéric PAILLOUX
//          
//
// Description:
//
// History
//     Version 1.0 - 2010 December 20
//     - Initial public release
// 	   Version 1.1 - 2011 February 19
//	   - Draw border
//	   - Matrix able to manager 256 characters
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
#include "IsiLcdDisplay.h"
#include "color.h"
#include "RoundRect.h"
#include <complex>



#define SEGMENT7_WIDTH_FACTOR			(float) 0.15
#define SEGMENT16_WIDTH_FACTOR			(float) 0.07
#define MATRIX5x7_WIDTH_FACTOR			(float) 0.15
#define SEGMENT_SEPARATOR_FACTOR		(float) 0.95	
#define LEFT_RIGHT_GAP_FACTOR			(float) 0.2
#define UP_DOWN_GAP_FACTOR				(float) 0.1
#define LEFT_RIGHT_GAP_FACTOR_MATRIX	(float) 0.15
#define UP_DOWN_GAP_FACTOR_MATRIX		(float) 0.2


#define GLOW_SCALE_X					(float) 1.5
#define GLOW_SCALE_Y					(float) 3

#define GLOW_SCALE_X_DIAG				(float) 3
#define GLOW_SCALE_Y_DIAG				(float) 1.2


#define NB_SEGMENT_DOT				3

// 7 Segments LCD matrix //////////////////////////////////////////////////////////
#define BASE_7SEGMENT_DIGIT		0x20
#define NB_CHAR_TABLE_7SEGMENT	39
BYTE Segment7Def[NB_CHAR_TABLE_7SEGMENT*SEGMENT_7_ARRAY_SIZE] ={0,0,0,0,0,0,0,0,0,0,0,0,	//sp 0x20
																0,0,0,0,0,0,0,0,0,0,0,0,	//! 0x21
																0,0,0,0,0,0,0,0,0,0,0,0,	//" 0x22
																0,0,0,0,0,0,0,0,0,0,0,0,	//# 0x23
																0,0,0,0,0,0,0,0,0,0,0,0,	//$ 0x24
																0,0,0,0,0,0,0,0,0,0,0,0,	//% 0x25
																0,0,0,0,0,0,0,0,0,0,0,0,	//& 0x26
																0,0,0,0,0,0,0,0,0,0,0,0,	//' 0x27
																1,0,0,1,1,1,0,0,0,0,0,0,	//( 0x28
																1,1,1,1,0,0,0,0,0,0,0,0,	//) 0x29
																0,0,0,0,0,0,0,0,0,0,0,0,	//* 0x2A
																0,0,0,0,0,0,0,0,1,0,0,0,	//+ 0x2B
																0,0,0,0,0,0,0,0,0,0,0,0,	//, 0x2C
																0,0,0,0,0,0,0,1,0,0,0,0,	//- 0x2D
																0,0,0,0,0,0,0,0,0,1,0,0,	//. 0x2E
																1,1,1,1,1,1,0,0,0,0,0,0,	/// 0x2F
																1,1,1,1,1,1,0,0,0,0,0,0,	//0 0x30
																0,1,1,0,0,0,0,0,0,0,0,0,	//1 0x31
																1,1,0,1,1,0,1,0,0,0,0,0,	//2 0x32
																1,1,1,1,0,0,1,0,0,0,0,0,	//3 0x33
																0,1,1,0,0,1,1,0,0,0,0,0,	//4 0x34
																1,0,1,1,0,1,1,0,0,0,0,0,	//5 0x35
																0,0,1,1,1,1,1,0,0,0,0,0,	//6 0x36
																1,1,1,0,0,0,0,0,0,0,0,0,	//7 0x37
																1,1,1,1,1,1,1,0,0,0,0,0,	//8 0x38
																1,1,1,0,0,1,1,0,0,0,0,0,	//9 0x39
																0,0,0,0,0,0,0,0,0,0,0,0,	//: 0x3A
																0,0,0,0,0,0,0,0,0,0,0,0,	//; 0x3B
																0,0,0,0,0,0,0,0,0,0,0,0,	//< 0x3C
																0,0,0,0,0,0,0,0,0,0,0,0,	//= 0x3D
																0,0,0,0,0,0,0,0,0,0,0,0,	//> 0x3E
																0,0,0,0,0,0,0,0,0,0,0,0,	//? 0x3F
																0,0,0,0,0,0,0,0,0,0,0,0,	//@ 0x40
																1,1,1,0,1,1,1,0,0,0,0,0,	//A 0x41
																0,0,1,1,1,1,1,0,0,0,0,0,	//B 0x42
																1,0,0,1,1,1,0,0,0,0,0,0,	//C 0x43
																0,1,1,1,1,0,1,0,0,0,0,0,	//D 0x44
																1,0,0,1,1,1,1,0,0,0,0,0,	//E 0x45
																1,0,0,0,1,1,1,0,0,0,0,0};	//F 0x46

// 16 Segments LCD matrix /////////////////////////////////////////////////////////
#define NB_CHAR_TABLE_16SEGMENT 64
#define BASE_16SEGMENT_DIGIT	0x20							  //0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
BYTE Segment16Def[NB_CHAR_TABLE_16SEGMENT*SEGMENT_16_ARRAY_SIZE]  ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//sp 0x20
																	0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,	//! 0x21
																	0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,	//" 0x22
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//# 0x23
																	1,1,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,	//$ 0x24
																	1,0,0,1,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,0,0,	//% 0x25
																	0,1,0,1,1,1,0,0,1,1,0,0,0,1,0,1,0,0,0,0,0,	//& 0x26
																	0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,	//' 0x27
																	1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,	//( 0x28
																	0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//) 0x29
																	0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,	//* 0x2A
																	0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,	//+ 0x2B
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,	//, 0x2C
																	0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,	//- 0x2D
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,	//. 0x2E
																	0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,	/// 0x2F
																	1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,1,0,0,0,0,0,	//0 0x30
																	0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//1 0x31
																	1,1,1,0,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,	//2 0x32
																	1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,	//3 0x33
																	0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,	//4 0x34
																	1,1,0,0,1,1,0,1,0,0,1,0,0,0,1,0,0,0,0,0,0,	//5 0x35
																	1,1,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,	//6 0x36
																	1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//7 0x37
																	1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,	//8 0x38
																	1,1,1,1,1,1,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,  //9 0x39
																	0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,	//: 0x3A
																	0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,	//; 0x3B
																	0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,	//< 0x3C
																	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//= 0x3D
																	0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,	//> 0x3E
																	0,1,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,	//? 0x3F
																	1,1,1,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,	//@ 0x40
																	1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,	//A 0x41
																	1,1,1,1,1,1,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0,	//B 0x42
																	1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,	//C 0x43
																	1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,	//D 0x44
																	1,1,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,	//E 0x45
																	1,1,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,	//F 0x46
																	1,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,	//G 0x47
																	0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,	//H 0x48
																	0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,	//I 0x49
																	0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//J 0x4A
																	0,0,0,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,	//K 0x4B
																	0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,	//L 0x4C
																	0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,	//M 0x4D
																	0,0,1,1,0,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,	//N 0x4E
																	1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,	//O 0x4F
																	1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,	//P 0x50
																	1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,	//Q 0x51
																	1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,0,0,0,0,0,0,	//R 0x52
																	1,1,0,1,1,1,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,  //S 0x53
																	1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,	//T 0x54
																	0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,	//U 0x55
																	0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,1,0,0,0,0,0,	//V 0x56
																	0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,	//W 0x57
																	0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,	//X 0x58
																	0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,	//Y 0x59
																	1,1,0,0,1,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,	//Z 0x5A
																	0,1,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,	//[ 0x5B
																	0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,	//\ 0x5C
																	1,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,	//] 0x5D
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//^ 0x5E
																	0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};  //_ 0x5F

#define 	TO_ARGB( alpha, colorref)	Color( alpha, GetRValue(colorref), GetGValue(colorref), GetBValue(colorref))
// X * Y LCD matrix ///////////////////////////////////////////////////////////////
// This kind of matrix is loaded dynamically (from a bitmap)
// Look at the size of the 16 segments matrix I let you imagine the size of this one :)
#define BASE_MATRIX_DIGIT					0x00 //0x20
#define MATRIX_BITMAP_NB_CHAR_PER_LINE		32
#define MATRIX_BITMAP_NB_LINE				8
#define NB_CHAR_TABLE_MATRIX				MATRIX_BITMAP_NB_CHAR_PER_LINE * MATRIX_BITMAP_NB_LINE


// Set blend factors and positions for the path gradient brush.
REAL fac[] = {
	0.0f, 
	0.25f,    // 25 percent of the way from transparent to color
	0.4f,     // 40 percent of the way from transparent to blue
	1.0f};

	REAL posfac[] = {
		0.0f, 
		0.5f,   // 50 percent of the way from the boundary to the center
		0.8f,   // 80 percent of the way from the boundary to the center
		1.0f};

///////////////////////////////////////////////////////////////////////////////////
//IMPLEMENT_DYNAMIC(CIsiLcdDisplay, CIsiCompBase)

///////////////////////////////////////////////////////////////////////////////////
CIsiLcdDisplay::CIsiLcdDisplay()
{
	m_iBlink			= 0;			 
	m_BlinkStatus		= TRUE;
	m_bShowSign			= FALSE;
	m_cColorBkgnd		= RGB(0,0,0);
	m_iValueNbDigit		= 4;
	m_bEnableValue		= TRUE;
	m_bHexMode			= FALSE;
	m_bStringMode		= FALSE;
	m_Style				= Segment7;
	m_bShowOffSegments	= TRUE;
	m_fSizeSegmentModifier = 1;
	m_pMatrixData		= NULL;
	m_pDotsMatrixPosition=NULL;
	m_iScrollSpeed		= 0;
	m_iCurPosition		= 0;
	SetColor( RGB(150,0,0) );
}

///////////////////////////////////////////////////////////////////////////////////
CIsiLcdDisplay::~CIsiLcdDisplay()
{
	// Delete matrix data if needed ///////////////////////////////////////////////
	if ( m_pMatrixData )
		delete m_pMatrixData;
	m_MatrixSize.SetSize(0,0);
	if ( m_pDotsMatrixPosition )
		delete m_pDotsMatrixPosition;
}

///////////////////////////////////////////////////////////////////////////////////
int CIsiLcdDisplay::GetNbDigit()
{
	if (  ( m_Style == Segment7 ) || (  m_Style ==  Segment16) )
	{
		if ( m_bShowSign && !m_bStringMode && !m_bHexMode )
			return m_iValueNbDigit +1;
	}
	return m_iValueNbDigit;
}

///////////////////////////////////////////////////////////////////////////////////
COLORREF CIsiLcdDisplay::GetColorOff()
{
	// If off color has been specified then return it /////////////////////////////
	if ( m_cColorSegmentOff )
		return m_cColorSegmentOff;

	// Auto calculate OFF color ///////////////////////////////////////////////////
	CColor AutoColor(m_cColorSegmentOn);
	AutoColor.SetLuminance( AutoColor.GetLuminance() / 8);
	return AutoColor.ToRGB();
}

///////////////////////////////////////////////////////////////////////////////////
COLORREF CIsiLcdDisplay::GetGlowColor()
{
	// Auto calculate glow color if needed ////////////////////////////////////////
	if ( !m_cColorSegmentGlow  )
	{
		CColor AutoColor(m_cColorSegmentOn);
		float flum = AutoColor.GetLuminance() * 2;
		if( flum > 1 ) flum = 1;
		AutoColor.SetLuminance( flum );
		return  AutoColor.ToRGB();
	}
	else if( ( m_cColorSegmentGlow == -1 ) || (m_cColorSegmentGlow == 0x00FFFFFF) ) 
	{
		return NULL;
	}

	return m_cColorSegmentGlow;
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::GetSegmentPath( int iIndex, GraphicsPath **PathDigit,  GraphicsPath **PathGlow  )
{
	if (m_Style == Segment7 )
	{
		switch( iIndex )
		{
		case 0:
		case 3:
		case 6: if ( PathDigit ) *PathDigit = m_pathSegmentH.Clone();
			if ( PathGlow  ) *PathGlow  = m_pathSegmentH_Glow.Clone();
			break;
		case 1:
		case 2:
		case 4: 
		case 5: if ( PathDigit ) *PathDigit =  m_pathSegmentV.Clone(); 
			if ( PathGlow  ) *PathGlow  =  m_pathSegmentV_Glow.Clone();
			break;
		case 7: if ( PathDigit ) *PathDigit =  m_pathSegmentMinus.Clone(); 
			if ( PathGlow )  *PathGlow =  m_pathSegmentMinus_Glow.Clone(); 
			break;
		case 8: if ( PathDigit ) *PathDigit =  m_pathSegmentPlus.Clone(); 
			if ( PathGlow )  *PathGlow =  m_pathSegmentPlus_Glow.Clone();
			break;
		default: ASSERT( 0 );
		}
	}
	else if ( m_Style == Segment16 )
	{
		switch( iIndex )
		{
		case 0:
		case 1:
		case 4:
		case 5: 
		case 10: 
		case 11: if ( PathDigit ) *PathDigit = m_pathSegmentH.Clone();
			if ( PathGlow  ) *PathGlow  = m_pathSegmentH_Glow.Clone();
			break;

		case 2:
		case 3:
		case 6: 
		case 7:
		case 8:
		case 9: if ( PathDigit ) *PathDigit =  m_pathSegmentV.Clone(); 
			if ( PathGlow  ) *PathGlow  =  m_pathSegmentV_Glow.Clone();
			break;

		case 12:
		case 14: if ( PathDigit ) *PathDigit =  m_pathSegmentDiag1.Clone(); 
			if ( PathGlow  ) *PathGlow  =  m_pathSegmentDiag1_Glow.Clone();
			break;

		case 13:
		case 15:  if ( PathDigit ) *PathDigit =  m_pathSegmentDiag2.Clone(); 
			if ( PathGlow  ) *PathGlow  =  m_pathSegmentDiag2_Glow.Clone();
			break;

		case 16: if ( PathDigit ) *PathDigit =  m_pathSegmentMinus.Clone(); 
			if ( PathGlow )  *PathGlow =  m_pathSegmentMinus_Glow.Clone(); 
			break;

		case 17: if ( PathDigit ) *PathDigit =  m_pathSegmentPlus.Clone(); 
			if ( PathGlow )  *PathGlow =  m_pathSegmentPlus_Glow.Clone();
			break;

		default: ASSERT( 0 );
		}
	}
	else
	{
		if ( PathDigit ) *PathDigit =  m_pathDot.Clone(); 
		if ( PathGlow )  *PathGlow =  m_pathDotGlow.Clone();
	}
}



///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::DrawBackgroundDigit(Graphics* pGraphics, BOOL bSign, BOOL bLastDigit  )
{
	// Create off color ///////////////////////////////////////////////////////////
	SolidBrush SBrushOff( TO_ARGB( 255, GetColorOff() ) );

	int iStartIndex =  0;
	int iEndEndex = 0;
	int iDotIndex = 0;
	PointF *pos = NULL;

	switch( m_Style )
	{
	case Segment7: if( bSign ) 
				   {
					   iStartIndex = 7;
					   iEndEndex = 9;
				   }
				   else
				   {
					   iEndEndex = 7;
				   }
				   iDotIndex = 9;
				   pos = m_7SegmentsPosition;
				   break;
	case Segment16:
		if( bSign ) 
		{
			iStartIndex = 16;
			iEndEndex = 18;
		}
		else
		{
			iEndEndex = 16;
		}
		iDotIndex = 18;
		pos = m_16SegmentsPosition;
		break;
	case MatrixDot:
	case MatrixSquare:
		pos = m_pDotsMatrixPosition;
		iEndEndex = m_MatrixSize.cx * m_MatrixSize.cy;
		break;
	default: ASSERT( 0 ); // Invalid case
	}

	// Draw segments //////////////////////////////////////////////////////////
	for (int iIndex =iStartIndex; iIndex < iEndEndex; iIndex++ )
	{
		// Select path to be used /////////////////////////////////////////////////
		GraphicsPath *Path = NULL;
		GetSegmentPath( iIndex, &Path, NULL );

		if( Path )
		{
			Matrix TmpMatrix;
			TmpMatrix.Translate( pos[iIndex].X, pos[iIndex].Y );
			Path->Transform( &TmpMatrix );
			pGraphics->FillPath( &SBrushOff, Path);
			delete Path;
		}
	}

	// Draw dots only if this is not the last digit ///////////////////////////////
	if ( !bLastDigit )
	{
		ASSERT( pos );
		for ( int iNumDot = 0; iNumDot < NB_SEGMENT_DOT; iNumDot++ )
		{
			// Draw the dot ///////////////////////////////////////////////////////////
			Matrix TmpMatrix;
			TmpMatrix.Translate( pos[iDotIndex + iNumDot].X, pos[iDotIndex + iNumDot].Y );
			GraphicsPath *TmpPath = m_pathDot.Clone();
			TmpPath->Transform(&TmpMatrix);
			pGraphics->FillPath( &SBrushOff, TmpPath);
			delete TmpPath;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::DrawBackgroundDigitOn(Graphics* pGraphics, TCHAR Digit, BOOL bShowDot[])
{
	// Create On color ///////////////////////////////////////////////////////////
	SolidBrush SBrushON( TO_ARGB(255,m_cColorSegmentOn) );

	int iEndEndex;
	int iDotIndex;

	PointF *pos = NULL;
	int iNbSegPerChar=0;
	BYTE *SegmentDef;

	switch( m_Style )
	{
	case Segment7: iEndEndex = 9;
		iDotIndex = 9;
		pos = m_7SegmentsPosition;
		SegmentDef =Segment7Def;
		iNbSegPerChar = SEGMENT_7_ARRAY_SIZE;
		break;
	case Segment16: iEndEndex = 16;
		iDotIndex = 18;
		pos = m_16SegmentsPosition;
		SegmentDef = Segment16Def;
		iNbSegPerChar = SEGMENT_16_ARRAY_SIZE;
		break;
	case MatrixDot:
	case MatrixSquare:
		pos			  = m_pDotsMatrixPosition;
		iEndEndex	  = m_MatrixSize.cx * m_MatrixSize.cy;
		iNbSegPerChar = m_MatrixSize.cx * m_MatrixSize.cy;
		SegmentDef = m_pMatrixData;
		break;
	default: ASSERT( 0 );	// Invalid case
	}

	int iCharIndex = 0;
	if (m_Style == Segment7 )
	{
		iCharIndex = Digit -BASE_7SEGMENT_DIGIT;
		if ( iCharIndex >= NB_CHAR_TABLE_7SEGMENT )
			iCharIndex -= 0x20; // Try upper case;

		// Check for valid digit ////////////////////////////////////////////////
		if ( ( iCharIndex < 0 ) || ( iCharIndex >= NB_CHAR_TABLE_7SEGMENT ) )
			return; // Invalid char then can't draw it
	}
	else if (m_Style == Segment16 )
	{
		iCharIndex = Digit -BASE_16SEGMENT_DIGIT;
		if ( iCharIndex >= NB_CHAR_TABLE_16SEGMENT )
			iCharIndex -= 0x20; // Try upper case;

		// Check for valid digit ////////////////////////////////////////////////
		if ( ( iCharIndex < 0 ) || ( iCharIndex >= NB_CHAR_TABLE_16SEGMENT ) )
			return; // Invalid char then can't draw it
	}
	else
	{
		iCharIndex = Digit -BASE_MATRIX_DIGIT;
		if ( iCharIndex >= NB_CHAR_TABLE_MATRIX )
			iCharIndex -= 0x20; // Try upper case;

		// Check for valid digit //////////////////////////////////////////////////
		if ( ( iCharIndex < 0 ) || ( iCharIndex >= NB_CHAR_TABLE_MATRIX ) )
			return; // Invalid char then can't draw it
	}

	// Light on segments //////////////////////////////////////////////////////////
	for (int iIndex =0; iIndex < iEndEndex; iIndex++ )
	{
		if ( !SegmentDef )
			continue;

		if( !SegmentDef[iCharIndex * iNbSegPerChar +iIndex] )
		{
			// Segment is off then nothing to do //////////////////////////////////
			continue;
		}

		// Select path to be used /////////////////////////////////////////////////////
		GraphicsPath *PathDigit = NULL;
		GraphicsPath *PathGlow = NULL;
		GetSegmentPath( iIndex, &PathDigit, &PathGlow  );


		ASSERT( PathDigit );
		ASSERT( PathGlow );

		if( PathDigit && PathGlow ) // always true... but in case of :)
		{
			// Draw segment with ON color /////////////////////////////////////////
			Matrix TmpMatrix;
			TmpMatrix.Translate( pos[iIndex].X, pos[iIndex].Y );
			PathDigit->Transform( &TmpMatrix );
			pGraphics->FillPath( &SBrushON, PathDigit);
			if( GetGlowColor() )
			{
				// Create glow brush //////////////////////////////////////////////
				PathGlow->Transform( &TmpMatrix );
				PathGradientBrush PathBrushGlow( PathGlow );
				PathBrushGlow.SetCenterColor( TO_ARGB(255,GetGlowColor() ) );
				Color OutColor(0,0,0,0);
				int iNbColor = 1;
				PathBrushGlow.SetSurroundColors( &OutColor, &iNbColor );
				PathBrushGlow.SetBlend(fac, posfac, 4);
				pGraphics->FillPath( &PathBrushGlow, PathGlow);
			}
		}

		// Clean up ///////////////////////////////////////////////////////////////
		if( PathDigit )
			delete  PathDigit;
		if( PathGlow )
			delete  PathGlow;
	}

	// Draw dot if needed /////////////////////////////////////////////////////////
	for ( int iNumDot = 0; iNumDot < NB_SEGMENT_DOT; iNumDot++ )
	{
		if ( bShowDot[iNumDot] )
		{
			ASSERT( ( m_Style == Segment7 ) || (  m_Style ==  Segment16) );

			// Draw the dot ///////////////////////////////////////////////////////////
			Matrix TmpMatrix;
			TmpMatrix.Translate( pos[iDotIndex+iNumDot].X, pos[iDotIndex+iNumDot].Y );
			GraphicsPath *TmpPath = m_pathDot.Clone();
			TmpPath->Transform(&TmpMatrix);
			pGraphics->FillPath( &SBrushON, TmpPath);

			if ( GetGlowColor() )
			{
				GraphicsPath *TmpPathGlow = m_pathDotGlow.Clone();
				TmpPathGlow->Transform(&TmpMatrix);
				PathGradientBrush PathBrushGlow( TmpPathGlow );
				PathBrushGlow.SetCenterColor( TO_ARGB(255,GetGlowColor() ) );
				Color OutColor(0,0,0,0);
				int iNbColor = 1;
				PathBrushGlow.SetSurroundColors( &OutColor, &iNbColor );
				PathBrushGlow.SetBlend(fac, posfac, 4);
				pGraphics->FillPath( &PathBrushGlow, TmpPathGlow);
				delete TmpPathGlow;
			}

			if ( TmpPath )
				delete TmpPath;		
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::DrawGloss( Graphics* pGraphics, CRect &WindowRect)
{
	Rect lRect = m_CGdiBorder.GetClientRect();
	if ( ( m_CGdiBorder.GetShape() == CGdiBorder::Rectange )  ||
		 ( m_CGdiBorder.GetShape() == CGdiBorder::RoundRect )  )
	{
		float width = (float) lRect.Width;
		float height = (float) lRect.Height;

		float xOffset = (float) (m_iRoundBorder ? m_iRoundBorder / 4 : 2);
		float yOffset = (float) (m_iRoundBorder ? m_iRoundBorder / 4 : 2);
		RectF glossRect( xOffset,
						 yOffset,
						 width - 2* xOffset,
						 (float)(height * 0.6));
		glossRect.Offset( (REAL) lRect.X, (REAL) lRect.Y );

		LinearGradientBrush gradientBrush(glossRect, Color(m_bShowGlass,255,255,255), Color(0,255,255,255), LinearGradientModeVertical);

		GraphicsPath PathBorder;
		CRoundRect lCRoundRect;
		lCRoundRect.GetRoundRectPath( &PathBorder, Rect( (int)glossRect.X, (int)glossRect.Y, (int)glossRect.Width, (int)glossRect.Height),  m_iRoundBorder);
		pGraphics->FillPath(&gradientBrush, &PathBorder);
	}
	else
	{
		RectF glossRect( lRect.X + (float)(lRect.Width * 0.10),
						 lRect.Y  + (float)(lRect.Height * 0.03),
						 (float)(lRect.Width * 0.80),
						 (float)(lRect.Height * 0.7)  );

		RectF glossBrush( glossRect );
		glossBrush.Inflate(1,1);

		LinearGradientBrush gradientBrush(glossBrush, Color(m_bShowGlass,255,255,255), Color::Transparent, LinearGradientModeVertical);
		pGraphics->FillEllipse(&gradientBrush, glossRect);
	}
}

///////////////////////////////////////////////////////////////////////////////////
BOOL CIsiLcdDisplay::RebuildGDIbkGnd( Graphics* pGraphics, CRect &WindowRect)
{
	// Draw the border ////////////////////////////////////////////////////////////
	m_CGdiBorder.Draw( pGraphics, WindowRect );

	// Reset all path /////////////////////////////////////////////////////////////
	m_pathSegmentV.Reset();
	m_pathSegmentH.Reset();
	m_pathSegmentV_Glow.Reset();
	m_pathSegmentH_Glow.Reset();
	m_pathDot.Reset();
	m_pathDotGlow.Reset();
	m_pathSegmentDiag1.Reset();
	m_pathSegmentDiag2.Reset();
	m_pathSegmentDiag1_Glow.Reset();
	m_pathSegmentDiag2_Glow.Reset();

	// Recalculate rect of digit //////////////////////////////////////////////////
	if ( m_iValueNbDigit < m_iValuePrecision )
		m_iValueNbDigit = m_iValuePrecision;
	if( m_iValueNbDigit <= 0 )
		m_iValueNbDigit = 1;

	int iNbDigit = GetNbDigit();

	// Draw back color if control is not transparent ///////////////////////////////
	if( !m_bTransparent )
	{
		SolidBrush backGroundBrush( TO_ARGB(255,m_cColorBkgnd) );
		GraphicsPath PathBorder;
		m_CGdiBorder.GetClientPath( &PathBorder );
		pGraphics->FillPath(&backGroundBrush, &PathBorder);
	}

	if( ( m_CGdiBorder.GetShape() ==  CGdiBorder::Circle ) || 
		( m_CGdiBorder.GetShape() ==  CGdiBorder::Ellipse )			)
	{
		RectF RectFclient;
		RectFclient.X =		(REAL) m_CGdiBorder.GetClientRect().X;
		RectFclient.Y =		(REAL) m_CGdiBorder.GetClientRect().Y;
		RectFclient.Width = (REAL) m_CGdiBorder.GetClientRect().Width;
		RectFclient.Height =(REAL) m_CGdiBorder.GetClientRect().Height;
		REAL DimX = RectFclient.Width - RectFclient.Width/3;
		REAL DimY = sqrt(  RectFclient.Width/2 * RectFclient.Width/2 * 8/9 );
		WindowRect.SetRect( 0,0, (int) DimX, (int) DimY);
		WindowRect.OffsetRect( (int) (RectFclient.Width / 2) - WindowRect.Width() / 2, 
			(int) (RectFclient.Height /2) - WindowRect.Height() / 2 ); 
		WindowRect.OffsetRect( (int) RectFclient.X, (int) RectFclient.Y );
	}


	float fwidth = (float) WindowRect.Width() / (float) iNbDigit;
	float fhight = (float) WindowRect.Height();

	//RectF DigitRect( 0,0, width, hight );

	// Calculate constants ////////////////////////////////////////////////////////
	float fSegmentHight;
	float fLetRightGap = 0;
	float fUpDownGap = 0;
	switch( m_Style )
	{
	case Segment7:  fSegmentHight = fwidth * SEGMENT7_WIDTH_FACTOR * m_fSizeSegmentModifier; 
		fLetRightGap  = fwidth * LEFT_RIGHT_GAP_FACTOR;
		fUpDownGap	  = fhight * UP_DOWN_GAP_FACTOR;
		break;
	case Segment16: fSegmentHight = fwidth * SEGMENT16_WIDTH_FACTOR * m_fSizeSegmentModifier; 
		fLetRightGap  = fwidth * LEFT_RIGHT_GAP_FACTOR;
		fUpDownGap	  = fhight * UP_DOWN_GAP_FACTOR;
		break;
	case MatrixDot:
	case MatrixSquare: fSegmentHight = fwidth * MATRIX5x7_WIDTH_FACTOR * m_fSizeSegmentModifier; 
		fLetRightGap  = fwidth * LEFT_RIGHT_GAP_FACTOR_MATRIX;
		fUpDownGap	  = fhight * UP_DOWN_GAP_FACTOR_MATRIX;
		break;
	default: ASSERT( 0 );	// Invalid case
	}

	float fHalfSegmentHight = fSegmentHight / 2;



	float fSegmentHLength;
	switch( m_Style )
	{
	case Segment7:	fSegmentHLength = fwidth  - (2*fLetRightGap);		break;
	case Segment16:	fSegmentHLength = (fwidth  - (2*fLetRightGap))/2;	break;
	case MatrixDot:
	case MatrixSquare: fSegmentHLength = 0;								break; // Not used
	default: ASSERT( 0 );	// Invalid case
	}

	float fSegmentHMidleLength	= fSegmentHLength - fSegmentHight;
	float fSegmentVLength		= (fhight - (2*fUpDownGap) - (2*fSegmentHight)) / 2 + fHalfSegmentHight; // - 2* fSegmentSeparator;
	float fSegmentVMidleLength	= fSegmentVLength - fSegmentHight ;
	// Create matrix 
	Matrix MatrixSeparator;
	MatrixSeparator.Scale( SEGMENT_SEPARATOR_FACTOR,SEGMENT_SEPARATOR_FACTOR );

	if ( ( m_Style == Segment7 ) || ( m_Style == Segment16 ) )
	{
		// Create horizontal segment //////////////////////////////////////////////////
		PointF SegmentH[ 6 ];		// Horizontal segment
		SegmentH[0] = PointF(-fSegmentHLength/2, 0);
		SegmentH[1] = PointF(-fSegmentHMidleLength/2, -fHalfSegmentHight);
		SegmentH[2] = PointF(fSegmentHMidleLength/2, -fHalfSegmentHight);
		SegmentH[3] = PointF(fSegmentHLength/2, 0);
		SegmentH[4] = PointF(fSegmentHMidleLength/2, fHalfSegmentHight);
		SegmentH[5] = PointF(-fSegmentHMidleLength/2, fHalfSegmentHight);

		// Create vertical segment /////////////////////////////////////////////////////
		PointF SegmentV[ 6 ];		// Vertical segment
		SegmentV[0] = PointF(0,-fSegmentVLength/2);
		SegmentV[1] = PointF(fHalfSegmentHight, -fSegmentVMidleLength/2);
		SegmentV[2] = PointF(fHalfSegmentHight,  fSegmentVMidleLength/2);
		SegmentV[3] = PointF(0, fSegmentVLength/2);
		SegmentV[4] = PointF(-fHalfSegmentHight, fSegmentVMidleLength/2 );
		SegmentV[5] = PointF(-fHalfSegmentHight, -fSegmentVMidleLength/2);

		// Create glow effect matrix //////////////////////////////////////////////////
		Matrix MatrixGlowV;
		MatrixGlowV.Scale( GLOW_SCALE_Y,GLOW_SCALE_X );
		// Create vertical segment path ///////////////////////////////////////////////
		m_pathSegmentV.AddPolygon(SegmentV, 6);
		m_pathSegmentV.Transform( &MatrixSeparator );
		m_pathSegmentV_Glow.AddPolygon(SegmentV, 6);
		m_pathSegmentV_Glow.Transform( &MatrixGlowV );

		// Create horizontal segment path /////////////////////////////////////////////
		Matrix MatrixGlowH;
		MatrixGlowH.Scale( GLOW_SCALE_X,GLOW_SCALE_Y );
		m_pathSegmentH.AddPolygon(SegmentH, 6);
		m_pathSegmentH.Transform( &MatrixSeparator );
		m_pathSegmentH_Glow.AddPolygon(SegmentH, 6);
		m_pathSegmentH_Glow.Transform( &MatrixGlowH );
	}

	if ( m_Style == Segment16 )
	{
		// Create diagonal segment (type1) ////////////////////////////////////////////
		PointF SegmentDiag1[ 6 ];		// Diagonal segment type1
		float fHalfSegmentDiag1Length	= fSegmentHMidleLength/2;
		float fHalfSegmentDiag1Height	= fSegmentVMidleLength/2;
		float fSegmentDiag1Wide			= fSegmentHight/2;

		SegmentDiag1[0] = PointF(-fHalfSegmentDiag1Length,-fHalfSegmentDiag1Height);
		SegmentDiag1[1] = PointF(-fHalfSegmentDiag1Length+fSegmentDiag1Wide,-fHalfSegmentDiag1Height);
		SegmentDiag1[2] = PointF(fHalfSegmentDiag1Length,  fHalfSegmentDiag1Height-fSegmentDiag1Wide);
		SegmentDiag1[3] = PointF(fHalfSegmentDiag1Length, fHalfSegmentDiag1Height);
		SegmentDiag1[4] = PointF(fHalfSegmentDiag1Length-fSegmentDiag1Wide, fHalfSegmentDiag1Height );
		SegmentDiag1[5] = PointF(-fHalfSegmentDiag1Length, -fHalfSegmentDiag1Height +fSegmentDiag1Wide);

		// Create diagonal segment (type2) ////////////////////////////////////////////
		PointF SegmentDiag2[ 6 ];		// Diagonal segment type2
		SegmentDiag2[0] = PointF(fHalfSegmentDiag1Length,-fHalfSegmentDiag1Height);
		SegmentDiag2[1] = PointF(fHalfSegmentDiag1Length-fSegmentDiag1Wide,-fHalfSegmentDiag1Height);
		SegmentDiag2[2] = PointF(-fHalfSegmentDiag1Length,  fHalfSegmentDiag1Height-fSegmentDiag1Wide);
		SegmentDiag2[3] = PointF(-fHalfSegmentDiag1Length, fHalfSegmentDiag1Height);
		SegmentDiag2[4] = PointF(-fHalfSegmentDiag1Length+fSegmentDiag1Wide, fHalfSegmentDiag1Height );
		SegmentDiag2[5] = PointF(fHalfSegmentDiag1Length, -fHalfSegmentDiag1Height +fSegmentDiag1Wide);

		// Create diagonal1 segment path /////////////////////////////////////////////
		Matrix MatrixGlowDiag1;
		MatrixGlowDiag1.Scale( GLOW_SCALE_X_DIAG,GLOW_SCALE_Y_DIAG);
		m_pathSegmentDiag1.AddPolygon(SegmentDiag1, 6);
		m_pathSegmentDiag1.Transform( &MatrixSeparator );
		m_pathSegmentDiag1_Glow.AddPolygon(SegmentDiag1, 6);
		m_pathSegmentDiag1_Glow.Transform( &MatrixGlowDiag1 );

		//Unfortunately the non rational scale will change the angle of the segment, 
		//so we have to calculate the original angle, the final angle and make  
		//a rotation to neutralize the unwanted rotation.
		PointF OroginalPoint,ScaledPoint;
		m_pathSegmentDiag1.GetLastPoint(&OroginalPoint);
		m_pathSegmentDiag1_Glow.GetLastPoint(&ScaledPoint);
		Matrix MatrixRotCorrectionDiag1;
		float lSeg1 = sqrt( (OroginalPoint.X *OroginalPoint.X) + (OroginalPoint.Y * OroginalPoint.Y) );
		float lSeg2 = sqrt( (ScaledPoint.X   *ScaledPoint.X  ) + (ScaledPoint.Y * ScaledPoint.Y    ) );
		float fAngle1 = acos( OroginalPoint.X / lSeg1) * (float) 180 / (float) 3.14116; // Grad to degree conversion
		float fAngle2 = acos(  ScaledPoint.X  / lSeg2) * (float) 180 / (float) 3.14116; // Grad to degree conversion
		MatrixRotCorrectionDiag1.Rotate( -(fAngle1 - fAngle2)  );
		m_pathSegmentDiag1_Glow.Transform( &MatrixRotCorrectionDiag1 );

		// Create diagonal2 segment path /////////////////////////////////////////////
		Matrix MatrixGlowDiag2;
		MatrixGlowDiag2.Scale( GLOW_SCALE_X_DIAG,GLOW_SCALE_Y_DIAG );
		m_pathSegmentDiag2.AddPolygon(SegmentDiag2, 6);
		m_pathSegmentDiag2.Transform( &MatrixSeparator );
		m_pathSegmentDiag2_Glow.AddPolygon(SegmentDiag2, 6);
		m_pathSegmentDiag2_Glow.Transform( &MatrixGlowDiag2 );
		Matrix MatrixRotCorrectionDiag2;
		MatrixRotCorrectionDiag2.Rotate( (fAngle1 - fAngle2)  );
		m_pathSegmentDiag2_Glow.Transform( &MatrixRotCorrectionDiag2 );
	}

	// Create dot path ////////////////////////////////////////////////////////////
	Matrix MatrixGlowDot;
	MatrixGlowDot.Scale( GLOW_SCALE_X,GLOW_SCALE_X );
	if ( m_Style != MatrixSquare )
	{
		m_pathDot.AddEllipse(-fHalfSegmentHight,-fHalfSegmentHight,fSegmentHight,fSegmentHight);
		m_pathDotGlow.AddEllipse(-fHalfSegmentHight,-fHalfSegmentHight,fSegmentHight,fSegmentHight);
	}
	else
	{
		m_pathDot.AddRectangle( RectF(-fHalfSegmentHight,-fHalfSegmentHight,fSegmentHight,fSegmentHight ) );
		m_pathDotGlow.AddEllipse( RectF(-fHalfSegmentHight,-fHalfSegmentHight,fSegmentHight,fSegmentHight) );
	}
	m_pathDotGlow.Transform( &MatrixGlowDot );

	// Create sign segment if needed ///////////////////////////////////////////////
	if( m_bShowSign )
	{
		// Create negative sign ////////////////////////////////////////////////////
		PointF SegmentMinus[ 4 ];		// Minus segment
		SegmentMinus[0] = PointF(-fSegmentHMidleLength/2,-fSegmentHight/2);
		SegmentMinus[1] = PointF(fSegmentHMidleLength/2,-fSegmentHight/2);
		SegmentMinus[2] = PointF(fSegmentHMidleLength/2, fSegmentHight/2);
		SegmentMinus[3] = PointF(-fSegmentHMidleLength/2, fSegmentHight/2);


		// Create plus sign ///////////////////////////////////////////////////////
		PointF SegmentPlus[ 12 ];		// plus segment
		SegmentPlus[0] = PointF(-fSegmentHMidleLength/2, -fSegmentHight/2);
		SegmentPlus[1] = PointF(-fSegmentHMidleLength/6, -fSegmentHight/2);
		SegmentPlus[2] = PointF(-fSegmentHMidleLength/6, -fSegmentHMidleLength/2);
		SegmentPlus[3] = PointF(fSegmentHMidleLength/6, -fSegmentHMidleLength/2);
		SegmentPlus[4] = PointF(fSegmentHMidleLength/6, -fSegmentHMidleLength/6);
		SegmentPlus[5] = PointF(fSegmentHMidleLength/2, -fSegmentHMidleLength/6);
		SegmentPlus[6] = PointF(fSegmentHMidleLength/2,  fSegmentHMidleLength/6);
		SegmentPlus[7] = PointF(fSegmentHMidleLength/6,  fSegmentHMidleLength/6);
		SegmentPlus[8] = PointF(fSegmentHMidleLength/6,  fSegmentHMidleLength/2);
		SegmentPlus[9] = PointF(-fSegmentHMidleLength/6, fSegmentHMidleLength/2);
		SegmentPlus[10] = PointF(-fSegmentHMidleLength/6, fSegmentHMidleLength/6);
		SegmentPlus[11] = PointF(-fSegmentHMidleLength/2, fSegmentHMidleLength/6);

		m_pathSegmentMinus.Reset();
		m_pathSegmentPlus.Reset();
		m_pathSegmentMinus_Glow.Reset();
		m_pathSegmentPlus_Glow.Reset();

		// Create minus segment path //////////////////////////////////////////////
		Matrix MatrixGlowMinus;
		MatrixGlowMinus.Scale( GLOW_SCALE_X, GLOW_SCALE_Y );
		m_pathSegmentMinus.AddPolygon(SegmentMinus, 4);
		m_pathSegmentMinus_Glow.AddPolygon(SegmentMinus, 4);
		m_pathSegmentMinus_Glow.Transform( &MatrixGlowMinus );

		// Create plus segment path ///////////////////////////////////////////////
		Matrix MatrixGlowPlus;
		MatrixGlowPlus.Scale( GLOW_SCALE_X, GLOW_SCALE_X );
		m_pathSegmentPlus.AddPolygon(SegmentPlus, 12);
		m_pathSegmentPlus_Glow.AddPolygon(SegmentPlus, 12);
		m_pathSegmentPlus_Glow.Transform( &MatrixGlowPlus );
	}

	// Create segments positions ///////////////////////////////////////////////////
	if ( m_Style == Segment7 )
	{ 
		m_7SegmentsPosition[0] = PointF(fwidth/2,fUpDownGap + fHalfSegmentHight );				// H
		m_7SegmentsPosition[1] = PointF(fwidth-fLetRightGap, fSegmentVLength /2 + fUpDownGap + fHalfSegmentHight/*hight / 4 + fSegmentSeparator + fUpDownGap/2*/ );	// V
		m_7SegmentsPosition[2] = PointF(fwidth-fLetRightGap, fhight / 2 /*+ fSegmentSeparator*/ + fSegmentVLength / 2);		// V
		m_7SegmentsPosition[3] = PointF(fwidth/2,fhight - fUpDownGap - fHalfSegmentHight );		// H
		m_7SegmentsPosition[4] = PointF(fLetRightGap,fhight / 2 /*+ fSegmentSeparator*/ + fSegmentVLength / 2);			// V
		m_7SegmentsPosition[5] = PointF(fLetRightGap, fSegmentVLength /2 + fUpDownGap + fHalfSegmentHight);			// V
		m_7SegmentsPosition[6] = PointF(fwidth/2,fhight / 2);										// H
		m_7SegmentsPosition[7] = PointF(fwidth/2, fhight / 2 /*+ fSegmentSeparator*/ + fSegmentVLength / 2);// -
		m_7SegmentsPosition[8] = PointF(fwidth/2, fhight / 4 /*+ fSegmentSeparator*/ + fUpDownGap /2);// +
		m_7SegmentsPosition[9] = PointF(fwidth/*-fHalfSegmentHight*/, fhight - fUpDownGap  );			// .

		m_7SegmentsPosition[10] = PointF(fwidth, fSegmentVLength /2 + fUpDownGap + fHalfSegmentHight  );	// ( : 1/2)
		m_7SegmentsPosition[11] = PointF(fwidth, fhight / 2 + fSegmentVLength / 2 );						// ( : 2/2)
	}
	else if ( m_Style == Segment16 )
	{
		m_16SegmentsPosition[0] = PointF( fSegmentHLength/2 + fLetRightGap, fUpDownGap + fHalfSegmentHight );				// H
		m_16SegmentsPosition[1] = PointF( fSegmentHLength*1.5f   + fLetRightGap, fUpDownGap + fHalfSegmentHight );				// H

		m_16SegmentsPosition[2] = PointF(fwidth-fLetRightGap, fSegmentVLength /2 + fUpDownGap + fHalfSegmentHight);	// V
		m_16SegmentsPosition[3] = PointF(fwidth-fLetRightGap, fhight / 2 + fSegmentVLength / 2);		// V

		m_16SegmentsPosition[4] = PointF(fSegmentHLength*1.5f   + fLetRightGap,fhight - fUpDownGap - fHalfSegmentHight );		// H
		m_16SegmentsPosition[5] = PointF(fSegmentHLength/2 + fLetRightGap,fhight - fUpDownGap - fHalfSegmentHight );		// H

		m_16SegmentsPosition[6] = PointF(fLetRightGap, fhight / 2 + fSegmentVLength / 2);			// V
		m_16SegmentsPosition[7] = PointF(fLetRightGap, fSegmentVLength /2 + fUpDownGap + fHalfSegmentHight);			// V

		m_16SegmentsPosition[8] = PointF(fwidth/2,fSegmentVLength /2 + fUpDownGap + fHalfSegmentHight);										// H
		m_16SegmentsPosition[9] = PointF(fwidth/2,fhight / 2 + fSegmentVLength / 2 );				// H

		m_16SegmentsPosition[10] = PointF(fSegmentHLength/2    + fLetRightGap,  fhight / 2  );		// H
		m_16SegmentsPosition[11] = PointF(fSegmentHLength*1.5f + fLetRightGap,  fhight / 2  );			// V


		m_16SegmentsPosition[12] = PointF(fSegmentHLength/2 + fLetRightGap     , fSegmentVLength /2 + fUpDownGap + fHalfSegmentHight);			// V
		m_16SegmentsPosition[13] = PointF(fSegmentHLength*1.5f   + fLetRightGap, fSegmentVLength /2 + fUpDownGap + fHalfSegmentHight);	

		m_16SegmentsPosition[14] = PointF(fSegmentHLength*1.5f  + fLetRightGap, fhight / 2 + fSegmentVLength / 2);			// V
		m_16SegmentsPosition[15] = PointF(fSegmentHLength/2 + fLetRightGap    , fhight / 2 + fSegmentVLength / 2);	


		m_16SegmentsPosition[16] = PointF(fwidth/2, fhight / 2  + fSegmentVLength / 2);						// -
		m_16SegmentsPosition[17] = PointF(fwidth/2, fhight / 4  + fUpDownGap /2);								// +
		m_16SegmentsPosition[18] = PointF(fwidth, fhight - fUpDownGap  );										// .

		m_16SegmentsPosition[19] = PointF(fwidth, fSegmentVLength /2 + fUpDownGap + fHalfSegmentHight  );	// ( : 1/2)
		m_16SegmentsPosition[20] = PointF(fwidth, fhight / 2 + fSegmentVLength / 2 );							// ( : 2/2)
	}
	else
	{
		// Check if matrix is loaded //////////////////////////////////////////////
		// Otherwise load the default matrix //////////////////////////////////////
		if( m_pMatrixData )
		{
//			LoadMatrix( IDB_MATRIX5x7 ); // Load default matrix 

			// Prepare matrix dot position ////////////////////////////////////////////
			float fStepX = (fwidth - 2*fLetRightGap)/ (m_MatrixSize.cx-1);
			float fStepY = (fhight - 2*fUpDownGap)  / (m_MatrixSize.cy-1);
			int iIndex = 0;
			for ( float fIndexY = 0; fIndexY < m_MatrixSize.cy; fIndexY++ )
			{
				for ( float fIndexX = 0; fIndexX < m_MatrixSize.cx; fIndexX++ )
				{
					ASSERT( m_pDotsMatrixPosition );
					m_pDotsMatrixPosition[iIndex] = PointF( fLetRightGap + fIndexX*fStepX /*+ fHalfSegmentHight*/,
						fUpDownGap   + fIndexY*fStepY /*+ fHalfSegmentHight*/ );
					iIndex++;
				}
			}
		}
		else
		{
			ASSERT( 0 );
			// you must call LoadMatrix before
		}
	}

	// Draw background segment ////////////////////////////////////////////////////
	if ( m_bShowOffSegments )
	{
		for ( int iIndex =0; iIndex < iNbDigit; iIndex++ )
		{
			//pGraphics->ResetTransform();
			Matrix TmpMatrix;
			TmpMatrix.Translate( iIndex * fwidth + WindowRect.left, (float) WindowRect.top );
			pGraphics->SetTransform( &TmpMatrix );
			DrawBackgroundDigit( pGraphics, m_bShowSign && !iIndex, iIndex == (iNbDigit-1) );
		}
	}

	return CIsiCompBase::RebuildGDIbkGnd(pGraphics,WindowRect);
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::SetBlink( int iPeriod )
{
	m_iBlink = iPeriod;
	if( m_hWnd )
	{
		if( m_iBlink )
		{
			SetTimer( ISI_SEGMENT_GAUGE_BLINK_TIMER, m_iBlink, NULL );
		}
		else
		{
			KillTimer( ISI_SEGMENT_GAUGE_BLINK_TIMER );
			m_BlinkStatus = TRUE;
			Invalidate();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
int CIsiLcdDisplay::GetBlink( ) const
{
	return m_iBlink;
}

///////////////////////////////////////////////////////////////////////////////////
// Set segments color 
// cOff is the off segment color 0 means auto calculate off color
// cGlow is the light effect color 0 means auto calculate color -1 means no light effect
///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::SetColor( COLORREF cOn, COLORREF cOff /*= 0*/, COLORREF cGlow /*= 0*/)
{
	m_cColorSegmentOn	= cOn;
	m_cColorSegmentOff  = cOff;
	m_cColorSegmentGlow = cGlow;

	InvalidateBkGnd();	 // We must regenerate the background								
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::ShowSign( BOOL bShow , BOOL bRedraw /*= TRUE*/ )
{
	m_bShowSign = bShow;
	InvalidateBkGnd();
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::HexMode( BOOL bHex, BOOL bRedraw /*= TRUE*/ )
{
	m_bHexMode = bHex;
	InvalidateBkGnd();	 // We must regenerate the background		
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::ShowOffSegment( BOOL bShow , BOOL bRedraw /*= TRUE*/ )
{
	m_bShowOffSegments = bShow;
	if( m_hWnd && bRedraw )
	{
		Invalidate();									// Redraw the control
	}
}

///////////////////////////////////////////////////////////////////////////////////
BOOL CIsiLcdDisplay::IsOffSegmentVisible()
{
	return m_bShowOffSegments;
}

///////////////////////////////////////////////////////////////////////////////////
// If bString = TRUE you must use SetWindowText() method to update the control value
// otherwise use SetValue() method
///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::StringMode( BOOL bString, BOOL bRedraw /*= TRUE*/ )
{
	m_bStringMode = bString;
	InvalidateBkGnd();	 // We must regenerate the background		
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::EnableValue( BOOL bEnable )
{
	m_bEnableValue = bEnable;
	if( m_hWnd )
	{
		Invalidate();									// Redraw the control
	}
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::SetSegmentSize( float fSize, BOOL bRedraw /*= TRUE*/ )
{
	m_fSizeSegmentModifier = fSize;
	if( m_hWnd )
	{
		Invalidate();									// Redraw the control
	}
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::SetSegmentStyle( SegmentStyle Style, BOOL bRedraw /*= TRUE*/  )
{
	m_Style = Style;
	InvalidateBkGnd();	 // We must regenerate the background		
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::SetScrollSpeed( int iSpeed )
{
	m_iScrollSpeed = iSpeed;
	m_iCurPosition = 0;
	if( m_hWnd )
	{
		if( m_iScrollSpeed )
		{
			SetTimer( ISI_SEGMENT_GAUGE_SCROLL_TIMER, m_iScrollSpeed, NULL );
		}
		else
		{
			KillTimer( ISI_SEGMENT_GAUGE_SCROLL_TIMER );
			Invalidate();
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////
BOOL CIsiLcdDisplay::LoadMatrix( UINT uiRes )
{
	// Load the bitmap from resource //////////////////////////////////////////////
	CBitmap lBitmap;
	if ( !lBitmap.LoadBitmap( uiRes ) )
		return FALSE;

	BOOL bReturn = LoadMatrix( &lBitmap );

	lBitmap.DeleteObject(); // Delete the bitmap, we don't need it anymore

	return bReturn;
}

///////////////////////////////////////////////////////////////////////////////////
BOOL CIsiLcdDisplay::LoadMatrix( LPCTSTR lpzFileName )
{
	// Load the bitmap from file //////////////////////////////////////////////////
	CBitmap lBitmap;
	if ( !lBitmap.Attach( (HBITMAP)::LoadImage(NULL,lpzFileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE) ) )
		return FALSE;

	BOOL bReturn = LoadMatrix( &lBitmap );

	lBitmap.DeleteObject(); // Delete the bitmap, we don't need it anymore

	return bReturn;
}


///////////////////////////////////////////////////////////////////////////////////
BOOL CIsiLcdDisplay::LoadMatrix( CBitmap *pBitmap )
{
	// Delete existing matrix if any //////////////////////////////////////////////
	if ( m_pMatrixData )
		delete m_pMatrixData;
	if ( m_pDotsMatrixPosition )
		delete m_pDotsMatrixPosition;
	m_pMatrixData		 = NULL;
	m_pDotsMatrixPosition= NULL;
	m_MatrixSize.SetSize(0,0);


	CDC dc;
	dc.CreateCompatibleDC( NULL );
	HGDIOBJ hOld = dc.SelectObject( *pBitmap );

	// Get bitmap infos ///////////////////////////////////////////////////////////
	BITMAP BitmpInfos;
	pBitmap->GetBitmap( &BitmpInfos );

	// First pixel identify background color //////////////////////////////////////
	COLORREF bkColor = dc.GetPixel(0,0); // upper left pixel

	// Get matrix width ///////////////////////////////////////////////////////////
	while ( m_MatrixSize.cx < MAX_MATRIX_SIZE_X )
	{
		m_MatrixSize.cx++;
		if( dc.GetPixel(m_MatrixSize.cx,0) != bkColor )
			break;
	}

	// Get matrix hight ////////////////////////////////////////////////////////////
	while ( m_MatrixSize.cy < MAX_MATRIX_SIZE_Y )
	{
		m_MatrixSize.cy++;
		if( dc.GetPixel(0,m_MatrixSize.cy) != bkColor )
			break;
	}

	// Check matrix size ///////////////////////////////////////////////////////////
	if ( ( m_MatrixSize.cx > MAX_MATRIX_SIZE_X ) || ( m_MatrixSize.cy > MAX_MATRIX_SIZE_Y ) )
	{
		// Invalid size ////////////////////////////////////////////////////////////
		m_MatrixSize.SetSize(0,0);
		return FALSE;
	}

	// Check bitmap dimension //////////////////////////////////////////////////////
	if ( BitmpInfos.bmWidth < (m_MatrixSize.cx + 1) * MATRIX_BITMAP_NB_CHAR_PER_LINE )
	{
		// Invalid bitmap size 
		return FALSE;
	}
	if ( BitmpInfos.bmHeight < (m_MatrixSize.cy + 1) * MATRIX_BITMAP_NB_LINE  )
	{
		// Invalid bitmap size 
		return FALSE;
	}

	m_pMatrixData		  = new BYTE[ m_MatrixSize.cx * m_MatrixSize.cy * NB_CHAR_TABLE_MATRIX];
	m_pDotsMatrixPosition = new PointF[ m_MatrixSize.cx * m_MatrixSize.cy ];

	// Read matrix ////////////////////////////////////////////////////////////////
	int iDigitNumber = 0;
	for (int iY = 0; iY < MATRIX_BITMAP_NB_LINE; iY++ )
	{
		for (int iX = 0; iX < MATRIX_BITMAP_NB_CHAR_PER_LINE; iX++ )
		{
			for ( int iPixelY = 0; iPixelY < m_MatrixSize.cy; iPixelY++ )
			{
				for ( int iPixelX = 0; iPixelX < m_MatrixSize.cx; iPixelX++ )
				{
					COLORREF Col = dc.GetPixel(iX * (m_MatrixSize.cx + 1) + iPixelX, iY * (m_MatrixSize.cy + 1) + iPixelY );
					*(m_pMatrixData+iDigitNumber) = ( Col != bkColor );
					iDigitNumber++;
				}
			}
		}
	}

	dc.SelectObject(hOld );	
	dc.DeleteDC();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::OnPaint(Graphics* pGraphics, CRect &WindowRect )
{
	// Draw on segments only if value is enabled and the blink state is ON ////////
	if( m_BlinkStatus && m_bEnableValue )
	{
		// Recalculate rect of digit //////////////////////////////////////////////
		float width = (float) WindowRect.Width() / (float) GetNbDigit();

		CString StrValue;

		if( m_bStringMode )
		{
			GetWindowText( StrValue );
		}
		else
		{
			CString StrTmpFormat;
			if( m_bHexMode )
			{
				// Format value in hex mode ///////////////////////////////////////
				StrTmpFormat.Format( _T("%%0%dx"), m_iValueNbDigit);
				StrValue.Format( (LPCTSTR) StrTmpFormat, (int) m_dCurrentValue);
			}
			else
			{
				// Format value in decimal mode ///////////////////////////////////
				int iNbDigit = m_iValueNbDigit;
				if (  ( m_Style == Segment7 ) || (  m_Style ==  Segment16)  )
				{
					if( m_dCurrentValue< 0 )
						iNbDigit++;
				}
				else
				{
					if( ( m_dCurrentValue>= 0 ) && ( m_bShowSign ) )
						iNbDigit--;
				}
				StrTmpFormat.Format( _T("%%0%d.%df"), iNbDigit, m_iValuePrecision);
				StrValue.Format( (LPCTSTR) StrTmpFormat, m_dCurrentValue);


				if ( m_bShowSign && m_dCurrentValue >= 0 )
					StrValue.Insert(0,_T('+') );

				StrValue.Replace( _T(','), _T('.'));

				if (  ( m_Style == Segment7 ) || (  m_Style ==  Segment16)  )
				{
					if( ( StrValue.Find(_T('.')) != - 1 )		     && 
						( StrValue.GetLength() < GetNbDigit() + 1 )     )
					{
						if (StrValue[0] == _T('-') || StrValue[0] == _T('+') )
							StrValue.Insert(1,_T('0') );
						else
							StrValue.Insert(0,_T('0') );
					}
				}
			}

		}

		// Manage scroll option ///////////////////////////////////////////////////
		if( m_iScrollSpeed )
		{
			for (int iIndex = 0; iIndex <  GetNbDigit() -1; iIndex++ )
				StrValue.Insert(0,_T(' '));
			if ( m_iCurPosition >= StrValue.GetLength() )
				m_iCurPosition = 0;
		}
		else
		{
			ASSERT( m_iCurPosition == 0 );
		}

		// Draw lighted segments //////////////////////////////////////////////////
		int iTranslate = 0;
		for ( int iIndex =m_iCurPosition; iIndex < StrValue.GetLength(); iIndex++ )
		{
			// Skip the dot ///////////////////////////////////////////////////////
			if (  ( m_Style == Segment7 ) || (  m_Style ==  Segment16) )
			{
				if ( StrValue[iIndex] == _T('.') || StrValue[iIndex] == _T(':'))
					continue;
			}

			//pGraphics->ResetTransform();
			Matrix TmpMatrix;
			//TmpMatrix.Translate( iTranslate * width ,0 );
			TmpMatrix.Translate( iTranslate * width + WindowRect.left, (float) WindowRect.top );
			pGraphics->SetTransform( &TmpMatrix );
			BOOL bShowDot[NB_SEGMENT_DOT];
			ZeroMemory( bShowDot, sizeof(bShowDot) );

			if (  ( m_Style == Segment7 ) || (  m_Style ==  Segment16) )
			{
				if( iIndex < StrValue.GetLength() - 1)
				{
					if ( StrValue[iIndex + 1] == _T('.') )
					{
						bShowDot[0] = TRUE;
					}
					else if ( StrValue[iIndex + 1] == _T(':') )
					{
						bShowDot[1] = TRUE;
						bShowDot[2] = TRUE;
					}
				}
			}

			// 			if( ( iIndex == 0) && ( m_bShowSign ) )
			// 			{
			// 
			// 			}
			DrawBackgroundDigitOn( pGraphics, StrValue[iIndex], bShowDot  );
			iTranslate++;
			// Do not display characters outside the control //////////////////////
			if ( iTranslate >= GetNbDigit() )
				break;

		}

		// Reset transformation ///////////////////////////////////////////////////////
		//	pGraphics->ResetTransform();
	}

	// Draw the glass effect if needed ////////////////////////////////////////////
	//Matrix TmpMatrix;
	//TmpMatrix.Translate( (float) WindowRect.left, (float) WindowRect.top );
	//pGraphics->SetTransform( &TmpMatrix );
	pGraphics->ResetTransform();
	if ( m_bShowGlass )
	{
		DrawGloss( pGraphics, WindowRect );
	}
}

///////////////////////////////////////////////////////////////////////////////////
CGdiBorder*  CIsiLcdDisplay::GetBorder()
{
	return &m_CGdiBorder;
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::InvalidateBorder( BOOL bRedraw /*= TRUE */)
{
	InvalidateBkGnd();
	if ( m_hWnd && bRedraw )
		Invalidate();
}

///////////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CIsiSegmentGauge
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CIsiLcdDisplay, CIsiCompBase)
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////
void CIsiLcdDisplay::OnTimer(UINT_PTR nIDEvent)
{
	if ( nIDEvent == ISI_SEGMENT_GAUGE_BLINK_TIMER ) // Manage blinking
	{
		m_BlinkStatus = !m_BlinkStatus;
		Invalidate();
		return;
	}
	else if ( nIDEvent == ISI_SEGMENT_GAUGE_SCROLL_TIMER ) // Manage scroll
	{
		m_iCurPosition++;
		Invalidate();
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////////
int CIsiLcdDisplay::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CIsiCompBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Needed if the control has been configured before creation //////////////////
	SetBlink( m_iBlink );
	SetScrollSpeed( m_iScrollSpeed );

	return 0;
}
