/*********************************************************************/
/*     (c) Copyright IsiTrace 2008	   */
/*
	created:	13:3:2010   15:55
	filename: 	d:\dev\lib32\ExtCtlDll\include\IsiCompBase.h
	file path:	d:\dev\lib32\ExtCtlDll\include
	file base:	IsiCompBase
	file ext:	h
	author:		FPA
	
	purpose:	
*/                                                    
/*********************************************************************/

#pragma once

#include "GdiPlusWnd.h"

///////////////////////////////////////////////////////////////////////////////////
class CIsiCompBase : public CGdiPlusWnd<CWnd>
{
	//DECLARE_DYNAMIC(CIsiCompBase)

protected:
			double	m_dminValue;				// Min value
			double	m_dmaxValue;				// Max value
			double	m_dCurrentValue;			// Current value

			int		m_iNoOfDivisions;			// Number of division
			int		m_iNoOfSubDivisions;		// Number of subdivision

			int		m_iCalibrationNbDigit;		// Calibration nb digit
			int		m_iCalibrationPrecision;	// Calibration precision

			int		m_iValueNbDigit;			// Value nb digit
			int		m_iValuePrecision;			// Value precision

			BYTE	m_bShowGlass;				// Show glassy effect [0,255]

		   COLORREF m_cBorderColor;				// Border color
			int		m_iRoundBorder;				// Round border
			int		m_iBorderSize;				// Border size

			COLORREF m_cColorBkgnd;				// Background color
			COLORREF m_cColorBkgndLight;		// Background light effect color

			BOOL	m_bTransparent;				// Transparent status

protected:

public:
			 CIsiCompBase();									// Standard constructor
	virtual ~CIsiCompBase();									// Destructor

	virtual void CopyParam( CIsiCompBase *pSrc );				// Copy parameters
			void SetMinValue( double dVal, BOOL bRedraw = TRUE );	// Set minimal value
			void SetMaxValue( double dVal, BOOL bRedraw = TRUE );	// Set maximal value
	double	GetMinValue( ) const;								// Return minimal value
	double	GetMaxValue( ) const;								// Return maximal value
	virtual void	SetValue( double fVal);						// Set value
	virtual double	GetValue( ) const;							// Get value
	void	Transparent( BOOL bTransparent, BOOL bRedraw=TRUE );// Set transparent
	BOOL	IsTransparent();									// Return transparent status
	void	ShowGlass( BYTE bShow, BOOL bRedraw = TRUE );		// Show or hide glassy effect
	BYTE	GetShowGlass() const;								// Get glassy effect status
	void	SetNbDiv( int iVal, BOOL bRedraw = TRUE );			// Set number of division
	void	SetNbSubDiv( int iVal, BOOL bRedraw = TRUE );		// Set number of sub division
	int		GetNbDiv() const;									// Get number of division
	int		GetNbSubDiv() const;								// Get number of sub division
	void	SetRoundBorder( int iRound, BOOL bRedraw = TRUE );	// Set round border
	int		GetRoundBorder() const;								// Get round border
	void	SetBorderSize( int iSize, BOOL bRedraw = TRUE );	// Set border size
	int		GetBorderSize() const;								// Ge tborder size

	void	 SetBkgndColor( COLORREF c, BOOL bRedraw = TRUE );	// Set Background color
	COLORREF GetBkgndColor() const;								// Get Background color

	void	 SetBkgndLightColor( COLORREF c,BOOL bRedraw=TRUE );// Set Background color
	COLORREF GetBkgndLightColor() const;						// Get Background color

	// Precision
	void	SetCalibrationDigit( int iNbDigit, int iPrecision,  // Set calibration format
								 BOOL bRedraw = TRUE);
	int     GetCalibrationNbDigit() {return m_iCalibrationNbDigit;}	// Return calibration nb digit
	int     GetCalibrationPrecision() {return m_iCalibrationPrecision;}	// Return calibration precision

	void	SetValueNbDigit( int iNbDigit, int iPrecision,		// Set value format
						   BOOL bRedraw = TRUE);
	int     GetValueNbDigit() {return m_iValueNbDigit;}			// Return value nb digit
	int     GetValuePrecision() {return m_iValuePrecision;}		// Return value precision

virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle,
					const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);

virtual	CString GetInitString() const;							// Return initialization string
virtual	BOOL	InitFromString( CString& str );					// Init object form string

	DECLARE_MESSAGE_MAP()
};