#include "stdafx.h"
#include "OpenGLControl.h"
#include "pickobear.h"

COpenGLControl::COpenGLControl(void)
{
	img1 = NULL;
	m_camera = -1;
	img2 = NULL;

}


COpenGLControl::~COpenGLControl(void)
{
	if( m_camera!=-1) {
		VI.stopDevice (m_camera);
	}

    if(img1) 
		cvReleaseImage (& img1);

	if(img2) 
		cvReleaseImage (& img2);
}

BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void COpenGLControl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	ValidateRect(NULL);
}

void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

	// Map the OpenGL coordinates.
	glViewport(0, 0, cx, cy);

	// Projection view
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	// Set our current view perspective
	gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);

	// Model view
	glMatrixMode(GL_MODELVIEW);
}

int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	oglInitialize();

	return 0;
}

void COpenGLControl::OnDraw(CDC *pDC)
{
}

extern double m_Thresh1,m_Thresh2;

void COpenGLControl::OnTimer(UINT nIDEvent)
{  
	double delta;
	int ddepth;  
	int ind =1 , ni = 100;
	
	static int iCounter = 0;

	cv::Mat kernel;
	cv::Mat src, dst;
	cv::Point anchor;

	static IplImage *resultImg= NULL;

	delta = 0;
	ddepth = -1;
	anchor = cv::Point( -1, -1 );
	
	if( resultImg == NULL) {
		resultImg = cvCloneImage( img1 ); 
	}

	switch (nIDEvent)
	{
		case 1:
		{
			// fetch that data from the camera
			if (VI.isFrameNew(m_camera)){

				img1->imageData = ( char * ) VI.getPixels(m_camera, true , true ); 

				//cvAddWeighted( img1->imageData, 1.0/100., resultImg, 1, 0.0, resultImg );

				iCounter ++ ;
			}

			if ( iCounter  < 10 ) {
				return;
			}

			iCounter = 0;

			wglMakeCurrent(hdc, hrc);

			Squares( img1 , m_Thresh1, m_Thresh2, VI.getDeviceName( m_camera ) ) ;

			//cvConvertImage(img1, img2, CV_CVTIMG_FLIP);
         
#if 0 
			// Randomise buffer for testing
			CvRNG rng = cvRNG(rand());
			cvRandArr( &rng, img2, CV_RAND_UNI, cvScalar(0,0,0), cvScalar(256,256,256)); 
#endif

#if 0
			kernel_size = 3 + 2*( ind%5 );
			kernel = cv::Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
			
			filter2D(src, dst, ddepth , kernel, anchor, delta, cv::BORDER_DEFAULT );
#endif		
			//	cvFilter2D(img2,img2,kernel,anchor);

			// Create Texture
			
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img1->width, img1->height, GL_RGB, GL_UNSIGNED_BYTE, img1->imageData);


			// Clear color and depth buffer bits
			glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
			glEnable(GL_TEXTURE_2D);

			// Set Projection Matrix
			glMatrixMode (GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0, m_size.Width(), m_size.Height(), 0);

			// Switch to Model View Matrix
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			// Draw a textured quad
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f((float)m_size.Width(), 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f((float)m_size.Width(), (float)m_size.Height());
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, (float)m_size.Height());
			glEnd();

			//glFlush();

			SwapBuffers(hdc);
	

			break;
		}

		default:
			break;
	}

	CWnd::OnTimer(nIDEvent);
}

CString COpenGLControl::oglCreate(CRect rect, CRect orect, CWnd *parent, int DeviceID)
{
	CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	CreateEx(0, className, CAMCTRL_CLASSNAME, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

	// Set initial variables' values
	m_oldWindow	   = rect;
	m_originalRect = rect;
	m_size = orect;

	hWnd = parent;

	int numDevices = VI.listDevices();

	for( int i = 0 ; i < numDevices ; i++ ) {
	
		_RPT2(_CRT_WARN,"%d) Camera name is [%s]\n",i, VI.getDeviceName( i  ));

	}

	m_camera = DeviceID;

	VI.setupDevice(m_camera); 

	_RPT1(_CRT_WARN,"Camera name is [%s]\n",VI.getDeviceName(m_camera));

	img1 = cvCreateImage(cvSize(VI.getWidth(m_camera),VI.getHeight(m_camera) ),IPL_DEPTH_8U,3);
	assert(img1);

	img2 = cvCreateImage(cvSize(VI.getWidth(m_camera),VI.getHeight(m_camera) ),IPL_DEPTH_8U,3);
	assert(img2);

}

void COpenGLControl::oglInitialize(void)
{
	// Initial Setup:
	//
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16, // z-buffer depth
		0, 0, 0, 0, 0, 0, 0,
	};

	// Get device context only once.
	hdc = GetDC()->m_hDC;
	
	// Pixel format.
	m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, m_nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	// Basic Setup:
	//
	// Set color to use when clearing the background.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	// Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	
	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Send draw request
	OnDraw(NULL);
}