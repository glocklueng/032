
// NSL_BT_BlinkyBallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NSL_BT_BlinkyBall.h"
#include "NSL_BT_BlinkyBallDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSerial m_Serial;
#define NUM_TLC5947 ( 16)
#include <math.h>

unsigned char LEDChannels[24*NUM_TLC5947];

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNSL_BT_BlinkyBallDlg dialog




CNSL_BT_BlinkyBallDlg::CNSL_BT_BlinkyBallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNSL_BT_BlinkyBallDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNSL_BT_BlinkyBallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNSL_BT_BlinkyBallDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, &CNSL_BT_BlinkyBallDlg::OnBnClickedSend)
END_MESSAGE_MAP()


// CNSL_BT_BlinkyBallDlg message handlers

BOOL CNSL_BT_BlinkyBallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_Serial.Open(L"\\\\.\\COM34");
	
	m_Serial.Setup(CSerial::EBaud57600 );
	
	memset(&LEDChannels[0],0,sizeof(LEDChannels));

	m_Serial.Write(&LEDChannels[0],sizeof(LEDChannels));
	Sleep(200);
	m_Serial.Write(&LEDChannels[0],sizeof(LEDChannels));
	Sleep(200);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNSL_BT_BlinkyBallDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNSL_BT_BlinkyBallDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNSL_BT_BlinkyBallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void LEDscan2(int red, float degreeoffset,unsigned int count) 
{

  double brightnessfactor = 0.0;
  
  double scanindex = (1.0 + sin(degreeoffset*3.14159/180.0)) * ((double)(NUM_TLC5947 * 24 ) / 2.0);
    for(unsigned int LEDindex = 0; LEDindex < (count  * 24); LEDindex++) {
      

      brightnessfactor = exp(0.0 - fabs(scanindex - ((double)LEDindex + 0.5)) * 1.3);

      LEDChannels[LEDindex] = ( red * brightnessfactor);
    }
}

void LEDscan3(int red, float degreeoffset,unsigned int count) 
{
  double brightnessfactor = 0.0f;
  
  double scanindex = (1.0 + sin(degreeoffset*3.14159/180.0)) * ((double)(NUM_TLC5947 * 24 ) / 2.0);
  
    for(unsigned int LEDindex = 0; LEDindex < (count  * 24); LEDindex++) {
      

      brightnessfactor = exp(0.0 - fabs(scanindex - ((double)LEDindex + 0.5)) * 1.3);

      LEDChannels[LEDindex] = ( (4096-(red * brightnessfactor)));
    }
    

}
void LEDscan(int red, double degreeoffset,unsigned int count) 
{

  double brightnessfactor = 0.0f;
  
  double scanindex = (1.0 + sin(degreeoffset*3.14159/180.0)) * ((double)(NUM_TLC5947 * 24 ) / 2.0);
  
    for(unsigned int LEDindex = 0; LEDindex < (NUM_TLC5947  * 24); LEDindex++) {
      

      brightnessfactor = exp(0.0 - fabs(scanindex - ((double)LEDindex + 0.5)) * 1.3);

      LEDChannels[LEDindex] = ((red * brightnessfactor));
    }
    
}
void WriteLEDArray(unsigned int count) 
{
  if( count )
	  m_Serial.Write(&LEDChannels[0],(count*24)*sizeof(LEDChannels[0]));
}

void CNSL_BT_BlinkyBallDlg::OnBnClickedSend()
{

	for (int x = 0; x < 6; x++) 
	{
#if 1

	for (int i = 0; i < 255; i+=8) 
	{		

		for(unsigned int j = 0 ;  j < (NUM_TLC5947*24); j ++ ) {
		
			LEDChannels[ j ]  = 0;
			
		}

		m_Serial.Write(&LEDChannels[0],(NUM_TLC5947*24));

		Sleep( 100 );
	}
#endif

#if 0
	for (int i = 0; i< 255; i+=8) {		

		for(unsigned int j = 0 ;  j < (NUM_TLC5947*24); j ++ ) {
		
			LEDChannels[ j ]  = 255-i;
		}

		m_Serial.Write(&LEDChannels[0],(NUM_TLC5947*24));
				
		Sleep( 100 );
	}

#endif
	}

#if 0

	for (int i = 0; i< 4096; i++) {		

		for(unsigned int j = 0 ;  j < (NUM_TLC5947*24); j ++ ) {
		
			LEDChannels[ j ]  = i;
		}

		m_Serial.Write(&LEDChannels[0],sizeof(LEDChannels));
		Sleep(100);
	}
#endif

#if 0
	for (double offset = 240; offset < 310; offset += 1) {		
		LEDscan(4096, offset,1);
		m_Serial.Write(&LEDChannels[0],sizeof(LEDChannels));
	}
#endif

#if 0
	for(int i =1 ; i < 5 ;i++)
		for (double offset = 1; offset < 360; offset += 1) {

			LEDscan2(4096, offset,i/2);

			WriteLEDArray(i/2);  
		
		}
#endif


}
