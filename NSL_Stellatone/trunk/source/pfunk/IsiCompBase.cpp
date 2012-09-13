//
// IsiCompBase.cpp  : Version 1.0 
//
// Author:  Frédéric PAILLOUX
//          
//
// Description:
//
// History
//     Version 1.0 - 2010 December 20
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
#include "IsiCompBase.h"


// CIsiCompBase
///////////////////////////////////////////////////////////////////////////////////
//IMPLEMENT_DYNAMIC(CIsiCompBase, CGdiPlusWnd<CWnd>)

///////////////////////////////////////////////////////////////////////////////////
CIsiCompBase::CIsiCompBase()
{
	m_bReGenBackGnd = TRUE;
	m_dminValue		= 0;		
	m_dmaxValue		= 100;	
	m_dCurrentValue	= 50;
	m_bShowGlass	= 100;
	m_iRoundBorder	= 5;
	m_iBorderSize	= 1;
	m_cBorderColor  = RGB(0,0,0);
	m_iCalibrationNbDigit = 0;
	m_iCalibrationPrecision=0;
	m_iValueNbDigit = 0;
	m_iValuePrecision = 0;
	m_bTransparent = FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
CIsiCompBase::~CIsiCompBase()
{
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::CopyParam( CIsiCompBase *pSrc )
{
	m_bReGenBackGnd		= pSrc->m_bReGenBackGnd;
	m_dminValue			= pSrc->m_dminValue;
	m_dmaxValue			= pSrc->m_dmaxValue;
	m_dCurrentValue		= pSrc->m_dCurrentValue;
	m_iNoOfDivisions	= pSrc->m_iNoOfDivisions;
	m_iNoOfSubDivisions = pSrc->m_iNoOfSubDivisions;
	m_bShowGlass		= pSrc->m_bShowGlass;
	m_iCalibrationNbDigit	= pSrc->m_iCalibrationNbDigit;
	m_iCalibrationPrecision	= pSrc->m_iCalibrationPrecision;
	m_iValueNbDigit			= pSrc->m_iValueNbDigit;
	m_iValuePrecision		= pSrc->m_iValuePrecision;
	m_bTransparent = pSrc->m_bTransparent;
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::SetValue( double dVal )
{
	m_dCurrentValue = dVal;
	if ( m_hWnd )
		Invalidate();
}

///////////////////////////////////////////////////////////////////////////////////
double CIsiCompBase::GetValue() const
{
	return m_dCurrentValue; 
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::SetMinValue( double dVal, BOOL bRedraw /*= TRUE*/ )
{
	m_dminValue = dVal;
	m_bReGenBackGnd = TRUE;
	if ( bRedraw && m_hWnd )
		Invalidate();
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::SetMaxValue( double dVal, BOOL bRedraw /*= TRUE*/  )
{
	m_dmaxValue = dVal;
	m_bReGenBackGnd = TRUE;
	if ( bRedraw && m_hWnd )
		Invalidate();
}

///////////////////////////////////////////////////////////////////////////////////
double CIsiCompBase::GetMinValue( ) const
{
	return m_dminValue;
}

///////////////////////////////////////////////////////////////////////////////////
double CIsiCompBase::GetMaxValue( ) const
{
	return m_dmaxValue;
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::Transparent( BOOL bTransparent, BOOL bRedraw/*=TRUE*/ )
{
	m_bTransparent= bTransparent;
	m_bReGenBackGnd = TRUE;
	if ( bRedraw && m_hWnd )
		Invalidate();
}

///////////////////////////////////////////////////////////////////////////////////
BOOL CIsiCompBase::IsTransparent()
{
	return m_bTransparent;
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::ShowGlass( BYTE bShow, BOOL bRedraw /*= TRUE*/ )
{
	m_bShowGlass= bShow;
	if ( bRedraw && m_hWnd )
		Invalidate();
}

///////////////////////////////////////////////////////////////////////////////////
BYTE CIsiCompBase::GetShowGlass() const
{
	return m_bShowGlass;
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::SetNbDiv( int iVal, BOOL bRedraw /*= TRUE*/ )
{
	m_iNoOfDivisions= iVal;
	m_bReGenBackGnd = TRUE;
	if ( bRedraw && m_hWnd )
		Invalidate();
}
///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::SetNbSubDiv( int iVal, BOOL bRedraw /*= TRUE*/ )
{
	m_iNoOfSubDivisions= iVal;
	m_bReGenBackGnd = TRUE;
	if ( bRedraw && m_hWnd )
		Invalidate();
}
///////////////////////////////////////////////////////////////////////////////////
int CIsiCompBase::GetNbDiv() const
{
	return m_iNoOfDivisions;
}

///////////////////////////////////////////////////////////////////////////////////
int CIsiCompBase::GetNbSubDiv() const
{
	return m_iNoOfSubDivisions;
}


///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::SetRoundBorder( int iRound, BOOL bRedraw /*= TRUE*/ )
{
	m_iRoundBorder= iRound;
	m_bReGenBackGnd = TRUE;
	if ( bRedraw && m_hWnd )
		Invalidate();
}
///////////////////////////////////////////////////////////////////////////////////
int CIsiCompBase::GetRoundBorder() const
{
	return m_iRoundBorder;
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::SetBorderSize( int iSize, BOOL bRedraw /*= TRUE*/ )
{
	m_iBorderSize= iSize;
	m_bReGenBackGnd = TRUE;
	if ( bRedraw && m_hWnd )
		Invalidate();
}
///////////////////////////////////////////////////////////////////////////////////
int CIsiCompBase::GetBorderSize() const
{
	return m_iBorderSize;
}
///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::SetBkgndColor( COLORREF c, BOOL bRedraw /*= TRUE*/ )
{
	m_cColorBkgnd = c;
	m_bReGenBackGnd = TRUE;
	if ( bRedraw && m_hWnd )
		Invalidate();
}

///////////////////////////////////////////////////////////////////////////////////
COLORREF CIsiCompBase::GetBkgndColor() const
{
	return m_cColorBkgnd;
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::SetBkgndLightColor( COLORREF c,BOOL bRedraw/*=TRUE*/ )
{
	m_cColorBkgndLight = c;
	m_bReGenBackGnd = TRUE;
	if ( bRedraw && m_hWnd )
		Invalidate();
}

///////////////////////////////////////////////////////////////////////////////////
COLORREF CIsiCompBase::GetBkgndLightColor() const
{
	return m_cColorBkgndLight;
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::SetCalibrationDigit( int iNbDigit, int iPrecision, BOOL bRedraw /* = TRUE */) 
{
	m_iCalibrationNbDigit = iNbDigit;
	m_iCalibrationPrecision = iPrecision;
	if ( bRedraw && m_hWnd )
	{
		m_bReGenBackGnd = TRUE;
		Invalidate();
	}
}

///////////////////////////////////////////////////////////////////////////////////
void CIsiCompBase::SetValueNbDigit( int iNbDigit, int iPrecision, BOOL bRedraw /* = TRUE */) 
{
	m_iValueNbDigit = iNbDigit;
	m_iValuePrecision = iPrecision;
	if ( bRedraw && m_hWnd )
	{
		m_bReGenBackGnd = TRUE;
		Invalidate();
	}
}

///////////////////////////////////////////////////////////////////////////////////
CString CIsiCompBase::GetInitString() const
{
	return _T("");
}
///////////////////////////////////////////////////////////////////////////////////
BOOL CIsiCompBase::InitFromString( CString& str )
{
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
BOOL CIsiCompBase:: Create(LPCTSTR lpszText, DWORD dwStyle,
					const RECT& rect, CWnd* pParentWnd, UINT nID /*= 0xffff*/)
{
	return CGdiPlusWnd<CWnd>::Create( _T("Static"), lpszText, dwStyle, rect, pParentWnd, nID );
}

///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CIsiCompBase, CGdiPlusWnd<CWnd>)
END_MESSAGE_MAP()
