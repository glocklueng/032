#pragma once
#include "afxwin.h"
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"opengl32.lib")

#include <gl/gl.h>
#include <gl/glu.h>

class COpenGLControl : public CWnd
{
	public:
		/******************/
		/* Public Members */
		/******************/
		UINT_PTR m_unpTimer;

		int m_camera;

		IplImage* img1;
		IplImage* img2;


	private:
		/*******************/
		/* Private Members */
		/*******************/
		// Window information
		CWnd  *hWnd;
		HDC   hdc;			
		HGLRC hrc;			
		int   m_nPixelFormat;
		CRect m_rect;
		CRect m_oldWindow;
		CRect m_originalRect;
		CRect m_size;

	public:
		COpenGLControl(void);
		virtual ~COpenGLControl(void);

		CString oglCreate(CRect rect, CRect rect1,CWnd *parent,int camera=0);
		void oglInitialize(void);

		// Added message classes:
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg	void OnDraw(CDC *pDC);
		afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnTimer(UINT nIDEvent);

		DECLARE_MESSAGE_MAP()
};