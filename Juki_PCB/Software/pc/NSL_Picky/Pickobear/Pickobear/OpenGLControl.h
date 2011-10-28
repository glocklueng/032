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

		int m_camera,iCounter;

		IplImage* img1;
		IplImage* img2;
		IplImage *resultImg;

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
		void UpdateCamera( UINT id );

		// Added message classes:
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg	void OnDraw(CDC *pDC);
		afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point );
		DECLARE_MESSAGE_MAP()
		afx_msg void OnStnDblclickCam2();
};


class COGLThread : public CWinThread
{
public:
	COpenGLControl *p_OGLCamera1;
	COpenGLControl *p_OGLCamera2;

    COGLThread();
    virtual ~COGLThread();
    virtual BOOL InitInstance() ;
    virtual int Run() ;
    CString sName ;
} ;

