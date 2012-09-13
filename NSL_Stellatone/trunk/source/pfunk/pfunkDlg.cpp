
// pfunkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pfunk.h"
#include "pfunkDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CpfunkDlg dialog




CpfunkDlg::CpfunkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CpfunkDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpfunkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RENDER_WINDOW, m_LCDPanelA);
	DDX_Control(pDX, IDC_RENDER_WINDOW2, m_LCDPanelB);
	DDX_Control(pDX, IDC_RENDER_WINDOW3, m_LCDPanelC);
	DDX_Control(pDX, IDC_RENDER_WINDOW4, m_LCDPanelD);
}

BEGIN_MESSAGE_MAP(CpfunkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CpfunkDlg message handlers

extern void __cdecl  setup();
extern void __cdecl loop();

BOOL CpfunkDlg::OnInitDialog()
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


	m_LCDPanelA.StringMode(TRUE, TRUE);
	//m_LCDPanelA.LoadMatrix( IDB_MATRIX5x7 );  
	m_LCDPanelA.SetSegmentStyle(Segment16 , 1);
	m_LCDPanelA.SetSegmentSize(.8f,1);
	m_LCDPanelA.SetValueNbDigit( 20,0 ); 
	m_LCDPanelA.GetBorder()->SetStyle(CGdiBorder::Chrome);
	m_LCDPanelA.SetWindowText( ("PfUNK SOFTWARE SYNTH"));
	m_LCDPanelA.SetScrollSpeed(150);
	m_LCDPanelA.ShowGlass(80, 1 );

	m_LCDPanelB.StringMode(TRUE, TRUE);
	m_LCDPanelB.LoadMatrix( IDB_MATRIX5x7 );  
	m_LCDPanelB.SetSegmentStyle(MatrixSquare  , 1);
	m_LCDPanelB.SetSegmentSize(.9f,1);
	m_LCDPanelB.SetValueNbDigit( 20,0 ); 
	m_LCDPanelB.GetBorder()->SetStyle(CGdiBorder::Chrome);
	m_LCDPanelB.SetWindowText( (" [ NULLSPACE LABS ]"));
	m_LCDPanelB.SetColor( RGB(0,150,0) );
	m_LCDPanelB.ShowGlass(80, 1 );

	m_LCDPanelC.StringMode(TRUE, TRUE);
	//m_LCDPanelC.LoadMatrix( IDB_MATRIX5x7 );  
	m_LCDPanelC.SetSegmentStyle(Segment16 , 1);
	m_LCDPanelC.SetSegmentSize(1.f,1);
	m_LCDPanelC.SetValueNbDigit( 20,0 ); 
	m_LCDPanelC.GetBorder()->SetStyle(CGdiBorder::Raised);
	m_LCDPanelC.SetWindowText( ("BOOTING"));
	m_LCDPanelC.SetColor( RGB(0,0,255) );
	m_LCDPanelC.SetBlink( 100 );
	m_LCDPanelC.ShowGlass(80, 1 );

	m_LCDPanelD.StringMode(TRUE, TRUE);
	//m_LCDPanelD.LoadMatrix( IDB_MATRIX5x7 );  
	m_LCDPanelD.SetSegmentStyle(Segment16 , 1);
	m_LCDPanelD.SetSegmentSize(1.f,1);
	m_LCDPanelD.SetValueNbDigit( 20,0 ); 
	m_LCDPanelD.GetBorder()->SetStyle(CGdiBorder::Chrome);
	m_LCDPanelD.SetWindowText( ("BEEP BLOP BLIP"));
	m_LCDPanelD.SetColor( RGB(255,0,255) );
//	m_LCDPanelD.SetBlink( 100 );
	m_LCDPanelD.ShowGlass(80, 1 );

		// TODO: Add extra initialization here
	setup();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CpfunkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CpfunkDlg::OnPaint()
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
HCURSOR CpfunkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Do the black background
BOOL CpfunkDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);
    CBrush myBrush(RGB(0, 0, 0));
    CBrush *pOld = pDC->SelectObject(&myBrush);
    BOOL bRes  = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOld);   
    return bRes;                       
}




#include "..\src\fmdisplay.h"

extern FMDisplay          fmDisplay;
extern LiquidCrystal      lcd;

void LCDPrint(int row,int col, const char*text)
{
	if ( theApp.pDlg  == NULL )
		return;

	row = lcd.y;
	col = lcd.x;
	
	switch( row ) {
		case 0:
			theApp.pDlg->m_LCDPanelA.SetWindowText( text );
			break;
		case 1:
			theApp.pDlg->m_LCDPanelB.SetWindowText( text );
			break;
		case 2:
			theApp.pDlg->m_LCDPanelC.SetWindowText( text );
			break;
		case 3:
			theApp.pDlg->m_LCDPanelD.SetWindowText( text );
			break;
	}
}
