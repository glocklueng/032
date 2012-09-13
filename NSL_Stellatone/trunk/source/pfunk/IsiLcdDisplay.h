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


#pragma once

#include "IsiCompBase.h"
#include "GdiBorder.h"

#define ISI_SEGMENT_GAUGE_BLINK_TIMER		1
#define ISI_SEGMENT_GAUGE_SCROLL_TIMER		2

#define SEGMENT_7_ARRAY_SIZE				12
#define SEGMENT_16_ARRAY_SIZE				21

#define MAX_MATRIX_SIZE_X					20		// We have to define a limit
#define MAX_MATRIX_SIZE_Y					20		// We have to define a limit

//! An enumeration of the different style
enum SegmentStyle
{
	Segment7 = 0,
	Segment16,
	MatrixDot,
	MatrixSquare,
};

///////////////////////////////////////////////////////////////////////////////////
class CIsiLcdDisplay : public CIsiCompBase
{
	//DECLARE_DYNAMIC(CIsiLcdDisplay)
protected:
	SegmentStyle m_Style;								// Segments style

	int		m_iBlink;									// Blink delay
	BOOL	m_BlinkStatus;								// Blink status

	BOOL	m_bHexMode;									// Hex mode flag
	BOOL	m_bStringMode;								// String mode flag
	BOOL	m_bShowOffSegments;							// Show off segments flag
	BOOL	m_bShowSign;								// Show sign
	BOOL	m_bEnableValue;								// Enable value status

	// Scrolling data member ///////////////////////////////////////////////
	int		m_iScrollSpeed;								// ScroolSpeed ( 0 no scroll )
	int		m_iCurPosition;								// Current position

	float	m_fSizeSegmentModifier;						// Adjust the size (scale)of segment (1 no modification)

	COLORREF m_cColorSegmentOn;							// Color segment on
	COLORREF m_cColorSegmentOff;						// Color segment off
	COLORREF m_cColorSegmentGlow;						// Color segment glow

	PointF	 m_7SegmentsPosition[ SEGMENT_7_ARRAY_SIZE ];	// Segments position [0;6]  Digit segment
															//[7]    Plus segment
															//[8]    Minus segment
															//[9]    Dot segment
															//[10:11]Dot segment ( : )

	PointF	 m_16SegmentsPosition[ SEGMENT_16_ARRAY_SIZE];	// Segments position [0;6] Digit segment
															//[16]  Plus segment
															//[17]  Minus segment
															//[18]  Dot segment
															//[19:20]Dot segment ( : )
	PointF	 *m_pDotsMatrixPosition;					// Dot position 
	CSize    m_MatrixSize;								// Matrix dimension
	BYTE	 *m_pMatrixData;							// Matrix data;

	GraphicsPath m_pathSegmentV;						// Vertical segment path
	GraphicsPath m_pathSegmentH;						// Horizontal segment path
	GraphicsPath m_pathSegmentV_Glow;					// Vertical segment path glow
	GraphicsPath m_pathSegmentH_Glow;					// Horizontal segment path glow

	GraphicsPath m_pathSegmentMinus;					// Minus segment path
	GraphicsPath m_pathSegmentPlus;						// Plus segment path
	GraphicsPath m_pathSegmentMinus_Glow;				// Minus segment path glow
	GraphicsPath m_pathSegmentPlus_Glow;				// Plus segment path glow

	GraphicsPath m_pathDot;								// Dot path
	GraphicsPath m_pathDotGlow;							// Dot path glow

	GraphicsPath m_pathSegmentDiag1;					// Diagonal segment path
	GraphicsPath m_pathSegmentDiag2;					// Diagonal segment path
	GraphicsPath m_pathSegmentDiag1_Glow;				// Diagonal segment path glow
	GraphicsPath m_pathSegmentDiag2_Glow;				// Diagonal segment path glow

	CGdiBorder	 m_CGdiBorder;							// Border

protected:
	int		 GetNbDigit();								// Return number of digit according with current settings
	COLORREF GetColorOff();								// Return Off color
	COLORREF GetGlowColor();							// Return Glow color

	void    GetSegmentPath( int iIndex, GraphicsPath **PathDigit,  GraphicsPath **PathGlow  );

	void	DrawBackgroundDigit(Graphics* pGraphics, BOOL bSign, BOOL bLastDigit );
	void	DrawBackgroundDigitOn(Graphics* pGraphics, TCHAR Digit, BOOL bShowDot[]);
	void	DrawGloss( Graphics* pGraphics, CRect &WindowRect);
	virtual BOOL	RebuildGDIbkGnd(Graphics* pGraphics,		// Rebuild GDI background
		CRect &WindowRect );
	virtual	void	OnPaint(Graphics* pGraphics, CRect &WindowRect );	// Paint 

public:
			 CIsiLcdDisplay();									// Standard constructor
	virtual ~CIsiLcdDisplay();									// Destructor
	void	 SetBlink( int iPeriod );							// Set blink period ( 0 = stop blink)
	int		 GetBlink( ) const;									// Return blink period
	void	 ShowOffSegment( BOOL bShow , BOOL bRedraw = TRUE );// Show or hide off segments;
	BOOL	 IsOffSegmentVisible();								// Return TRUE if off segments are visible or not
	void	 StringMode( BOOL bString , BOOL bRedraw = TRUE );	// Set string mode ( if TRUE SetWindowText() method 
																// should be used to update control value
																// otherwise use SetValue method ).
	BOOL	 IsStringMode() 		{return m_bStringMode;}		// Return string mode status
	void	 ShowSign( BOOL bShow , BOOL bRedraw = TRUE );		// Show sign
	BOOL	 IsShowSign()			{return m_bShowSign;}		// Return show sign status
	void	 HexMode( BOOL bHex, BOOL bRedraw = TRUE );			// Set hex mode display
	BOOL	 IsHexMode()			{return m_bHexMode;}		// Return hex mode display
	void	 EnableValue( BOOL bEnable );						// Enable value
	BOOL	 IsEnableValue()		{return m_bEnableValue;}	// Return enable value status
	void	 SetSegmentSize( float fSize, BOOL bRedraw = TRUE );// Change size of segments (size = NormalSize * iSize )
	float  	 GetSegmentSize( )	{return m_fSizeSegmentModifier;}// Return segment size modifier
	void	 SetColor( COLORREF cOn,							// Set segment color
					   COLORREF cOff = 0, COLORREF cGlow = 0);	
	COLORREF GetColorSegmentOn()  {return m_cColorSegmentOn;}  	// Return segment On color
	COLORREF GetColorSegmentOff() {return m_cColorSegmentOff;}	// Return segment Off color
	COLORREF GetColorSegmentGlow(){return m_cColorSegmentGlow;}	// Return glow effect color
	void	 SetSegmentStyle( SegmentStyle Style, BOOL bRedraw = TRUE );// Set control style
	SegmentStyle GetSegmentStyle( ) {return m_Style;}					// Return control style
	void	 SetScrollSpeed( int iSpeed );						// Set scroll speed ( 0 stop scroll)
	int		 GetScrollSpeed() { return m_iScrollSpeed;}			// Return current scroll speed
	BOOL	 LoadMatrix( UINT uiRes );							// Load X * Y matrix from bitmap resource
	BOOL	 LoadMatrix( LPCTSTR lpzFileName );					// Load X * Y matrix from bitmap file
	BOOL	 LoadMatrix( CBitmap *pBitmap );					// Load X * Y matrix from bitmap object

	// New features
CGdiBorder*  GetBorder();										// Return border object
	void	 InvalidateBorder(BOOL bRedraw = TRUE );			// Force to update the border
protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};


