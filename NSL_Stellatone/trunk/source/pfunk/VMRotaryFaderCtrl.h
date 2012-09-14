/**
* @class		CVMRotaryFaderCtrl
* --------------------------------------------------------------------------------------------------------
* @system		VMMFC
* @desc			Extended MFC CSliderCtrl class
*				This is part of the VMMFC SampleCode
* @location		http://www.visiomedia.com/rooms/labor/src/vmmfc/index.htm
* @version		0170
* @author		Frank Luchs
* --------------------------------------------------------------------------------------------------------
* @history		1999-05-01	first draft
*				2000-01-21	extended keyboard control
*				2002-08-01	extended to 3 bitmaps (scale,knob,dot)
* --------------------------------------------------------------------------------------------------------
* @copyright	© 1985-2001 Visiomedia Software Corporation, All Rights Reserved.
* --------------------------------------------------------------------------------------------------------
*/

#ifndef _CVMRotaryFaderCtrl_
#define _CVMRotaryFaderCtrl_

#include "VMBitmap.h"



class CVMRotaryFaderCtrl : public CSliderCtrl
{

public:
	
	enum ePart
	{
		Scale,
		Knob,
		Dot,

		PartCount

	};

	void			SetBitmap(HBITMAP hbmp, int nWhich);
	HBITMAP			GetBitmap( int nWhich);

	void			Paint();

	virtual void	SetKnobRadius(int nKnobRadius);
	virtual int		GetKnobRadius() const;

	virtual void	SetZero(int nZero);
	virtual int		GetZero() const;


	CVMRotaryFaderCtrl();
	virtual ~CVMRotaryFaderCtrl();

	//{{AFX_VIRTUAL(CVMRotaryFaderCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	virtual bool SetKnobPos(int nPos);
	virtual bool UpdateKnobPos(int nDeltaPos);

	//{{AFX_MSG(CVMRotaryFaderCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	int		m_nZero; // not yet implemented
	int		m_nKnobRadius;

	bool	m_bDragging;

	CVMBitmap	m_Bitmap[PartCount];
	POINT		m_ptDot;
	POINT		m_ptKnob;
	POINT		m_ptCenter;
	POINT		m_ptHit;
};


#endif // _CVMRotaryFaderCtrl_


